#include "stdafx.h"
#include "CJobManagerNew.h"

// Include the Window's API around Thread Pools and then Wrapper Classes
#include <threadpoolapiset.h>
#include "ThreadPoolWrappers.h"

/////////////////////////////////////////////////////////////////////////////
// CJobManagerNew Implementation

// Pattern Singleton main instances
CJobManagerNew* CJobManagerNew::m_pInstance = nullptr;
CJobManagerNew* CJobManagerNew::m_pSyncInstance = nullptr;
CJobManagerNew* CJobManagerNew::m_pHistoryInstance = nullptr;

CJobManagerNew::CJobManagerNew() : m_bIsSync(false), m_bReadyForJobs(false), 
	m_nMinThreads(1), m_nMaxThreads(1), m_bTerminating(false), m_tpsi({0})
{
	// Make sure a Job can execute using COM
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
}

CJobManagerNew::CJobManagerNew(int nMinThreads, int nMaxThreads) : m_bIsSync(false),  m_bReadyForJobs(false),
	m_nMinThreads(nMinThreads), m_nMaxThreads(nMaxThreads), m_bTerminating(false),  m_tpsi({ 0 })
{
}

CJobManagerNew::~CJobManagerNew()
{
}

void CJobManagerNew::SetNonCritical(LPTHREADEDJOB job)
{
	job->dwFlags = job->dwFlags | JMJOBFLAG_NONCRITICAL;

}

void CJobManagerNew::SetHighPriority(LPTHREADEDJOB job)
{
	job->dwFlags = job->dwFlags | JMJOBFLAG_HIGHPRIORITY;
}

void CJobManagerNew::SetPriorityLowest(LPTHREADEDJOB job)
{
	job->dwFlags = job->dwFlags | JMJOBFLAG_PRIORITYLOWEST;
}

BOOL CJobManagerNew::ForceSynchronous(LPTHREADEDJOB job)
{
	if (!(job->dwFlags & JMJOBFLAG_SYNCHRONOUS)) {
		job->dwFlags = job->dwFlags | JMJOBFLAG_RESTARTSYNCHRONOUS;
		return FALSE;
	}
	else return TRUE;
}

// Add the job to the list and kick off a work item for it
BOOL CJobManagerNew::AddJob(LPTHREADEDJOB pJob)
{
	if (m_bTerminating) return FALSE;

	if (m_bReadyForJobs)
	{
		m_JobSync.Lock();
		ThreadPoolWrappers::CWork currentWork;

		if (m_bIsSync)
		{
			// Queue the job
			auto itrB = m_JobList.begin();
			m_JobList.insert(itrB, *pJob);
		}
		else
		{
			// Not Synchronize, so just add to back (FIFO)
			m_JobList.push_back(*pJob);
		}
		bool Success = SubmitWork(this->ExecuteJob, this, currentWork);
		m_JobSync.Unlock();
		return Success;
	}
	else
	{
		return FALSE;
	}
}

// Allows stack size to be changed for a pool
void CJobManagerNew::AssignStackSizes(size_t Reserve, size_t Commit)
{
	m_Pool.QueryStackInformation(&m_tpsi);
	m_tpsi.StackCommit = Commit;
	m_tpsi.StackReserve = Reserve;

	m_Pool.SetStackInformation(&m_tpsi);
}

// Change entire pool Priority
void CJobManagerNew::ChangePriority(TP_CALLBACK_PRIORITY Priority)
{
	m_Environment.SetPriority(Priority);
}

// Inform pool long jobs will be done
void CJobManagerNew::PerformLongJobs()
{
	m_Environment.SetCallbackRunsLong();
}

// Check if there is room available for adding a Job (Historical management)
BOOL CJobManagerNew::SlotAvailable()
{
	// New model allows management by the api
	return TRUE;
}

// Indicator that a specific Job Manager has no more jobs
BOOL CJobManagerNew::IsCompleted()
{
	CSingleLock	lockjobs(&m_JobSync, TRUE);
	return (m_JobList.empty());
}

// Indicator of how may Jobs still needs to run
int CJobManagerNew::JobsRemaining()
{
	CSingleLock	lockjobs(&m_JobSync, TRUE);
	return static_cast<int>(m_JobList.size());
}

// Allows a break for some execution
void CJobManagerNew::Wait(DWORD dwMillisec, BOOL bGUIprocessing)
{
	::Sleep(dwMillisec);
	if (bGUIprocessing) ProcessMessages();
}

// Internal startup function for a specific Job Manager
bool CJobManagerNew::Startup(bool AttachToLib)
{
	if (AttachToLib)
	{
		m_bReadyForJobs = CreateNew(GetModuleHandle(nullptr), m_nMinThreads, m_nMaxThreads, CleanupCallback);
	}
	else
	{
		m_bReadyForJobs = CreateNew(nullptr, m_nMinThreads, m_nMaxThreads, CleanupCallback);
	}
	m_Pool.QueryStackInformation(&m_tpsi);
	return m_bReadyForJobs;
}

// General function to get pointer to the custom Job Manager (Stack)
CJobManagerNew* CJobManagerNew::GetJobManager(BOOL bNewManager, int nThreads, bool AttachToLib)
{
	if (m_pInstance && bNewManager)
	{
		// Have to recreate, so terminate current instance
		delete m_pInstance;
		m_pInstance = nullptr;
	}

	if (!m_pInstance)
	{
		m_pInstance = new CJobManagerNew(1, nThreads);
		if (!m_pInstance->Startup(AttachToLib))
			return nullptr;
	}
	return m_pInstance;
}

// Return the Job Manager specificly used for synchronize jobs (Queue)
CJobManagerNew* CJobManagerNew::GetSyncJobManager(int nThreads)
{
	if (!m_pSyncInstance)
	{
		m_pSyncInstance = new CJobManagerNew(1, nThreads);
		if (!m_pSyncInstance->Startup(false))
		{
			return nullptr;
		}
		m_pSyncInstance->m_bIsSync = true;
	}
	return m_pSyncInstance;
}

// Return the Job Manager for History comparisons
CJobManagerNew* CJobManagerNew::GetHistoryJobManager()
{
	if (!m_pHistoryInstance)
	{
		m_pHistoryInstance = new CJobManagerNew(1, 2);
		if (!m_pHistoryInstance->Startup(false))
		{
			return nullptr;
		}
	}
	return m_pHistoryInstance;
}

// Terminate all job Managers if they are running, remember to do a shut down first
void CJobManagerNew::Terminate()
{
	if (m_pInstance)
	{
		while (m_pInstance->JobsRemaining() > 0)
		{
			Sleep(10);
		}
		delete m_pInstance;
	}
	m_pInstance = nullptr;
	if (m_pSyncInstance)
	{
		while (m_pSyncInstance->JobsRemaining() > 0)
		{
			Sleep(10);
		}
		delete m_pSyncInstance;
	}
	m_pSyncInstance = nullptr;
	if (m_pHistoryInstance)
	{
		while (m_pHistoryInstance->JobsRemaining() > 0)
		{
			Sleep(10);
		}
		delete m_pHistoryInstance;
	}
	m_pHistoryInstance = nullptr;
}

void CJobManagerNew::ProcessMessages()
{
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp);

	MSG msg = { 0 };
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		pApp->PumpMessage();
}

void CJobManagerNew::StartActiveJob()
{
	InterlockedIncrement(&m_nActiveJobs);
}

void CJobManagerNew::EndActiveJob()
{
	InterlockedDecrement(&m_nActiveJobs);
}

// This will be called when a cleanup cancels (Not normally happpening)
VOID NTAPI CJobManagerNew::CleanupCallback(_Inout_opt_ PVOID ObjectContext, _Inout_opt_ PVOID CleanupContext)
{
	UNREFERENCED_PARAMETER(ObjectContext);
	UNREFERENCED_PARAMETER(CleanupContext);
}

// Since JobManager (original) was based on specific thread start messages, this callback redirects to match that
VOID NTAPI CJobManagerNew::ExecuteJob(_Inout_ PTP_CALLBACK_INSTANCE Instance, _Inout_opt_ PVOID Context, _Inout_ PTP_WORK Work)
{
	// Using Work to indicate when the Work slot is done
	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(Work);
	if (Context)	// Context would be the CJobManagerNew always
	{
		auto pManager = (CJobManagerNew*)Context;
		if (pManager)
		{
			// Make sure a Job can execute using COM
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

			CSingleLock	lockjobs(&pManager->m_JobSync, TRUE);
			// Pop the next Job to do and execute that job
			THREADEDJOB aJobItem = pManager->m_JobList.back();
			pManager->m_JobList.pop_back();
			pManager->StartActiveJob();

			if (aJobItem.pFunction)
			{
				try
				{
					aJobItem.pFunction(&aJobItem);
				}
				catch (...)
				{

				}
			}

			pManager->EndActiveJob();
			// Remove COM usage (paired with the Co above)
			CoUninitialize();
		}
	}
}

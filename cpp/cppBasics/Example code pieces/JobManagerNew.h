#pragma once

#ifndef __C_JOBMANAGER_H__
#define __C_JOBMANAGER_H__

#include "ThreadPoolWrappers.h"
#include <vector>

const DWORD	JMJOBFLAG_NONE = 0x00000000;
const DWORD	JMJOBFLAG_NONCRITICAL = 0x00000001;
const DWORD	JMJOBFLAG_SYNCHRONOUS = 0x00000002;
const DWORD	JMJOBFLAG_RESTARTSYNCHRONOUS = 0x00000004;
const DWORD	JMJOBFLAG_HIGHPRIORITY = 0x00000008;
const DWORD	JMJOBFLAG_PRIORITYLOWEST = 0x00000010;

typedef BOOL(WINAPI* PJOBMANAGER_FEEDBACK)(LPVOID lpFeedback);
// Type defined a structure for job input
typedef struct tagThreadedJob
{
	tagThreadedJob() : pFunction(NULL), pfnFeedback(NULL), hWndParam(NULL), lParam1(NULL), lParam2(NULL), lParam3(NULL), lParam4(NULL), lParam5(NULL), lParam6(NULL), dwFlags(0)
	{
		//gParam = {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	}

	explicit tagThreadedJob(LPTHREAD_START_ROUTINE pfn, LPARAM p1 = NULL, LPARAM p2 = NULL, LPARAM p3 = NULL, LPARAM p4 = NULL, LPARAM p5 = NULL, LPARAM p6 = NULL) :
		pFunction(pfn), pfnFeedback(NULL), hWndParam(NULL), lParam1(p1), lParam2(p2), lParam3(p3), lParam4(p4), lParam5(p5), lParam6(p6), dwFlags(0)
	{
		//gParam = {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
	}

	BOOL Feedback(LPVOID p)
	{
		if (pfnFeedback)
			return pfnFeedback(p);

		return TRUE;
	}

	LPTHREAD_START_ROUTINE	pFunction;
	PJOBMANAGER_FEEDBACK	pfnFeedback;
	HWND					hWndParam;
	LPARAM					lParam1;
	LPARAM					lParam2;
	LPARAM					lParam3;
	LPARAM					lParam4;
	LPARAM					lParam5;
	LPARAM					lParam6;
	GUID					gParam1;
	GUID					gParam2;
	DWORD					dwFlags;
} THREADEDJOB, *LPTHREADEDJOB;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// The new Job Manager class, making use of Windows's Thread Pool support via Wrapper Classes for the
// API
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class CJobManagerNew : private ThreadPoolWrappers::CThreadPool
{
	friend class CCWThreadedJob;

public:
	CJobManagerNew();
	CJobManagerNew(int nMinThreads = -1, int nMaxThreads = -1);
	~CJobManagerNew();

	// Static Functions to retrieve specific job managers
	static CJobManagerNew* GetJobManager(BOOL bNewManager = FALSE, int nThreads = -1, bool AttachToLib = false);	// get a general job manager
	static CJobManagerNew* GetSyncJobManager(int nThreads = 1);		// get linear job manager
	static CJobManagerNew* GetHistoryJobManager();		// get history redlinning job manager
	static void Terminate();	// Terminate all job managers

	// Main Functions
	BOOL AddJob(LPTHREADEDJOB pJob);
	void ChangePriority(TP_CALLBACK_PRIORITY Priority);
	void PerformLongJobs();
	void AssignStackSizes(size_t Reserve, size_t Commit);
	BOOL IsCompleted();
	BOOL SlotAvailable();
	int  JobsRemaining();
	void Wait(DWORD dwMillisec, BOOL bGUIprocessing);
	void StartActiveJob();
	void EndActiveJob();


	// Static Functions for a job(s), via the manager
	static void SetNonCritical(LPTHREADEDJOB job);
	static void SetHighPriority(LPTHREADEDJOB job);
	static void SetPriorityLowest(LPTHREADEDJOB job);
	static BOOL ForceSynchronous(LPTHREADEDJOB job);

protected:
	virtual bool Startup(bool AttachToLib);

	TP_POOL_STACK_INFORMATION	m_tpsi;
	bool						m_bReadyForJobs;
	bool						m_bTerminating;
	bool						m_bIsSync;
	int							m_nMinThreads;
	int							m_nMaxThreads;

private:
	// Singleton pattern so that there will only be one of each
	static CJobManagerNew *m_pInstance;
	static CJobManagerNew *m_pSyncInstance;
	static CJobManagerNew *m_pHistoryInstance;

	// Callback functions
	static VOID NTAPI CleanupCallback(_Inout_opt_ PVOID ObjectContext, _Inout_opt_ PVOID CleanupContext);
	static VOID NTAPI ExecuteJob(_Inout_ PTP_CALLBACK_INSTANCE Instance, _Inout_opt_ PVOID Context, _Inout_ PTP_WORK Work);

	// Internal variables and Functions
	void ProcessMessages();
	mutable  CCriticalSection	m_JobSync;		// job level synchronization, locks just the job queue
	volatile LONG				m_nActiveJobs;
	std::vector<THREADEDJOB> m_JobList;
};

#endif


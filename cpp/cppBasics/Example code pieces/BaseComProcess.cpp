#include "StdAfx.h"
#include "BaseComProcess.h"

///////////////////////////////////////////////////////////////////////////////////
// Implementation Notes
//
// The Mutex, Events etc. each will try and generate a unique name descriptive name
// for the implementation. So in the code there is functions to generate these names
// also there is "#ifdef IS_PROXY" to indicate if a proxy is in use, which should be 
// added to the proxy project predefined. This will let the code now which kind of  
// event waiting to do, one supporting a Windows pump (MsgWaitForMultipleObjects)
// or one that does not (WaitForMultipleObjects)

// Also note the implementation was done with the idea that the memory shared should
// rather be double with a specific combination to flow to the server from
// the client and vice versa, so that is way there is 2 shared memory objects
// this also makes the logic easier to understand around flow. Also the reading, writing
// to shared memory try to use only copy commands to increase speed.

// Note that the events is placed inside an array so that counters can be used to determine
// what event woke up the thread and process it that way WAIT_OBJECT_0 presents the first event

//////////////////////////////////////////////////////////////////////////////////
// CIPCSharedTransfer

CIPCSharedTransfer::CIPCSharedTransfer()
{
	m_sSharedName = _T("");
	m_hSharedMutex = NULL;
	m_hReadReadyEvent = NULL;
	m_hSharedMapFile = NULL;
	m_pBuffer = NULL;
	m_isGlobal = false;
	m_bIsServer = false;
	m_dFileSize = 4096; // standard size
}

CIPCSharedTransfer::~CIPCSharedTransfer()
{
	ResetTransfer();
}

void CIPCSharedTransfer::ResetTransfer()
{
	if (m_pBuffer != NULL)
	{
		UnmapViewOfFile(m_pBuffer);
	}

	if (m_hSharedMapFile != NULL)
		CloseHandle(m_hSharedMapFile);
	if (m_hReadReadyEvent != NULL)
		CloseHandle(m_hReadReadyEvent);
	if (m_hSharedMutex != NULL)
		CloseHandle(m_hSharedMutex);

	m_hSharedMapFile = NULL;
	m_hReadReadyEvent = NULL;
	m_hSharedMutex = NULL;
	m_pBuffer = NULL;
}

CString CIPCSharedTransfer::GetFileNameFull()
{
	CString result;
	if (m_isGlobal)
	{
		result =  _T("Global\\");
	}
	else	
	{
		result =  _T("Local\\");
	}
	result += m_sSharedName;
	result += _T("_FILE");
	return result;
}

CString CIPCSharedTransfer::GetShareHandleNameFull(bool Writer)
{
	CString result;
	result = m_sSharedName;
	result += Writer ? _T("_EVENT") : _T("_MUTEX");
	result += Writer ? _T("_WRITE") : _T("_ACCESS");
	return result;
}

// Check if Shared Mutex controlling access to Shared File is available
bool CIPCSharedTransfer::WaitForAccess()
{
	DWORD dwWaitReady = 0;
	HANDLE WaitObjects[] = { m_hSharedMutex };

#ifdef IS_PROXY
	dwWaitReady = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);
#else
	dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE, QS_ALLINPUT);
#endif
	while (dwWaitReady == WAIT_OBJECT_0 + _countof(WaitObjects))
	{
#ifndef IS_PROXY
		MSG msg = { 0 };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				PostQuitMessage(static_cast<int>(msg.wParam));
				return false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#endif
#ifdef IS_PROXY
		dwWaitReady = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);
#else
		dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE, QS_ALLINPUT);
#endif
	}

	return (dwWaitReady == WAIT_OBJECT_0);
}

// Check if Mutex has been created, if it has write process may execute
bool CIPCSharedTransfer::WaitForReading()
{
	DWORD dwWaitReady = 0;
	HANDLE WaitObjects[] = { m_hReadReadyEvent };

#ifdef IS_PROXY
	dwWaitReady = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);
#else
	dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE, QS_ALLINPUT);
#endif

	while (dwWaitReady == WAIT_OBJECT_0 + _countof(WaitObjects))
	{
#ifndef IS_PROXY
		MSG msg = { 0 };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				PostQuitMessage(static_cast<int>(msg.wParam));
				return false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#endif
#ifdef IS_PROXY
		dwWaitReady = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);
#else
		dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE, QS_ALLINPUT);
#endif
	}

	return (dwWaitReady == WAIT_OBJECT_0);
}

bool CIPCSharedTransfer::Initialize(bool IsServer, bool IsGlobal, CString SharedName, DWORD Size)
{
	m_bIsServer = IsServer;
	m_isGlobal = IsGlobal;
	m_sSharedName = SharedName;
	m_dFileSize = Size;

	if (m_bIsServer)
	{
		m_hSharedMutex = CreateMutex( 
			NULL,								// default security descriptor
			FALSE,								// mutex not owned
			GetShareHandleNameFull(false));		// object name

		if (m_hSharedMutex == NULL)
		{
			return false;
		}

		m_hReadReadyEvent = CreateEvent(NULL, TRUE, FALSE,	GetShareHandleNameFull(true));	// object name

		if (m_hReadReadyEvent == NULL)
		{
			return false;
		}

		m_hSharedMapFile = CreateFileMapping(
						 INVALID_HANDLE_VALUE,    // use paging file
						 NULL,                    // default security
						 PAGE_READWRITE,          // read/write access
						 0,                       // maximum object size (high-order DWORD)
						 m_dFileSize,             // maximum object size (low-order DWORD)
						 GetFileNameFull());      // name of mapping object

		if (m_hSharedMapFile == NULL)
		{
			return false;
		}

	}
	else
	{
		// Client side will open the objects for use
		m_hSharedMutex = OpenMutex( 
			MUTEX_ALL_ACCESS,           // request full access
			FALSE,                      // handle not inheritable
			GetShareHandleNameFull(false));	// object name

		if (m_hSharedMutex == NULL)
		{
			return false;
		}

		m_hReadReadyEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, GetShareHandleNameFull(true));

		if (m_hReadReadyEvent == NULL)
		{
			return false;
		}
		
		m_hSharedMapFile = OpenFileMapping(
						   FILE_MAP_ALL_ACCESS,   // read/write access
						   FALSE,                 // do not inherit the name
						   GetFileNameFull());     // name of mapping object

		if (m_hSharedMapFile == NULL)
		{
			return false;
		}
	}

	m_pBuffer = MapViewOfFile(m_hSharedMapFile,		// handle to map object
					FILE_MAP_ALL_ACCESS,				// read/write permission
					0,									// No offset memory does not move
					0,									// No offset memory with Position to write
					0);									// Maximum size
	
	if (m_pBuffer == NULL)
		return false;

	return true;
}

DWORD CIPCSharedTransfer::push(CString input)
{
	DWORD result = 0;

	WaitForAccess();

	memset(m_pBuffer, 0, m_dFileSize);

	if (m_pBuffer != NULL)
	{
		CopyMemory(m_pBuffer,  input.GetBuffer(), input.GetLength() * sizeof(TCHAR));
		result = input.GetLength();
	}

	SetEvent(m_hReadReadyEvent);
	ReleaseMutex(m_hSharedMutex);
	
	return result;
}

CString CIPCSharedTransfer::pop(DWORD Size)
{
	TCHAR* pRBuf = new TCHAR[Size + 1];
	CString result;

	memset(pRBuf, 0, (Size + 1) * sizeof(TCHAR));

	WaitForReading();
	WaitForAccess();

	if (m_pBuffer != NULL)
	{
		CopyMemory((PVOID)pRBuf, m_pBuffer, Size);
		result.Format(_T("%s"), pRBuf);
	}

	delete[] pRBuf;

	ReleaseMutex(m_hSharedMutex); 
	ResetEvent(m_hReadReadyEvent);

	return result;
}

DWORD CIPCSharedTransfer::push_byte(BYTE* input, DWORD Size)
{
	DWORD result = 0;

	WaitForAccess();

	memset(m_pBuffer, 0, m_dFileSize);

	if (m_pBuffer != NULL)
	{
		CopyMemory((m_pBuffer), input, Size);
		result = Size;
	}

	SetEvent(m_hReadReadyEvent);
	ReleaseMutex(m_hSharedMutex);
	
	return result;
}

DWORD CIPCSharedTransfer::pop_byte(BYTE* output, DWORD Size)
{
	WaitForReading();
	WaitForAccess();

	if (m_pBuffer != NULL)
	{
		CopyMemory((PVOID)output, m_pBuffer, Size);
	}

	ReleaseMutex(m_hSharedMutex); 
	ResetEvent(m_hReadReadyEvent);

	return sizeof(output);
}

void* CIPCSharedTransfer::lock_buffer_write()
{
	DWORD result = 0;

	WaitForAccess();

	return m_pBuffer;
}

void* CIPCSharedTransfer::lock_buffer_read()
{
	WaitForReading();
	WaitForAccess();
	
	return m_pBuffer;
}

void CIPCSharedTransfer::unlock_buffer_write(void* pBuf)
{
	SetEvent(m_hReadReadyEvent);
	ReleaseMutex(m_hSharedMutex);
}

void CIPCSharedTransfer::unlock_buffer_read(void* pBuf)
{
	ReleaseMutex(m_hSharedMutex); 
	ResetEvent(m_hReadReadyEvent);
}

void CIPCSharedTransfer::Release()
{
	ResetTransfer();
}

//////////////////////////////////////////////////////////////////////////////////
// CBaseComProcess

CBaseComProcess::CBaseComProcess(bool IsServer, CString Name, DWORD Size)
{
	if(Size < 4096)
		Size = 4096;	// Min Size for buffers

	m_bIsServer = IsServer;
	m_sSharedName = Name;
	m_dSharedBufferSize = Size;
	m_bDoneTriggered = false;
	m_bCancelTriggered = false;

	m_hEventCancel = NULL;
	m_hEventCancelACK = NULL;
	m_hEventDone = NULL;
	m_hEventDoneACK = NULL;
	m_hServerReady = NULL;
	m_hClientReady = NULL;
}

CBaseComProcess::~CBaseComProcess(void)
{
	ResetCom();
}

void CBaseComProcess::ResetCom()
{
	m_bDoneTriggered = false;
	m_bCancelTriggered = false;

	// Process control Handles
	if (m_hEventCancel != NULL)
		CloseHandle(m_hEventCancel);
	if (m_hEventCancelACK != NULL)
		CloseHandle(m_hEventCancelACK);
	if (m_hEventDone != NULL)
		CloseHandle(m_hEventDone);
	if (m_hEventDoneACK != NULL)
		CloseHandle(m_hEventDoneACK);
	if (m_hServerReady != NULL)
		CloseHandle(m_hServerReady);
	if (m_hClientReady != NULL)
		CloseHandle(m_hClientReady);

	m_hEventCancel = NULL;
	m_hEventCancelACK = NULL;
	m_hEventDone = NULL;
	m_hEventDoneACK = NULL;
	m_hServerReady = NULL;
	m_hClientReady = NULL;
}

CString CBaseComProcess::GetDataShareNameFull(bool IsWriter)
{
	CString result;
	result = m_sSharedName;

	if (m_bIsServer)
	{
		result += IsWriter? _T("_OUT"): _T("_IN");
	}
	else
	{
		result += IsWriter? _T("_IN"): _T("_OUT");
	}

	return result;
}

CString CBaseComProcess::GetStartEventName(bool isServer)
{
	CString result;
	result = _T("CW_EVENT_");
	result += m_sSharedName;
	result += _T("_READY");
	if (isServer)
		result += _T("_SERVER");
	else
		result += _T("_CLIENT");

	return result;
}

CString CBaseComProcess::GetCancelEventName(bool isACK)
{
	CString result;
	result = _T("CW_EVENT_");
	result += m_sSharedName;
	result += _T("_CANCEL");
	if (isACK)
		result += _T("_ACK");

	return result;
}

CString CBaseComProcess::GetDoneEventName(bool isACK)
{
	CString result;
	result = _T("CW_EVENT_");
	result += m_sSharedName;
	result += _T("_DONE");
	if (isACK)
		result += _T("_ACK");

	return result;
}

CString CBaseComProcess::GetEventActionBaseName()
{
	CString result;
	result = _T("CW_EVENT_ACTION_");
	result += m_sSharedName;

	return result;
}

bool CBaseComProcess::Initialize()
{
	if (m_bIsServer)
	{
		// Server Side must create the events
		m_hEventCancel = CreateEvent(NULL, TRUE, FALSE, GetCancelEventName(false));

		if (m_hEventCancel == NULL)
		{
			ResetCom();
			return false;
		}

		m_hEventCancelACK = CreateEvent(NULL, TRUE, FALSE, GetCancelEventName(true));

		if (m_hEventCancelACK == NULL)
		{
			ResetCom();
			return false;
		}

		m_hEventDone = CreateEvent(NULL, TRUE, FALSE, GetDoneEventName(false));

		if (m_hEventDone == NULL)
		{
			ResetCom();
			return false;
		}

		m_hEventDoneACK = CreateEvent(NULL, TRUE, FALSE, GetDoneEventName(true));

		if (m_hEventDoneACK == NULL)
		{
			ResetCom();
			return false;
		}

		m_hServerReady = CreateEvent(NULL, TRUE, FALSE, GetStartEventName(true));

		if (m_hServerReady == NULL)
		{
			ResetCom();
			return false;
		}

		m_hClientReady = CreateEvent(NULL, TRUE, FALSE, GetStartEventName(false));

		if (m_hClientReady == NULL)
		{
			ResetCom();
			return false;
		}
	}
	else
	{
		// Client Side - Will open Events
		m_hEventCancel = OpenEvent(EVENT_ALL_ACCESS, FALSE, GetCancelEventName(false));

		if (m_hEventCancel == NULL)
		{
			ResetCom();
			return false;
		}

		m_hEventCancelACK = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetCancelEventName(true));

		if (m_hEventCancelACK == NULL)
		{
			ResetCom();
			return false;
		}

		m_hEventDone = OpenEvent(EVENT_ALL_ACCESS, FALSE, GetDoneEventName(false));

		if (m_hEventDone == NULL)
		{
			ResetCom();
			return false;
		}

		m_hEventDoneACK = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetDoneEventName(true));

		if (m_hEventDoneACK == NULL)
		{
			ResetCom();
			return false;
		}

		m_hServerReady = OpenEvent(EVENT_ALL_ACCESS, FALSE, GetStartEventName(true));

		if (m_hServerReady == NULL)
		{
			ResetCom();
			return false;
		}

		m_hClientReady = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetStartEventName(false));

		if (m_hClientReady == NULL)
		{
			ResetCom();
			return false;
		}
	}

	if (!InitActions(m_bIsServer))
	{
		ResetCom();
		return false;
	}

	// Shared Memory is then initialized
	if (!m_DataShare_Out.Initialize(m_bIsServer, false, GetDataShareNameFull(true), m_dSharedBufferSize))
	{
		ResetCom();
		return false;
	}

	if (!m_DataShare_In.Initialize(m_bIsServer, false, GetDataShareNameFull(false), m_dSharedBufferSize))
	{
		ResetCom();
		return false;
	}

	return true;
}

// Function that will handle connection and waiting between the 2 processes, ensures both sides running
bool CBaseComProcess::WaitForConnection()
{
	DWORD dwWaitReady = 0;
	
	if (m_bIsServer)
	{
		SetEvent(m_hServerReady);
		HANDLE WaitObjects[] = { m_hClientReady };

#ifdef IS_PROXY
		dwWaitReady = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);
#else
		dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE, QS_ALLINPUT);
#endif

		while (dwWaitReady == WAIT_OBJECT_0 + _countof(WaitObjects))
		{
#ifndef IS_PROXY
			MSG msg = { 0 };
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					PostQuitMessage(static_cast<int>(msg.wParam));
					return false;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
#endif
#ifdef IS_PROXY
			dwWaitReady = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);
#else
			dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE, QS_ALLINPUT);
#endif
		}
	}
	else
	{
		SetEvent(m_hClientReady);
		HANDLE WaitObjects[] = { m_hServerReady };

#ifdef IS_PROXY
		dwWaitReady = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);
#else
		dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE, QS_ALLINPUT);
#endif

		while (dwWaitReady == WAIT_OBJECT_0 + _countof(WaitObjects))
		{
#ifndef IS_PROXY
			MSG msg = { 0 };
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					PostQuitMessage(static_cast<int>(msg.wParam));
					return false;
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
#endif
#ifdef IS_PROXY
			dwWaitReady = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);
#else
			dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE, QS_ALLINPUT);
#endif
		}
	}
			
	return (dwWaitReady == WAIT_OBJECT_0);
}

void CBaseComProcess::TriggerCancelOrACK()
{
	if (m_bIsServer)
	{
		SetEvent(m_hEventCancel);
	}
	else
	{
		SetEvent(m_hEventCancelACK);
	}
}

void CBaseComProcess::TriggerDoneOrACK()
{
	if (m_bIsServer)
	{
		SetEvent(m_hEventDone);
	}
	else
	{
		SetEvent(m_hEventDoneACK);
	}
}

bool CBaseComProcess::IsCancelled()
{
	return m_bCancelTriggered;
}

bool CBaseComProcess::IsDone()
{
	return m_bDoneTriggered;
}

void CBaseComProcess::CopyMemoryTo(void* DestBuffer, DWORD Size)
{
	void* Source = m_DataShare_In.lock_buffer_read();
	if (Source)
	{
		memset(DestBuffer, 0, Size);
		memcpy(DestBuffer, Source, Size);
	}
	else
	{
		ASSERT(0);
	}
	m_DataShare_In.unlock_buffer_read(Source);
}

void CBaseComProcess::CopyMemoryFrom(void* SourceBuffer, DWORD Size)
{
	void* Destination = m_DataShare_Out.lock_buffer_write();
	if (Destination)
	{
		memset(Destination, 0, m_dSharedBufferSize);
		memcpy(Destination, SourceBuffer, Size);
	}
	else
	{
		ASSERT(0);
	}
	m_DataShare_Out.unlock_buffer_write(Destination);
}

void CBaseComProcess::CopyStringTo(CString &Destination)
{
	Destination = m_DataShare_In.pop(m_DataShare_In.m_dFileSize);
}

void CBaseComProcess::CopyStringFrom(CString Source)
{
	m_DataShare_Out.push(Source);
}

void* CBaseComProcess::GetReadBuffer()
{
	return m_DataShare_In.lock_buffer_read();
}

void* CBaseComProcess::GetWriteBuffer()
{
	void *p = m_DataShare_Out.lock_buffer_write();
	memset(p, 0, m_dSharedBufferSize);
	return p;
}

void CBaseComProcess::ReleaseReadBuffer(void* p)
{
	m_DataShare_In.unlock_buffer_read(p);
}

void CBaseComProcess::ReleaseWriteBuffer(void* p)
{
	m_DataShare_Out.unlock_buffer_write(p);
}

bool CBaseComProcess::InitActions(bool IsServer)
{
	return false;
}

// virtual (will be replaced by inherited clients)
void CBaseComProcess::ResetActions()
{}
void CBaseComProcess::ResetHandles()
{}
void CBaseComProcess::TriggerAction(int ActionCode)
{}
int CBaseComProcess::WaitForAction(DWORD Timeout)
{
	return -1;
}

//////////////////////////////////////////////////////////////////////////////////
// CExampleComProcess

CExampleComProcess::CExampleComProcess(bool IsServer, CString Name, DWORD Size)
	: CBaseComProcess(IsServer, Name, Size)
{
	m_hInit = NULL;
	m_hOpen = NULL;
	m_hClose = NULL;
	m_hRead = NULL;
	m_hWrite = NULL;
	m_hActionOK = NULL;
	m_hActionEOF = NULL;
	m_hActionError = NULL;
}

CExampleComProcess::~CExampleComProcess()
{
	ResetHandles();
}

void CExampleComProcess::ResetHandles()
{
	// Action Event Handles
	if (m_hInit != NULL)
		CloseHandle(m_hInit);
	if (m_hOpen != NULL)
		CloseHandle(m_hOpen);
	if (m_hClose != NULL)
		CloseHandle(m_hClose);
	if (m_hRead != NULL)
		CloseHandle(m_hRead);
	if (m_hWrite != NULL)
		CloseHandle(m_hRead);
	if (m_hActionOK != NULL)
		CloseHandle(m_hActionOK);
	if (m_hActionEOF != NULL)
		CloseHandle(m_hActionEOF);
	if (m_hActionError != NULL)
		CloseHandle(m_hActionError);

	m_hInit = NULL;
	m_hOpen = NULL;
	m_hClose = NULL;
	m_hRead = NULL;
	m_hWrite = NULL;
	m_hActionOK = NULL;
	m_hActionEOF = NULL;
	m_hActionError = NULL;
}

CString CExampleComProcess::GetEventActionName(EventExampleAction action)
{
	CString result;
	result = GetEventActionBaseName();
	switch(action)
	{
	case EIAOK:
		result += _T("_OK");
		break;
	case EIAEOF:
		result += _T("_EOF");
		break;
	case EIAError:
		result += _T("_ERROR");
		break;
	case EIAInit:
		result += _T("_INIT");
		break;
	case EIAOpen:
		result += _T("_OPEN");
		break;
	case EIAClose:
		result += _T("_CLOSE");
		break;
	case EIARead:
		result += _T("_READ");
		break;
	case EIAWrite:
		result += _T("_WRITE");
		break;
	}

	return result;
}

bool CExampleComProcess::InitActions(bool IsServer)
{
	if (m_bIsServer)
	{
		// Server Side must create the events
		// Action Event Create commands

		m_hInit = CreateEvent(NULL, TRUE, FALSE, GetEventActionName(EIAInit));
		if (m_hInit == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hOpen = CreateEvent(NULL, TRUE, FALSE, GetEventActionName(EIAOpen));

		if (m_hOpen == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hClose = CreateEvent(NULL, TRUE, FALSE, GetEventActionName(EIAClose));

		if (m_hClose == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hRead = CreateEvent(NULL, TRUE, FALSE, GetEventActionName(EIARead));

		if (m_hRead == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hWrite = CreateEvent(NULL, TRUE, FALSE, GetEventActionName(EIAWrite));

		if (m_hWrite == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hActionOK = CreateEvent(NULL, TRUE, FALSE, GetEventActionName(EIAOK));

		if (m_hActionOK == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hActionEOF = CreateEvent(NULL, TRUE, FALSE, GetEventActionName(EIAEOF));

		if (m_hActionEOF == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hActionError = CreateEvent(NULL, TRUE, FALSE, GetEventActionName(EIAError));

		if (m_hActionError == NULL)
		{
			ResetHandles();
			return false;
		}

	}
	else
	{
		// Client Side - Will open Events
		m_hInit = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetEventActionName(EIAInit));

		if (m_hInit == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hOpen = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetEventActionName(EIAOpen));

		if (m_hOpen == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hClose = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetEventActionName(EIAClose));

		if (m_hClose == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hRead = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetEventActionName(EIARead));

		if (m_hRead == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hRead = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetEventActionName(EIAWrite));

		if (m_hRead == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hActionOK = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetEventActionName(EIAOK));

		if (m_hActionOK == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hActionEOF = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetEventActionName(EIAEOF));

		if (m_hActionEOF == NULL)
		{
			ResetHandles();
			return false;
		}

		m_hActionError = OpenEvent(EVENT_ALL_ACCESS, TRUE, GetEventActionName(EIAError));

		if (m_hActionError == NULL)
		{
			ResetHandles();
			return false;
		}
	}

	return true;
}

void CExampleComProcess::TriggerAction(int Action)
{
	if (m_bIsServer)
	{
		switch(Action)
		{
			case EIAInit:
				SetEvent(m_hInit);
				break;
			case EIAOpen:
				SetEvent(m_hOpen);
				break;
			case EIAClose:
				SetEvent(m_hClose);
				break;
			case EIARead:
				SetEvent(m_hRead);
				break;
			case EIAWrite:
				SetEvent(m_hRead);
				break;
		};
	}
	else 
	{
		switch(Action)
		{
			case EIAOK:
				SetEvent(m_hActionOK);
				break;
			case EIAEOF:
				SetEvent(m_hActionEOF);
				break;
			case EIAError:
				SetEvent(m_hActionError);
				break;
		}
	}
}

void CExampleComProcess::ResetInit()
{
	ResetEvent(m_hInit);
}

void CExampleComProcess::ResetActions()
{
	if (m_bIsServer)
	{
		ResetEvent(m_hActionOK);
		ResetEvent(m_hActionEOF);
		ResetEvent(m_hActionError);
	}
	else
	{
		ResetEvent(m_hOpen);
		ResetEvent(m_hClose);
		ResetEvent(m_hRead);
		ResetEvent(m_hWrite);
	}
}

int CExampleComProcess::WaitForAction(DWORD Timeout)
{
	DWORD dwWaitReady = 0, dwCountresult = 0;
	HANDLE WaitObjectsClient[] = { m_hInit, m_hOpen, m_hClose, m_hRead, m_hWrite, m_hEventCancel, m_hEventDone };
	HANDLE WaitObjectsServer[] = { m_hActionOK, m_hActionEOF, m_hActionError, m_hEventCancelACK, m_hEventDoneACK };
	if (Timeout == 0) Timeout = INFINITE;
	
	if (m_bIsServer)
	{
#ifdef IS_PROXY
		dwWaitReady = WaitForMultipleObjects(_countof(WaitObjectsServer), WaitObjectsServer, false, INFINITE);
#else
		dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjectsServer), WaitObjectsServer, false, INFINITE, QS_ALLINPUT);
#endif
		dwCountresult = _countof(WaitObjectsServer);
	}
	else
	{
#ifdef IS_PROXY
		dwWaitReady = WaitForMultipleObjects(_countof(WaitObjectsClient), WaitObjectsClient, false, INFINITE);
#else
		dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjectsClient), WaitObjectsClient, false, INFINITE, QS_ALLINPUT);
#endif
		dwCountresult = _countof(WaitObjectsClient);
	}

	while (dwWaitReady == WAIT_OBJECT_0 + dwCountresult)
	{
#ifndef IS_PROXY
		MSG msg = { 0 };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				PostQuitMessage(static_cast<int>(msg.wParam));
				return EIANone;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
#endif
		if (m_bIsServer)
		{
#ifdef IS_PROXY
			dwWaitReady = WaitForMultipleObjects(_countof(WaitObjectsServer), WaitObjectsServer, false, INFINITE);
#else
			dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjectsServer), WaitObjectsServer, false, INFINITE, QS_ALLINPUT);
#endif
			dwCountresult = _countof(WaitObjectsServer);
		}
		else
		{
#ifdef IS_PROXY
			dwWaitReady = WaitForMultipleObjects(_countof(WaitObjectsClient), WaitObjectsClient, false, INFINITE);
#else
			dwWaitReady = MsgWaitForMultipleObjects(_countof(WaitObjectsClient), WaitObjectsClient, false, INFINITE, QS_ALLINPUT);
#endif
			dwCountresult = _countof(WaitObjectsClient);
		}
	}

	if (dwWaitReady <= WAIT_OBJECT_0 + _countof(WaitObjectsClient) - 1)
	{
		switch (dwWaitReady)
		{
		case WAIT_OBJECT_0:
			return ((m_bIsServer) ? EIAOK : EIAInit);
			break;
		case WAIT_OBJECT_0 + 1:
			return ((m_bIsServer) ? EIAEOF : EIAOpen);
			break;
		case WAIT_OBJECT_0 + 2:
			return ((m_bIsServer) ? EIAError : EIAClose);
			break;
		case WAIT_OBJECT_0 + 3:
			if (m_bIsServer)
			{
				m_bCancelTriggered = true;
				return EIAEnding;
			}
			else
			{
				return EIARead;
			}
			break;
		case WAIT_OBJECT_0 + 4:
			if (m_bIsServer)
			{
				m_bDoneTriggered = true;
				return EIAEnding;
			}
			else
			{
				return EIAWrite;
			}
			break;
		case WAIT_OBJECT_0 + 5:
			if (m_bIsServer)	// Only client side
			{
				return EIANone;
			}
			else
			{
				m_bCancelTriggered = true;
				return EIAEnding;
			}
			break;
		case WAIT_OBJECT_0 + 6:
			if (m_bIsServer)	// Only client side
			{
				return EIANone;
			}
			else
			{
				m_bDoneTriggered = true;
				return EIAEnding;
			}
			break;
		default:
			return EIANone;
			break;
		}
	}
	else
	{
		return EIANone;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// CProcessComMonitor

CProcessComMonitor::CProcessComMonitor()
{
	m_bCallerStopped = m_bDoneTriggered = false;
	m_hEventDone = NULL;
	m_hProcessMonitor = NULL;
	m_hCallingProcess = NULL;
	m_dwMonitorThreadId = 0;
}

CProcessComMonitor::~CProcessComMonitor()
{
	DWORD dwMonitorResult = 0;

	// Exiting - Give Threads time to quit themselves, timeout indicates it is still active
	if (m_hProcessMonitor != NULL)
	{
		if (WaitForSingleObject(m_hProcessMonitor, 5000) == WAIT_TIMEOUT)
		{
			// Force Thread to drop everything
			TerminateThread(m_hProcessMonitor, 0xDEAD);
			GetExitCodeThread(m_hProcessMonitor, &dwMonitorResult);
		}
		CloseHandle(m_hProcessMonitor);
	}

	if (m_hCallingProcess != NULL)
		CloseHandle(m_hCallingProcess);

	if (m_hEventDone != NULL)
		CloseHandle(m_hEventDone);

	m_hEventDone = NULL;
	m_hProcessMonitor = NULL;
	m_hCallingProcess = NULL;
	m_dwMonitorThreadId = 0;
}

bool CProcessComMonitor::StartMonitor(CString MonitorName, DWORD PID)
{
	// Create the named objects as required, including the thread that will run
	CString sEventname = _T("CW_MONITOR_EVENT_DONE_") + MonitorName;
	m_hCallingProcess = OpenProcess(SYNCHRONIZE, false, PID);

	m_hEventDone = CreateEvent(NULL, TRUE, FALSE, sEventname);
	if (m_hEventDone == NULL)
	{
		return false;
	}

	m_hProcessMonitor = CreateThread(
		NULL,
		0,
		&MonitorProc,
		(LPVOID)this,
		0,
		&m_dwMonitorThreadId);

	if (m_hProcessMonitor == NULL)
	{
		return false;
	}

	return true;
}

void CProcessComMonitor::StopMonitor()
{
	if (m_hEventDone) SetEvent(m_hEventDone);
}

DWORD WINAPI CProcessComMonitor::MonitorProc(LPVOID lpParameter)
{
	bool bProcessGood = true;

	if (lpParameter != NULL)
	{
		// Get Pointer back to using CProcessMonitor
		CProcessComMonitor* Caller = static_cast<CProcessComMonitor*>(lpParameter);
		HANDLE WaitObjects[] = { Caller->m_hCallingProcess, Caller->m_hEventDone };

		while (bProcessGood)
		{
			// Wait for a trigger, which will indicate the monitor state (Done or Stop)
			DWORD dwWait = WaitForMultipleObjects(_countof(WaitObjects), WaitObjects, false, INFINITE);

			if (dwWait >= WAIT_OBJECT_0)
			{
				// Process signalled it is closed either Calling Process terminated or Current Process
				bProcessGood = false;
				Caller->m_bDoneTriggered = (dwWait == WAIT_OBJECT_0 + 1);
				Caller->m_bCallerStopped = (dwWait == WAIT_OBJECT_0);
			}
		}
	}

	return 0;
}

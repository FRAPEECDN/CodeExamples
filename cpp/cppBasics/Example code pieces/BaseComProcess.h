////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BaseComprocess.h: Interface for establishing Inter Process Communication [Need to inherit class]
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include <atlstr.h>

// IPC Shared Object File for sharing a file (memory local or global) between processes
class CIPCSharedTransfer
{
private:

	HANDLE m_hSharedMutex;
	HANDLE m_hReadReadyEvent;
	HANDLE m_hSharedMapFile;
	void*  m_pBuffer;
	CString m_sSharedName;
	bool m_isGlobal;
	bool m_bIsServer;

	void ResetTransfer();
	CString GetFileNameFull();
	CString GetShareHandleNameFull(bool Writer);
	bool WaitForAccess();
	bool WaitForReading();

public:
	CIPCSharedTransfer();
	~CIPCSharedTransfer();

	bool Initialize(bool IsServer, bool IsGlobal, CString SharedName, DWORD Size);

	// Normal Reading/Writing (Text and Byte data)
	DWORD push(CString input);
	DWORD push_byte(BYTE* input, DWORD Size);
	CString pop(DWORD Size);
	DWORD pop_byte(BYTE* output, DWORD Size);

	// Pointer access control
	DWORD  m_dFileSize;
	void* lock_buffer_write();
	void* lock_buffer_read();
	void unlock_buffer_write(void* pBuf);
	void unlock_buffer_read(void* pBuf);
	void Release();
};

// IPC Base Process, needs to be inherited from, but provides the basic events for Cancelling, Done and Ready (Starting point for both IPC)
// Inherited classes should extend the events as needed for custom actions
class CBaseComProcess
{
protected:
	bool m_bIsServer;
	CString m_sSharedName;
	DWORD m_dSharedBufferSize;
	CIPCSharedTransfer m_DataShare_In;
	CIPCSharedTransfer m_DataShare_Out;
	
	// Internal Methods
	void ResetCom();

	// Names for Events, Memory to access from Processes
	CString GetStartEventName(bool isServer);
	CString GetDataShareNameFull(bool IsWriter);
	CString GetCancelEventName(bool isACK);
	CString GetDoneEventName(bool isACK);
	CString GetEventActionBaseName();

	// Event Handles [Start, Cancel, Done]
	HANDLE m_hEventCancel;
	HANDLE m_hEventCancelACK;
	HANDLE m_hEventDone;
	HANDLE m_hEventDoneACK;
	HANDLE m_hServerReady;
	HANDLE m_hClientReady;

	// Indicators for ending either cancel or normal
	bool   m_bDoneTriggered;
	bool   m_bCancelTriggered;
	
	// Commands handling the requests and responses (Inherited classes) Customizable per actions needed
	virtual bool InitActions(bool IsServer);
	virtual void ResetActions();
	virtual void ResetHandles();
	virtual void TriggerAction(int ActionCode);
	virtual int WaitForAction(DWORD Timeout);
	
public:

	CBaseComProcess(bool IsServer, CString Name, DWORD Size);
	~CBaseComProcess();

	// Start calls [Constructor, Initialize, Wait]
	bool Initialize();
	bool WaitForConnection();

	// Triggers to Cancel or Signal Done
	void TriggerCancelOrACK();
	void TriggerDoneOrACK();

	// Query Cancel or Done
	bool IsCancelled();
	bool IsDone();

	// Memory Copy - Basic Commands, it supports buffers, strings and pointers
	// so that there is some flexibility based on what is needed
	void CopyMemoryTo(void* DestBuffer, DWORD Size);
	void CopyMemoryFrom(void* SourceBuffer, DWORD Size);
	void CopyStringTo(CString &Destination);
	void CopyStringFrom(CString Source);
	void* GetReadBuffer();
	void* GetWriteBuffer();
	void ReleaseReadBuffer(void* p);
	void ReleaseWriteBuffer(void* p);
};

enum EventExampleAction { EIANone, EIAEnding, EIAOK, EIAEOF, EIAError, EIAInit, EIAOpen, EIAClose, EIARead, EIAWrite };

// Specific implemented class serving as example which is handling a database
// with concepts of init, open, read, write and close
class CExampleComProcess : public CBaseComProcess
{
private:

	// Event Handles for EventActions
	HANDLE m_hInit;
	HANDLE m_hOpen;
	HANDLE m_hClose;
	HANDLE m_hRead;
	HANDLE m_hWrite;

	HANDLE m_hActionOK;
	HANDLE m_hActionEOF;
	HANDLE m_hActionError;

	CString GetEventActionName(EventExampleAction action);

public:
	CExampleComProcess(bool IsServer, CString Name, DWORD Size);
	~CExampleComProcess();

	virtual bool InitActions(bool IsServer);
	virtual void ResetActions();
	virtual void ResetHandles();
	virtual void TriggerAction(int ActionCode);
	virtual int WaitForAction(DWORD Timeout);

	void ResetInit();	// Init is only used once to set Handlers etc.

};


// This is a class in the IPC which will create a Thread to keep track of the calling process
// If the calling process terminates for some reason, the other process will get a signal then to 
// quit as well. This will ensure that everything does shut down correctly.
class CProcessComMonitor
{
private:
	bool   m_bDoneTriggered;

	HANDLE m_hEventDone;
	HANDLE m_hProcessMonitor;
	HANDLE m_hCallingProcess;
	DWORD m_dwMonitorThreadId;

	static DWORD WINAPI MonitorProc(LPVOID lpParameter);

public:
	CProcessComMonitor();
	~CProcessComMonitor();

	bool StartMonitor(CString MonitorName, DWORD PID);
	void StopMonitor();
	bool m_bCallerStopped;
};
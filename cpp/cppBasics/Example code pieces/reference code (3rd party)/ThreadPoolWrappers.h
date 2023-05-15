/*
Module : ThreadPoolWrappers.h
Purpose: Defines the interface for a set of classes to encapsulate the Windows Thread Pool APIs.
History: PJN / 04-03-2018 1. Initial public release

Copyright (c) 2018 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


///////////////////////// Macros / Defines ////////////////////////////////////

#pragma once

#ifndef __THREADPOOLWRAPPERS_H__
#define __THREADPOOLWRAPPERS_H__

#ifndef CTHREADPOOLWRAPPERS_EXT_CLASS
#define CTHREADPOOLWRAPPERS_EXT_CLASS
#endif //#ifndef CTHREADPOOLWRAPPERS_EXT_CLASS

#ifndef CTHREADPOOLWRAPPERS_EXT_API
#define CTHREADPOOLWRAPPERS_EXT_API
#endif //#ifndef CTHREADPOOLWRAPPERS_EXT_API


///////////////////////// Includes ////////////////////////////////////////////

#ifndef _THREADPOOLAPISET_H_
#pragma message("To avoid this message, please put threadpoolapiset.h in your pre compiled header (normally stdafx.h)")
#include <threadpoolapiset.h>
#endif //#ifndef _THREADPOOLAPISET_H_

#include <assert.h>


///////////////////////// Implementation  /////////////////////////////////////

//Namespaces
namespace ThreadPoolWrappers
{

//RAII class wrapper for a PTP_POOL
class CTHREADPOOLWRAPPERS_EXT_CLASS CPool
{
public:
//Constructors / Destructors
  CPool() : m_Pool(nullptr)
  {
  }

  CPool(_In_ const CPool&) = delete;

  CPool(_In_ CPool&& pool) : CPool()
  {
    Attach(pool.Detach());
  }

  explicit CPool(_In_opt_ PTP_POOL pPool) : m_Pool(pPool)
  {
  }

  ~CPool()
  {
    if (m_Pool != nullptr)
      Close();
  }

//Methods
  CPool& operator=(_In_ const CPool&) = delete;

  CPool& operator=(_In_ CPool&& pool)
  {
    if (m_Pool != nullptr)
      Close();
    Attach(pool.Detach());
    return *this;
  }

  void Attach(_In_opt_ PTP_POOL pPool)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Pool == nullptr);
  #endif //#ifdef _DEBUG

    m_Pool = pPool;
  }

  PTP_POOL Detach()
  {
    PTP_POOL pPool = m_Pool;
    m_Pool = nullptr;
    return pPool;
  }

  _Must_inspect_result_ bool Create()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Pool == nullptr);
  #endif //#ifdef _DEBUG

    m_Pool = CreateThreadpool(nullptr);
    return (m_Pool != nullptr) ? true : false;
  }

  void Close()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Pool != nullptr);
  #endif //#ifdef _DEBUG

    CloseThreadpool(m_Pool);
    m_Pool = nullptr;
  }

  void SetThreadMaximum(_In_ DWORD cThrdMost)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Pool != nullptr);
  #endif //#ifdef _DEBUG

    SetThreadpoolThreadMaximum(m_Pool, cThrdMost);
  }

  BOOL SetThreadMinimum(_In_ DWORD cThrdMic)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Pool != nullptr);
  #endif //#ifdef _DEBUG

    return SetThreadpoolThreadMinimum(m_Pool, cThrdMic);
  }

  BOOL SetStackInformation(_In_ PTP_POOL_STACK_INFORMATION ptpsi)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Pool != nullptr);
  #endif //#ifdef _DEBUG

    return SetThreadpoolStackInformation(m_Pool, ptpsi);
  }

  BOOL QueryStackInformation(_Out_ PTP_POOL_STACK_INFORMATION ptpsi)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Pool != nullptr);
  #endif //#ifdef _DEBUG

    return QueryThreadpoolStackInformation(m_Pool, ptpsi);
  }

  PTP_POOL get() const
  {
    return m_Pool;
  }

protected:
//Member variables
  PTP_POOL m_Pool;
};


//RAII class wrapper for a PTP_CLEANUP_GROUP
class CTHREADPOOLWRAPPERS_EXT_CLASS CCleanupGroup
{
public:
//Constructors / Destructors
  CCleanupGroup() : m_CleanupGroup(nullptr)
  {
  }

  CCleanupGroup(_In_ const CCleanupGroup&) = delete;

  CCleanupGroup(_In_ CCleanupGroup&& cleanupGroup) : CCleanupGroup()
  {
    Attach(cleanupGroup.Detach());
  }

  explicit CCleanupGroup(_In_opt_ PTP_CLEANUP_GROUP pCleanupGroup) : m_CleanupGroup(pCleanupGroup)
  {
  }

  ~CCleanupGroup()
  {
    if (m_CleanupGroup != nullptr)
      Close();
  }

//Methods
  CCleanupGroup& operator=(_In_ const CCleanupGroup&) = delete;

  CCleanupGroup& operator=(_In_ const CCleanupGroup&& cleanupGroup)
  {
    if (m_CleanupGroup != nullptr)
      Close();
    m_CleanupGroup = cleanupGroup.m_CleanupGroup;
    return *this;
  }

  void Attach(_In_opt_ PTP_CLEANUP_GROUP pCleanupGroup)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_CleanupGroup == nullptr);
  #endif //#ifdef _DEBUG

    m_CleanupGroup = pCleanupGroup;
  }

  PTP_CLEANUP_GROUP Detach()
  {
    PTP_CLEANUP_GROUP pCleanupGroup = m_CleanupGroup;
    m_CleanupGroup = nullptr;
    return pCleanupGroup;
  }

  _Must_inspect_result_ bool Create()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_CleanupGroup == nullptr);
  #endif //#ifdef _DEBUG

    m_CleanupGroup = CreateThreadpoolCleanupGroup();
    return (m_CleanupGroup != nullptr) ? true : false;
  }

  void Close()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_CleanupGroup != nullptr);
  #endif //#ifdef _DEBUG

    CloseThreadpoolCleanupGroup(m_CleanupGroup);
    m_CleanupGroup = nullptr;
  }

  void CloseMembers(_In_ BOOL fCancelPendingCallbacks, _Inout_opt_ PVOID pvCleanupContext = nullptr)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_CleanupGroup != nullptr);
  #endif //#ifdef _DEBUG

    CloseThreadpoolCleanupGroupMembers(m_CleanupGroup, fCancelPendingCallbacks, pvCleanupContext);
  }

  PTP_CLEANUP_GROUP get() const
  {
    return m_CleanupGroup;
  }

protected:
//Member variables
  PTP_CLEANUP_GROUP m_CleanupGroup;
};


//RAII class wrapper for a TP_CALLBACK_ENVIRON
class CTHREADPOOLWRAPPERS_EXT_CLASS CEnvironment : public TP_CALLBACK_ENVIRON
{
public:
//Constructors / Destructors
  CEnvironment()
  {
    InitializeThreadpoolEnvironment(this);
  }

  CEnvironment(_In_ const CEnvironment&) = delete;
  CEnvironment(_In_ CEnvironment&&) = delete;

  ~CEnvironment()
  {
    Destroy();
  }

//Methods
  CEnvironment& operator=(_In_ const CEnvironment&) = delete;
  CEnvironment& operator=(_In_ const CEnvironment&&) = delete;

  void SetPool(_In_ const CPool& pool)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(pool.get() != nullptr);
  #endif //#ifdef _DEBUG

    SetThreadpoolCallbackPool(this, pool.get());
  }

  void SetCleanupGroup(_In_ const CCleanupGroup& group, _In_opt_ PTP_CLEANUP_GROUP_CANCEL_CALLBACK callback)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(group.get() != nullptr);
  #endif //#ifdef _DEBUG

    SetThreadpoolCallbackCleanupGroup(this, group.get(), callback);
  }

  void SetCallbackRunsLong()
  {
    SetThreadpoolCallbackRunsLong(this);
  }

  void SetActivationContext(_In_opt_ _ACTIVATION_CONTEXT* context)
  {
    TpSetCallbackActivationContext(this, context);
  }

  void SetNoActivationContext()
  {
    TpSetCallbackNoActivationContext(this);
  }

  void SetCallbackLibrary(_In_ PVOID DllHandle)
  {
    SetThreadpoolCallbackLibrary(this, DllHandle);
  }

  void SetFinalizationCallback(_In_ PTP_SIMPLE_CALLBACK callback)
  {
    TpSetCallbackFinalizationCallback(this, callback);
  }

__if_exists(SetThreadpoolCallbackPriority)
{
  void SetPriority(_In_ TP_CALLBACK_PRIORITY Priority)
  {
    SetThreadpoolCallbackPriority(this, Priority);
  }
}

  void SetPersistent()
  {
    SetThreadpoolCallbackPersistent(this);
  }

protected:
  void Destroy()
  {
    DestroyThreadpoolEnvironment(this);
  }
};


//RAII class wrapper for a PTP_TIMER
class CTHREADPOOLWRAPPERS_EXT_CLASS CTimer
{
public:
//Constructors / Destructors
  CTimer() : m_Timer(nullptr),
             m_bAutoRelease(true)
  {
  }

  CTimer(_In_ const CTimer&) = delete;

  CTimer(_In_ CTimer&& timer) : CTimer()
  {
    Attach(timer.Detach(), timer.m_bAutoRelease);
  }

  explicit CTimer(_In_opt_ PTP_TIMER pTimer, bool bAutoRelease) : m_Timer(pTimer),
                                                                  m_bAutoRelease(bAutoRelease)
  {
  }

  ~CTimer()
  {
    if (m_Timer != nullptr)
      Close();
  }

//Methods
  CTimer& operator=(_In_ const CTimer&) = delete;

  CTimer& operator=(_In_ CTimer&& timer)
  {
    if (m_Timer != nullptr)
      Close();
    Attach(timer.Detach(), timer.m_bAutoRelease);
    return *this;
  }

  void Attach(_In_opt_ PTP_TIMER pTimer, bool bAutoRelease)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer == nullptr);
  #endif //#ifdef _DEBUG

    m_Timer = pTimer;
    m_bAutoRelease = bAutoRelease;
  }

  PTP_TIMER Detach()
  {
    PTP_TIMER pTimer = m_Timer;
    m_Timer = nullptr;
    return pTimer;
  }

  _Must_inspect_result_ BOOL Create(_In_ PTP_TIMER_CALLBACK pfnti, _Inout_opt_ PVOID pv, _In_opt_ CEnvironment* pEnvironment)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer == nullptr);
  #endif //#ifdef _DEBUG

    m_Timer = CreateThreadpoolTimer(pfnti, pv, pEnvironment ? pEnvironment : nullptr);
    m_bAutoRelease = true;
    return (m_Timer != nullptr) ? TRUE : FALSE;
  }

  void Close()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer != nullptr);
  #endif //#ifdef _DEBUG

    if (m_bAutoRelease)
      CloseThreadpoolTimer(m_Timer);
    m_Timer = nullptr;
  }

  void Set(_In_opt_ PFILETIME pftDueTime, _In_ DWORD msPeriod, _In_opt_ DWORD msWindowLength)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer != nullptr);
  #endif //#ifdef _DEBUG

    SetThreadpoolTimer(m_Timer, pftDueTime, msPeriod, msWindowLength);
  }

  void Cease()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer != nullptr);
  #endif //#ifdef _DEBUG

    SetThreadpoolTimer(m_Timer, nullptr, 0, 0);
  }

  BOOL SetOnceOffAbsolute(_In_ const SYSTEMTIME& DueTime, _In_opt_ DWORD msWindowLength)
  {
    //Convert from the SystemTime to the filetime struct
    FILETIME ft;
    if (!SystemTimeToFileTime(&DueTime, &ft))
      return FALSE;
    Set(&ft, 0, msWindowLength);
    return TRUE;
  }

  void SetOnceOffRelative(_In_ DWORD dwMilliseconds, _In_opt_ DWORD msWindowLength)
  {
    //Convert from the milliseconds to FILETIME representation (100-nanoseconds)
    //We also need to make it negative
    __int64 nFileTimes = static_cast<__int64>(-10000) * static_cast<__int64>(dwMilliseconds);
    LARGE_INTEGER li;
    li.LowPart = static_cast<DWORD>(nFileTimes & 0xFFFFFFFF);
    li.HighPart = static_cast<LONG>(nFileTimes >> 32); 
    Set(reinterpret_cast<FILETIME*>(&li), 0, msWindowLength);
  }
  
  BOOL SetPeriodicAbsolute(_In_ const SYSTEMTIME& DueTime, _In_ DWORD msPeriod, _In_opt_ DWORD msWindowLength)
  {
    //Convert from the SystemTime to the FILETIME struct
    FILETIME ft;
    if (!SystemTimeToFileTime(&DueTime, &ft))
      return FALSE;
    LARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    Set(reinterpret_cast<FILETIME*>(&li), msPeriod, msWindowLength);
    return TRUE;
  }
  
  void SetPeriodicRelative(_In_ DWORD dwMilliseconds, _In_ DWORD msPeriod, _In_opt_ DWORD msWindowLength)
  {
    //Convert from the milliseconds to FILETIME representation (100-nanoseconds)
    //We also need to make it negative
    __int64 nFileTimes = static_cast<__int64>(-10000) * static_cast<__int64>(dwMilliseconds);
    LARGE_INTEGER li;
    li.LowPart = static_cast<DWORD>(nFileTimes & 0xFFFFFFFF);
    li.HighPart = static_cast<LONG>(nFileTimes >> 32); 
    Set(reinterpret_cast<FILETIME*>(&li), msPeriod, msWindowLength);
  }

__if_exists(SetThreadpoolTimerEx)
{
  BOOL SetEx(_In_opt_ PFILETIME pftDueTime, _In_ DWORD msPeriod, _In_opt_ DWORD msWindowLength)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer != nullptr);
  #endif //#ifdef _DEBUG

    return SetThreadpoolTimerEx(m_Timer, pftDueTime, msPeriod, msWindowLength);
  }

  BOOL CeaseEx()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer != nullptr);
  #endif //#ifdef _DEBUG

    return SetThreadpoolTimerEx(m_Timer, nullptr, 0, 0);
  }

  BOOL SetOnceOffAbsoluteEx(_In_ const SYSTEMTIME& DueTime, _In_opt_ DWORD msWindowLength)
  {
    //Convert from the SystemTime to the filetime struct
    FILETIME ft;
    if (!SystemTimeToFileTime(&DueTime, &ft))
      return FALSE;
    return SetEx(&ft, 0, msWindowLength);
  }

  BOOL SetOnceOffRelativeEx(_In_ DWORD dwMilliseconds, _In_opt_ DWORD msWindowLength)
  {
    //Convert from the milliseconds to FILETIME representation (100-nanoseconds)
    //We also need to make it negative
    __int64 nFileTimes = static_cast<__int64>(-10000) * static_cast<__int64>(dwMilliseconds);
    LARGE_INTEGER li;
    li.LowPart = static_cast<DWORD>(nFileTimes & 0xFFFFFFFF);
    li.HighPart = static_cast<LONG>(nFileTimes >> 32); 
    return SetEx(reinterpret_cast<FILETIME*>(&li), 0, msWindowLength);
  }
  
  BOOL SetPeriodicAbsoluteEx(_In_ const SYSTEMTIME& DueTime, _In_ DWORD msPeriod, _In_opt_ DWORD msWindowLength)
  {
    //Convert from the SystemTime to the FILETIME struct
    FILETIME ft;
    if (!SystemTimeToFileTime(&DueTime, &ft))
      return FALSE;
    return SetEx(&ft, msPeriod, msWindowLength);
  }
  
  BOOL SetPeriodicRelativeEx(_In_ DWORD dwMilliseconds, _In_ DWORD msPeriod, _In_opt_ DWORD msWindowLength)
  {
    //Convert from the milliseconds to FILETIME representation (100-nanoseconds)
    //We also need to make it negative
    __int64 nFileTimes = static_cast<__int64>(-10000) * static_cast<__int64>(dwMilliseconds);
    LARGE_INTEGER li;
    li.LowPart = static_cast<DWORD>(nFileTimes & 0xFFFFFFFF);
    li.HighPart = static_cast<LONG>(nFileTimes >> 32); 
    return SetEx(reinterpret_cast<FILETIME*>(&li), msPeriod, msWindowLength);
  }
}

  BOOL IsSet()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer != nullptr);
  #endif //#ifdef _DEBUG

    return IsThreadpoolTimerSet(m_Timer);
  }

  void WaitForCallbacks(_In_ BOOL fCancelPendingCallbacks)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Timer != nullptr);
  #endif //#ifdef _DEBUG

    return WaitForThreadpoolTimerCallbacks(m_Timer, fCancelPendingCallbacks);
  }

  PTP_TIMER get() const
  {
    return m_Timer;
  }

  bool GetAutoRelease() const { return m_bAutoRelease; };
  void SetAutoRelease(bool bAutoRelease) { m_bAutoRelease = bAutoRelease; };

protected:
//Member variables
  PTP_TIMER m_Timer;
  bool      m_bAutoRelease;
};


//RAII class wrapper for a PTP_WAIT
class CTHREADPOOLWRAPPERS_EXT_CLASS CWait
{
public:
//Constructors / Destructors
  CWait() : m_Wait(nullptr),
            m_bAutoRelease(true)
  {
  }

  CWait(_In_ const CWait&) = delete;

  CWait(_In_ CWait&& wait) : CWait()
  {
    Attach(wait.Detach(), wait.m_bAutoRelease);
  }

  explicit CWait(PTP_WAIT pWait, bool bAutoRelease) : m_Wait(pWait),
                                                      m_bAutoRelease(bAutoRelease)
  {
  }

  ~CWait()
  {
    if (m_Wait != nullptr)
      Close();
  }

//Methods
  CWait& operator=(_In_ const CWait&) = delete;

  CWait& operator=(_In_ CWait&& wait)
  {
    if (m_Wait != nullptr)
      Close();
    Attach(wait.Detach(), wait.m_bAutoRelease);
    return *this;
  }

  void Attach(_In_opt_ PTP_WAIT pWait, bool bAutoRelease)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Wait == nullptr);
  #endif //#ifdef _DEBUG

    m_Wait = pWait;
    m_bAutoRelease = bAutoRelease;
  }

  PTP_WAIT Detach()
  {
    PTP_WAIT pWait = m_Wait;
    m_Wait = nullptr;
    return pWait;
  }

  _Must_inspect_result_ bool Create(_In_ PTP_WAIT_CALLBACK pfnwa, _Inout_opt_ PVOID pv, _In_opt_ CEnvironment* pEnvironment)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Wait == nullptr);
  #endif //#ifdef _DEBUG

    m_Wait = CreateThreadpoolWait(pfnwa, pv, pEnvironment ? pEnvironment : nullptr);
    m_bAutoRelease = true;
    return (m_Wait != nullptr) ? true : false;
  }

  void Close()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Wait != nullptr);
  #endif //#ifdef _DEBUG

    if (m_bAutoRelease)
      CloseThreadpoolWait(m_Wait);
    m_Wait = nullptr;
  }

  void Set(_In_opt_ HANDLE h, _In_opt_ PFILETIME pftTimeout)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Wait != nullptr);
  #endif //#ifdef _DEBUG

    SetThreadpoolWait(m_Wait, h, pftTimeout);
  }

  void SetRelative(_In_opt_ HANDLE h, _In_ DWORD dwMilliseconds)
  {
    //Convert from the milliseconds to FILETIME representation (100-nanoseconds)
    //We also need to make it negative
    __int64 nFileTimes = static_cast<__int64>(-10000) * static_cast<__int64>(dwMilliseconds);
    LARGE_INTEGER li;
    li.LowPart = static_cast<DWORD>(nFileTimes & 0xFFFFFFFF);
    li.HighPart = static_cast<LONG>(nFileTimes >> 32);
    return Set(h, reinterpret_cast<FILETIME*>(&li));
  }

  BOOL SetAbsolute(_In_opt_ HANDLE h, _In_ const SYSTEMTIME& DueTime)
  {
    //Convert from the SystemTime to the FILETIME struct
    FILETIME ft;
    if (!SystemTimeToFileTime(&DueTime, &ft))
      return FALSE;
    Set(h, &ft);
    return TRUE;
  }

__if_exists(SetThreadpoolWaitEx)
{
  BOOL SetEx(_In_opt_ HANDLE h, _In_opt_ PFILETIME pftTimeout, _Reserved_ PVOID Reserved = nullptr)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Wait != nullptr);
  #endif //#ifdef _DEBUG

    return SetThreadpoolWaitEx(m_Wait, h, pftTimeout, Reserved);
  }

  BOOL SetExRelative(_In_opt_ HANDLE h, _In_ DWORD dwMilliseconds, _Reserved_ PVOID Reserved = nullptr)
  {
    //Convert from the milliseconds to FILETIME representation (100-nanoseconds)
    //We also need to make it negative
    __int64 nFileTimes = static_cast<__int64>(-10000) * static_cast<__int64>(dwMilliseconds);
    LARGE_INTEGER li;
    li.LowPart = static_cast<DWORD>(nFileTimes & 0xFFFFFFFF);
    li.HighPart = static_cast<LONG>(nFileTimes >> 32);
    return SetEx(h, reinterpret_cast<FILETIME*>(&li), Reserved);
  }

  BOOL SetExAbsolute(_In_opt_ HANDLE h, _In_ const SYSTEMTIME& DueTime, _Reserved_ PVOID Reserved = nullptr)
  {
    //Convert from the SystemTime to the FILETIME struct
    FILETIME ft;
    if (!SystemTimeToFileTime(&DueTime, &ft))
      return FALSE;
    return SetEx(h, &ft, Reserved);
  }
}

  void WaitForCallbacks(_In_ BOOL fCancelPendingCallbacks)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Wait != nullptr);
  #endif //#ifdef _DEBUG

    return WaitForThreadpoolWaitCallbacks(m_Wait, fCancelPendingCallbacks);
  }

  PTP_WAIT get() const
  {
    return m_Wait;
  }

  bool GetAutoRelease() const { return m_bAutoRelease; };
  void SetAutoRelease(bool bAutoRelease) { m_bAutoRelease = bAutoRelease; };

protected:
//Member variables
  PTP_WAIT m_Wait;
  bool     m_bAutoRelease;
};


//RAII class wrapper for a PTP_IO
class CTHREADPOOLWRAPPERS_EXT_CLASS CIO
{
public:
//Constructors / Destructors
  CIO() : m_Io(nullptr),
          m_bAutoRelease(true)
  {
  }

  CIO(_In_ const CIO&) = delete;

  CIO(_In_ CIO&& io) : CIO()
  {
    Attach(io.Detach(), io.m_bAutoRelease);
  }

  explicit CIO(PTP_IO pIO, bool bAutoRelease) : m_Io(pIO),
                                                m_bAutoRelease(bAutoRelease)
  {
  }

  ~CIO()
  {
    if (m_Io != nullptr)
      Close();
  }

//Methods
  CIO& operator=(_In_ const CIO&) = delete;

  CIO& operator=(_In_ CIO&& io)
  {
    if (m_Io != nullptr)
      Close();
    Attach(io.Detach(), io.m_bAutoRelease);
    return *this;
  }

  void Attach(_In_opt_ PTP_IO pIo, bool bAutoRelease)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Io == nullptr);
  #endif //#ifdef _DEBUG

    m_Io = pIo;
    m_bAutoRelease = bAutoRelease;
  }

  PTP_IO Detach()
  {
    PTP_IO pIo = m_Io;
    m_Io = nullptr;
    return pIo;
  }

  _Must_inspect_result_ bool Create(_In_ HANDLE fl, _In_ PTP_WIN32_IO_CALLBACK pfnio, _Inout_opt_ PVOID pv, _In_opt_ CEnvironment* pEnvironment)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Io == nullptr);
  #endif //#ifdef _DEBUG

    m_Io = CreateThreadpoolIo(fl, pfnio, pv, pEnvironment ? pEnvironment : nullptr);
    m_bAutoRelease = true;
    return (m_Io != nullptr) ? true : false;
  }

  void Close()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Io != nullptr);
  #endif //#ifdef _DEBUG

    if (m_bAutoRelease)
      CloseThreadpoolIo(m_Io);
    m_Io = nullptr;
  }

  void Start()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Io != nullptr);
  #endif //#ifdef _DEBUG

    StartThreadpoolIo(m_Io);
  }

  void Cancel()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Io != nullptr);
  #endif //#ifdef _DEBUG

    return CancelThreadpoolIo(m_Io);
  }

  void WaitForCallbacks(_In_ BOOL fCancelPendingCallbacks)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Io != nullptr);
  #endif //#ifdef _DEBUG

    return WaitForThreadpoolIoCallbacks(m_Io, fCancelPendingCallbacks);
  }

  PTP_IO get() const
  {
    return m_Io;
  }

  bool GetAutoRelease() const { return m_bAutoRelease; };
  void SetAutoRelease(bool bAutoRelease) { m_bAutoRelease = bAutoRelease; };

protected:
//Member variables
  PTP_IO m_Io;
  bool   m_bAutoRelease;
};


//RAII class wrapper for a PTP_WORK
class CTHREADPOOLWRAPPERS_EXT_CLASS CWork
{
public:
//Constructors / Destructors
  CWork() : m_Work(nullptr),
            m_bAutoRelease(true)
  {
  }

  CWork(_In_ const CWork&) = delete;

  CWork(_In_ CWork&& work) : CWork()
  {
    Attach(work.Detach(), work.m_bAutoRelease);
  }

  explicit CWork(PTP_WORK pWork, bool bAutoRelease) : m_Work(pWork),
                                                      m_bAutoRelease(bAutoRelease)
  {
  }

  ~CWork()
  {
    if (m_Work != nullptr)
      Close();
  }

//Methods
  CWork& operator=(_In_ const CWork&) = delete;

  CWork& operator=(_In_ CWork&& work)
  {
    if (m_Work != nullptr)
      Close();
    Attach(work.Detach(), work.m_bAutoRelease);
    return *this;
  }

  void Attach(_In_opt_ PTP_WORK pWork, bool bAutoRelease)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Work == nullptr);
  #endif //#ifdef _DEBUG

    m_Work = pWork;
    m_bAutoRelease = bAutoRelease;
  }

  PTP_WORK Detach()
  {
    PTP_WORK pWork = m_Work;
    m_Work = nullptr;
    return pWork;
  }

  _Must_inspect_result_ bool Create(_In_ PTP_WORK_CALLBACK pfnwk, _Inout_opt_ PVOID pv, _In_opt_ CEnvironment* pEnvironment)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Work == nullptr);
  #endif //#ifdef _DEBUG

    m_Work = CreateThreadpoolWork(pfnwk, pv, pEnvironment ? pEnvironment : nullptr);
    m_bAutoRelease = true;
    return (m_Work != nullptr) ? true : false;
  }

  void Close()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Work != nullptr);
  #endif //#ifdef _DEBUG

    if (m_bAutoRelease)
      CloseThreadpoolWork(m_Work);
    m_Work = nullptr;
  }

  void Submit()
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Work != nullptr);
  #endif //#ifdef _DEBUG

    SubmitThreadpoolWork(m_Work);
  }

  void WaitForCallbacks(_In_ BOOL fCancelPendingCallbacks)
  {
    //Validate our parameters
  #ifdef _DEBUG
    assert(m_Work != nullptr);
  #endif //#ifdef _DEBUG

    WaitForThreadpoolWorkCallbacks(m_Work, fCancelPendingCallbacks);
  }

  PTP_WORK get() const
  {
    return m_Work;
  }

  bool GetAutoRelease() const { return m_bAutoRelease; };
  void SetAutoRelease(bool bAutoRelease) { m_bAutoRelease = bAutoRelease; };

protected:
//Member variables
  PTP_WORK m_Work;
  bool     m_bAutoRelease;
};


//A concrete thread pool class which uses cleanup groups and can work with the built in thread pool or a freshly allocated thread pool.
//This implementation is partly based on the thread pool sample from http://www.dorodnic.com/blog/2015/10/17/windows-threadpool
class CThreadPool
{
public:
//Constructors / Destructors
  CThreadPool()
  {
  }

  CThreadPool(const CThreadPool&) = delete;

  CThreadPool(CThreadPool&&) = delete;

  virtual ~CThreadPool()
  {
    m_CleanupGroup.CloseMembers(FALSE, nullptr); //Wait for all previously scheduled tasks to complete
  }

//Methods
  BOOL Create(PVOID DllHandle = nullptr, PTP_CLEANUP_GROUP_CANCEL_CALLBACK cleanupCallback = nullptr)
  {
    //Create uses the default thread pool so no need to create a new CPool

    //1. Initialize the thread pool environment
    if (DllHandle != nullptr)
      m_Environment.SetCallbackLibrary(DllHandle);

    //2. Create the thread pool cleanup group
    if (!m_CleanupGroup.Create())
      return FALSE;
    m_Environment.SetCleanupGroup(m_CleanupGroup, cleanupCallback);

    return TRUE;
  }

  BOOL CreateNew(PVOID DllHandle = nullptr, int nMinThreads = -1, int nMaxThreads = -1, PTP_CLEANUP_GROUP_CANCEL_CALLBACK cleanupCallback = nullptr)
  {
    //1. Create the new thread pool
    if (!m_Pool.Create())
      return FALSE;

    //2. Initialize the thread pool environment
    m_Environment.SetPool(m_Pool);
    if (DllHandle != nullptr)
      m_Environment.SetCallbackLibrary(DllHandle);

    //3. Create the thread pool cleanup group
    if (!m_CleanupGroup.Create())
      return FALSE;
    m_Environment.SetCleanupGroup(m_CleanupGroup, cleanupCallback);

    //4. Setup the min and max threads if required
    if (nMinThreads != -1)
    {
      if (!m_Pool.SetThreadMinimum(nMinThreads))
        return FALSE;
    }
    if (nMaxThreads != -1)
      m_Pool.SetThreadMaximum(nMaxThreads);

    return TRUE;
  }

  CThreadPool& operator=(CThreadPool&&) = delete;

  bool SubmitWork(_In_ PTP_WORK_CALLBACK pfnwk, _In_opt_ PVOID pv, _Inout_ ThreadPoolWrappers::CWork& work)
  {
    if (!work.Create(pfnwk, pv, &m_Environment))
      return false;
    work.SetAutoRelease(false); //Auto release not necessary because we are using cleanup groups
    work.Submit();
    return true;
  }

  bool SubmitOnceOffAbsoluteTimer(_In_ PTP_TIMER_CALLBACK pfnti, _In_opt_ PVOID pv, _In_ const SYSTEMTIME& DueTime, _In_opt_ DWORD msWindowLength, _Inout_ ThreadPoolWrappers::CTimer& timer)
  {
    if (!timer.Create(pfnti, pv, &m_Environment))
      return false;
    timer.SetAutoRelease(false); //Auto release not necessary because we are using cleanup groups
    timer.SetOnceOffAbsolute(DueTime, msWindowLength);
    return true;
  }

  bool SubmitOnceOffRelativeTimer(_In_ PTP_TIMER_CALLBACK pfnti, _In_opt_ PVOID pv, _In_ DWORD dwMilliseconds, _In_opt_ DWORD msWindowLength, _Inout_ ThreadPoolWrappers::CTimer& timer)
  {
    if (!timer.Create(pfnti, pv, &m_Environment))
      return false;
    timer.SetAutoRelease(false); //Auto release not necessary because we are using cleanup groups
    timer.SetOnceOffRelative(dwMilliseconds, msWindowLength);
    return true;
  }

  bool SubmitPeriodicAbsoluteTimer(_In_ PTP_TIMER_CALLBACK pfnti, _In_opt_ PVOID pv, _In_ const SYSTEMTIME& DueTime, _In_ DWORD msPeriod, _In_opt_ DWORD msWindowLength, _Inout_ ThreadPoolWrappers::CTimer& timer)
  {
    if (!timer.Create(pfnti, pv, &m_Environment))
      return false;
    timer.SetAutoRelease(false); //Auto release not necessary because we are using cleanup groups
    return timer.SetPeriodicAbsolute(DueTime, msPeriod, msWindowLength);
  }

  bool SubmitPeriodicRelativeTimer(_In_ PTP_TIMER_CALLBACK pfnti, _In_opt_ PVOID pv, _In_ DWORD dwMilliseconds, _In_ DWORD msPeriod, _In_opt_ DWORD msWindowLength, _Inout_ ThreadPoolWrappers::CTimer& timer)
  {
    if (!timer.Create(pfnti, pv, &m_Environment))
      return false;
    timer.SetAutoRelease(false); //Auto release not necessary because we are using cleanup groups
    timer.SetPeriodicRelative(dwMilliseconds, msPeriod, msWindowLength);
    return true;
  }

  bool WaitForKernelObjectAbsolute(_In_ PTP_WAIT_CALLBACK pfnwa, _In_opt_ PVOID pv, _In_ HANDLE handle, _In_ const SYSTEMTIME& DueTime, _Inout_ ThreadPoolWrappers::CWait& wait)
  {
    if (!wait.Create(pfnwa, pv, &m_Environment))
      return false;
    wait.SetAutoRelease(false); //Auto release not necessary because we are using cleanup groups
    return wait.SetAbsolute(handle, DueTime);
  }

  bool WaitForKernelObjectRelative(_In_ PTP_WAIT_CALLBACK pfnwa, _In_opt_ PVOID pv, _In_ HANDLE handle, _In_ DWORD dwMilliseconds, _Inout_ ThreadPoolWrappers::CWait& wait)
  {
    if (!wait.Create(pfnwa, pv, &m_Environment))
      return false;
    wait.SetAutoRelease(false); //Auto release not necessary because we are using cleanup groups
    wait.SetRelative(handle, dwMilliseconds);
    return true;
  }

  bool SubmitIO(_In_ PTP_WIN32_IO_CALLBACK pfnio, _In_opt_ PVOID pv, _In_ HANDLE fl, _Inout_ ThreadPoolWrappers::CIO& io)
  {
    if (!io.Create(fl, pfnio, pv, &m_Environment))
      return false;
    io.SetAutoRelease(false); //Auto release not necessary because we are using cleanup groups
    io.Start();
    return true;
  }

//Member variables
  ThreadPoolWrappers::CEnvironment  m_Environment;
  ThreadPoolWrappers::CCleanupGroup m_CleanupGroup;
  ThreadPoolWrappers::CPool         m_Pool;
};


}; //namespace ThreadPoolWrappers

#endif //#ifndef __THREADPOOLWRAPPERS_H__

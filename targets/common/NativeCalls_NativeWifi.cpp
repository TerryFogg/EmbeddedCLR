//-----------------------------------------------------------------------------
//
//                   ** WARNING! **
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------

#include "NativeCalls.h"
#include "nanoCLR_Types.h"
#include "NativeNetwork.h"

HRESULT Library_NativeCalls_NativeWifi::Initialize___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        bool result = WiFi::Initialize();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeWifi::IsAvailable___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        bool result = WiFi::IsAvailable();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeWifi::Connect___STATIC__BOOLEAN__STRING__STRING__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *szSsid = stack.Arg1().RecoverString();
        const char *szPassword = stack.Arg2().RecoverString();
        CLR_RT_HeapBlock hbTimeout;
        hbTimeout.SetInteger((CLR_INT64)20000 * TIME_CONVERSION__TO_MILLISECONDS);

        WiFi::Connect(0, (char *)szSsid, (char *)szPassword);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeWifi::Disconnect___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        bool result = WiFi::Disconnect();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeWifi::IsConnected___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        bool result = WiFi::IsConnected();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeWifi::Enable___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        bool result = WiFi::Enable();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeWifi::Disable___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        bool result = WiFi::Disable();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeWifi::GetScanResults___STATIC__SZARRAY_U1(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        int numberOfRecords = WiFi::ScanToCache();
        CLR_RT_HeapBlock &top = stack.PushValueAndClear();
        CLR_RT_HeapBlock_Array::CreateInstance(top, numberOfRecords, g_CLR_RT_WellKnownTypes.m_UInt8);
        CLR_RT_HeapBlock_Array *array = top.DereferenceArray();
        CLR_UINT8 *packedScanReport = array->GetFirstElement();
        WiFi::GetScanResults(packedScanReport, numberOfRecords);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeWifi::GetRSSI___STATIC__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        bool result = WiFi::GetRSSI();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

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

#include "nanoFramework_NativeCalls.h"
#include "nanoCLR_Types.h"
#include "NativeNetwork.h"

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::Initialize___STATIC__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::Initialize();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::IsAvailable___STATIC__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::IsAvailable();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::
    Connect___STATIC__BOOLEAN__STRING__STRING__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        const char *szSsid = stack.Arg1().RecoverString();
        const char *szPassword = stack.Arg2().RecoverString();
        CLR_RT_HeapBlock hbTimeout;
        hbTimeout.SetInteger((CLR_INT64)20000 * TIME_CONVERSION__TO_MILLISECONDS);

        WiFi::Connect(0, (char *)szSsid, (char *)szPassword);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::Disconnect___STATIC__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::Disconnect();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::IsConnected___STATIC__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::IsConnected();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::Enable___STATIC__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::Enable();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::Disable___STATIC__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::Disable();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::StartScan___STATIC__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::StartScan();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::GetScanResults___STATIC__SZARRAY_U1(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::GetScanResults();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_nanoFramework_NativeCalls_nanoFramework_NativeNetwork_NativeWifi::GetRSSI___STATIC__I4__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        bool result = WiFi::GetRSSI();
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

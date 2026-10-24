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
#include "NativeNetwork.h"

HRESULT Library_NativeCalls_NativeDns::ResolveHost___STATIC__I4__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *host = stack.Arg0().RecoverString();
        CLR_RT_HeapBlock_Array *pAddress = stack.Arg1().DereferenceArray();
        FAULT_ON_NULL_ARG(pAddress);
        uint32_t address = 0;
        NativeSockets::ResolveHost(host, &address);
        stack.SetResult_Boolean(address);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

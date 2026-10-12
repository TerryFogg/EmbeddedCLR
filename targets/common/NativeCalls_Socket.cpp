//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//
#include "NativeNetwork.h"
#include "NativeCalls.h"

HRESULT Library_NativeCalls_Socket::Create___STATIC__I4__SocketProtocol(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        CLR_INT32 protocol = stack.Arg0().NumericByRef().s4;
        bool result = (NativeSockets::Create((SocketProtocol)protocol) > 0);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::Bind___STATIC__BOOLEAN__I4__U2(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        CLR_INT32 port = stack.Arg1().NumericByRef().s2;
        bool result = NativeSockets::Bind(socket, port);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::Connect___STATIC__BOOLEAN__I4__STRING__U2__I4(CLR_RT_StackFrame &stack)
{
    // args:
    // 0: socket (I4)
    // 1: host (STRING)
    // 2: port (U2)
    // 3: timeoutMs (I4)
    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        const char *host = stack.Arg1().RecoverString();
        CLR_INT32 port = stack.Arg2().NumericByRef().s4;
        CLR_INT32 timeoutMs = stack.Arg3().NumericByRef().s4;
        bool result = NativeSockets::Connect(socket, host, (uint16_t)port, timeoutMs);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::Listen___STATIC__BOOLEAN__I4__U2__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        CLR_INT32 port = stack.Arg1().NumericByRef().s2;
        CLR_INT32 maxConnections = stack.Arg1().NumericByRef().s4;
        bool result = NativeSockets::Listen(socket,port,maxConnections);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::Accept___STATIC__I4__I4__I4(
    CLR_RT_StackFrame &stack)
{
    // args:
    // 0: socket (I4)
    // 1: timeoutMs (I4)
    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        CLR_INT32 timeoutMs = stack.Arg1().NumericByRef().s4;
        int32_t result = NativeSockets::Accept(socket, timeoutMs);
        stack.SetResult_I4(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::Send___STATIC__I4__I4__SZARRAY_U1__I4__I4(CLR_RT_StackFrame &stack)
{
    // args:
    // 0: socket (I4)
    // 1: data (SZARRAY_U1)
    // 2: offset (I4)
    // 3: length (I4)
    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        CLR_RT_HeapBlock_Array *pData = stack.Arg1().DereferenceArray();
        CLR_INT32 offset = stack.Arg2().NumericByRef().s4;
        CLR_INT32 length = stack.Arg3().NumericByRef().s4;
        uint8_t *data = (uint8_t *)pData->GetFirstElement();
        int32_t result = NativeSockets::Send(socket, data, offset, length);
        stack.SetResult_I4(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::SendTo___STATIC__I4__I4__STRING__U2__SZARRAY_U1__I4__I4(CLR_RT_StackFrame &stack)
{
    // args:
    // 0: socket (I4)
    // 1: host (STRING)
    // 2: port (U2)
    // 3: data (SZARRAY_U1)
    // 4: offset (I4)
    // 5: length (I4)
    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        const char *host = stack.Arg1().RecoverString();
        CLR_INT32 port = stack.Arg2().NumericByRef().s4;
        CLR_RT_HeapBlock_Array *pData = stack.Arg3().DereferenceArray();
        CLR_INT32 offset = stack.Arg4().NumericByRef().s4;
        CLR_INT32 length = stack.Arg5().NumericByRef().s4;
        uint8_t *data = (uint8_t *)pData->GetFirstElement();
        int32_t result = NativeSockets::SendTo(socket, host, (uint16_t)port, data, offset, length);
        stack.SetResult_I4(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::Receive___STATIC__I4__I4__SZARRAY_U1__I4__I4__I4(CLR_RT_StackFrame &stack)
{
    // args:
    // 0: socket (I4)
    // 1: buffer (SZARRAY_U1)
    // 2: offset (I4)
    // 3: length (I4)
    // 4: timeoutMs (I4)

    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        CLR_RT_HeapBlock_Array *pBuffer = stack.Arg1().DereferenceArray();
        CLR_INT32 offset = stack.Arg2().NumericByRef().s4;
        CLR_INT32 length = stack.Arg3().NumericByRef().s4;
        CLR_INT32 timeoutMs = stack.Arg4().NumericByRef().s4;
        uint8_t *buffer = (uint8_t *)pBuffer->GetFirstElement();
        int32_t result = NativeSockets::Receive(socket, buffer, offset, length, timeoutMs);
        stack.SetResult_I4(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::ReceiveFrom___STATIC__I4__I4__SZARRAY_U1__I4__I4__I4__SZARRAY_U1__SZARRAY_U1(CLR_RT_StackFrame &stack)
{
    // args:
    // 0: socket (I4)
    // 1: buffer (SZARRAY_U1)
    // 2: offset (I4)
    // 3: length (I4)
    // 4: timeoutMs (I4)
    // 5: remoteAddress (SZARRAY_U1) - expected length >= 4
    // 6: remotePort (SZARRAY_U1) - expected length >= 2

    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        CLR_RT_HeapBlock_Array *pBuffer = stack.Arg1().DereferenceArray();
        CLR_INT32 offset = stack.Arg2().NumericByRef().s4;
        CLR_INT32 length = stack.Arg3().NumericByRef().s4;
        CLR_INT32 timeoutMs = stack.Arg4().NumericByRef().s4;
        CLR_RT_HeapBlock_Array *pRemoteAddress = stack.Arg5().DereferenceArray();
        CLR_RT_HeapBlock_Array *pRemotePort = stack.Arg6().DereferenceArray();
        uint8_t *buffer = (uint8_t *)pBuffer->GetFirstElement();
        uint8_t *remoteAddress = (uint8_t *)pRemoteAddress->GetFirstElement();
        uint8_t *remotePort = (uint8_t *)pRemotePort->GetFirstElement();
        int32_t result =
            NativeSockets::ReceiveFrom(socket, buffer, offset, length, timeoutMs, remoteAddress, remotePort);
        stack.SetResult_I4(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::IsConnected___STATIC__BOOLEAN__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        bool result = NativeSockets::IsConnected(socket);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Socket::Close___STATIC__BOOLEAN__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        CLR_INT32 socket = stack.Arg0().NumericByRef().s4;
        bool result = NativeSockets::Close(socket);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

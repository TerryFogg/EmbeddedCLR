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

#include "NativeNetwork.h"
#include "NativeCalls.h"

HRESULT Library_NativeCalls_Bluetooth::Enable___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        stack.SetResult_Boolean(Bluetooth::Enable());
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::Disable___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        stack.SetResult_Boolean(Bluetooth::Disable());
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::GetState___STATIC__BluetoothState(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        stack.SetResult_I4((int32_t)Bluetooth::GetState());
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::StartScan___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        stack.SetResult_Boolean(Bluetooth::StartScan());
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::StopScan___STATIC__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        stack.SetResult_Boolean(Bluetooth::StopScan());
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::DeviceCount___STATIC__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        stack.SetResult_I4(Bluetooth::GetDeviceCount());
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::GetDeviceAddress___STATIC__STRING__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        int32_t index = stack.Arg0().NumericByRef().s4;

        LPCSTR address = Bluetooth::GetDeviceAddress(index);

        NANOCLR_CHECK_HRESULT(
            CLR_RT_HeapBlock_String::CreateInstance(stack.PushValueAndClear(), (address != nullptr) ? address : ""));
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::GetDeviceName___STATIC__STRING__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        int32_t index = stack.Arg0().NumericByRef().s4;
        LPCSTR name = Bluetooth::GetDeviceName(index);
        NANOCLR_CHECK_HRESULT(CLR_RT_HeapBlock_String::CreateInstance(stack.PushValueAndClear(), (name != nullptr) ? name : ""));
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::Connect___STATIC__BOOLEAN__STRING__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        LPCSTR address = stack.Arg0().RecoverString();
        int32_t timeoutMs = stack.Arg1().NumericByRef().s4;
        stack.SetResult_Boolean(Bluetooth::Connect(address, timeoutMs));
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::Disconnect___STATIC__BOOLEAN__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        LPCSTR address = stack.Arg0().RecoverString();
        stack.SetResult_Boolean(Bluetooth::Disconnect(address));
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::ReadCharacteristic___STATIC__I4__STRING__STRING__SZARRAY_U1(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        LPCSTR deviceAddress = stack.Arg0().RecoverString();
        LPCSTR characteristicUuid = stack.Arg1().RecoverString();
        CLR_RT_HeapBlock_Array *bufferArray = stack.Arg2().DereferenceArray();
        if (bufferArray == nullptr)
        {
            stack.SetResult_I4(0);
            NANOCLR_SET_AND_LEAVE(S_OK);
        }
        uint8_t *buffer = (uint8_t *)bufferArray->GetFirstElement();
        uint32_t length = bufferArray->m_numOfElements;
        int32_t bytesRead = Bluetooth::ReadCharacteristic(deviceAddress, characteristicUuid, buffer, length);
        stack.SetResult_I4(bytesRead);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_Bluetooth::WriteCharacteristic___STATIC__BOOLEAN__STRING__STRING__SZARRAY_U1(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        LPCSTR deviceAddress = stack.Arg0().RecoverString();
        LPCSTR characteristicUuid = stack.Arg1().RecoverString();
        CLR_RT_HeapBlock_Array *bufferArray = stack.Arg2().DereferenceArray();
        if (bufferArray == nullptr)
        {
            stack.SetResult_Boolean(false);
            NANOCLR_SET_AND_LEAVE(S_OK);
        }
        uint8_t *buffer = (uint8_t *)bufferArray->GetFirstElement();
        uint32_t length = bufferArray->m_numOfElements;
        bool result = Bluetooth::WriteCharacteristic(deviceAddress, characteristicUuid, buffer, length);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

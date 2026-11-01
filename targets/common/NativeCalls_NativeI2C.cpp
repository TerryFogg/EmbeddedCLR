//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "NativeCalls.h"
#include "NativeIO.h"

HRESULT Library_NativeCalls_NativeI2C::Probe___STATIC__BOOLEAN__I4__U2__I4(CLR_RT_StackFrame &stack)
{
    // C# signature:
    // public static extern bool Probe(int Bus, ushort SlaveAddress, int TimeoutMilliseconds);

    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        int timeoutMs = 10;
        int bus = stack.Arg0().NumericByRef().s4;
        int slaveAddress = stack.Arg1().NumericByRef().u2;
        bool result = I2cIO::Probe(bus, slaveAddress, timeoutMs);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeI2C::Open___STATIC__BOOLEAN__I4__I4__I4(CLR_RT_StackFrame &stack)
{
    // C# signature:
    // public static extern bool Open(int Bus, int Sda, int Scl);

    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());    
        int bus = stack.Arg0().NumericByRef().s4;
        int pinSDA = stack.Arg1().NumericByRef().s4;
        int pinSCL = stack.Arg2().NumericByRef().s4;
        I2cIO::Initialize(bus, pinSDA, pinSCL);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeI2C::AddSlaveDevice___STATIC__BOOLEAN__I4__BusSpeed__U2(CLR_RT_StackFrame &stack)
{
    // C# signature:
    // public static extern bool AddSlaveDevice(int Bus, BusSpeed I2C_speed, ushort SlaveAddress);

    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        int bus = stack.Arg0().NumericByRef().s4;
        BusSpeed I2C_speed = (BusSpeed)stack.Arg1().NumericByRef().s4;
        unsigned short slaveAddress = stack.Arg2().NumericByRef().s4;
        I2cIO::AddDevice(bus, I2C_speed, slaveAddress);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeI2C::Write___STATIC__BOOLEAN__U2__SZARRAY_U1__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        // C# signature:
        // public static extern bool Write( ushort SlaveAddress, byte[] WriteBuffer, int WriteSize);

        FAULT_ON_NULL(stack.This());
        int timeoutMs = 10;
        int slaveAddress = stack.Arg0().NumericByRef().u2;
        CLR_RT_HeapBlock_Array *dataBuffer = stack.Arg1().DereferenceArray();
        uint8_t *writeBuffer = dataBuffer->GetElement(0);
        int writeSize = stack.Arg2().NumericByRef().s4;
        bool result = I2cIO::Write(slaveAddress, writeBuffer, writeSize, timeoutMs);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeI2C::Read___STATIC__BOOLEAN__U2__SZARRAY_U1__I4(CLR_RT_StackFrame &stack)
{
    // C# signature:
    // "Read(int Bus, ushort SlaveAddress, byte[] ReadBuffer, int ReadSize)"

    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        int timeoutMs = 10;
        int slaveAddress = stack.Arg0().NumericByRef().u2;
        CLR_RT_HeapBlock_Array *dataBuffer = stack.Arg1().DereferenceArray();
        uint8_t *readBuffer = dataBuffer->GetElement(0);
        int readSize = stack.Arg2().NumericByRef().s4;
        bool result = I2cIO::Read(slaveAddress, readBuffer, readSize, timeoutMs);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeI2C::WriteRead___STATIC__BOOLEAN__U2__SZARRAY_U1__I4__SZARRAY_U1__I4( CLR_RT_StackFrame &stack)
{
    // C# signature:
    // "WriteRead(int Bus, ushort SlaveAddress, byte[] WriteBuffer, int WriteSize, byte[] ReadBuffer, int ReadSize)"

    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This());
        int timeoutMs = 10;
        int slaveAddress = stack.Arg0().NumericByRef().u2;
        CLR_RT_HeapBlock_Array *writeDataBuffer = stack.Arg1().DereferenceArray();
        uint8_t *writeBuffer = writeDataBuffer->GetElement(0);
        int writeSize = stack.Arg2().NumericByRef().s4;
        CLR_RT_HeapBlock_Array *readDataBuffer = stack.Arg3().DereferenceArray();
        uint8_t *readBuffer = readDataBuffer->GetElement(0);
        int readSize = stack.Arg4().NumericByRef().s4;
        bool result = I2cIO::WriteRead(slaveAddress, writeBuffer, writeSize, readBuffer, readSize, timeoutMs);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

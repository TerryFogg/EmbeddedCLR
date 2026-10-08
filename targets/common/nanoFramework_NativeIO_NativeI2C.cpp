//
// Copyright (c) .NET Foundation and Contributors
// See LICENSE file in the project root for full license information.
//

#include "nanoFramework_NativeIO.h"
#include "NativeIO.h"

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeI2C::Open___STATIC__BOOLEAN__I4__I4__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        int i2c_bus_number_number = stack.Arg0().NumericByRef().s4;
        int pinSDA = stack.Arg1().NumericByRef().s4;
        int pinSCL = stack.Arg2().NumericByRef().s4;
        I2cIO::Initialize(i2c_bus_number_number, pinSDA, pinSCL);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeI2C::AddSlaveDevice___STATIC__BOOLEAN__I4__nanoFrameworkNativeIOBusSpeed__U2(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        int i2c_bus_number_number = stack.Arg0().NumericByRef().s4;
        BusSpeed I2C_speed = (BusSpeed)stack.Arg1().NumericByRef().s4;
        unsigned short slaveAddress = stack.Arg2().NumericByRef().s4;
        I2cIO::AddDevice(i2c_bus_number_number, I2C_speed, slaveAddress);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeI2C::Probe___STATIC__BOOLEAN__I4__U2__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        I2cIO::Initialize(
            stack.Arg0().NumericByRef().s4,
            stack.Arg1().NumericByRef().s4,
            stack.Arg2().NumericByRef().s4);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeI2C::
    Write___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        I2cIO::Initialize(
            stack.Arg0().NumericByRef().s4,
            stack.Arg1().NumericByRef().s4,
            stack.Arg2().NumericByRef().s4);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeI2C::Read___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        I2cIO::Initialize(
            stack.Arg0().NumericByRef().s4,
            stack.Arg1().NumericByRef().s4,
            stack.Arg2().NumericByRef().s4);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

HRESULT Library_nanoFramework_NativeIO_nanoFramework_NativeIO_NativeI2C::
    WriteRead___STATIC__BOOLEAN__I4__U2__SZARRAY_U1__I4__SZARRAY_U1__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        I2cIO::Initialize(
            stack.Arg0().NumericByRef().s4,
            stack.Arg1().NumericByRef().s4,
            stack.Arg2().NumericByRef().s4);
    }
    NANOCLR_NOCLEANUP_NOLABEL();
}

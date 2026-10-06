# nanoCLR native interop — one‑page cheat‑sheet

Quick reference for accessing arguments, primitives, strings, arrays and returning results
in native methods that use CLR_RT_StackFrame / CLR_RT_HeapBlock.

--

Usage basics
- Arguments: stack.Arg0(), stack.Arg1(), ...
- Instance: stack.This()
- Create return object slot: stack.PushValueAndClear()

Numeric access (use NumericByRef())
- 32-bit signed:    .NumericByRef().s4   (CLR_INT32)
- 32-bit unsigned:  .NumericByRef().u4   (CLR_UINT32)
- 64-bit signed:    .NumericByRef().s8   (CLR_INT64)
- 8-bit unsigned:   .NumericByRef().u1   (byte / uint8_t)
- float (single):   .NumericByRef().r4
- double:           .NumericByRef().r8

Strings
- Read managed string: const char *s = stack.ArgN().RecoverString();
  - RecoverString() returns UTF-8 const char*; check for NULL (use FAULT_ON_NULL()).
- Return string: NANOCLR_CHECK_HRESULT(stack.SetResult_String(cstr));

Objects & arrays
- Dereference object:    CLR_RT_HeapBlock *obj = heapBlock.Dereference();
- Dereference array:     CLR_RT_HeapBlock_Array *arr = heapBlock.DereferenceArray();
- Span/SpanByte pattern:
  - CLR_RT_HeapBlock *span = stack.ArgN().Dereference();
  - CLR_RT_HeapBlock_Array *arr = span[SpanByte::FIELD___array].DereferenceArray();
  - int start  = span[SpanByte::FIELD___start].NumericByRef().s4;
  - int length = span[SpanByte::FIELD___length].NumericByRef().s4;
  - void *elemPtr = arr->GetElement(start);
  - Copy with memcpy(elemPtr, localBuf, length) or vice-versa.

Return helpers (common SetResult_* helpers)
- stack.SetResult_I4(int32)
- stack.SetResult_I8(int64)
- stack.SetResult_Boolean(bool)
- stack.SetResult_R4(float)
- stack.SetResult_R8(double)
- stack.SetResult_String(const char*)
- stack.SetResult_SZARRAY_U1(...)  // return byte[]

Safety & patterns
- Always FAULT_ON_NULL(ptr) after RecoverString() or Dereference() when null is invalid.
- Check disposed flags in instance methods: pThis[FIELD___disposedValue].NumericByRef().u1
- Do NOT keep pointers to managed memory across async/DMA operations. Copy data into native buffers
  before starting hardware transfers.
- Use NANOCLR_CHECK_HRESULT / NANOCLR_SET_AND_LEAVE for error handling.
- Use stack.SetupTimeoutFromTicks / g_CLR_RT_ExecutionEngine.WaitEvents for waits that must
  allow other CLR threads to run.

Quick examples
- Read int arg:
  - CLR_INT32 val = stack.Arg1().NumericByRef().s4;
- Read string arg:
  - const char *s = stack.Arg1().RecoverString(); FAULT_ON_NULL(s);
- Return bool:
  - stack.SetResult_Boolean(resultBool);
- Read SpanByte and copy:
  - CLR_RT_HeapBlock *span = stack.Arg1().Dereference();
  - CLR_RT_HeapBlock_Array *arr = span[SpanByte::FIELD___array].DereferenceArray();
  - int off = span[SpanByte::FIELD___start].NumericByRef().s4;
  - int len = span[SpanByte::FIELD___length].NumericByRef().s4;
  - memcpy(localBuf, arr->GetElement(off), len);

References (examples in repo)
- GPIO: src/System.Device.Gpio/sys_dev_gpio_native_System_Device_Gpio_GpioController.cpp
- I2C (SpanBytes): targets/ESP32/_nanoCLR/System.Device.I2c/sys_dev_i2c_native_System_Device_I2c_I2cDevice.cpp
- WiFi strings: targets/ESP32/_nanoCLR/System.Device.Wifi/sys_dev_wifi_native_System_Device_Wifi_WifiAdapter.cpp

--
Generated: concise one‑page cheat‑sheet for quick printing / paste into editor.

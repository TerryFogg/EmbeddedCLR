#include "NativeCalls.h"
#include "NativeFile.h"


HRESULT Library_NativeCalls_NativeStorageSystem::Exists___STATIC__BOOLEAN__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        bool result = File::Exists(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::Delete___STATIC__BOOLEAN__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        bool result = File::Delete(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::Move___STATIC__BOOLEAN__STRING__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *source = stack.Arg0().RecoverString();
        const char *destination = stack.Arg1().RecoverString();
        bool result = File::Move(source, destination);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::Length___STATIC__I8__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        CLR_INT64 length = File::Length(path);
        stack.SetResult_I8(length);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::Read___STATIC__I4__STRING__SZARRAY_U1__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        CLR_RT_HeapBlock_Array *pArray = stack.Arg1().DereferenceArray();
        int offset = stack.Arg2().NumericByRef().s4;
        int count = stack.Arg3().NumericByRef().s4;
        File::Read(path, (CLR_UINT8 *)pArray->GetFirstElement(), offset, count);
        stack.SetResult_I4(0);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::Write___STATIC__I4__STRING__SZARRAY_U1__I4__I4__BOOLEAN(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        CLR_RT_HeapBlock_Array *pArray = stack.Arg1().DereferenceArray();
        int offset = stack.Arg2().NumericByRef().s4;
        int count = stack.Arg3().NumericByRef().s4;
        bool append = stack.Arg4().NumericByRef().s4;
        int32_t bytesWritten = File::Write(path, (CLR_UINT8 *)pArray->GetFirstElement(), offset, count, append);
        stack.SetResult_I4(bytesWritten);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::CreateDirectory___STATIC__BOOLEAN__STRING( CLR_RT_StackFrame &stack )
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        bool result = File::Delete(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::DeleteDirectory___STATIC__BOOLEAN__STRING( CLR_RT_StackFrame &stack )
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        bool result = File::Delete(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::DirectoryExists___STATIC__BOOLEAN__STRING( CLR_RT_StackFrame &stack )
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        bool result = File::Exists(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::FileCount___STATIC__I4__STRING( CLR_RT_StackFrame &stack )
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        int fileCount = File::FileCount(path);
        stack.SetResult_I4(fileCount);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::GetFileName___STATIC__STRING__STRING__I4( CLR_RT_StackFrame &stack )
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        int index = stack.Arg1().NumericByRef().s4;
        char fileName[64];
        int maxLength = 64;
        if (File::GetFileName(path, index, fileName, maxLength))
        {
            stack.SetResult_String(fileName);
        }
        else
        {
            stack.SetResult_String("/0");
        }
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::DirectoryCount___STATIC__I4__STRING( CLR_RT_StackFrame &stack )
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        int fileCount = File::FileCount(path);
        stack.SetResult_I4(fileCount);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeStorageSystem::GetDirectoryName___STATIC__STRING__STRING__I4( CLR_RT_StackFrame &stack )
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        int index = stack.Arg1().NumericByRef().s4;
        char directoryName[64];
        int maxLength = 64;
        if (File::GetFileName(path, index, directoryName, maxLength))
        {
            stack.SetResult_String(directoryName);
        }
        else
        {
            stack.SetResult_String("/0");
        }
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

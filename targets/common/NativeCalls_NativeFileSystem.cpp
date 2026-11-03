#include "NativeCalls.h"
#include "NativeFile.h"
#include "ff.h"

HRESULT Library_NativeCalls_NativeFileSystem::
    FileOpen___STATIC__I4__STRING__EmbeddedCLRIOFileMode__EmbeddedCLRIOFileAccess(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        const char *path = stack.Arg0().RecoverString();
        int mode = stack.Arg1().NumericByRef().s4;
        int access = stack.Arg2().NumericByRef().s4;

        int32_t fileHandle = File::FileOpen(path, mode, access);
        if (fileHandle != -1)
        {
            stack.SetResult_I4(fileHandle);
        }
        else
        {
            NANOCLR_SET_AND_LEAVE(CLR_E_FILE_IO);
        }
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileClose___STATIC__BOOLEAN__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        bool result = File::FileClose(file);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileFlush___STATIC__BOOLEAN__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        bool result = File::FileFlush(file);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileCreate___STATIC__BOOLEAN__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        int32_t fileHandle = File::FileOpen(path, 1, 2);
        if (fileHandle != -1)
        {
            stack.SetResult_I4(fileHandle);
        }
        else
        {
            NANOCLR_SET_AND_LEAVE(CLR_E_FILE_IO);
        }
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileDelete___STATIC__BOOLEAN__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        bool result = File::Delete(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_SET_AND_LEAVE(S_OK);
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileExists___STATIC__BOOLEAN__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        bool result = File::FileExists(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}


HRESULT Library_NativeCalls_NativeFileSystem::FileCount___STATIC__I4__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        int fileCount = File::FileCount(path);
        stack.SetResult_I4(fileCount);
    }
    NANOCLR_NOCLEANUP();
}
HRESULT Library_NativeCalls_NativeFileSystem::FileMove___STATIC__BOOLEAN__STRING__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *source = stack.Arg0().RecoverString();
        const char *destination = stack.Arg1().RecoverString();
        bool result = File::FileMove(source, destination);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileRename___STATIC__BOOLEAN__STRING__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *oldName = stack.Arg0().RecoverString();
        const char *newName = stack.Arg1().RecoverString();
        bool result = File::FileRename(oldName, newName);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileCopy___STATIC__BOOLEAN__STRING__STRING__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *FromName = stack.Arg0().RecoverString();
        const char *ToName = stack.Arg1().RecoverString();
        bool overwrite = stack.Arg2().NumericByRef().s4;
        bool result = File::FileCopy(FromName, ToName, overwrite);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileSetLength___STATIC__BOOLEAN__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        int length = stack.Arg1().NumericByRef().s4;
        bool result = File::FileSetLength(file, length);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileGetLength___STATIC__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        CLR_INT32 length = File::FileGetLength(file);
        stack.SetResult_I4(length);
    }
    NANOCLR_NOCLEANUP();
}
HRESULT Library_NativeCalls_NativeFileSystem::FileGetLength___STATIC__I4__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        CLR_INT32 length = File::FileGetLength(path);
        stack.SetResult_I4(length);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileGetPosition___STATIC__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        CLR_INT32 length = File::FileGetPosition(file);
        stack.SetResult_I4(length);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileRead___STATIC__I4__I4__SZARRAY_U1__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        CLR_RT_HeapBlock_Array *pArray = stack.Arg1().DereferenceArray();
        CLR_UINT8 *buffer = (CLR_UINT8 *)pArray->GetFirstElement();
        int offset = stack.Arg2().NumericByRef().s4;
        int length = stack.Arg3().NumericByRef().s4;

        CLR_INT32 bytesRead = File::FileRead(file, buffer, offset, length);
        stack.SetResult_I4(bytesRead);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileWrite___STATIC__I4__I4__SZARRAY_U1__I4__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        CLR_RT_HeapBlock_Array *pArray = stack.Arg1().DereferenceArray();
        CLR_UINT8 *buffer = (CLR_UINT8 *)pArray->GetFirstElement();
        int offset = stack.Arg2().NumericByRef().s4;
        int count = stack.Arg3().NumericByRef().s4;
        bool append = stack.Arg4().NumericByRef().s4;
        int32_t bytesWritten = File::FileWrite(file, buffer, offset, count, append);
        stack.SetResult_I4(bytesWritten);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileSeek___STATIC__I4__I4__I4__EmbeddedCLRIOSeekOrigin(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        int position = stack.Arg1().NumericByRef().s4;
        bool result = File::FileSeek(file, position);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileGetLastWriteTime___STATIC__I8__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        bool result = File::FileGetLastWriteTime(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileGetAttributes___STATIC__I4__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        CLR_INT32 length = File::FileGetAttributes(file);
        stack.SetResult_I4(length);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::FileSetAttributes___STATIC__BOOLEAN__STRING__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        FIL *file = (FIL *)stack.Arg0().NumericByRef().s4;
        int attributes = stack.Arg1().NumericByRef().s4;
        CLR_INT32 length = File::FileSetAttributes(file, attributes);
        stack.SetResult_I4(length);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DirectoryCreate___STATIC__BOOLEAN__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        bool result = File::CreateDirectory(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DirectoryDelete___STATIC__BOOLEAN__STRING__BOOLEAN(
    CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        bool result = File::RemoveDirectory(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DirectoryExists___STATIC__BOOLEAN__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        bool result = File::DirectoryExists(path);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DirectoryMove___STATIC__BOOLEAN__STRING__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *source = stack.Arg0().RecoverString();
        const char *destination = stack.Arg1().RecoverString();
        bool result = File::DirectoryMove(source, destination);
        stack.SetResult_Boolean(result);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DirectoryGetFileName___STATIC__STRING__STRING__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        int index = stack.Arg1().NumericByRef().s4;
        char fileName[64];
        int maxLength = 64;
        if (File::DirectoryGetFileName(path, index, fileName, maxLength))
        {
            stack.SetResult_String(fileName);
        }
        else
        {
            stack.SetResult_String("/0");
        }
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DirectoryCount___STATIC__I4__STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        int fileCount = File::DirectoryCount(path);
        stack.SetResult_I4(fileCount);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DriveTotalSize___STATIC__I8_STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        CLR_INT64 totalSize = File::DriveTotalSize(path);
        stack.SetResult_I8(totalSize);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DriveFreeSpace___STATIC__I8_STRING(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        CLR_INT64 freeSpace = File::DriveFreeSpace(path);
        stack.SetResult_I8(freeSpace);
    }
    NANOCLR_NOCLEANUP();
}

HRESULT Library_NativeCalls_NativeFileSystem::DirectoryGetName___STATIC__STRING__STRING__I4(CLR_RT_StackFrame &stack)
{
    NANOCLR_HEADER();
    {
        FAULT_ON_NULL(stack.This())
        const char *path = stack.Arg0().RecoverString();
        int index = stack.Arg1().NumericByRef().s4;
        char directoryName[64];
        int maxLength = 64;
        if (File::DirectoryGetName(path, index, directoryName, maxLength))
        {
            stack.SetResult_String(directoryName);
        }
        else
        {
            stack.SetResult_String("/0");
        }
    }
    NANOCLR_NOCLEANUP();
}

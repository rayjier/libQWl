#pragma once
#include <vector>
#include <afxwin.h>

#define NameSpaceBegin(name) namespace name {
#define NameSpaceEnd(name) }
#define FREE(p) do{if(p!=NULL)free(p);p=NULL;}while(0)

LIB_COMMON_API DWORD ExecuteProgram(LPCTSTR cmd,LPCTSTR argu,CString& stdout_string);
LIB_COMMON_API HWND GetMainWindowHwnd();
LIB_COMMON_API CString IniReadStringValue(LPCTSTR filename,LPCTSTR section,LPCTSTR key,LPCTSTR default_value);
LIB_COMMON_API DWORD IniReadStringValue(LPCTSTR filename,LPCTSTR section,LPCTSTR key,LPSTR value,LPCTSTR default_value);
LIB_COMMON_API int IniReadIntValue(LPCTSTR filename,LPCTSTR section,LPCTSTR key,int default_value=0);
LIB_COMMON_API BOOL IniWriteStringValue(LPCTSTR filename,LPCTSTR section,LPCTSTR key,LPCTSTR format,...);

NameSpaceBegin(Path)
LIB_COMMON_API CString Combine(LPCTSTR path1,LPCTSTR path2);
LIB_COMMON_API CString GetAppConfigFile(LPCTSTR filename="config.ini");
LIB_COMMON_API CString GetAppDirectory();
LIB_COMMON_API CString GetAppFullPath();
LIB_COMMON_API CString GetAppFileName();
LIB_COMMON_API CString RemoveFileName(LPCTSTR path);
LIB_COMMON_API CString GetFileName(LPCTSTR path);
NameSpaceEnd(Path)

NameSpaceBegin(Text)
LIB_COMMON_API CString Format(LPCTSTR format,...);
LIB_COMMON_API int Explode(LPCTSTR text,LPCTSTR seperator,std::vector<CString>& arr);
LIB_COMMON_API bool IsNumber(LPCTSTR string_);
LIB_COMMON_API bool IsFloat(LPCTSTR string_);
NameSpaceEnd(Text)
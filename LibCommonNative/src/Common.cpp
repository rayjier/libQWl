#include "stdafx.h"
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#pragma warning( disable : 4996 )

LIB_COMMON_API DWORD ExecuteProgram(LPCTSTR cmd,LPCTSTR argu,CString& stdout_string)  
{  
	HWND hwnd = NULL;
	DWORD exit_code = -1;

	char buffer[1024]; 
	STARTUPINFO si;  
	PROCESS_INFORMATION pi;  
	HANDLE hRead, hWrite;  
	BOOL succeed;  
	SECURITY_ATTRIBUTES sa;  

	stdout_string.Empty();
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
	sa.bInheritHandle = TRUE; 
	sa.lpSecurityDescriptor = NULL;  
	succeed = CreatePipe(&hRead, &hWrite, &sa, 1024);  
	if (!succeed)  
	{  
		return GetLastError();
	}  

	memset(&si, 0, sizeof(si));  
	si.cb = sizeof(si);  
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;  
	si.wShowWindow = SW_HIDE;  
	si.hStdOutput = hWrite; 

	if (!CreateProcess(NULL, (LPSTR)cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))  
	{    
		CloseHandle(hRead);  
		CloseHandle(hWrite);  
		return GetLastError();
	}  

	CloseHandle(hWrite);  

	DWORD dwReaded;

	while (ReadFile(hRead, buffer, 255, &dwReaded, NULL))  
	{  
		if (dwReaded == 0) break;  
		buffer[dwReaded] = '\0';
		stdout_string += buffer;
		OutputDebugString(buffer);
	}  
	WaitForSingleObject( pi.hProcess, INFINITE );  

	if (!GetExitCodeProcess(pi.hProcess, &exit_code))  
		exit_code = GetLastError();  

	CloseHandle(hRead); 
	return exit_code;
}
static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) 
{ 
	DWORD dwCurProcessId = *((DWORD*)lParam); 
	DWORD dwProcessId = 0; 

	GetWindowThreadProcessId(hwnd, &dwProcessId); 
	if(dwProcessId == dwCurProcessId && GetParent(hwnd) == NULL)
	{ 
		*((HWND *)lParam) = hwnd;
		return FALSE; 
	} 
	return TRUE; 
} 

LIB_COMMON_API HWND GetMainWindowHwnd()
{
	DWORD dwCurrentProcessId = GetCurrentProcessId();
	if(!EnumWindows(EnumWindowsProc,(LPARAM)&dwCurrentProcessId)) 
	{     
		return (HWND)dwCurrentProcessId; 
	} 
	return NULL; 
}

LIB_COMMON_API CString IniReadStringValue(LPCTSTR filename,LPCTSTR section,LPCTSTR key,LPCTSTR default_value)
{
	CString value;
	::GetPrivateProfileString(section,key,default_value,value.GetBuffer(MAX_PATH),MAX_PATH,filename);
	value.ReleaseBuffer();
	return value;
}
LIB_COMMON_API DWORD IniReadStringValue(LPCTSTR filename,LPCTSTR section,LPCTSTR key,LPSTR value,LPCTSTR default_value)
{
	return ::GetPrivateProfileString(section,key,default_value,value,strlen(value)-1,filename);
}
LIB_COMMON_API int IniReadIntValue(LPCTSTR filename,LPCTSTR section,LPCTSTR key,int default_value/*=0*/)
{
	return ::GetPrivateProfileInt(section,key,default_value,filename);
}
LIB_COMMON_API BOOL IniWriteStringValue(LPCTSTR filename,LPCTSTR section,LPCTSTR key,LPCTSTR format,...)
{
	va_list arg;
	CString value;
	int len = 0;

	va_start(arg, format);
	len = vsnprintf(NULL,0,format,arg);
	va_end(arg);

	va_start(arg, format);
	vsprintf(value.GetBuffer(len),format,arg);
	va_end(arg);
	value.ReleaseBuffer();

	return ::WritePrivateProfileString(section,key,value,filename);
}

NameSpaceBegin(Path)
LIB_COMMON_API CString Combine(LPCTSTR path1,LPCTSTR path2)
{
	CString path;
	::PathCombine(path.GetBuffer(MAX_PATH),path1,path2);
	path.ReleaseBuffer();
	return path;
}
LIB_COMMON_API CString GetAppConfigFile(LPCTSTR filename)
{
	return Combine(GetAppDirectory(),filename);
}
LIB_COMMON_API CString GetAppFullPath()
{
	CString sPath;
	GetModuleFileName(NULL,sPath.GetBuffer(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	return sPath;
}
LIB_COMMON_API CString GetAppDirectory()
{
	CString sPath = GetAppFullPath();
	return RemoveFileName(sPath);
}
LIB_COMMON_API CString GetAppFileName()
{
	CString sPath = GetAppFullPath();
	return GetFileName(sPath);
}
LIB_COMMON_API CString GetFileName(LPCTSTR path)
{
	return PathFindFileName(path);
}
LIB_COMMON_API CString RemoveFileName(LPCTSTR path)
{
	CString newpath = path;
	if (newpath.IsEmpty()) return newpath;
	BOOL bRet = PathRemoveFileSpec(newpath.GetBuffer(newpath.GetLength()));
	newpath.ReleaseBuffer();
	if (bRet) return newpath;
	return CString();
}
NameSpaceEnd(Path)


NameSpaceBegin(Text)
LIB_COMMON_API CString Format(LPCTSTR format,...)
{
	va_list arg;
	CString text;
	int len = 0;

	va_start(arg, format);
	len = vsnprintf(NULL,0,format,arg);
	va_end(arg);

	va_start(arg, format);
	vsprintf(text.GetBuffer(len),format,arg);
	va_end(arg);
	text.ReleaseBuffer();
	return text;
}

LIB_COMMON_API int Explode(LPCTSTR text,LPCTSTR seperator,std::vector<CString>& arr)
{
	int last = 0;
	int pos;
	int current;
	CString value;
	CString _seperator(seperator);
	CString _text(text);

	int offset = _seperator.GetLength();
	int length = _text.GetLength();

	arr.clear();

	if (_seperator.IsEmpty())
	{
		arr.push_back(_text);
		return arr.size();
	}

	while (1)
	{
		pos = _text.Find(_seperator,last);
		current = pos;
		if (pos==-1)
			current = length;
		value = _text.Mid(last,current - last);
		last = pos + offset;
		if (!value.IsEmpty())
			arr.push_back(value);
		if (pos==-1)
			break;
	}
	return arr.size();
}
LIB_COMMON_API bool IsNumber(LPCTSTR value)
{
	int len = strlen(value);
	char ch = NULL;  
	for (int i=0;i<len;i++)  
	{  
		ch = value[i];
		if (ch > '9' || ch < '0')  
			return false;
	}  
	return true;
}
LIB_COMMON_API bool IsFloat(LPCTSTR value)
{
	int len = strlen(value);
	char ch = NULL;
	int number = 0;
	int dot = 0;
	for (int i=0;i<len;i++)  
	{  
		ch = value[i];
		if (ch >='0' && ch <='9'){
			number++;
			continue;
		}
		if (ch=='.') {
			dot++;
			continue;
		}
		return false;
	}  
	return (number && dot);
}
NameSpaceEnd(Text)
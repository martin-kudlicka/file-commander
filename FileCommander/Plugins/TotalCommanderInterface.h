/// \file interface to handle Windows specified types and structures

#ifndef TOTAL_COMMANDER_INTERFACE_H
#define TOTAL_COMMANDER_INTERFACE_H

#define MAX_PATH 260

//typedef int BOOL;
typedef int DWORD;
//typedef void *HANDLE;
//typedef wchar_t WCHAR;
//typedef WCHAR TCHAR;
typedef unsigned short WORD;

/*typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME;
typedef struct _WIN32_FIND_DATA {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	DWORD dwReserved0;
	DWORD dwReserved1;
	TCHAR cFileName[MAX_PATH];
	TCHAR cAlternateFileName[14];
} WIN32_FIND_DATA;*/

#endif TOTAL_COMMANDER_INTERFACE_H

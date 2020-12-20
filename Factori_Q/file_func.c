#include <Windows.h>
#include <stdio.h>
#include "HardCodedData.h"
/*========================================================================*/
/*
Input: file_name, desired_access, share_mode, creation_disposition
Output: HANDLE to file
Creats file simply.
*/
HANDLE CreateFileSimple(LPCSTR file_name, DWORD desired_access, DWORD share_mode, DWORD creation_disposition)
{
	return CreateFileA(file_name,
		desired_access,
		share_mode,
		NULL,
		creation_disposition,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
/*========================================================================*/
/*
Input: p_start_routine, p_params, p_thread_id
Output: HANDLE to thread
Creats thread simply.
*/
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID p_params, LPDWORD p_thread_id)
{
	HANDLE thread_handle;
	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread\nReceived null pointer\n");
		return NULL;
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread\nReceived null pointer\n");
		return NULL;
	}

	thread_handle = CreateThread(
		NULL,
		0,
		p_start_routine,
		p_params,
		0,
		p_thread_id);

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		return NULL;
	}
	return thread_handle;

}
/*========================================================================*/
/*
Input: HANDLE to file, distance_to_move, move_method
Output: 0 if succseded (-1 otherwise).
Set a poiter of file in disired location (distance_to_move bytes from start).
*/
int SetFilePointerSimple(HANDLE file, long distance_to_move , DWORD move_method)
{
	DWORD dw_ptr_low = SetFilePointer(file, distance_to_move, NULL, move_method);
	//printf("DtM:%d\n", distance_to_move);
	if (dw_ptr_low == INVALID_SET_FILE_POINTER &&
		GetLastError() != NO_ERROR)
	{
		if (!CloseHandle(file))
		{
			printf("HANDLE_CLOSING_ERROR\n");
			return FAILURE_CODE;
		}
		printf("ERROR: %d\n", GetLastError());
		printf("FAILED_TO_SET_FILE_POINTER\n");
		return FAILURE_CODE;
	}
	return 0;
}
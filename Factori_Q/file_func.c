#include <Windows.h>
#include <stdio.h>
#include "HardCodedData.h"

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
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID p_params, LPDWORD p_thread_id)
{
	HANDLE thread_handle;
	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread\nReceived null pointer");
		return NULL;
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread\nReceived null pointer");
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
int SetFilePointerSimple(HANDLE file, long distance_to_move , DWORD move_method)
{
	DWORD dw_ptr_low = SetFilePointer(file, distance_to_move, NULL, move_method);
	//printf("DtM:%d\n", distance_to_move);
	if (dw_ptr_low == INVALID_SET_FILE_POINTER &&
		GetLastError() != NO_ERROR)
	{
		if (!CloseHandle(file))
		{
			printf("HANDLE_CLOSING_ERROR");
			return FAILURE_CODE;
		}
		printf("ERROR: %d\n", GetLastError());
		printf("FAILED_TO_SET_FILE_POINTER");
		return FAILURE_CODE;
	}
	return 0;
}
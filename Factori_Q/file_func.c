#include <Windows.h>
#define ERROR_CODE -1
#include <stdio.h>

void ExitFailure(char* exit_message, int exitcode)
{
	printf("%s\n", exit_message);
	exit(exitcode);
}
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
		ExitFailure("Error when creating a thread\nReceived null pointer", ERROR_CODE);
		return NULL;
	}

	if (NULL == p_thread_id)
	{
		ExitFailure("Error when creating a thread\nReceived null pointer", ERROR_CODE);
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
		ExitFailure("Couldn't create thread\n", ERROR_CODE);
	}
	return thread_handle;

}
void SetFilePointerSimple(HANDLE file, long distance_to_move , DWORD move_method)
{
	DWORD dw_ptr_low = SetFilePointer(file, distance_to_move, NULL, move_method);
	//printf("DtM:%d\n", distance_to_move);
	if (dw_ptr_low == INVALID_SET_FILE_POINTER &&
		GetLastError() != NO_ERROR)
	{
		if (!CloseHandle(file))
		{
			ExitFailure("HANDLE_CLOSING_ERROR", -1);
		}
		printf("ERROR: %d\n", GetLastError());
		ExitFailure("FAILED_TO_SET_FILE_POINTER", -1);
	}
}
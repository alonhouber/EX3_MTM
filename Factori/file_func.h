#ifndef FILE_FUNC_H
#define FILE_FUNC_H
#include <Windows.h>

void ExitFailure(char* exit_message, int exitcode);
HANDLE CreateFileSimple(LPCSTR file_name, DWORD desired_access,
    DWORD share_mode, DWORD creation_disposition);
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
    LPVOID p_params, LPDWORD p_thread_id);
void SetFilePointerSimple(HANDLE file, long distance_to_move, DWORD move_method);
#endif 

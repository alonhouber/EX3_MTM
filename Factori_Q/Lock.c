#include <Windows.h>
#include <stdio.h>
#include "Lock.h"

Lock* New__Lock(int number_of_threads)
{
	Lock* my_lock = (Lock*)malloc(sizeof(Lock));
	if (my_lock == NULL) {
		printf("MEMORY_ALLOCATION_FAILED");
		return NULL;
	}
	my_lock->read_lock = CreateSemaphore(NULL, number_of_threads, number_of_threads, NULL);
	if (my_lock->read_lock == NULL){
		return NULL;
	}
	my_lock->write_lock = CreateSemaphore(NULL, number_of_threads, number_of_threads, NULL);
	if (my_lock->write_lock == NULL) {
		return NULL;
	}
	return my_lock;
}

BOOL Write__Release(Lock* my_Lock, int number_of_threads) {
	return ReleaseSemaphore(my_Lock->write_lock, number_of_threads, NULL);
}

BOOL Read__Release(Lock* my_Lock) {
	return ReleaseSemaphore(my_Lock->read_lock, 1, NULL) && ReleaseSemaphore(my_Lock->write_lock, 1, NULL);
}

BOOL Read__Lock(Lock* my_Lock, int wait_time) {
	DWORD wait_code;	
	wait_code = WaitForSingleObject(my_Lock->write_lock, INFINITE);	
	if (WAIT_OBJECT_0 != wait_code) {
		printf("write_locked\n");
		return  FALSE;
	}	
	wait_code = WaitForSingleObject(my_Lock->read_lock, INFINITE);
	if (WAIT_OBJECT_0 != wait_code) {
		printf("read_locked\n");
		return  FALSE;
	}	
	return TRUE;
}

BOOL Write__Lock(Lock* my_Lock, int wait_time, int number_of_threads) {
	DWORD wait_code;
	for (int i = 0; i < number_of_threads; i++)
	{
		wait_code = WaitForSingleObject(my_Lock->write_lock, wait_time);
		if (WAIT_OBJECT_0 != wait_code) {
			return  FALSE;
		}
	}		
	return TRUE;
}

BOOL Destroy__lock(Lock* my_Lock)
{
	BOOL succeded = TRUE;
	if (CloseHandle(my_Lock->read_lock) == 0)
	{
		succeded = FALSE;
	}
	if (CloseHandle(my_Lock->write_lock) == 0)
	{
		succeded = FALSE;
	}
	free(my_Lock);
	return succeded;
}




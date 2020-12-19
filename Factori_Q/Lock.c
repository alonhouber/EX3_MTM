#include <Windows.h>
#define WAIT_TIME 5000
#include <stdio.h>
typedef struct Lock {
	HANDLE read_lock;
	HANDLE write_lock;
}Lock;

Lock* New__Lock(int number_of_threads)
{
	Lock* my_lock = (Lock*)malloc(sizeof(Lock));
	if (my_lock == NULL) {
		printf("MEMORY_ALLOCATION_FAILED");
		return NULL;
	}
	my_lock->read_lock = CreateSemaphoreA(NULL, 0, number_of_threads, NULL);
	if (my_lock->read_lock == NULL){
		return NULL;
	}
	my_lock->write_lock = CreateSemaphoreA(NULL, 0, 1, NULL);
	if (my_lock->write_lock == NULL) {
		return NULL;
	}
	return my_lock;
}

BOOL Read__Lock(Lock* my_Lock) {
	DWORD wait_code;
	wait_code = WaitForSingleObject(my_Lock->write_lock, WAIT_TIME);
	if (WAIT_OBJECT_0 != wait_code) {		
		return  FALSE;
	}
	wait_code = WaitForSingleObject(my_Lock->read_lock, WAIT_TIME);
	if (WAIT_OBJECT_0 != wait_code) {
		return  FALSE;
	}
	return TRUE;
}

BOOL Read__Release(Lock* my_Lock) {
	return ReleaseSemaphore(my_Lock->read_lock, 1, NULL);
}



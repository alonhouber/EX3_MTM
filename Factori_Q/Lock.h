#ifndef LOCK_H
#define LOCK_H
#include <Windows.h>

typedef struct Lock {
	HANDLE read_lock;
	HANDLE write_lock;
}Lock;

Lock* New__Lock(int number_of_threads);
BOOL Write__Release(Lock* my_Lock, int number_of_threads);
BOOL Read__Release(Lock* my_Lock);
BOOL Read__Lock(Lock* my_Lock, int wait_time);
BOOL Write__Lock(Lock* my_Lock, int wait_time, int number_of_threads);
BOOL Destroy__lock(Lock* my_Lock);
#endif 

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <stdbool.h>
#include "file_func.h"
#include "Queue.h"
#include "Lock.h"
#include "List.h"
#include "HardCodedData.h"
list* Get__PrimeFactors(int number)
{
	list* p_prime_numbers_head = NULL;
	while (number % 2 == 0)
	{
		p_prime_numbers_head = Add__ToList(p_prime_numbers_head, 2);
		if (p_prime_numbers_head == NULL)
		{
			return NULL;
		}
		number /= 2;
	}
	int factor_to_check = 3;
	for (; factor_to_check <= (int)sqrt(number); factor_to_check += 2)
	{
		while (number % factor_to_check == 0)
		{
			p_prime_numbers_head = Add__ToList(p_prime_numbers_head, factor_to_check);
			if (p_prime_numbers_head == NULL)
			{
				return NULL;
			}
			number /= factor_to_check;
		}
	}
	if (number > 2)
	{
		p_prime_numbers_head = Add__ToList(p_prime_numbers_head, number);
		if (p_prime_numbers_head == NULL)
		{
			return NULL;
		}
	}
	return p_prime_numbers_head;
}

typedef struct {
	char mission_file_name[_MAX_PATH];	
	int number_of_missions;
	Queue* priority_Q;
	Lock* my_lock;
	int number_of_threads;
}Thread_Params;

int char_to_int(char char_num)
{
	if (char_num >= '0' && char_num <= '9')
		return char_num - '0';
	return 0;
}
/*========================================================================*/
/* Input: File Handle to file of numbers
   Output: The number in the current line which the handle points at.
   return -1 if Failed
   */
int Get_Number(HANDLE file_handle)
{
	char current_char = '\0';
	int number = 0;
	int num_of_char = 0;
	if (!ReadFile(file_handle,
		&current_char,
		READ_ONE_CHAR,
		NULL,
		NULL))
	{
		printf("READ_FILE_FAIL\n");
		return -1;
	}
	while (current_char != '\r')
	{
		number = number * DECIMAL_BASE;
		num_of_char = char_to_int(current_char);
		number += num_of_char;
		if (!ReadFile(file_handle,
			&current_char,
			READ_ONE_CHAR,
			NULL,
			NULL))
		{
			printf("READ_FILE_FAIL\n");
			return -1;
		}
	}
	return number;
}
int Get_Priority(HANDLE priority_file_handle)
{
	int priority = Get_Number(priority_file_handle);
	if (SetFilePointerSimple(priority_file_handle, 1, FILE_CURRENT) == FAILURE_CODE)
	{
		return FAILURE_CODE;
	}
	return priority;
}
int Get_Mission(HANDLE mission_file_handle)
{
	return Get_Number(mission_file_handle);	
}
/*========================================================================*/
/* Input: File Handle to file of numbers and current mission number
   Output: Write the formatted result of the mission number to EOF
   return false if Failed else true
   */
bool Write_Mission(HANDLE mission_file_handle, int mission_number)
{	
	list* current_mission_head = NULL;
	current_mission_head = Get__PrimeFactors(mission_number);
	if (current_mission_head == NULL)
	{
		if (mission_number >= 2)
		{
			return false;
		}		
	}
	int memory_size = Get__line_list_length(current_mission_head, mission_number);	
	char* list_format_string = (char*)malloc(sizeof(char) * memory_size);
	if (list_format_string == NULL)
	{
		Free__List(current_mission_head);
		printf("MEMORY_ALLOCATION_FAILURE\n");
		return false;
	}
	list_format_string = Print__List(current_mission_head, mission_number, list_format_string, memory_size);
	if (list_format_string == NULL)
	{
		free(list_format_string);
		Free__List(current_mission_head);
		return false;
	}
	if (SetFilePointerSimple(mission_file_handle, 0, FILE_END) == FAILURE_CODE)
	{
		free(list_format_string);
		Free__List(current_mission_head);
		return false;
	}
	if (!WriteFile(mission_file_handle,
		list_format_string,
		strlen(list_format_string),
		NULL,
		NULL))
	{
		free(list_format_string);
		Free__List(current_mission_head);
		printf("Write_FILE_FAIL\n");
		return false;
	}
	free(list_format_string);
	Free__List(current_mission_head);
	return true;
}
Queue* Create_Priority_Queue(HANDLE priority_file_handle, int number_of_missions)
{
	Queue* priority_Q = New__Queue();
	if (priority_Q == NULL)
	{
		printf("MEMORY_ALLOCATION_FAILED\n");
		return NULL;
	}
	int priority_number = 0;
	while (priority_Q->client_count < number_of_missions)
	{		
		priority_number = Get_Priority(priority_file_handle);
		if (priority_number == -1)
		{
			return NULL;
		}
		if (Push__Queue(priority_Q, priority_number) == FAILURE_CODE)
		{
			return NULL;
		}
	}
	return priority_Q;	
}
BOOL CloseHandleSimple(HANDLE h_to_close)
{
	if (CloseHandle(h_to_close) == FALSE)
	{
		printf("FAILED_TO_CLOSE_HANDLE\n");
		return FALSE;
	}
	return TRUE;
}
DWORD WINAPI Read_And_Write(LPVOID lp_params)
{
	Thread_Params* p_thread_params = (Thread_Params*)lp_params;
	/*==========================================================================================*/
	/* Open Mission File */
	HANDLE mission_file_handle = NULL;
	mission_file_handle = CreateFileSimple(p_thread_params->mission_file_name,
		(GENERIC_READ | GENERIC_WRITE) , FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING);
	if (mission_file_handle == INVALID_HANDLE_VALUE){
		DWORD dw = GetLastError();
		printf("error: %d/n", dw);
		printf("FAILED_TO_OPEN\n");
		return FAILURE_CODE;
	}		
	/*==========================================================================================*/
	/* Run Missions until Queue is Empty */	
	while (!Empty__Queue(p_thread_params->priority_Q)){		
		while (Read__Lock(p_thread_params->my_lock, WAIT_TIME_READ_LOCK) == FALSE);			
		/*==========================================================================================*/
		/* Get Priority */
		int mission_start_byte = Pop__Queue(p_thread_params->priority_Q);
		if (mission_start_byte == -1){
			Read__Release(p_thread_params->my_lock);	
			printf("Queue is Empty\n");
			if (CloseHandleSimple(mission_file_handle) == FALSE)
			{
				return FAILURE_CODE;
			}
			return FAILURE_CODE;
		}
		/*==========================================================================================*/
		/* Get Mission */
		int mission_number = 0;
		if (SetFilePointerSimple(mission_file_handle, mission_start_byte, FILE_BEGIN) == FAILURE_CODE)
		{
			Read__Release(p_thread_params->my_lock);
			if (CloseHandleSimple(mission_file_handle) == FALSE)
			{
				return FAILURE_CODE;
			}
			return FAILURE_CODE;
		}
		mission_number = Get_Mission(mission_file_handle);
		if (mission_number == -1){//What if we get -1 as a mission?
			Read__Release(p_thread_params->my_lock);	
			if (CloseHandleSimple(mission_file_handle) == FALSE)
			{
				return FAILURE_CODE;
			}
			return FAILURE_CODE;
		}
		/*==========================================================================================*/
		Read__Release(p_thread_params->my_lock);		
		/*==========================================================================================*/
		/* Secure Mission Writing and Write result at EOF */
		while (Write__Lock__Mutex(p_thread_params->my_lock, WAIT_TIME_WRITE_LOCK) == FALSE);
		while (Write__Lock(p_thread_params->my_lock, WAIT_TIME_WRITE_LOCK, p_thread_params->number_of_threads) == FALSE);
		if (Write_Mission(mission_file_handle, mission_number) == false){		
			if (CloseHandleSimple(mission_file_handle) == FALSE)
			{
				return FAILURE_CODE;
			}
			return FAILURE_CODE;
		}	
		Write__Release(p_thread_params->my_lock, p_thread_params->number_of_threads);
		Write__Release__Mutex(p_thread_params->my_lock);
	}	
	if (CloseHandleSimple(mission_file_handle) == FALSE)
	{		
		return FAILURE_CODE;
	}
	return 0;
}
void Free__Thread_Params(Thread_Params* p_thread_params)
{
	Destroy__Queue(p_thread_params->priority_Q);
	Destroy__lock(p_thread_params->my_lock);
	free(p_thread_params);
}

int Create_And_Handle_Threads(char* mission_file_name, char* priority_file_name,
	int number_of_missions, int number_of_threads) {
	/*==========================================================================================*/
	/* Create Thread Params */
	Thread_Params* p_thread_params = (Thread_Params*)malloc(sizeof(Thread_Params));
	if (p_thread_params == NULL) {
		printf("MEMORY_ALLOCATION_FAILURE\n");
		return -1;
	}
	if (snprintf(p_thread_params->mission_file_name, _MAX_PATH, "%s", mission_file_name) == 0)
	{
		free(p_thread_params);
		return -1;
	}
	p_thread_params->number_of_missions = number_of_missions;
	p_thread_params->number_of_threads = number_of_threads;
	/* Create Queue and assume it to Thread Params */
	HANDLE priority_file_handle;
	priority_file_handle = CreateFileSimple(priority_file_name,
		GENERIC_READ, 0, OPEN_EXISTING);
	if (priority_file_handle == INVALID_HANDLE_VALUE) {
		free(p_thread_params);
		printf("FAILED_TO_OPEN\n");
		return -1;
	}
	p_thread_params->priority_Q = Create_Priority_Queue(priority_file_handle, number_of_missions);
	if (p_thread_params->priority_Q == NULL)
	{
		free(p_thread_params);
		if (CloseHandleSimple(priority_file_handle) == FALSE)
		{
			return -1;
		}			
		return -1;
	}
	Lock* new_lock = New__Lock(number_of_threads);
	if (new_lock == NULL)
	{		
		Destroy__Queue(p_thread_params->priority_Q);
		free(p_thread_params);
		if (CloseHandleSimple(priority_file_handle) == FALSE)
		{
			return -1;
		}
		return -1;
	}
	p_thread_params->my_lock = new_lock;
	/*==========================================================================================*/
	/* Create Thread */
	HANDLE thread_handles[MAXIMUM_WAIT_OBJECTS];
	DWORD thread_id[MAXIMUM_WAIT_OBJECTS];
	for (int i = 0; i < p_thread_params->number_of_threads; i++)
	{
		thread_handles[i] = CreateThreadSimple(Read_And_Write, p_thread_params, &(thread_id[i]));
		if (thread_handles == NULL) {			
			Free__Thread_Params(p_thread_params);
			printf("FAILED_TO_CREATE_THREAD\n");			
			for (int j = i; j >= 0; j--)
			{
				CloseHandleSimple(thread_handles[j]);				
			}
			CloseHandleSimple(priority_file_handle);
			return -1;
		}
	}

	/*==========================================================================================*/
	/* Wait for Thread */
	DWORD wait_code;
	wait_code = WaitForMultipleObjects(p_thread_params->number_of_threads, thread_handles, TRUE, WAIT_TIME);
	if (WAIT_OBJECT_0 != wait_code) {
		Free__Thread_Params(p_thread_params);
		printf("Error when waiting\n");
		for (int j = 0; j < number_of_threads; j++)
		{
			CloseHandleSimple(thread_handles[j]);
		}
		if (CloseHandleSimple(priority_file_handle) == FALSE)
		{
			return -1;
		}		
		return -1;
	}
	Free__Thread_Params(p_thread_params);
	BOOL close_handle_success = TRUE;
	for (int j = 0; j < number_of_threads; j++)
	{
		if (CloseHandleSimple(thread_handles[j]) == FALSE)
		{
			close_handle_success = FALSE;
		}
	}
	if (CloseHandleSimple(priority_file_handle) == FALSE)
	{
		close_handle_success = FALSE;
	}	
	if (!close_handle_success)
	{
		return -1;
	}
	return  1;
}

int main(int argc, char* argv[])
{	
	int missions_num, threads_num;
	missions_num = (int)strtol(argv[MISSIONS_NUM_ARGUMET], NULL, DECIMAL_BASE);
	threads_num = (int)strtol(argv[THREADS_NUM_ARGUMET], NULL, DECIMAL_BASE);
	if (Create_And_Handle_Threads(argv[MISSION_FILE_NAME_ARGUMENT], argv[PRIORITY_FILE_NAME_ARGUMENT], missions_num, threads_num) == -1)
	{
		return -1;
	}	
	return 0;
}
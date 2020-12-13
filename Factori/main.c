#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include "file_func.h"
#define DECIMAL_BASE (int)10
#define READ_ONE_CHAR 1
#define WAIT_TIME 20000
#define MISSION_FILE_NAME_ARGUMENT 1
#define PRIORITY_FILE_NAME_ARGUMENT 2
#define MISSIONS_NUM_ARGUMET 3
#define THREADS_NUM_ARGUMET 4

typedef struct list_t {
	int number;
	struct list_t* next;
}list;
list* New__List(int number)
{
	list* new_list = (list*) malloc(sizeof(list));	
	if (new_list != NULL)
	{
		new_list->number = number;
		new_list->next = NULL;
	}
	return new_list;
}
list* Add__ToList(list* head, int number)
{
	if (head == NULL)
	{
		head = New__List(number);
		return head;
	}
	list* last_num;
	last_num = head;
	while (last_num->next != NULL)
	{
		last_num = last_num->next;
	}
	last_num->next = New__List(number);
	return head;
}
char* Print__List(list* head, int number)
{
	int memory_size = 40;
	char* list_format_string = (char*)malloc(sizeof(char) * memory_size);
	if (list_format_string == NULL)
	{
		ExitFailure("Fail", -1);
		return NULL;
	}
	snprintf(list_format_string, memory_size, "The prime factors of %d are: ", number);
	if (head != NULL)
	{
		while (head->next != NULL)
		{
			memory_size += 11;
			snprintf(list_format_string, memory_size, "%d", head->number);			
			head = head->next;
		}
		memory_size += 11;
		snprintf(list_format_string, memory_size, "%d", head->number);
	}
	printf("\n");
}
void Free__List(list* head)
{
	while (head != NULL)
	{
		list* temp = head;
		head = head->next;
		free(temp);
	}
}
list* Get__PrimeFactors(int number)
{
	list* p_prime_numbers_head = NULL;
	while (number % 2 == 0)
	{
		p_prime_numbers_head = Add__ToList(p_prime_numbers_head,2);
		number /= 2;
	}
	int factor_to_check = 3;
	for  (;  factor_to_check <= (int)sqrt(number) ; factor_to_check+=2)
	{
		while (number % factor_to_check == 0)
		{
			p_prime_numbers_head = Add__ToList(p_prime_numbers_head,factor_to_check);
			number /= factor_to_check;
		}
	}
	if (number > 2)
	{
		p_prime_numbers_head = Add__ToList(p_prime_numbers_head,number);
	}
	return p_prime_numbers_head;
}
typedef struct {
	char mission_file_name[_MAX_PATH];
	char priority_file_name[_MAX_PATH];
	int number_of_missions;
}Thread_Params;

int char_to_int(char char_num)
{
	if(char_num >= '0' && char_num <= '9')
		return char_num - '0';
	return 0;
}

int Get_Priority(HANDLE priority_file_handle)
{
	char current_char = '\0';
	int priority = 0;
	int num_of_char = 0;
	if (!ReadFile(priority_file_handle,
		&current_char,
		READ_ONE_CHAR,
		NULL,
		NULL))
	{
		ExitFailure("READ_FILE_FAIL", -1);
	}
	while (current_char != '\r')
	{
		priority = priority * DECIMAL_BASE;
		num_of_char = char_to_int(current_char);
		priority += num_of_char;
		if (!ReadFile(priority_file_handle,
			&current_char,
			READ_ONE_CHAR,
			NULL,
			NULL))
		{
			ExitFailure("READ_FILE_FAIL", -1);
		}
	}
	SetFilePointerSimple(priority_file_handle, 1, FILE_CURRENT);
	printf("Prio %d", priority);
	return priority;
}

DWORD WINAPI Read_And_Write(LPVOID lp_params )
{
	Thread_Params* p_thread_params = (Thread_Params*)lp_params;
	HANDLE mission_file_handle, priority_file_handle;
	mission_file_handle = CreateFileSimple(p_thread_params->mission_file_name, 
		GENERIC_READ, 0, OPEN_EXISTING);
	if (mission_file_handle == INVALID_HANDLE_VALUE)
	{
		ExitFailure("FAILED_TO_OPEN", -1);
		return -1;
	}

	priority_file_handle = CreateFileSimple(p_thread_params->priority_file_name,
		GENERIC_READ, 0, OPEN_EXISTING);
	if (priority_file_handle == INVALID_HANDLE_VALUE)
	{
		ExitFailure("FAILED_TO_OPEN", -1);
	}	
	int i = 0;
	while (i < p_thread_params->number_of_missions)
	{
		int mission_start_byte = Get_Priority(priority_file_handle);
		int mission_number = 0;
		SetFilePointerSimple(mission_file_handle, mission_start_byte, FILE_BEGIN);
		char current_char = '\r';
		if (!ReadFile(mission_file_handle,
			&current_char,
			READ_ONE_CHAR,
			NULL,
			NULL))
		{
			ExitFailure("READ_FILE_FAIL", -1);
			return -1;
		}
		while (current_char != '\r')
		{
			mission_number *= DECIMAL_BASE;
			mission_number += char_to_int(current_char);
			if (!ReadFile(mission_file_handle,
				&current_char,
				READ_ONE_CHAR,
				NULL,
				NULL))
			{
				ExitFailure("READ_FILE_FAIL", -1);
			}
		}
		list* current_mission_head = NULL; 
		current_mission_head = Get__PrimeFactors(mission_number);
		Print__List(current_mission_head,mission_number);
		Free__List(current_mission_head);		
		i++;
	}
	return 1;
}
int Create_And_Handle_Threads(char* mission_file_name, char* priority_file_name,
	int number_of_missions, int number_of_threads)
{
	Thread_Params* p_thread_params = (Thread_Params*)malloc(sizeof(Thread_Params));
	if (p_thread_params == NULL)
	{
		ExitFailure("MEMORY_ALLOCATION_FAILURE", -1);
		return -1;
	}
	snprintf(p_thread_params->mission_file_name, _MAX_PATH, "%s", mission_file_name);
	snprintf(p_thread_params->priority_file_name, _MAX_PATH, "%s", priority_file_name);
	p_thread_params->number_of_missions = number_of_missions;

	HANDLE thread_handle;
	DWORD thread_id;
	thread_handle = CreateThreadSimple(Read_And_Write, p_thread_params, &thread_id);
	if (thread_handle == NULL)
	{
		ExitFailure("FAILED_TO_CREATE_THREAD", -1);
		return -1;
	}
	DWORD wait_code;
	wait_code = WaitForSingleObject(thread_handle, WAIT_TIME);
	if (WAIT_OBJECT_0 != wait_code)
	{
		ExitFailure("Error when waiting", -1);
	}
	free(p_thread_params);
	return  1;
}
int main(int argc, char* argv[])
{
	int missions_num, threads_num;
	missions_num = strtol(argv[MISSIONS_NUM_ARGUMET], NULL, DECIMAL_BASE);
	threads_num = strtol(argv[THREADS_NUM_ARGUMET], NULL, DECIMAL_BASE);
	Create_And_Handle_Threads(argv[MISSION_FILE_NAME_ARGUMENT], argv[PRIORITY_FILE_NAME_ARGUMENT], missions_num, threads_num);
	/*printf("Enter Number (-1 to stop): \n");
	scanf_s("%d", &x);
	while (x != -1)
	{		
		list* head = Get__PrimeFactors(x);		
		Print__List(head, x);
		Free__List(head);
		printf("Enter Number (-1 to stop): \n");
		scanf_s("%d", &x);		
	}*/
	return 0;
}

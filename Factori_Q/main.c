#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <stdbool.h>
#include "file_func.h"
#include "Queue.h"

#define DECIMAL_BASE (int)10
#define READ_ONE_CHAR 1
#define WAIT_TIME 20000
#define MISSION_FILE_NAME_ARGUMENT 1
#define PRIORITY_FILE_NAME_ARGUMENT 2
#define MISSIONS_NUM_ARGUMET 3
#define THREADS_NUM_ARGUMET 4
#define START_OF_LINE_LEN 30
#define COMMA_AND_SPACE_LEN 2

typedef struct list_t {
	int number;
	struct list_t* next;
}list;
list* New__List(int number)
{
	list* new_list = (list*)malloc(sizeof(list));
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
int get_len_of_num(int num)
{
	int counter = 0;
	while (num > 0)
	{
		num = num / DECIMAL_BASE;
		counter++;
	}
	return counter;
}

int Get__line_list_length(list* head, int num)
{
	list* current_pos = head;
	int len = START_OF_LINE_LEN + get_len_of_num(num);
	if (head != NULL)
	{
		while (current_pos->next != NULL)
		{
			len = len + COMMA_AND_SPACE_LEN + get_len_of_num(current_pos->number);
			current_pos = current_pos->next;
		}
		len = len + get_len_of_num(current_pos->number);
	}
	return len;
}

char* Print__List(list* head, int number, char* list_format_string, int memory_size)
{
	list* temp_head = head;
	snprintf(list_format_string, memory_size, "The prime factors of %d are: ", number);
	char* temp_string = list_format_string;
	if (temp_head != NULL)
	{
		while (temp_head->next != NULL)
		{
			temp_string = list_format_string;
			snprintf(list_format_string, memory_size, "%s%d, ", temp_string, temp_head->number);
			temp_head = temp_head->next;
		}
		temp_string = list_format_string;
		snprintf(list_format_string, memory_size, "%s%d", temp_string, temp_head->number);
	}
	snprintf(list_format_string, memory_size, "%s\r\n", temp_string);
	return list_format_string;
}
void Free__List(list* head)
{
	list* temp = head;
	while (head != NULL)
	{
		head = head->next;
		free(temp);
		temp = head;
	}
}
list* Get__PrimeFactors(int number)
{
	list* p_prime_numbers_head = NULL;
	while (number % 2 == 0)
	{
		p_prime_numbers_head = Add__ToList(p_prime_numbers_head, 2);
		number /= 2;
	}
	int factor_to_check = 3;
	for (; factor_to_check <= (int)sqrt(number); factor_to_check += 2)
	{
		while (number % factor_to_check == 0)
		{
			p_prime_numbers_head = Add__ToList(p_prime_numbers_head, factor_to_check);
			number /= factor_to_check;
		}
	}
	if (number > 2)
	{
		p_prime_numbers_head = Add__ToList(p_prime_numbers_head, number);
	}
	return p_prime_numbers_head;
}
typedef struct {
	char mission_file_name[_MAX_PATH];	
	int number_of_missions;
	Queue* priority_Q;
}Thread_Params;

int char_to_int(char char_num)
{
	if (char_num >= '0' && char_num <= '9')
		return char_num - '0';
	return 0;
}
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
		printf("READ_FILE_FAIL");
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
			printf("READ_FILE_FAIL");
			return -1;
		}
	}
	return number;
}
int Get_Priority(HANDLE priority_file_handle)
{
	int priority = Get_Number(priority_file_handle);
	SetFilePointerSimple(priority_file_handle, 1, FILE_CURRENT);
	return priority;
}
int Get_Mission(HANDLE mission_file_handle)
{
	int mission = Get_Number(mission_file_handle);
	return mission;
}
bool Write_Mission(HANDLE mission_file_handle, int mission_number)
{
	list* current_mission_head = NULL;
	current_mission_head = Get__PrimeFactors(mission_number);
	int memory_size = Get__line_list_length(current_mission_head, mission_number);
	printf("Mem Size: %d\n", memory_size);
	char* list_format_string = (char*)malloc(sizeof(char) * memory_size);
	list_format_string = Print__List(current_mission_head, mission_number, list_format_string, memory_size);
	SetFilePointerSimple(mission_file_handle, 0, FILE_END);
	if (!WriteFile(mission_file_handle,
		list_format_string,
		strlen(list_format_string),
		NULL,
		NULL))
	{
		free(list_format_string);
		Free__List(current_mission_head);
		printf("Write_FILE_FAIL");
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
		printf("MEMORY_ALLOCATION_FAILED");
		return NULL;
	}
	int priority_number = 0;
	while (priority_Q->client_count < number_of_missions)
	{		
		priority_number = Get_Priority(priority_file_handle);
		Push__Queue(priority_Q, priority_number);					
	}
	return priority_Q;	
}

DWORD WINAPI Read_And_Write(LPVOID lp_params)
{
	Thread_Params* p_thread_params = (Thread_Params*)lp_params;
	HANDLE mission_file_handle;
	mission_file_handle = CreateFileSimple(p_thread_params->mission_file_name,
		GENERIC_READ | GENERIC_WRITE, 0, OPEN_EXISTING);
	if (mission_file_handle == INVALID_HANDLE_VALUE)
	{
		ExitFailure("FAILED_TO_OPEN", -1);
		return -1;
	}		
	while (p_thread_params->priority_Q->client_count > 0)
	{
		int mission_start_byte = Pop__Queue(p_thread_params->priority_Q);
		if (mission_start_byte == -1)
		{
			Destroy__Queue(p_thread_params->priority_Q);
			return -1;
		}
		int mission_number = 0;
		SetFilePointerSimple(mission_file_handle, mission_start_byte, FILE_BEGIN);
		mission_number = Get_Mission(mission_file_handle);
		if (mission_number == -1)//What if we get -1 as a mission?
		{
			Destroy__Queue(p_thread_params->priority_Q);
			return -1;
		}
		if (Write_Mission(mission_file_handle, mission_number) == false)
		{
			Destroy__Queue(p_thread_params->priority_Q);
			return -1;
		}
	}
	return 1;
}
int Create_And_Handle_Threads(char* mission_file_name, char* priority_file_name,
	int number_of_missions, int number_of_threads)
{
	Thread_Params* p_thread_params = (Thread_Params*)malloc(sizeof(Thread_Params));
	if (p_thread_params == NULL)
	{
		printf("MEMORY_ALLOCATION_FAILURE");
		return -1;
	}
	snprintf(p_thread_params->mission_file_name, _MAX_PATH, "%s", mission_file_name);	
	p_thread_params->number_of_missions = number_of_missions;

	HANDLE priority_file_handle;
	priority_file_handle = CreateFileSimple(priority_file_name,
		GENERIC_READ, 0, OPEN_EXISTING);
	if (priority_file_handle == INVALID_HANDLE_VALUE)
	{
		free(p_thread_params);
		ExitFailure("FAILED_TO_OPEN", -1);
		return -1;
	}
	p_thread_params->priority_Q = Create_Priority_Queue(priority_file_handle, number_of_missions);

	HANDLE thread_handle;
	DWORD thread_id;
	thread_handle = CreateThreadSimple(Read_And_Write, p_thread_params, &thread_id);
	if (thread_handle == NULL)
	{
		free(p_thread_params);
		ExitFailure("FAILED_TO_CREATE_THREAD", -1);
		return -1;
	}
	DWORD wait_code;
	wait_code = WaitForSingleObject(thread_handle, WAIT_TIME);
	if (WAIT_OBJECT_0 != wait_code)
	{
		free(p_thread_params);
		ExitFailure("Error when waiting", -1);
		return  1;
	}
	free(p_thread_params);
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
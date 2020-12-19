#include <string.h>
#include <stdbool.h>
#include "List.h"

#define DECIMAL_BASE (int)10
#define START_OF_LINE_LEN 30
#define COMMA_AND_SPACE_LEN 2

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

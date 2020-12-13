#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#define READ_ONE_CHAR 1
#define READ_FILE_FAIL -1
#define DECIMAL_BASE 10

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

void Print__List(list * head, int number)
{
	printf("The prime factors of %d are: ", number);
	if (head != NULL)
	{
		while (head->next != NULL)
		{
			printf("%d, ", head->number);
			head = head->next;
		}
		printf("%d\n", head->number);
	}
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

int char_to_int(char char_num)
{
	return char_num - '0';
}

int get_priority(HANDLE priority_file)
{
	char current_char = '\0';
	int priority = 0;
	int num_of_char = 0;
	if (!ReadFile(priority_file,
		&current_char,
		READ_ONE_CHAR,
		NULL,
		NULL))
	{
		ExitFailure("READ_FILE_FAIL", READ_FILE_FAIL);
	}
	while (current_char != '\r')
	{
		priority = priority * DECIMAL_BASE;
		num_of_char = char_to_int(current_char);
		priority += num_of_char;
		if (!ReadFile(priority_file,
			current_char,
			READ_ONE_CHAR,
			NULL,
			NULL))
		{
			ExitFailure("READ_FILE_FAIL", READ_FILE_FAIL);
		}
	}
	return priority;
}

int main(int argc, char* argv[])
{
	int x;
	printf("Enter Number (-1 to stop): \n");
	scanf_s("%d", &x);
	while (x != -1)
	{		
		list* head = Get__PrimeFactors(x);		
		Print__List(head, x);
		Free__List(head);
		printf("Enter Number (-1 to stop): \n");
		scanf_s("%d", &x);		
	}
	return 0;
}

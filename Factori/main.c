#include <stdio.h>
#include <math.h>

typedef struct list {
	int number;
	list* next;
}list;

list* New__List(int number)
{
	list* new_list = (list*) malloc(sizeof(list));
	new_list->number = number;
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

list* Get__PrimeFactors(int number)
{
	list* p_prime_numbers_head = NULL;
	while (number % 2 == 0)
	{
		p_prime_numbers_head = Add__ToList(p_prime_numbers_head,2);
		number /= 2;
	}
	int factor_to_check = 3;
	for  (;  factor_to_check < (int)sqrt(number) ; factor_to_check+=2)
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
}

int main(int argc, char* argv[])
{
		
}

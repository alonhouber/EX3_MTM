#include <stdio.h>
#include <math.h>
#include <stdlib.h>
typedef struct list_t {
	int number;
	struct list_t* next;
}list;

list* New__List(int number)
{
	list* new_list = (list*) malloc(sizeof(list));	
	new_list->number = number;
	new_list->next = NULL;
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
	while (head->next != NULL)
	{
		printf("%d, ",head->number);
		head = head->next;
	}
	printf("%d\n", head->number);
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
	return p_prime_numbers_head;
}

int main(int argc, char* argv[])
{
	int x;
	scanf_s("%d", &x);
	list* head = Get__PrimeFactors(x);
	Print__List(head, x);

}

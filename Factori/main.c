#include <stdio.h>

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

int main(int argc, char* argv[])
{
	
}
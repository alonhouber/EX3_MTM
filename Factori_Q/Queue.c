#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Queue.h"

QClient* New__QClient(int number)
{
	QClient* new_client = (QClient*)malloc(sizeof(QClient));
	if (new_client == NULL)
	{
		return NULL;
	}
	new_client->number = number;
	new_client->previous = NULL;
	return new_client;
}

Queue* New__Queue()
{
	Queue* new_Q = (Queue*)malloc(sizeof(Queue));
	if (new_Q == NULL)
	{
		return NULL;
	}
	new_Q->first = NULL;
	new_Q->last = NULL;
	new_Q->client_count = 0;
	return new_Q;
}

void Push__Queue(Queue* my_Q , int number)
{
	QClient* new_QC = New__QClient(number);
	if (Empty__Queue(my_Q))
	{		
		my_Q->first = new_QC;
		my_Q->last = new_QC;
	}
	else
	{
		my_Q->last->previous = new_QC;
		my_Q->last = new_QC;
	}
	(my_Q->client_count)++;
}

int Pop__Queue(Queue* my_Q)
{	
	if (Empty__Queue(my_Q))
	{
		return -1;
	}
	else
	{
		int retval = my_Q->first->number;
		QClient* temp = my_Q->first;
		if (my_Q->first == my_Q->last)
			my_Q->last = NULL;
		my_Q->first = temp->previous;
		free(temp);
		(my_Q->client_count)--;
		return retval;
	}
}

int Top__Queue(Queue* my_Q)
{
	if (Empty__Queue(my_Q))
	{
		return -1;
	}
	else
	{				
		return my_Q->first->number;
	}
}

bool Empty__Queue (Queue* my_Q)
{	
	if (my_Q->first == NULL || my_Q->last == NULL)
	{
		return true;
	}
	if (my_Q->client_count == 0)
	{
		return true;
	}
	return false;
}

void Destroy__Queue(Queue* my_Q)
{
	while (!Empty__Queue(my_Q))		
	{
		Pop__Queue(my_Q);
	}
}

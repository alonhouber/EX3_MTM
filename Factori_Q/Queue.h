#ifndef QUEUE_H
#define QUEUE_H
typedef struct QClient
{
	int number;
	struct QClient* previous;
}QClient;

typedef struct Queue
{
	QClient* first;
	QClient* last;
	int client_count;
}Queue;

Queue* New__Queue();
//Queue New__Queue1();

void Push__Queue(Queue* my_Q, int number);
int Pop__Queue(Queue* my_Q);
int Top__Queue(Queue* my_Q);


#endif 

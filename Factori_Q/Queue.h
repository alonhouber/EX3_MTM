#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
typedef struct QClient
{
	int number;
	struct QClient* previous;
}QClient;

typedef struct Queue
{
	QClient* first;//Pointer to the first element in the Q
	QClient* last;//Pointer to the last element in the Q to improve efficency
	int client_count;//Number of elements in Q
}Queue;
/*========================================================================*/
/* Initialize Queue, Allocate the required memory and returns the new Queue.
   If it fails returns NULL, it's on the user to check.
   */
Queue* New__Queue();
/*========================================================================*/
/* Input: Pointer to Q and the element(int number) of the Q
   Creates a new element and adds it to the end of the Q
   Output: returns 0 if successful and -1(FAILURE_CODE)   
   */
int Push__Queue(Queue* my_Q, int number);
/*========================================================================*/
/* Input: Pointer to Q
   Removes the head of the Q, updates the Q accordingly
   Output: returns the element of the head(number) anf if Empty returns -1(FAILURE_CODE)
   */
int Pop__Queue(Queue* my_Q);
/*========================================================================*/
/* Input: Pointer to Q
   Output: returns the element of the head(number) anf if Empty returns -1(FAILURE_CODE)
   */
int Top__Queue(Queue* my_Q);
/*========================================================================*/
/* Input: Pointer to Q
   Output: returns if the Q is empty
   */
bool Empty__Queue(Queue* my_Q);
/*========================================================================*/
/*  Input: Pointer to Q
    Frees all resources related to the Q
   */
void Destroy__Queue(Queue* my_Q);

#endif 

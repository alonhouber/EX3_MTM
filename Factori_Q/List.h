#ifndef LIST_H
#define LIST_H

typedef struct list_t {
	int number;
	struct list_t* next;
}list;

list* New__List(int number);
list* Add__ToList(list* head, int number);
void Free__List(list* head);
char* Print__List(list* head, int number, char* list_format_string, int memory_size);
int Get__line_list_length(list* head, int num);
#endif 
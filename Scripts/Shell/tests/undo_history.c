#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

// A C program to demonstrate linked list based implementation of queue 
// A linked list (LL) node to store a queue entry
struct QNode
{
	int16_t type;
    struct QNode *next;
};
 
// The queue, front stores the front node of LL and rear stores ths
// last node of LL
struct Queue
{
    struct QNode *front, *rear;
};
 
// A utility function to create a new linked list node.
struct QNode* newNode(int16_t type)
{
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode));
    temp->type = type;
    temp->next = NULL;
    return temp; 
}
 
// A utility function to create an empty queue
struct Queue *createQueue()
{
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void store_asm(struct Queue **qq, int16_t type)
{
    if (*qq == NULL)
       *qq = createQueue();
 
    // Create a new LL node
    struct QNode *temp = newNode(type);
 
    // If queue is empty, then new node is front and rear both
    if ((*qq)->rear == NULL)
    {
       (*qq)->front = (*qq)->rear = temp;
       return;
    }
 
    // Add the new node at the end of queue and change rear
	temp->next = (*qq)->rear;
    (*qq)->rear = temp;
}
 
struct QNode * load_asm(struct Queue **qq)
{
    if ((*qq) == NULL)
       return NULL;
 
    // If queue is empty, return NULL.
    if ((*qq)->rear == NULL)
       return NULL;
 
    // Store previous front and move front one node ahead
    struct QNode *temp = (*qq)->rear;
    (*qq)->rear = (*qq)->rear->next;
 
    // If front becomes NULL, then change rear also as NULL
    if ((*qq)->rear == NULL)
       (*qq)->front = NULL;
    return temp;
}

int main(void) {
	struct Queue * history;
	history = NULL;
	store_asm(&history, 1);
	store_asm(&history, 2);
	store_asm(&history, 3);
	
    puts("attempting to list node history");
    // obtain the first added instruction
    struct QNode * node = malloc(1); // this gets freed anyway
    int nodes = 0;
    while (node != NULL) {
        // drain the list until empty
        free(node);
        node = load_asm(&history);
		printf("node %d = %p\n", nodes, node);
        if (node == NULL) break;
		if (node) {
			printf("node->type = 0x%02x\n", node->type);
			printf("node->type = %d\n", node->type);
		}
        nodes++;
    }
    if (nodes == 0) puts("i have no history");
    else puts("i have no nodes left in history");
	
	// should result in 3, 2, 1
		
	return 0;
}

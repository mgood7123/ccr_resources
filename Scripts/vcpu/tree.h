#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINKED 1
#define TREE 2
#define NODE_TYPE TREE
bool NODE_DEBUG = false;

struct tree_QNode
{
	int level;
	int index;
	int ref;
	char * data;
	int type;
	struct tree_QNode *next;
	struct tree_QNode* prev; // Pointer to previous node in DLL
#if NODE_TYPE == TREE
	int branch_count;
	struct tree_QNode **branch;
#endif
};

// The queue, front stores the front node of LL and rear stores ths
// last node of LL
struct tree_Queue
{
	struct tree_QNode *front, *rear;
};

ssize_t tree_free_asm(struct tree_Queue **q)
{
	puts("");
	ssize_t frees = 0;
	if ((q) == NULL) return frees;
	if ((*q) == NULL) return frees;
	struct tree_QNode *temp = NULL, *next = NULL;
	temp = (*q)->front;
	while (temp) {
		next = (*q)->front->next;
		frees++;
		free(temp);
		temp = NULL;
		(*q)->front = next;
		temp = next;
	}
	frees++;
	free(*q);
	*q = NULL;
	return frees;
}


// A C program to demonstrate linked list based implementation of queue 
// A linked list (LL) node to store a queue entry
int tree_index_start = 0;
int tree_ref_start = 1;
int tree_global_indent = 0;

#if NODE_TYPE == LINKED
	#define node_init(t, i, r, d, n) t->index = i; t->ref = r; t->data = d; t->next = n; t->level = tree_global_indent; t->prev = NULL;
#elif NODE_TYPE == TREE
	#define node_init(t, i, r, d, n, bc, b) t->index = i; t->ref = r; t->data = d; t->next = n; t->branch_count = bc; t->branch = b; t->level = tree_global_indent; t->prev = NULL;
#endif

#define STORE_NORMAL 1
#define STORE_EMPTY 2

#define NODE_TYPE_NORMAL 1
#define NODE_TYPE_BRANCH_START 2
#define NODE_TYPE_BRANCH_END 3
#define NODE_TYPE_BRANCH_CHOICE_START 4
#define NODE_TYPE_BRANCH_CHOICE_END 5

#define NODE_TYPE_DECODE(type) type==NODE_TYPE_NORMAL?"normal":type==NODE_TYPE_BRANCH_START?"branch start":type==NODE_TYPE_BRANCH_END?"branch end":type==NODE_TYPE_BRANCH_CHOICE_START?"choice start":type==NODE_TYPE_BRANCH_CHOICE_END?"choice end":"NONE"

#define POINTER_LIST_TYPE void *

POINTER_LIST_TYPE ** ptr_list = NULL;
ssize_t ptr_list_size = 0;

int last_ref = 0;

int ptr_add(POINTER_LIST_TYPE * ptr) {
	if (!ptr) return -1;
	if (!ptr_list) {
		ptr_list = malloc(sizeof(*ptr_list)*2);
		ptr_list_size++;
	}
	else if (ptr_list) {
		ptr_list = realloc(ptr_list, sizeof(*ptr_list)*(ptr_list_size+1));
	}
	ptr_list[ptr_list_size-1] = ptr;
	ptr_list_size++;
	ptr_list[ptr_list_size-1] = NULL;
	return 0;
}

int ptr_print(void) {
	if (!ptr_list) return -1;
	POINTER_LIST_TYPE ** pl;
	for (pl = ptr_list; *pl; pl++) pp(*pl);
	return 0;
}

int ptr_set(POINTER_LIST_TYPE data) {
	if (!ptr_list) return -1;
	POINTER_LIST_TYPE ** pl;
	for (pl = ptr_list; *pl; pl++) **pl = data;
	return 0;
}

bool ptr_exists(POINTER_LIST_TYPE data) {
	if (!ptr_list) return false;
	POINTER_LIST_TYPE ** pl;
	for (pl = ptr_list; *pl; pl++) if (*pl == data) return true;
	return false;
}

int ptr_set_free(void) {
	if (!ptr_list) return -1;
	POINTER_LIST_TYPE ** pl;
	for (pl = ptr_list; *pl; pl++) {
		free(*pl);
		*pl = NULL;
	}
	return 0;
}

int ptr_free(void) {
	if (!ptr_list) return -1;
	POINTER_LIST_TYPE ** pl;
	for (pl = ptr_list; *pl; pl++) *pl = NULL;
	free(ptr_list);
	ptr_list = NULL;
	ptr_list_size = 0;
	return 0;
}

// A utility function to create a new linked list node.
struct tree_QNode* tree_newNode(char * data, int type)
{
	struct tree_QNode *temp = (struct tree_QNode*)malloc(sizeof(*temp));
#if NODE_TYPE == LINKED
	if (type == STORE_NORMAL) {
		node_init(temp, tree_index_start++, tree_ref_start++, data, NULL);
	} else if (type == STORE_EMPTY) {
		node_init(temp, 0, 0, NULL, NULL);
	}
#elif NODE_TYPE == TREE
	if (type == STORE_NORMAL) {
		node_init(temp, tree_index_start++, tree_ref_start++, data, NULL, 0, NULL);
	} else if (type == STORE_EMPTY) {
		node_init(temp, 0, 0, NULL, NULL, 0, NULL);
	}
#endif
	return temp;
}

// A utility function to create an empty queue
struct tree_Queue *tree_createQueue()
{
	struct tree_Queue *q = (struct tree_Queue*)malloc(sizeof(*q));
	q->front = q->rear = NULL;
	return q;
}
int prev_type = 0;
int curr_type = 0;
void tree_store_asm(struct tree_Queue *q, char * data, int type)
{
	if (NODE_DEBUG) ps(NODE_TYPE_DECODE(type))
	if (curr_type) prev_type = curr_type;
	curr_type = type;
	// Create a new LL node
	int t = 0;
	if (type == NODE_TYPE_NORMAL) t = STORE_NORMAL;
	else if (
		type == NODE_TYPE_BRANCH_START
		||
		type == NODE_TYPE_BRANCH_END
		||
		type == NODE_TYPE_BRANCH_CHOICE_START
		||
		type == NODE_TYPE_BRANCH_CHOICE_END
			) t = STORE_EMPTY;
	struct tree_QNode *temp = tree_newNode(data, t);
	temp->type = type;

	// If queue is empty, then new node is front and rear both
	if (q->rear == NULL)
	{
		temp->prev = NULL;
		q->front = q->rear = temp;
		return;
	}
	if (prev_type == NODE_TYPE_NORMAL) {
		
		if (type == NODE_TYPE_BRANCH_CHOICE_END) {
			// we reach a the end of a branch choice, add the address of the last reference, the pointer to the next node, to an array of pointers
		}
	}
	else if(type == NODE_TYPE_NORMAL && prev_type == NODE_TYPE_BRANCH_END) {
		// we reach a node and we where previously at a branch, set all referenced logged to the value of the current node reference, then we free the pointer list
	}
	// Add the new node at the end of queue and change rear
#if NODE_TYPE == TREE
	if(type == NODE_TYPE_NORMAL) {
#endif
		temp->prev = q->rear;
		// allocate a new array for our new item
		if (!q->rear->next) {
			if (NODE_DEBUG) printf("allocating node is %p\n", q->rear);
			q->rear->next = malloc(sizeof(*q->rear->next));
		} else {
			puts("next already allocated, aborting");
			abort();
		}
		// update rear->next to point to the new item
		q->rear->next = temp;
		// update rear to point to the rear->next, which is the item we just added, updating the pointer rear from our old item to our new item
		q->rear = q->rear->next;
#if NODE_TYPE == TREE
	}
	else if(type == NODE_TYPE_BRANCH_START || type == NODE_TYPE_BRANCH_CHOICE_START) {
		temp->prev = q->rear;
	// 	struct QNode * p = q->rear;
	// 	if (type == NODE_TYPE_BRANCH_CHOICE_START) {
	// 		while(p) {
	// 			if (p->type == NODE_TYPE_BRANCH_START) {
	// 				puts("FOUND BRANCH START");
	// // 				q->rear = p;
	// 				break;
	// 			}
	// 			if (p->prev) p = p->prev;
	// 			else p = NULL;
	// 		}
	// 	}
		// allocate a new array for our new item
		if (!q->rear->branch) {
			q->rear->branch_count = 1;
			q->rear->branch = malloc(sizeof(*q->rear->branch)*q->rear->branch_count);
		} else {
			q->rear->branch_count++;
			q->rear->branch = realloc(q->rear->branch, sizeof(* q->rear->branch)*q->rear->branch_count);
		}
		// update rear->next to point to the new item
		q->rear->branch[q->rear->branch_count-1] = temp;
		
		// update rear to point to the rear->next, which is the item we just added, updating the pointer rear from our old item to our new item
		q->rear = q->rear->branch[q->rear->branch_count-1];
		if (NODE_DEBUG) pp(q->rear->prev)
		if (NODE_DEBUG) pp(temp->prev)
	}
	else if(type == NODE_TYPE_BRANCH_CHOICE_END) {
		while (q->rear->type != NODE_TYPE_BRANCH_START) {
			if (NODE_DEBUG) printf("jumping from %p (type is %s) to %p (type is %s)\n", q->rear, NODE_TYPE_DECODE(q->rear->type), q->rear->prev, !q->rear->prev?"NULL":NODE_TYPE_DECODE(q->rear->prev->type));
			q->rear = q->rear->prev;
		}
	}
	else if(type == NODE_TYPE_BRANCH_END) {
		if (NODE_DEBUG) printf("jumping from %p (type is %s) to %p (type is %s)\n", q->rear, NODE_TYPE_DECODE(q->rear->type), q->rear->prev, !q->rear->prev?"NULL":NODE_TYPE_DECODE(q->rear->prev->type));
		q->rear = q->rear->prev;
	}
#endif
}

void tree_add(struct tree_Queue **q, char * data) {
	if (!(*q)) (*q) = tree_createQueue();
	tree_store_asm((*q), data, NODE_TYPE_NORMAL);
}

void tree_add_branch(struct tree_Queue **q) {
	tree_global_indent++;
	if (!(*q)) (*q) = tree_createQueue();
	tree_store_asm((*q), 0, NODE_TYPE_BRANCH_START);
	tree_global_indent++;
}

void tree_end_branch(struct tree_Queue **q) {
	tree_global_indent--;
	if (!(*q)) (*q) = tree_createQueue();
	tree_store_asm((*q), 0, NODE_TYPE_BRANCH_END);
	tree_global_indent--;
}

void tree_add_branch_choice(struct tree_Queue **q) {
	if (!(*q)) (*q) = tree_createQueue();
	tree_store_asm((*q), 0, NODE_TYPE_BRANCH_CHOICE_START);
	tree_global_indent++;
}

void tree_end_branch_choice(struct tree_Queue **q) {
	if (!(*q)) (*q) = tree_createQueue();
	tree_store_asm((*q), 0, NODE_TYPE_BRANCH_CHOICE_END);
	tree_global_indent--;
}

struct tree_QNode * tree_load_asm(struct tree_Queue **q)
{
	// If queue is empty, return NULL.
	if ((q) == NULL) return NULL;
	if ((*q) == NULL) return NULL;
	if ((*q)->front == NULL)
	return NULL;

	// Store previous front and move front one node ahead
	struct tree_QNode *temp = (*q)->front;
	// check if front->next is NULL, if NULL it means we have reached the end of the queue
	(*q)->front = (*q)->front->next;
	// If front becomes NULL, then change rear also as NULL
	if ((*q)->front == NULL)
	(*q)->rear = NULL;
	return temp;
}

#define tree_add_indent(indent) { int i = 0; for (; i < indent; i++) printf("    "); }

struct tree_Queue * list = NULL;

void printnode(struct tree_QNode * p) {
	if (!p) return;
	int indentation;
	indentation = p->level;
// 	tree_add_indent(indentation);
// 	pp(p)
// 	tree_add_indent(indentation);
// 	ps(NODE_TYPE_DECODE(p->type))
// 	tree_add_indent(indentation);
// 	pi(p->data)
// 	tree_add_indent(indentation);
// 	pp(p->prev)
// 	tree_add_indent(indentation);
// 	pp(p->next)
#if NODE_TYPE == TREE
#endif
	if (p->type == NODE_TYPE_BRANCH_START) {
		tree_add_indent(indentation);
		printf("branch start\n");
	}
	else if (p->type == NODE_TYPE_BRANCH_END) {
		tree_add_indent(indentation);
		printf("branch end\n");
	}
	else if (p->type == NODE_TYPE_BRANCH_CHOICE_START) {
		tree_add_indent(indentation);
		printf("branch choice start\n");
	}
	else if (p->type == NODE_TYPE_BRANCH_CHOICE_END) {
		tree_add_indent(indentation);
		printf("branch choice end\n");
	}
	else if (p->type == NODE_TYPE_NORMAL) {
		tree_add_indent(indentation);
		printf("%d -> %d (data: %s)\n", p->index, p->ref, p->data);
	}
}

void print_prev(struct tree_QNode * p) {
	puts("NULL");
	while(p) {
		printnode(p);
		if (p->prev) p = p->prev;
		else p = NULL;
	}
}

void print_(struct tree_QNode * p) {
	while(p) {
		printnode(p);
#if NODE_TYPE == LINKED
		p = p->next;
#elif NODE_TYPE == TREE
		if (p->next) p = p->next;
		else p = NULL;
#endif
	}
	puts("NULL");
}

void print(struct tree_QNode * p) {
	int indentation = 0;
	while(p) {
		indentation = p->level;
		printnode(p);
// 		if (p->next) {
// 			tree_add_indent(indentation);
// 			puts("previewing next");
// 			printnode(p->next);
// 			tree_add_indent(indentation);
// 			puts("next previewed");
// 		}
#if NODE_TYPE == TREE
		if (p->branch_count) {
// 			puts("printing branches");
			for (int i = 0; i < p->branch_count; i++) {
				tree_add_indent(indentation+1);
				printf("printing branch %d\n", i);
// 				puts("");
				print(p->branch[i]);
// 				puts("");
				tree_add_indent(indentation+1);
				printf("finished printing branch %d\n", i);
// 				puts("");
			}
		}
#endif
		p = p->next;
	}
	tree_add_indent(indentation);
	puts("NULL");
}

int findcondition1(struct tree_QNode * r) {
	while (r->type != NODE_TYPE_BRANCH_START) {
		ptr_add(r);
		r = r->prev;
		if (!r) break;
	}
	if (r) {
		if (r->type == NODE_TYPE_BRANCH_START && !r->prev->next) {
			ptr_add(r);
			r = r->prev;
			return findcondition1(r);
		}
		return r->prev->next->index;
	}
	return -1;
}

int findcondition2(struct tree_QNode * r, int branch_choice_number) {
	if (r->branch) {
		if (r->branch[branch_choice_number+1]) {
			if (!ptr_exists(r->branch[branch_choice_number+1]) && !ptr_exists(r->branch[branch_choice_number+1]->branch[0])) {
				ptr_free();
				return r->branch[branch_choice_number+1]->branch[0]->next->index;
			}
		}
	}
	while (r->type != NODE_TYPE_BRANCH_START) {
		ptr_add(r);
		r = r->prev;
		if (!r) return -1;
	}
	if (r) {
		if (r->type == NODE_TYPE_BRANCH_START && !r->prev->next) {
			ptr_add(r);
			r = r->prev;
			return findcondition2(r, branch_choice_number);
		}
		ptr_free();
		return -1;
	}
}

int findcondition3(struct tree_QNode * r, int branch_number, int branch_choice_number) {
	bool disabled = false;
	a:
		if (r) {
			if (r->branch) {
				if (r->branch[branch_choice_number+1]) {
					if (ptr_exists(r->branch[branch_choice_number+1]) || ptr_exists(r->branch[branch_choice_number+1]->branch[0])) disabled = true;
					ptr_free();
					if (!disabled) {
						goto b;
					}
				}
			}
		}
		while (r->type != NODE_TYPE_BRANCH_START) {
			ptr_add(r);
			r = r->prev;
			if (!r) break;
		}
		if (r) {
			if (r->type == NODE_TYPE_BRANCH_START && !r->prev->next) {
				ptr_add(r);
				r = r->prev;
				goto a;
			}
			if (r->prev->branch_count > 1) {
// 				pi(branch_number)
// 				pi(r->prev->branch_count)
				if (branch_number+1 != r->prev->branch_count) {
					return r->prev->branch[branch_number+1]->branch[0]->next->index;
				}
			}
		}
	b:
	ptr_free();
	return -1;
}

#if NODE_TYPE == TREE
void fix(struct tree_QNode * p, int depth, int current_branch, int branch_number, int branch_choice_number, int branch_count) {
	int indentation = 0;
	while(p) {
		indentation = p->level;
// 		tree_add_indent(indentation);
		if (p->type == NODE_TYPE_BRANCH_START || p->type == NODE_TYPE_BRANCH_CHOICE_START) {
// 			printf("%s\n", NODE_TYPE_DECODE(p->type));
		}
		else {
// 			printf("%d", p->index);
			if (p->next || p->branch) {
// 				printf(">%d", p->ref);
// 				puts("");
			}
			else {
				int condition_1 = findcondition1(p);
				int condition_2 = findcondition2(p, current_branch);
				int condition_3 = findcondition3(p, branch_number, branch_choice_number);
				if (condition_3 != -1) {
// 					printf(">%d (%d to %d)", condition_3, p->ref, condition_3);
					p->ref = condition_3;
				}
				else if (condition_2 != -1) {
// 					printf(">%d (%d to %d)", condition_2, p->ref, condition_2);
					p->ref = condition_2;
				}
				else if (condition_1 != -1) {
// 					printf(">%d (%d to %d)", condition_1, p->ref, condition_1);
					p->ref = condition_1;
				}
				bool enabled = false;
				if (enabled == true) {
	// 				puts("");
	// 				ptr_free();
	// 				pi(findcondition1(p))
	// 				ptr_free();
	// 				pi(findcondition2(p, current_branch))
	// 				ptr_free();
	// 				pi(findcondition3(p, depth, branch_number, branch_choice_number))
	// 				ptr_free();
					struct tree_QNode * r = p;
					bool disabled = false;
					a:
						if (r) {
	// 						pi(r->prev->branch_count)
							if (r->prev->branch_count > 1) {
	// 							printf("%s\n", NODE_TYPE_DECODE(r->type));
	// 							pi(branch_number)
								if (branch_number+1 != r->prev->branch_count) {
	// 								pi(r->prev->branch[branch_number+1]->branch[0]->next->index)
									printf(">%d (%d to %d)", r->prev->branch[branch_number+1]->branch[0]->next->index, p->ref, r->prev->branch[branch_number+1]->branch[0]->next->index);
	// 								p->ref = r->prev->branch[branch_number+1]->branch[0]->next->index;
								}
							}
							if (r->branch) {
								if (r->branch[branch_choice_number+1]) {
									if (ptr_exists(r->branch[branch_choice_number+1]) || ptr_exists(r->branch[branch_choice_number+1]->branch[0])) disabled = true;
									ptr_free();
									if (!disabled) {
										printf(">%d (%d to %d)", r->branch[branch_choice_number+1]->branch[0]->next->index, p->ref, r->branch[branch_choice_number+1]->branch[0]->next->index);
										p->ref = r->branch[branch_choice_number+1]->branch[0]->next->index;
										goto b;
									}
								}
							}
						}
						while (r->type != NODE_TYPE_BRANCH_START) {
							ptr_add(r);
							r = r->prev;
							if (!r) break;
						}
						if (r) {
	// 						pi(r->prev->branch_count)
							if (r->type == NODE_TYPE_BRANCH_START && !r->prev->next) {
								ptr_add(r);
								r = r->prev;
								goto a;
							}
	// 						printf("%s\n", NODE_TYPE_DECODE(r->type));
	// 						if (r->prev->branch_count > 1) {
	// 							pi(branch_number)
	// 							pi(r->prev->branch_count)
	// 							if (branch_number+1 != r->prev->branch_count) {
	// 								pi(r->prev->branch[branch_number+1]->branch[0]->next->index)
	// 								printf(">%d (%d to %d)", r->prev->branch[branch_number+1]->branch[0]->next->index, p->ref, r->prev->branch[branch_number+1]->branch[0]->next->index);
	// 								p->ref = r->prev->branch[branch_number+1]->branch[0]->next->index;
	// 							}
	// 							else {
	// 								printf(">%d (%d to %d)", r->prev->next->index, p->ref, r->prev->next->index);
	// 								p->ref = r->prev->next->index;
	// 							}
	// 						}
	// 						else {
								printf(">%d (%d to %d)", r->prev->next->index, p->ref, r->prev->next->index);
								p->ref = r->prev->next->index;
	// 						}
						}
				}
				b:
// 				puts("");
				ptr_free();
			}
		}
		if (p->branch_count) {
			for (int i = 0; i < p->branch_count; i++) {
// 				printf("p->type = %s\n", NODE_TYPE_DECODE(p->type));
// 				printf("p->branch[i]->type = %s\n", NODE_TYPE_DECODE(p->branch[i]->type));
				int bn = branch_number;
				int bcn = branch_choice_number;
				if (p->type == NODE_TYPE_NORMAL) bn = i;
				if (p->type == NODE_TYPE_BRANCH_START) bcn = i;
				fix(p->branch[i], depth+1, i, bn, bcn, p->branch_count);
			}
		}
		p = p->next;
	}
}
#endif

int main1(void) {
	tree_index_start = 0;
	tree_ref_start = 1;
	


/* attempt to visualize this in a expression grammar
root ::= 1 A 13
A ::=
	2 3 B 6
	7 8 C
B ::=
	4
C ::=
	9 10 D
D ::=
	11
	
structure:

root: 1 A 13 > NULL
		A: 2 3 B 6 > 13
			B: 4 5 > 6
		A: 7 8 C
			C: 9 10 D
				D: 11 > 13
int l = 1;
	tree_add(&list, l++);
	tree_add_branch(&list);
		tree_add_branch_choice(&list);
			tree_add(&list, l++);
			tree_add(&list, l++);
			tree_add_branch(&list);
				tree_add_branch_choice(&list);
					tree_add(&list, l++);
					tree_add(&list, l++);
				tree_end_branch_choice(&list);
			tree_end_branch(&list);
			tree_add(&list, l++);
		tree_end_branch_choice(&list);
		tree_add_branch_choice(&list);
			tree_add(&list, l++);
			tree_add(&list, l++);
			tree_add_branch(&list);
				tree_add_branch_choice(&list);
					tree_add(&list, l++);
					tree_add(&list, l++);
					tree_add_branch(&list);
						tree_add_branch_choice(&list);
							tree_add(&list, l++);
							tree_add(&list, l++);
						tree_end_branch_choice(&list);
						tree_add_branch_choice(&list);
							tree_add(&list, l++);
							tree_add(&list, l++);
						tree_end_branch_choice(&list);
					tree_end_branch(&list);
				tree_end_branch_choice(&list);
			tree_end_branch(&list);
					tree_add(&list, l++);
		tree_end_branch_choice(&list);
	tree_end_branch(&list);
	tree_add(&list, l++);
/*
  attempt to visualize this in a expression grammar
root ::= 0 A 61
A ::=
	1 2 B
	5 6 C
B ::=
	3 4
C ::=
	7 8 D
D ::=
	9 10 F R
	57 58 S
F ::=
	G 41 42 P 45 46 47
	48 49 Q 52 53 54
G ::=
	11 12 H
	15 16 I
H ::=
	13 14
I ::=
	17 18 J
J ::=
	19 20 K N
	37 38 O
K ::=
	21 22 L 25 26 27
	28 29 M 32 33 34
L ::=
	23 24
M ::=
	 30 31
N ::=
	35 36
O ::=
	39 40
P ::=
	43 44
Q ::=
	50 51
R ::=
	55 56
S ::=
	59 60
	
structure:

root: 0 A 61 > NULL
		A: 1 2 B
			B: 3 4 > 61 // currently 4 > 5
		A: 5 6 C
			C: 7 8 D
				D: 9 10 F R
					F: G 41 42 P 45 46 47 > 55 (R) // currently 47 > 61
						G: 11 12 H
							H: 13 14 > 41
						G: 15 16 I
							I: 16 18 J
								J: 19 20 K N
									K: 21 22 L 25 26 27 > 35 (N) // currently 27 > 41
										L: 23 24 > 25
									K: 28 29 M 32 33 34 > 35 (N)
										M: 30 31 > 32
									N: 35 36 > 41
								J: 37 38 O
									O: 39 40 > 41
						P: 43 44 > 45
					F: 48 49 Q 52 53 54 > 55 (R)
						Q: 50 51 > 52
					R: 55 56 > 61
				D: 57 58 S
					S: 59 60 > 61
	tree_add(&list, 1);
	tree_add_branch(&list);
		tree_add_branch_choice(&list);
			tree_add(&list, 2);
			tree_add(&list, 3);
			tree_add_branch(&list);
				tree_add_branch_choice(&list);
					tree_add(&list, 4);
					tree_add(&list, 5);
				tree_end_branch_choice(&list);
			tree_end_branch(&list);
		tree_end_branch_choice(&list);
		tree_add_branch_choice(&list);
			tree_add(&list, 6);
			tree_add(&list, 7);
			tree_add_branch(&list);
				tree_add_branch_choice(&list);
					tree_add(&list, 8);
					tree_add(&list, 9);
					tree_add_branch(&list);
						tree_add_branch_choice(&list);
							tree_add(&list, 10);
							tree_add(&list, 11);
							tree_add_branch(&list);
								tree_add_branch_choice(&list);
									tree_add_branch(&list);
										tree_add_branch_choice(&list);
											tree_add(&list, 12);
											tree_add(&list, 13);
											tree_add_branch(&list);
												tree_add_branch_choice(&list);
													tree_add(&list, 14);
													tree_add(&list, 15);
												tree_end_branch_choice(&list);
											tree_end_branch(&list);
										tree_end_branch_choice(&list);
										tree_add_branch_choice(&list);
											tree_add(&list, 16);
											tree_add(&list, 17);
											tree_add_branch(&list);
												tree_add_branch_choice(&list);
													tree_add(&list, 18);
													tree_add(&list, 19);
													tree_add_branch(&list);
														tree_add_branch_choice(&list);
															tree_add(&list, 20);
															tree_add(&list, 21);
															tree_add_branch(&list);
																tree_add_branch_choice(&list);
																	tree_add(&list, 22);
																	tree_add(&list, 23);
																	tree_add_branch(&list);
																		tree_add_branch_choice(&list);
																			tree_add(&list, 24);
																			tree_add(&list, 25);
																		tree_end_branch_choice(&list);
																	tree_end_branch(&list);
																	tree_add(&list, 26);
																	tree_add(&list, 27);
																	tree_add(&list, 28);
																tree_end_branch_choice(&list);
																tree_add_branch_choice(&list);
																	tree_add(&list, 29);
																	tree_add(&list, 30);
																	tree_add_branch(&list);
																		tree_add_branch_choice(&list);
																			tree_add(&list, 31);
																			tree_add(&list, 32);
																		tree_end_branch_choice(&list);
																	tree_end_branch(&list);
																	tree_add(&list, 33);
																	tree_add(&list, 34);
																	tree_add(&list, 35);
																tree_end_branch_choice(&list);
															tree_end_branch(&list);
															tree_add_branch(&list);
																tree_add_branch_choice(&list);
																	tree_add(&list, 36);
																	tree_add(&list, 37);
																tree_end_branch_choice(&list);
															tree_end_branch(&list);
														tree_end_branch_choice(&list);
														tree_add_branch_choice(&list);
															tree_add(&list, 38);
															tree_add(&list, 39);
															tree_add_branch(&list);
																tree_add_branch_choice(&list);
																	tree_add(&list, 40);
																	tree_add(&list, 41);
																tree_end_branch_choice(&list);
															tree_end_branch(&list);
														tree_end_branch_choice(&list);
													tree_end_branch(&list);
												tree_end_branch_choice(&list);
											tree_end_branch(&list);
										tree_end_branch_choice(&list);
									tree_end_branch(&list);
									tree_add(&list, 42);
									tree_add(&list, 43);
									tree_add_branch(&list);
										tree_add_branch_choice(&list);
											tree_add(&list, 44);
											tree_add(&list, 45);
										tree_end_branch_choice(&list);
									tree_end_branch(&list);
									tree_add(&list, 46);
									tree_add(&list, 47);
									tree_add(&list, 48);
								tree_end_branch_choice(&list);
								tree_add_branch_choice(&list);
									tree_add(&list, 49);
									tree_add(&list, 50);
									tree_add_branch(&list);
										tree_add_branch_choice(&list);
											tree_add(&list, 51);
											tree_add(&list, 52);
										tree_end_branch_choice(&list);
									tree_end_branch(&list);
									tree_add(&list, 53);
									tree_add(&list, 54);
									tree_add(&list, 55);
								tree_end_branch_choice(&list);
							tree_end_branch(&list);
							tree_add_branch(&list);
								tree_add_branch_choice(&list);
									tree_add(&list, 56);
									tree_add(&list, 57);
								tree_end_branch_choice(&list);
							tree_end_branch(&list);
						tree_end_branch_choice(&list);
						tree_add_branch_choice(&list);
							tree_add(&list, 58);
							tree_add(&list, 59);
							tree_add_branch(&list);
								tree_add_branch_choice(&list);
									tree_add(&list, 60);
									tree_add(&list, 61);
								tree_end_branch_choice(&list);
							tree_end_branch(&list);
						tree_end_branch_choice(&list);
					tree_end_branch(&list);
				tree_end_branch_choice(&list);
			tree_end_branch(&list);
		tree_end_branch_choice(&list);
	tree_end_branch(&list);
	tree_add(&list, 62);
 */
	tree_global_indent = 0;
#if NODE_TYPE == TREE
	fix(list->front, 0, 0, 0, 0, 0);
#endif
// 	print(list->front);
	// this part should be left to the garbage collector to clean up
#if NODE_TYPE == LINKED
	psize_t(free_asm(&list));
#endif
// 	parsemain("new.parse", "new.parse_out");
	return 0;
}

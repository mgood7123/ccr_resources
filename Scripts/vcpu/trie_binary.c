// C implementation of search and insert operations 
// on Trie 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#include "../Shell/builtins/printfmacro.h"
#include "../Shell/builtins/regex_str.h"



// backtracking history
// A C program to demonstrate linked list based implementation of queue 
// A linked list (LL) node to store a queue entry
struct Backtrack_QNode
{
	struct TrieNode *pCrawl;
	struct TrieNode *segments;
	int level;
	int index;
	int idx;
	const char * key;
	int branch_real_index;
	struct Backtrack_QNode *next;
};

// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Backtrack_Queue
{
	struct Backtrack_QNode *front, *rear;
};

// A utility function to create a new linked list node.
struct Backtrack_QNode* Backtrack_newNode(struct TrieNode *pCrawl, struct TrieNode *segments, int level, int index, int idx, const char * key, int branch_real_index)
{
	struct Backtrack_QNode *temp = (struct Backtrack_QNode*)malloc(sizeof(struct Backtrack_QNode));
	temp->pCrawl = pCrawl;
	temp->segments = segments;
	temp->level = level;
	temp->index = index;
	temp->idx = idx;
	temp->key = key;
	temp->branch_real_index = branch_real_index;
	temp->next = NULL;
	return temp; 
}

// A utility function to create an empty queue
struct Backtrack_Queue *Backtrack_createQueue()
{
	struct Backtrack_Queue *q = (struct Backtrack_Queue*)malloc(sizeof(struct Backtrack_Queue));
	q->front = q->rear = NULL;
	return q;
}

void Backtrack_store_asm(struct Backtrack_Queue **qq, struct TrieNode *pCrawl, struct TrieNode *segments, int level, int index, int idx, const char * key, int branch_real_index)
{
	if (*qq == NULL)
	*qq = Backtrack_createQueue();

	// Create a new LL node
	struct Backtrack_QNode *temp = Backtrack_newNode(pCrawl, segments, level, index, idx, key, branch_real_index);

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

struct Backtrack_QNode * Backtrack_load_asm(struct Backtrack_Queue **qq)
{
	if ((qq) == NULL) return NULL;
	if ((*qq) == NULL) return NULL;
	if ((*qq) == NULL) return NULL;

	// If queue is empty, return NULL.
	if ((*qq)->rear == NULL)
	return NULL;

	// Store previous front and move front one node ahead
	struct Backtrack_QNode *temp = (*qq)->rear;
	(*qq)->rear = (*qq)->rear->next;

	// If front becomes NULL, then change rear also as NULL
	if ((*qq)->rear == NULL)
	(*qq)->front = NULL;
	return temp;
}



#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 

// Alphabet size (# of symbols) 
#define ALPHABET_SIZE (26) 

// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 
#define INT_TO_INDEX(c) ((int)c - (int)'0')

// trie node 
struct TrieNode 
{ 
	struct TrieNode *children[ALPHABET_SIZE];
	int * idx;
	int count;
	bool branch;
	// isEndOfWord is true if the node represents 
	// end of a word 
	bool isEndOfWord;
	struct TrieNode ** next;
	int next_count;
}; 

bool is_branch = true;
bool is_not_branch = false;

// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(void) 
{ 
	struct TrieNode *pNode = NULL; 

	pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); 

	if (pNode) 
	{ 
		int i; 

		pNode->idx = NULL;
		pNode->count = 0;
		pNode->branch = false;
		pNode->isEndOfWord = false; 
		pNode->next = NULL;
		pNode->next_count = 0;
		for (i = 0; i < ALPHABET_SIZE; i++) 
			pNode->children[i] = NULL; 
	} 

	return pNode; 
} 

// If not present, inserts key into trie 
// If the key is prefix of trie node, just marks leaf node 
void insert(struct TrieNode *root, const char *key, int dest_idx, struct TrieNode *next)
{ 
	int level; 
	int length = strlen(key); 
	int index;

	struct TrieNode *pCrawl = root;
// 	pi(dest_idx)
	for (level = 0; level < length; level++) 
	{ 
// 		pi(level);
// 		pc(key[level])
		str_new(ch);
		str_insert_char(ch, key[level]);
		if (ch.type & STR_TYPE_DIGIT) index = INT_TO_INDEX(key[level]);
		else if (ch.type & STR_TYPE_ALPHABETIC) index = CHAR_TO_INDEX(key[level]);
		str_free(ch)
// 		pi(index)
// 		pp(pCrawl->children[index])
		if (!pCrawl->children[index]) {
			pCrawl->children[index] = getNode();
		}
		
		if (level == length-1) {
// 			pp(pCrawl->children[index])
// 			pp(pCrawl->children[index]->idx)
			pCrawl->children[index]->next_count++;
			if (!pCrawl->children[index]->next) {
				pCrawl->children[index]->next = malloc(sizeof(int*)*pCrawl->children[index]->next_count);
// 				pp(pCrawl->children[index]->idx)
			}
			else {
				pCrawl->children[index]->next = realloc(pCrawl->children[index]->next, sizeof(int*)*pCrawl->children[index]->next_count);
			}
			pCrawl->children[index]->next[pCrawl->children[index]->next_count-1] = next;
			if (pCrawl->children[index]->next_count > 1) pCrawl->children[index]->branch = true;
			pCrawl->children[index]->count++;
			if (!pCrawl->children[index]->idx) {
				pCrawl->children[index]->idx = malloc(sizeof(int*)*pCrawl->children[index]->count);
// 				pp(pCrawl->children[index]->idx)
			}
			else {
				pCrawl->children[index]->idx = realloc(pCrawl->children[index]->idx, sizeof(int*)*pCrawl->children[index]->count);
			}
			pCrawl->children[index]->idx[pCrawl->children[index]->count-1] = dest_idx;
// 			pi(pCrawl->children[index]->idx[pCrawl->children[index]->count-1])
		}
		
		pCrawl = pCrawl->children[index]; 
	} 

	// mark last node as leaf 
	pCrawl->isEndOfWord = true; 
} 

// Returns true if key presents in trie, else false 
bool search(struct TrieNode **root, const char *key, int idx, bool * branches, int branch_index) 
{ 
	int level; 
	int length = strlen(key); 
	int index; 
	struct TrieNode *pCrawl = *root; 
// 	pi(idx);
	for (level = 0; level < length; level++) 
	{
		str_new(ch);
		str_insert_char(ch, key[level]);
		if (ch.type & STR_TYPE_DIGIT) index = INT_TO_INDEX(key[level]);
		else if (ch.type & STR_TYPE_ALPHABETIC) index = CHAR_TO_INDEX(key[level]);
		str_free(ch)
// 		pi(branch_index)
// 		pi(index)
// 		pi(level)
// 		pc(key[level])
// 		pp(pCrawl)
// 		pp(pCrawl->children)
// 		pp(pCrawl->children[index])
		if (!pCrawl->children[index]) {
			index = CHAR_TO_INDEX('x');
			if (!pCrawl->children[index]) {
				return false;
			}
		}

		pCrawl = pCrawl->children[index]; 
// 		pi(pCrawl->next_count);
		if (branch_index > pCrawl->next_count-1) {
			puts("error, branch index is greater than branch count");
			return false;
		}
// 		pp(pCrawl->next);
// 		pb(pCrawl->branch);
		*branches = pCrawl->branch;
		if (pCrawl->next) {
// 			pp(pCrawl->next[branch_index]);
			if (pCrawl->next[branch_index]) {
// 				puts("modifying root");
				*root = pCrawl->next[branch_index];
			}
		}
	} 

// 	pi(pCrawl->count)
	bool final_match = false;
	for (int i = 0; i < pCrawl->count; i++) {
// 		pi(pCrawl->idx[i])
		if (pCrawl->idx[i] == idx) final_match = true;
	}
// 	pb(final_match)
// 	pb(pCrawl->branch);
	return (pCrawl != NULL && pCrawl->isEndOfWord && final_match); 
} 

// Returns true if key presents in trie, else false 
bool search_segmented(struct TrieNode *root, struct TrieNode *segments, const char *key) 
{ 
	int level; 
	int length = strlen(key); 
	int index; 
	int branch_index = 0;
	int branch_real_index = 0;
	int branch_sub_index = 0;
	struct TrieNode *pCrawl = root; 
	
	str_new(segment);
	int idx = 0;
	int undo = 0;
	bool next_reload = false;
	bool reset_branch_on_next_load = false;
	
	struct Backtrack_Queue * Backtrack_queue = NULL;

	for (level = 0; level < length; level++) 
	{
// 		puts("");
// 			sleep(1);
		if (next_reload) {
			puts("attempting to load last backtrack reference");
			struct Backtrack_QNode * Backtrack_node = NULL;
			Backtrack_node = Backtrack_load_asm(&Backtrack_queue);
// 			pp(Backtrack_node)
			if (Backtrack_node) {
				puts("backtrack reference loaded successfully");
				pp(pCrawl)
				pp(segments)
				pi(level)
				pi(index)
				pi(idx)
				ps(key)
				pi(branch_real_index)
				pp(Backtrack_node->pCrawl)
				pp(Backtrack_node->segments)
				pi(Backtrack_node->level)
				pi(Backtrack_node->index)
				pi(Backtrack_node->idx)
				ps(Backtrack_node->key)
				pi(Backtrack_node->branch_real_index)
				for (int i = 0; i < undo; i++) {
					str_undo_(segment);
				}
				undo = 0;
				pCrawl = Backtrack_node->pCrawl;
				segments = Backtrack_node->segments;
				level = Backtrack_node->level;
				index = Backtrack_node->index;
				idx = Backtrack_node->idx;
				key = Backtrack_node->key;
				branch_real_index = Backtrack_node->branch_real_index+1;
			}
		}
		if (next_reload) {
			branch_index = branch_real_index;
			reset_branch_on_next_load = true;
		}
		else if (reset_branch_on_next_load) {
			branch_index = branch_sub_index;
			reset_branch_on_next_load = false;
		}
		else branch_index = branch_sub_index;
// 		pi(level)
// 		pi(branch_index)
// 		pi(branch_sub_index)
// 		pi(branch_real_index)
// 		pc(key[level])
		str_new(ch);
		str_insert_char(ch, key[level]);
		if (ch.type & STR_TYPE_DIGIT) index = INT_TO_INDEX(key[level]);
		else if (ch.type & STR_TYPE_ALPHABETIC) index = CHAR_TO_INDEX(key[level]);
		str_free(ch)
		
		str_insert_char(segment, key[level]);
		
// 		ps(segment.string);
// 		pp(segments)
		bool branches = false;
		if (branch_index) printf("selecting chain %d\n", branch_index);
// 		sleep(1);
		struct TrieNode *segments_prev = segments, *segments_curr = segments;
// 		pp(segments_curr)
// 		pp(segments_prev)
		bool result1 = search(&segments, segment.string, idx, &branches, branch_index);
		segments_curr = segments;
// 		pp(segments_curr)
// 		pp(segments_prev)
// 		pb(branches)
		if (branches) {
			// store a backtrack reference to this branch
			puts("creating backtrack reference");
			pp(pCrawl)
			pp(segments_curr)
			pp(segments_prev)
			pi(level)
			pi(index)
			pi(idx)
			ps(key)
			pi(branch_real_index)
			Backtrack_store_asm(&Backtrack_queue, pCrawl, segments_prev, level, index, idx, key, 0);
			puts("backtrack reference created successfully");
		}
// 		pp(segments)
// 		pi(idx)
// 		pb(result1);
		undo++;
		next_reload = false;
		if (result1) {
			printf("match %c\n", key[level]);
			str_reset(segment);
			idx++;
			undo = 0;
		}
		else {
			printf("fail %c\n", key[level]);
			next_reload = true;
			level--;
			continue;
		}
// 		sleep(5);
// 		pi(idx)
		
		if (!pCrawl->children[index]) {
			index = CHAR_TO_INDEX('x');
			if (!pCrawl->children[index]) return false; 
		}

		pCrawl = pCrawl->children[index]; 
	}
	str_free(segment);
	struct Backtrack_QNode * Backtrack_node = malloc(1);
	int Backtrack_Nodes = 0;
	while (Backtrack_node != NULL) {
		// drain the list until empty
		free(Backtrack_node);
		Backtrack_node = Backtrack_load_asm(&Backtrack_queue);
		if (Backtrack_node == NULL) break;
		Backtrack_Nodes++;
	}
	printf("freed %d node%s\n", Backtrack_Nodes, Backtrack_Nodes==1?"":"s");
	return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 

#define isp(root, segments, c) printf("%s --- %s\n", c, output[search_segmented(root, segments, c)] )

// Driver 
int main() 
{ 
	char output[][32] = {"Not present in trie", "Present in trie"}; 

/*

                INPUT:
def0:
35:NULL
85:NULL
def1:
91:NULL
92:NULL

rule1 : |1|2|def0|def1|6|

                TARGET OUTPUT:
    insert(sub_root, "1", 0, rule_0);
        insert(rule_0, "2", 1, rule_1); // restore point
                        insert(rule_1, "3", 2, rule_2);
                        insert(rule_2, "5", 3, rule_3);
        insert(rule_0, "2", 1, rule_4); // restore point
                        insert(rule_4, "8", 2, rule_5);
                        insert(rule_5, "5", 3, rule_3);
        insert(rule_3, "9", 4, rule_4); // restore point
                        insert(rule_4, "1", 5, rule_5);
        insert(rule_3, "9", 4, rule_7); // restore point
                        insert(rule_7, "2", 5, rule_5);
        insert(rule_5, "6", 6, NULL);

        
        
	
	// Construct trie 
	insert(segments, "0011", 0, segments1);
	pp(segments->next)
	insert(segments1, "001", 1, NULL);
	insert(segments1, "0", 2, NULL);
	insert(segments1, "0", 3, NULL);
	insert(root, "001100100", 0, NULL);

	// Search for different keys 
	isp(root, segments, "001100100");
*/

	// Construct trie
	
/* shall aim to be equivilant to this

rule_1 ::= "1" "0";
rule_2 ::= "0" "1";
rule_3 ::= "0" "0";

*/
	// rule 1
	
	// when adding a new rule indexing resets to 0
	
	struct TrieNode *root = getNode(); 
	struct TrieNode *sub_root = getNode(); 
	struct TrieNode *rule_0 = getNode();
	struct TrieNode *rule_1 = getNode();
	struct TrieNode *rule_2 = getNode(); 
	struct TrieNode *rule_3 = getNode(); 
	struct TrieNode *rule_4 = getNode(); 
	struct TrieNode *rule_5 = getNode(); 
	struct TrieNode *rule_6 = getNode(); 
	struct TrieNode *rule_7 = getNode(); 
	struct TrieNode *rule_8 = getNode(); 
	struct TrieNode *rule_9 = getNode(); 
	struct TrieNode *rule_10 = getNode(); 
	struct TrieNode *rule_11 = getNode(); 
	struct TrieNode *rule_12 = getNode(); 
	struct TrieNode *rule_13 = getNode(); 
	struct TrieNode *rule_14 = getNode(); 

// 	insert(root, "12356", 0, NULL);
// 	insert(root, "12856", 0, NULL);
// 	insert(sub_root, "1", 0, rule_1);
// 		insert(rule_1, "2", 1, rule_2);
// 			insert(rule_2, "3", 2, rule_3);
// 			insert(rule_3, "5", 3, rule_4);
// 		insert(rule_1, "2", 1, rule_13);
// 			insert(rule_13, "8", 2, rule_14);
// 			insert(rule_14, "5", 3, rule_4);
// 		insert(rule_4, "6", 4, NULL);
// 	
// 	insert(root, "12456", 0, NULL);
// 	insert(sub_root, "1", 0, rule_5);
// 		insert(rule_5, "2", 1, rule_6);
// 		insert(rule_6, "4", 2, rule_7);
// 		insert(rule_7, "5", 3, rule_8);
// 		insert(rule_8, "6", 4, NULL);
// 		
// 	insert(root, "12457", 0, NULL);
// 	insert(sub_root, "1", 0, rule_9);
// 		insert(rule_9, "2", 1, rule_10);
// 		insert(rule_10, "4", 2, rule_11);
// 		insert(rule_11, "5", 3, rule_12);
// 		insert(rule_12, "7", 4, NULL);
// combinations go in the `root` trie, segments go in subsequent trie`s
insert(root, "1235916", 0, NULL);
insert(root, "1235926", 0, NULL);
insert(root, "1285916", 0, NULL);
insert(root, "1285926", 0, NULL);
    insert(sub_root, "1", 0, rule_0);
        insert(rule_0, "2", 1, rule_1);
			insert(rule_1, "3", 2, rule_2);
			insert(rule_2, "5", 3, rule_3);
        insert(rule_0, "2", 1, rule_4);
			insert(rule_4, "8", 2, rule_5);
			insert(rule_5, "5", 3, rule_3);
        insert(rule_3, "9", 4, rule_4);
			insert(rule_4, "1", 5, rule_5);
        insert(rule_3, "9", 4, rule_7);
			insert(rule_7, "2", 5, rule_5);
        insert(rule_5, "6", 6, NULL);
// // i:
// 	// s1
// 	insert(sub_root, "1", 0, rule_0);
// 		// s2, b, restore point
// 		insert(rule_0, "2", 1, rule_1);
// 			// C
// 				// s3
// 				insert(rule_1, "3", 2, rule_2);
// 				// s5
// 				insert(rule_2, "5", 3, rule_3); // link to next statement, which happens to be a definition
// 			// C
// 			// s2, restore point
// 			insert(rule_0, "2", 1, rule_4);
// 				// s8
// 				insert(rule_4, "8", 2, rule_5);
// 				// s5
// 				insert(rule_5, "5", 3, rule_3); // link to next statement, which happens to be a definition
// 		// e
// 		// s9, b, restore point, because that this is directly after a previous branch the restore point is set to the start of this branch
// 			// C
// 				// s9
// 				insert(rule_3, "9", 4, rule_6);
// 				// s1
// 				insert(rule_6, "1", 5, rule_8); // link to end
// 			// C
// 				// s9, restore point
// 				insert(rule_3, "9", 4, rule_7);
// 				// s9
// // 				insert(rule_6, "9", 5, rule_7);
// 				// s2
// 				insert(rule_7, "2", 5, rule_8); // link to end
// 		// s6, end of rule
// 		insert(rule_8, "6", 6, NULL);

	// Search for different keys
	// > means pass
	// < means backtrack to
	isp(root, sub_root, "1235916");
	isp(root, sub_root, "1235926");
	isp(root, sub_root, "1285916");
	isp(root, sub_root, "1285926");
// 	isp(root, sub_root, "12356"); // 1>2>3>5>6
// 	isp(root, sub_root, "12856"); // 1>2>3<2>8>5>6
// 	isp(root, sub_root, "12456"); // 1>2>3<2>8<2<1>2>4>5>6
// 	isp(root, sub_root, "12457"); // 1>2>3<2>8<2<1>2>4>5>6<1>2>4>5>7
	return 0; 
} 

/*

adding rule 1: segments [0, 1, 0] root [010]
adding rule 2: segments [0, 1, 1] root [011]
segment.string = 0
result = true
segment.string = 1
result = true
segment.string = 0
result = false
010 --- Present in trie
segment.string = 0
result = true
segment.string = 1
result = true
segment.string = 1
result = true
011 --- Present in trie

*/

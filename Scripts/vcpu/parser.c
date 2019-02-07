#include "../Shell/builtins/printfmacro.h"
#include "mpc/mpc.h"
#include "mpc/mpc.c"
#include <stdbool.h>
#include "../Shell/builtins/libstring.h"
#include "../Shell/builtins/regex_str.h"
#include "../Shell/builtins/env.h"

// #define evalfile_
#define evalfileI_

#ifdef evalfile_
static void AP(mpc_ast_t *a, int d, FILE *fp) {
	
	int i;
	
	if (a == NULL) {
		fprintf(fp, "NULL\n");
		return;
	}
	
	for (i = 0; i < d; i++) { fprintf(fp, "    "); }
	if (strlen(a->contents)) {
		fprintf(fp, "tag: '%s', contents: '%s'\n", a->tag, 
			a->contents);
	} else {
		fprintf(fp, "tag: '%s' \n", a->tag);
	}
	
	for (i = 0; i < a->children_num; i++) {
		AP(a->children[i], d+1, fp);
	}
	
}
#endif

#ifdef evalfileI_
char * outfile = "tmpfile.c";
int find_definition_strings_called = 0;

#include "tree.h"

int instruction_index = -1;
int rule_index = -1;
int chain_index = -1;
bool passed_first = false;
bool passed_next = false;
int actual_index = 0;




// A C program to demonstrate linked list based implementation of queue 
// A linked list (LL) node to store a queue entry
struct trie_QNode
{
	char * rule_name;
	int rule_name_index;
	char * key;
	int index;
	char * rule_next;
	int rule_next_index;
	env_t speculative_code;
	env_t action_code;
    struct trie_QNode *next;
};
 
// The queue, front stores the front node of LL and rear stores ths
// last node of LL
struct trie_Queue
{
    struct trie_QNode *front, *rear;
};
 
// A utility function to create a new linked list node.
struct trie_QNode* trie_newNode(char * a, int b, char * c, int d, char * e, int f)
{
    struct trie_QNode *temp = (struct trie_QNode*)malloc(sizeof(struct trie_QNode));
	temp->rule_name = a;
	temp->rule_name_index = b;
	temp->key = c;
	temp->index = d;
	temp->rule_next = e;
	temp->rule_next_index = f;
    temp->next = NULL;
	temp->speculative_code = NULL;
	temp->action_code = NULL;
    return temp;
}
 
// A utility function to create an empty queue
struct trie_Queue *trie_createQueue()
{
    struct trie_Queue *q = (struct trie_Queue*)malloc(sizeof(struct trie_Queue));
    q->front = q->rear = NULL;
    return q;
}

void trie_store_asm(struct trie_Queue *q, char * rule_name, int rule_name_index, char * key, int index, char * rule_next, int rule_next_index)
{
    // Create a new LL node
    struct trie_QNode *temp = trie_newNode(rule_name,rule_name_index,key,index,rule_next,rule_next_index);
 
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
       q->front = q->rear = temp;
		q->rear->rule_next = NULL;
		q->rear->rule_next_index = -1;
       return;
    }
 
    // Add the new node at the end of queue and change rear
    if (q->rear) {
		q->rear->rule_next = rule_name;
		q->rear->rule_next_index = rule_name_index;
	}
    q->rear->next = temp;
    q->rear = temp;
    if (q->rear) {
		q->rear->rule_next = NULL;
		q->rear->rule_next_index = -1;
	}
}

void trie_store_asm2(struct trie_Queue *q, char * rule_name, int rule_name_index, char * key, int index, char * rule_next, int rule_next_index)
{
    // Create a new LL node
    struct trie_QNode *temp = trie_newNode(rule_name,rule_name_index,key,index,rule_next,rule_next_index);
 
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
       q->front = q->rear = temp;
       return;
    }
 
    // Add the new node at the end of queue and change rear
    if (q->rear) {
		q->rear->rule_next = rule_name;
		q->rear->rule_next_index = rule_name_index;
	}
    q->rear->next = temp;
    q->rear = temp;
}

void trie_store_asm3(struct trie_Queue *q, char * rule_name, int rule_name_index, char * key, int index, char * rule_next, int rule_next_index)
{
    // Create a new LL node
    struct trie_QNode *temp = trie_newNode(rule_name,rule_name_index,key,index,rule_next,rule_next_index);
 
    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
       q->front = q->rear = temp;
       return;
    }
 
    // Add the new node at the end of queue and change rear
    q->rear->next = temp;
    q->rear = temp;
}

struct trie_QNode * trie_load_asm(struct trie_Queue **q)
{
    // If queue is empty, return NULL.
	if ((q) == NULL) return NULL;
	if ((*q) == NULL) return NULL;
    if ((*q)->front == NULL)
       return NULL;
 
    // Store previous front and move front one node ahead
    struct trie_QNode *temp = (*q)->front;
    (*q)->front = (*q)->front->next;
 
    // If front becomes NULL, then change rear also as NULL
    if ((*q)->front == NULL)
       (*q)->rear = NULL;
    return temp;
}

int trie_queue_add(struct trie_Queue **q, char * rule_name, int rule_name_index, char * key, int index, char * rule_next, int rule_next_index) {
    if (!(*q)) (*q) = trie_createQueue();
    trie_store_asm((*q), rule_name,rule_name_index,key,index,rule_next,rule_next_index);
    return 0;
}

int trie_queue_add2(struct trie_Queue **q, char * rule_name, int rule_name_index, char * key, int index, char * rule_next, int rule_next_index) {
    if (!(*q)) (*q) = trie_createQueue();
    trie_store_asm2((*q), rule_name,rule_name_index,key,index,rule_next,rule_next_index);
    return 0;
}

int trie_queue_add3(struct trie_Queue **q, char * rule_name, int rule_name_index, char * key, int index, char * rule_next, int rule_next_index) {
    if (!(*q)) (*q) = trie_createQueue();
    trie_store_asm3((*q), rule_name,rule_name_index,key,index,rule_next,rule_next_index);
    return 0;
}

char * trie_root = "root";
char * trie_sub_root = "sub_root";
char * trie_rule_prefix = "rule_";

struct trie_Queue * trie_trie = NULL;


void piece2(mpc_ast_t *a, bool is_segmented, bool is_word, bool is_number, struct regex_string * b1, struct regex_string * b2) {
	str_reg(a->tag);
// 	if (is_segmented) ps(a->tag)
// 	if ((((str_is("word|regex") || str_is("word_segmented|word|regex"))) || ((str_is("char") || str_is("number|regex") || str_is("number_segmented|number|regex"))))) {
// 		puts("START");
// 		pb(is_word)
// 		pb(is_number)
// 		pb(is_segmented)
// 		ps(a->tag)
// 		puts("END");
// 	}
	if(str_is("name_segmented|>") || str_is("indexedname_segmented|>")) {
// 		puts("attempting to piece together segmented name");
		int i = 0;
		for (i = 0; i < a->children_num; i++) {
			piece2(a->children[i], true, true, false, b1, b2);
		}
// 		puts("attempted");
	}
	else if ((((str_is("word|regex") || str_is("word_segmented|word|regex")) && is_word) || ((str_is("char") || str_is("number|regex") || str_is("number_segmented|number|regex")) && is_number)) && is_segmented) {
		if (a->contents != NULL) {
// 			ps(a->contents);
			if (is_number) {
				if (b1 != NULL) {
					if (a->contents[0] != '#') str_insert_string(*b1, a->contents)
				}
			}
			else if(is_word) {
				if (b2 != NULL) {
					str_insert_string(*b2, a->contents)
				}
			}
		}
	}
	else if (str_is("word_segmented|>") && is_segmented) {
// 		puts("attempting to piece together segmented word");
		int i = 0;
		for (i = 0; i < a->children_num; i++) {
			piece2(a->children[i], is_segmented, true, false, b1, b2);
		}
// 		puts("attempted");
	}
	else if (str_is("num_segmented|>")) {
// 		puts("attempting to piece together segmented number");
		int i = 0;
		for (i = 0; i < a->children_num; i++) {
			piece2(a->children[i], true, false, true, b1, b2);
		}
// 		puts("attempted");
	}
	else if (str_is("number_segmented|>") && is_segmented) {
// 		puts("attempting to piece together segmented number");
		int i = 0;
		for (i = 0; i < a->children_num; i++) {
			piece2(a->children[i], is_segmented, false, true, b1, b2);
		}
// 		puts("attempted");
	}
}

bool find_tree_string(mpc_ast_t *a, char * string) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->contents, string) == 0) {
// 		str_new(tmp);
// 		str_insert_string(tmp, "puts(\"");
// 		str_insert_string(tmp, a->contents);
// 		str_insert_string(tmp, " \");\n");
// 		str_output_append(tmp.indented, outfile);
// 		str_free(tmp);
		return true;
	}
	return false;
}

bool find_tree_string2(mpc_ast_t *a) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "ISA_DEFINE|word|regex") == 0 || strcmp(a->tag, "word|regex") == 0 ) {
// 		ps(a->contents);
// 		str_new(tmp);
// 		str_insert_string(tmp, "puts(\"");
// 		str_insert_string(tmp, a->contents);
// 		str_insert_string(tmp, " \");\n");
// 		str_output_append(tmp.indented, outfile);
// 		str_free(tmp);
		return true;
	} else if (strcmp(a->tag, "ISA_DEFINE|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			if (find_tree_string2(a->children[i]) == true) return true;
		}
	}
	return false;
}

bool find_tree_string3(mpc_ast_t *a, struct regex_string * b1) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	
	if (strcmp(a->tag, "hash|char") == 0) {
		str_insert_string(*b1, a->contents);
		return false;
	}
	if (strcmp(a->tag, "ISA_DEFINE|word|regex") == 0 || strcmp(a->tag, "word|regex") == 0) {
		str_insert_string(*b1, a->contents);
		return true;
	} else if (strcmp(a->tag, "ISA_DEFINE|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			if (find_tree_string3(a->children[i], b1) == true) return true;
		}
	}
	return false;
}

bool find_definition_root_(mpc_ast_t *a, struct regex_string * aa) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "definition_start|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			if (find_tree_string3(a->children[i], aa) == true) {
				return true;
			}
		}
	}
	else {
		for (i = 0; i < a->children_num; i++) {
			find_definition_root_(a->children[i], aa);
		}
	}
	return false;
}
int final_rule = 0;
int rule_index_tmp = 0;
bool find_definition_strings(mpc_ast_t *a, bool omit_else) {
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "word|regex") == 0) {
		find_definition_strings_called++;
		pi(find_definition_strings_called)
		tree_add_branch_choice(&list);
// 		printf("%s\n", a->contents);
		ps(a->contents);
		int actual_index_tmp = 0;
		char * zrule_name;
		int zrule_name_index;
		char * zkey;
		int zindex;
		if (find_definition_strings_called == 1) {
			zrule_name = trie_trie->rear->rule_name;
			zrule_name_index = trie_trie->rear->rule_name_index;
			zkey = trie_trie->rear->key;
			zindex = trie_trie->rear->index;
			rule_index_tmp = rule_index+1;
			final_rule = rule_index_tmp;
		}
		else if (find_definition_strings_called == 2) rule_index_tmp++;
		actual_index_tmp = actual_index+1;
		printf("insert(%s%d, \"%s\", %d, %s%d);\n", zrule_name, zrule_name_index, zkey, zindex, trie_rule_prefix, rule_index_tmp-(find_definition_strings_called==1?0:1));
		trie_queue_add3(&trie_trie, zrule_name, zrule_name_index, strdup(zkey), zindex, trie_rule_prefix, rule_index_tmp-(find_definition_strings_called==1?0:1));
		for (int level = 0; a->contents[level]; level++) {
			str_new(ch);
			str_insert_char(ch, a->contents[level])
			tree_add(&list, strdup(ch.string));
			printf("insert(%s%d, \"%s\", %d, %s%d);\n", trie_rule_prefix, rule_index_tmp-1, ch.string, actual_index_tmp, trie_rule_prefix, a->contents[level+1]?rule_index_tmp:final_rule);
// 			if (level == 0) {
// 				puts("add");
				if (find_definition_strings_called == 1) {
					if (a->contents[level+1]) {
						puts("RULE");
						trie_queue_add(&trie_trie, trie_rule_prefix, rule_index_tmp-1, strdup(ch.string), actual_index_tmp, trie_rule_prefix, rule_index_tmp);
					} else {
						puts("FINAL");
						trie_queue_add2(&trie_trie, trie_rule_prefix, rule_index_tmp-1, strdup(ch.string), actual_index_tmp, trie_rule_prefix, final_rule);
					}
				} else {
						puts("RULE FINAL");
					trie_queue_add3(&trie_trie, trie_rule_prefix, rule_index_tmp-1, strdup(ch.string), actual_index_tmp, trie_rule_prefix, a->contents[level+1]?rule_index_tmp:final_rule);
				}
				pp(trie_trie->rear->rule_next)
			str_free(ch);
			rule_index_tmp++;
			pi(rule_index_tmp)
			if (find_definition_strings_called == 1 && a->contents[level+1]) final_rule++;
			actual_index_tmp++;
// 			pp(trie_trie->rear)
// 			pp(trie_trie->rear->rule_name)
// 			ps(trie_trie->rear->rule_name)
// 			pi(trie_trie->rear->rule_name_index)
// 			pp(trie_trie->rear->key)
// 			ps(trie_trie->rear->key)
// 			pi(trie_trie->rear->index)
// 			pp(trie_trie->rear->rule_next)
// 			ps(trie_trie->rear->rule_next)
// 			pi(trie_trie->rear->rule_next_index)
		}
		pi(final_rule)
// 		str_new(tmp);
// 		if (omit_else == false) if (find_definition_strings_called >1) str_insert_string(tmp, "else ");
// 		str_insert_string(tmp, "if (!");
// 		str_insert_string(tmp, "regexEngineb(");
// 		str_insert_int(tmp, strlen(a->contents));
// 		str_insert_string(tmp, ", ");
// 		str_insert_int(tmp, strlen(a->contents));
// 		str_insert_string(tmp, ", \"");
// 		if (strlen(a->contents) != 1) {
// 			str_insert_int(tmp, strlen(a->contents)-1);
// 			str_insert_string(tmp, "-");
// 		}
// 		str_insert_string(tmp, "0,");
// 		str_insert_string(tmp, a->contents);
// 		str_insert_string(tmp, "\", binstr.data, ");
// 		if (find_definition_strings_called >1) { str_insert_string(tmp, "true"); }
// 		else { str_insert_string(tmp, "false"); }
// 		str_insert_string(tmp, ")");
// 		str_insert_string(tmp, ") {");
// // 		str_insert_string(tmp, "printf(\"a->contents = ");
// // 		str_insert_string(tmp, a->contents);
// // 		str_insert_string(tmp, ", regexEngineb_bits = %s\\n\", regexEngineb_bits);\n");
// 		str_insert_string(tmp, "}");
// 		str_output_append(tmp.indented, outfile);
// 		str_free(tmp);
		tree_end_branch_choice(&list);
	}

	return false;
}

bool find_definition_items(mpc_ast_t *a) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "item|bracketed_item|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			find_definition_strings(a->children[i], true);
		}
	}
	else if (strcmp(a->tag, "item|normal_item|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			find_definition_strings(a->children[i], false);
		}
	}
	else {
		for (i = 0; i < a->children_num; i++) {
			find_definition_items(a->children[i]);
		}
	}
	return false;
}

bool find_definition_statements(mpc_ast_t *a) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "definition_statement|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			find_definition_items(a->children[i]);
		}
	}
	else {
		for (i = 0; i < a->children_num; i++) {
			find_definition_statements(a->children[i]);
		}
	}
	return false;
}

bool find_definition_root(mpc_ast_t *a, char * string) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "root|definition|>") == 0) {
		find_definition_strings_called = 0;
		for (i = 0; i < a->children_num; i++) {
			if (find_definition_root(a->children[i], string) == true) {
				find_definition_statements(a);
			}
		}
		find_definition_strings_called = 0;
	}
	else if (strcmp(a->tag, "definition_start|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			if (find_tree_string(a->children[i], string) == true) return true;
		}
	}
	else {
		for (i = 0; i < a->children_num; i++) {
			find_definition_root(a->children[i], string);
		}
	}
	return false;
}

bool find_instruction_root_(mpc_ast_t *a, struct regex_string * aa) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "root|instruction|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			find_instruction_root_(a->children[i], aa);
		}
	}
	else if (strcmp(a->tag, "instruction_start|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			if (find_tree_string3(a->children[i], aa) == true) {
				return true;
			}
		}
	}
	else {
		for (i = 0; i < a->children_num; i++) {
			find_instruction_root_(a->children[i], aa);
		}
	}
	return false;
}

bool find_instruction_root(mpc_ast_t *a) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "root|instruction|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			find_instruction_root(a->children[i]);
		}
	}
	else if (strcmp(a->tag, "instruction_start|>") == 0) {
		for (i = 0; i < a->children_num; i++) {
			if (find_tree_string2(a->children[i]) == true) return true;
		}
	}
	else {
		for (i = 0; i < a->children_num; i++) {
			find_instruction_root(a->children[i]);
		}
	}
	return false;
}

void aquire_action_code(mpc_ast_t *a, struct regex_string * r) {
	if (a->children_num) {
		str_reg(a->children[0]->tag);
		if (str_is("name|>") || str_is("num|>") || str_is("indexedname|>") || str_is("name_segmented|>") || str_is("num_segmented|>") || str_is("indexedname_segmented|>")) return;
	}
// 	str_insert_string(*r, "\n/*ACTION CODE BEGIN*/");
	int i = 0;
	for (i = 0; i < a->children_num; i++) {
		str_reg(a->children[i]->tag);
		if (str_is("regex")) {
			if (a->children[i]->contents != NULL) {
				str_insert_string(*r, a->children[i]->contents);
			}
		}
	}
// 	str_insert_string(*r, "/*ACTION CODE END*/\n");
}

struct action_code_builder_object_references {
	int argv_id; // argv index
	int * content_id; // content index indexes
	int content_amount;
	int occurence_number;
	int instances;
};

struct action_code_builder_references {
	int argc;
	env_t argv;
	env_t tag;
	struct action_code_builder_object_references * ref;
	struct action_code_builder_content * content;
	int 	instances;
};

struct action_code_builder_content {
	struct regex_string content; // content of action code
	int instances;
};

struct action_code_builder {
	struct action_code_builder_references * references;
	int instances;
} * action_code_builder_struct;

int action_code_builder_argument_index = 0;

void Build_Action_Code_Structure_Quick_Scan(mpc_ast_t *a, int * Build_Action_Code_Structure_Quick_Scan_Matches) {
	int i;
	
	if (a == NULL) {
		return;
	}

	bool is_an_action_structure = false;
	bool is_an_action_segmented_structure = false;
	bool has_tag = false;
	
	if (strlen(a->tag)) has_tag = true;
	
	if (has_tag) {
		if (strcmp(a->tag, "action_code|>") == 0) is_an_action_structure = true;
		else if (strcmp(a->tag, "action_code_segmented|>") == 0) is_an_action_segmented_structure = true;
	}
	
	if (!is_an_action_structure && !is_an_action_segmented_structure) for (i = 0; i < a->children_num; i++) {
		Build_Action_Code_Structure_Quick_Scan(a->children[i], Build_Action_Code_Structure_Quick_Scan_Matches);
	}
	else (*Build_Action_Code_Structure_Quick_Scan_Matches)++;
}

struct binder {
	int bind_index;
	int bind_index_old;
	bool bind_index_updated;
	bool bind_segmented;
	int bind_type;
	char * target_content_name;
	int instances;
} * action_code_binder;
int action_code_binder_skip_next = 0;
#define BIND_NONE -1
#define BIND_NAME 1
#define BIND_NUM 2
#define BIND_FUSED 3

int number_ifs = 0;
mpc_ast_t *ra = NULL;
int instructions = 0;
mpc_ast_t *prev;
bool isimportant = false;

int tag_index = 0;
bool has_action_structures = false;

void Build_Action_Code_Structure3(mpc_ast_t *a, int d, bool is_sub_contents_part_of_action, bool renew) {
	if (renew) {
		if (action_code_builder_struct == NULL) {
			action_code_builder_struct = malloc(sizeof(struct action_code_builder)*1);
			action_code_builder_struct->instances = 1;

			action_code_builder_struct[action_code_builder_struct->instances-1].references = malloc(sizeof(struct action_code_builder_references)*1);
			action_code_builder_struct[action_code_builder_struct->instances-1].references->instances = 1;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->argc = 0;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->argv = NULL;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->tag = NULL;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->content = NULL;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref = malloc(sizeof(struct action_code_builder_object_references)*1);
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->instances = 1;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->argv_id = -1;
		}
		else {
			action_code_builder_struct = realloc(action_code_builder_struct, sizeof(struct action_code_builder)*(action_code_builder_struct->instances+1));
			action_code_builder_struct->instances++;
			
			action_code_builder_struct[action_code_builder_struct->instances-1].references = malloc(sizeof(struct action_code_builder_references)*1);
			action_code_builder_struct[action_code_builder_struct->instances-1].references->instances = 1;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->argc = 0;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->argv = NULL;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->tag = NULL;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->content = NULL;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref = malloc(sizeof(struct action_code_builder_object_references)*1);
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->instances = 1;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->argv_id = -1;
		}
	}
	int i;
	
	if (a == NULL) {
		return;
	}
	bool is_an_action = false;
	bool is_an_action_structure = false;
	bool is_an_action_segmented_structure = false;
	bool has_tag = false;
	bool has_contents = false;
	
	if (strlen(a->tag)) has_tag = true;
	if (strlen(a->contents)) has_contents = true;
		
	if (has_tag) {
		if (strcmp(a->tag, "action|>") == 0) is_an_action = true;
		else if (strcmp(a->tag, "action_code|>") == 0) is_an_action_structure = true;
		else if (strcmp(a->tag, "action_code_segmented|>") == 0) is_an_action_segmented_structure = true;
		
		if (is_an_action_segmented_structure || is_an_action_structure) is_sub_contents_part_of_action = true;
		
		if (!is_sub_contents_part_of_action || is_an_action_structure || is_an_action_segmented_structure) {
			if (action_code_binder != NULL) {
				free(action_code_binder);
				action_code_binder = NULL;
			}
		}
		
		if (is_sub_contents_part_of_action == false && (strcmp(a->tag, "ISA_DEFINE|word|regex") == 0 || strcmp(a->tag, "ISA_RULE|word|regex") == 0 || strcmp(a->tag, "word|regex") == 0 || strcmp(a->tag, "binary|regex") == 0)) {
			if (action_code_builder_struct[action_code_builder_struct->instances-1].references->ref != NULL && action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->argv_id != -1) {
				action_code_builder_struct[action_code_builder_struct->instances-1].references->ref = realloc(
					action_code_builder_struct[action_code_builder_struct->instances-1].references->ref,
					sizeof(
						struct action_code_builder_object_references
					)
					*
					(
						action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->instances+1
					)
				);
				action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->instances++;
			}
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->instances-1].content_id = NULL;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->instances-1].content_amount = 0;
			
			action_code_builder_struct[action_code_builder_struct->instances-1].references->argc++;
			
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->instances-1].argv_id = action_code_builder_struct[action_code_builder_struct->instances-1].references->argc-1;
			
			action_code_builder_struct[action_code_builder_struct->instances-1].references->tag = env__add2(action_code_builder_struct[action_code_builder_struct->instances-1].references->tag, a->tag);

			action_code_builder_struct[action_code_builder_struct->instances-1].references->argv = env__add2(action_code_builder_struct[action_code_builder_struct->instances-1].references->argv, a->contents);
			
			int occurence = 0, tag_index = action_code_builder_struct[action_code_builder_struct->instances-1].references->argc-1;
			for (int i = 0; i < tag_index; i++) if (strcmp(action_code_builder_struct[action_code_builder_struct->instances-1].references->argv[i], action_code_builder_struct[action_code_builder_struct->instances-1].references->argv[tag_index]) == 0) occurence++;
			action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_builder_struct[action_code_builder_struct->instances-1].references->ref->instances-1].occurence_number = occurence;
		}
	}

	if (is_sub_contents_part_of_action) {
		// how to build the structure of a reference...
		if (has_tag) {
			// try to get tag type
			str_reg(a->tag);
			if (str_is("name|>")) {
				if (action_code_binder_skip_next != 0) {
					action_code_binder_skip_next--;
				} else {
					if (action_code_binder == NULL) {
						action_code_binder = malloc(sizeof(struct binder)*1);
						action_code_binder->instances = 1;
						action_code_binder_skip_next = 0;
					}
					else {
						action_code_binder = realloc(action_code_binder, sizeof(struct binder)*(action_code_binder->instances+1));
						action_code_binder->instances++;
					}
					action_code_binder[action_code_binder->instances-1].target_content_name = a->children[1]->contents;
					action_code_binder[action_code_binder->instances-1].bind_index = 0;
					action_code_binder[action_code_binder->instances-1].bind_index_updated = false;
					action_code_binder[action_code_binder->instances-1].bind_type = BIND_NAME;
				}
			}
			else if (str_is("num|>")) {
				if (action_code_binder_skip_next != 0) {
					action_code_binder_skip_next--;
				} else {
					if (action_code_binder == NULL) {
						action_code_binder = malloc(sizeof(struct binder)*1);
						action_code_binder->instances = 1;
						action_code_binder_skip_next = 0;
					}
					else {
						action_code_binder = realloc(action_code_binder, sizeof(struct binder)*(action_code_binder->instances+1));
						action_code_binder->instances++;
					}
					action_code_binder[action_code_binder->instances-1].target_content_name = NULL;
					action_code_binder[action_code_binder->instances-1].bind_index = atoi(a->children[1]->contents);
					action_code_binder[action_code_binder->instances-1].bind_index_updated = false;
					action_code_binder[action_code_binder->instances-1].bind_type = BIND_NUM;
				}
			}
			else if (str_is("indexedname|>")) {
				if (action_code_binder_skip_next != 0) {
					action_code_binder_skip_next--;
				} else {
					if (action_code_binder == NULL) {
						action_code_binder = malloc(sizeof(struct binder)*1);
						action_code_binder->instances = 1;
						action_code_binder_skip_next = 0;
					}
					else {
						action_code_binder = realloc(action_code_binder, sizeof(struct binder)*(action_code_binder->instances+1));
						action_code_binder->instances++;
					}
					action_code_binder[action_code_binder->instances-1].target_content_name = a->children[1]->contents;
					action_code_binder[action_code_binder->instances-1].bind_index= atoi(a->children[2]->children[1]->contents);
					action_code_binder[action_code_binder->instances-1].bind_index_updated = false;
					action_code_binder[action_code_binder->instances-1].bind_type = BIND_FUSED;
					action_code_binder_skip_next = 1;
				}
			}
			else if (str_is("name_segmented|>")) {
				if (action_code_binder_skip_next != 0) {
					action_code_binder_skip_next--;
				} else {
					if (action_code_binder == NULL) {
						action_code_binder = malloc(sizeof(struct binder)*1);
						action_code_binder->instances = 1;
					}
					else {
						action_code_binder = realloc(action_code_binder, sizeof(struct binder)*(action_code_binder->instances+1));
						action_code_binder->instances++;
					}
					str_new(name__)
					piece2(a, false, false, false, NULL, &name__);
					action_code_binder[action_code_binder->instances-1].target_content_name = strdup(name__.string);
					str_free(name__);
					action_code_binder[action_code_binder->instances-1].bind_index = 0;
					action_code_binder[action_code_binder->instances-1].bind_index_updated = false;
					action_code_binder[action_code_binder->instances-1].bind_type = BIND_NAME;
				}
			}
			else if (str_is("num_segmented|>")) {
				if (action_code_binder_skip_next != 0) {
					action_code_binder_skip_next--;
				} else {
					if (action_code_binder == NULL) {
						action_code_binder = malloc(sizeof(struct binder)*1);
						action_code_binder->instances = 1;
					}
					else {
						action_code_binder = realloc(action_code_binder, sizeof(struct binder)*(action_code_binder->instances+1));
						action_code_binder->instances++;
					}
					str_new(index__)
					piece2(a, false, false, false, &index__, NULL);
					action_code_binder[action_code_binder->instances-1].target_content_name = NULL;
					action_code_binder[action_code_binder->instances-1].bind_index= atoi(strdup(index__.string));
					action_code_binder[action_code_binder->instances-1].bind_index_updated = false;
					str_free(index__);
					action_code_binder[action_code_binder->instances-1].bind_type = BIND_NUM;
				}
			}
			else if (str_is("indexedname_segmented|>")) {
				if (action_code_binder_skip_next != 0) {
					action_code_binder_skip_next--;
				} else {
					if (action_code_binder == NULL) {
						action_code_binder = malloc(sizeof(struct binder)*1);
						action_code_binder->instances = 1;
					}
					else {
						action_code_binder = realloc(action_code_binder, sizeof(struct binder)*(action_code_binder->instances+1));
						action_code_binder->instances++;
					}
					str_new(index__);
					str_new(name__);
					piece2(a, false, false, false, &index__, &name__);
					action_code_binder[action_code_binder->instances-1].target_content_name = strdup(name__.string);
					str_free(name__);
					action_code_binder[action_code_binder->instances-1].bind_index= atoi(strdup(index__.string));
					action_code_binder[action_code_binder->instances-1].bind_index_updated = false;
					str_free(index__);
					action_code_binder[action_code_binder->instances-1].bind_type = BIND_FUSED;
					action_code_binder_skip_next = 1;
				}
			}
			if (is_an_action && action_code_binder != NULL) {
				for (int ii = 0; ii < action_code_binder->instances; ii++) {
					if (action_code_binder[action_code_binder->instances-1].bind_index_updated) {
						continue;
					}
					if (action_code_binder[ii].bind_type == BIND_NAME || action_code_binder[ii].bind_type == BIND_FUSED) {
						int tag_index = action_code_builder_struct[action_code_builder_struct->instances-1].references->argc-1;
						for (int i = 0; i < action_code_builder_struct[action_code_builder_struct->instances-1].references->argc; i++) {
							if (strcmp(action_code_builder_struct[action_code_builder_struct->instances-1].references->argv[i], action_code_binder[ii].target_content_name) == 0) {
								if (action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[i].occurence_number == action_code_binder[ii].bind_index) {
									action_code_binder[ii].bind_index_old = action_code_binder[ii].bind_index;
									action_code_binder[ii].bind_index = i;
									action_code_binder[ii].bind_index_updated = true;
									break;
								}
							}
						}
						if (action_code_binder[ii].bind_index_updated == false) {
							printf("error: occurence number %d of target %s not found, aborting execution\n", action_code_binder[ii].bind_index, action_code_binder[ii].target_content_name);
							abort();
						}
					}
				}
				
				// implements multi 'action code' storage
				
				if (is_an_action) {
					// get action code contents
					if (action_code_builder_struct[action_code_builder_struct->instances-1].references->content == NULL) {
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content = malloc(sizeof(struct action_code_builder_content)*1);
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances = 1;
					}
					else {
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content = realloc(action_code_builder_struct[action_code_builder_struct->instances-1].references->content, sizeof(struct action_code_builder_content)*(action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances+1));
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances++;
					}
					str_mallocr(action_code_builder_struct[action_code_builder_struct->instances-1].references->content[action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances-1].content, 1);
					aquire_action_code(a, &action_code_builder_struct[action_code_builder_struct->instances-1].references->content[action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances-1].content);
					env_t index_check = env__new();
					for (int ii = 0; ii < action_code_binder->instances; ii++) {
						// itterate through the bind indexes
						str_int2string(idx, action_code_binder[ii].bind_index);
						if (env__get_name(index_check, idx) != NULL) continue;
						index_check = env__add2(index_check, idx);
						free(idx);
						// create an instance of content_id for each action code
						if (action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id == NULL) {
							action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id = malloc(sizeof(int)*1);
							action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_amount = 1;
						}
						else {
							action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id = realloc(action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id, sizeof(int)*(action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_amount+1));
							action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_amount++;
						}
						action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id[action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_amount-1] =
						
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances-1
						
						;
					}
					env__free(index_check);
					index_check = NULL;
				}
			}
		}
	}
	for (i = 0; i < a->children_num; i++) {
		Build_Action_Code_Structure3(a->children[i], d+1, is_sub_contents_part_of_action, false);
	}
}


/*
void find_production_rules(mpc_ast_t *orig, mpc_ast_t *a) {

	if (strcmp(a->tag, "root|instruction|>") == 0) {
		str_new(Production_Rule);
		find_instruction_root_(a, &Production_Rule);
		printf("production rule (type: instruction): %s\n", Production_Rule.string);
// 		TYPEINFO(Production_Rule);
		str_free(Production_Rule);
		if (strcmp(a->tag, "root|instruction|>") == 0 || strcmp(a->tag, "instruction|>") == 0) {
			tag_index = 0;
			ra = a;
			int matches = 0;
			Build_Action_Code_Structure_Quick_Scan(a, &matches);
			if (matches != 0) {
				has_action_structures = true;
				Build_Action_Code_Structure3(a, 0, false, true);
				for(int i = 1; i < action_code_builder_struct[action_code_builder_struct->instances-1].references->argc; i++) {
					str_new(tmp);
					str_insert_string(tmp, action_code_builder_struct[action_code_builder_struct->instances-1].references->argv[i]);
// 					TYPEINFO(tmp);
					char * type = NULL;
					if (tmp.type & STR_TYPE_BINARY){
						type = "binary   ";
					}
					else {
						puts("FINDING");
						find_definition_root(orig, a->contents);
						puts("FOUND");
						type = "reference";
					}
					printf("    production rule (type: %s): %s\n",  type, tmp.string);
					type = NULL;
					str_free(tmp);
				}
			} else puts("no code structures detected");
		}
	}
	else if (strcmp(a->tag, "root|definition|>") == 0) {
		str_new(Production_Rule);
		find_definition_root_(a, &Production_Rule);
		printf("production rule (type: definition ): %s\n", Production_Rule.string);
// 		TYPEINFO(Production_Rule);
		find_definition_root3(a, Production_Rule.string);
		str_free(Production_Rule);
	}

	for (int i = 0; i < a->children_num; i++) {
		find_production_rules(a, a->children[i]);
	}
}
*/

void find_rules(mpc_ast_t *a, mpc_ast_t *orig, int d, bool is_sub_contents_part_of_instruction, bool is_sub_contents_part_of_sub_rule, bool is_sub_contents_part_of_action) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return;
	}
	
	bool is_end = false;
	if (strcmp(a->tag, "instruction_end|>") == 0 || strcmp(a->tag, "instruction_end|EOL|string") == 0) is_end = true;
	
	bool is_an_action = false;
	bool is_an_action_structure = false;
	bool is_an_action_segmented_structure = false;

	if (strcmp(a->tag, "action|>") == 0) is_an_action = true;
	else if (strcmp(a->tag, "action_code|>") == 0) is_an_action_structure = true;
	else if (strcmp(a->tag, "action_code_segmented|>") == 0) is_an_action_segmented_structure = true;
	
	if (is_an_action_segmented_structure == true || is_an_action_structure == true) is_sub_contents_part_of_action = true;
	
	if (strcmp(a->tag, "root|instruction|>") == 0 || strcmp(a->tag, "instruction|>") == 0) {
		tag_index = 0;
		is_sub_contents_part_of_instruction = true;
		ra = a;
		int matches = 0;
		has_action_structures = false;
		Build_Action_Code_Structure_Quick_Scan(a, &matches);
		if (matches != 0) {
			has_action_structures = true;
			Build_Action_Code_Structure3(a, 0, false, true);
		}
	} else if (strcmp(a->tag, "root|definition|>") == 0 || strcmp(a->tag, "root|>") == 0) {
		is_sub_contents_part_of_instruction = false;
	}
	
	if (
		is_an_action == false &&
		is_an_action_structure == false &&
		is_an_action_segmented_structure == false
	) for (i = 0; i < a->children_num; i++) {
		find_rules(a->children[i], orig, d+1, is_sub_contents_part_of_instruction, is_sub_contents_part_of_sub_rule, is_sub_contents_part_of_action);
	}
}

/*
				if (is_an_action) {
					// get action code contents
					if (action_code_builder_struct[action_code_builder_struct->instances-1].references->content == NULL) {
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content = malloc(sizeof(struct action_code_builder_content)*1);
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances = 1;
					}
					else {
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content = realloc(action_code_builder_struct[action_code_builder_struct->instances-1].references->content, sizeof(struct action_code_builder_content)*(action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances+1));
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances++;
					}
					str_mallocr(action_code_builder_struct[action_code_builder_struct->instances-1].references->content[action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances-1].content, 1);
					aquire_action_code(a, &action_code_builder_struct[action_code_builder_struct->instances-1].references->content[action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances-1].content);
					env_t index_check = env__new();
					for (int ii = 0; ii < action_code_binder->instances; ii++) {
						// itterate through the bind indexes
						str_int2string(idx, action_code_binder[ii].bind_index);
						if (env__get_name(index_check, idx) != NULL) continue;
						index_check = env__add2(index_check, idx);
						// create an instance of content_id for each action code
						if (action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id == NULL) {
							action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id = malloc(sizeof(int)*1);
							action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_amount = 1;
						}
						else {
							action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id = realloc(action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id, sizeof(int)*(action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_amount+1));
							action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_amount++;
						}
						action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_id[action_code_builder_struct[action_code_builder_struct->instances-1].references->ref[action_code_binder[ii].bind_index].content_amount-1] =
						
						action_code_builder_struct[action_code_builder_struct->instances-1].references->content->instances-1
						
						;
					}
					env__free(index_check);
					index_check = NULL;
				}
*/








bool update_on_next_pass = false;

void find_instructions3(mpc_ast_t *a, mpc_ast_t *orig, int d, bool is_sub_contents_part_of_instruction, bool is_sub_contents_part_of_sub_rule, bool is_sub_contents_part_of_action, bool first_call) {
	int i;
	if (first_call) {
		rule_index++;
		instruction_index = -1;
		find_rules(a, orig, d, is_sub_contents_part_of_instruction, is_sub_contents_part_of_sub_rule, is_sub_contents_part_of_action);
		
		for (int ii = 0; ii < 0; ii++) {
// 		for (int ii = 0; ii < action_code_builder_struct->instances; ii++) {
			printf("action_code_builder_struct[%d].references->argc = %d\n", ii, action_code_builder_struct[ii].references->argc);
			for(int i = 0; i < action_code_builder_struct[ii].references->argc; i++) {
				printf("action_code_builder_struct[%d].references->argv[%d] = %s\n", ii, i,  action_code_builder_struct[ii].references->argv[i]);
				printf("action_code_builder_struct[%d].references->tag[%d] = %s\n", ii, i,  action_code_builder_struct[ii].references->tag[i]);
				if (strcmp(action_code_builder_struct[ii].references->tag[i], "ISA_RULE|word|regex") == 0 || (strcmp(action_code_builder_struct[ii].references->tag[i], "word|regex") == 0 && is_sub_contents_part_of_sub_rule == true)) {
// 					puts("FINDING");
// 					find_definition_root(orig, action_code_builder_struct[ii].references->argv[i]);
// 					puts("FOUND");
				}
				int ac_amount = action_code_builder_struct[ii].references->ref[i].content_amount;
				pi(ac_amount)
				if (ac_amount) {
					for (int iii = 0; iii < ac_amount; iii++) {
						char * ac_contents = action_code_builder_struct[ii].references->content[action_code_builder_struct[ii].references->ref[i].content_id[iii]].content.string;
						
						ps(ac_contents);
					}
				}
// 				str_new(tmp);
// 				str_insert_string(tmp, action_code_builder_struct[ii].references->argv[i]);
// // 					TYPEINFO(tmp);
// 				char * type = NULL;
// 				if (tmp.type & STR_TYPE_BINARY){
// 					type = "binary   ";
// 				}
// 				else {
// 					type = "reference";
// 				}
// 				printf("    production rule (type: %s): %s\n",  type, tmp.string);
// 				type = NULL;
// 				str_free(tmp);
			}
		}
// 		abort();
	}
	if (a == NULL) {
		printf("NULL\n");
		return;
	}
	
	bool is_end = false;
	if (strcmp(a->tag, "instruction_end|>") == 0 || strcmp(a->tag, "instruction_end|EOL|string") == 0) is_end = true;
	
	bool is_an_action = false;
	bool is_an_action_structure = false;
	bool is_an_action_segmented_structure = false;

	if (strcmp(a->tag, "action|>") == 0) is_an_action = true;
	else if (strcmp(a->tag, "action_code|>") == 0) is_an_action_structure = true;
	else if (strcmp(a->tag, "action_code_segmented|>") == 0) is_an_action_segmented_structure = true;
	
	if (is_an_action_segmented_structure == true || is_an_action_structure == true) is_sub_contents_part_of_action = true;
	
	if (strcmp(a->tag, "root|instruction|>") == 0 || strcmp(a->tag, "instruction|>") == 0) {
// 		for (i = 0; i < d; i++) printf("    ");
		chain_index++;
// 		printf("sub_root > chain %d\n", chain_index);
		passed_first = true;
		instruction_index++;
		str_new(Production_Rule);
		find_instruction_root_(a, &Production_Rule);
		for (i = 0; i < d; i++) printf("    ");
		printf("production rule (type: instruction): %s\n", Production_Rule.string);
		str_free(Production_Rule);
		tag_index = 0;
		is_sub_contents_part_of_instruction = true;
		ra = a;
		int matches = 0;
		has_action_structures = false;
		Build_Action_Code_Structure_Quick_Scan(a, &matches);
		if (matches != 0) has_action_structures = true;
	} else if (strcmp(a->tag, "root|definition|>") == 0 || strcmp(a->tag, "root|>") == 0) {
		is_sub_contents_part_of_instruction = false;
	}
	
	
	if (is_sub_contents_part_of_instruction == true) {
		if (is_end == true) {
// 			printf("tag: '%s', contents: '%s'\n", a->tag, a->contents);
// 			pi(number_ifs)
			while(number_ifs != 0) {
				number_ifs--;
// 				str_new(tmp);
// 				pi(number_ifs)
// 				if (number_ifs != 0) { str_insert_string(tmp, "} else read__string_undo(binstr.data);\n"); }
// 				else str_insert_string(tmp, "}");
// 				str_output_append(tmp.indented, outfile);
// 				str_free(tmp);
			}
		}
		else {
			if (strlen(a->contents)) {
				str_new(tmp);
				str_insert_string(tmp, a->contents);
				if (!(tmp.type & STR_TYPE_BLANK) && strcmp(a->contents, "|") && strcmp(a->contents, ":")) {
					char * type = "";
					if (tmp.type & STR_TYPE_BINARY){
						type = "binary   ";
					}
					else if (tmp.type & STR_TYPE_DIGIT){
						type = "digit    ";
					}
					else {
						type = "reference";
					}
					if (!passed_first) {
						if (passed_next) {
					if (update_on_next_pass && (tmp.type & STR_TYPE_DIGIT || tmp.type & STR_TYPE_BINARY)) {
						pb(update_on_next_pass)
						pi(rule_index)
						pi(final_rule)
						pi(rule_index_tmp-1)
						pi(passed_first)
						pi(passed_next)
						rule_index = rule_index_tmp-1;
						update_on_next_pass = false;
					}
// 							pi(rule_index)
// 							rule_index++;
// 							pi(rule_index)
							actual_index = 0;
							passed_next = false;
							tree_add(&list, a->contents);
							trie_queue_add(&trie_trie, trie_sub_root, -1, strdup(tmp.string), actual_index, trie_rule_prefix, rule_index);
						}
						else {
							if ((tmp.type & STR_TYPE_BINARY) || (tmp.type & STR_TYPE_DIGIT)) {
								pi(rule_index)
								rule_index++;
								pi(rule_index)
								actual_index++;
								tree_add(&list, a->contents);
								trie_queue_add(&trie_trie, trie_rule_prefix, rule_index-1, strdup(tmp.string), actual_index, trie_rule_prefix, rule_index);
							}
						}
					} else {
						passed_first = false;
						passed_next = true;
					}
					for (i = 0; i < d; i++) printf("    ");
					printf("    production segment (type: %s): %s \n",  type, tmp.string);
					printf("tag: '%s', contents: '%s'\n", a->tag, a->contents);
					if (strcmp(a->tag, "binary|regex") == 0) {
						if (number_ifs == 0) instructions++;
						number_ifs++;
// 						ps(a->contents)
						if (number_ifs == 1) find_instruction_root(ra);
					}
					if (!strcmp(type, "reference") && !strcmp(a->tag, "ISA_RULE|word|regex")) {
						puts("FINDING");
						tree_add_branch(&list);
						find_definition_root(orig, a->contents);
						tree_end_branch(&list);
						puts("FOUND");
						if (final_rule) {
							pi(rule_index)
							rule_index = final_rule;
							update_on_next_pass = true;
							pi(rule_index)
							final_rule = 0;
						}
					}
					if (strcmp(a->tag, "ISA_DEFINE|word|regex") == 0 || strcmp(a->tag, "ISA_RULE|word|regex") == 0 || strcmp(a->tag, "word|regex") == 0 || strcmp(a->tag, "binary|regex") == 0) {
						if (is_sub_contents_part_of_action == false && has_action_structures) {
							int ac_amount = action_code_builder_struct[instruction_index].references->ref[tag_index].content_amount;
							if (ac_amount) {
								for (int i = 0; i < ac_amount; i++) {
									char * ac_contents = action_code_builder_struct[instruction_index].references->content[action_code_builder_struct[instruction_index].references->ref[tag_index].content_id[i]].content.string;
// 									STR_INDENT_SIZE = 4;
// 									STR_INDENT_LEVEL = STR_INDENT_SIZE*(d+2);
									str_new(action);
									str_insert_string(action, "{")
									str_insert_string(action, ac_contents)
									str_insert_string(action, "}")
// 									for (int in = 0; in < d+1; in++) printf("    ");
// 									printf("%s%d:%s = action code = {\n%s\n", trie_trie->rear->rule_name, trie_trie->rear->rule_name_index, trie_trie->rear->key, action.indented.string);
									trie_trie->rear->action_code = env__add(trie_trie->rear->action_code, action.indented.string);
									str_free(action);
									STR_INDENT_LEVEL = 0;
// 									str_output_append(action_code_builder_struct[instruction_index].references->content[action_code_builder_struct[instruction_index].references->ref[tag_index].content_id[i]].content.indented, outfile);
								}
								if (strcmp(a->contents, action_code_builder_struct[instruction_index].references->argv[tag_index]) != 0) {
									puts("INCONSISTANCY DETECTED");
									abort();
								};
							}
							tag_index++;
						}
					}
				}
				str_free(tmp);
			} else {
// 				printf("tag: '%s'\n", a->tag);
				if (strcmp(a->tag, "ISA_RULE|>") == 0) is_sub_contents_part_of_sub_rule = true;
				if(is_an_action) {
					str_new(tmp);
					aquire_action_code(a, &tmp);
// 					STR_INDENT_SIZE = 4;
// 					STR_INDENT_LEVEL = STR_INDENT_SIZE*(d+2);
					str_new(action);
					str_insert_string(action, "{")
					str_insert_string(action, tmp.string)
					str_insert_string(action, "}")
// 					for (int in = 0; in < d+1; in++) printf("    ");
// 					printf("%s%d:%s = action code = {\n%s\n", trie_trie->rear->rule_name, trie_trie->rear->rule_name_index, trie_trie->rear->key, action.indented.string);
					trie_trie->rear->action_code = env__add(trie_trie->rear->action_code, action.indented.string);
					str_free(action);
					STR_INDENT_LEVEL = 0;
// 					str_output_append(tmp.indented, outfile);
					str_free(tmp);
				}
			}
		}
	}
	if (
		is_an_action == false &&
		is_an_action_structure == false &&
		is_an_action_segmented_structure == false
	) for (i = 0; i < a->children_num; i++) {
		find_instructions3(a->children[i], orig, d+1, is_sub_contents_part_of_instruction, is_sub_contents_part_of_sub_rule, is_sub_contents_part_of_action, false);
	}
	if (d == 0) {
// 		print(list->front);
		fix(list->front, 0, 0, 0, 0, 0);
		print(list->front);
// 		abort();
		printf("/*\n\
		INPUT:\n\
def0:\n\
35:NULL\n\
85:NULL\n\
def1:\n\
91:NULL\n\
92:NULL\n\
\n\
rule1 : |1|2|def0|def1|6|\n\
\n\
		TARGET OUTPUT:\n\
    insert(sub_root, \"1\", 0, rule_0);\n\
        insert(rule_0, \"2\", 1, rule_1);\n\
			insert(rule_1, \"3\", 2, rule_2);\n\
			insert(rule_2, \"5\", 3, rule_3);\n\
        insert(rule_0, \"2\", 1, rule_4); // if rule_1 fails backtrack to rule_0 and select rule_4\n\
			insert(rule_4, \"8\", 2, rule_5);\n\
			insert(rule_5, \"5\", 3, rule_3);\n\
        insert(rule_3, \"9\", 4, rule_4);\n\
			insert(rule_4, \"1\", 5, rule_5);\n\
        insert(rule_3, \"9\", 4, rule_7); // if rule_4 fails backtrack to rule_3 and select rule_7\n\
			insert(rule_7, \"2\", 5, rule_5);\n\
        insert(rule_5, \"6\", 6, NULL);\n\
*/\n");
		
		struct trie_QNode * node = trie_newNode(NULL,0,NULL,0,NULL,0); // this gets freed anyway
		int nodes = 0;
		int start_indent = 0;
		for (i = 0; i < start_indent; i++) printf("    ");
		printf("struct TrieNode * %s = getNode(); \n", trie_root);
		for (i = 0; i < start_indent; i++) printf("    ");
		printf("struct TrieNode * %s = getNode(); \n", trie_sub_root);
		env_t index_check = env__new();
		while (node != NULL) {
			// drain the list until empty
			if (node->key) free(node->key);
			free(node);
			node = trie_load_asm(&trie_trie);
			if (node == NULL) break;
// 			pp(node)
// 			pp(node->rule_name)
// 			ps(node->rule_name)
// 			pi(node->rule_name_index)
// 			pp(node->key)
// 			ps(node->key)
// 			pi(node->index)
// 			pp(node->rule_next)
// 			ps(node->rule_next)
// 			pi(node->rule_next_index)
			if (node->action_code) {
				printf("action_code_function_%d (void) {\n", nodes);
				puts("// listing action_code");
				env__list(node->action_code);
				puts("// listed action_code");
				printf("}\n");
			}
			if (node->rule_next && node->rule_next_index != -1) {
				bool rule_does_not_exist = true;
				bool next_rule_does_not_exist = true;
				str_int2string(idxA, node->rule_name_index);
				str_int2string(idxB, node->rule_next_index);
				if (env__get_name(index_check, idxA) != NULL) rule_does_not_exist = false;
				index_check = env__add2(index_check, idxA);
				free(idxA);
				if (env__get_name(index_check, idxB) != NULL) next_rule_does_not_exist = false;
				index_check = env__add2(index_check, idxB);
				free(idxB);
				if (node->rule_name_index == -1) {
					// is sub_root
					if (rule_does_not_exist) {
						for (i = 0; i < start_indent+1; i++) printf("    ");
						printf("struct TrieNode * %s%d = getNode(); \n", node->rule_name, node->rule_name_index);
					}
					if (next_rule_does_not_exist) {
						for (i = 0; i < start_indent+1; i++) printf("    ");
						printf("struct TrieNode * %s%d = getNode(); \n", node->rule_next, node->rule_next_index);
					}
					for (i = 0; i < start_indent+1; i++) printf("    ");
					printf("insert(%s, \"%s\", %d, %s%d);\n", node->rule_name, node->key, node->index, node->rule_next, node->rule_next_index);
				}
				else {
					// is rule
					if (rule_does_not_exist) {
						for (i = 0; i < start_indent+2; i++) printf("    ");
						printf("struct TrieNode * %s%d = getNode(); \n", node->rule_name, node->rule_name_index);
					}
					if (next_rule_does_not_exist) {
						for (i = 0; i < start_indent+2; i++) printf("    ");
						printf("struct TrieNode * %s%d = getNode(); \n", node->rule_next, node->rule_next_index);
					}
					for (i = 0; i < start_indent+2; i++) printf("    ");
					printf("insert(%s%d, \"%s\", %d, %s%d);\n", node->rule_name, node->rule_name_index, node->key, node->index, node->rule_next, node->rule_next_index);
				}
			} else {
				if (node->rule_name_index == -1) {
					// is sub_root
					for (i = 0; i < start_indent+1; i++) printf("    ");
					printf("insert(%s, \"%s\", %d, NULL);\n", node->rule_name, node->key, node->index);
				} else {
					// is rule
					for (i = 0; i < start_indent+2; i++) printf("    ");
					printf("insert(%s%d, \"%s\", %d, NULL);\n", node->rule_name, node->rule_name_index, node->key, node->index);
				}
			}
			nodes++;
		}
		env__free(index_check);
		if (trie_trie) 	free(trie_trie);
	}
}








#endif
#ifdef evalfile_
void evalfile(char * string, mpc_parser_t *p, mpc_result_t * r) {
	if (mpc_parse_contents(string, p, r)) { // parse file specified in string with parser p and store resulting AST into r->output
			AP(r->output, 0, stdout); // prints full AST
			mpc_ast_delete(r->output);
	} else {
		mpc_err_print(r->error);
		mpc_err_delete(r->error);
	}
}
#endif
#ifdef evalfileI_
void evalfileI(char * string, mpc_parser_t *p, mpc_result_t * r) {
	if (mpc_parse_contents(string, p, r)) { // parse file specified in string with parser p and store resulting AST into r->output
			instructions = 0;
			find_instructions3(r->output, r->output, 0, false, false, false, true); // attempts to print instruction part of ast with definition replacement done in instructions
			mpc_ast_delete(r->output);
	} else {
		mpc_err_print(r->error);
		mpc_err_delete(r->error);
	}
}
#endif
void eval(char * string, mpc_parser_t *p, mpc_result_t * r) {
	if (mpc_parse("test", string, p, r)) {
		mpc_ast_print(r->output);
			mpc_ast_delete(r->output);
		} else {
			mpc_err_print(r->error);
			mpc_err_delete(r->error);
		}
}

#define mpcnew(x)	 mpc_parser_t * x = mpc_new(#x)

int main(int argc, char **argv) {
	mpcnew(parser_root);
	mpcnew(root);
	mpcnew(definition);
	mpcnew(instruction);
	mpcnew(definition_start);
	mpcnew(definition_statement);
	mpcnew(definition_item);
	mpcnew(instruction_start);
	mpcnew(instruction_statement);
	mpcnew(instruction_item);
	mpcnew(instruction_end);
	mpcnew(EOL);
	mpcnew(hash);
	mpcnew(ISA_DEFINE);
	mpcnew(colon);
	mpcnew(ha_statement);
	mpcnew(item);
	mpcnew(bracketed_item);
	mpcnew(normal_item);
	mpcnew(pair);
	mpcnew(single);
	mpcnew(S);
	mpcnew(space);
	mpcnew(tab);
	mpcnew(word);
	mpcnew(binary);
	mpcnew(ISA_RULE);
	mpcnew(comma);
	mpcnew(comment);
	mpcnew(single_line_comment);
	mpcnew(multi_line_comment);
	
	mpcnew(action_code);
	mpcnew(name);
	mpcnew(num);
	mpcnew(indexedname);
	mpcnew(number);
	
	
	mpcnew(word_segmented);
	mpcnew(number_segmented);
	mpcnew(action_code_segmented);
	mpcnew(name_segmented);
	mpcnew(num_segmented);
	mpcnew(indexedname_segmented);
	mpcnew(SS);
	mpcnew(action);
	mpc_err_t* err = mpca_lang_contents(MPCA_LANG_WHITESPACE_SENSITIVE, "grammer_parser.parse", parser_root, root, definition, instruction, definition_start, definition_statement, definition_item, instruction_start, instruction_statement, instruction_item, instruction_end, item, bracketed_item, normal_item, single, pair, hash, colon, word, binary, ISA_RULE, ISA_DEFINE, EOL, S, space, tab, comma, comment, single_line_comment, multi_line_comment, action_code, name, num, indexedname, number, action_code_segmented, name_segmented, num_segmented, indexedname_segmented, number_segmented, word_segmented, SS, action);
	if (err != NULL) {
		mpc_err_print(err);
		mpc_err_delete(err);
		exit(1);
	}
	mpc_result_t r;
#ifdef evalfile_
// 	evalfile("Intel4004TEST.isa", parser_root, &r);
	evalfile("Intel4004.isa", parser_root, &r);
#endif
#ifdef evalfileI_
	STR_INDENT_LEVEL = 0;
	STR_INDENT_SIZE = 8;
	str_new(ff);
	str_include(ff, "ISA_test_PRE.c");
	str_output(ff, outfile);
	evalfileI("Intel4004.isa", parser_root, &r);
	str_reset(ff);
	str_insert_string(ff, "}");
	str_output_append(ff.indented, outfile);
	str_free(ff);
#endif
// mpc_parser_t * re = mpc_new("re");
// mpc_define(re, mpc_re("(ab(cd))+"));
// mpc_result_t rr;
// if (mpc_parse("test", "abcd", re, &rr)) {
// 	puts("passed");
// 	ps(rr.output)
// } else {
// 	puts("failed");
// 	mpc_err_print(rr.error);
// 	mpc_err_delete(rr.error);
// }
	return 0;
}

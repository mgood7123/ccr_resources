#include "mpc/mpc.h"
#include "mpc/mpc.c"
#include <stdbool.h>
#include "../Shell/builtins/printfmacro.h"
#include "../Shell/builtins/libstring.h"
#include "../Shell/builtins/regex_str.h"
#ifndef BOM_MARK
char * filetostring(char * file) {
	char * buf;
	size_t size;
	read__(file, &buf, &size);
	buf[size] = NULL;
	return strdup(buf);
}
#endif

char * outfile = "tmpfile.c";

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

bool find_tree_string(mpc_ast_t *a, char * string) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->contents, string) == 0) {
		str_new(tmp);
		str_insert_string(tmp, " puts(\"");
		str_insert_string(tmp, a->contents);
		str_insert_string(tmp, " \");\n");
		str_output_append(tmp, outfile);
		str_free(tmp);
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
	if (strcmp(a->tag, "ISA_DEFINE|word|regex") == 0) {
		str_new(tmp);
		str_insert_string(tmp, " puts(\"");
		str_insert_string(tmp, a->contents);
		str_insert_string(tmp, " \");\n");
		str_output_append(tmp, outfile);
		str_free(tmp);
		return true;
	}
	return false;
}
int find_definition_strings_called = 0;
bool find_definition_strings(mpc_ast_t *a, bool omit_else) {
	if (a == NULL) {
		printf("NULL\n");
		return false;
	}
	if (strcmp(a->tag, "word|regex") == 0) {
		find_definition_strings_called++;
		printf("%s\n", a->contents);
		str_new(tmp);
		if (omit_else == false) if (find_definition_strings_called >1) str_insert_string(tmp, "else ");
		str_insert_string(tmp, "if (!bytecmpcRE(read__string(binstr.data, strlen(\"");
		str_insert_string(tmp, a->contents);
		str_insert_string(tmp, "\"), ");
		if (find_definition_strings_called >1) { str_insert_string(tmp, "true"); }
		else { str_insert_string(tmp, "false"); }
		str_insert_string(tmp, "), \"");
		str_insert_string(tmp, a->contents);
		str_insert_string(tmp, "\")) {\n");
		str_insert_string(tmp, " printf(\"a->contents = ");
		str_insert_string(tmp, a->contents);
		str_insert_string(tmp, ", bits = %s\\n\", bits);\n");
		str_insert_string(tmp, "}\n");
		str_output_append(tmp, outfile);
		str_free(tmp);
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
			if (find_definition_root(a->children[i], string) == true) find_definition_statements(a);
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

int number_ifs = 0;
mpc_ast_t *ra = NULL;
void find_instructions(mpc_ast_t *a, mpc_ast_t *orig, int d, bool is_sub_contents_part_of_instruction) {
	int i;
	if (a == NULL) {
		printf("NULL\n");
		return;
	}
	if (strcmp(a->tag, "root|instruction|>") == 0) {
		is_sub_contents_part_of_instruction = true;
		ra = a;
	} else if (strcmp(a->tag, "root|definition|>") == 0 || strcmp(a->tag, "root|>") == 0) {
		is_sub_contents_part_of_instruction = false;
	}
	if (is_sub_contents_part_of_instruction == true) {
		for (i = 0; i < d; i++) printf("    ");
		if (strcmp(a->tag, "instruction_end|>") == 0 || strcmp(a->tag, "instruction_end|EOL|string") == 0) {
			while(number_ifs != 0) {
				number_ifs--;
				str_new(tmp);
				str_insert_string(tmp, "} else read__string_undo(binstr.data);\n");
				str_output_append(tmp, outfile);
				str_free(tmp);
			}
		}
		if (strlen(a->contents)) {
			printf("tag: '%s', contents: '%s'\n", a->tag, a->contents);
			if (strcmp(a->tag, "binary|regex") == 0) {
				number_ifs++;
				str_new(tmp);
				str_insert_string(tmp, "if (!bytecmpcRE(read__string(binstr.data, strlen(\"");
				str_insert_string(tmp, a->contents);
				str_insert_string(tmp, "\"), false), \"");
				str_insert_string(tmp, a->contents);
				str_insert_string(tmp, "\")) {\n");
				str_insert_string(tmp, " printf(\"a->contents = ");
				str_insert_string(tmp, a->contents);
				str_insert_string(tmp, ", bits = %s\\n\", bits);\n");
				str_output_append(tmp, outfile);
				str_free(tmp);
				if (number_ifs == 1) find_instruction_root(ra);
			}
			if (strcmp(a->tag, "ISA_RULE|word|regex") == 0) {
				puts("FINDING");
				find_definition_root(orig, a->contents);
				puts("FOUND");
			}
		} else {
			printf("tag: '%s'\n", a->tag);
		}
	}
	for (i = 0; i < a->children_num; i++) {
		find_instructions(a->children[i], orig, d+1, is_sub_contents_part_of_instruction);
	}
}

void evalfile(char * string, mpc_parser_t *p, mpc_result_t * r) {
	if (mpc_parse_contents(string, p, r)) { // parse file specified in string with parser p and store resulting AST into r->output
// 			AP(r->output, 0, stdout); // prints full AST
			find_instructions(r->output, r->output, 0, false); // attempts to print instruction part of ast with definition replacement done in instructions
			mpc_ast_delete(r->output);
	} else {
		mpc_err_print(r->error);
		mpc_err_delete(r->error);
	}
}

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
	mpcnew(definition_end);
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
	mpc_err_t* err = mpca_lang_contents(MPCA_LANG_WHITESPACE_SENSITIVE, "grammer_parser.parse", parser_root, root, definition, instruction, definition_start, definition_statement, definition_item, definition_end, instruction_start, instruction_statement, instruction_item, instruction_end, item, bracketed_item, normal_item, single, pair, hash, colon, word, binary, ISA_RULE, ISA_DEFINE, EOL, S, space, tab, comma, comment);
	if (err != NULL) {
		mpc_err_print(err);
		mpc_err_delete(err);
		exit(1);
	}
	mpc_result_t r;
	
	char * str = filetostring("ISA_test_PRE.c");
	str_new(ff);
	str_insert_string(ff, str);
	str_output(ff, outfile);
	evalfile("Intel4004TEST.isa", parser_root, &r);
	str_reset(ff);
	str_insert_string(ff, "}\n");
	str_output_append(ff, outfile);
	str_free(ff);
	return 0;
}

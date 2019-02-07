#include "../builtins/regex_str.h"
#include "../builtins/printfmacro.h"

int main(void) {
	str_new(test);
	ps(test.string); TYPEINFO(test);
	str_insert_string(test, "Hello World");
	ps(test.string); TYPEINFO(test);
	str_insert_string(test, "!");
	ps(test.string); TYPEINFO(test);
	str_insert_int(test, 123456);
	ps(test.string); TYPEINFO(test);
	str_insert_string(test, "'qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM foo5'あ");
	ps(test.string); TYPEINFO(test);
	str_free(test);
	
	
	
	// test type identification
	
	str_new(bin);
	ps(bin.string); TYPEINFO(bin);
	str_insert_int(bin, 0);
	ps(bin.string); TYPEINFO(bin);
	str_insert_int(bin, 2);
	ps(bin.string); TYPEINFO(bin);
	puts("undoing");
	str_undo(bin);
	ps(bin.string); TYPEINFO(bin);
	puts("undoing");
	str_undo(bin);
	ps(bin.string); TYPEINFO(bin);
	str_free(bin);
	
	str_new(bin2);
	ps(bin2.string); TYPEINFO(bin2);
	puts("adding 1");
	str_insert_string(bin2, "1");
	ps(bin2.string); TYPEINFO(bin2);
	puts("adding 0");
	str_insert_string(bin2, "0");
	ps(bin2.string); TYPEINFO(bin2);
	puts("adding x");
	str_insert_string(bin2, "x");
	ps(bin2.string); TYPEINFO(bin2);
	puts("adding 1");
	str_insert_string(bin2, "1");
	ps(bin2.string); TYPEINFO(bin2);
	str_free(bin2);
	return -1;
}

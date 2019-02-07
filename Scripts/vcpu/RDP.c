#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* Our BNF:
 *        Formula ::= Constant | Proposition | UnaryFormula | BinaryFormula
 *       Constant ::= "T" | "F"
 *    Proposition ::= [a-z0-9]+
 *   UnaryFormula ::= LeftParen UnaryOperator Formula RightParen
 *  BinaryFormula ::= LeftParen BinaryOperator Formula Forumla RightParen
 *      LeftParen ::= "("
 *     RightParen ::= ")"
 *  UnaryOperator ::= "\neg"
 * BinaryOperator ::= "\vee" | "\wedge" | "\rightarrow" | "\leftrightarrow"
 */

/* prototype for each of the parsing frunctions we'll use */
bool Formula(char *s[]);
bool Constant(char *s[]);
bool Proposition(char *s[]);
bool UnaryFormula(char *s[]);
bool BinaryFormula(char *s[]);
bool LeftParen(char *s[]);
bool RightParen(char *s[]);
bool UnaryOperator(char *s[]);
bool BinaryOperator(char *s[]);

/* two helper functions well need */
void SkipWhitespace(char *s[]);
bool FormulaWrapper(char *s[]); /* more on this one later */

/* some string constants (so i don't have to type these out ever again, and
 * they can be easily changed) */
const char *STRING_NOT     = "!";
const char *STRING_OR      = "|";
const char *STRING_AND     = "&";
const char *STRING_IMPLIES = ">";
const char *STRING_IFF     = "<>";

/* a constant denoting the maximum length (in character) of any proposition
 * string */
const int  MAX_PROP_LENGTH = 100;


/*****************************************************************************
 * Our Two Helper Functions, "SkipWhiteSpace" and "match".
 * These are used extensively in our parsing functions.
 ****************************************************************************/

/* advances the input string past any white space */
void SkipWhitespace(char *s[]) {
   while (*s[0] == ' ' || *s[0] == '\n' || *s[0] == '\t')
      *s = *s + 1;
}

/* Compares the input string against a fixed, known string (token).
 * If it maches, the input string is advanced past the token and
 * true is returned.  Otherwise, the input string remains unchnaged
 * and false is returned.
 */
bool match(char *s[], const char *token) {
   if (strncmp(*s, token, strlen(token)) == 0) {
      *s = *s + strlen(token);
      return true;
   }

   return false;
}


/*****************************************************************************
 * Our Parsing Functions.
 * One for each rule of our BNF.  Note that each mirrors our BNF VERY CLOSELY.
 *
 * ALL of them behave in the following, identical fashion:
 *
 *    function "bool Expressions(char *s[])" will
 *       1. Compare the input string "s" against the expression it represents
 *          (from the BNF).
 *
 *       2. IF the input string matches the expressions, THEN
 *             the input string will be advanced past the expression, and
 *             true will be returned.
 *
 *       3. OTHERWISE, if the input string does not match the expression, THEN
 *             the input string will remain unchanged and false will be
 *             returned.
 *
 ****************************************************************************/

/* Identify a formula (see BNF).  Note this is a CHOICE rule. */
bool Formula(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   if (Constant(s) || Proposition(s) || UnaryFormula(s) || BinaryFormula(s))
      return true;

   *s = original;
   return false;
}

/* Identify a unary formula (see BNF).  Note this is a SEQUENCE rule. */
bool UnaryFormula(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   if (!LeftParen(s)) {
      *s = original;
      return false;
   }

   if (!UnaryOperator(s)) {
      *s = original;
      return false;
   }

   if (!Formula(s)) {
      *s = original;
      return false;
   }

   if (!RightParen(s)) {
      *s = original;
      return false;
   }

   return true;
}

/* Identify a binary formula (see BNF).  Note this is a SEQUENCE rule. */
bool BinaryFormula(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   if (!LeftParen(s)) {
      *s = original;
      return false;
   }

   if (!BinaryOperator(s)) {
      *s = original;
      return false;
   }

   if (!Formula(s)) {
      *s = original;
      return false;
   }

   if (!Formula(s)) {
      *s = original;
      return false;
   }

   if (!RightParen(s)) {
      *s = original;
      return false;
   }

   return true;
}

/* Identify a left-parenthesis. Note this is a LITERAL TEXT rule. */
bool LeftParen(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   if (match(s, "("))
      return true;

   *s = original;
   return false;
}

/* Identify a right-parenthesis. Note this is a LITERAL TEXT rule. */
bool RightParen(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   if (match(s, ")"))
      return true;

   *s = original;
   return false;
}

/* Identify a constant (either 'T' or 'F', that's it).
 * Note this is a CHOICE rule between two LITERAL TEXT items.*/
bool Constant(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   if (match(s, "T") || match(s, "F"))
      return true;

   *s = original;
   return false;
}

/* Identify a proposition (any string with a-z).
 * Note this is a simple PATTERN rule (simple enough we can use sscanf for) */
bool Proposition(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   /* these are used in sscanf to record was was read from string */
   char prop[MAX_PROP_LENGTH];
   int  numCharsRead;

   if (sscanf(*s, "%[a-z0-9]%n", &prop, &numCharsRead) == 1) {
      *s = *s + numCharsRead;
      return true;
   }

   *s = original;
   return false;
}

/* Identify a unary operator (currently only '\neg').
 * Note this is a LITERAL TEXT rule. */
bool UnaryOperator(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   if (match(s, STRING_NOT))
      return true;

   *s = original;
   return false;
}

/* Identify a binary operator (either '\vee', '\wedge', etc.)
 * Note this is a CHOICE rule among LITERAL TEXT options. */
bool BinaryOperator(char *s[]) {

   char *original = *s;
   SkipWhitespace(s);

   if (match(s, STRING_OR) || match(s, STRING_AND)
   ||  match(s, STRING_IMPLIES) || match(s, STRING_IFF))
      return true;

   *s = original;
   return false;
}


/*****************************************************************************
 * Our "Wrapper" function for parsing.
 * This is what we call to check if a string contains a wff.
 ****************************************************************************/
bool FormulaWrapper(char *s[]) {

   char *original = *s;

   if (Formula(s)) {
      SkipWhitespace(s);
      if (*s[0] == '\0')
         return true;
      else {
		 return FormulaWrapper(s);
//          *s = original;
//          return false;
// 		 return true;
      }
   }
   return false;
}


/*****************************************************************************
 * A simple test program.  It prompts the user to enter formulae of
 * propositional logic, one per line (max 1000 characters per line) and
 * then parses it, telling the user if what they entered was a wff or not.
 ****************************************************************************/
int main(int argc, char *argv[])
{
   /* buffer to store string read-in from user */
   const int MAX_LINE_SIZE = 1000;
   char input[MAX_LINE_SIZE];
   char *copy;

   /* prompt user */
   printf("Enter formulae of proposition logic, one per line (max %d characters)\n",
      MAX_LINE_SIZE);
   printf("Each time I'll tell you if it's a wff.  Hit CTRL+D to exit.\n");

   /* start loop ... */
   while (fgets(input, MAX_LINE_SIZE, stdin) != NULL) {

      /* reset 'copy' to point to start of input buffer
       * remember we have to do this b/c "FormulaWrapper" (and all the parsing
       * functions) modify the string-pointer given to it.
       */
      copy = input;

      if (FormulaWrapper(&copy))
         printf("===> it's a wff :)\n");
      else
         printf("===> NOT A WFF :(\n");
   }

   return 0;
}

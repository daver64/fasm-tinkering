#ifndef CODEGEN_H_
#define CODEGEN_H_
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>

#include <windows.h>

// codegen
const wchar_t *widen(const std::string &narrow, std::wstring &wide);
int exec_program(int argc, const char *argv[]);
void emit_asm(FILE *asmfile, const char *sourcecode);
void emit_program_header(FILE *asmfile);
void emit_code_section_header(FILE *asmfile);
void emit_data_section_header(FILE *asmfile);
void emit_import_section(FILE *asmfile);

// lexxing

#define LEXX_OK (200)
#define LEXX_ERROR (201)
#define LEXX_ERROR_NULL_SOURCE (202)
#define LEXX_ERROR_SYNTAX_ERROR (203)
#define LEXX_ERROR_UNKNOWN_ERROR (204)
#define LEXX_FINISHED (205)

#define LEXX_TOKEN_TABLE_OFFSET (300)

#define LEXX_TOKEN_IF (300)
#define LEXX_TOKEN_ELSE (301)
#define LEXX_TOKEN_WHILE (302)
#define LEXX_TOKEN_DO (303)
#define LEXX_TOKEN_FOR (304)
#define LEXX_TOKEN_STRUCT (305)

#define LEXX_TOKEN_ADDOP (306)
#define LEXX_TOKEN_MULOP (307)
#define LEXX_TOKEN_LEFT_PAREN (308)
#define LEXX_TOKEN_RIGHT_PAREN (309)
#define LEXX_TOKEN_EQUALS (310)
#define LEXX_TOKEN_LT (311)
#define LEXX_TOKEN_GT (312)
#define LEXX_TOKEN_LEFT_CURLY_PAREN (313)
#define LEXX_TOKEN_RIGHT_CURLY_PAREN (314)
#define LEXX_TOKEN_LEFT_SQUARE_PAREN (315)
#define LEXX_TOKEN_RIGHT_SQUARE_PAREN (316)
#define LEXX_TOKEN_INTEGER (317)
#define LEXX_TOKEN_FLOAT (318)
#define LEXX_TOKEN_STRING (319)
#define LEXX_TOKEN_LABEL (320)
#define LEXX_TOKEN_COMMA (321)
#define LEXX_TOKEN_SEMICOLON (322)
#define LEXX_TOKEN_COLON (323)
#define LEXX_TOKEN_FULLSTOP (324)
#define LEXX_TOKEN_AT (325)
#define LEXX_TOKEN_HASH  (326)
#define LEXX_TOKEN_BANG (327)
#define LEXX_TOKEN_DOUBLEQUOTE (328)
#define LEXX_TOKEN_POUND (329)
#define LEXX_TOKEN_DOLLAR (330)
#define LEXX_TOKEN_PERCENT (331)
#define LEXX_TOKEN_CARET (332)
#define LEXX_TOKEN_AND (333)
#define LEXX_TOKEN_BACKSLASH (334)
#define LEXX_TOKEN_FORWARDSLASH (335)
#define LEXX_TOKEN_QUESTION (336)
#define LEXX_TOKEN_PIPE (337)
#define LEXX_TOKEN_UNDERSCORE (338)
#define LEXX_TOKEN_SINGLEQUOTE (339)

#define LEXX_TOKEN_TYPE_INT (340)
#define LEXX_TOKEN_TYPE_FLOAT (341)
#define LEXX_TOKEN_TYPE_STRING (342)

#define LEXX_TOKEN_ARRAY (343)
#define LEXX_TOKEN_MAP (344)

#define LEXX_TOKEN_TABLE_SIZE (42)


#define LEXX_NOT_PUNCTUATION (500)
#define LEXX_NOT_ADDOP (501)
#define LEXX_NOT_MULOP (502)

#define LEXX_LISPISH (1000)
#define LEXX_BASICISH (1001)
#define LEXX_FORTHISH (1002)
#define LEXX_CISH (1003)

struct KeywordEntry
{
	const char *keyword;
	const int ktok_value;
};

struct SourceFile
{
	char *source;
	char *source_end;
};

struct Token 
{
	int tokentype;
	int ivalue;
	float fvalue;
	char *strvalue;
	char cvalue;
	struct Token *next;
	struct Token *prev;
} ;

struct Grammar 
{
	struct Grammar *left;
	struct Grammar *right;
};

SourceFile *open_sourcefile(const char *sourcefile);
void close_sourcefile(SourceFile*);
Token *lexx(SourceFile*);
Token* lexx_scan(SourceFile*);
int lexx_decode(Token*);
void lexx_dump(Token*);
void lexx_free_tokens(Token*);
int lexx_token_count(Token*);
Token *lexx_get_token(Token*,int);
Token *lexx_get_last_token(Token*);
int lexx_consume_next_token(Token **tok);

Grammar *yacc_parse(Token*,int);
char *yacc_codegen(Grammar*);

#endif

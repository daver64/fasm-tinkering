#include "codegen.h"


char *look = NULL;
char *end_of_source = NULL;
char *start_of_source = NULL;
KeywordEntry kwtable[] = {
	{"for", LEXX_TOKEN_FOR},
	{"do", LEXX_TOKEN_DO},
	{"while", LEXX_TOKEN_WHILE},
	{"else", LEXX_TOKEN_ELSE},
	{"struct", LEXX_TOKEN_STRUCT},
	{NULL, 0}};

int lexx_fetch_next_char()
{
	look++;
	if (look >= end_of_source)
		return LEXX_FINISHED;
	return LEXX_OK;
}

int lexx_unfetch_char()
{
	look--;
	return LEXX_OK;
}

int lexx_skipws()
{
	while (*look == ' ' || *look == '\t' || *look == '\n')
	{
		look++;
	}
	return LEXX_OK;
}

int lexx_isdigit(const char c)
{
	return (c >= '0' && c <= '9');
}

int lexx_isalpha(const char c)
{
	return (toupper(c) >= 'A' && toupper(c) <= 'Z');
}
int64_t lexx_read_integer()
{
	char *str = (char*)malloc(64);
	int i = 0;
	do
	{
		str[i] = *look;
		i++;
		lexx_fetch_next_char();
	} while (lexx_isdigit(*look) && i < 64);
	str[i] = 0;
	int64_t result = 0;
	sscanf(str, "%lld", &result);
	free(str);
	return result;
}
char *lexx_read_label()
{
	char *str = (char*)malloc(64);
	int i = 0;
	do
	{
		str[i] = *look;
		i++;
		lexx_fetch_next_char();
	} while (lexx_isalpha(*look) && i < 64);
	str[i] = 0;
	return str;
}
int lexx_ispunctuation()
{
	switch (*look)
	{
	case '{':
		return LEXX_TOKEN_LEFT_CURLY_PAREN;
		break;

	case '}':
		return LEXX_TOKEN_RIGHT_CURLY_PAREN;
		break;

	case ';':
		return LEXX_TOKEN_SEMICOLON;
		break;

	case ':':
		return LEXX_TOKEN_COLON;
		break;

	case ',':
		return LEXX_TOKEN_COMMA;
		break;

	case '.':
		return LEXX_TOKEN_FULLSTOP;
		break;

	case '(':
		return LEXX_TOKEN_LEFT_PAREN;
		break;

	case ')':
		return LEXX_TOKEN_RIGHT_PAREN;
		break;

	case '[':
		return LEXX_TOKEN_LEFT_SQUARE_PAREN;
		break;

	case ']':
		return LEXX_TOKEN_RIGHT_SQUARE_PAREN;
		break;

	case '=':
		return LEXX_TOKEN_EQUALS;
		break;

	case '<':
		return LEXX_TOKEN_LT;
		break;

	case '>':
		return LEXX_TOKEN_GT;
		break;

	case '@':
		return LEXX_TOKEN_AT;
		break;

	case '#':
		return LEXX_TOKEN_HASH;
		break;
	case '!':
		return LEXX_TOKEN_BANG;
		break;
	case '\"':
		return LEXX_TOKEN_DOUBLEQUOTE;
		break;
	case '$':
		return LEXX_TOKEN_DOLLAR;
		break;
	case '%':
		return LEXX_TOKEN_PERCENT;
		break;
	case '^':
		return LEXX_TOKEN_CARET;
		break;
	case '&':
		return LEXX_TOKEN_AND;
		break;
	case '_':
		return LEXX_TOKEN_UNDERSCORE;
		break;
	}

	return LEXX_NOT_PUNCTUATION;
}
int lexx_isaddop()
{
	switch (*look)
	{
	case '+':
		return LEXX_TOKEN_ADDOP;
		break;

	case '-':
		return LEXX_TOKEN_ADDOP;
		break;
	}

	return LEXX_NOT_ADDOP;
}
int lexx_ismulop()
{
	switch (*look)
	{
	case '*':
		return LEXX_TOKEN_MULOP;
		break;

	case '/':
		return LEXX_TOKEN_MULOP;
		break;
	}

	return LEXX_NOT_MULOP;
}


int lexx_consume_next_token(Token **tok)
{
	if (!tok)
		return LEXX_ERROR;
	if (!(*tok))
		return LEXX_ERROR_NULL_SOURCE;

	Token *removed_token = *tok;
	if (removed_token->strvalue)
	{
		free(removed_token->strvalue);
	}

	if ((*tok)->next != NULL)
	{
		*tok = (*tok)->next;
		(*tok)->prev = NULL;
	}
	else
	{
		printf("free last %p\n",(*tok));
		free( (*tok));
		*tok=NULL;
		return LEXX_OK;
	}

	printf("free token %p\n",removed_token);
	free(removed_token);
	return LEXX_OK;
}
int lexx_iskeyword(char *str)
{
	KeywordEntry *ke = &kwtable[0];
	do
	{
		if (strcmp(ke->keyword, str) == 0)
			return ke->ktok_value;
		ke++;
	} while (ke->keyword != NULL);

	return LEXX_TOKEN_LABEL;
}

int lexx_istype(char *str)
{
	if (strcmp(str, "int") == 0)
	{
		return LEXX_TOKEN_TYPE_INT;
	}
	else if (strcmp(str, "float") == 0)
	{
		return LEXX_TOKEN_TYPE_FLOAT;
	}
	else if (strcmp(str, "string") == 0)
	{
		return LEXX_TOKEN_TYPE_STRING;
	}

	return LEXX_TOKEN_LABEL;
}
Token *lexx_scan(SourceFile *sf)
{
	Token *lexx_root = NULL;
	Token *lexx_root_ptr = NULL;
	Token *token = NULL;

	int tokcount = 0;
	do
	{
		token = lexx(sf);
		tokcount++;
		if (lexx_root == NULL)
		{
			token->prev = NULL;
			token->next = NULL;
			lexx_root = token;
			lexx_root_ptr = token;
		}
		else
		{
			if (token->tokentype != LEXX_FINISHED)
			{
				lexx_root_ptr->next = token;
				token->prev = lexx_root_ptr;
				lexx_root_ptr = token;
				lexx_root_ptr->next = NULL;
			}
			else
			{
				tokcount--;
			}
		}

	} while (token->tokentype != LEXX_FINISHED && token->tokentype != LEXX_ERROR_UNKNOWN_ERROR && token->tokentype != LEXX_ERROR);
	return lexx_root;
}

Token *lexx(SourceFile *sourcefile)
{
	Token *token = (Token*)malloc(sizeof(Token));
	memset(token, 0, sizeof(Token));

	if (!sourcefile)
	{
		token->tokentype = LEXX_ERROR_NULL_SOURCE;
		return token;
	}
	if (!sourcefile->source)
	{
		token->tokentype = LEXX_ERROR_NULL_SOURCE;
		return token;
	}

	if (!look)
	{
		look = sourcefile->source;
		start_of_source = sourcefile->source;
		end_of_source = sourcefile->source_end;
	}

	lexx_skipws();

	int punctuation_temp = LEXX_NOT_PUNCTUATION;
	int addop_temp = LEXX_NOT_ADDOP;
	int mulop_temp = LEXX_NOT_MULOP;

	if (*look == '\0')
	{
		look = NULL;
		start_of_source = NULL;
		end_of_source = NULL;
		token->tokentype = LEXX_FINISHED;
		return token;
	}

	if (lexx_isdigit(*look))
	{
		int64_t n = lexx_read_integer();
		token->tokentype = LEXX_TOKEN_INTEGER;
		token->ivalue = n;
		return token;
	}
	else if (lexx_isalpha(*look))
	{
		char *label = lexx_read_label();
		token->tokentype = lexx_iskeyword(label);
		// if lexx_is_keyword() returns LABEL, then it might still be
		// a var type, so we check for that . lexx_istype() returns
		// LABEL if it's not a var type either.
		if (token->tokentype == LEXX_TOKEN_LABEL)
			token->tokentype = lexx_istype(label);
		token->strvalue = label;
		return token;
	}
	else if (*look == '\"')
	{
		// attempt to read a quoted string
		lexx_fetch_next_char();
		if (*look == '\"')
		{
			// empty string..
			token->tokentype = LEXX_TOKEN_STRING;
			token->strvalue = (char*)malloc(1);
			token->strvalue[0] = '\0';
			return token;
		}
		else
		{
			char *tmp_ptr = look;
			char *str = (char*)malloc(1024);
			int len = 0;
			int lresult = LEXX_OK;
			do
			{
				str[len] = *look;
				lresult = lexx_fetch_next_char();
				len++;

			} while (lresult != LEXX_FINISHED && *look != '\"' && len < 1023);
			lexx_fetch_next_char();
			str[len] = 0;
			// printf("STRING={%s}LEN={%d}\n",str,len);
			token->strvalue = str;
			token->tokentype = LEXX_TOKEN_STRING;
			return token;
		}
	}
	else if ((punctuation_temp = lexx_ispunctuation()) != LEXX_NOT_PUNCTUATION)
	{
		token->tokentype = punctuation_temp;
		token->cvalue = *look;
		lexx_fetch_next_char();
		return token;
	}
	else if ((addop_temp = lexx_isaddop()) != LEXX_NOT_ADDOP)
	{
		token->tokentype = addop_temp;
		token->cvalue = *look;
		lexx_fetch_next_char();
		return token;
	}
	else if ((mulop_temp = lexx_ismulop()) != LEXX_NOT_MULOP)
	{
		token->tokentype = mulop_temp;
		token->cvalue = *look;
		lexx_fetch_next_char();
		return token;
	}
	token->tokentype = LEXX_ERROR;
	return token;
}
void lexx_dump(Token *token)
{
	if(!token)
		return;
	Token *tok_iter = token;
	do
	{
		lexx_decode(tok_iter);
		printf("\t <--tok=%p  ", tok_iter);
		printf("tok->prev=%p  ", tok_iter->prev);
		printf("tok->next=%p -->\n", tok_iter->next);

		tok_iter = tok_iter->next;
	} while (tok_iter != NULL);
}
void lexx_free_tokens(Token *lexx_root)
{
	if(!lexx_root)
		return;
	Token *lexx_ptr = lexx_root;
	while (lexx_ptr->next != NULL)
	{
		if (lexx_ptr->prev != NULL)
		{
			// printf("free addr %p ,",lexx_ptr->prev);
			free(lexx_ptr->prev);
		}
		lexx_ptr = lexx_ptr->next;
	}
	// printf("free addr %p\n",lexx_ptr);
	free(lexx_ptr);
}
int lexx_token_count(Token *lexx_root)
{
	Token *lexx_ptr = lexx_root;
	int count = 1;
	while (lexx_ptr->next != NULL)
	{
		count++;
		lexx_ptr = lexx_ptr->next;
	}
	return count;
}
Token *lexx_get_token(Token *lexx_root, int index)
{
	Token *lexx_ptr = lexx_root;
	int count = 0;
	int maxindex = lexx_token_count(lexx_root);
	while (lexx_ptr->next != NULL && count < maxindex)
	{
		count++;
		if (count == index)
			return lexx_ptr;
		lexx_ptr = lexx_ptr->next;
	}
	return NULL;
}
Token *lexx_get_last_token(Token *lexx_root)
{
	Token *lexx_ptr = lexx_root;
	int count = 0;
	while (lexx_ptr->next != NULL)
	{
		lexx_ptr = lexx_ptr->next;
	}
	return lexx_ptr;
}
SourceFile *open_sourcefile(const char *sourcefile)
{
	SourceFile *sf = NULL;
	FILE *fp = fopen(sourcefile, "r");
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		long len = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		sf = (SourceFile*)malloc(sizeof(SourceFile));
		if (sf)
		{
			sf->source = (char*)malloc(len + 1);
			if (sf)
			{
				long lread = fread(sf->source, 1, len, fp);
				if (lread != len)
				{
					free(sf->source);
					free(sf);
					sf = NULL;
				}
				sf->source_end = sf->source + lread;
			}
		}
		fclose(fp);
	}
	return sf;
}
void close_sourcefile(SourceFile *sf)
{
	if (sf)
	{
		if (sf->source)
		{
			free(sf->source);
		}
		free(sf);
	}
}
int lexx_decode(Token *token)
{
	if (!token)
	{
		printf("NULL TOKEN\n");
		return LEXX_ERROR;
	}
	printf("[");
	switch (token->tokentype)
	{
	case LEXX_OK:
		printf("LEXX_OK");
		break;

	case LEXX_ERROR:
		printf("LEXX_ERROR");
		break;

	case LEXX_ERROR_NULL_SOURCE:
		printf("LEXX_ERROR_NULL_SOURCE");
		break;

	case LEXX_ERROR_SYNTAX_ERROR:
		printf("LEXX_ERROR_SYNTAX_ERROR");
		break;

	case LEXX_ERROR_UNKNOWN_ERROR:
		printf("LEXX_ERROR_UNCKNOWN_ERROR");
		break;

	case LEXX_FINISHED:
		printf("LEXX_FINISHED");
		break;

	case LEXX_TOKEN_IF:
		printf("KEYWORD %s", token->strvalue);
		break;

	case LEXX_TOKEN_ELSE:
		printf("KEYWORD %s", token->strvalue);
		break;

	case LEXX_TOKEN_WHILE:
		printf("KEYWORD %s", token->strvalue);
		break;

	case LEXX_TOKEN_DO:
		printf("KEYWORD %s", token->strvalue);
		break;

	case LEXX_TOKEN_FOR:
		printf("KEYWORD %s", token->strvalue);
		break;

	case LEXX_TOKEN_STRUCT:
		printf("KEYWORD %s", token->strvalue);
		break;

	case LEXX_TOKEN_ADDOP:
		printf("%c", token->cvalue);
		break;

	case LEXX_TOKEN_MULOP:
		printf("%c", token->cvalue);
		break;

	case LEXX_TOKEN_COMMA:
		printf(",");
		break;

	case LEXX_TOKEN_SEMICOLON:
		printf(";");
		break;

	case LEXX_TOKEN_COLON:
		printf(":");
		break;

	case LEXX_TOKEN_FULLSTOP:
		printf(".");
		break;

	case LEXX_TOKEN_EQUALS:
		printf("=");
		break;

	case LEXX_TOKEN_FLOAT:
		printf("FLOAT %lf", token->fvalue);
		break;

	case LEXX_TOKEN_STRING:
		printf("STRING \"%s\"", token->strvalue);
		break;

	case LEXX_TOKEN_LABEL:
		printf("LABEL %s", token->strvalue);
		break;

	case LEXX_TOKEN_INTEGER:
		printf("INT %d", token->ivalue);
		break;

	case LEXX_TOKEN_LEFT_CURLY_PAREN:
		printf("{");
		break;

	case LEXX_TOKEN_RIGHT_CURLY_PAREN:
		printf("}");
		break;

	case LEXX_TOKEN_LEFT_PAREN:
		printf("(");
		break;

	case LEXX_TOKEN_RIGHT_PAREN:
		printf(")");
		break;

	case LEXX_TOKEN_LEFT_SQUARE_PAREN:
		printf("[");
		break;

	case LEXX_TOKEN_RIGHT_SQUARE_PAREN:
		printf("]");
		break;

	case LEXX_TOKEN_AT:
		printf("@");
		break;

	case LEXX_TOKEN_HASH:
		printf("#");
		break;

	case LEXX_TOKEN_LT:
		printf("<");
		break;

	case LEXX_TOKEN_GT:
		printf(">");
		break;
	case LEXX_TOKEN_BANG:
		printf("!");
		break;
	case LEXX_TOKEN_DOUBLEQUOTE:
		printf("\"");
		break;
	case LEXX_TOKEN_POUND:
		printf("£");
		break;
	case LEXX_TOKEN_DOLLAR:
		printf("$");
		break;
	case LEXX_TOKEN_PERCENT:
		printf("%%");
		break;
	case LEXX_TOKEN_CARET:
		printf("^");
		break;
	case LEXX_TOKEN_AND:
		printf("&");
		break;
	case LEXX_TOKEN_BACKSLASH:
		printf("\\");
		break;
	case LEXX_TOKEN_FORWARDSLASH:
		printf("/");
		break;
	case LEXX_TOKEN_QUESTION:
		printf("?");
		break;
	case LEXX_TOKEN_PIPE:
		printf("|");
		break;
	case LEXX_TOKEN_UNDERSCORE:
		printf("_");
		break;
	case LEXX_TOKEN_SINGLEQUOTE:
		printf("'");
		break;

	case LEXX_TOKEN_TYPE_INT:
		printf("INTVAR");
		break;

	case LEXX_TOKEN_TYPE_FLOAT:
		printf("FLOATVAR");
		break;

	case LEXX_TOKEN_TYPE_STRING:
		printf("STRINGVAR");
		break;
	default:
		printf("UNKNOWN %d", token->tokentype);
		break;
	}

	printf("]");
	return LEXX_OK;
}

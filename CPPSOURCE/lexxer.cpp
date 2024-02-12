#include "codegen.h"

FILE *lexx_source_file{nullptr};

char look{0};


bool lexx_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}
void lexx_get_next_character()
{
    look = fgetc(lexx_source_file);
}
void lexx_skipws()
{
    while (lexx_isspace(look))
    {
        lexx_get_next_character();
    }
}
void lexx_open_file(const char *filename)
{
    lexx_source_file = fopen(filename, "r");
    if (!lexx_source_file)
    {
        fprintf(stderr, "couldn't open %s\n", filename);
        exit(1);
    }
}

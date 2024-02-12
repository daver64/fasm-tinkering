
/*

    Testing ideas for code generation.
    spit out assembler source for a working executable file.

*/
#include "codegen.h"

void emit_asm(FILE *asmfile, const char *sourcecode)
{
    fprintf(asmfile, "%s\n", sourcecode);
}
void emit_program_header(FILE *asmfile)
{
    emit_asm(asmfile, "format pe console");
    emit_asm(asmfile, "include 'win32ax.inc'");
    emit_asm(asmfile, "entry main");
}
void emit_code_section_header(FILE *asmfile)
{
    emit_asm(asmfile, "\nsection '.text' code executable readable");
}
void emit_data_section_header(FILE *asmfile)
{
    emit_asm(asmfile, "\nsection '.data' writeable readable");
}
void emit_import_section(FILE *asmfile)
{
    emit_asm(asmfile, "\nsection '.imports' import data readable");
    emit_asm(asmfile, "\nlibrary kernel32,'kernel32.dll', \\");
    emit_asm(asmfile, "\tmsvcrt,'msvcrt.dll'");
    emit_asm(asmfile, "\nimport kernel32,\\");
    emit_asm(asmfile, "\tExitProcess,'ExitProcess'");
    emit_asm(asmfile, "\nimport msvcrt,\\");
    emit_asm(asmfile, "\tprintf,'printf',\\");
    emit_asm(asmfile, "\tsystem,'system',\\");
    emit_asm(asmfile, "\tgetchar,'getchar',\\");
    emit_asm(asmfile, "\tputchar,'putchar',\\");
    emit_asm(asmfile, "\tputs,'puts',\\");
    emit_asm(asmfile, "\tfopen,'fopen',\\");
    emit_asm(asmfile, "\tfclose,'fclose',\\");
    emit_asm(asmfile, "\tfprintf,'fprintf',\\");
    emit_asm(asmfile, "\tfread,'fread',\\");
    emit_asm(asmfile, "\tfwrite,'fwrite',\\");
    emit_asm(asmfile, "\tfseek,'fseek',\\");
    emit_asm(asmfile, "\tftell,'ftell',\\");
    emit_asm(asmfile, "\tmalloc,'malloc',\\");
    emit_asm(asmfile, "\tfree,'free'\\");
}

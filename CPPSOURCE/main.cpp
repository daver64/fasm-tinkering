/*

    Testing ideas for code generation.
    spit out assembler source for a working executable file.

*/

#include "codegen.h"


//
// TODO: accept parameters including source file to convert to assembler.
// for now we just construct a simple working program by hand.
int main(int argc, char *argv[])
{

    FILE *asmfile = fopen("main.asm", "w");
    if(!asmfile)
    {
        fprintf(stderr,"couldn't open assembler file to write to.\n");
        return -1;
    }
    emit_program_header(asmfile);

    emit_data_section_header(asmfile);
    emit_asm(asmfile, "fmt_hex_int db '%X',0");
    emit_asm(asmfile, "fmt_dec_int db '%d',0");
    emit_asm(asmfile, "fmt_cstr db '%s',0");
    emit_asm(asmfile, "fmt_float db '%f',0");

    emit_asm(asmfile, "version_str db 'version 1.0',0");

    emit_code_section_header(asmfile);
    emit_asm(asmfile, "main:");
    emit_asm(asmfile, "\tmov eax,version_str");
    emit_asm(asmfile, "\tcinvoke printf,fmt_cstr,eax");
    emit_asm(asmfile, "\tinvoke ExitProcess");

    emit_import_section(asmfile);
    fclose(asmfile);

    const char *args[]{
        "..\\fasm.exe",
        "main.asm",
        "main.exe"};
    exec_program(3, args);

    return 0;
}
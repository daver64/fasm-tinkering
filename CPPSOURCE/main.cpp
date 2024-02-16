/*

    Testing ideas for code generation.
    spit out assembler source for a working executable file.

*/

#include "codegen.h"

int build_test_program();

//
// TODO: accept parameters including source file to convert to assembler.
// for now we just construct a simple working program by hand.
int main(int argc, char *argv[])
{
    //fprintf(stderr,"dfrc ver 0.1\n");
    SourceFile *sf=open_sourcefile("..\\foo.nc");
    //fprintf(stderr,"lexx_scan\n");
    Token *lexx_root = lexx_scan(sf);
    //fprintf(stderr,"lexx_dump\n");
    lexx_dump(lexx_root);
    //fprintf(stderr,"scanning...\n");
    do {
        lexx_consume_next_token(&lexx_root);
        //fprintf(stdout,"\n\tToken Consumed lexx_ptr=%p\n",lexx_root);
        //lexx_dump(lexx_root);
    } while(lexx_root);
    lexx_free_tokens(lexx_root);
    close_sourcefile(sf);
    return 0;
}

int build_test_program()
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
/*

    Testing ideas for code generation.
    spit out assembler source for a working executable file.

*/

#include "codegen.h"

int build_test_program();

void emit_new_callframe(FILE *asmfile, const char *label, int argc, int *argv[])
{
    fprintf(asmfile, "%s:\n", label);
    fprintf(asmfile, "push ebp\n");
    fprintf(asmfile, "mov ebp,esp\n");
}

void emit_finalise_callframe(FILE *asmfile)
{
    fprintf(asmfile, "mov esp,ebp\n");
    fprintf(asmfile, "pop ebp\n");
    fprintf(asmfile, "ret\n");
}
//
// TODO: accept parameters including source file to convert to assembler.
// for now we just construct a simple working program by hand.
int main(int argc, char *argv[])
{
    FILE *asmfile = fopen("..\\foo.asm", "w");
    emit_program_header(asmfile);
    SourceFile *sf = open_sourcefile("..\\foo.nc");
    Token *current_token = lexx_scan(sf);
    emit_data_section_header(asmfile);
    emit_code_section_header(asmfile);

    // fprintf(asmfile,"\n\nmain:\n");
    emit_new_callframe(asmfile, "main", 0, 0);
    fprintf(asmfile, "xor eax,eax\n");
    fprintf(asmfile, "xor edx,edx\n");
    fprintf(asmfile, "xor ecx,ecx\n");
    fprintf(asmfile, "xor ebx,ebx\n");
    fprintf(asmfile, "xor esi,esi\n");
    fprintf(asmfile, "mov edi,nstack\n");
    do
    {

        if (current_token->tokentype == LEXX_TOKEN_INTEGER)
        {
            fprintf(asmfile, "mov eax,%d\n", current_token->ivalue);
            fprintf(asmfile, "sub edi,4\n");
            fprintf(asmfile, "mov [edi],eax\n\n");
        }
        else if (current_token->tokentype == LEXX_TOKEN_ADDOP)
        {
            fprintf(asmfile, "mov ebx,[edi]\n");
            fprintf(asmfile, "add edi,4\n");
            fprintf(asmfile, "mov eax,[edi]\n");
            if (current_token->cvalue == '+')
                fprintf(asmfile, "add eax,ebx\n");
            else if (current_token->cvalue == '-')
                fprintf(asmfile, "sub eax,ebx\n");
            fprintf(asmfile, "mov [edi],eax\n\n");
        }
        else if (current_token->tokentype == LEXX_TOKEN_MULOP)
        {
            fprintf(asmfile, "xor edx,edx\n");
            fprintf(asmfile, "mov ebx,[edi]\n");
            fprintf(asmfile, "add edi,4\n");
            fprintf(asmfile, "mov eax,[edi]\n");
            if (current_token->cvalue == '*')
                fprintf(asmfile, "imul eax,ebx\n");
            else if (current_token->cvalue == '/')
                fprintf(asmfile, "idiv ebx\n");
            fprintf(asmfile, "mov [edi],eax\n\n");
        }
        else if (current_token->tokentype == LEXX_TOKEN_FULLSTOP)
        {
            fprintf(asmfile, "cinvoke printf, fmt_int,dword [edi]\n\n");
        }
        lexx_consume_next_token(&current_token);
    } while (current_token);

    fprintf(asmfile, "mov eax,0\n");
    fprintf(asmfile, "invoke ExitProcess,eax\n\n");

    emit_finalise_callframe(asmfile);
    lexx_free_tokens(current_token);
    close_sourcefile(sf);
    emit_import_section(asmfile);
    fclose(asmfile);
    return 0;
}

int build_test_program()
{

    FILE *asmfile = fopen("main.asm", "w");
    if (!asmfile)
    {
        fprintf(stderr, "couldn't open assembler file to write to.\n");
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
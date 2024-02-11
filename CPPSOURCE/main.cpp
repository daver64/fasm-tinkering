/*

    Testing ideas for code generation.
    spit out assembler source for a working executable file.
    
*/

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <string>

#include <windows.h>
const wchar_t *widen(const std::string &narrow, std::wstring &wide)
{
    size_t length = narrow.size();

    if (length > 0)
    {
        wide.resize(length);
        length = MultiByteToWideChar(CP_UTF8, 0, narrow.c_str(), (int)length, (wchar_t *)wide.c_str(), (int)length);
        wide.resize(length);
    }
    else
    {
        wide.clear();
    }

    return wide.c_str();
}
int exec_program(int argc, const char *argv[])
{
    if (argc < 1)
    {
        fprintf(stderr, "program filename must be supplied\n");
        return -1;
    }
    std::string app = argv[0];
    std::string arg;
    for (int i = 1; i < argc; i++)
    {
        arg = arg + std::string(argv[i]) + std::string(" ");
    }
    // fprintf(stderr,"program=%s\n",app.c_str());
    // fprintf(stderr,"args=%s\n",arg.c_str());
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    std::wstring app_w;
    widen(app, app_w);

    std::wstring arg_w;
    widen(arg, arg_w);

    std::wstring input = app_w + L" " + arg_w;
    wchar_t *arg_concat = const_cast<wchar_t *>(input.c_str());
    const wchar_t *app_const = app_w.c_str();

    if (!CreateProcessW(app_const,
                        arg_concat,
                        NULL,
                        NULL,
                        FALSE,
                        0,
                        NULL,
                        NULL,
                        &si,
                        &pi))
    {
        fprintf(stderr, "Error %d\n", GetLastError());
        return -2;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}

void emit_program_header(FILE *asmfile)
{
    fprintf(asmfile, "%s\n", "format pe console");
    fprintf(asmfile, "%s\n", "include 'win32ax.inc'");
    fprintf(asmfile, "%s\n", "entry main");
}
void emit_code_section_header(FILE *asmfile)
{
    fprintf(asmfile, "%s\n", "\nsection '.text' code executable readable");
}
void emit_data_section_header(FILE *asmfile)
{
    fprintf(asmfile, "%s\n", "\nsection '.data' writeable readable");
}
void emit_import_section(FILE *asmfile)
{
    fprintf(asmfile, "%s\n", "\nsection '.imports' import data readable");
    fprintf(asmfile, "%s\n", "\nlibrary kernel32,'kernel32.dll', \\");
    fprintf(asmfile, "%s\n", "\tmsvcrt,'msvcrt.dll'");
    fprintf(asmfile, "%s\n", "\nimport kernel32,\\");
    fprintf(asmfile, "%s\n", "\tExitProcess,'ExitProcess'");
    fprintf(asmfile, "%s\n", "\nimport msvcrt,\\");
    fprintf(asmfile, "%s\n", "\tprintf,'printf',\\");
    fprintf(asmfile, "%s\n", "\tsystem,'system',\\");
    fprintf(asmfile, "%s\n", "\tgetchar,'getchar',\\");
    fprintf(asmfile, "%s\n", "\tputchar,'putchar',\\");
    fprintf(asmfile, "%s\n", "\tputs,'puts',\\");
    fprintf(asmfile, "%s\n", "\tfopen,'fopen',\\");
    fprintf(asmfile, "%s\n", "\tfclose,'fclose',\\");
    fprintf(asmfile, "%s\n", "\tfprintf,'fprintf',\\");
    fprintf(asmfile, "%s\n", "\tfread,'fread',\\");
    fprintf(asmfile, "%s\n", "\tfwrite,'fwrite',\\");
    fprintf(asmfile, "%s\n", "\tfseek,'fseek',\\");
    fprintf(asmfile, "%s\n", "\tftell,'ftell',\\");
    fprintf(asmfile, "%s\n", "\tmalloc,'malloc',\\");
    fprintf(asmfile, "%s\n", "\tfree,'free'\\");
}
void emit_asm(FILE *asmfile, const char *sourcecode)
{
    fprintf(asmfile, "%s\n", sourcecode);
}
int main(int argc, char *argv[])
{

    FILE *asmfile = fopen("main.asm", "w");
    emit_program_header(asmfile);

    emit_code_section_header(asmfile);
    emit_asm(asmfile, "main:");
    emit_asm(asmfile, "\tmov eax,0");
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
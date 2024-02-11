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

int exec_fasm(const char *sourcefile, const char *outfile)
{
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    std::string app("..\\fasm.exe");
    std::string arg;
    arg=arg+std::string(sourcefile)+std::string(" ")+std::string(outfile);
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
        return 1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}

void emit_header(FILE *asmfile)
{
    fprintf(asmfile, "%s\n", "format pe64 console");
    fprintf(asmfile, "%s\n", "include 'win64ax.inc'");
    fprintf(asmfile, "%s\n", "entry main");
}
void emit_code_section(FILE *asmfile)
{
    fprintf(asmfile,"%s\n","section '.txt' code executable readable");
    fprintf(asmfile,"%s\n","main:");
    fprintf(asmfile,"%s\n","mov rax,0");
    fprintf(asmfile,"%s\n","invoke ExitProcess");
}
void emit_data_section(FILE *asmfile)
{
    fprintf(asmfile,"%s\n","section '.data' writeable readable");  
}
void emit_footer(FILE *asmfile)
{
    fprintf(asmfile,"%s\n", "section '.imports' import data readable");
    fprintf(asmfile, "%s\n", "library kernel32,'kernel32.dll', \\");
    fprintf(asmfile, "%s\n", "\tmsvcrt,'msvcrt.dll'");
    fprintf(asmfile, "%s\n", "import kernel32,\\");
    fprintf(asmfile, "%s\n", "\tExitProcess,'ExitProcess'");
    fprintf(asmfile, "%s\n", "import msvcrt,\\");
    fprintf(asmfile, "%s\n", "\tprintf,'printf',\\");
    fprintf(asmfile, "%s\n", "\tsystem,'system',\\");
    fprintf(asmfile, "%s\n", "\tgetchar,'getchar'");
}
int main(int argc, char *argv[])
{

    FILE *asmfile=fopen("main.asm","w");
    emit_header(asmfile);
    emit_code_section(asmfile);
    emit_footer(asmfile);
    fclose(asmfile);

    exec_fasm("main.asm","main.exe");
    
    return 0;
}
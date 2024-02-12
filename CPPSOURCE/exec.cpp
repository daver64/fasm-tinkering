#include "codegen.h"


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
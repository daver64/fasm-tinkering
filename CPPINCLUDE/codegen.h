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
void lexx_get_next_character();
void lexx_skipws();

#endif

format pe console
include 'win32ax.inc'
entry main

section '.data' writeable readable
times 512 dd 0
nstack: dd 0
fmt_int: db '%d',0

section '.text' code executable readable
main:
push ebp
mov ebp,esp
xor eax,eax
xor edx,edx
xor ecx,ecx
xor ebx,ebx
xor esi,esi
mov edi,nstack
mov eax,84
sub edi,4
mov [edi],eax

mov eax,2
sub edi,4
mov [edi],eax

xor edx,edx
mov ebx,[edi]
add edi,4
mov eax,[edi]
idiv ebx
mov [edi],eax

cinvoke printf, fmt_int,dword [edi]

mov eax,0
invoke ExitProcess,eax

mov esp,ebp
pop ebp
ret

section '.imports' import data readable

library kernel32,'kernel32.dll', \
	msvcrt,'msvcrt.dll'

import kernel32,\
	ExitProcess,'ExitProcess'

import msvcrt,\
	printf,'printf',\
	system,'system',\
	getchar,'getchar',\
	putchar,'putchar',\
	puts,'puts',\
	fopen,'fopen',\
	fclose,'fclose',\
	fprintf,'fprintf',\
	fread,'fread',\
	fwrite,'fwrite',\
	fseek,'fseek',\
	ftell,'ftell',\
	malloc,'malloc',\
	free,'free'\

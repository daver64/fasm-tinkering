format pe64 console


include 'win64ax.inc'

entry main

section '.data' data readable writeable

strHello db 'Hello World !',13,10,0
strPause db 'pause',0

section '.txt' code executable readable

main:
    cinvoke printf,strHello
    cinvoke getchar
    invoke ExitProcess, 0

section '.imports' import data readable

library kernel32,'kernel32.dll',\
    msvcrt,'msvcrt.dll' 

import kernel32,\
    ExitProcess,'ExitProcess'

import msvcrt,\
    printf,'printf',\
    system,'system',\
    getchar,'getchar'
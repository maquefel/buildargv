/*
Copyright (c) 2019, Nikita Shubin <me@maquefel.me>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the "buildargv" project.
*/

/*
* Parse cmdline into argc, argv[]
*/

#include "buildargv.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>

%%{
    machine buildargv;
    write data;
}%%

int buildargv(char* input, char*** argv, int *argc)
{
    const char *p = input;
    const char *pe = input + strlen(input);
    const char *eof = pe;
    const char* ts, * te;
    int cs, act, top, stack[2], curline;

    const char *argv_s;
    argv_s = p;

    int ret = 0;
    int errsv = 0;

    (*argv) = 0;
    char **nargv;
    int argc_ = 0;

    %%{
        action dquote_err {
            ret = -1;
            errsv = BUILDARGV_EDQUOTE;
        }

        action squote_err {
            ret = -1;
            errsv = BUILDARGV_ESQUOTE;
        }

        action start_arg {
            argv_s = p;
        }

        action end_arg {
            nargv = (char**)realloc((*argv), (argc_ + 1)*sizeof(char*));
            (*argv) = nargv;
            (*argv)[argc_] = strndup(argv_s, p - argv_s);
            argc_++;
        }

        skip := any @{ fret; };
        dquote :=  ( '\\'>{ fcall skip; } | ^["\\] )+ :> ["] @{ fret; } @err(dquote_err);
        squote :=  ( '\\'>{ fcall skip; } | ^['\\] )+ :> ['] @{ fret; } @err(squote_err);

        whitespace = [ \t];
        arg = '\''> { fcall squote; } | '"'>{ fcall dquote; } | ( '\\'>{fcall skip;} | ^[ \t"'\\] )+;

        lineElement = arg >start_arg %end_arg | whitespace;

        main := lineElement**;

        # Initialize and execute.
        write init;
        write exec;
    }%%

    /** last element zero */
    nargv = (char**)realloc((*argv), (argc_ + 1)*sizeof(char*));
    (*argv) = nargv;
    (*argv)[argc_] = 0;

    *argc = argc_;

    if(ret == -1)
    {
        errno = errsv;
        return -1;
    }

    return 0;
};

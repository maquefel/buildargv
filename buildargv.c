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


static const char _buildargv_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1,
	3, 1, 4, 1, 5, 1, 6, 1,
	7, 1, 8, 1, 11, 2, 2, 9,
	2, 2, 10, 2, 2, 11, 2, 3,
	2, 3, 3, 2, 9, 3, 3, 2,
	10, 3, 3, 2, 11, 0
};

static const char _buildargv_trans_keys[] = {
	1, 0, 1, 0, 2, 4, 2, 4,
	3, 4, 3, 4, 0, 4, 0, 4,
	0, 4, 1, 0, 1, 0, 1, 0,
	0
};

static const char _buildargv_char_class[] = {
	0, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,
	1, 2, 1, 1, 1, 1, 3, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 4, 0
};

static const char _buildargv_index_offsets[] = {
	0, 0, 0, 3, 6, 8, 10, 15,
	20, 25, 25, 25, 0
};

static const char _buildargv_indicies[] = {
	2, 1, 3, 4, 1, 3, 2, 6,
	7, 6, 9, 8, 10, 11, 12, 14,
	13, 15, 16, 17, 14, 18, 15, 16,
	19, 0
};

static const char _buildargv_index_defaults[] = {
	0, 0, 1, 1, 5, 5, 8, 13,
	18, 2, 2, 2, 0
};

static const char _buildargv_trans_cond_spaces[] = {
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 0
};

static const char _buildargv_cond_targs[] = {
	9, 3, 0, 3, 10, 5, 5, 11,
	7, 6, 8, 8, 7, 7, 6, 8,
	8, 7, 7, 7, 0
};

static const char _buildargv_cond_actions[] = {
	9, 0, 0, 11, 13, 0, 15, 17,
	5, 0, 24, 21, 27, 0, 7, 37,
	33, 19, 30, 41, 0
};

static const char _buildargv_eof_actions[] = {
	0, 0, 0, 1, 0, 3, 0, 7,
	7, 0, 0, 0, 0
};

static const char _buildargv_nfa_targs[] = {
	0, 0
};

static const char _buildargv_nfa_offsets[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0
};

static const char _buildargv_nfa_push_actions[] = {
	0, 0
};

static const char _buildargv_nfa_pop_trans[] = {
	0, 0
};

static const int buildargv_start = 6;
static const int buildargv_first_final = 6;
static const int buildargv_error = 0;

static const int buildargv_en_skip = 1;
static const int buildargv_en_dquote = 2;
static const int buildargv_en_squote = 4;
static const int buildargv_en_main = 6;



int buildargv(char* input, char*** argv, int *argc)
{
	const char *p = input;
	const char *pe = input + strlen(input);
	const char *eof = pe;
	const char* ts, * te;
	int cs, act, top, stack[2], curline;
	
	const char *argv_s;
	argv_s = p;
	
	int ret=0;
	int errsv=0;
	
	(*argv) = 0;
	char **nargv;
	int argc_=0;
	
	
	{
		cs = (int)buildargv_start;
		top = 0;
	}
	
	{
		int _trans = 0;
		const char *_acts;
		unsigned int _nacts;
		const char *_keys;
		const char *_inds;
		{
			
			if ( p == pe )
			goto _test_eof;
			if ( cs == 0 )
			goto _out;
			_resume:  {
				_keys = ( _buildargv_trans_keys + ((cs<<1)));
				_inds = ( _buildargv_indicies + (_buildargv_index_offsets[cs]));
				
				if ( ( (*( p))) <= 92 && ( (*( p))) >= 9 )
				{
					int _ic = (int)_buildargv_char_class[(int)( (*( p))) - 9];
					if ( _ic <= (int)(*( _keys+1)) && _ic >= (int)(*( _keys)) )
					_trans = (int)(*( _inds + (int)( _ic - (int)(*( _keys)) ) )); 
					else
					_trans = (int)_buildargv_index_defaults[cs];
				}
				else {
					_trans = (int)_buildargv_index_defaults[cs];
				}
				
				goto _match_cond;
			}
			_match_cond:  {
				cs = (int)_buildargv_cond_targs[_trans];
				
				if ( _buildargv_cond_actions[_trans] == 0 )
				goto _again;
				
				_acts = ( _buildargv_actions + (_buildargv_cond_actions[_trans]));
				_nacts = (unsigned int)(*( _acts));
				_acts += 1;
				while ( _nacts > 0 ) {
					switch ( (*( _acts)) )
					{
						case 2:  {
							{
								
								argv_s = p;
							}
							break; }
						case 3:  {
							{
								
								nargv = (char**)realloc((*argv), (argc_ + 1)*sizeof(char*));
								(*argv) = nargv;
								(*argv)[argc_] = strndup(argv_s, p - argv_s);
								argc_++;
							}
							break; }
						case 4:  {
							{
								{top -= 1;cs = stack[top];goto _again;} }
							break; }
						case 5:  {
							{
								{stack[top] = cs; top += 1;cs = 1; goto _again;}}
							break; }
						case 6:  {
							{
								{top -= 1;cs = stack[top];goto _again;} }
							break; }
						case 7:  {
							{
								{stack[top] = cs; top += 1;cs = 1; goto _again;}}
							break; }
						case 8:  {
							{
								{top -= 1;cs = stack[top];goto _again;} }
							break; }
						case 9:  {
							{
								{stack[top] = cs; top += 1;cs = 4; goto _again;}}
							break; }
						case 10:  {
							{
								{stack[top] = cs; top += 1;cs = 2; goto _again;}}
							break; }
						case 11:  {
							{
								{stack[top] = cs; top += 1;cs = 1; goto _again;}}
							break; }
					}
					_nacts -= 1;
					_acts += 1;
				}
				
				
			}
			_again:  {
				if ( cs == 0 )
				goto _out;
				p += 1;
				if ( p != pe )
				goto _resume;
			}
			_test_eof:  { {}
				if ( p == eof )
				{
					const char *__acts;
					unsigned int __nacts;
					__acts = ( _buildargv_actions + (_buildargv_eof_actions[cs]));
					__nacts = (unsigned int)(*( __acts)); __acts += 1;
					while ( __nacts > 0 ) {
						switch ( (*( __acts)) ) {
							case 0:  {
								{
									
									ret = -1;
									errsv = BUILDARGV_EDQUOTE;
								}
								break; }
							case 1:  {
								{
									
									ret = -1;
									errsv = BUILDARGV_ESQUOTE;
								}
								break; }
							case 3:  {
								{
									
									nargv = (char**)realloc((*argv), (argc_ + 1)*sizeof(char*));
									(*argv) = nargv;
									(*argv)[argc_] = strndup(argv_s, p - argv_s);
									argc_++;
								}
								break; }
						}
						__nacts -= 1;
						__acts += 1;
					}
				}
				
			}
			_out:  { {}
			}
		}
	}
	
	
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

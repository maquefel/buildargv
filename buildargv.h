#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#define BUILDARGV_EOK     0
#define BUILDARGV_EDQUOTE 1
#define BUILDARGV_ESQUOTE 2

int buildargv(char* input, char*** argv, int *argc);
#endif

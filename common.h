#ifndef CTEST1_COMMON_H
#define CTEST1_COMMON_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
void output(FILE* file, const char* restrict format, ...);
void dump_args_filename(char* filename, int argc, char *argv[]);
void dumpArgs(FILE *file, int argc, char **argv);
char* getTime();

#endif //CTEST1_COMMON_H

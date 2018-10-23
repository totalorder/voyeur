#include "common.h"

void dump_args_filename(char* filename, int argc, char *argv[]) {
    FILE* file;
    file = fopen(filename, "w");

    dumpArgs(file, argc, argv);

    fclose(file);
}

void dumpArgs(FILE *file, int argc, char **argv) {
    char *time = getTime();
    output(file, "Current time: %s\n", time);
    free(time);

    int i;
    for(i = 0; i < argc; i++) {
        output(file, "%s\n", argv[i]);
    }
}

char* getTime() {
    time_t rawTime;
    struct tm* timeInfo;
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    size_t bufferSize = 20 * sizeof(char);
    char* buffer = malloc(bufferSize);
    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", timeInfo);
    return buffer;
}

void output(FILE* file, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    vfprintf(file, format, args);
    va_end(args);
}
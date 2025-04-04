#ifndef GLOBALS_H
#define GLOBALS_H

#define MAX_PROCESSES 20

typedef struct {
    int pid;
    char user[32];
    float cpu;
    float mem;
    char etime[16];
    char command[256];
} Process;

extern Process processes[MAX_PROCESSES];
extern int selected;
extern int sort_mode;

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include "globals.h"

int compare_by_cpu(const void *a, const void *b) {
    float diff = ((Process*)b)->cpu - ((Process*)a)->cpu;
    return (diff > 0) - (diff < 0);
}

int compare_by_mem(const void *a, const void *b) {
    float diff = ((Process*)b)->mem - ((Process*)a)->mem;
    return (diff > 0) - (diff < 0);
}

int compare_by_pid(const void *a, const void *b) {
    return ((Process*)a)->pid - ((Process*)b)->pid;
}
void fetch_processes() {
    FILE *fp = popen("ps -eo user,pid,%cpu,%mem,etime,comm --sort=-%cpu", "r");
    if (!fp) return;

    char line[512];
    fgets(line, sizeof(line), fp);
    int count = 0;
    while (fgets(line, sizeof(line), fp) && count < MAX_PROCESSES) {
        sscanf(line, "%31s %d %f %f %15s %255[^\n]",
               processes[count].user,
               &processes[count].pid,
               &processes[count].cpu,
               &processes[count].mem,
               processes[count].etime,
               processes[count].command);
        count++;
    }
    pclose(fp);

    if (sort_mode == 0) qsort(processes, count, sizeof(Process), compare_by_mem);
    else if (sort_mode == 1) qsort(processes, count, sizeof(Process), compare_by_cpu);
    else qsort(processes, count, sizeof(Process), compare_by_pid);
}

void kill_selected_process() {
    if (processes[selected].pid < 1000) return;
    char cmd[64];
    sprintf(cmd, "kill -9 %d", processes[selected].pid);
    system(cmd);
}

void pause_process() {
    char cmd[64];
    sprintf(cmd, "kill -STOP %d", processes[selected].pid);
    system(cmd);
}

void resume_process() {
    char cmd[64];
    sprintf(cmd, "kill -CONT %d", processes[selected].pid);
    system(cmd);
}

void search_process() {
    char name[256];
    echo();
    mvprintw(MAX_PROCESSES + 8, 2, "Enter process name: ");
    getnstr(name, 255);
    noecho();
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (strstr(processes[i].command, name)) {
            selected = i;
            return;
        }
    }
    mvprintw(MAX_PROCESSES + 9, 2, "Process not found!");
    refresh();
    sleep(2);
}

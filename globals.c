#include "globals.h"

Process processes[MAX_PROCESSES];
int selected = 0;
int sort_mode = 1; // 0 = MEM, 1 = CPU, 2 = PID

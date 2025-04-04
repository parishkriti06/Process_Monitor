#include <ncurses.h>
#include <unistd.h>
#include "globals.h"
#include "process.h"

void init_colors() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);      // Default
    init_pair(2, COLOR_CYAN, COLOR_BLACK);       // Header
    init_pair(3, COLOR_GREEN, COLOR_BLACK);      // Normal rows
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);     // Selected row
}

void display_processes() {
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(2, 2, "%-8s %-10s %-6s %-6s %-7s %s", "PID", "USER", "%CPU", "%MEM", "ETIME", "COMMAND");
    attroff(COLOR_PAIR(2) | A_BOLD);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (i == selected) {
            attron(COLOR_PAIR(4) | A_BOLD);
        } else {
            attron(COLOR_PAIR(3));
        }

        mvprintw(i + 4, 2, "%-8d %-10s %-6.2f %-6.2f %-7s %s",
                 processes[i].pid,
                 processes[i].user,
                 processes[i].cpu,
                 processes[i].mem,
                 processes[i].etime,
                 processes[i].command);

        if (i == selected) {
            attroff(COLOR_PAIR(4) | A_BOLD);
        } else {
            attroff(COLOR_PAIR(3));
        }
    }

    attron(COLOR_PAIR(1));
    mvprintw(MAX_PROCESSES + 6, 2, "[↑/↓] Navigate  [ENTER] Kill  [p] Pause  [r] Resume  [/] Search");
    mvprintw(MAX_PROCESSES + 7, 2, "[s1] CPU  [s2] MEM  [s3] PID  [q] Quit");
    mvprintw(MAX_PROCESSES + 8, 2, "Sorting by: %s", sort_mode == 0 ? "MEM" : sort_mode == 1 ? "CPU" : "PID");
    attroff(COLOR_PAIR(1));
}

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    halfdelay(5);  // Waits up to 0.5 seconds for key press
    init_colors();

    while (1) {
        fetch_processes();

        // Prevent invalid selection index
        if (selected >= MAX_PROCESSES) selected = MAX_PROCESSES - 1;

        clear();
        display_processes();
        refresh();

        int ch = getch();
        switch (ch) {
            case ERR:
                continue;  // No key pressed
            case 'q':
                endwin();
                return 0;
            case KEY_UP:
                if (selected > 0) selected--;
                break;
            case KEY_DOWN:
                if (selected < MAX_PROCESSES - 1) selected++;
                break;
            case 10:  // Enter
                kill_selected_process();
                break;
            case 'p':
                pause_process();
                break;
            case 'r':
                resume_process();
                break;
            case '/':
                search_process();
                break;
            case 's': {
                int ch2 = getch();
                if (ch2 == '1') sort_mode = 1;
                else if (ch2 == '2') sort_mode = 0;
                else if (ch2 == '3') sort_mode = 2;
                break;
            }
        }
    }

    endwin();
    return 0;
}

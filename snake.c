#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "snake.h"

int maxY, maxX, speed, lastscore, highscore = 0;
char mode[11];

struct snake *s;
struct point *a;

int main()
{
    s = malloc(sizeof(struct snake)); // allocate memory for structs
    a = malloc(sizeof(struct point));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    nodelay(stdscr, TRUE); // init curses settings
    srand(time(NULL));     // seed time for random
    start_color(); // set color pairs for snake and apple
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    maxY = getmaxy(stdscr);
    maxX = getmaxx(stdscr);

    s->alive = TRUE;
    s->len = 1;
    s->dir = 3; // start going right
    s->segs[0].x = (int)maxX / 2;
    s->segs[0].y = (int)maxY / 2; // start snake in the middle
    a->x = rand() % (maxX - 2) + 1;
    a->y = rand() % (maxY - 2) + 1;

    attron(COLOR_PAIR(1));
    mvaddstr((int)maxY / 2 - 5, (int)maxX / 2 - 3, "Snake");
    attroff(COLOR_PAIR(1));
    mvaddstr((int)maxY / 2 + 5, (int)maxX / 2 - 13, "Press '?' to view controls");
    menu();

    while (s->alive)
    { // main game loop
        clear();
        int ch = getch();

        switch (ch)
        {
        case 'q':
            endwin();
            exit(0);
        case ' ':
        case 'p':
            mvaddstr((int)maxY / 2, (int)maxX / 2 - 15, "Paused. Press any key to resume");
        pause:
            if (getch() < 0)
                goto pause; // keep looping until a key is pressed
            printw("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            break;
        case 'w':
        case 'k':
        case KEY_UP:
            if (s->dir != 2 || s->len == 1)
            {
                s->dir = 0;
            }
            break;
        case 'a':
        case 'h':
        case KEY_LEFT:
            if (s->dir != 3 || s->len == 1)
            {
                s->dir = 1;
            }
            break;
        case 's':
        case 'j':
        case KEY_DOWN:
            if (s->dir != 0 || s->len == 1)
            {
                s->dir = 2;
            }
            break;
        case 'd':
        case 'l':
        case KEY_RIGHT:
            if (s->dir != 1 || s->len == 1)
            {
                s->dir = 3;
            }
            break;
        }

        if (s->segs[0].x == 1 || s->segs[0].x == maxX - 1 ||
            s->segs[0].y == 1 || s->segs[0].y == maxY - 1)
        {
            s->alive = FALSE;
        }
        for (int i = 1; i < s->len; i++)
        {
            if (s->segs[0].x == s->segs[i].x &&
                s->segs[0].y == s->segs[i].y)
            {
                s->alive = FALSE;
                break;
            }
        }

        draw();
        refresh();
        napms(speed);
    }

    lastscore = s->len - 1;
    highscore = lastscore > highscore ? lastscore : highscore;
    main();
}

void draw()
{
    box(stdscr, 0, 0);
    mvprintw(0, maxX - 10, "Score: %d", s->len - 1);
    mvprintw(0, 2, "Mode: %s", mode);

    struct point prev[100];
    memcpy(prev, s->segs, sizeof(s->segs));

    switch (s->dir)
    {
    case 0:
        s->segs[0].y--;
        break;
    case 1:
        s->segs[0].x--;
        break;
    case 2:
        s->segs[0].y++;
        break;
    case 3:
        s->segs[0].x++;
        break;
    }

    if (s->segs[0].x == a->x && s->segs[0].y == a->y)
    { // ate apple
        a->x = rand() % (maxX - 2) + 1;
        a->y = rand() % (maxY - 2) + 1;
        s->len++;
    }
    
    attron(COLOR_PAIR(1));
    for (int i = 1; i < s->len; i++)
    {
        s->segs[i] = prev[i - 1];
        mvaddch(s->segs[i].y, s->segs[i].x, '=');
    }

    mvaddch(s->segs[0].y, s->segs[0].x, '>');
    attron(COLOR_PAIR(2));
    mvaddch(a->y, a->x, 'a'); // draw apple
    attroff(COLOR_PAIR(2));
}

void menu()
{
    WINDOW *menu;
    int ch, i = 0;

    menu = newwin(7, 14, (int)maxY / 2 - 3, (int)maxX / 2 - 7); // mode menu window
    keypad(menu, TRUE);
    box(stdscr, 0, 0);
    box(menu, 0, 0);
    curs_set(0);

    mvprintw(0, 2, "Best: %d", highscore);
    mvprintw(0, maxX - 10, "Score: %d", lastscore);
    refresh();

    wattron(menu, A_STANDOUT);
    for (i = 0; i < 4; i++)
    {
        mvwprintw(menu, i + 1, 2, "%-10s", modes[i]);
        wattroff(menu, A_STANDOUT);
    }

    wrefresh(menu);

    i = 0;

    while ((ch = wgetch(menu)) != '\n' && ch != KEY_ENTER && ch != ' ')
    {
        mvwprintw(menu, i + 1, 2, "%-10s", modes[i]);
        switch (ch)
        {
        case 'q':
            endwin();
            exit(0);
        case 'w':
        case 'k':
        case KEY_UP:
            i--;
            i = (i < 0) ? 3 : i;
            break;
        case 's':
        case 'j':
        case KEY_DOWN:
            i++;
            i = (i > 3) ? 0 : i;
            break;
        case '?':
            mvprintw(1, 2, "Controls:");
            mvprintw(2, 4, "w or Up Arrow Key       - Move up");
            mvprintw(3, 4, "a or Left Arrow Key     - Move left");
            mvprintw(4, 4, "s or Down Arrow Key     - Move down");
            mvprintw(5, 4, "d or Right Arrow Key    - Move right");
            mvprintw(7, 2, "Game:");
            mvprintw(8, 4, "p or Space              - Pause");
            mvprintw(9, 4, "q                       - Quit");

            refresh();
        }

        wattron(menu, A_STANDOUT);
        mvwprintw(menu, i + 1, 2, "%-10s", modes[i]);
        wattroff(menu, A_STANDOUT);
    }
    speed = speeds[i];
    strcpy(mode, modes[i]);
    delwin(menu);
}

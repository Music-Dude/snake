#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "snake.h"

int maxY, maxX;
int speed;
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

    maxY = getmaxy(stdscr);
    maxX = getmaxx(stdscr);

    s->alive = TRUE;
    s->len = 1;
    s->dir = 'd'; // start going right
    s->segs[0].x = (int)maxX / 2;
    s->segs[0].y = (int)maxY / 2; // start snake in the middle
    a->x = rand() % (maxX - 2) + 1;
    a->y = rand() % (maxY - 2) + 1;

    mvaddstr((int)maxY / 2 - 5, (int)maxX / 2 - 3, "Snake");
    refresh();
    menu();

    while (s->alive)
    { // main game loop
        clear();
        int ch = getch();
        mvprintw(50, 50, "%c", ch);
        switch (ch)
        {
        case 'q':
            s->alive = FALSE;
            break;
        case ' ':
        case 'p':
            mvaddstr((int)maxY / 2, (int)maxX / 2 - 15, "Paused. Press any key to resume");
        pause:
            if (getch() < 0)
                goto pause; // keep looping until a key is pressed
            printw("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            break;

        case 'w':
        case KEY_UP:
        case 'a':
        case KEY_LEFT:
        case 's':
        case KEY_DOWN:
        case 'd':
        case KEY_RIGHT:
            s->dir = ch;
        }

        if (s->segs[0].x == 1 || s->segs[0].x == maxX - 1 ||
            s->segs[0].y == 1 || s->segs[0].y == maxY - 1)
        {
            s->alive = FALSE;
            break;
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

    endwin();
}

void draw()
{
    box(stdscr, 0, 0);
    mvprintw(0, maxX - 10, "Score: %d", s->len);

    struct point prev[100];
    memcpy(prev, s->segs, sizeof(s->segs));

    switch (s->dir)
    {
    case KEY_UP:
    case 'w':
        s->segs[0].y--;
        break;
    case KEY_LEFT:
    case 'a':
        s->segs[0].x--;
        break;
    case KEY_DOWN:
    case 's':
        s->segs[0].y++;
        break;
    case KEY_RIGHT:
    case 'd':
        s->segs[0].x++;
        break;
    }

    if (s->segs[0].x == a->x && s->segs[0].y == a->y)
    { // ate apple
        a->x = rand() % (maxX - 2) + 1;
        a->y = rand() % (maxY - 2) + 1;
        s->len++;
    }

    for (int i = 1; i < s->len; i++)
    {
        s->segs[i] = prev[i - 1];
        mvaddch(s->segs[i].y, s->segs[i].x, '=');
    }

    mvaddch(s->segs[0].y, s->segs[0].x, '>');
    mvaddstr(a->y, a->x, "a"); // draw apple
}

void menu()
{
    WINDOW *menu;
    char *options[4] = {"Easy", "Medium", "Difficult", "Expert"};
    char sel[11];
    int ch, i = 0;

    menu = newwin(7, 14, (int)maxY / 2 - 3, (int)maxX / 2 - 7); // mode menu window
    noecho();
    keypad(menu, TRUE);
    box(menu, 0, 0);
    curs_set(0);

    for (i = 0; i < 4; i++)
    {
        if (i == 0)
            wattron(menu, A_STANDOUT);
        else
            wattroff(menu, A_STANDOUT);
        sprintf(sel, "%-10s", options[i]);
        mvwprintw(menu, i + 1, 2, sel);
    }

    wrefresh(menu);

    i = 0;

    while ((ch = wgetch(menu)) != '\n' && ch != KEY_ENTER)
    {
        sprintf(sel, "%-10s", options[i]);
        mvwprintw(menu, i + 1, 2, "%s", sel);
        switch (ch)
        {
        case 'q':
            endwin();
            exit(0);
        case 'w':
        case KEY_UP:
            i--;
            i = (i < 0) ? 3 : i;
            break;
        case 's':
        case KEY_DOWN:
            i++;
            i = (i > 3) ? 0 : i;
            break;
        }

        wattron(menu, A_STANDOUT);
        sprintf(sel, "%-10s", options[i]);
        mvwprintw(menu, i + 1, 2, "%s", sel);
        wattroff(menu, A_STANDOUT);
    }
    speed = speeds[i];
    delwin(menu);
}
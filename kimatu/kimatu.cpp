#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <curses.h>
#include "game.h"

int main() {
    srand(time(NULL));

    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    Init();

    clock_t lastClock = clock();

    while (1) {
        clock_t nowClock = clock();
        if (nowClock >= lastClock + INTERVAL) {
            lastClock = nowClock;

            UpdateGame();

            DrawScreen();
        }

        int ch = getch();
        switch (ch) {
        case KEY_LEFT:
            player.x--;
            break;

        case KEY_RIGHT:
            player.x++;
            break;

        case KEY_UP:
            if (playerBullet.isFired)
                break;
            playerBullet.x = player.x;
            playerBullet.y = player.y - 1;
            playerBullet.isFired = true;
            break;
        }

        if (player.x < 0)
            player.x = 0;
        if (player.x >= SCREEN_WIDTH)
            player.x = SCREEN_WIDTH - 1;

        if (starBulletIntersectPlayer()) {
            Init();
            continue;
        }

        DrawScreen();
    }

    endwin();
    return 0;
}

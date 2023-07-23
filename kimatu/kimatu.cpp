#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <curses.h>
#include "game.h"

int main() {

    get_iniDirectory(ini_playername);

    readCSV(result);

    //初期化設定
    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    DrawStartScreen(ini_playername, result);

    int ch1;
    int ch3;
    while (1) {
        ch1 = getch();
        if (ch1 == ' ')//スペースキーが押されたらループを抜ける
            break;
    }

    Init();//ゲームの初期化

    clock_t lastClock = clock();

    while (1) {
        clock_t nowClock = clock();

        if (nowClock >= lastClock + INTERVAL) {//球と敵の移動時間に差を生ませる
            lastClock = nowClock;

            UpdateGame();

            DrawScreen();

        }

        int ch2 = getch();
        switch (ch2) {
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

        if (TimerExpired(&timer))
            break;

        DrawScreen();
    }

    
    while (1) {
        DrawEndScreen(ini_playername, result);
        ch3 = getch();
        if (ch3 == ' ') {
            break;
        }
    }
    compareScores(result);

    writeCSV(result);

    endwin();
    
    return 0;
}

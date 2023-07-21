#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <curses.h>
#include "game.h"

const char* tileAA[TILE_MAX] = {
    " ",    // TILE_NONE
    "*",    // TILE_STAR
    "^",    // TILE_PLAYER
    "|",    // TILE_PLAYER_BULLET
    ".",    // TILE_STAR_BULLET
};

VEC2 directions[] = {
    {1, 0},     // DIRECTION_RIGHT
    {0, 1},     // DIRECTION_DOWN
    {-1, 0},    // DIRECTION_LEFT
};


int screen[SCREEN_HEIGHT][SCREEN_WIDTH];
STAR stars[STAR_ROW][STAR_COLUMN];
PLAYER player;
PLAYER_BULLET playerBullet;
STAR_BULLET starBullet[STAR_COLUMN];
int starDirection;
int starCount;
Timer timer;//ゲーム時間のタイマー
int score = 0;

/*void get_iniDirectoriy(char* ini_filename1, char* ini_filename2, char* ini_filename3) {
    char section[CHARBUFF];
    char keyword1[CHARBUFF];
    char keyword2[CHARBUFF];
    char keyword3[CHARBUFF];
    char currentDirectory[CHARBUFF];
    GetCurrentDirectory(CHARBUFF, currentDirectory);

    sprintf_s(section, "section1");
    sprintf_s(keyword1, "keyword1");
    sprintf_s(keyword2, "keyword2");
    sprintf_s(keyword3, "keyword3");
    char settingfile[CHARBUFF];
    sprintf_s(settingfile, "%s\\setting.ini", currentDirectory);

    GetPrivateProfileString(section, keyword1, "none", ini_filename1, CHARBUFF, settingfile);
    GetPrivateProfileString(section, keyword2, "none", ini_filename2, CHARBUFF, settingfile);
    GetPrivateProfileString(section, keyword3, "none", ini_filename3, CHARBUFF, settingfile);
}

void reeadCSV(const char* filename, int score[10]) {
    FILE* fp;
    char line[BUFFSIZE];
    char* token;
    char* next_token;
    int row = 0, column = 0; //行, 列

    errno_t error;

    error = fopen_s(&fp, filename, "r");
    if (error != 0) {
        fprintf_s(stderr, "failed to open\n");
    }
    else {
        //取得した文字列がNULLではないかつ列が5を超えるまで繰り返す(配列は0から5番目まで)
        while (fgets(line, BUFFSIZE, fp) != NULL && row < RANKING + 1) {//fgetsはfpの中身をlineに文字列のアドレスとして返す

            column = 0;
            if (row != 0) {
                //strtok(分割する文字列, 区切り文字) 区切った最初の要素をchar型で返す
                token = strtok_s(line, ",", &next_token);//next_tokenには区切った次の配列のポインタが入る

                //strtokは分割が終了するとNULLを返すのでNULLになるまでかつ列が2未満の場合繰り返す
                while (token != NULL && column < RESULTS + 1) {

                    if (column != 0) {
                        data[row - 1][column - 1] = strtod(token, NULL);// tokenにある文字列をdouble型に変換して格納
                    }

                    //strtokの第1引数にNULLを指定すると前回の呼び出しアドレスから始まる
                    token = strtok_s(NULL, ",", &next_token);//NULLの部分が前のnext_tokenのポインタを自然と参照してくれる


                    column++;
                }
            }
            row++;
        }

        fclose(fp); // ファイルを閉じる
    }
}*/


void DrawStartScreen() {
    clear();
    mvprintw(SCREEN_HEIGHT / 2 - 2, (SCREEN_WIDTH - 20) / 2, "Shooting Game");
    mvprintw(SCREEN_HEIGHT / 2, (SCREEN_WIDTH - 17) / 2, "Press sny key");
    refresh();
}

//ゲーム画面をターミナルに描画
void DrawScreen() {
    clear();
    for (int y = 0; y < STAR_ROW; y++) {//星の描画
        for (int x = 0; x < STAR_COLUMN; x++) {
            if (!stars[y][x].isHit)
                mvaddstr(stars[y][x].y, stars[y][x].x, tileAA[TILE_STAR]);
        }
    }
    mvaddstr(player.y, player.x, tileAA[TILE_PLAYER]);//プレイヤーの描画

    if (playerBullet.isFired)
        mvaddstr(playerBullet.y, playerBullet.x, tileAA[TILE_PLAYER_BULLET]);

    for (int x = 0; x < STAR_COLUMN; x++) {
        if (starBullet[x].isFired)
            mvaddstr(starBullet[x].y, starBullet[x].x, tileAA[TILE_STAR_BULLET]);
    }

    mvprintw(LINES - 1, 0, "Score: %d", score);
    mvprintw(LINES - 1, COLS - 10, "Time: %ld", GAMETIME - (timer.current - timer.start));
    refresh();
}

void Init() {//ゲーム初期化関数
    memset(screen, 0, sizeof(screen));

    for (int y = 0; y < STAR_ROW; y++) {
        for (int x = 0; x < STAR_COLUMN; x++) {
            stars[y][x].x = x * 2;
            stars[y][x].y = y * 2;
            stars[y][x].isHit = false;
        }
    }

    starDirection = DIRECTION_RIGHT;

    //星の移動速度と球の更新速度に差を生じさせる
    starCount = 0;

    //プレイヤーの始まる場所
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT / 2;

    //球は両者とも発射されていない
    playerBullet.isFired = false;

    for (int x = 0; x < STAR_COLUMN; x++)
        starBullet[x].isFired = false;

    DrawScreen();
}

bool playerBulletIntersectStars() {
    for (int y = 0; y < STAR_ROW; y++) {
        for (int x = 0; x < STAR_COLUMN; x++) {
            if ((!stars[y][x].isHit) && (stars[y][x].x == playerBullet.x) && (stars[y][x].y == playerBullet.y)) {
                stars[y][x].isHit = true;
                playerBullet.isFired = false;
                return true;
            }
        }
    }
    return false;
}

bool starBulletIntersectPlayer() {
    for (int x = 0; x < STAR_COLUMN; x++) {
        if ((starBullet->isFired) && (starBullet[x].x == player.x) && (starBullet[x].y == player.y)) {
            return true;
        }
    }
    return false;
}

void UpdateGame() {

    if (playerBullet.isFired) {
        playerBullet.y--;
        if (playerBullet.y < 0)
            playerBullet.isFired = false;
        playerBulletIntersectStars();
    }

    
    starCount++;
    if (starCount > 5) {
        starCount = 0;
        int nextDirection = starDirection;

        for (int y = 0; y < STAR_ROW; y++) {
            for (int x = 0; x < STAR_COLUMN; x++) {
                if (stars[y][x].isHit)
                    continue;

                stars[y][x].x += directions[starDirection].x;
                stars[y][x].y += directions[starDirection].y;

                switch (starDirection) {
                case DIRECTION_RIGHT:
                    if (stars[y][x].x >= SCREEN_WIDTH - 1)
                        nextDirection = DIRECTION_DOWN;
                    break;

                case DIRECTION_DOWN:
                    if (stars[y][x].x >= SCREEN_WIDTH - 1)
                        nextDirection = DIRECTION_LEFT;
                    if (stars[y][x].x <= 0)
                        nextDirection = DIRECTION_RIGHT;
                    break;

                case DIRECTION_LEFT:
                    if (stars[y][x].x <= 0)
                        nextDirection = DIRECTION_DOWN;
                    break;
                }
            }
        }

        starDirection = nextDirection;

        for (int x = 0; x < STAR_COLUMN; x++) {
            if (!starBullet[x].isFired) {
                if (rand() % STAR_COLUMN)
                    continue;
                starBullet[x].isFired = true;

                int starrow = -1;
                for (int y = 0; y < STAR_ROW; y++) {
                    if (!stars[y][x].isHit)
                        starrow = y;
                }

                if (starrow > 0) {
                    starBullet[x].isFired = true;
                    starBullet[x].x = stars[starrow][x].x;
                    starBullet[x].y = stars[starrow][x].y + 1;
                }
            }
        }
    }

    for (int x = 0; x < STAR_COLUMN; x++) {
        if (starBullet[x].isFired) {
            starBullet[x].y++;
            if (starBullet[x].y >= SCREEN_HEIGHT)
                starBullet[x].isFired = false;
        }
    }

    if (starBulletIntersectPlayer()) {
        Init();
        return;
    }
}

void updateTimer(struct Timer* timer) {
    timer->current = time(NULL);
}

int TimerExpired(struct Timer* timer) {
    return timer->current - timer->start >= GAMETIME;
}
#pragma once

#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 30

#define FPS 20
#define INTERVAL (1000 / FPS)

#define STAR_COLUMN 11
#define STAR_ROW 5

#define CHARBUFF 124
#define BUFFSIZE 1024

#define RANKING 5
#define RESULTS 2

#define GAMETIME 20
enum {
    TILE_NONE,          // ��ʂɉ�������
    TILE_STAR,
    TILE_PLAYER,
    TILE_PLAYER_BULLET,
    TILE_STAR_BULLET,
    TILE_MAX
};

enum {//���̈ړ�����
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_MAX,
};

typedef struct {
    int x, y;
} VEC2;

typedef struct {
    int x, y;
    bool isHit;
} STAR;

typedef struct {
    int x, y;
} PLAYER;

typedef struct {
    int x, y;
    bool isFired;//���˂���Ă��邩�ǂ���
} PLAYER_BULLET;

typedef struct {
    int x, y;
    bool isFired;
} STAR_BULLET;

struct RESULT{
    char playername[BUFFSIZE];
    int playerscore;
};

struct Timer {
    time_t start;
    time_t current;
};


//�\���́A�ϐ��錾 �Ăяo����game.c��2�d��`�ƂȂ�Ȃ��ׂ�extern���g��
extern int screen[SCREEN_HEIGHT][SCREEN_WIDTH];
extern STAR stars[STAR_ROW][STAR_COLUMN];
extern PLAYER player;
extern PLAYER_BULLET playerBullet;
extern STAR_BULLET starBullet[STAR_COLUMN];
extern int starDirection;//���̐i�s���x
extern int starCount;//���̈ړ����x�����ƕω��������
extern RESULT result[5];
extern Timer timer;
extern char ini_playername[BUFFSIZE];
extern int score;

void get_iniDirectory(char* ini_playername);
void readCSV(struct RESULT result[]);
void compareScores(struct RESULT result[]);
void writeCSV(struct RESULT result[]);
void DrawStartScreen(char *ini_playername, struct RESULT result[]);
void DrawEndScreen(char* ini_playername, struct RESULT result[]);
void Init();
void DrawScreen();
void UpdateGame();
bool playerBulletIntersectStars();
bool starBulletIntersectPlayer();
void updateTimer(struct Timer* timer);
int TimerExpired(struct Timer* timer);


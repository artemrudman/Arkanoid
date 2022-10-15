// ARKANOID

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <math.h>

#define width 95
#define height 25
M_PI = 3.14159265358979323846;
#define c_border '#'
#define c_racket '@'
#define c_brick (char)176
#define brickwidth 8

typedef struct {
    int x, y;
    int w;
} TRacket;

typedef struct {
    float x, y;
    int ix, iy;
    float alfa;
    float speed;
} TBall;


char lvlMap[height][width];
char mas[height][width + 1];
TRacket racket;
TBall ball;
int hitCnt = 0;
int maxHitCnt = 0;
int lvl = 1;
boolean run = FALSE;

void setcur(int x, int y);

void initBall()
{
    moveBall(2, 2);
    ball.alfa = -1;
    ball.speed = 0.5;
}

void putBall()
{
    mas[ball.iy][ball.ix] = '*';
}

int moveBall(float x, float y)
{
    ball.x = x;
    ball.y = y;
    ball.ix = (int)round(ball.x);
    ball.iy = (int)round(ball.y);
}

void autoMoveBall()
{
    if (ball.alfa < 0) ball.alfa += M_PI * 2;
    if (ball.alfa > M_PI * 2) ball.alfa -= M_PI * 2;

    TBall bl = ball;
    moveBall(ball.x + cos(ball.alfa) * ball.speed
        , ball.y + sin(ball.alfa) * ball.speed);

    if ((mas[ball.iy][ball.ix] == c_border) ||
        (mas[ball.iy][ball.ix] == c_brick) ||
        (mas[ball.iy][ball.ix] == c_racket))
    {
        if (mas[ball.iy][ball.ix] == c_racket)
            hitCnt++;

        if (mas[ball.iy][ball.ix] == c_brick)
        {
            int brickNom = (ball.ix-1)/brickwidth;
            int dx = 1 + brickNom * brickwidth;
            for (int i = 0; i < brickwidth; i++)
            {
                static char* c;
                c = &lvlMap[ball.iy][i + dx];
                if (*c == c_brick) *c = ' ';
            }
        }

        if ((ball.ix != bl.ix) && (ball.iy != bl.iy))
        {
            if (mas[bl.iy][ball.ix] == mas[ball.iy][bl.ix])
                bl.alfa = bl.alfa + M_PI;
            else
            {
                if (mas[bl.iy][ball.ix] == c_border)
                    bl.alfa = (2 * M_PI - bl.alfa) + M_PI;
                else
                    bl.alfa = (2 * M_PI - bl.alfa);
            }
        }
        else if (ball.iy == bl.iy)
            bl.alfa = (2 * M_PI - bl.alfa) + M_PI;
        else
            bl.alfa = (2 * M_PI - bl.alfa);

        ball = bl;
    }
}

void initRacket()
{
    racket.w = 10;
    racket.x = (width - racket.w) / 2;
    racket.y = height - 1;
}

void putRacket()
{
    for (int i = racket.x; i < racket.x + racket.w; i++)
        mas[racket.y][i] = c_racket;
}

void lvlMapPuzzile(int lvl)
{
    if (lvl == 1)
    {
        for (int i = 17; i < width - 15; i++)
            lvlMap[5][i] = lvlMap[6][i] = c_brick;
    }

    if (lvl == 2)
        for (int i = 25; i < 68; i++) {

            lvlMap[1][i] = lvlMap[2][i] = c_brick;
            lvlMap[8][i] = lvlMap[9][i] = c_brick;
            lvlMap[10][i] = c_border;
        }

    if (lvl == 3)

        for (int i = 19; i < 78; i++) {

            lvlMap[1][i] = lvlMap[2][i] = c_brick;
            lvlMap[5][i] = lvlMap[6][i] = c_brick;
            lvlMap[9][i] = lvlMap[10][i] = c_brick;
            lvlMap[11][i] = c_border;
        }
}
void lvlMapInit(int lvl)
{
    memset(lvlMap, ' ', sizeof(lvlMap));

    lvlMapPuzzile(lvl);

    for (int i = 0; i < width; i++)
        lvlMap[0][i] = c_border;
    for (int j = 0; j < height; j++)
        lvlMap[j][0] = lvlMap[j][width-1] = c_border;
}

void lvlMapPut()
{
    memset(mas, 0, sizeof(mas));
    for (int j = 0; j < height; j++)
        memcpy(mas[j], lvlMap[j], sizeof(**lvlMap) * width);
}

void show()
{
    setcur(0, 0);
    for (int i = 0; i < height; i++)
    {
        printf("%s", mas[i]);
        if (i == 1)
            printf("   lvl %i   ", lvl);
        if (i == 3)
            printf("   hit %i   ", hitCnt);
        if (i == 4)
            printf("   max %i   ", maxHitCnt);
        if (i < height - 1)
            printf("\n");
    }
}

void moveRacket(int x)
{
    racket.x = x;
    if (racket.x < 1)
        racket.x = 1;
    if (racket.x + racket.w >= width)
        racket.x = width - 1 - racket.w;
}

void setcur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ShowPreview()
{
    system("cls");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t    LEVEL %d", lvl);
    Sleep(1000);
    system("cls");
}

void checkFailed() 
{
    if (ball.iy >= height-1)
    {
        run = FALSE;
        if (hitCnt > maxHitCnt)
            maxHitCnt = hitCnt;
        hitCnt = 0;
    }
}

lvlMapCheckCount()
{
    int cnt = 0;
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
            if (lvlMap[j][i] == c_brick)
                cnt++;
    return cnt;
}

void checkWin()
{
    if (lvlMapCheckCount() == 0)
    {
        lvl++;
        if (lvl > 3) lvl = 1;
        lvlMapInit(lvl);
        run = FALSE;
        maxHitCnt = 0;
        hitCnt = 0;
        ShowPreview();
    }
}

void ballWork()
{
    if (run)
        autoMoveBall();
    else
        moveBall(racket.x + racket.w / 2, racket.y - 1);
}

int main()
{
    initRacket();
    initBall();
    lvlMapInit(lvl);
    ShowPreview();

    do
    {
        ballWork();
        checkFailed();
        checkWin();

        lvlMapPut();
        putRacket();
        putBall();
        show();
        if (GetKeyState('A') < 0) moveRacket(racket.x - 1);
        if (GetKeyState('D') < 0) moveRacket(racket.x + 1);
        if (GetKeyState('W') < 0) run = TRUE;
            
        Sleep(8);
    } while (GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}

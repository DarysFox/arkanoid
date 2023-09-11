#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <cmath>

//parametry hry
#define width 65
#define height 25
typedef struct{
    int x,y;
    int w;
} TRacket;

typedef struct{
    float x, y;
    int ix, iy;
    float alfa;
    float speed;
}TBall;

char mas[height][width+1];
TRacket racket;
TBall ball;
int hitCnt = 0;
int maxHitCnt = 0;

void initBall()
{
    ball.x = 2;
    ball.y = 2;
    ball.alfa = -1;
    ball.speed = 0.5;
}

void putBall()
{
    mas[ball.iy][ball.ix] = '*';
}

void moveBall(float x, float y)
{
    ball.x = x;
    ball.y = y;
    ball.ix = (int)round(ball.x);
    ball.iy = (int)round(ball.y);
}

void autoMoveBall()
{
    if(ball.alfa < 0) ball.alfa += M_PI*2;
    if(ball.alfa > M_PI*2) ball.alfa -= M_PI*2;

    TBall bl = ball;
    moveBall(ball.x + cos(ball.alfa) * ball.speed,
            ball.y + sin(ball.alfa) * ball.speed);

    if((mas[ball.iy][ball.ix] == '#') || (mas[ball.iy][ball.ix] == '@'))
    {
        if(mas[ball.iy][ball.ix] == '@')
            hitCnt++;

        if((ball.ix != bl.ix) && (ball.iy != bl.iy))
        {
                if(mas[bl.iy][bl.ix] == mas[ball.iy][bl.ix])
                    bl.alfa = bl.alfa + M_PI;
                else
                {
                    if(mas[bl.iy][ball.ix] == '#')
                        bl.alfa = (2*M_PI - bl.alfa) + M_PI;
                    else
                        bl.alfa = (2*M_PI - bl.alfa);
                }
        }
        else if (ball.iy == bl.iy)
            bl.alfa = (2*M_PI - bl.alfa) + M_PI;
        else
            bl.alfa = (2*M_PI - bl.alfa);

        ball = bl;
        autoMoveBall();
    }
}

void initRacket()
{
    racket.w = 7;
    racket.x = (width - racket.w) / 2;
    racket.y = height - 1;
}

void putRacket()
{
    for(int i=racket.x; i<racket.x + racket.w; i++)
        mas[racket.y][i] = '@';
}

//tvar pole
void init()
{
    if(hitCnt<= 5)
    {
        for(int i=0; i < width; i++)
            mas[0][i] = '#';
            mas[0][width] = '\0';

        strncpy(mas[1], mas[0], width+1);
        for(int i=1; i<width-1; i++)
            mas[1][i] = ' ';

        for(int i=2; i<height; i++)
            strncpy(mas[i], mas[1], width+1);
    }

    else if(hitCnt > 5)
    {
        for(int i=0; i < width; i++)
            mas[0][i] = '#';
        mas[0][width] = '\0';

        strncpy(mas[1], mas[0], width+1);
        for(int i=1; i<width-1; i++)
            mas[1][i] = ' ';

        for(int i=2; i<height; i++)
            strncpy(mas[i], mas[1], width+1);

        for(int i=20; i<50; i++)
            mas[10][i] = '#';
    }
}

//output udaje
void show()
{
    for(int i=0; i<height; i++)
    {
        printf("%s", mas[i]);

        if(i == 3)
            if(hitCnt < 5)
                printf("  level 1 ");
            else if(hitCnt > 5)
                printf("  level 2 ");

        if(i == 4)
            printf("  hit %i  ", hitCnt);
        if(i == 5)
            printf("  max %i  ", maxHitCnt);
        if(i < height - 1)
            printf("\n");
    }
}

int colour(int col)
{
        printf("  black x green-1 \n  green x yellow-2 \n  red x black-3 \n  yellow x pink-5 \n  default - black x white \n" );
        printf("  choose color:  ");
        scanf(" %d", &col);

        if(col == 1)
            system("Color 0A");

        else if(col == 2)
            system("Color AE");

        else if(col == 3)
            system("Color 04");

        else if(col == 4)
            system("Color 9F");

        else if(col == 5)
            system("Color ED");
    return col;
}


void moveRacket(int x)
{
    racket.x = x;
    if(racket.x < 1)
        racket.x = 1;
    if(racket.x + racket.w >= width)
        racket.x = width - 1 - racket.w;
}

void setcur(float x, float y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
    int col;

    colour(col);

    char c;

    BOOL run = FALSE;
    initRacket();
    initBall();

    do
    {
        setcur(0,0);

        if(run)
            autoMoveBall();
        if(ball.iy > height)
        {
           run = FALSE;
           if(hitCnt > maxHitCnt)
            maxHitCnt = hitCnt;

        hitCnt = 0;
        }

        init();
        putRacket();
        putBall();
        show();
        if(GetKeyState('A') < 0)moveRacket(racket.x - 1);
        if(GetKeyState('D') < 0)moveRacket(racket.x + 1);
        if(GetKeyState('W') < 0) run = TRUE;
        if(!run)
            moveBall(racket.x + racket.w / 2, racket.y - 1);
        Sleep(1);
    }
    while(GetKeyState(VK_ESCAPE) >= 0);

    return 0;
}

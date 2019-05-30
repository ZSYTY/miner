#include "game.h"
#include "utils.h"
#include "random.h"
#include "graphics.h"
#include "extgraph.h"

#include <math.h>
#include <windows.h>

static int score;
static int state;
#define WAITING 0
#define DOWN 1
#define UP 2
static linkHead linkGold;
static gold *got;
extern double width, height;
#define boardRatio (4.0 / 5)

int checkIntersect(gold aGold)
{
    linkHead p = linkGold;
    double l1, r1, l2, r2, u1, d1, u2, d2;
    l1 = aGold.x, d1 = aGold.y;
    if (aGold.type == SMALL || aGold.type == GEM)
        r1 = aGold.x + width / 60, u1 = aGold.y + height / 60;
    else if (aGold.type == MEDIUM || aGold.type == STONE)
        r1 = aGold.x + width / 25, u1 = aGold.y + height / 25;
    else
        r1 = aGold.x + width / 10, u1 = aGold.y + height / 10;
    while (p != NULL)
    {
        gold *cur = p->data;
        l2 = cur->x, d2 = cur->y;
        if (cur->type == SMALL || aGold.type == GEM)
            r2 = cur->x + width / 60, u2 = cur->y + height / 60;
        else if (cur->type == MEDIUM || aGold.type == STONE)
            r2 = cur->x + width / 25, u2 = cur->y + height / 25;
        else
            r2 = cur->x + width / 10, u2 = cur->y + height / 10;
        if (min(r1, r2) > max(l1, l2) || min(u1, u2) > max(d1, d2))
            return 0;
        p = p->next;
    }
    return 1;
}

void clearGold()
{
    linkGold = NULL;
    score = 0;
}

void generateGold(int type)
{
    linkHead p = linkGold;
    gold *aGold = malloc(sizeof(gold));
    aGold->type = type;
    if (aGold->type == SMALL || aGold->type == GEM)
    {
        do
        {
            aGold->x = RandomReal(0, width * 59 / 60);
            aGold->y = RandomReal(0, height * 5 / 7 * 59 / 60);
        } while (!checkIntersect(*aGold));
    }
    else if (aGold->type == MEDIUM || aGold->type == STONE)
    {
        do
        {
            aGold->x = RandomReal(0, width * 24 / 25);
            aGold->y = RandomReal(0, height * 5 / 7 * 24 / 25);
        } while (!checkIntersect(*aGold));
    }
    else
    {
        do
        {
            aGold->x = RandomReal(0, width * 9 / 10);
            aGold->y = RandomReal(0, height * 5 / 7 * 9 / 10);
        } while (!checkIntersect(*aGold));
    }
    linkGold = insNode(linkGold, aGold);
}

void drawGold()
{
    SetPenColor("Yellow");
    linkHead p = linkGold;
    while (p != NULL)
    {
        gold *cur = p->data;
        if (cur->type == SMALL)
        {
            drawFilledRect(cur->x, cur->y, width / 60, height / 60);
        }
        else if (cur->type == MEDIUM)
        {
            drawFilledRect(cur->x, cur->y, width / 25, height / 25);
        }
        else if (cur->type == LARGE)
        {
            drawFilledRect(cur->x, cur->y, width / 10, height / 10);
        }
        else if (cur->type == STONE)
        {
            SetPenColor("Gray");
            drawFilledRect(cur->x, cur->y, width / 25, height / 25);
        }
        else if (cur->type == GEM)
        {
            SetPenColor("Blue");
            drawFilledRect(cur->x, cur->y, width / 60, height / 60);
        }
        p = p->next;
    }
}

gold *checkMeet(double x, double y)
{
    linkHead p = linkGold;
    gold *cur = NULL;
    while (p != NULL)
    {
        cur = p->data;
        if ((cur->type == SMALL || cur->type == GEM) && x >= cur->x && x <= cur->x + width / 60 && y >= cur->y && y <= cur->y + height / 60)
            return cur;
        else if ((cur->type == MEDIUM || cur->type == STONE) && x >= cur->x && x <= cur->x + width / 25 && y >= cur->y && y <= cur->y + height / 25)
            return cur;
        else if (cur->type == LARGE && x >= cur->x && x <= cur->x + width / 10 && y >= cur->y && y <= cur->y + height / 10)
            return cur;
        p = p->next;
    }
    return NULL;
}

void displayBoard()
{
    double width = GetWindowWidth();
    double height = GetWindowHeight();

    SetPenColor("Black");
    MovePen(0, height * boardRatio);
    DrawLine(width, 0);

    double minerWidth = 0.8;
    double minerHeight = 0.8;

    drawRectangle(width / 2 - minerWidth / 2, height * boardRatio, minerWidth, minerHeight);

    // generateGold();
    // drawGold();
}

void displayState()
{
    SetPenColor("Black");
    SetEraseMode(TRUE);
    static char stateText[MAX_TEXT_LENGTH + 1];
    sprintf(stateText, "当前分数：%d", score);
    MovePen(0, height * 5 / 6);
    DrawTextString(stateText);
    SetEraseMode(FALSE);
    DrawTextString(stateText);
}

void displayMap()
{
    int i, j, counter[5];
    counter[SMALL] = RandomInteger(2, 3);
    counter[MEDIUM] = RandomInteger(1, 2);
    counter[LARGE] = RandomInteger(1, 2);
    counter[STONE] = RandomInteger(1, 2);
    counter[GEM] = RandomInteger(1, 2);
    for (i = 4; i >= 0; i--)
        for (j = 0; j < counter[i]; j++)
            generateGold(i);
    drawGold();
}

#define defaultTimer 1
#define pi 3.14159265
#define cLength 0.3

void moniter(int timerID)
{
    static double theta = pi;
    static double dTheta = 0.01;
    static double dR = 0.05;
    static double centerX;
    static double centerY;    
    if (timerID == defaultTimer)
    {
        double dx, dy;
        switch (state)
        {
        case WAITING:
            centerX = width * .5;
            centerY = height * boardRatio;
            SetPenColor("Black");
            SetEraseMode(TRUE);
            drawVector(centerX, centerY, cLength, theta - pi / 8);
            drawVector(centerX, centerY, cLength, theta + pi / 8);
            dTheta -= 0.001 * cos(theta);
            theta += dTheta;
            SetEraseMode(FALSE);
            drawVector(centerX, centerY, cLength, theta - pi / 8);
            drawVector(centerX, centerY, cLength, theta + pi / 8);
            displayBoard();
            break;

        case DOWN:
            SetPenColor("Black");
            SetEraseMode(TRUE);
            drawVector(centerX, centerY, cLength, theta - pi / 8);
            drawVector(centerX, centerY, cLength, theta + pi / 8);
            dx = dR * cos(theta), dy = dR * sin(theta);
            MovePen(centerX, centerY);
            centerX += dx;
            centerY += dy;
            SetEraseMode(FALSE);
            DrawLine(dx, dy);
            drawVector(centerX, centerY, cLength, theta - pi / 8);
            drawVector(centerX, centerY, cLength, theta + pi / 8);
            got = checkMeet(centerX, centerY);
            if (got != NULL)
            {
                state = UP;
            }
            if (centerX <= 0 || centerX >= width || centerY <= 0)
            {
                state = UP;
            }
            break;

        case UP:
            SetPenColor("Black");
            SetEraseMode(TRUE);
            drawVector(centerX, centerY, cLength, theta - pi / 8);
            drawVector(centerX, centerY, cLength, theta + pi / 8);
            dx = dR * cos(theta), dy = dR * sin(theta);
            MovePen(centerX, centerY);
            DrawLine(dx, dy);
            centerX -= dx;
            centerY -= dy;
            if (got != NULL)
                drawGold();
            SetEraseMode(FALSE);
            drawVector(centerX, centerY, cLength, theta - pi / 8);
            drawVector(centerX, centerY, cLength, theta + pi / 8);            
            if (got != NULL)
            {
                linkHead p = linkGold;
                static gold *cur = NULL;
                while (p != NULL)
                {
                    cur = p->data;
                    if (cur->x == got->x && cur->y == got->y)
                    {
                        cur->x -= dx;
                        cur->y -= dy;
                        break;
                    }
                    p = p->next;
                }
                drawGold();
            }
            if (centerY >= height * boardRatio)
            {
                if (got != NULL)
                {
                    if (got->type == SMALL)
                    score += 50;
                    else if (got->type == MEDIUM)
                        score += 100;
                    else if (got->type == LARGE)
                        score += 500;
                    else if (got->type == STONE)
                        score += 20;
                    else if (got->type == GEM)
                        score += 600;
                    linkHead p = linkGold;
                    static gold *cur = NULL;
                    while (p != NULL)
                    {
                        cur = p->data;
                        if (cur->x == got->x && cur->y == got->y)
                        {
                            delNode(linkGold, p);
                            break;
                        }
                        p = p->next;
                    }
                    got = NULL;
                    clearScreen();
                    displayBoard();
                    drawGold();
                    displayState();
                }
                state = WAITING;
            }
            break;
        }
    }
}

void handler(int key, int event)
{
    if (key == VK_DOWN && event == KEY_DOWN && state == WAITING)
    {
        state = DOWN;
    }
}

void initGame()
{
//    InitConsole();
    Randomize();
    clearScreen();
    clearGold();
    displayBoard();
    displayState();
    displayMap();

    registerKeyboardEvent(&handler);
    registerTimerEvent(&moniter);
    startTimer(defaultTimer, 20);
}

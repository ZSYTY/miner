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
static linkNode *got;
extern double width, height;
#define boardRatio (4.0 / 5)
static int scoreMap[GEM + 1] = {50, 100, 500, 20, 600};
static int ratioMap[GEM + 1] = {60, 25, 10, 25, 60};
static char colorMap[GEM + 1][10] = {"Yellow", "Yellow", "Yellow", "Gray", "Blue"};

int checkIntersect(gold aGold)
{
    linkHead p = linkGold;
    double l1, r1, l2, r2, u1, d1, u2, d2;
    l1 = aGold.x, d1 = aGold.y;
    r1 = aGold.x + width / ratioMap[aGold.type];
    u1 = aGold.y + height / ratioMap[aGold.type];
    while (p != NULL)
    {
        gold *cur = p->data;
        l2 = cur->x, d2 = cur->y;
        r2 = cur->x + width / ratioMap[cur->type];
        u2 = cur->y + height / ratioMap[cur->type];
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
    double boundRatio = (ratioMap[type] - 1.0) / ratioMap[type];
    do
    {
        aGold->x = RandomReal(0, width * boundRatio);
        aGold->y = RandomReal(0, height * boundRatio * 5 / 7);
    } while (!checkIntersect(*aGold));
    linkGold = insNode(linkGold, aGold);
}

void drawGold()
{
    linkHead p = linkGold;
    while (p != NULL)
    {
        gold *cur = p->data;
        SetPenColor(colorMap[cur->type]);
        drawFilledRect(cur->x, cur->y, width / ratioMap[cur->type], height / ratioMap[cur->type]);
        p = p->next;
    }
}

linkNode *checkMeet(double x, double y)
{
    linkNode *p = linkGold;
    gold *cur = NULL;
    while (p != NULL)
    {
        cur = p->data;
        if (x >= cur->x && x <= cur->x + width / ratioMap[cur->type] && y >= cur->y && y <= cur->y + height / ratioMap[cur->type])
            return p;
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

void refresh()
{
    clearScreen();
    displayBoard();
    drawGold();
    displayState();
}

#define defaultTimer 1
#define pi 3.14159265
#define cLength 0.3

void moniter(int timerID)
{
    static double theta = pi;
    static double dTheta = 0;
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
            dTheta -= 0.001 * cos(theta);
            theta += dTheta;
            break;

        case DOWN:
            dx = dR * cos(theta), dy = dR * sin(theta);
            centerX += dx;
            centerY += dy;
            got = checkMeet(centerX, centerY);
            if (got != NULL)
                state = UP;
            if (centerX <= 0 || centerX >= width || centerY <= 0)
                state = UP;
            break;

        case UP:
            dx = dR * cos(theta), dy = dR * sin(theta);
            centerX -= dx;
            centerY -= dy;
            if (got != NULL)
            {
                gold *cur = got->data;
                cur->x -= dx;
                cur->y -= dy;
            }
            if (centerY >= height * boardRatio)
            {
                if (got != NULL)
                {
                    gold *cur = got->data;
                    score += scoreMap[cur->type];
                    linkGold = delNode(linkGold, got);
                    got = NULL;
                }
                state = WAITING;
            }
            break;
        }
        refresh();
        MovePen(width * .5, height * boardRatio);
        DrawLine(centerX - width * .5, centerY - height * boardRatio);
        drawVector(centerX, centerY, cLength, theta - pi / 8);
        drawVector(centerX, centerY, cLength, theta + pi / 8);
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

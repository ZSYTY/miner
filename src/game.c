#include "game.h"
#include "start.h"
#include "utils.h"
#include "random.h"
#include "graphics.h"
#include "extgraph.h"

#include <math.h>
#include <windows.h>

static int score, target, level, countdown;
static int state;
#define WAITING 0
#define DOWN 1
#define UP 2
static linkHead linkGold;
extern double width, height;
#define boardRatio (4.0 / 5)
#define refreshInterval 20
static int scoreMap[GEM + 1] = {50, 100, 500, 20, 600};
static int ratioMap[GEM + 1] = {60, 25, 10, 25, 60};
static char colorMap[GEM + 1][20] = {"Gold3", "Gold2", "Gold1", "Gray", "Ivory"};
static char outColorMap[GEM + 1][20] = {"Goldenrod3", "Goldenrod2", "Goldenrod1", "Gray21", "Blue"};
static double speedMap[GEM + 1] = {0.1, 0.05, 0.02, 0.05, 0.1};

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
    while (linkGold != NULL)
        linkGold = delNode(linkGold, linkGold);
    // score = 0;
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
        SetPenColor(outColorMap[cur->type]);
        drawFilledRect(cur->x, cur->y, width / ratioMap[cur->type], height / ratioMap[cur->type]);
        double delta = 0.04;
        SetPenColor(colorMap[cur->type]);
        drawFilledRect(cur->x + delta, cur->y + delta, width / ratioMap[cur->type] - delta * 2, height / ratioMap[cur->type] - delta * 2);
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

    SetPenColor("Gray21");
    MovePen(0, height * boardRatio);
    DrawLine(width, 0);

    double r = 0.8;
    MovePen(width / 2 + r, height * boardRatio);
    DrawArc(r, 0, 180);
}

void displayState()
{
    SetPenColor("Gray21");
    static char stateText[MAX_TEXT_LENGTH + 1];
    sprintf(stateText, " 当前分数：%d  目标分数：%d  剩余时间：%d", score, target, countdown / 1000);
    MovePen(0, height * 5 / 6);
    DrawTextString(stateText);
}

void generateMap()
{
    countdown = 60 * 1000;
    target = 1000 * (level + 1);
    clearGold();
    int i, j, counter[5];
    counter[SMALL] = RandomInteger(2, 3);
    counter[MEDIUM] = RandomInteger(1, 2);
    counter[LARGE] = RandomInteger(1, 2);
    counter[STONE] = RandomInteger(1, 2);
    counter[GEM] = RandomInteger(1, 2);
    for (i = 4; i >= 0; i--)
        for (j = 0; j < counter[i]; j++)
            generateGold(i);
}

void refresh()
{
    clearScreen();
    displayBoard();
    drawGold();
    displayState();
}

#define defaultTimer 1
#define successTimer 2
#define failureTimer 3
#define pi 3.14159265
#define originSpeed 0.05
#define cLength 0.3

void runtime()
{
    static double theta = pi;
    static double dTheta = 0;
    static double dR = originSpeed;
    static double centerX;
    static double centerY;
    static linkNode *got;
    if (countdown <= 0)
    {
        cancelTimer(defaultTimer);
        if (score >= target)
        {
            level++;
            // TODO: draw sth
            startTimer(successTimer, 3000);
        }
        else
        {
            // TODO: draw sth
            startTimer(failureTimer, 3000);
        }
    }
    countdown -= refreshInterval;

    double dx, dy;
    double cx = cLength * cos(theta), cy = cLength * sin(theta);
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
        got = checkMeet(centerX + cx, centerY + cy);
        if (got != NULL)
        {
            state = UP;
            gold *cur = got->data;
            dR = speedMap[cur->type];
        }
        if (centerX <= 0 || centerX >= width || centerY <= 0)
        {
            state = UP;
            dR *= 2;
        }
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
            dR = originSpeed;
        }
        break;
    }
    refresh();
    MovePen(width * .5, height * boardRatio);
    DrawLine(centerX - width * .5, centerY - height * boardRatio);
    SetPenColor("Black");
    MovePen(centerX, centerY);
    DrawLine(cx, cy);
    drawVector(centerX + cx, centerY + cy, cLength, theta - pi / 5);
    drawVector(centerX + cx, centerY + cy, cLength, theta + pi / 5);
}

void moniter(int timerID)
{
    switch (timerID)
    {
    case defaultTimer:
        runtime();
        break;

    case successTimer:
        cancelTimer(successTimer);
        initGame();
        break;

    case failureTimer:
        cancelTimer(failureTimer);
        initStartPage();
        break;
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
    generateMap();
    refresh();

    registerKeyboardEvent(&handler);
    registerTimerEvent(&moniter);
    startTimer(defaultTimer, refreshInterval);
}

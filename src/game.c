#include "game.h"
#include "utils.h"
#include "random.h"
#include "graphics.h"
#include "extgraph.h"
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

static int score;
static linkHead linkGold;
extern double width, height;

int checkIntersect(gold aGold)
{
    linkHead p = linkGold;
    double l1, r1, l2, r2, u1, d1, u2, d2;
    l1 = aGold.x, d1 = aGold.y;
    if (aGold.type == 0)
        r1 = aGold.x + width / 64, u1 = aGold.y + height / 64;
    else if (aGold.type == 1)
        r1 = aGold.x + width / 25, u1 = aGold.y + height / 25;
    else
        r1 = aGold.x + width / 10, u1 = aGold.y + height / 10;
    while (p != NULL)
    {
        gold *cur = p->data;
        l2 = cur->x, d2 = cur->y;
        if (cur->type == 0)
            r2 = cur->x + width / 64, u2 = cur->y + height / 64;
        else if (cur->type == 1)
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
    if (aGold->type == 0)
    {
        do
        {
            aGold->x = RandomReal(0, width * 63 / 64);
            aGold->y = RandomReal(0, height * 5 / 7 * 63 / 64);
        } while (!checkIntersect(*aGold));
    }
    else if (aGold->type == 1)
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
        if (cur->type == 0)
        {
            drawFilledRect(cur->x, cur->y, width / 64, height / 64);
        }
        else if (cur->type == 1)
        {
            drawFilledRect(cur->x, cur->y, width / 25, height / 25);
        }
        else
        {
            drawFilledRect(cur->x, cur->y, width / 10, height / 10);
        }
        p = p->next;
    }
}

void displayBoard()
{
    double width = GetWindowWidth();
    double height = GetWindowHeight();

    SetPenColor("Black");
    MovePen(0, height / 5 * 4);
    DrawLine(width, 0);

    double minerWidth = 0.8;
    double minerHeight = 0.8;

    drawRectangle(width / 2 - minerWidth / 2, height / 5 * 4, minerWidth, minerHeight);

    // generateGold();
    // drawGold();
}

void displayState()
{
    static char stateText[MAX_TEXT_LENGTH + 1];
    sprintf(stateText, "当前分数：%d", score);
    MovePen(0, height * 5 / 6);
    DrawTextString(stateText);
}

void displayMap()
{
    int i, j, counter[3];
    counter[0] = RandomInteger(1, 1);
    counter[1] = RandomInteger(1, 1);
    counter[2] = RandomInteger(1, 1);
    for (i = 2; i >= 0; i--)
        for (j = 0; j < counter[i]; j++)
            generateGold(i);
    drawGold();
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
}

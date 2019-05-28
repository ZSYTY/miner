#include "game.h"
#include "utils.h"
#include "random.h"
#include "graphics.h"
#include "extgraph.h"

static int score;

void generateGold()
{
}

void drawGold()
{
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

    generateGold();
    drawGold();
}

void displayState()
{
    static char stateText[MAX_TEXT_LENGTH + 1];
    sprintf(stateText, "当前分数：%d", score);
    MovePen(0, 0.1);
    DrawTextString(stateText);
}

void initGame()
{
    clearScreen();
    displayBoard();
    displayState();
}
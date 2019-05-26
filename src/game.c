#include "game.h"
#include "utils.h"
#include "graphics.h"
#include "extgraph.h"

void initGame() {
    clearScreen();

    double width = GetWindowWidth();
    double height = GetWindowHeight();

    MovePen(0, height / 5 * 4);
    DrawLine(width, 0);

    double minerWidth = 0.8;
    double minerHeight = 0.8;

    drawRectangle(width / 2 - minerWidth / 2, height / 5 * 4, minerWidth, minerHeight);
}
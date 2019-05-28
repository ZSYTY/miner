#include "game.h"
#include "start.h"
#include "utils.h"

static button *start = NULL, *resume = NULL, *quit = NULL;
static linkHead buttonList;

void insAll()
{
    insButton(start);
    insButton(resume);
    insButton(quit);
}

void enableAll()
{
    enableButton(start);
    enableButton(resume);
    enableButton(quit);
}

void disableAll()
{
    disableButton(start);
    disableButton(resume);
    disableButton(quit);
}

void DrawFilledRect(double x, double y, double width, double height)
{
    MovePen(x, y);
    StartFilledRegion(1);
    DrawLine(width, 0);
    DrawLine(0, height);
    DrawLine(-width, 0);
    DrawLine(0, -height);
    EndFilledRegion();
}

void D(double x, double y)
{
    MovePen(x, y);
    SetPenColor("Cyan");
    StartFilledRegion(1);
    DrawArc(-0.5, 0, 180);
    EndFilledRegion();
    MovePen(x + 0.1, y);
    SetPenColor("Red");
    StartFilledRegion(1);
    DrawArc(-0.4, 0, 180);
    EndFilledRegion();
    SetPenColor("Cyan");
    DrawFilledRect(x, y, 1, 0.05);
}

void L(double x, double y)
{
    SetPenColor("Gray");
    DrawFilledRect(x - 0.1, y + 0.25, 0.5, 0.1);
    DrawFilledRect(x + 0.3, y + 0.25, 0.1, -0.9);
}

void O(double x, double y)
{
    SetPenColor("White");
    MovePen(x, y);
    StartFilledRegion(1);
    DrawEllipticalArc(0.225, 0.325, 0, 360);
    EndFilledRegion();
    MovePen(x - 0.075, y);
    SetPenColor("Magenta");
    StartFilledRegion(1);
    DrawEllipticalArc(0.15, 0.2, 0, 360);
    EndFilledRegion();
}

void G(double x, double y)
{
    SetPenColor("Dark Gray");
    DrawFilledRect(x - 1, y + 0.25, 0.5, 0.1);
    DrawFilledRect(x - 1.05, y + 0.35, 0.1, -0.9);
    DrawFilledRect(x - 1.05, y - 0.55, 0.45, -0.1);
    DrawFilledRect(x - 0.7, y - 0.55, 0.1, 0.15);
    DrawFilledRect(x - 0.8, y - 0.3, 0.3, 0.1);
}
void DrawFace(double x, double y)
{
    O(x - 0.5, y + 0.2);
    G(x - 0.5, y + 0.5);
    L(x - 0.5, y + 0.5);
    D(x - 1.25, y - 0.3);
}
void DrawHead(double x, double y)
{
    SetPenColor("Red");
    MovePen(x, y + 1.1);
    StartFilledRegion(1);
    DrawArc(1.1, 0, 180);
    EndFilledRegion();
    SetPenColor("Brown");
    DrawFilledRect(x, y + 1.1, -2.2, -0.1);
    MovePen(x - 0.55, y + 1.475);
    SetPenColor("Light Gray");
    StartFilledRegion(1);
    DrawArc(0.2, 0, 360);
    EndFilledRegion();
    MovePen(x - 0.65, y + 1.475);
    SetPenColor("Yellow");
    StartFilledRegion(1);
    DrawArc(0.1, 0, 360);
    EndFilledRegion();
}

void DrawBody(double x, double y)
{
    SetPenColor("Blue");
    MovePen(x - 2.2, y - 1.1);
    StartFilledRegion(1);
    DrawArc(-1.1, 0, 180);
    EndFilledRegion();
    SetPenColor("Green");
    DrawFilledRect(x - 2.2, y - 1.1, 2.2, -0.1);
    DrawFilledRect(x - 1.1, y - 1.25, 0.6, -0.4);
}

void DrawChan(double x, double y)
{
    SetPenColor("Brown");
    DrawFilledRect(x, y, 0.4, 0.1);
    DrawFilledRect(x + 0.15, y, 0.1, 1.2);
    SetPenColor("Light Gray");
    DrawFilledRect(x - 0.05, y + 1.1, 0.5, 0.5);
    MovePen(x - 0.05, y + 1.6);
    StartFilledRegion(1);
    DrawEllipticalArc(-0.25, 0.3, 0, 360);
    EndFilledRegion();
}

void MineCar(double x, double y, char *b)
{
    SetPenColor("Brown");
    DrawFilledRect(x - 0.75, y, 2.25, 0.15);
    DrawFilledRect(x - 0.6, y, 1.95, -0.45);
    MovePen(x - 0.15, y - 0.45);
    SetPenColor("Dark Gray");
    StartFilledRegion(1);
    DrawArc(0.15, 0, 360);
    EndFilledRegion();
    MovePen(x + 1.2, y - 0.45);
    StartFilledRegion(1);
    DrawArc(0.15, 0, 360);
    EndFilledRegion();
    SetPenColor("Yellow");
    DrawFilledRect(x - 0.57, y + 0.15, 1.8, 0.4);
    DrawFilledRect(x + 1.2, y + 0.15, 0.1, 0.6);
    DrawFilledRect(x, y + 0.55, 0.6, 0.3);
    DrawFilledRect(x, y + 0.55, -0.3, 0.12);
    DrawFilledRect(x + 0.8, y + 0.55, 0.2, 0.24);
    DrawFilledRect(x + 0.8, y + 0.55, -0.1, 0.36);
    DrawFilledRect(x + 0.8, y + 0.55, -0.2, 0.48);
    MovePen(x + 0.1, y + 0.4);
    SetPenColor("Red");
    DrawTextString(b);
}

void drawStartPage(double width, double height)
{
    SetPenColor("Black");
    DrawFilledRect(width, height, -width, -height);
    DrawFace(width / 2 - 1.5, height / 2);
    DrawHead(width / 2 - 1.5, height / 2);
    DrawChan(width / 2 - 3.7, height / 2 - 1);
    DrawBody(width / 2 - 1.5, height / 2);
    MineCar(width / 2, height / 2 + 0.5, "开始游戏");
    MineCar(width / 2 + 0.8, height / 2 - 0.5, "继续游戏");
    MineCar(width / 2 + 1.6, height / 2 - 1.5, "退出游戏");
}

void initStartPage()
{
    double width = GetWindowWidth();
    double height = GetWindowHeight();

    drawStartPage(width, height);

    double buttonWidth = width / 4;
    double buttonHeight = 1;

    start = createButton(width / 2, height / 2 + 0.5, buttonWidth, buttonHeight, "开始游戏", &startGame);

    resume = createButton(width / 2 + 0.8, height / 2 - 0.5, buttonWidth, buttonHeight, "继续游戏", &resumeGame);

    quit = createButton(width / 2 + 1.6, height / 2 - 1.5, buttonWidth, buttonHeight, "退出游戏", &quitGame);

    registerMouseEvent(buttonCallBack);
    insAll();
    enableAll();
}

void startGame()
{
    disableAll();
    initGame();
}

void resumeGame()
{
    disableAll();
    initGame();
}

void quitGame()
{
    ExitGraphics();
}
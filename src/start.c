#include "game.h"
#include "start.h"
#include "utils.h"

static button *start = NULL, *resume = NULL, *quit = NULL;

void enableAll() {
    enableButton(start);
    enableButton(resume);
    enableButton(quit);
}

void disableAll() {
    disableButton(start);
    disableButton(resume);
    disableButton(quit);
}

void initStartPage() {
    double width = GetWindowWidth();
    double height = GetWindowHeight();
    double buttonWidth = width / 4;
    double buttonHeight = 1;
    
    start = createButton(width / 2 - buttonWidth / 2, height / 2 + buttonHeight * 2, buttonWidth, buttonHeight, "��ʼ��Ϸ", &startGame);
    drawButton(start);
    insButton(start);

    resume = createButton(width / 2 - buttonWidth / 2, height / 2, buttonWidth, buttonHeight, "������Ϸ", &resumeGame);
    drawButton(resume);
    insButton(resume);

    quit = createButton(width / 2 - buttonWidth / 2, height / 2 - buttonHeight * 2, buttonWidth, buttonHeight, "�˳���Ϸ", &quitGame);
    drawButton(quit);
    insButton(quit);

    registerMouseEvent(globalCallBack);
    enableAll();
}

void startGame() {
    disableAll();
    initGame();
}

void resumeGame() {
    disableAll();
    initGame();
}

void quitGame() {
    ExitGraphics();
}
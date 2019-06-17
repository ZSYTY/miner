#include "start.h"
#include "utils.h"

static button *start = NULL, *resume = NULL, *quit = NULL;

void initStartPage(double width, double height) {
    double buttonWidth = width / 4;
    double buttonHeight = 1;
    
    start = createButton(width / 2 - buttonWidth / 2, height / 2 + buttonHeight * 2, buttonWidth, buttonHeight, "��ʼ��Ϸ", &startGame);
    drawButton(start);
    enableButton(start);
    insButton(start);

    resume = createButton(width / 2 - buttonWidth / 2, height / 2, buttonWidth, buttonHeight, "������Ϸ", &resumeGame);
    drawButton(resume);
    enableButton(resume);
    insButton(resume);

    quit = createButton(width / 2 - buttonWidth / 2, height / 2 - buttonHeight * 2, buttonWidth, buttonHeight, "�˳���Ϸ", &quitGame);
    drawButton(quit);
    enableButton(quit);
    insButton(quit);

    registerMouseEvent(globalCallBack);
}

void startGame() {
    
}

void resumeGame() {

}

void quitGame() {
    ExitGraphics();
}
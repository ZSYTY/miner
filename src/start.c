#include "start.h"
#include "utils.h"

static button *start = NULL, *resume = NULL, *quit = NULL;

void initStartPage(double width, double height) {
    double buttonWidth = width / 4;
    double buttonHeight = 1;
    
    start = createButton(width / 2 - buttonWidth / 2, height / 2 + buttonHeight * 2, buttonWidth, buttonHeight, "开始游戏", &startGame);
    drawButton(start);
    enableButton(start);
    insButton(start);

    resume = createButton(width / 2 - buttonWidth / 2, height / 2, buttonWidth, buttonHeight, "继续游戏", &resumeGame);
    drawButton(resume);
    enableButton(resume);
    insButton(resume);

    quit = createButton(width / 2 - buttonWidth / 2, height / 2 - buttonHeight * 2, buttonWidth, buttonHeight, "退出游戏", &quitGame);
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
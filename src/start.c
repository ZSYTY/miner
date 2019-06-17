#include "start.h"
#include "utils.h"

void initStartPage(double width, double height) {
    double buttonWidth = width / 4;
    double buttonHeight = 1;
    
    button *start = createButton(width / 2 - buttonWidth / 2, height / 2 + buttonHeight * 2, buttonWidth, buttonHeight, "��ʼ��Ϸ", &startGame);
    drawButton(start);
    enableButton(start);

    button *resume = createButton(width / 2 - buttonWidth / 2, height / 2, buttonWidth, buttonHeight, "������Ϸ", &resumeGame);
    drawButton(resume);
    enableButton(resume);

    button *exit = createButton(width / 2 - buttonWidth / 2, height / 2 - buttonHeight * 2, buttonWidth, buttonHeight, "�˳���Ϸ", &exitGame);
    drawButton(exit);
    enableButton(exit);
}

void startGame() {

}

void resumeGame() {

}

void exitGame() {

}
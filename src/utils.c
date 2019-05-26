#include "utils.h"

#include <string.h>

void drawRectangle(double x, double y, double width, double height) {
    MovePen(x, y);
    DrawLine(width, 0);
    DrawLine(0, height);
    DrawLine(-width, 0);
    DrawLine(0, -height);
}

void drawCircle(double x, double y, double r) {
    MovePen(x + r, y);
    DrawArc(r, 0, 360);
}

void clearScreen() {
    string preColor = GetPenColor();
    SetPenColor("White");

    StartFilledRegion(1);
    drawRectangle(0, 0, GetWindowWidth(), GetWindowHeight());
    EndFilledRegion();
    
    SetPenColor(preColor);   
}

button *createButton(double x, double y, double width, double height, char text[], buttonClickCallBack cb) {
    button *b = malloc(sizeof(button));
    b->x = x;
    b->y = y;
    b->width = width;
    b->height = height;
    strcpy(b->text, text);
    b->callback = cb;
    b->disabled = TRUE;
    return b;
}

void drawButton(button *b) {
    drawRectangle(b->x, b->y, b->width, b->height);
    MovePen(b->x + b->width / 2 - TextStringWidth(b->text) / 2, b->y + b->height / 2);
    DrawTextString(b->text);
}

void enableButton(button *b) {
    b->disabled = FALSE;
}

void disableButton(button *b) {
    b->disabled = TRUE;
}

bool isIn(button *b, int xi, int yi) {
    double x = ScaleXInches(xi), y = ScaleYInches(yi);
    return x >= b->x && x <= b->x + b->width && y >= b->y && y <= b->y + b->height;
}


linkHead ButtonList = NULL;

void insButton(button *bt) {
    linkNode* head = newNode();
    head->bt = bt;
    head->next = ButtonList;
    ButtonList = head;
}

linkNode* newNode() {
    return (linkNode *)malloc(sizeof(linkNode));
}

void globalCallBack(int x, int y, int button, int event) {
    if (button != LEFT_BUTTON || event != BUTTON_DOWN) {
        return;
    }
    linkNode* p = ButtonList;
    while (p != NULL) {
        if (! p->bt->disabled && isIn(p->bt, x, y)) {
            (*p->bt->callback)();
        }
        p = p->next;
    }
}
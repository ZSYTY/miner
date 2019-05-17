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
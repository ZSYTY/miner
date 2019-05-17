#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"

void drawRectangle(double x, double y, double width, double height);
void drawCircle(double x, double y, double r);


#define MAX_TEXT_LENGTH 80

typedef void (*buttonClickCallBack) (void);

typedef struct {
    double x, y, width, height;
    char text[MAX_TEXT_LENGTH + 1];
    buttonClickCallBack callback;
    bool disabled;
} button;

button* createButton(double x, double y, double width, double height, char text[], buttonClickCallBack cb);
void drawButton(button* b);
void enableButton(button *b);
void disableButton(button *b);
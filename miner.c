#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "genlib.h"
#include "conio.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "utils.h"
#include "start.h"
#include "game.h"

void Main() {
    SetWindowTitle("�ƽ��");
    InitGraphics();
    double width = GetWindowWidth(), height = GetWindowHeight();
    initStartPage(width, height);
}
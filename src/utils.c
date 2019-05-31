#include "utils.h"

#include <math.h>
#include <string.h>

void addColor(string name, int r, int g, int b)//添加颜色
{
    DefineColor(name, 1.0 * r / 255, 1.0 * g / 255, 1.0 * b / 255);
}

void initColor()//自定义颜色
{
    addColor("Azure", 240, 255, 255);
    addColor("Gold1", 255, 215, 0);
    addColor("Gold2", 238, 201, 0);
    addColor("Gold3", 205, 173, 0);
    addColor("Goldenrod1", 255, 193, 37);
    addColor("Goldenrod2", 238, 180, 34);
    addColor("Goldenrod3", 205, 155, 29);
    addColor("Gray21", 54, 54, 54);
    addColor("CornflowerBlue", 100, 149, 237);
    addColor("SpringGreen3", 0, 205, 102);
    addColor("LightCyan", 224, 255, 255);
    addColor("Ivory", 255, 255, 240);
}

void drawRectangle(double x, double y, double width, double height)//画矩形
{
    MovePen(x, y);
    DrawLine(width, 0);
    DrawLine(0, height);
    DrawLine(-width, 0);
    DrawLine(0, -height);
}

void drawFilledRect(double x, double y, double width, double height)//在矩形中填充颜色
{
    StartFilledRegion(1);
    drawRectangle(x, y, width, height);
    EndFilledRegion();
}

void drawCircle(double x, double y, double r)//画圆
{
    MovePen(x + r, y);
    DrawArc(r, 0, 360);
}

void drawVector(double r, double theta)//画向量
{
    DrawLine(r * cos(theta), r * sin(theta));
}

void clearScreen()//清屏幕
{
    string preColor = GetPenColor();
    SetPenColor("Azure");

    StartFilledRegion(1);
    drawRectangle(0, 0, GetWindowWidth(), GetWindowHeight());
    EndFilledRegion();

    SetPenColor(preColor);
}

button *createButton(double x, double y, double width, double height, char text[], buttonClickCallBack cb)//画按钮
{
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

void drawRoundRectangle(double x, double y, double w, double h)//四个角是圆的的矩形
{
    double r = 0.05;
    MovePen(x + r, y);
    DrawLine(w - r * 2, 0);
    DrawArc(r, 270, 90);
    DrawLine(0, h - r * 2);
    DrawArc(r, 0, 90);
    DrawLine(r * 2 - w, 0);
    DrawArc(r, 90, 90);
    DrawLine(0, r * 2 - h);
    DrawArc(r, 180, 90);
}

void drawButton(button *b)//画按钮
{
    SetPenColor("Gray21");
    drawRoundRectangle(b->x, b->y, b->width, b->height);
    MovePen(b->x + b->width / 2 - TextStringWidth(b->text) / 2, b->y + b->height / 2 - 0.05);
    DrawTextString(b->text);
}

void enableButton(button *b)//开启按钮
{
    b->disabled = FALSE;
}

void disableButton(button *b)//关闭按钮
{
    b->disabled = TRUE;
}

bool isIn(button *b, int xi, int yi)
{
    double x = ScaleXInches(xi), y = ScaleYInches(yi);
    return x >= b->x && x <= b->x + b->width && y >= b->y && y <= b->y + b->height;
}

linkHead insNode(linkHead head, void *data)//将数据插入链表
{
    linkNode *newHead = newNode();
    newHead->data = data;
    newHead->next = head;
    if (head != NULL)
    {
        head->pre = newHead;
    }
    return newHead;
}

linkHead delNode(linkHead head, linkNode *node)//将节点从链表中删除
{
    if (node->pre != NULL)
    {
        node->pre->next = node->next;
    }
    else
    {
        head = node->next;
    }

    if (node->next != NULL)
    {
        node->next->pre = node->pre;
    }

    free(node);
    return head;
}

linkNode *newNode()//新建链表
{
    linkNode *head = (linkNode *)malloc(sizeof(linkNode));
    head->pre = head->next = NULL;
    return head;
}

linkHead buttonList;//按钮的链表

void buttonCallBack(int x, int y, int bt, int event)//按钮反应
{
    if (bt != LEFT_BUTTON || event != BUTTON_DOWN)
    {
        return;
    }
    linkNode *p = buttonList;
    while (p != NULL)//遍历按钮的链表，检测是否点到了
    {
        button *currentButton = p->data;
        if (!currentButton->disabled && isIn(p->data, x, y))
        {
            (*currentButton->callback)();
        }
        p = p->next;
    }
}

void insButton(button *b)//向按钮链表中插入
{
    buttonList = insNode(buttonList, b);
}
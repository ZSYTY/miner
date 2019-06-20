#include "utils.h"

#include <math.h>
#include <string.h>

//添加颜色
void addColor(string name, int r, int g, int b) 
{
    DefineColor(name, 1.0 * r / 255, 1.0 * g / 255, 1.0 * b / 255);
}

//自定义颜色
void initColor() 
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
    addColor("Silver", 192, 192, 192);
    addColor("Cobalt", 61, 89, 171);
    SetFont("微软雅黑");
    SetPointSize(16);
}

void drawDiamond(double x, double y, double width, double height)
{
    MovePen(x, y + height / 2);
    SetPenColor("Cobalt");
    StartFilledRegion(1);
    DrawLine(width / 4, width / 4);
    DrawLine(width / 2, 0);
    DrawLine(width / 4, -width / 4);
    DrawLine(-width / 2, -(height));
    DrawLine(-width / 2, (height));
    EndFilledRegion();
}

//画不规则石头或者金块
void drawFilledIrregular(double x, double y, double width, double height) 
{
    drawFilledRect(x, y, width, height);
    drawFilledRect(x + width, y, width * 0.1, width / 4);
    drawFilledRect(x, y + height * 0.6, -height / 6, -width / 5);
    drawFilledRect(x, y, height, -width * 0.1);
    drawFilledRect(x + 0.2 * width, y + height, width * 0.4, height * 0.15);
    drawFilledRect(x + width * 0.1, y, height * 0.8, -height * 0.25);
    drawFilledRect(x + width, y, -width, -height * 0.1);
}

//画矩形
void drawRectangle(double x, double y, double width, double height) 
{
    MovePen(x, y);
    DrawLine(width, 0);
    DrawLine(0, height);
    DrawLine(-width, 0);
    DrawLine(0, -height);
}

//在矩形中填充颜色
void drawFilledRect(double x, double y, double width, double height) 
{
    StartFilledRegion(1);
    drawRectangle(x, y, width, height);
    EndFilledRegion();
}

//画圆
void drawCircle(double x, double y, double r) 
{
    MovePen(x + r, y);
    DrawArc(r, 0, 360);
}

//画向量
void drawVector(double r, double theta) 
{
    DrawLine(r * cos(theta), r * sin(theta));
}

//清屏幕
void clearScreen() 
{
    string preColor = GetPenColor();
    SetPenColor("Azure");

    StartFilledRegion(1);
    drawRectangle(0, 0, GetWindowWidth(), GetWindowHeight());
    EndFilledRegion();

    SetPenColor(preColor);
}

//画按钮
button *createButton(double x, double y, 
					double width, double height, char text[], 
					buttonClickCallBack cb) 
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

//四个角是圆的的矩形
void drawRoundRectangle(double x, double y, double w, double h) 
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

//画按钮
void drawButton(button *b) 
{
    SetPenColor("Blue");
    StartFilledRegion(1);
    drawRoundRectangle(b->x, b->y, b->width, b->height);
    EndFilledRegion();
    MovePen(b->x + b->width / 2 - TextStringWidth(b->text) / 2, b->y + b->height / 2 - 0.05);
    
	SetPenColor("White");
    DrawTextString(b->text);
    SetPenColor("Gray21");
}

//开启按钮
void enableButton(button *b) 
{
    b->disabled = FALSE;
}

//关闭按钮
void disableButton(button *b) 
{
    b->disabled = TRUE;
}

bool isIn(button *b, int xi, int yi)
{
    double x = ScaleXInches(xi), y = ScaleYInches(yi);
    return x >= b->x && x <= b->x + b->width && y >= b->y && y <= b->y + b->height;
}

//将数据插入链表
linkHead insNode(linkHead head, void *data) 
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

//将节点从链表中删除
linkHead delNode(linkHead head, linkNode *node) 
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

//新建链表
linkNode *newNode() 
{
    linkNode *head = (linkNode *)malloc(sizeof(linkNode));
    head->pre = head->next = NULL;
    return head;
}

linkHead buttonList; //按钮的链表

//按钮反应
void buttonCallBack(int x, int y, int bt, int event) 
{
    if (bt != LEFT_BUTTON || event != BUTTON_DOWN)
    {
        return;
    }
    linkNode *p = buttonList;
    
    //遍历按钮的链表，检测是否点到了
	while (p != NULL) 
    {
        button *currentButton = p->data;
        if (!currentButton->disabled && isIn(p->data, x, y))
        {
            (*currentButton->callback)();
            return;
        }
        p = p->next;
    }
}

//向按钮链表中插入
void insButton(button *b) 
{
    buttonList = insNode(buttonList, b);
}

//创建排行榜文件
void initrank() 
{
    FILE *fp = fopen(rankfile, "w");
    
	int i;
    for (i = 1; i <= 5; i++)
    {
        fprintf("%d\n", 0);
    }
    
	fclose(fp);
}

//增加成绩记录
void ranking(int score) 
{
    int scores[MAX_RANK_NUM + 1] = {0}, i = 0;
    int j, k;

    FILE *fp = fopen(rankfile, "r");
    if (fp != NULL)
    {
        while (~fscanf(fp, "%d", &scores[i]))
    	{
    		i++;
		}

		for (j = i; j > 0; j--)
		{
			if (score > scores[j - 1])
			{
				scores[j] = scores[j - 1];
			}
			else
			{
				break;
			}
		}
		scores[j] = score;
    }
    fclose(fp);

    fp = fopen(rankfile, "w");
    for (j = 0; j < MAX_RANK_NUM && j <= i; j++)
    {
        fprintf(fp, "%d\n", scores[j]);
    }
    fclose(fp);
}

#include "game.h"
#include "start.h"
#include "utils.h"

double width;
double height;
static button *start = NULL;
static button *resume = NULL;
static button *quit = NULL;
static button *rank = NULL;
static button *help = NULL;
static button *back = NULL;

//���밴ť
void insAll() 
{
    insButton(start);
    insButton(resume);
    insButton(quit);
    insButton(rank);
    insButton(help);
    insButton(back);
}

//������ť
void enableAll() 
{
    enableButton(start);
    enableButton(resume);
    enableButton(quit);
    enableButton(rank);
    enableButton(help);
}

//�رհ�ť
void disableAll() 
{
    disableButton(start);
    disableButton(resume);
    disableButton(quit);
    disableButton(rank);
    disableButton(help);
}

//����ʼ����Ŀ󹤡��󳵵�
void D(double x, double y)
{

    MovePen(x + 0.1, y);
    SetPenColor("Red");
    StartFilledRegion(1);
    DrawArc(-0.4, 0, 180);
    EndFilledRegion();
}

void L(double x, double y)
{
    SetPenColor("Gray");
    drawFilledRect(x - 0.1, y + 0.25, 0.5, 0.2);
    drawFilledRect(x + 0.3, y + 0.25, 0.2, 0.9);
}

void O(double x, double y)
{
    SetPenColor("Silver");
    MovePen(x + 0.3, y - 0.1);
    StartFilledRegion(1);
    DrawArc(0.5, 0, 360);
    EndFilledRegion();
    
	SetPenColor("White");
    MovePen(x + 0.2, y - 0.1);
    StartFilledRegion(1);
    DrawArc(0.4, 0, 360);
    EndFilledRegion();
    
	MovePen(x, y - 0.06);
    SetPenColor("Black");
    StartFilledRegion(1);
    DrawArc(0.2, 0, 360);
    EndFilledRegion();
}

void G(double x, double y)
{
    SetPenColor("Dark Gray");
    drawFilledRect(x - 1, y + 0.25, 0.4, 0.2);
    drawFilledRect(x - 1, y + 0.35, 0.2, -0.9);
    drawFilledRect(x - 1, y - 0.55, 0.45, -0.2);
    drawFilledRect(x - 0.6, y - 0.75, 0.15, 0.45);
}
void DrawFace(double x, double y)
{
    O(x - 0.8, y + 0.5);
    G(x - 1.6, y - 0.8);
    L(x + 0.1, y - 0.8);
    SetPenColor("Black");
    drawFilledRect(x - 1.4, y - 0.7, 0.8, 0.05);
}

void DrawSmile(double x, double y)
{
    O(x - 0.8, y + 0.5);
    G(x - 1.6, y - 0.8);
    L(x + 0.1, y - 0.8);
    D(x - 1.5, y - 0.4);
}

void DrawAngry(double x, double y)
{
    O(x - 0.8, y + 0.5);
    G(x - 1.6, y - 0.8);
    SetPenColor("Gray");
    drawFilledRect(x, y - 0.55, 0.3, 0.2);
    drawFilledRect(x + 0.2, y - 0.35, 0.2, -1.2);
    
	MovePen(x - 0.6, y - 0.8);
    SetPenColor("Red");
    StartFilledRegion(1);
    DrawArc(0.4, 0, 180);
    DrawEllipticalArc(-0.4, 0.2, 0, 180);
    EndFilledRegion();
}

void DrawHat(double x, double y)
{
    SetPenColor("Red");
    MovePen(x, y + 1.1);
    StartFilledRegion(1);
    DrawArc(1.1, 0, 180);
    EndFilledRegion();
    
	SetPenColor("Brown");
    drawFilledRect(x, y + 1.1, -2.2, -0.1);
    
	MovePen(x - 0.8, y + 1.6);
    SetPenColor("Light Gray");
    StartFilledRegion(1);
    DrawArc(0.2, 0, 360);
    EndFilledRegion();
    
	MovePen(x - 0.9, y + 1.6);
    SetPenColor("Yellow");
    StartFilledRegion(1);
    DrawArc(0.1, 0, 360);
    EndFilledRegion();
}

void DrawBody(double x, double y)
{
    SetPenColor("Yellow");
    drawFilledRect(x - 2.2, y - 1.1, 2.2, 2.2);
    
	SetPenColor("Blue");
    MovePen(x - 2.2, y - 1.1);
    StartFilledRegion(1);
    DrawArc(-1.1, 0, 180);
    EndFilledRegion();
    
	SetPenColor("SpringGreen3");
    drawFilledRect(x - 2.2, y - 1.1, 2.2, -0.1);
    drawFilledRect(x - 1.1, y - 1.25, 0.6, -0.4);
}

void DrawShovel(double x, double y)
{
    SetPenColor("Brown");
    drawFilledRect(x - 0.4, y - 0.1, 0.75, 0.2);
    drawFilledRect(x, y - 0.1, 0.15, 1.2);
    
	SetPenColor("Light Gray");
    drawFilledRect(x - 0.2, y + 1.1, 0.5, 0.5);
    MovePen(x - 0.2, y + 1.6);
    StartFilledRegion(1);
    DrawEllipticalArc(-0.25, 0.3, 0, 360);
    EndFilledRegion();
}

void MineCar(double x, double y, char *b)
{
    SetPenColor("Brown");
    drawFilledRect(x - 0.75, y, 2.25, 0.15);
    drawFilledRect(x - 0.6, y, 1.95, -0.45);
    MovePen(x - 0.15, y - 0.45);
    
	SetPenColor("Dark Gray");
    StartFilledRegion(1);
    DrawArc(0.15, 0, 360);
    EndFilledRegion();
    
	MovePen(x + 1.2, y - 0.45);
    StartFilledRegion(1);
    DrawArc(0.15, 0, 360);
    EndFilledRegion();
    
	SetPenColor("Gold1");
    drawFilledRect(x - 0.57, y + 0.15, 1.8, 0.4);
    drawFilledRect(x + 1.2, y + 0.15, 0.1, 0.6);
    drawFilledRect(x, y + 0.55, 0.6, 0.3);
    drawFilledRect(x, y + 0.55, -0.3, 0.12);
    drawFilledRect(x + 0.8, y + 0.55, 0.2, 0.24);
    drawFilledRect(x + 0.8, y + 0.55, -0.1, 0.36);
    drawFilledRect(x + 0.8, y + 0.55, -0.2, 0.48);
    
	MovePen(x + 0.1, y + 0.4);
    SetPenColor("Red");
    DrawTextString(b);
}

void drawStartPage(double width, double height) //��ʼ����
{
    SetPenColor("Azure");
    drawFilledRect(width, height, -width, -height);
    
	DrawHat(width / 2 - 1.5, height / 2);
    DrawBody(width / 2 - 1.5, height / 2);
    DrawShovel(width / 2 - 3.7, height / 2 - 1);
    DrawFace(width / 2 - 1.5, height / 2);
    
	MineCar(width / 2, height / 2 + 2, "��ʼ��Ϸ");
    MineCar(width / 2 + 0.8, height / 2 + 1, "������Ϸ");
    MineCar(width / 2 + 1.6, height / 2, "�˳���Ϸ");
    MineCar(width / 2 + 0.8, height / 2 - 1, "���а�");
    MineCar(width / 2, height / 2 - 2, "����");
}

void closeModal() // �ر�ģ̬��
{
    enableAll();
    disableButton(back);
    initStartPage();
}

void openModal() // ��ģ̬��
{
    double modalWidth = width / 4, modalHeight = height / 2;
    disableAll();
    enableButton(back);
    SetPenColor("LightCyan");
    StartFilledRegion(1);
    drawRoundRectangle(width / 2 - modalWidth / 2, height / 2 - modalHeight / 2, modalWidth, modalHeight);
    EndFilledRegion();
    drawButton(back);
}

void intro() // ����˵��
{
    openModal();
    double ox = width * 3 / 8 + 0.2, oy = height * 3 / 4 - 0.5, lineHeight = 0.4;
    SetPointSize(12);
    
	MovePen(ox, oy);
    DrawTextString("��Ϸ˵����");
    oy -= lineHeight;
    
	MovePen(ox, oy);
    DrawTextString("���¼������Ź�������ȡ��ʯ��");
    oy -= lineHeight;
    
	MovePen(ox, oy);
    DrawTextString("û�б�Ҫ���ϼ�����Ϊ��û��ըҩ��");
    oy -= lineHeight;
    
	MovePen(ox, oy);
    DrawTextString("Ŀ����������Զ��κ�����ʽ����");
    oy -= lineHeight;
    
	MovePen(ox, oy);
    DrawTextString("������Ϸ���������ܳż��ְɣ�");
    oy -= lineHeight;
    SetPointSize(16);
}

void initStartPage() //��ʼ����ʼ����
{
    width = GetWindowWidth();
    height = GetWindowHeight();

    drawStartPage(width, height);

    double buttonWidth = width / 8;
    double buttonHeight = 1;
    double delta = 0.05;

    if (start == NULL)
    {
        start = createButton(width / 2, height / 2 + 2, buttonWidth, buttonHeight, "��ʼ��Ϸ", &startGame);
        resume = createButton(width / 2 + 0.8, height / 2 + 1, buttonWidth, buttonHeight, "������Ϸ", &resumeGame);
        quit = createButton(width / 2 + 1.6, height / 2, buttonWidth, buttonHeight, "�˳���Ϸ", &quitGame);
        rank = createButton(width / 2 + 0.8, height / 2 - 1, buttonWidth, buttonHeight, "���а�", &displayRank);
        help = createButton(width / 2, height / 2 - 2, buttonWidth, buttonHeight, "����", &intro);

        buttonWidth = 0.6, buttonHeight = 0.3, delta = 0.05;
        back = createButton(width * 5 / 8 - buttonWidth - delta, height / 4 + delta, buttonWidth, buttonHeight, "����", &closeModal);

        registerMouseEvent(buttonCallBack);
        insAll();
    }

    enableAll();
}

void startGame() //��ʼ��Ϸ
{
    disableAll();
    newGame();
}

void resumeGame() //������Ϸ
{
    disableAll();
    loadGame();
}

void quitGame() //�˳���Ϸ
{
    ExitGraphics();
}

void displayRank() // ��ʾ����
{
    openModal();
    
	FILE *fp = fopen(rankfile, "r");
    int scores[MAX_RANK_NUM], i = 0, j;
    char buffer[MAX_TEXT_LENGTH];
    while (~fscanf(fp, "%d", &scores[i]))
        i++;
    
	double ox = width * 3 / 8 + 0.2, oy = height * 3 / 4 - 0.5, lineHeight = 0.4;
    MovePen(ox, oy);
    DrawTextString("����              ����");
    
	for (j = 0; j < i; j++)
    {
        oy -= lineHeight;
        MovePen(ox, oy);
        sprintf(buffer, "%-18d%-18d", j + 1, scores[j]);
        DrawTextString(buffer);
    }
    
	fclose(fp);
}

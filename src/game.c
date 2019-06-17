#include "game.h"
#include "start.h"
#include "utils.h"
#include "random.h"
#include "graphics.h"
#include "extgraph.h"

#include <math.h>
#include <windows.h>

enum
{
    WAITING,
    DOWN,
    UP,
    PAUSED
};//״̬����
static int score, target, level, countdown;//������Ŀ��������ȼ���ʱ�䵹��
static int state;
static linkHead linkGold;//�洢�������
extern double width, height;//���Ŀ�͸�
#define boardRatio (4.0 / 5)
#define refreshInterval 20
#define defaultTimer 1
#define successTimer 2
#define failureTimer 3
#define pi 3.14159265
#define originSpeed 0.075
#define cLength 0.15
static int scoreMap[GEM + 1] = {50, 100, 500, 20, 600};
static int ratioMap[GEM + 1] = {60, 25, 10, 25, 60};
static char colorMap[GEM + 1][20] = {"Gold3", "Gold2", "Gold1", "Gray", "Ivory"};//�����õ���ɫ
static char outColorMap[GEM + 1][20] = {"Goldenrod3", "Goldenrod2", "Goldenrod1", "Gray21", "Blue"};//���������ɫ
static double speedMap[GEM + 1] = {0.1, 0.05, 0.01, 0.01, 0.1};//�����������ķ����ٶ�
static button *pause;

int checkIntersect(gold aGold)//������ɿ��ʱ�򣬼���Ƿ����������ཻ
{
    linkHead p = linkGold;
    double l1, r1, l2, r2, u1, d1, u2, d2;
    l1 = aGold.x, d1 = aGold.y;
    r1 = aGold.x + width / ratioMap[aGold.type];
    u1 = aGold.y + height / ratioMap[aGold.type];//�������ɵĿ��״̬
    while (p != NULL)//������������Ƿ��ཻ
    {
        gold *cur = p->data;
        l2 = cur->x, d2 = cur->y;
        r2 = cur->x + width / ratioMap[cur->type];
        u2 = cur->y + height / ratioMap[cur->type];
        if (min(r1, r2) > max(l1, l2) || min(u1, u2) > max(d1, d2))
            return 0;//�ཻ
        p = p->next;
    }
    return 1;//���ཻ
}

void clearGold()//�����
{
    while (linkGold != NULL)
        linkGold = delNode(linkGold, linkGold);
    // score = 0;
}

void generateGold(int type)//���ݿ�����Ͳ�����
{
    linkHead p = linkGold;
    gold *aGold = malloc(sizeof(gold));
    aGold->type = type;
    double boundRatio = (ratioMap[type] - 1.0) / ratioMap[type];
    do
    {
        aGold->x = RandomReal(0, width * boundRatio);
        aGold->y = RandomReal(0, height * boundRatio * 5 / 7);
    } while (!checkIntersect(*aGold));//һֱ������ֱ���䲻�뵱ǰ�κο��ཻ
    linkGold = insNode(linkGold, aGold);
}

void drawGold()//������������󣬻�����
{
    linkHead p = linkGold;
    while (p != NULL)//������������ÿһ���ڵ㣬����ͼ
    {
        gold *cur = p->data;
        SetPenColor(outColorMap[cur->type]);
        drawFilledRect(cur->x, cur->y, width / ratioMap[cur->type], height / ratioMap[cur->type]);
        double delta = 0.04;
        SetPenColor(colorMap[cur->type]);
        drawFilledRect(cur->x + delta, cur->y + delta, width / ratioMap[cur->type] - delta * 2, height / ratioMap[cur->type] - delta * 2);
        p = p->next;
    }
}

linkNode *checkMeet(double x, double y)//��鹳���Ƿ������˿�������������ؿ��ָ��
{
    linkNode *p = linkGold;
    gold *cur = NULL;
    while (p != NULL)
    {
        cur = p->data;
        if (x >= cur->x - .8 * cLength && x <= cur->x + width / ratioMap[cur->type] + .8 * cLength && y >= cur->y && y <= cur->y + height / ratioMap[cur->type] + cLength)
            return p;//ץ����
        p = p->next;
    }
    return NULL;//ûץ��
}

void displayBoard()//������
{
    double width = GetWindowWidth();
    double height = GetWindowHeight();

    SetPenColor("Gray21");
    MovePen(0, height * boardRatio);
    DrawLine(width, 0);

    double r = 0.8;
    MovePen(width / 2 + r, height * boardRatio);
    DrawArc(r, 0, 180);
}

void displayState()//����״̬
{
    SetPenColor("Gray21");
    static char stateText[MAX_TEXT_LENGTH + 1];
    sprintf(stateText, " ��ǰ������%d  Ŀ�������%d  ʣ��ʱ�䣺%d", score, target, countdown / 1000);
    MovePen(0, height * 5 / 6);
    DrawTextString(stateText);
}

void generateMap()//���������ͼ
{
    countdown = 60 * 1000;
    target += 500 * (level + 1);
    state = WAITING;
    clearGold();
    int i, j, counter[5];//���ֿ�������ٸ�
    counter[SMALL] = RandomInteger(2, min(4, 3 + level));
    counter[MEDIUM] = RandomInteger(1, 2);
    counter[LARGE] = RandomInteger(max(1, 2 - level), max(1, 2 - level));
    counter[STONE] = RandomInteger(min(1 + level, 3), min(2 + level, 4));
    counter[GEM] = RandomInteger(min(2, 1 + level), min(2 + level, 4));
    for (i = 4; i >= 0; i--)
        for (j = 0; j < counter[i]; j++)
            generateGold(i);
}

void refresh()//ˢ�½���
{
    clearScreen();
    displayBoard();
    drawGold();
    displayState();
    drawButton(pause);
}

void drawHook(double x, double y, double theta)//������
{
    SetPenColor("Gray21");
    MovePen(x, y);
    drawVector(cLength, theta - pi / 4);
    drawVector(cLength, theta - pi / 10);
    MovePen(x, y);
    drawVector(cLength, theta + pi / 4);
    drawVector(cLength, theta + pi / 10);
}

void drawSuccess()//��ͨ�ؽ���
{
    // SetEraseMode(TRUE);
    // refresh();
    // clearScreen();
    // clearScreen();
    // clearScreen();
    // clearScreen();
    // clearScreen();
    // clearScreen();
    // SetEraseMode(FALSE);
    // displayBoard();
    SetPenColor("Green");
    SetPenSize(8);
    static char stateText[MAX_TEXT_LENGTH + 1];
    sprintf(stateText, " ��ϲ��˳������ ");
    MovePen(width / 2, height / 2);
    DrawTextString(stateText);
    SetPenSize(1);
}

void runtime()//��ƽ��ʱ�Ķ���
{
    static double theta = pi;
    static double dTheta = 0;
    static double dR = originSpeed;
    static double centerX;
    static double centerY;
    static linkNode *got;
    if (countdown <= 0)//ʱ�䵽
    {
        cancelTimer(defaultTimer);
        got = NULL;
        if (score >= target)//���Ŀ��
        {
            level++;
            drawSuccess();
            startTimer(successTimer, 5000);
        }
        else//δ���Ŀ��
        {
            // TODO: draw sth
            level = 0;
            startTimer(failureTimer, 5000);
        }
    }
    countdown -= refreshInterval;

    double dx, dy;
    double cx = cLength * cos(theta), cy = cLength * sin(theta);
    switch (state)//�ֹ��ӵ�״̬��
    {
    case WAITING://�ȴ�״̬��������ת
        centerX = width * .5;
        centerY = height * boardRatio;
        dTheta -= 0.001 * cos(theta);
        theta += dTheta;
        break;

    case DOWN://�½�״̬������Ƿ�ץ����
        dx = dR * cos(theta), dy = dR * sin(theta);
        centerX += dx;
        centerY += dy;
        got = checkMeet(centerX + cx, centerY + cy);
        if (got != NULL)//ץ����
        {
            state = UP;
            gold *cur = got->data;
            dR = speedMap[cur->type];
        }
        if (centerX <= 0 || centerX >= width || centerY <= 0)//�����߽�
        {
            state = UP;
            dR *= 2;
        }
        break;

    case UP://����״̬���ж��Ƿ�Я����
        dx = dR * cos(theta), dy = dR * sin(theta);
        centerX -= dx;
        centerY -= dy;
        if (got != NULL)
        {
            gold *cur = got->data;
            cur->x -= dx;
            cur->y -= dy;
        }
        if (centerY >= height * boardRatio)//���ӻص�ԭ��
        {
            if (got != NULL)
            {
                gold *cur = got->data;
                score += scoreMap[cur->type];
                linkGold = delNode(linkGold, got);
                got = NULL;
            }
            state = WAITING;
            dR = originSpeed;
        }
        break;
    }
    refresh();//ˢ�µ�ǰ��ͼ
    MovePen(width * .5, height * boardRatio);
    DrawLine(centerX - width * .5, centerY - height * boardRatio);
    SetPenColor("Black");
    MovePen(centerX, centerY);
    DrawLine(cx, cy);
    drawHook(centerX + cx, centerY + cy, theta);
}

void moniter(int timerID)
{
    switch (timerID)
    {
    case defaultTimer:
        runtime();
        break;

    case successTimer:
        cancelTimer(successTimer);
        initGame();
        break;

    case failureTimer:
        cancelTimer(failureTimer);
        disableButton(pause);
        score = 0;
        initStartPage();
        break;
    }
}

void handler(int key, int event)
{
    if (key == VK_DOWN && event == KEY_DOWN && state == WAITING)
    {
        state = DOWN;
    }
}

void pauseGame()
{
    static int preState;
    if (state == PAUSED)
    {
        state = preState;
        startTimer(defaultTimer, refreshInterval);
    }
    else
    {
        preState = state;
        state = PAUSED;
        cancelTimer(defaultTimer);
    }
}

void initButton()
{
    double buttonWidth = 0.6, buttonHeight = 0.3, delta = 0.05;
    pause = createButton(delta, height - buttonHeight - delta, buttonWidth, buttonHeight, "��ͣ", &pauseGame);
    insButton(pause);
    drawButton(pause);
    enableButton(pause);
}

void initGame()
{
    if (pause == NULL)
        initButton();

    Randomize();
    generateMap();
    refresh();

    registerKeyboardEvent(&handler);
    registerTimerEvent(&moniter);
    startTimer(defaultTimer, refreshInterval);
}

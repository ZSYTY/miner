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
};                                          //状态参数
static int score, target, level, countdown; //分数，目标分数，等级，时间倒数
static int state;
static linkHead linkGold;    //存储矿的链表
extern double width, height; //屏的宽和高
#define boardRatio (4.0 / 5) //矿区占屏幕的比例
#define refreshInterval 20   //刷新间隔
#define defaultTimer 1
#define successTimer 2
#define failureTimer 3
#define pi 3.14159265     //圆周率
#define originSpeed 0.075 //钩子初始速度
#define cLength 0.15      //钩子的尺寸参数
static int scoreMap[GEM + 1] = {50, 100, 500, 20, 600};
static int ratioMap[GEM + 1] = {60, 25, 10, 25, 60};
static char colorMap[GEM + 1][20] = {"Gold3", "Gold2", "Gold1", "Gray", "Ivory"};                    //画矿用的颜色
static char outColorMap[GEM + 1][20] = {"Goldenrod3", "Goldenrod2", "Goldenrod1", "Gray21", "Blue"}; //画界面的颜色
static double speedMap[GEM + 1] = {0.1, 0.05, 0.01, 0.01, 0.1};                                      //钩子碰到矿后的返回速度
static button *pause, *save, *resume, *quit;                                                         // 游戏界面的四个按钮

int checkIntersect(gold aGold) //随机生成矿的时候，检查是否有两个矿相交
{
    linkHead p = linkGold;
    double l1, r1, l2, r2, u1, d1, u2, d2;
    l1 = aGold.x, d1 = aGold.y;
    r1 = aGold.x + width / ratioMap[aGold.type];
    u1 = aGold.y + height / ratioMap[aGold.type]; //即将生成的矿的状态
    while (p != NULL)                             //遍历链表，检查是否相交
    {
        gold *cur = p->data;
        l2 = cur->x, d2 = cur->y;
        r2 = cur->x + width / ratioMap[cur->type];
        u2 = cur->y + height / ratioMap[cur->type];
        if (min(r1, r2) > max(l1, l2) || min(u1, u2) > max(d1, d2))
            return 0; //相交
        p = p->next;
    }
    return 1; //不相交
}

void clearGold() //清除矿
{
    while (linkGold != NULL)
        linkGold = delNode(linkGold, linkGold);
}

void generateGold(int type) //根据矿的类型产生矿
{
    linkHead p = linkGold;
    gold *aGold = malloc(sizeof(gold));
    aGold->type = type;
    double boundRatio = (ratioMap[type] - 1.0) / ratioMap[type];
    do
    {
        aGold->x = RandomReal(0, width * boundRatio);
        aGold->y = RandomReal(0, height * boundRatio * 5 / 7);
    } while (!checkIntersect(*aGold)); //一直产生矿，直到其不与当前任何矿相交
    linkGold = insNode(linkGold, aGold);
}

void drawGold() //构建完矿的链表后，画出矿
{
    linkHead p = linkGold;
    while (p != NULL) //遍历矿的链表的每一个节点，并画图
    {
        gold *cur = p->data;
        if (cur->type != GEM)
        {
            SetPenColor(outColorMap[cur->type]);
            drawFilledIrregular(cur->x, cur->y, width / ratioMap[cur->type], height / ratioMap[cur->type]);
            double delta = 0.04;
            SetPenColor(colorMap[cur->type]);
            drawFilledIrregular(cur->x + delta, cur->y + delta, width / ratioMap[cur->type] - delta * 2, height / ratioMap[cur->type] - delta * 2);
            p = p->next;
        }
        else
        {
            SetPenColor(outColorMap[cur->type]);
            drawDiamond(cur->x, cur->y, width / ratioMap[cur->type], height / ratioMap[cur->type]);
            double delta = 0.04;
            SetPenColor(colorMap[cur->type]);
            drawDiamond(cur->x + delta, cur->y + delta, width / ratioMap[cur->type] - delta * 2, height / ratioMap[cur->type] - delta * 2);
            p = p->next;
        }
    }
}

linkNode *checkMeet(double x, double y) //检查钩子是否碰到了矿，如果碰到，返回矿的指针
{
    linkNode *p = linkGold;
    gold *cur = NULL;
    while (p != NULL)
    {
        cur = p->data;
        if (x >= cur->x - .8 * cLength && x <= cur->x + width / ratioMap[cur->type] + cLength && y >= cur->y && y <= cur->y + height / ratioMap[cur->type] + cLength)
            return p; //抓到了
        p = p->next;
    }
    return NULL; //没抓到
}

void displayBoard() //画背景
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

void displayState() //标明状态
{
    SetPenColor("Gray21");
    static char stateText[MAX_TEXT_LENGTH + 1];
    sprintf(stateText, " 当前分数：%d  目标分数：%d  剩余时间：%d", score, target, countdown / 1000);
    MovePen(0, height * 5 / 6);
    DrawTextString(stateText);
}

void generateMap() //随机产生地图
{
    countdown = 60 * 1000;
    target = 250 * (level + 1) * (level + 2);
    state = WAITING;
    clearGold();
    int i, j, counter[5]; //各种矿产生多少个
    counter[SMALL] = RandomInteger(2, min(4, 3 + level));
    counter[MEDIUM] = RandomInteger(1, 2);
    counter[LARGE] = RandomInteger(max(1, 2 - level), max(1, 2 - level));
    counter[STONE] = RandomInteger(min(1 + level, 3), min(2 + level, 4));
    counter[GEM] = RandomInteger(min(2, 1 + level), min(2 + level, 4));
    for (i = 4; i >= 0; i--)
        for (j = 0; j < counter[i]; j++)
            generateGold(i);
}

void refresh() //刷新界面
{
    DisplayClear();
    clearScreen();
    displayBoard();
    drawGold();
    displayState();
    drawButton(pause);
}

void drawHook(double x, double y, double theta) //画钩子
{
    SetPenColor("Gray21");
    MovePen(x, y);
    drawVector(cLength, theta - pi / 4);
    drawVector(cLength, theta - pi / 10);
    MovePen(x, y);
    drawVector(cLength, theta + pi / 4);
    drawVector(cLength, theta + pi / 10);
}

void drawSuccess() //画通关界面
{
    clearScreen();
    static char stateText[MAX_TEXT_LENGTH + 1];
    width = GetWindowWidth();
    height = GetWindowHeight();
    DrawHat(width / 2, height / 2);
    DrawBody(width / 2, height / 2);
    DrawShovel(width / 2 - 2.2, height / 2 - 1);
    DrawSmile(width / 2, height / 2);
    SetPenColor("SpringGreen3");
    sprintf(stateText, " 恭喜你通过本关，即将进入下一关 ");
    MovePen(width / 2 + 0.5, height / 2 + 0.5);
    DrawTextString(stateText);
    SetPenSize(1);
}

void drawFailure() //画失败界面
{
    clearScreen();
    static char stateText[MAX_TEXT_LENGTH + 1];
    width = GetWindowWidth();
    height = GetWindowHeight();
    DrawHat(width / 2, height / 2);
    DrawBody(width / 2, height / 2);
    DrawShovel(width / 2 - 2.2, height / 2 - 1);
    DrawAngry(width / 2, height / 2);
    SetPenColor("Red");
    SetPenSize(8);
    sprintf(stateText, " 很遗憾，你没有通关 \n 本次分数：%d分\n", score);
    MovePen(width / 2 + 0.5, height / 2 + 0.5);
    DrawTextString(stateText);
    SetPenSize(1);
}

void runtime() //玩黄金矿工时的动画
{
    static double theta = pi;
    static double dTheta = 0;
    static double dR = originSpeed;
    static double centerX;
    static double centerY;
    static linkNode *got;
    if (countdown <= 0) //时间到
    {
        cancelTimer(defaultTimer);
        got = NULL;

        if (score >= target) //完成目标
        {
            level++;
            drawSuccess();
            startTimer(successTimer, 5000);
        }
        else //未完成目标
        {
            ranking(score);
            level = 0;
            drawFailure();
            startTimer(failureTimer, 5000);
        }
        return;
    }
    countdown -= refreshInterval;

    double dx, dy;
    double cx = cLength * cos(theta), cy = cLength * sin(theta);
    switch (state) //分钩子的状态画
    {
    case WAITING: //等待状态，钩子旋转
        dR = originSpeed;
        centerX = width * .5;
        centerY = height * boardRatio;
        dTheta -= 0.001 * cos(theta);
        theta += dTheta;
        break;

    case DOWN: //下降状态，检测是否抓到矿
        dx = dR * cos(theta), dy = dR * sin(theta);
        centerX += dx;
        centerY += dy;
        got = checkMeet(centerX + cx, centerY + cy);
        if (got != NULL) //抓到矿
        {
            state = UP;
            gold *cur = got->data;
            dR = speedMap[cur->type];
        }
        if (centerX <= 0 || centerX >= width || centerY <= 0) //超出边界
        {
            state = UP;
            dR *= 2;
        }
        break;

    case UP: //上升状态，判断是否携带矿
        dx = dR * cos(theta), dy = dR * sin(theta);
        centerX -= dx;
        centerY -= dy;
        if (got != NULL)
        {
            gold *cur = got->data;
            cur->x -= dx;
            cur->y -= dy;
        }
        if (centerY >= height * boardRatio) //钩子回到原点
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
    refresh(); //刷新当前的图
    SetPenSize(3);
    MovePen(width * .5, height * boardRatio);
    DrawLine(centerX - width * .5, centerY - height * boardRatio);
    SetPenColor("Black");
    MovePen(centerX, centerY);
    DrawLine(cx, cy);
    drawHook(centerX + cx, centerY + cy, theta);
    SetPenSize(1);
}

void moniter(int timerID) //根据时间执行
{
    switch (timerID)
    {
    case defaultTimer: //游戏中
        runtime();
        break;

    case successTimer: //游戏结束，通关
        cancelTimer(successTimer);
        initGame();
        break;

    case failureTimer: //游戏结束，没通关
        cancelTimer(failureTimer);
        disableButton(pause);
        score = 0;
        initStartPage();
        break;
    }
}

void handler(int key, int event) //用户按“下键”，放下钩子
{
    if (key == VK_DOWN && event == KEY_DOWN && state == WAITING)
    {
        state = DOWN;
    }
}

void pauseGame() //暂停游戏
{
    static int preState;
    if (state == PAUSED) //当前已暂停，恢复
    {
        state = preState;
        strcpy(pause->text, "暂停");
        disableButton(save);
        disableButton(resume);
        disableButton(quit);
        startTimer(defaultTimer, refreshInterval);
    }
    else //当前未暂停，即暂停
    {
        preState = state;
        state = PAUSED;
        strcpy(pause->text, "继续");
        runtime();
        MineCar(width / 2, height / 2 + 0.5, "保存游戏");
        MineCar(width / 2 + 0.8, height / 2 - 0.5, "继续游戏");
        MineCar(width / 2 + 1.6, height / 2 - 1.5, "退出游戏");
        enableButton(save);
        enableButton(resume);
        enableButton(quit);
        cancelTimer(defaultTimer);
    }
}

void saveGame() // 保存游戏
{
    FILE *fp = fopen(savefile, "w");
    fprintf(fp, "%d %d %d\n", score, level, countdown);
    linkNode *p = linkGold;
    while (p != NULL)
    {
        gold *cur = p->data;
        fprintf(fp, "%lf %lf %d\n", cur->x, cur->y, cur->type);
        p = p->next;
    }
    fclose(fp);

    pauseGame();
}

void returnMenu() // 返回主菜单
{
    strcpy(pause->text, "暂停");
    disableButton(pause);
    disableButton(save);
    disableButton(resume);
    disableButton(quit);
    initStartPage();
}

void initButton() //初始化按钮
{
    double buttonWidth = 0.6, buttonHeight = 0.3, delta = 0.05;
    pause = createButton(delta, height - buttonHeight - delta, buttonWidth, buttonHeight, "暂停", &pauseGame);
    insButton(pause);
    drawButton(pause);
    enableButton(pause);

    buttonWidth = width / 8;
    buttonHeight = 1;

    save = createButton(width / 2, height / 2 + 0.5, buttonWidth, buttonHeight, "保存游戏", &saveGame);
    resume = createButton(width / 2 + 0.8, height / 2 - 0.5, buttonWidth, buttonHeight, "继续游戏", &pauseGame);
    quit = createButton(width / 2 + 1.6, height / 2 - 1.5, buttonWidth, buttonHeight, "退出游戏", &returnMenu);
    insButton(save);
    insButton(resume);
    insButton(quit);
}

void initGame() //游戏初始化
{
    if (pause == NULL)
        initButton();

    Randomize();
    generateMap();
    refresh();

    enableButton(pause);
    registerKeyboardEvent(&handler);
    registerTimerEvent(&moniter);
    startTimer(defaultTimer, refreshInterval);
}

void loadGame() //加载存档
{
    FILE *fp = fopen(savefile, "r");
    if (fp != NULL)
    {
        if (!~fscanf(fp, "%d%d%d", &score, &level, &countdown))
        {
            initGame();
            fclose(fp);
            return;
        }

        if (pause == NULL)
            initButton();

        enableButton(pause);

        target = 250 * (level + 1) * (level + 2);
        state = WAITING;
        clearGold();

        double x, y;
        goldType type;
        while (~fscanf(fp, "%lf%lf%d", &x, &y, &type))
        {
            gold *aGold = malloc(sizeof(gold));
            aGold->type = type;
            aGold->x = x;
            aGold->y = y;
            linkGold = insNode(linkGold, aGold);
        }

        refresh();
        registerKeyboardEvent(&handler);
        registerTimerEvent(&moniter);
        startTimer(defaultTimer, refreshInterval);
    }
    else
    {
        initGame();
    }
    fclose(fp);
}

void newGame() // 重新开始游戏
{
    level = score = 0;
    initGame();
}

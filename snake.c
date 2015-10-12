#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define INIT_X 0     //游戏窗口左上角的X轴坐标
#define INIT_Y 0     //游戏窗口左上角的Y轴坐标
#define HEIGHT 20    //游戏窗口的高度
#define WIDTH 20     //游戏窗口的宽度

/*蛇节点*/
typedef struct Snake
{
    int x;
    int y;
    struct Snake *pre;
    struct Snake *next;
}SNAKE, *pSNAKE;

/*食物*/
typedef struct Food
{
    int x;
    int y;
}FOOD, *pFOOD;

/*设置文本为绿色*/
void set_color_green(void)
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Handle, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
}

/*设置文本为红色*/
void set_color_red(void)
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
}

/*光标移到指定位置*/
void goto_pos(HANDLE hOut, int x, int y)
{
    COORD pos;
    pos.X = x;    //横坐标
    pos.Y = y;    //纵坐标
    SetConsoleCursorPosition(hOut, pos);
}

/*初始化游戏窗口**/
void init_frame()
{
	int i = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);    //定义显示器句柄变量

	set_color_red();

	goto_pos(hOut, INIT_X + 2 * WIDTH + 6, INIT_Y + 2);    //打印游戏名称
	printf("贪吃蛇游戏");

	set_color_green();

	goto_pos(hOut, INIT_X + 2 * WIDTH + 3, INIT_Y + 4);    //打印选择菜单
	printf("方向：↑↓←→");

	goto_pos(hOut, INIT_X + 2 * WIDTH + 3, INIT_Y + 6);
	printf("退出：Esc");

	//打印上横框
	for(i = 0; i < 2 * WIDTH; i += 2)
	{
		goto_pos(hOut, INIT_X + i, INIT_Y);
		printf("*");
	}
	//打印下横框
	for(i = 0; i < 2 * WIDTH; i += 2)
	{
		goto_pos(hOut, INIT_X + i, INIT_Y + HEIGHT -1);
		printf("*");
	}
	//打印左竖框
	for(i = 1; i < 20; i++)
	{
		goto_pos(hOut, INIT_X, INIT_Y + i);
		printf("*");
	}
	//打印右竖框
	for(i = 1; i < 20; i++)
	{
		goto_pos(hOut, INIT_X + 2 * WIDTH - 2, INIT_Y + i);
		printf("*");
	}
}

/*初始化蛇*/
pSNAKE init_snake()
{
    pSNAKE pHead = (pSNAKE)malloc(sizeof(SNAKE));
    srand((unsigned)(time(NULL)));

    if (pHead == NULL)
    {
        printf("内存分配失败！\n");
        exit(-1);
    }

	//蛇不能在边框上
    pHead -> x = (rand() % (WIDTH - 2)) * 2 + 2;
    pHead -> y = rand() % (HEIGHT - 2) + 1;
    pHead -> next = NULL;
    pHead -> pre = NULL;

    return pHead;
}

/*初始化食物*/
FOOD init_food(HANDLE hOut, pSNAKE pHead)
{
    FOOD food;
    pSNAKE pSnake = pHead;
    srand((unsigned)time(NULL) + 1);

	while (1)
	{
		food.x = (rand() % (WIDTH - 2)) * 2 + 2;
    	food.y = rand() % (HEIGHT - 2) + 1;

		//如果食物在蛇身上
	    while (pSnake != NULL)
	    {
	    	if (food.x == pSnake -> x && food.y == pSnake -> y) break;
			pSnake = pSnake -> next;
		}
		
		if (pSnake == NULL) break;
	}
    
	goto_pos(hOut, food.x, food.y);
	printf("■");

    return food;
}

/*吃食物*/
void eat_food(HANDLE hOut, pSNAKE pHead, pFOOD pFood)
{
	//如果蛇头的位置与食物的位置相同
	if (pHead -> x == pFood -> x && pHead -> y == pFood -> y)
	{
		pSNAKE pEnd = (pSNAKE)malloc(sizeof(SNAKE));
		pSNAKE pSnake = pHead;

		if (pEnd == NULL)
		{
			printf("内存分配失败！\n");
            exit(-1);
		}

		//指向蛇尾
		while (pSnake -> next != NULL) pSnake = pSnake -> next;

		//在蛇尾插入新节点
		pEnd -> pre = pSnake;
		pEnd -> next = NULL;
		pEnd -> x = pSnake -> x;
		pEnd -> y = pSnake -> y;
		pSnake -> next = pEnd;
	}
}

/*按键*/
char press_keyboard(char c)
{
	char key;
	if (kbhit())
	{
		key = getch();
	
		if (c != 80 && key== 72) c = key;          //向上移动
		else if (c != 72 && key == 80) c = key;    //向下移动
		else if (c != 77 && key == 75) c = key;    //向左移动
		else if (c != 75 && key == 77) c = key;    //向右移动
		else if (key == 27) c = key;               //按 ESC 键退出
	}

	return c;
}

/*移动蛇*/
int move_snake(HANDLE hOut, pSNAKE pHead, pFOOD pFood, char c)
{
	int game_over = 0;

	pSNAKE pSnake = pHead;
	int end_x = 0, end_y = 0, pre_x = 1, pre_y = 1;

	//指向蛇尾
	while (pSnake -> next != NULL) pSnake = pSnake -> next;
	
	//如果最后一个节点不是头节点
	if (pSnake != pHead)
	{
		end_x = pSnake -> x;
		end_y = pSnake -> y;
		pre_x = pSnake -> pre -> x;
		pre_y = pSnake -> pre -> y;
	}
	
	//清除蛇尾
	goto_pos(hOut, pSnake -> x, pSnake -> y);
	printf("  ");

	//从蛇尾向蛇头前进
	while (pSnake != pHead)
	{
		pSnake -> x = pSnake -> pre -> x;
		pSnake -> y = pSnake -> pre -> y;
		pSnake = pSnake -> pre;
	}

	switch(c)
	{
		case 72:    //向上移动
			pHead -> y -= 1;
			if (pHead -> y <= 0) pHead -> y += (HEIGHT - 2);
			break;
		case 80:    //向下移动
			pHead -> y += 1;
			if (pHead -> y >= HEIGHT - 1) pHead -> y -= (HEIGHT - 2);
			break;
		case 75:    //向左移动
			pHead -> x -= 2;
			if (pHead -> x <= 0) pHead -> x += (WIDTH * 2 - 4);
			break;
		case 77:    //向右移动
			pHead -> x += 2;
			if (pHead -> x >= WIDTH * 2 - 2) pHead -> x -= (WIDTH * 2 -4);
			break;
	}
	
	//如果最后一个节点的坐标与其前一个节点坐标相同，则说明刚吃完食物，初始化一个食物
	if (end_x == pre_x && end_y == pre_y)
	{
		*pFood = init_food(hOut, pHead);
	}

	//当蛇头碰到蛇身时游戏结束
	pSnake = pHead -> next;
	while (pSnake != NULL)
	{
		if (pSnake -> x == pHead  -> x && pSnake -> y == pHead -> y) game_over = 1;
        pSnake = pSnake -> next;
	}

	return game_over;
}

/*打印蛇*/
void print_snake(HANDLE hOut, pSNAKE pHead)
{
	pSNAKE pSnake = pHead;
	while (pSnake != NULL)
	{
		goto_pos(hOut, pSnake -> x, pSnake -> y);
		if (pSnake == pHead) printf("■");
		else printf("□");
		pSnake = pSnake -> next;
	}
}

/*释放内存*/
void free_memory(pSNAKE *pHead)
{
    pSNAKE pDelete = NULL, pSnake = NULL;

    while (*pHead != NULL)
    {
        pSnake = (*pHead) -> next;
        if (pSnake != NULL)
        {
            pSnake -> pre = NULL;
        }
        pDelete = *pHead;
        free(pDelete);
        pDelete = NULL;
        *pHead = pSnake;
    }
}

int main()
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); //定义显示器句柄变量
	int game_over = 0;
	char c = 77, key;    //初始化运行方向向右

    init_frame();                          //初始化游戏窗口
    pSNAKE pHead = init_snake();           //初始化蛇
    FOOD food = init_food(hOut, pHead);    //随机产生一个食物

    while (1)
    {
    	eat_food(hOut, pHead, &food);
    	
    	key = press_keyboard(c);
    	if (key == 27)
    	{
    		game_over = 1;
    		break;
		}
		else c = key;

		game_over = move_snake(hOut, pHead, &food, key);
		if (game_over) break;

		print_snake(hOut, pHead);
		
		Sleep(200);
	}

	if (game_over)
    {
    	system("cls");
        printf("游戏结束！\n");

        free_memory(&pHead);
        if (pHead == NULL)
            printf("内存释放成功！\n");
        else
            printf("内存释放失败！\n");
    }

    getch();
    return 0;
}

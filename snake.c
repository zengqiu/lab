#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define INIT_X 0     //��Ϸ�������Ͻǵ�X������
#define INIT_Y 0     //��Ϸ�������Ͻǵ�Y������
#define HEIGHT 20    //��Ϸ���ڵĸ߶�
#define WIDTH 20     //��Ϸ���ڵĿ��

/*�߽ڵ�*/
typedef struct Snake
{
    int x;
    int y;
    struct Snake *pre;
    struct Snake *next;
}SNAKE, *pSNAKE;

/*ʳ��*/
typedef struct Food
{
    int x;
    int y;
}FOOD, *pFOOD;

/*�����ı�Ϊ��ɫ*/
void set_color_green(void)
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Handle, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
}

/*�����ı�Ϊ��ɫ*/
void set_color_red(void)
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
}

/*����Ƶ�ָ��λ��*/
void goto_pos(HANDLE hOut, int x, int y)
{
    COORD pos;
    pos.X = x;    //������
    pos.Y = y;    //������
    SetConsoleCursorPosition(hOut, pos);
}

/*��ʼ����Ϸ����**/
void init_frame()
{
	int i = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);    //������ʾ���������

	set_color_red();

	goto_pos(hOut, INIT_X + 2 * WIDTH + 6, INIT_Y + 2);    //��ӡ��Ϸ����
	printf("̰������Ϸ");

	set_color_green();

	goto_pos(hOut, INIT_X + 2 * WIDTH + 3, INIT_Y + 4);    //��ӡѡ��˵�
	printf("���򣺡�������");

	goto_pos(hOut, INIT_X + 2 * WIDTH + 3, INIT_Y + 6);
	printf("�˳���Esc");

	//��ӡ�Ϻ��
	for(i = 0; i < 2 * WIDTH; i += 2)
	{
		goto_pos(hOut, INIT_X + i, INIT_Y);
		printf("*");
	}
	//��ӡ�º��
	for(i = 0; i < 2 * WIDTH; i += 2)
	{
		goto_pos(hOut, INIT_X + i, INIT_Y + HEIGHT -1);
		printf("*");
	}
	//��ӡ������
	for(i = 1; i < 20; i++)
	{
		goto_pos(hOut, INIT_X, INIT_Y + i);
		printf("*");
	}
	//��ӡ������
	for(i = 1; i < 20; i++)
	{
		goto_pos(hOut, INIT_X + 2 * WIDTH - 2, INIT_Y + i);
		printf("*");
	}
}

/*��ʼ����*/
pSNAKE init_snake()
{
    pSNAKE pHead = (pSNAKE)malloc(sizeof(SNAKE));
    srand((unsigned)(time(NULL)));

    if (pHead == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(-1);
    }

	//�߲����ڱ߿���
    pHead -> x = (rand() % (WIDTH - 2)) * 2 + 2;
    pHead -> y = rand() % (HEIGHT - 2) + 1;
    pHead -> next = NULL;
    pHead -> pre = NULL;

    return pHead;
}

/*��ʼ��ʳ��*/
FOOD init_food(HANDLE hOut, pSNAKE pHead)
{
    FOOD food;
    pSNAKE pSnake = pHead;
    srand((unsigned)time(NULL) + 1);

	while (1)
	{
		food.x = (rand() % (WIDTH - 2)) * 2 + 2;
    	food.y = rand() % (HEIGHT - 2) + 1;

		//���ʳ����������
	    while (pSnake != NULL)
	    {
	    	if (food.x == pSnake -> x && food.y == pSnake -> y) break;
			pSnake = pSnake -> next;
		}
		
		if (pSnake == NULL) break;
	}
    
	goto_pos(hOut, food.x, food.y);
	printf("��");

    return food;
}

/*��ʳ��*/
void eat_food(HANDLE hOut, pSNAKE pHead, pFOOD pFood)
{
	//�����ͷ��λ����ʳ���λ����ͬ
	if (pHead -> x == pFood -> x && pHead -> y == pFood -> y)
	{
		pSNAKE pEnd = (pSNAKE)malloc(sizeof(SNAKE));
		pSNAKE pSnake = pHead;

		if (pEnd == NULL)
		{
			printf("�ڴ����ʧ�ܣ�\n");
            exit(-1);
		}

		//ָ����β
		while (pSnake -> next != NULL) pSnake = pSnake -> next;

		//����β�����½ڵ�
		pEnd -> pre = pSnake;
		pEnd -> next = NULL;
		pEnd -> x = pSnake -> x;
		pEnd -> y = pSnake -> y;
		pSnake -> next = pEnd;
	}
}

/*����*/
char press_keyboard(char c)
{
	char key;
	if (kbhit())
	{
		key = getch();
	
		if (c != 80 && key== 72) c = key;          //�����ƶ�
		else if (c != 72 && key == 80) c = key;    //�����ƶ�
		else if (c != 77 && key == 75) c = key;    //�����ƶ�
		else if (c != 75 && key == 77) c = key;    //�����ƶ�
		else if (key == 27) c = key;               //�� ESC ���˳�
	}

	return c;
}

/*�ƶ���*/
int move_snake(HANDLE hOut, pSNAKE pHead, pFOOD pFood, char c)
{
	int game_over = 0;

	pSNAKE pSnake = pHead;
	int end_x = 0, end_y = 0, pre_x = 1, pre_y = 1;

	//ָ����β
	while (pSnake -> next != NULL) pSnake = pSnake -> next;
	
	//������һ���ڵ㲻��ͷ�ڵ�
	if (pSnake != pHead)
	{
		end_x = pSnake -> x;
		end_y = pSnake -> y;
		pre_x = pSnake -> pre -> x;
		pre_y = pSnake -> pre -> y;
	}
	
	//�����β
	goto_pos(hOut, pSnake -> x, pSnake -> y);
	printf("  ");

	//����β����ͷǰ��
	while (pSnake != pHead)
	{
		pSnake -> x = pSnake -> pre -> x;
		pSnake -> y = pSnake -> pre -> y;
		pSnake = pSnake -> pre;
	}

	switch(c)
	{
		case 72:    //�����ƶ�
			pHead -> y -= 1;
			if (pHead -> y <= 0) pHead -> y += (HEIGHT - 2);
			break;
		case 80:    //�����ƶ�
			pHead -> y += 1;
			if (pHead -> y >= HEIGHT - 1) pHead -> y -= (HEIGHT - 2);
			break;
		case 75:    //�����ƶ�
			pHead -> x -= 2;
			if (pHead -> x <= 0) pHead -> x += (WIDTH * 2 - 4);
			break;
		case 77:    //�����ƶ�
			pHead -> x += 2;
			if (pHead -> x >= WIDTH * 2 - 2) pHead -> x -= (WIDTH * 2 -4);
			break;
	}
	
	//������һ���ڵ����������ǰһ���ڵ�������ͬ����˵���ճ���ʳ���ʼ��һ��ʳ��
	if (end_x == pre_x && end_y == pre_y)
	{
		*pFood = init_food(hOut, pHead);
	}

	//����ͷ��������ʱ��Ϸ����
	pSnake = pHead -> next;
	while (pSnake != NULL)
	{
		if (pSnake -> x == pHead  -> x && pSnake -> y == pHead -> y) game_over = 1;
        pSnake = pSnake -> next;
	}

	return game_over;
}

/*��ӡ��*/
void print_snake(HANDLE hOut, pSNAKE pHead)
{
	pSNAKE pSnake = pHead;
	while (pSnake != NULL)
	{
		goto_pos(hOut, pSnake -> x, pSnake -> y);
		if (pSnake == pHead) printf("��");
		else printf("��");
		pSnake = pSnake -> next;
	}
}

/*�ͷ��ڴ�*/
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
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); //������ʾ���������
	int game_over = 0;
	char c = 77, key;    //��ʼ�����з�������

    init_frame();                          //��ʼ����Ϸ����
    pSNAKE pHead = init_snake();           //��ʼ����
    FOOD food = init_food(hOut, pHead);    //�������һ��ʳ��

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
        printf("��Ϸ������\n");

        free_memory(&pHead);
        if (pHead == NULL)
            printf("�ڴ��ͷųɹ���\n");
        else
            printf("�ڴ��ͷ�ʧ�ܣ�\n");
    }

    getch();
    return 0;
}

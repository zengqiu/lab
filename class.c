//author: zengqiu

/*
class.txt ���� ���� ��ʼʱ�� ����ʱ�� �ص�
1 ���� 14:00 14:50 ��A401
1 ��ʷ 15:00 15:50 ��A501
1 ���� 16:00 16:50 ��A601
1 Ӣ�� 19:00 19:50 ��A701
1 ���� 20:00 20:50 ��A801
1 ��· 21:00 21:50 ��A901
2 ���� 08:00 08:50 ��B001
2 ���� 09:00 09:50 ��B101
2 ��ѧ 10:10 11:00 ��B201
2 ���� 11:10 12:00 ��B301
2 ���� 14:00 14:50 ��B401
2 ��ʷ 15:00 15:50 ��B501
2 ���� 16:00 16:50 ��B601
2 Ӣ�� 19:00 19:50 ��B701
2 ���� 20:00 20:50 ��B801
2 ��· 21:00 21:50 ��B901
3 ���� 08:00 08:50 ��C001
3 ���� 09:00 09:50 ��C101
3 ��ѧ 10:10 11:00 ��C201
3 ���� 11:10 12:00 ��C301
3 ���� 14:00 14:50 ��C401
3 ��ʷ 15:00 15:50 ��C501
3 ���� 16:00 16:50 ��C601
3 Ӣ�� 19:00 19:50 ��C701
3 ���� 20:00 20:50 ��C801
3 ��· 21:00 21:50 ��C901
4 ���� 08:00 08:50 ��D001
4 ���� 09:00 09:50 ��D101
4 ��ѧ 10:10 11:00 ��D201
4 ���� 11:10 12:00 ��D301
4 ���� 14:00 14:50 ��D401
4 ��ʷ 15:00 15:50 ��D501
4 ���� 16:00 16:50 ��D601
4 Ӣ�� 19:00 19:50 ��D701
4 ���� 20:00 20:50 ��D801
4 ��· 21:00 21:50 ��D901
5 ���� 08:00 08:50 ��E001
5 ���� 09:00 09:50 ��E101
5 ��ѧ 10:10 11:00 ��E201
5 ���� 11:10 12:00 ��E301
5 ���� 14:00 14:50 ��E401
5 ��ʷ 15:00 15:50 ��E501
5 ���� 16:00 16:50 ��E601
5 Ӣ�� 19:00 19:50 ��E701
5 ���� 20:00 20:50 ��E801
5 ��· 21:00 21:50 ��E901
6 ���� 08:00 08:50 ��F001
6 ���� 09:00 09:50 ��F101
6 ��ѧ 10:10 11:00 ��F201
6 ���� 11:10 12:00 ��F301
6 ���� 14:00 14:50 ��F401
6 ��ʷ 15:00 15:50 ��F501
6 ���� 16:00 16:50 ��F601
6 Ӣ�� 19:00 19:50 ��F701
6 ���� 20:00 20:50 ��F801
6 ��· 21:00 21:50 ��F901
7 ���� 08:00 08:50 ��G001
7 ���� 09:00 09:50 ��G101
7 ��ѧ 10:10 11:00 ��G201
7 ���� 11:10 12:00 ��G301
7 ���� 14:00 14:50 ��G401
7 ��ʷ 15:00 15:50 ��G501
7 ���� 16:00 16:50 ��G601
7 Ӣ�� 19:00 19:50 ��G701
7 ���� 20:00 20:50 ��G801
7 ��· 21:00 21:50 ��G901
*/

#include <stdio.h>
#include <windows.h>

int today_flag = 0;    //���տγ�״̬��ʶ��0���γ�δȫ��������1���γ�ȫ������

char result_top[30];       //�γ�ʱ��/���տγ�״̬
char result_bottom[30];    //�γ����ƺ͵�ַ/���տγ�״̬

/*�γ���Ϣ*/
typedef struct ClassInfo
{
    int weekday;
    char name[20];
    char time_begin[6];
    char time_end[6];
    char address[20];
}CLASSINFO;

/*�γ̽ڵ�*/
typedef struct Node
{
    CLASSINFO classinfo;
    struct Node *next;
}NODE, *pNODE;

/*��ʼ������ڵ�*/
pNODE init_node()
{
    pNODE pNode = (pNODE)malloc(sizeof(NODE));

    if (pNode == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(-1);
    }

    pNode->next = NULL;

    return pNode;
}

/*���ļ��е���������һ������*/
void openfile(pNODE pHead)
{
    FILE *fp;
    CLASSINFO classinfo;
    pNODE pNode, pNew;
    pNode = pHead;

    if((fp=fopen("class.txt", "rb")) == NULL) fprintf(stdout, "Open file failed.");
    else
    {
        while(fscanf(fp, "%d %s %s %s %s", &classinfo.weekday, classinfo.name, classinfo.time_begin, classinfo.time_end, classinfo.address) != EOF)
        {
            pNew = init_node();
            pNew->classinfo = classinfo;
            pNode->next = pNew;
            pNode = pNew;
        }
        fclose(fp);
    }
}

/*��ӡ����*/
void print_list(pNODE pHead)
{
    pNODE pNode;
    pNode = pHead;
    pNode = pNode->next;

    while (pNode != NULL)
    {
        printf("���ڣ�%d ���ƣ�%s ��ʼ��%s ������%s ��ַ��%s\n", pNode->classinfo.weekday, pNode->classinfo.name, pNode->classinfo.time_begin, pNode->classinfo.time_end, pNode->classinfo.address);
        pNode = pNode->next;
    }
}

/*��ȡ��ǰʱ��γ���Ϣ*/
void get_class_info(int weekday, int hour, int min, pNODE pHead)
{
    pNODE pNode;
    pNode = pHead;
    pNode = pHead->next;
    
    int count = 0;    //�����жϸÿγ��Ƿ��ǽ����һ�ڿ�

    char time_now[20] = "";
    sprintf(time_now, "%02d:%02d", hour, min);

    while (pNode != NULL)
    {
        //�����п�
        if (weekday == pNode->classinfo.weekday)
        {
            //��ǰʱ��С�ڵ�һ�ڿε�ʱ�䣬����ʾ��һ�ڿ���Ϣ��time_now <= pHead->next->classinfo.time_begin
            if (count == 0 && (strcmp(time_now, pNode->classinfo.time_begin) < 0 || strcmp(time_now, pNode->classinfo.time_begin) == 0))
            {
                //pHead->next->classinfo.*
                sprintf(result_top, "%s-%s", pNode->classinfo.time_begin, pNode->classinfo.time_end);
                sprintf(result_bottom, "%s %s", pNode->classinfo.name, pNode->classinfo.address);
                
                return;
            }

            //��ǰʱ����ڿγ̿�ʼʱ����С�ڿγ̽���ʱ�䣬����ʾ�ÿ���Ϣ��time_now >= pNode->classinfo.time_begin && time_now <= pNode->classinfo.time_end
            if ((strcmp(time_now, pNode->classinfo.time_begin) > 0 || strcmp(time_now, pNode->classinfo.time_begin) == 0) && (strcmp(time_now, pNode->classinfo.time_end) < 0 || strcmp(time_now, pNode->classinfo.time_end) == 0))
            {
                //pNode->classinfo.*
                sprintf(result_top, "%s-%s", pNode->classinfo.time_begin, pNode->classinfo.time_end);
                sprintf(result_bottom, "%s %s", pNode->classinfo.name, pNode->classinfo.address);
                
                return;
            }

            //�½ڿβ�Ϊ���ҵ�ǰʱ�������ڿν���ʱ��С���½ڿο�ʼʱ�䣬����ʾ�½ڿογ���Ϣ��pNode->next != NULL && time_now > pNode->classinfo.time_end && time_now < pNode->next->classinfo.time_begin
            if (pNode->next != NULL && strcmp(time_now, pNode->classinfo.time_end) > 0 && strcmp(time_now, pNode->next->classinfo.time_begin) < 0)
            {
                //pNode->next->classinfo.*
                sprintf(result_top, "%s-%s", pNode->next->classinfo.time_begin, pNode->next->classinfo.time_end);
                sprintf(result_bottom, "%s %s", pNode->next->classinfo.name, pNode->next->classinfo.address);
                
                return;
            }

            //��ǰʱ��������һ�ڿΣ�pNode->next == NULL && time_now > pNode->classinfo.time_end
            if (pNode->next->classinfo.weekday != weekday && strcmp(time_now, pNode->classinfo.time_end) > 0)
            {
                //���տγ̽���
                today_flag = 1;
                break;
            }
            
            count += 1;
        }

        pNode = pNode->next;
    }

    if (today_flag)
    {
        //���տγ̽���
        sprintf(result_top, "���տγ̽���");
    }
    
    if (pNode == NULL)
    {
        //����û��
        sprintf(result_top, "����û��");
    }

    //���տγ���Ϣ�ж�
    pNode = pHead->next;
    int tomorrow = (weekday + 1 > 7) ? 1 : (weekday + 1);
    while (pNode != NULL)
    {
        if (tomorrow == pNode->classinfo.weekday)
        {
            //�����п�
            sprintf(result_bottom, "�����п�");
            return;
        }

        pNode = pNode->next;
    }

    if (pNode == NULL)
    {
        //����û��
        sprintf(result_bottom, "����û��");
        return;
    }
}

int main()
{
    pNODE pHead = init_node();
    openfile(pHead);
    print_list(pHead);

    get_class_info(3, 10, 55, pHead);

    printf("%s\n", result_top);
    printf("%s\n", result_bottom);

    return 0;
}
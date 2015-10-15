//author: zengqiu

/*
class.txt 星期 名称 开始时间 结束时间 地点
1 地理 14:00 14:50 南A401
1 历史 15:00 15:50 南A501
1 政治 16:00 16:50 南A601
1 英语 19:00 19:50 南A701
1 体育 20:00 20:50 南A801
1 电路 21:00 21:50 北A901
2 高数 08:00 08:50 南B001
2 物理 09:00 09:50 南B101
2 化学 10:10 11:00 南B201
2 生物 11:10 12:00 南B301
2 地理 14:00 14:50 南B401
2 历史 15:00 15:50 南B501
2 政治 16:00 16:50 南B601
2 英语 19:00 19:50 南B701
2 体育 20:00 20:50 南B801
2 电路 21:00 21:50 北B901
3 高数 08:00 08:50 南C001
3 物理 09:00 09:50 南C101
3 化学 10:10 11:00 南C201
3 生物 11:10 12:00 南C301
3 地理 14:00 14:50 南C401
3 历史 15:00 15:50 南C501
3 政治 16:00 16:50 南C601
3 英语 19:00 19:50 南C701
3 体育 20:00 20:50 南C801
3 电路 21:00 21:50 北C901
4 高数 08:00 08:50 南D001
4 物理 09:00 09:50 南D101
4 化学 10:10 11:00 南D201
4 生物 11:10 12:00 南D301
4 地理 14:00 14:50 南D401
4 历史 15:00 15:50 南D501
4 政治 16:00 16:50 南D601
4 英语 19:00 19:50 南D701
4 体育 20:00 20:50 南D801
4 电路 21:00 21:50 北D901
5 高数 08:00 08:50 南E001
5 物理 09:00 09:50 南E101
5 化学 10:10 11:00 南E201
5 生物 11:10 12:00 南E301
5 地理 14:00 14:50 南E401
5 历史 15:00 15:50 南E501
5 政治 16:00 16:50 南E601
5 英语 19:00 19:50 南E701
5 体育 20:00 20:50 南E801
5 电路 21:00 21:50 北E901
6 高数 08:00 08:50 南F001
6 物理 09:00 09:50 南F101
6 化学 10:10 11:00 南F201
6 生物 11:10 12:00 南F301
6 地理 14:00 14:50 南F401
6 历史 15:00 15:50 南F501
6 政治 16:00 16:50 南F601
6 英语 19:00 19:50 南F701
6 体育 20:00 20:50 南F801
6 电路 21:00 21:50 北F901
7 高数 08:00 08:50 南G001
7 物理 09:00 09:50 南G101
7 化学 10:10 11:00 南G201
7 生物 11:10 12:00 南G301
7 地理 14:00 14:50 南G401
7 历史 15:00 15:50 南G501
7 政治 16:00 16:50 南G601
7 英语 19:00 19:50 南G701
7 体育 20:00 20:50 南G801
7 电路 21:00 21:50 北G901
*/

#include <stdio.h>
#include <windows.h>

int today_flag = 0;    //今日课程状态标识，0：课程未全部结束；1：课程全部结束

char result_top[30];       //课程时间/今日课程状态
char result_bottom[30];    //课程名称和地址/明日课程状态

/*课程信息*/
typedef struct ClassInfo
{
    int weekday;
    char name[20];
    char time_begin[6];
    char time_end[6];
    char address[20];
}CLASSINFO;

/*课程节点*/
typedef struct Node
{
    CLASSINFO classinfo;
    struct Node *next;
}NODE, *pNODE;

/*初始化链表节点*/
pNODE init_node()
{
    pNODE pNode = (pNODE)malloc(sizeof(NODE));

    if (pNode == NULL)
    {
        printf("内存分配失败！\n");
        exit(-1);
    }

    pNode->next = NULL;

    return pNode;
}

/*把文件中的内容做成一个链表*/
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

/*打印链表*/
void print_list(pNODE pHead)
{
    pNODE pNode;
    pNode = pHead;
    pNode = pNode->next;

    while (pNode != NULL)
    {
        printf("星期：%d 名称：%s 开始：%s 结束：%s 地址：%s\n", pNode->classinfo.weekday, pNode->classinfo.name, pNode->classinfo.time_begin, pNode->classinfo.time_end, pNode->classinfo.address);
        pNode = pNode->next;
    }
}

/*获取当前时间课程信息*/
void get_class_info(int weekday, int hour, int min, pNODE pHead)
{
    pNODE pNode;
    pNode = pHead;
    pNode = pHead->next;
    
    int count = 0;    //用于判断该课程是否是今天第一节课

    char time_now[20] = "";
    sprintf(time_now, "%02d:%02d", hour, min);

    while (pNode != NULL)
    {
        //今日有课
        if (weekday == pNode->classinfo.weekday)
        {
            //当前时间小于第一节课的时间，则显示第一节课信息：time_now <= pHead->next->classinfo.time_begin
            if (count == 0 && (strcmp(time_now, pNode->classinfo.time_begin) < 0 || strcmp(time_now, pNode->classinfo.time_begin) == 0))
            {
                //pHead->next->classinfo.*
                sprintf(result_top, "%s-%s", pNode->classinfo.time_begin, pNode->classinfo.time_end);
                sprintf(result_bottom, "%s %s", pNode->classinfo.name, pNode->classinfo.address);
                
                return;
            }

            //当前时间大于课程开始时间且小于课程结束时间，则显示该课信息：time_now >= pNode->classinfo.time_begin && time_now <= pNode->classinfo.time_end
            if ((strcmp(time_now, pNode->classinfo.time_begin) > 0 || strcmp(time_now, pNode->classinfo.time_begin) == 0) && (strcmp(time_now, pNode->classinfo.time_end) < 0 || strcmp(time_now, pNode->classinfo.time_end) == 0))
            {
                //pNode->classinfo.*
                sprintf(result_top, "%s-%s", pNode->classinfo.time_begin, pNode->classinfo.time_end);
                sprintf(result_bottom, "%s %s", pNode->classinfo.name, pNode->classinfo.address);
                
                return;
            }

            //下节课不为空且当前时间大于这节课结束时间小于下节课开始时间，则显示下节课课程信息：pNode->next != NULL && time_now > pNode->classinfo.time_end && time_now < pNode->next->classinfo.time_begin
            if (pNode->next != NULL && strcmp(time_now, pNode->classinfo.time_end) > 0 && strcmp(time_now, pNode->next->classinfo.time_begin) < 0)
            {
                //pNode->next->classinfo.*
                sprintf(result_top, "%s-%s", pNode->next->classinfo.time_begin, pNode->next->classinfo.time_end);
                sprintf(result_bottom, "%s %s", pNode->next->classinfo.name, pNode->next->classinfo.address);
                
                return;
            }

            //当前时间大于最后一节课：pNode->next == NULL && time_now > pNode->classinfo.time_end
            if (pNode->next->classinfo.weekday != weekday && strcmp(time_now, pNode->classinfo.time_end) > 0)
            {
                //今日课程结束
                today_flag = 1;
                break;
            }
            
            count += 1;
        }

        pNode = pNode->next;
    }

    if (today_flag)
    {
        //今日课程结束
        sprintf(result_top, "今日课程结束");
    }
    
    if (pNode == NULL)
    {
        //今日没课
        sprintf(result_top, "今日没课");
    }

    //明日课程信息判断
    pNode = pHead->next;
    int tomorrow = (weekday + 1 > 7) ? 1 : (weekday + 1);
    while (pNode != NULL)
    {
        if (tomorrow == pNode->classinfo.weekday)
        {
            //明日有课
            sprintf(result_bottom, "明日有课");
            return;
        }

        pNode = pNode->next;
    }

    if (pNode == NULL)
    {
        //明日没课
        sprintf(result_bottom, "明日没课");
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
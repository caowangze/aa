#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFESIBLE -1
#define OVERFLOW -2

struct PCB{
    char name[20];       //进程名	
    int Arrive_Time;     //到达时间	
    int Serve_Time;      //服务时间	
  //int Finish_Time;     //完成时间	
    int priority;        //优先级
};

typedef struct LNode            //链表结点定义
{	
     PCB data;
     struct LNode *next;
}LNode,*LinkList;
typedef struct Queue         //反馈队列结点定义，实质上还是一个单链表
{
   PCB data;                 	
   int prio;                  //反馈队列的优先级	
   int timeblock;             //反馈队列分配的时间片	
   struct Queue *next;        //指向下一个反馈队列的指针
}Queue,*LinkQueue;


int Create(LinkList &L)           //利用尾插法创建进程
{
   LinkList p,r;	
   int i,n;	
   L=(LinkList)malloc(sizeof(LNode));	
   L->next=NULL;	
   r=L;	
   printf("请输入进程的个数：\n");	
   scanf("%d",&n);	
   if(n<=0)	
   {		
       printf("请输入正整数！\n");		
       return ERROR;
   }	
   printf("请依次输入进程名、到达时间、服务时间、优先级\n");
   for(i=0;i<n;i++)
   {		
       p=(LinkList)malloc(sizeof(LNode));
       scanf("%s",&(p->data.name));	
       scanf("%d",&(p->data.Arrive_Time));	
       scanf("%d",&(p->data.Serve_Time));	
      //scanf("%d",&(p->data.Finish_Time));	
       scanf("%d",&(p->data.priority));	
       r->next=p;	
       r=p;
    }
    r->next=NULL;
    return OK;
}

int Show(LinkList L)           //输出当前队列中的进程的信息
{
    LinkList p;
    p=L->next;
    if(!p)
    {		
        printf("为空！\n");	
	return ERROR;
    }
    while(p)
    {	
	printf("进程名：%s  到达时间：%d  服务时间：%d  优先级：%d\n",p->data.name,p-     >data.Arrive_Time,p->data.Serve_Time,p->data.priority);	
	p=p->next;
    }
    return OK;
}


void Sort(LinkList &L)   //按照到达时间先后排序
{	
      LinkList p,q;
      PCB temp;
      p=L->next;	
      q=L->next;
      while((p!=NULL)&&(p->next!=NULL))//当创建的队列中有两个及以上队列的时候，利用冒泡排序法按照到达时间先后进行排序
	{	
	    for(p=L->next;p->next!=NULL;p=p->next)	
	    {		
	        for(q=L->next;q->next!=NULL;q=q->next)		
	       {			
	           if(q->data.Arrive_Time>q->next->data.Arrive_Time)
                   {				    		
                       temp=q->data;				
                       q->data=q->next->data;			
                       q->next>data=temp;		
		    }		
	        }	
	     }	
          }
}


int getCount(LinkList &L,int time)      //查看当前就绪队列中的进程数，其中time是宏观运行时间
{
	int count=0;
	LinkList q;
	q=L->next;
	while(q!=NULL&&q->data.Arrive_Time<=time)
	{	
        	count++;
		q=q->next;
	}
	return count;
}

void Delet(LinkList &L,LinkList p)   //删除结点函数,删除p的下一个结点
{	
    LinkList q;	
    q=p->next;	
    p->next=q->next;	
    free(q);
}


/*------------------------------先来先服务算法-----------------------------------------*/
void FCFS(LinkList &L)
{
	LinkList p,q;
	int time=0,count;
	while(L->next!=NULL)
	{	
	    p=L->next;
	    count=getCount(L,time);	
            if(count=0)		
        	time++;	
       	    else	
	   {		
        	printf("\n");	
		q=L;		
        	printf("进程%s正在执行……\n",p->data.name);
	        printf("进程%s开始执行的时间为：%d\n",p->data.name,time);	
		time+=p->data.Serve_Time;	
		printf("进程%s完成的时间为：%d\n",p->data.name,time);
		printf("进程%s的周转时间为：%d\n",p->data.name,time-p->data.Arrive_Time);
		Delet(L,q);		
	        printf("\n");	
          }	
       }
}

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
typedef struct node  
{  
char name[20];   
int prio;   
int round;   
int cputime;   
int needtime;   
char state;    
int count;    
struct node *next;  
}PCB;  
typedef struct Queue 
{  
PCB *LinkPCB;   
int prio;     
int round;   
struct Queue *next; 
}ReadyQueue;  
PCB *run=NULL,*finish=NULL;  
ReadyQueue *Head = NULL;
int num;     
int ReadyNum;    
void Output();        
void InsertFinish(PCB *in);      
void InsertPrio(ReadyQueue *in);    
void PrioCreate();      
void GetFirst(ReadyQueue *queue);    
void InsertLast(PCB *in,ReadyQueue *queue);  
void ProcessCreate();       
void RoundRun(ReadyQueue *timechip);   
void MultiDispatch();        
 
int main(void)  
{  
PrioCreate();
ProcessCreate(); 
MultiDispatch(); 
Output();  
return 0;  
}  
void Output() 
{  
ReadyQueue *print = Head;  
PCB *p;  
printf("Process name /  priority /  rounds / tcpu time /  required time /  process status /  counter / n");  
while(print)  
{  
  if(print ->LinkPCB != NULL)  
  {  
   p=print ->LinkPCB;  
   while(p)  
   {  
    printf("%s/ %d/ %d/ %d/ %d/ / %c/ / %d/n",p->name,p->prio,p->round,p->cputime,p->needtime,p->state,p->count);     
    p = p->next;  
   }  
  }  
  print = print->next;  
}  
p = finish;  
while(p!=NULL)  
{  
  printf("%s/ %d/ %d/ %d/ %d/ / %c/ / %d/n",p->count);  
  p = p->next;  
}  
p = run;  
while(p!=NULL)  
{  
  printf("%s/ %d/ %d/ %d/ %d/ / %c/ / %d/n",p->count);  
  p = p->next;  
}  
 
 
}  
void InsertFinish(PCB *in)  /*将进程插入到完成队列尾部*/  
{  
PCB *fst;  
fst = finish;  
 
if(finish == NULL)  
{  
  in->next = finish;  
  finish = in;  
}  
else  
{  
  while(fst->next != NULL)  
  {  
   fst = fst->next;  
  }  
  in ->next = fst ->next;  
  fst ->next = in;  
}  
}  
void InsertPrio(ReadyQueue *in)  /*创建就绪队列，规定优先数越小，优先级越低*/  
{  
ReadyQueue *fst,*nxt;  
fst = nxt = Head;  
 
if(Head == NULL)    /*如果没有队列，则为第一个元素*/  
{  
  in->next = Head;  
  Head = in;  
}  
else       /*查到合适的位置进行插入*/  
{  
  if(in ->prio >= fst ->prio)  /*比第一个还要大，则插入到队头*/  
  {  
   in->next = Head;  
   Head = in;  
  }  
  else  
  {  
   while(fst->next != NULL)  /*移动指针查找第一个别它小的元素的位置进行插入*/  
   {  
    nxt = fst;  
    fst = fst->next;  
   }  
 
   if(fst ->next == NULL)  /*已经搜索到队尾，则其优先级数最小，将其插入到队尾即可*/  
   {  
    in ->next = fst ->next;  
    fst ->next = in;  
   }  
   else     /*插入到队列中*/  
   {  
    nxt = in;  
    in ->next = fst;  
   }  
  }  
}  
}  
void PrioCreate() /*创建就绪队列输入函数*/  
{  
ReadyQueue *tmp;  
int i;  
 
printf("Enter the number of ready queues：");  
scanf("%d",&ReadyNum);  
 
printf("Enter the CPU time slice of each ready queue：");  
for(i = 0;i < ReadyNum; i++)  
{  
  if((tmp = (ReadyQueue *)malloc(sizeof(ReadyQueue)))==NULL)  
  {  
   perror("malloc");  
   exit(1);  
  }  
  scanf("%d",&(tmp->round));  /*输入此就绪队列中给每个进程所分配的CPU时间片*/  
  tmp ->prio = 50 - tmp->round;  /*设置其优先级，时间片越高，其优先级越低*/  
  tmp ->LinkPCB = NULL;    /*初始化其连接的进程队列为空*/  
  tmp ->next = NULL;  
  InsertPrio(tmp);     /*按照优先级从高到低，建立多个就绪队列*/  
}  
}  
void GetFirst(ReadyQueue *queue)     /*取得某一个就绪队列中的队头进程*/  
{  
run = queue ->LinkPCB;  
 
if(queue ->LinkPCB != NULL)  
{  
  run ->state = 'R';  
  queue ->LinkPCB = queue ->LinkPCB ->next;  
  run ->next = NULL;  
}  
}  
void InsertLast(PCB *in,ReadyQueue *queue)  /*将进程插入到就绪队列尾部*/  
{  
PCB *fst;  
fst = queue->LinkPCB;  
 
if( queue->LinkPCB == NULL)  
{  
  in->next =  queue->LinkPCB;  
  queue->LinkPCB = in;  
}  
else  
{  
  while(fst->next != NULL)  
  {  
   fst = fst->next;  
  }  
  in ->next = fst ->next;  
  fst ->next = in;  
}  
}  
void ProcessCreate() /*进程创建函数*/  
{  
PCB *tmp;  
int i;  
 
printf("Enter the number of processes：");  
scanf("%d",&num);  
printf("Enter the process name and process time：");  
for(i = 0;i < num; i++)  
{  
  if((tmp = (PCB *)malloc(sizeof(PCB)))==NULL)  
  {  
   perror("malloc");  
   exit(1);  
  }  
  scanf("%s",tmp->name);  
  getchar();        /*吸收回车符号*/  
  scanf("%d",&(tmp->needtime));  
  tmp ->cputime = 0;  
  tmp ->state ='W';  
  tmp ->prio = 50 - tmp->needtime;  /*设置其优先级，需要的时间越多，优先级越低*/  
  tmp ->round = Head ->round;  
  tmp ->count = 0;  
  InsertLast(tmp,Head);      /*按照优先级从高到低，插入到就绪队列*/  
}  
}  
void RoundRun(ReadyQueue *timechip)    /*时间片轮转调度算法*/  
{  
 
int flag = 1;  
 
GetFirst(timechip);  
while(run != NULL)  
{  
  while(flag)  
  {  
   run->count++;  
   run->cputime++;  
   run->needtime--;  
   if(run->needtime == 0) /*进程执行完毕*/  
   {  
    run ->state = 'F';  
    InsertFinish(run);  
    flag = 0;  
   }  
   else if(run->count == timechip ->round)/*时间片用完*/  
   {  
    run->state = 'W';  
    run->count = 0;   /*计数器清零，为下次做准备*/  
    InsertLast(run,timechip);  
    flag = 0;  
   }  
  }  
  flag = 1;  
  GetFirst(timechip);  
}  
}  
void MultiDispatch()   /*多级调度算法，每次执行一个时间片*/  
{  
int flag = 1;  
int k = 0;  
 
ReadyQueue *point;  
point = Head;  
 
GetFirst(point);  
while(run != NULL)  
{  
  Output();  
  if(Head ->LinkPCB!=NULL)  
   point = Head;  
  while(flag)  
  {  
   run->count++;  
   run->cputime++;  
   run->needtime--;  
   if(run->needtime == 0) /*进程执行完毕*/  
   {  
    run ->state = 'F';  
    InsertFinish(run);  
    flag = 0;  
   }  
   else if(run->count == run->round)/*时间片用完*/  
   {  
    run->state = 'W';  
    run->count = 0;   /*计数器清零，为下次做准备*/  
    if(point ->next!=NULL)  
    {  
     run ->round = point->next ->round;/*设置其时间片是下一个就绪队列的时间片*/  
     InsertLast(run,point->next);  /*将进程插入到下一个就绪队列中*/  
     flag = 0;  
    }  
    else  
    {  
     RoundRun(point);   /*如果为最后一个就绪队列就调用时间片轮转算法*/  
     break;  
    }  
   }  
   ++k;  
   if(k == 3)  
   {  
    ProcessCreate();  
   }  
  }  
  flag = 1;  
  if(point ->LinkPCB == NULL)/*就绪队列指针下移*/  
   point =point->next;  
  if(point ->next ==NULL)  
  {  
   RoundRun(point);  
   break;  
  }  
  GetFirst(point);  
}  
}   

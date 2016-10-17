#include "csapp.h"
#include <stdlib.h>

int cnt_write;
sem_t* mutex;
sem_t *W,*R;

void* Writer(void* tid)
{
   // Pthread_detach(Pthread_self());
    P(mutex);
    if(cnt_write ++ == 0) /* first write */
        P(R);
    V(mutex);

    P(W);
    /* writing */
    Sleep(1);
    Write2File(*((int*)tid),1);

    P(mutex);
    if(--cnt_write ==0) {V(R);}
    V(W);
    V(mutex);
}

void* Reader(void* tid)
{
    //Pthread_detach(Pthread_self());
    int isfirstReader = 0;

    /* #region
    此段不能使用简单的P(mutex); V(mutex); P(R)
    这是为了防止此情况的出现: 在没有任何操作进行和等待的情况下,假设reader和writer先后到达.
    当reader在V(mutex)后 P(R)之前,若后到的writer在自己的mutex中抢先抢占了P(R),那么此时writer会先于reader执行.
    */
    P(mutex);
    if(cnt_write == 0)
    {
        P(R);
        isfirstReader = 1;
    }
    V(mutex);
    /*endregion*/
    if(!isfirstReader) {P(R);}
    /* Reading */
    Sleep(1);
    Write2File(*((int*)tid),2);
    V(R);
}

void Write2File(int tid,int type)
{
    char strW[30] = "Writer ";
    char strR[30] = "Reader ";
    char id[] = {tid +'0',0};
    char strD[] = " is done!\r\n";

    FILE* fd = fopen("/home/fei/git/ParallelComputing/P-C Model/test","a+");
    if (fd==0) { printf("can't open file\n"); return;}
    //fseek(fp,0,SEEK_END);
     char *test ;
    if(type == 1)       {test = strcat(strcat(strW,id),strD);}
    else if(type == 2)  {test = strcat(strcat(strR,id),strD);}
    fwrite(test,1,strlen(test),fd);
    fclose(fd);
    fflush(fd);
}

int main()
{
    mutex = (sem_t*)malloc(sizeof(sem_t));
    W =(sem_t*)malloc(sizeof(sem_t));
    R =(sem_t*)malloc(sizeof(sem_t));
    cnt_write = 0;
    Sem_init(mutex,0,1);
    Sem_init(W,0,1);
    Sem_init(R,0,1);

    Sleep(1);

    int i;
    pthread_t tid[8];
    int a[8] = {1,2,3,4,5,6,7,8};
    for(i=0;i<5;i++)
    {
        Pthread_create(&tid[i],NULL,Writer,&a[i]);
    }
    for(i=5;i<7;i++)
    {
        Pthread_create(&tid[i],NULL,Reader,&a[i]);
    }
    for(i=7;i<8;i++)
    {
        Pthread_create(&tid[i],NULL,Writer,&a[i]);
    }

    for(i=0;i<8;i++)
    {
        Pthread_join(tid[i],NULL);
    }
    return 0;
}




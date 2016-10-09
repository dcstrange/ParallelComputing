#include "csapp.h"

int cnt_write;
sem_t* mutex;
sem_t *W,*R;

int Run()
{
    cnt_write = 0;
    Sem_init(mutex,0,1);
    Sem_init(W,0,1);
    Sem_init(R,0,1);

    return 0;
}

void writer(void* tid)
{
    P(mutex);
    if(cnt_write ++ == 0) /* first write */
        P(R);
    V(mutex);

    P(W);
    /* writing */
    Sleep(1);
    printf("Writer %d is done!",*((int*)tid));

    P(mutex);
    if(--cnt_write ==0) {V(R);}
    V(W);
    V(mutex);
}

void reader(void* tid)
{
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
    printf("Reader %d is done!",*((int*)tid));
    V(R);
}


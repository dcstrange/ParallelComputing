#include "csapp.h"


#define N  5   /* The order of the matrix */
#define PN  2   /* The number of threads */

int _Num = 0;
int _MatrixIn[N][N],_MatrixOut[N][N];
pthread_t _Tidps[PN];
sem_t _Sem;

void* ThreadTask(void* tid)
{
    int mynum;
    int i,j;
    while(1)
    {
        P(&_Sem);
        mynum =_Num;
        _Num++;
        V(&_Sem);

        if(mynum < N*N)
        {
            i = mynum/N;
            j = mynum%N;
            Calculation(i,j);
        }
        else
        {
            printf("Thread %d will finish\r\n",(int)tid);
            return;
        }
    }
}

int main()
{

    InitMatrix();
    PrintMatrix(_MatrixIn);

    Sem_init(&_Sem,0,1);
    int i;
    for(i=0; i<PN; i++)
    {
        Pthread_create(&_Tidps[i],NULL,ThreadTask,NULL);
    }

    for(i=0; i<PN; i++)
    {
        Pthread_join(_Tidps[i],NULL);
    }

      PrintMatrix(_MatrixOut);
    exit(0);
}

void InitMatrix()
{
    int i,j;
    srand((unsigned) time(NULL)); //用时间做种，每次产生随机数不一样
    for(i=0; i<N; i++)
    {
        for(j=0; j<N; j++)
            _MatrixIn[i][j] = rand() % 50;
    }
}


int Calculation(int row, int column)
{
    int k,sum;
    sum = 0;
    for(k=0;k<N;k++)
    {
        sum += _MatrixIn[row][k]*_MatrixIn[k][column];
    }
    _MatrixOut[row][column] = sum;
    return 0;
}

void PrintMatrix(int matrix[][N])
{
    int i,j;
    for(i=0; i<N; i++)
    {
        printf("\r\n");
        for(j=0; j<N; j++)
            printf("%d\t",matrix[i][j]);
    }
}

#include <stdio.h>
void enqueue(int *, int);
int dequeue(int *);
int qempty(void);
int head = 0;
int tail = 0;
int qerror(void);
#define N 4
/*int main(void)
  {
  int i = 0;
  int v[N];
  while (i < 10) {
  enqueue(v, i);
  i++;
  }
  while (!qempty())
  printf("%d\n", dequeue(v));
  }
  */
// Drop patient  if  queue it is full

void enqueue(int *q, int x)
{   
    printf("head=%d\ttail=%d\n", head, tail+1);
    if (head == tail) 
        fprintf(stderr, "full\n");
    exit(1);
    q[tail] = x;
    if (tail == N)      
        tail = 0;
    else
        tail++;
    printf("tail=%d\n", tail);
//    printf("rand=%d\n", rand() % 5);
 //   printf("rand5-8=%d\n", 4+ rand() % 4);
}
int dequeue(int *q)
{
    
    printf("head=%d\ttail=%d\n", head, tail+1);
    int x;
   if (head == tail)
   fprintf(stderr, "empty\n");
    x = q[head];
    if (head == N) 
        head = 0;
    else
        head++;
    return x;
}
int qempty(void)
{
    return head == tail;
}
int qerror(void)
{   printf("head%d=\ttail=%d\n", head, tail);
    if (head == tail+1)
        fprintf(stderr, "coda piena\n");
}

int qfull(void)
{
    return head = tail +1;
}    

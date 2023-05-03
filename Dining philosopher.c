#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<time.h>

sem_t chopstick[5];
sem_t room;
int total;

int eat(int p)
{
    clock_t start_t , end_t ;          
    start_t=clock();
	printf("\nPosopher %d is eating",p);
	end_t = clock();
	total = end_t-start_t;
	
	return total;	
}

void *posopher(void * num)
{
	int p=*(int *)num;

	sem_wait(&room);
	printf("\nPosopher %d is hungry",p);
	sem_wait(&chopstick[p]);
	sem_wait(&chopstick[(p+1)%5]);

	eat(p);
	sleep(10);
	printf("\nPosopher %d has finished eating",p);
	printf("\ntotal time taken by the posopher %d is %d msecs",p,total);

	sem_post(&chopstick[(p+1)%5]);
	sem_post(&chopstick[p]);
	sem_post(&room);
}

int main()
{
	int i,a[5];
	pthread_t tid[5];
	sem_init(&room,0,5);
	
	for(i=0;i<5;i++)
		sem_init(&chopstick[i],0,1);
		
	for(i=0;i<5;i++)
	{
		a[i]=i;
		pthread_create(&tid[i],NULL,posopher,(void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}

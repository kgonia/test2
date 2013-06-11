#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <Windows.h>

using namespace std;

#define NUM_THREADS     3
const int N = 7;
pthread_mutex_t lock;

struct pole {
	bool poz1;
	bool poz2;
};

pole miejsceImprezy[N][N];

void WypiszTab(){
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			if ( miejsceImprezy[i][j].poz1==true )
				cout << "*";
			else
				cout << "0";
		}
		cout <<endl;
	}

	cout <<endl;
	system("cls");
}

struct uczestnikThreadData {
	int threadId;
	int upicie;
	int x;
	int y;
};

uczestnikThreadData uczestnikThreadDataArray[NUM_THREADS];

struct ochroniarzThreadData{
	int threadId;
	int x;
	int y;
};

ochroniarzThreadData ochroniarzThreadDataArray[NUM_THREADS];

int couter=0;

void losuj(int &a, int &b){
	a=rand()%N;
	b=rand()%N;
}

void* ZmienMiejsceUczestnik(void *arg){

	pthread_mutex_lock(&lock);
   uczestnikThreadData *my_data;
  
   my_data = (struct uczestnikThreadData *) arg;
   int xOld = my_data -> x;
   int yOld = my_data->y;

   losuj(my_data -> x,my_data -> y);

   couter++;
   while(1) {
	   if(miejsceImprezy[my_data->x][my_data->y].poz1)
		   losuj(my_data -> x,my_data -> y);
	   else{
		   miejsceImprezy[my_data->x][my_data->y].poz1=true;
		   break;
	   }
   }
      
   miejsceImprezy[xOld][yOld].poz1=false;

   WypiszTab();
   pthread_mutex_unlock(&lock);

   
   pthread_exit(0);
     

   return NULL;
}

		

int main ()
{
	srand(time(NULL));
	pthread_t threads[NUM_THREADS];

	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

	WypiszTab();
	int p=0;

	while(1){
		for (int t=0; t<NUM_THREADS; t++)
		{
		uczestnikThreadDataArray[t].threadId=t;
		pthread_create(&threads[t], NULL, ZmienMiejsceUczestnik, (void *) &uczestnikThreadDataArray[t]);
		
		}
		if (p==40)
			break;
		else
			p++;
		
	}

//	Sleep(1000);

	for (int i=0; i< NUM_THREADS; i++)
	pthread_join(threads[i], NULL);
	cout <<couter;
	/*
	WypiszTab();
	cout <<couter;
    
    pthread_mutex_destroy(&lock);
	*/
}
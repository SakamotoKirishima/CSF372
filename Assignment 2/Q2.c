#include<stdio.h>
#include<stdlib.h>
#include <semaphore.h>
#include <fcntl.h> 
#include <sys/types.h>
#include<pthread.h>
#include<unistd.h>
sem_t semA, semB, semC, semD;
pthread_mutex_t lock;
int A,B,C,D;
int p1A, p1B, p1C, p2B, p2C, p2D, p3A, p3C, p3D, p4A, p4B, p4D, p5A, p6B, p7C, p8D;
int initsemaphore(sem_t* sem, int value){
	int pshared=0;
	int ret;
	ret = sem_init(sem, pshared, value);
	return ret;

}
int releaseA(){
	int x= rand()%3;
	if (x<2){
			A++;
			printf("%s\n", "A is collected.");
			return 1;
			
		}
	return 0;
}
int releaseB(){
	int x= rand()%4;
	if (x<3){
		B++;
		printf("%s\n", "B is collected.");
		return 1;
	}
	return 0;
}
int releaseC(){
	int x= rand()%5;
	if (x<3){
		C++;
		printf("%s\n", "C is collected.");
		return 1;
	}
	return 0;
}
int releaseD(){
	int x= rand()%3;
	if (x<2){
		D++;
		printf("%s\n", "D is collected.");
		return 1;
	}
	return 0;
}
void realrun1(){
	printf("%s\n", "1 is running");
	sleep(30);
}
int release1A(){
	int check=releaseA();
	if (check) {
		p1A--;
		return 1;
	}
	return 0;
}
int release1B(){
	int check=releaseB();
	if (check){
		p1B--;
		return 1;
	} 
	return 0;
}
int release1C(){
	int check=releaseC();
	if (check){
		p1C--;
		return 1;
	}
	return 0; 
}
int release2B(){
	int check=releaseB();
	if (check){
		p2B--;
		return 1;
	} 
	return 0;
}
int release2C(){
	int check=releaseC();
	if (check){
		p2C--;
		return 1;
	}
	return 0; 
}
int release2D(){
	int check=releaseD();
	if (check){
		p2D--;
		return 1;
	}
	return 0; 
}
int release3A(){
	int check=releaseA();
	if (check){
		p3A--;
		return 1;
	} 
	return 0;
}
int release3C(){
	int check=releaseC();
	if (check){
		p3C--;
		return 1;
	} 
	return 0;
}
int release3D(){
	int check=releaseD();
	if (check){
		p3D--;
		return 1;
	} 
	return 0;
}
int release4A(){
	int check=releaseA();
	if (check){
		p4A--;
		return 1;
	} 
	return 0;
}
int release4B(){
	int check=releaseB();
	if (check){
		p4B--;
		return 1;
	} 
	return 0;
}
int release4D(){
	int check=releaseD();
	if (check){
		p4D--;
		return 1;
	} 
	return 0;
}
int release5A(){
	int check=releaseA();
	if (check){
		p5A--;
		return 1;
	} 
	return 0;
}
int release6B(){
	int check=releaseB();
	if (check){
		p6B--;
		return 1;
	} 
	return 0;
}
int release7C(){
	int check=releaseC();
	if (check){
		p7C--;
		return 1;
	} 
	return 0;
}
int release8D(){
	int check=releaseD();
	if (check){
		p8D--;
		return 1;
	} 
	return 0;
}
void * run1(void * args){
	printf("%s\n",  "1 has started.");
	int check1, check2, check3;
	check3=check2=check1=0;
	pthread_mutex_lock(&lock);
	if (p1A>0){
		check1=1;
	}
	if (p1B>0){
		check2=1;
	} 
	if (p1C>0){
		check3=1;
	}
	if ((A>0||check1) && (B>0||check2) && (C>0||check3)){
		if (!check1){
			sem_wait(&semA);
			A--;
			p1A++;
		}
		if (!check2){
			sem_wait(&semB);
			B--;
			p1B++;
		}
		if (!check3){
			C--;
			sem_wait(&semC);
			p1C++;
		}
		realrun1();
		int check= release1A();
		if (check) sem_post(&semA);
		
		check= release1B();
		if (check) sem_post(&semB);
		
		check= release1C();
		if (check)sem_post(&semC);
	}
	printf("%s\n",  "1 has terminated.");
	printf("A=%d B=%d C= %d D= %d\n", A, B, C, D);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void * run2(void * args){
	printf("%s\n",  "2 has started.");
	int check1, check2, check3;
	check3=check2=check1=0;
	pthread_mutex_lock(&lock);
	if (p2B>0){
		check1=1;
	}
	if (p2C>0){
		check2=1;
	} 
	if (p2D>0){
		check3=1;
	}
	if ((B>0||check1) && (C>0||check2) && (D>0||check3)){
		if (!check1){
			sem_wait(&semB);
			B--;
			p2B++;
		}
		if (!check2){
			sem_wait(&semC);
			C--;
			p2C++;
		}
		if (!check3){
			sem_wait(&semD);
			D--;
			p2D++;
		}
		printf("%s\n", "2 is running");
		sleep(30);
		int check= release2B();
		if (check) sem_post(&semB);
		
		check= release2C();
		if (check) sem_post(&semC);
		
		check= release2D();
		if (check)sem_post(&semD);
	}
	printf("%s\n",  "2 has terminated.");
	printf("A=%d B=%d C= %d D= %d\n", A, B, C, D);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void * run3(void * args){
	printf("%s\n",  "3 has started.");
	int check1, check2, check3;
	check3=check2=check1=0;
	pthread_mutex_lock(&lock);
	if (p3A>0){
		check1=1;
	}
	if (p3C>0){
		check2=1;
	} 
	if (p3D>0){
		check3=1;
	}
	if ((A>0||check1) && (C>0||check2) && (D>0||check3)){
		if (!check1){
			sem_wait(&semA);
			A--;
			p3A++;
		}
		if (!check2){
			sem_wait(&semC);
			C--;
			p3C++;
		}
		if (!check3){
			sem_wait(&semD);
			D--;
			p3D++;
		}
		printf("%s\n", "3 is running");
		sleep(30);
		int check= release3A();
		if (check) sem_post(&semA);
		
		check= release3C();
		if (check) sem_post(&semC);
		
		check= release3D();
		if (check)sem_post(&semD);
	}
	printf("%s\n",  "3 has terminated.");
	printf("A=%d B=%d C= %d D= %d\n", A, B, C, D);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void * run4(void * args){
	printf("%s\n",  "4 has started.");
	int check1, check2, check3;
	check3=check2=check1=0;
	pthread_mutex_lock(&lock);
	if (p4A>0){
		check1=1;
	}
	if (p4B>0){
		check2=1;
	} 
	if (p4D>0){
		check3=1;
	}
	if ((A>0||check1) && (B>0||check2) && (D>0||check3)){
		if (!check1){
			sem_wait(&semA);
			A--;
			p4A++;
		}
		if (!check2){
			sem_wait(&semB);
			B--;
			p4B++;
		}
		if (!check3){
			sem_wait(&semD);
			D--;
			p4D++;
		}
		printf("%s\n", "4 is running");
		sleep(30);
		int check= release4A();
		if (check) sem_post(&semA);
		
		check= release4B();
		if (check) sem_post(&semB);
		
		check= release4D();
		if (check)sem_post(&semD);
	}
	printf("%s\n",  "4 has terminated.");
	printf("A=%d B=%d C= %d D= %d\n", A, B, C, D);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void * run5(void * args){
	printf("%s\n",  "5 has started.");
	int check1, check2, check3;
	check3=check2=check1=0;
	pthread_mutex_lock(&lock);
	if (p5A>0){
		check1=1;
	}
	if (A>0||check1){
		if (!check1){
			sem_wait(&semA);
			A--;
			p5A++;
		}
		printf("%s\n", "5 is running");
		sleep(30);
		int check= release5A();
		if (check) sem_post(&semA);
	}
	printf("%s\n",  "5 has terminated.");
	printf("A=%d B=%d C= %d D= %d\n", A, B, C, D);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void * run6(void * args){
	printf("%s\n",  "6 has started.");
	int check1, check2, check3;
	check3=check2=check1=0;
	pthread_mutex_lock(&lock);
	if (p6B>0){
		check1=1;
	}
	if (B>0||check1){
		if (!check1){
			sem_wait(&semB);
			B--;
			p6B++;
		}
		printf("%s\n", "6 is running");
		sleep(30);
		int check= release6B();
		if (check) sem_post(&semB);
	}
	printf("%s\n",  "6 has terminated.");
	printf("A=%d B=%d C= %d D= %d\n", A, B, C, D);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void * run7(void * args){
	printf("%s\n",  "7 has started.");
	int check1, check2, check3;
	check3=check2=check1=0;
	pthread_mutex_lock(&lock);
	if (p7C>0){
		check1=1;
	}
	if (C>0||check1){
		if (!check1){
			sem_wait(&semC);
			C--;
			p7C++;
		}
		printf("%s\n", "7 is running");
		sleep(30);
		int check= release7C();
		if (check) sem_post(&semC);
	}
	printf("%s\n",  "7 has terminated.");
	printf("A=%d B=%d C= %d D= %d\n", A, B, C, D);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void * run8(void * args){
	printf("%s\n",  "8 has started.");
	int check1, check2, check3;
	check3=check2=check1=0;
	pthread_mutex_lock(&lock);
	if (p8D>0){
		check1=1;
	}
	if (D>0||check1){
		if (!check1){
			sem_wait(&semD);
			D--;
			p8D++;
		}
		printf("%s\n", "8 is running");
		sleep(30);
		int check= release8D();
		if (check) sem_post(&semD);
	}
	printf("%s\n",  "8 has terminated.");
	printf("A=%d B=%d C= %d D= %d\n", A, B, C, D);
	pthread_mutex_unlock(&lock);
	return NULL;
}
int main(){
	int n;
	scanf("%d", &n);
	int initialA, initialB, initialC, initialD;
	scanf("%d %d %d %d", &initialA,&initialB, &initialC, &initialD);
	A=initialA;
	B=initialB;
	C=initialC;
	D=initialD;
	p1A=p1B=p1C=p2B=p2C=p2D=p3A=p3C=p3D=p4A=p4B=p4D=p5A=p6B=p7C=p8D=0;
	pthread_t p1, p2, p3, p4, p5, p6, p7, p8;
	initsemaphore(&semA, initialA);
	initsemaphore(&semB, initialB);
	initsemaphore(&semC, initialC);
	initsemaphore(&semD, initialD);
	pthread_mutex_init(&lock, NULL);
	while(n>0){
		pthread_create(&p1, NULL, run1, NULL);
		pthread_create(&p2, NULL, run2, NULL);
		pthread_create(&p3, NULL, run3, NULL);
		pthread_create(&p4, NULL, run4, NULL);
		pthread_create(&p5, NULL, run5, NULL);
		pthread_create(&p6, NULL, run6, NULL);
		pthread_create(&p7, NULL, run7, NULL);
		pthread_create(&p8, NULL, run8, NULL);
		pthread_join(p1, NULL);
		pthread_join(p2,NULL);
		pthread_join(p3,NULL);
		pthread_join(p4,NULL);
		pthread_join(p5,NULL);
		pthread_join(p6,NULL);
		pthread_join(p7,NULL);
		pthread_join(p8,NULL);
		n--;
	}
	sem_destroy(&semA);
	sem_destroy(&semB);
	sem_destroy(&semC);
	sem_destroy(&semD);
	return 0;
}
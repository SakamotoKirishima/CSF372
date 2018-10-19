#include<stdio.h>
#include<stdlib.h>

//Process structure
typedef struct Process {
	int pid;
	char state;
	int arrival_time;
	int cpu_burst;
	int wait_time;
	char* scheduling_policy;
	int time_quantum;
	bool preemption;
// add other fields which you feel are necessary
} Process;

#define .at .process.arrival_time

//Event structure
typedef struct Event {
	enum eventType { Arrival, CPUburstCompletion, TimerExpired };
	double time; // time units since the start of the simulation
	// add other fields which you feel are necessary
} Event;

//Global variables
int CPU;
Process processes[];

//Heaps
#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

//Heap Implementation for ready queue

typedef struct processNode {
	Process process ;
} processNode ;

typedef struct minProcessHeap {
	int size ;
	processNode *elem ;
} minProcessHeap ;

void swapProcessNodes(processNode *n1,processNode *n2) {
	processNode temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

minProcessHeap initProcessHeap() {
	minProcessHeap hp;
	hp.size = 0;
	return hp;
}

void heapifyProcess(minProcessHeap *hp, int i) {
	int smallest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].at < hp->elem[i].at) ? LCHILD(i) : i ;
	if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].at < hp->elem[smallest].at) {
		smallest = RCHILD(i) ;
	}
	if(smallest != i) {
		swap(&(hp->elem[i]), &(hp->elem[smallest])) ;
		heapifyProcess(hp, smallest) ;
	}
}

void buildMinProcessHeap(minProcessHeap *hp, Process *arr, int size) {
	int i ;
	for(i = 0; i < size; i++) {
		if(hp->size) {
			hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(processNode)) ;
		} else {
			hp->elem = malloc(sizeof(processNode)) ;
		}
		processNode nd ;
		nd.process = arr[i] ;
		hp->elem[(hp->size)++] = nd ;
	}

	for(i = (hp->size - 1) / 2; i >= 0; i--) {
		heapifyProcess(hp, i) ;
	}
}

void insertProcessNode(minProcessHeap *hp, Process process) {
	if(hp->size) {
		hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(processNode)) ;
	} else {
		hp->elem = malloc(sizeof(processNode)) ;
	}

	processNode nd ;
	nd.process = process ;

	int i = (hp->size)++ ;
	while(i && nd.at < hp->elem[PARENT(i)].at) {
		hp->elem[i] = hp->elem[PARENT(i)] ;
		i = PARENT(i) ;
	}
	hp->elem[i] = nd ;
}

void deleteProcessNode(minProcessHeap *hp) {
    if(hp->size) {
        printf("Deleting process %d\n\n", hp->elem[0].process.pid) ;
        hp->elem[0] = hp->elem[--(hp->size)] ;
        hp->elem = realloc(hp->elem, hp->size * sizeof(processNode)) ;
        heapifyProcess(hp, 0) ;
    } else {
        printf("\nMin Heap is empty!\n") ;
        free(hp->elem) ;
    }
}

//Heap implementation of event queue

typedef struct EventNode {
	Event event ;
} EventNode ;

typedef struct minEventHeap {
	int size ;
	EventNode *elem ;
} minEventHeap ;

void swapEventNodes(EventNode *n1,EventNode *n2) {
	EventNode temp = *n1;
	*n1 = *n2;
	*n2 = temp;
}

minEventHeap initEventHeap() {
	minEventHeap hp;
	hp.size = 0;
	return hp;
}

void heapifyEvent(minEventHeap *hp, int i) {
	int smallest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].at < hp->elem[i].at) ? LCHILD(i) : i ;
	if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].at < hp->elem[smallest].at) {
		smallest = RCHILD(i) ;
	}
	if(smallest != i) {
		swap(&(hp->elem[i]), &(hp->elem[smallest])) ;
		heapifyEvent(hp, smallest) ;
	}
}

void buildMinEventHeap(minEventHeap *hp, Event *arr, int size) {
	int i ;
	for(i = 0; i < size; i++) {
		if(hp->size) {
			hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(EventNode)) ;
		} else {
			hp->elem = malloc(sizeof(EventNode)) ;
		}
		EventNode nd ;
		nd.event = arr[i] ;
		hp->elem[(hp->size)++] = nd ;
	}

	for(i = (hp->size - 1) / 2; i >= 0; i--) {
		heapifyEvent(hp, i) ;
	}
}

void insertEventNode(minEventHeap *hp, Event event) {
	if(hp->size) {
		hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(EventNode)) ;
	} else {
		hp->elem = malloc(sizeof(EventNode)) ;
	}

	EventNode nd ;
	nd.Event = event ;

	int i = (hp->size)++ ;
	while(i && nd.at < hp->elem[PARENT(i)].at) {
		hp->elem[i] = hp->elem[PARENT(i)] ;
		i = PARENT(i) ;
	}
	hp->elem[i] = nd ;
}

void deleteEventNode(minEventHeap *hp) {
    if(hp->size) {
        printf("Deleting event") ;
        hp->elem[0] = hp->elem[--(hp->size)] ;
        hp->elem = realloc(hp->elem, hp->size * sizeof(EventNode)) ;
        heapifyEvent(hp, 0) ;
    } else {
        printf("\nMin Heap is empty!\n") ;
        free(hp->elem) ;
    }
}





void b_sort(Process temp[],int n)
{
	Process t;
	int i,j;
	for(i=1;i<n;i++)
		for(j=0;j<n-i;j++){
			if(temp[j].at > temp[j+1].at){
				t = temp[j];
				temp[j] = temp[j+1];
				temp[j+1] = t;
			}
		}
	}

	int accept(Process P[]){
		int i,n;
		printf("\n Enter total no. of Process : ");
		scanf("%d",&n);
		for(i=0;i<n;i++){
			printf("\n PROCESS [%d]",i+1);
			printf(" Enter process name : ");
			scanf("%s",&P[i].name);
			printf(" Enter burst time : ");
			scanf("%d",&P[i].bt);
			printf(" Enter arrival time : ");
			scanf("%d",&P[i].at);
			printf(" Enter priority : ");
			scanf("%d",&P[i].prt);
		}
		printf("\n PROC.\tB.T.\tA.T.\tPRIORITY");
		for(i=0;i<n;i++)
			printf("\n %s\t%d\t%d\t%d",P[i].name,P[i].bt,P[i].at,P[i].prt);
		return n;
	}

	int main(){

		int mode;
		printf("1: FCFS\n2: Multilevel Queue\nPlease enter your choice: ");
		scanf("%d",&mode);
		if(mode==1){
			//FCFS
		}
		else if (mode==2){
			//Multilevel queue
		}
		else{
			printf("Error in choice..... exiting\n");
		}
	}
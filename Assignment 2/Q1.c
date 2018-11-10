#include<stdio.h>
#include<stdlib.h>

//Process structure
typedef struct Process {
	int pid;
    char state;			//N = Not arrived yet, A = Arrived/Ready, R = Running
    int arrival_time;
    int cpu_burst;
    
// add other fields which you feel are necessary
} Process;

//Event structure
typedef struct Event {
	enum eventType { Arrival, CPUburstCompletion, TimerExpired } type;
    double time; // time of this event
    
} Event;

//Global variables
int CPU;

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
	int smallest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].process.arrival_time < hp->elem[i].process.arrival_time) ? LCHILD(i) : i ;
	if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].process.arrival_time < hp->elem[smallest].process.arrival_time) {
		smallest = RCHILD(i) ;
	}
	if(smallest != i) {
		swapProcessNodes(&(hp->elem[i]), &(hp->elem[smallest])) ;
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
	while(i && nd.process.arrival_time < hp->elem[PARENT(i)].process.arrival_time) {
		hp->elem[i] = hp->elem[PARENT(i)] ;
		i = PARENT(i) ;
	}
	hp->elem[i] = nd ;
}

void deleteProcessNode(minProcessHeap *hp) {
	if(hp->size) {
		//printf("Deleting process %d\n\n", hp->elem[0].process.pid) ;
		hp->elem[0] = hp->elem[--(hp->size)] ;
		hp->elem = realloc(hp->elem, hp->size * sizeof(processNode)) ;
		heapifyProcess(hp, 0) ;
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
	int smallest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].event.time < hp->elem[i].event.time) ? LCHILD(i) : i ;
	if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].event.time < hp->elem[smallest].event.time) {
		smallest = RCHILD(i) ;
	}
	if(smallest != i) {
		swapEventNodes(&(hp->elem[i]), &(hp->elem[smallest])) ;
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
	nd.event = event ;

	int i = (hp->size)++ ;
	while(i && nd.event.time < hp->elem[PARENT(i)].event.time) {
		hp->elem[i] = hp->elem[PARENT(i)] ;
		i = PARENT(i) ;
	}
	hp->elem[i] = nd ;
}

void deleteEventNode(minEventHeap *hp) {
	if(hp->size) {
		//printf("Deleting event\n") ;
		hp->elem[0] = hp->elem[--(hp->size)] ;
		hp->elem = realloc(hp->elem, hp->size * sizeof(EventNode)) ;
		heapifyEvent(hp, 0) ;
	}
}

void insertProcesses(int processCount, Process processes[]) {
	int i;
	for(i=0;i<processCount;i++){
		printf("Enter arrival time for process %d: ",i+1);
		scanf("%d",&processes[i].arrival_time);
		printf("Enter cpu_burst for process %d: ",i+1);
		scanf("%d",&processes[i].cpu_burst);
		processes[i].state = 'N';
		processes[i].pid = i+1;
	}

	//Bubble sort processes
	int j;
	for(i=0;i<processCount-1;i++) {
		for(j=0;j<processCount-1;j++) {
			if (processes[j].arrival_time > processes[j+1].arrival_time) {
				Process temp = processes[j];
				processes[j] = processes[j+1];
				processes[j+1] = temp;
			}
		}
	}

}


int main() {

	CPU = -1;

	int mode;
	printf("1: FCFS\n2: Multilevel Queue\nPlease enter your choice: ");
	scanf("%d",&mode);

	double time = 0.0;
        double waitTime = 0.0;	//TODO: Use this

        int processCount;
        printf("Enter number of processes: ");
        scanf("%d",&processCount);
        Process processes[processCount];
        insertProcesses(processCount, processes);		//TODO: Set this up properly
        int processCounter = 0;
        minProcessHeap processHeap = initProcessHeap();
        minProcessHeap *processHeapPtr = &processHeap;
        //buildMinProcessHeap(processHeapPtr, processes, size);
        minEventHeap eventHeap = initEventHeap();
        minEventHeap *eventHeapPtr = &eventHeap;
        Event initialEvent;
        initialEvent.type = Arrival;
        initialEvent.time = processes[processCounter].arrival_time;
        insertEventNode(&eventHeap,initialEvent);

        if(mode==1) {

        	while(eventHeap.size != 0) {
        		Event currentEvent = eventHeapPtr->elem[0].event;
        		time = currentEvent.time;
        		switch(currentEvent.type) {
            //TODO
        			case Arrival:
        			processes[processCounter].state = 'A';
        			insertProcessNode(processHeapPtr,processes[processCounter]);
        			printf("Process with PID %d is now in ready queue\n",processes[processCounter].pid);
        			if(CPU==-1) {
        				CPU = processes[processCounter].pid;
        				printf("Running process with pid = %d\n",CPU);
        				processes[processCounter].state = 'R';
        				//implicitly, waiting time is 0

        				//Adding CPUBurst of this process;
        				Event e;
        				e.type = CPUburstCompletion;
        				e.time = time+(processes[processCounter].cpu_burst);
        				insertEventNode(eventHeapPtr,e);
        			}

        			processCounter++;

                	//Adding next Arrival process;
        			if(processCounter < processCount){
        				Event e;
        				e.type = Arrival;
        				e.time = processes[processCounter].arrival_time;
        				insertEventNode(eventHeapPtr,e);
        			}

        			deleteEventNode(eventHeapPtr);
        			break;


        			case CPUburstCompletion:
        			printf("Process with id %d has finished execution\n", CPU);
        			CPU = -1;
        			deleteProcessNode(processHeapPtr);
        			if(processHeapPtr->size != 0){
        				Process *process = &((processHeapPtr->elem[0]).process);
        				process->state = 'R';
        				CPU = process->pid;
        				printf("Running process with pid = %d\n",CPU);
        				waitTime+=(time-(process->arrival_time));
                    	//Adding CPUBurst of this process;
        				Event e;
        				e.type = CPUburstCompletion;
        				e.time = time+(process->cpu_burst);
        				insertEventNode(eventHeapPtr,e);
        			}

        			deleteEventNode(eventHeapPtr);
        			break;
        			case TimerExpired:
                //Unused in FCFS
        			break;
        		}
        	}
        waitTime/=processCount; //TODO: Change this to number of processes
        printf("Average waitTime = %lf\n",waitTime);
    }
    else if (mode==2) {

        //Multilevel queue

    	while(eventHeap.size != 0) {
    		Event currentEvent = eventHeapPtr->elem[0].event;
    		time = currentEvent.time;
    		switch(currentEvent.type) {
            //TODO
    			case Arrival:
    			insertProcessNode(processHeapPtr,processes[processCounter++]);
    			Process *process = &((processHeapPtr->elem[0]).process);
    			process->state = 'A';
    			printf("Process with PID %d is now in ready queue\n",process->pid);
    			if(CPU==-1) {
    				CPU = process->pid;
    				printf("Running process with pid = %d\n",CPU);
    				process->state = 'R';
                    //implicitly, waiting time is 0
    			}

                //Adding CPUBurst of this process;
    			Event e;
    			e.type = CPUburstCompletion;
    			e.time = time+(process->cpu_burst);

                //Adding next Arrival process;
    			if(processCounter < processCount){
    				e.type = Arrival;
    				e.time = processes[processCounter].arrival_time;
    				insertEventNode(eventHeapPtr,e);
    			}

    			deleteEventNode(eventHeapPtr);
    			break;
    			case CPUburstCompletion:
    			CPU = -1;
    			printf("Process with id %d has finished execution\n", CPU);
    			deleteProcessNode(processHeapPtr);
    			if(processHeapPtr->size != 0){
    				Process *process = &((processHeapPtr->elem[0]).process);
    				process->state = 'R';
    				CPU = process->pid;
    				printf("Running process with pid = %d\n",CPU);
    				waitTime+=(time-(process->arrival_time));
                    //Adding CPUBurst of this process;
    				Event e;
    				e.type = CPUburstCompletion;
    				e.time = time+(process->cpu_burst);
    				insertEventNode(eventHeapPtr,e);
    			}

    			deleteEventNode(eventHeapPtr);
    			break;
    			case TimerExpired:
                //Unused in FCFS
    			break;
    		}
    	}
        waitTime/=processCount; //TODO: Change this to number of processes
        printf("Average waitTime = %lf\n",waitTime);
    }
    else {
    	printf("Error in choice..... exiting\n");
    }
}
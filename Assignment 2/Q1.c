#include<stdio.h>
#include<stdlib.h>

//Process structure
typedef struct Process {
	int pid;            //Will be the data entry number for simplicity
    char state;			//N = Not arrived yet, A = Arrived/Ready, R = Running
    int arrival_time;
    int cpu_burst;
    
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

typedef struct minProcessHeap {
	int size ;
	Process *elem ;
} minProcessHeap ;

void swapProcessNodes(Process *p1, Process *p2) {
	Process temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

minProcessHeap initProcessHeap() {
	minProcessHeap hp;
	hp.size = 0;
	return hp;
}

void heapifyProcess(minProcessHeap *hp, int i) {
	int smallest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].arrival_time < hp->elem[i].arrival_time) ? LCHILD(i) : i ;
	if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].arrival_time < hp->elem[smallest].arrival_time) {
		smallest = RCHILD(i) ;
	}
	if(smallest != i) {
		swapProcessNodes(&(hp->elem[i]), &(hp->elem[smallest])) ;
		heapifyProcess(hp, smallest) ;
	}
}

void insertProcessNode(minProcessHeap *hp, Process process) {
	if(hp->size) {
		hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(Process)) ;
	} else {
		hp->elem = malloc(sizeof(Process)) ;
	}

	int i = (hp->size)++ ;
	while(i && process.arrival_time < hp->elem[PARENT(i)].arrival_time) {
		hp->elem[i] = hp->elem[PARENT(i)] ;
		i = PARENT(i) ;
	}
	hp->elem[i] = process ;
}

void deleteProcessNode(minProcessHeap *hp) {
	if(hp->size) {
		//printf("Deleting process %d\n\n", hp->elem[0].process.pid) ;
		hp->elem[0] = hp->elem[--(hp->size)] ;
		hp->elem = realloc(hp->elem, hp->size * sizeof(Process)) ;
		heapifyProcess(hp, 0) ;
	}
}

//Heap implementation of event queue

typedef struct minEventHeap {
	int size ;
	Event *elem ;
} minEventHeap ;

void swapEventNodes(Event *e1,Event *e2) {
	Event temp = *e1;
	*e1 = *e2;
	*e2 = temp;
}

minEventHeap initEventHeap() {
	minEventHeap hp;
	hp.size = 0;
	return hp;
}

void heapifyEvent(minEventHeap *hp, int i) {
	int smallest = (LCHILD(i) < hp->size && hp->elem[LCHILD(i)].time < hp->elem[i].time) ? LCHILD(i) : i ;
	if(RCHILD(i) < hp->size && hp->elem[RCHILD(i)].time < hp->elem[smallest].time) {
		smallest = RCHILD(i) ;
	}
	if(smallest != i) {
		swapEventNodes(&(hp->elem[i]), &(hp->elem[smallest])) ;
		heapifyEvent(hp, smallest) ;
	}
}

void insertEventNode(minEventHeap *hp, Event event) {
	if(hp->size) {
		hp->elem = realloc(hp->elem, (hp->size + 1) * sizeof(Event)) ;
	} else {
		hp->elem = malloc(sizeof(Event)) ;
	}

	int i = (hp->size)++ ;
	while(i && event.time < hp->elem[PARENT(i)].time) {
		hp->elem[i] = hp->elem[PARENT(i)] ;
		i = PARENT(i) ;
	}
	hp->elem[i] = event ;
}

void deleteEventNode(minEventHeap *hp) {
	if(hp->size) {
		//printf("Deleting event\n") ;
		hp->elem[0] = hp->elem[--(hp->size)] ;
		hp->elem = realloc(hp->elem, hp->size * sizeof(Event)) ;
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

	//Bubble sort processes (I am just lazy to optimize this)
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

    //Initialize
	double time = 0.0;         //Current time of the system since start
    double waitTime = 0.0;     //Sum of wait times of all processes 

    int processCount;          //Number of processes
    printf("Enter number of processes: ");
    scanf("%d",&processCount);
    Process processes[processCount];
    insertProcesses(processCount, processes);	//Get process data from user
    int processCounter = 0;						//Process array is sorted, this is used to get next process from it
    minProcessHeap processHeap = initProcessHeap();
    minProcessHeap *processHeapPtr = &processHeap;		//get heap pointer (passed to other heap functions)
    minEventHeap eventHeap = initEventHeap();
    minEventHeap *eventHeapPtr = &eventHeap;			//get heap pointer (passed to other heap functions)

    //Initial Entry
    Event initialEvent;
    initialEvent.type = Arrival;
    initialEvent.time = processes[processCounter].arrival_time;
    insertEventNode(&eventHeap,initialEvent);

    //Queue cases
    if(mode==1) {

    	//FCFS
    	//Loop while any events remain
    	while(eventHeap.size != 0) {
    		Event currentEvent = eventHeapPtr->elem[0];
    		time = currentEvent.time;					//Update system clock to given time (as this is a simulation)
    		switch(currentEvent.type) {

    			case Arrival:
    			processes[processCounter].state = 'A';
    			insertProcessNode(processHeapPtr,processes[processCounter]);
    			printf("Process with PID %d is now in ready queue\n",processes[processCounter].pid);

    			//If there are no currently running processes, run the new process right away
    			if(CPU==-1) {
    				CPU = processes[processCounter].pid;
    				printf("Running process with pid = %d\n",CPU);
    				processes[processCounter].state = 'R';
        			//implicitly, waiting time is 0 (as it ran right away)

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

    			deleteEventNode(eventHeapPtr);		//Delete current event as it has occured now
    			break;


    			case CPUburstCompletion:
    			printf("Process with id %d has finished execution\n", CPU);
    			CPU = -1;				//CPU is now not running any process
    			deleteProcessNode(processHeapPtr);		//Process finished execution (no need for it to be in the heap)

    			//Run next process if it exists
    			if(processHeapPtr->size != 0){
    				Process process = processHeapPtr->elem[0];
    				process.state = 'R';
    				CPU = process.pid;
    				printf("Running process with pid = %d\n",CPU);
    				waitTime+=(time-(process.arrival_time));

                    //Adding CPUBurst of this process;
    				Event e;
    				e.type = CPUburstCompletion;
    				e.time = time+(process.cpu_burst);
    				insertEventNode(eventHeapPtr,e);
    			}

    			deleteEventNode(eventHeapPtr);		//Delete current event as it has occured now
    			break;
    			case TimerExpired:
                //Unused in FCFS
    			break;
    		}
    	}

    	waitTime/=processCount;		//Get avg. wait time
    	printf("Average waitTime = %lf\n",waitTime);
    }
    else if (mode==2) {

        /*

        //Multilevel queue

    	while(eventHeap.size != 0) {
    		Event currentEvent = eventHeapPtr->elem[0];
    		time = currentEvent.time;
    		switch(currentEvent.type) {
            //TODO
    			case Arrival:
    			insertProcessNode(processHeapPtr,processes[processCounter++]);
    			Process *process = &((processHeapPtr->elem[0]));
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
    				Process *process = &((processHeapPtr->elem[0]));
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

        */
    }
    else {
    	printf("Error in choice..... exiting\n");
    }
}
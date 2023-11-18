#include <iostream>
#include <climits>
#include <queue>
using namespace std ; 

class Process
{
    int processId;
    int arrivalTime ; 
    int burstTime ;
    int waitingTime ; 
    int turnaroundTime ; 
    int priority ;
    public : 
    Process()
    {
        this->processId = -1 ; 
        this->arrivalTime = -1 ; 
        this->burstTime = -1 ; 
        this->waitingTime = -1 ; 
        this->turnaroundTime = -1 ; 
    }

    Process(int processId, int arrivalTime, int burstTime)
    {
        this->processId = processId ; 
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime ;
        this->waitingTime = -1 ; 
        this->turnaroundTime = -1  ; 
    }

    Process(int processId, int arrivalTime, int burstTime, int priority)
    {
        this->processId = processId ; 
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime ;
        this->priority = priority ; 
        this->waitingTime = -1 ; 
        this->turnaroundTime = -1  ; 
    }

    friend class ProcessScheduler ;
};

class ProcessScheduler
{
    private : 
    
    int getProcessSJF(Process processes[], int burstTimes[], int n, int currTime)
    {
        int minIndex = 0 ; 
        for(int i = 0 ; i < n ; i++)
        {
            if(processes[i].arrivalTime<=currTime)
            {
                if(burstTimes[i]<burstTimes[minIndex])
                {
                    minIndex = i ; 
                }
            }
        }
        return minIndex ;
    }

    int getProcessOnPriority(Process processes[],bool isExecuted[], int n , int currTime)
    {
        int maxPriority = INT_MAX;
        int maxIndex = -1;
        for(int i = 0 ; i < n; i++)
        {
            if(!isExecuted[i] && processes[i].arrivalTime<=currTime)
            {
                if(processes[i].priority < maxPriority)
                {
                    maxPriority = processes[i].priority ; 
                    maxIndex = i ;
                }
            }
        }

        return maxIndex ; 
    }

    void displaySchedulingDetails(int processTimeLine[], int totalTime, Process processses[], int n)
    {
        cout<<"Process Time line : "<<endl;
        for(int i = 0 ; i < totalTime ; i++)
        {
            cout<<i+1<<" : "<<processTimeLine[i]<<endl;
        }

        cout<<"Waiting and Turnaround Time "<<endl;
        for(int i = 0 ; i < n ; i++)
        {
            cout<<processses[i].processId<<"  "<<processses[i].waitingTime<<"  "<<processses[i].turnaroundTime<<endl;
        }
    }
    
    int getTotalTime(Process processes[], int n)
    {
        int totalTime = 0 ; 
        for(int i = 0 ; i < n ; i++)
        {
            totalTime = totalTime + processes[i].burstTime ;
        }
        return totalTime ; 
    }
    
    public : 

    void FCFS(Process processes[], int n )
    {
        int totalTime = getTotalTime(processes,n);
        // Process Time line 
        int processTimeLine[totalTime];
        int currTime = 0 ;
        for(int i = 0 ; i < n ; i++)
        {
            for(int j = 0 ; j < processes[i].burstTime ; j++)
            {
                processTimeLine[currTime] = processes[i].processId ; 
                currTime++;
            }
            processes[i].turnaroundTime = currTime-processes[i].arrivalTime;
            processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime ; 
        }
        
        displaySchedulingDetails(processTimeLine, totalTime, processes, n);
    }

    void SJFPreemptive(Process processes[], int n)
    {
        int totalTime = getTotalTime(processes,n);
        
        int burstTimes[n] ; 
        for(int i = 0 ; i < n ; i++)
        {
            burstTimes[i] = processes[i].burstTime ; 
        }
        
        int processTimeLine[totalTime];
        for(int i= 0 ; i < totalTime ; i++)
        {
            int processIndex = getProcessSJF(processes,burstTimes,n,i);
            processTimeLine[i] = processes[processIndex].processId;
            burstTimes[processIndex]--;
            if(!burstTimes[processIndex])
            {
                burstTimes[processIndex] = INT_MAX;
                processes[processIndex].turnaroundTime = i - processes[processIndex].arrivalTime+1;
                processes[processIndex].waitingTime = processes[processIndex].turnaroundTime - processes[processIndex].burstTime ; 
            }
        }

        displaySchedulingDetails(processTimeLine,totalTime,processes,n);
    }

    void priorityNonPreemptive(Process processes[], int n)
    {
        int totalTime = getTotalTime(processes,n);

        bool* isExecuted  = new bool[n]();

        int processTimeLine[totalTime];
        int currTime = 0; 
        
        for(int i = 0 ; i < n ; i++)
        {
            int processIndex = getProcessOnPriority(processes,isExecuted,n,currTime);
            
            for(int j = 0 ; j < processes[processIndex].burstTime ; j++)
            {
                processTimeLine[currTime] = processes[processIndex].processId;
                currTime++;
            }
            processes[processIndex].turnaroundTime = currTime - processes[processIndex].arrivalTime;
            processes[processIndex].waitingTime = processes[processIndex].turnaroundTime - processes[processIndex].burstTime ;
            isExecuted[processIndex] = true ;  
        }

        displaySchedulingDetails(processTimeLine, totalTime, processes, n);
    }

    void roundRobin(Process processes[], int n, int timeQuant)
    {
        int totalTime = getTotalTime(processes,n);
        
        int remainingTime[n] ; 
        for(int i = 0 ; i<n ; i++)
        {
            remainingTime[i] = processes[i].burstTime;
        }

        queue<int>readyQueue;
        readyQueue.push(0);
        int processTimeLine[totalTime] ; 
        int currTime = 0 ;  
        while(!readyQueue.empty())
        {
            int processIndex = readyQueue.front();
            readyQueue.pop();
            int quantTime = timeQuant ; 
            while(quantTime && remainingTime[processIndex])
            {
                processTimeLine[currTime] = processes[processIndex].processId;
                cout<<currTime<<" : "<<processIndex<<endl;
                quantTime--;
                remainingTime[processIndex]--;
                currTime++;
                for(int i = 0 ; i < n ;i++)
                {
                    if(currTime == processes[i].arrivalTime)
                    {
                        readyQueue.push(i);
                    }
                }
            }
            if(remainingTime[processIndex])
            {
                readyQueue.push(processIndex);
            }
            else
            {
                processes[processIndex].turnaroundTime = currTime - processes[processIndex].arrivalTime ; 
                processes[processIndex].waitingTime = processes[processIndex].turnaroundTime - processes[processIndex].burstTime ; 
            }
        }

        displaySchedulingDetails(processTimeLine, totalTime, processes, n);
    }

};



int main()
{
    ProcessScheduler s; 

    // FCFS 
    // int FCFSn = 5 ;
    // Process FCFSData[FCFSn];
    // FCFSData[0] = Process(1,0,4);
    // FCFSData[1] = Process(2,1,3);
    // FCFSData[2] = Process(3,2,1);
    // FCFSData[3] = Process(4,3,2);
    // FCFSData[4] = Process(5,4,5);    
    // s.FCFS(FCFSData,FCFSn);

    // int FCFSn = 6 ;
    // Process FCFSData[FCFSn];
    // FCFSData[0] = Process(1,0,9);
    // FCFSData[1] = Process(2,1,3);
    // FCFSData[2] = Process(3,1,2);
    // FCFSData[3] = Process(4,1,4);
    // FCFSData[4] = Process(5,2,3);    
    // FCFSData[5] = Process(5,3,2);    
    // s.FCFS(FCFSData,FCFSn);

    
    // SJFPreemptive 
    // int SJFPn = 5;
    // Process SJFData[SJFPn];
    // SJFData[0] = Process(1,0,3);
    // SJFData[1] = Process(2,1,8);
    // SJFData[2] = Process(3,2,6);
    // SJFData[3] = Process(4,4,4);
    // SJFData[4] = Process(5,5,2);
    // s.SJFPreemptive(SJFData, SJFPn);

    // SJFPreemptive 
    // int SJFPn = 4;
    // Process SJFData[SJFPn];
    // SJFData[0] = Process(1,0,6);
    // SJFData[1] = Process(2,1,4);
    // SJFData[2] = Process(3,2,2);
    // SJFData[3] = Process(4,3,3);
    // s.SJFPreemptive(SJFData, SJFPn);


    // Priority Non Preemptive
    // int PNPn = 7 ; 
    // Process PNPData[PNPn];
    // id, arrival time, burst time, priority
    // PNPData[0] = Process(1,0,3,2);
    // PNPData[1] = Process(2,1,4,3);
    // PNPData[2] = Process(3,2,5,6);
    // PNPData[3] = Process(4,4,2,5);
    // PNPData[4] = Process(5,5,4,4);
    // PNPData[5] = Process(6,6,9,7);
    // PNPData[6] = Process(7,7,10,10);
    // s.priorityNonPreemptive(PNPData,PNPn);  


    // Round Robin 
    // int RRn = 4;
    // Process RRData[RRn];
    // RRData[0] = Process(1,0,5);
    // RRData[1] = Process(1,1,4);
    // RRData[2] = Process(1,2,2);
    // RRData[3] = Process(1,3,1);
    // s.roundRobin(RRData,RRn,2);

    // int RRn = 6;
    // Process RRData[RRn];
    // RRData[0] = Process(1,0,7);
    // RRData[1] = Process(1,1,4);
    // RRData[2] = Process(1,2,15);
    // RRData[3] = Process(1,3,11);
    // RRData[4] = Process(1,4,20);
    // RRData[5] = Process(1,4,9);
    // s.roundRobin(RRData,RRn,5);

    

    
    return 0 ; 
}
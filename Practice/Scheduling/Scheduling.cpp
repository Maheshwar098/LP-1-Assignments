#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <cmath>
using namespace std;

class Process
{
    public : 
    int id ; 
    int arrivalTime ;
    int burstTime ; 
    int completionTime;
    int waitingTime;
    int turnAroundTime;
    int priority;

    public : 
    Process(int id, int arrivalTime, int burstTime)
    {
        this->id = id  ;
        this->arrivalTime = arrivalTime ;
        this->burstTime = burstTime ;
        this->completionTime = -1;
        this->waitingTime = -1;
        this->turnAroundTime = -1;
        this->priority = -1 ;
    }

    Process(int id, int arrivalTime, int burstTime, int priority)
    {
        this->id = id ;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
        this->priority = priority;
        this->completionTime = -1 ; 
        this->waitingTime = -1 ;
        this->turnAroundTime = -1 ;
    }

};

class TimeSlot
{
    public:
    int start;
    int end;
    int processId;

    public:
    TimeSlot(int start, int end, int processId)
    {
        this->start = start;
        this->end = end;
        this->processId = processId;
    }
};

class Scheduler
{
    private :
    void displayProcesses(vector<Process>processes)
    {
        int n = processes.size();
        cout<<"Process Info : "<<endl;
        cout<<"id"<<"   "<<"AT"<<"    "<<"BT"<<"    "<<"CT"<<"    "<<"WT"<<"    "<<"TT"<<endl;
        for(int i = 0 ; i < n ; i++)
        {
            cout<<processes[i].id<<"    "<<processes[i].arrivalTime<<"    "<<processes[i].burstTime<<"   "<<processes[i].completionTime<<"    "<<processes[i].waitingTime<<"    "<<processes[i].turnAroundTime<<endl;
        }
    }

    int getTotalTime(vector<Process>processes)
    {
        int s = 0 ;
        int len = processes.size();
        for(int i = 0 ; i < len ; i++)
        {
            s = s + processes[i].burstTime;
        }
        return s;
    }

    void displayTimeLine(vector<TimeSlot>timeLine)
    {
        int n = timeLine.size();
        cout<<"TimeLine : "<<endl;
        for(int i = 0 ; i < n ; i++)
        {
            cout<<timeLine[i].start<<" - "<<timeLine[i].end<<" -> "<<"P"+to_string(timeLine[i].processId)<<endl;
        }
    }

    float getAvgWT(vector<Process>processes)
    {
        int n = processes.size();
        int s = 0;
        for(int i = 0 ; i < n ; i++)
        {
            s = s + processes[i].waitingTime;
        }

        float avg = (float)s/(float)n;

        return avg;

    }
    
    int getSJ(int remainingTime[], vector<Process>processes, int currTime)
    {
        int minTime = INT_MAX;
        int n = processes.size();
        int minIndx = -1;
        for(int i = 0 ; i < n ; i++)
        {
            if(processes[i].arrivalTime <= currTime && remainingTime[i] < minTime && remainingTime[i])
            {
                minTime = remainingTime[i];
                minIndx = i ;
            }
        }

        return minIndx;
    }

    int getMaxPriorityProcess(vector<Process>processes, int currTime, bool isExecuted[])
    {
        int n = processes.size() ;
        int maxPriority = INT_MAX;
        int maxPriorityIndx = -1 ;
        for(int i = 0 ; i < n ; i++)
        {
            if(processes[i].priority < maxPriority&& processes[i].arrivalTime <= currTime && isExecuted[i] == false)
            {
                maxPriority = processes[i].priority;
                maxPriorityIndx = i ;
            }
        }
        return maxPriorityIndx;
    }
    
    void updateQueue(queue<int>&q,int startTime, int currTime, vector<Process>processes, int remainingTime[])
    {
        int n = processes.size();
        for(int i = 0 ; i < n ; i++ )
        {
            if(processes[i].arrivalTime > startTime && processes[i].arrivalTime <= currTime)
            {
                q.push(i);
            }
        }
    }

    public:
    static bool comp(Process p1, Process p2)
    {
        return p1.arrivalTime < p2.arrivalTime;
    }

    void FCFS(vector<Process>processes)
    {
        int n = processes.size();
        sort(processes.begin(),processes.end(),comp);

        vector<TimeSlot>timeLine;
        int currTime = processes[0].arrivalTime ;
        for(int i = 0 ; i < n ; i++)
        {
            int startTime = currTime;
            if(processes[i].arrivalTime>currTime)
            {
                timeLine.push_back(TimeSlot(startTime,processes[i].arrivalTime,-1));
                currTime = processes[i].arrivalTime;
                startTime = currTime;
            }
            currTime = currTime + processes[i].burstTime;
            processes[i].completionTime = currTime;
            processes[i].turnAroundTime = processes[i].completionTime -processes[i].arrivalTime;
            processes[i].waitingTime = processes[i].turnAroundTime-processes[i].burstTime;
            timeLine.push_back(TimeSlot(startTime,currTime,processes[i].id));
        }
        
        displayTimeLine(timeLine);
        displayProcesses(processes);
        float avgWT = getAvgWT(processes);
        cout<<"Average Waiting time : "<<avgWT<<endl;


    }

    void SJFPreemptive(vector<Process>processes)
    {
        int n = processes.size();
        sort(processes.begin() , processes.end() , comp);
        int totalTime = getTotalTime(processes);
        int remainingTime[n];
        for(int i = 0 ; i < n ; i++)
        {
            remainingTime[i] = processes[i].burstTime;
        }

        int timeStamps[totalTime];
        for(int i = 0 ; i < totalTime ; i++)
        {
            int indx = getSJ(remainingTime, processes, i);
            timeStamps[i] = processes[indx].id;
            remainingTime[indx]--;
            if(remainingTime[indx] == 0)
            {
                processes[indx].completionTime = i+1;
                processes[indx].turnAroundTime = processes[indx].completionTime - processes[indx].arrivalTime;
                processes[indx].waitingTime = processes[indx].turnAroundTime - processes[indx].burstTime;
            }
        }
        
        vector<TimeSlot>timeLine ; 
        for(int i = 0 ; i < totalTime ; i++)
        {
            int j = i+1 ;
            while(j<totalTime && timeStamps[j] == timeStamps[i])
            {
                j++;
            }
            timeLine.push_back(TimeSlot(i,j,timeStamps[i]));
            i = j-1;
        }    
        displayTimeLine(timeLine);
        displayProcesses(processes);
        float avgWT = getAvgWT(processes);
        cout<<"Average Waiting time : "<<avgWT<<endl;
    }

    void priorityNonPreemptive(vector<Process>processes)
    {
        int n = processes.size();
        sort(processes.begin(), processes.end(), comp);
        vector<TimeSlot>timeLine;
        int currTime = processes[0].arrivalTime;
        bool isExecuted[n] = {0};
        for(int i = 0 ; i < n ; i++)
        {
            int indx = getMaxPriorityProcess( processes, currTime,isExecuted);
            int startTime = currTime ; 
            currTime = currTime + processes[indx].burstTime;
            isExecuted[indx] = true;
            processes[indx].completionTime = currTime;
            processes[indx].turnAroundTime = processes[indx].completionTime - processes[indx].arrivalTime;
            processes[indx].waitingTime = processes[indx].turnAroundTime - processes[indx].burstTime;
            timeLine.push_back(TimeSlot(startTime, currTime, processes[indx].id));
        }
        displayTimeLine(timeLine);
        displayProcesses(processes);
    }

    void roundRobin(vector<Process>processes, int timeQuant)
    {
        int n = processes.size();
        sort(processes.begin(), processes.end(), comp);
        int remainingTime[n];
        for(int i = 0 ; i < n ; i++)
        {
            remainingTime[i] = processes[i].burstTime;
        }

        int currTime = processes[0].arrivalTime;
        queue<int>q;
        vector<TimeSlot>timeLine;
        q.push(0);
        while(!q.empty())
        {
            int startTime = currTime;
            int indx = q.front();
            q.pop();
            int executionTime = min(timeQuant,remainingTime[indx]);
            currTime = currTime + executionTime;
            remainingTime[indx] = remainingTime[indx] - executionTime;
            updateQueue(q,startTime,currTime,processes,remainingTime);
            if(remainingTime[indx] == 0)
            {
                processes[indx].completionTime = currTime;
                processes[indx].turnAroundTime = processes[indx].completionTime - processes[indx].arrivalTime;
                processes[indx].waitingTime = processes[indx].turnAroundTime - processes[indx].burstTime;
            }
            else
            {
                q.push(indx);
            }
            timeLine.push_back(TimeSlot(startTime, currTime, processes[indx].id));
            
        }

        displayTimeLine(timeLine);
        displayProcesses(processes);

    }
};

int main()
{
    // FCFS_TC#1
    // vector<Process>p;
    // p.push_back(Process(1,2,6));
    // p.push_back(Process(2,5,2));
    // p.push_back(Process(3,1,8));
    // p.push_back(Process(4,0,3));
    // p.push_back(Process(5,4,4));

    // ----------------------------------------------------
    // FCFS_TC#2
    // vector<Process>p;
    // p.push_back(Process(1,0,10));
    // p.push_back(Process(2,10,5));
    // p.push_back(Process(3,18,8));
    
    // Scheduler sc;
    // sc.FCFS(p);

    // ==========================================================================
    // SJFP_TC#1
    // vector<Process>processes;
    // processes.push_back(Process(1,2,6));
    // processes.push_back(Process(2,5,2));
    // processes.push_back(Process(3,1,8));
    // processes.push_back(Process(4,0,3));
    // processes.push_back(Process(5,4,4));
    // Scheduler sc;
    // sc.SJFPreemptive(processes);

    // ==========================================================================
    // vector<Process>processes;
    // processes.push_back(Process(1,0,3,2));
    // processes.push_back(Process(2,2,5,6));
    // processes.push_back(Process(3,1,4,3));
    // processes.push_back(Process(4,4,2,5));
    // processes.push_back(Process(5,6,9,7));
    // processes.push_back(Process(6,5,4,4));
    // processes.push_back(Process(7,7,10,10));
    // Scheduler sc;
    // sc.priorityNonPreemptive(processes);
    
    // ==========================================================================
    vector<Process>processes;
    processes.push_back(Process(1,0,5));
    processes.push_back(Process(2,1,6));
    processes.push_back(Process(3,2,3));
    processes.push_back(Process(4,3,1));
    processes.push_back(Process(5,4,5));
    processes.push_back(Process(6,6,4));
    Scheduler sc;
    sc.roundRobin(processes,4);
//     P1	0	5
// P2	1	6
// P3	2	3
// P4	3	1
// P5	4	5
// P6	6	4


    return 0 ;
}
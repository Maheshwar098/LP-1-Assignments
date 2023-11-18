#include <iostream>
#include <vector>
#include <algorithm>
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

    public : 
    Process(int id, int arrivalTime, int burstTime)
    {
        this->id = id  ;
        this->arrivalTime = arrivalTime ;
        this->burstTime = burstTime ;
        this->completionTime = -1;
        this->waitingTime = -1;
        this->turnAroundTime = -1;
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
        int totalTime = getTotalTime(processes);
    }
};

int main()
{
    // FCFSTC#1
    // vector<Process>p;
    // p.push_back(Process(1,2,6));
    // p.push_back(Process(2,5,2));
    // p.push_back(Process(3,1,8));
    // p.push_back(Process(4,0,3));
    // p.push_back(Process(5,4,4));

    // ----------------------------------------------------
    // FCFSTC#2
    // vector<Process>p;
    // p.push_back(Process(1,0,10));
    // p.push_back(Process(2,10,5));
    // p.push_back(Process(3,18,8));
    
    // Scheduler sc;
    // sc.FCFS(p);

    // ==========================================================================
    Scheduler sc;
    sc.FCFS(p);

    return 0 ;
}
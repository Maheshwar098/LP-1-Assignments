#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

class Process
{
    public: 
    int id;
    bool isActive ; 

    public:
    Process(int id)
    {
        this->id =id ;
        isActive = true;
    }
};

class BullyElector
{
    private : 
    vector<Process>processes;

    public : 
    BullyElector(vector<Process>processes)
    {
        this->processes = processes;
    }

    void bullyElection(int deActivatedNodeId, int initiatorId)
    {
        int n = processes.size();
        int winnerId = -1;
        processes[deActivatedNodeId-1].isActive = false;

        int startIndx = initiatorId-1;

        for(int i = startIndx ; i < n ; i++)
        {
            if(processes[i].isActive)
            {
                for(int j = i+1 ; j < n ; j++)
                {
                    cout<<"Process "<<processes[i].id<<" sends message to "<<processes[j].id<<endl;
                }
                bool hasGotResponse = false;
                for(int j = i+1 ; j < n ; j++)
                {
                    if(processes[j].isActive)
                    {
                        cout<<"Process "<<processes[i].id<<" receives response from "<<processes[j].id<<endl;
                        hasGotResponse = true;
                    }
                }

                if(!hasGotResponse)
                {
                    cout<<"Process "<<processes[i].id<<" doesnt receive response"<<endl;
                    cout<<"Process "<<processes[i].id<<" wins the election"<<endl;
                    winnerId = processes[i].id;
                }
            }
            
        }
    
        for(int i = 0 ; i < n ; i++)
        {
            cout<<"Process "<<winnerId<<" sends victory message to Process "<<processes[i].id<<endl;
        }
    }
};

class RingElector
{
    private : 
    vector<Process>processes;

    public : 
    RingElector(vector<Process>processes)
    {
        this->processes = processes;
    }
    
    void displayVector(vector<int>v)
    {
        int len = v.size();
        for(int i = 0 ; i < len ; i++)
        {
            cout<<v[i]<<"   ";
        }
        cout<<endl;
    }

    void RingElection(int deactivatedId, int initiatorId)
    {
        processes[deactivatedId-1].isActive = false;
        int n = processes.size();
        vector<int>status;
        int i = (initiatorId-1)%n;
        while(true)
        {
            if(processes[i].isActive)
            {
                status.push_back(processes[i].id);
            }

            cout<<processes[i].id<<" : ";
            displayVector(status);
            i = (i+1)%n;

            if(processes[i].id == initiatorId)
            {
                break;
            }
        }

        cout<<initiatorId<<" : ";
        displayVector(status);
        int len = status.size();
        int maxId = -1 ;
        for(int i = 0 ; i < len ; i++)
        {
            maxId = max(status[i],maxId);
        }
        cout<<"New coordiantor :  Process "<<maxId<<endl;
    }
};

int main()
{
    vector<Process>processes;
    processes.push_back(Process(1));
    processes.push_back(Process(2));
    processes.push_back(Process(3));
    processes.push_back(Process(4));
    processes.push_back(Process(5));
    processes.push_back(Process(6));
    processes.push_back(Process(7));
    // BullyElector e(processes);
    // e.bullyElection(7,4);
    // cout<<"-----------------------------------------------"<<endl;
    // e.bullyElection(6,2);
    
    RingElector r(processes); 
    r.RingElection(7,4);
    cout<<"-----------------------------------------------"<<endl;
    r.RingElection(6,3);


    return 0 ;
}
#include<bits/stdc++.h>
using namespace std;

class Process{
    public:
    int pId;
    bool isActive;
    
    Process(int pId){
        this->pId = pId;
        isActive = true;
    }
};

class Bully{
    public:

    vector<Process> p;
    int currCoordId;

    Bully(int n){
        
        for(int i=0; i<n; i++){
            Process newP(i);
            p.push_back(newP);
        }

        int id;
        cout<<"Enter current coordinator id : ";
        cin>>id;
        this->currCoordId = id;

    }

    void failCurrCoordinator(){
        for(int i=0; i<p.size(); i++){
            if(p[i].pId == currCoordId){
                p[i].isActive = false;
                currCoordId = -1;
            }
        }
    }

    bool startElection(int pId, int &newId){

            if(p[pId].isActive == false){
                return false;
            }

            bool result = false;
            
            if(pId<p.size()){
                for(int i=pId+1; i<p.size(); i++){
                    result = result || startElection(i, newId);
                }
                if(result==false && newId==-1){
                    newId = pId;
                    this->currCoordId = newId;
                }
            }
            
            return result;
    }

};

int main(){
    int n = -1;
    Bully b(7);
    cout<<b.currCoordId<<endl;
    b.failCurrCoordinator();
    b.startElection(3, n);
    cout<<b.currCoordId<<endl;
    cout<<n<<endl;
    n = -1;
    b.failCurrCoordinator();
    b.startElection(3, n);
    cout<<b.currCoordId<<endl; 
    cout<<n<<endl;
    return 0;
}
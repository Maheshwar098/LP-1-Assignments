#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std ; 

void displayArray(int arr[], int n);

class PageReplacementHandler
{
    private : 

    void displayPageStates(vector<vector<int>>memoryStates, int pageCount, int memoryCapacity)
    {
        for(int i = 0 ; i < memoryCapacity ; i++)
        {
            for(int j =0 ; j < pageCount ; j++)
            {
                cout<<memoryStates[j][i]<<"   ";
            }
            cout<<endl;
        }
    }

    public : 

    void FIFO(int pages[], int pageCount , int memoryCapacity)
    {
        displayArray(pages, pageCount);
        queue<int>pagesQueue ; 
        vector<vector<int>>memoryStates;
        int pageFaultCnt= 0 ;
        for(int i = 0 ; i < pageCount ; i++)
        {
            int currPage = pages[i];
            bool hit = false ; 
            queue<int>tempQ = pagesQueue ; 
            vector<int>memoryState(memoryCapacity, -1) ;
            int index = 0 ;
            while(!tempQ.empty())
            {
                int front = tempQ.front();
                if(front == currPage)
                {
                    hit = true ; 
                }
                tempQ.pop();
                memoryState[index] = front ; 
                index++;
            } 

            if(!hit)
            {
                pageFaultCnt++;
                if(pagesQueue.size()<memoryCapacity)
                {
                    pagesQueue.push(currPage);
                    memoryState[pagesQueue.size()-1] = currPage;
                }
                else
                {
                    pagesQueue.pop();
                    pagesQueue.push(currPage);
                    for(int j = 0 ; j < memoryCapacity-1 ; j++)
                    {
                        memoryState[j] = memoryState[j+1];
                    }
                    memoryState[memoryCapacity-1] = currPage;
                }
                
            }
            memoryStates.push_back(memoryState);
        }

        displayPageStates(memoryStates,pageCount,memoryCapacity);
        cout<<"Pagefaults : "<<pageFaultCnt<<endl;
    }

    void optimalPageReplacementAlgo(int pages[], int pageCount, int memoryCapacity)
    {
        vector<vector<int>>memoryStates ; 
        int pageFaultCnt = 0 ;
        for(int i = 0 ; i < pageCount ; i++)
        {
            int currPage = pages[i];
            if(memoryStates.empty())
            {
                vector<int>memoryState(memoryCapacity, -1);
                memoryState[0] = currPage ;
                memoryStates.push_back(memoryState);
            }
            else
            {
                vector<int>recentState = memoryStates.back();
                bool isInserted = false;
                bool hit = false ;
                for(int j = 0 ; j <memoryCapacity ; j++)
                {
                    if(recentState[j]==-1)
                    {
                        recentState[j] = currPage;
                        isInserted = true ;
                        break;
                    }
                    if(recentState[j] == currPage)
                    {
                        hit = true ; 
                        break ;
                    }
                }

                if(!isInserted && !hit)
                {
                    pageFaultCnt++;
                    vector<int>distTrack(memoryCapacity, INT_MAX);
                    for(int j = 0 ; j < memoryCapacity ; j++)
                    {
                        int page = recentState[j];
                        for(int k = i+1 ; k < pageCount ; k++)
                        {
                            if(pages[k] == page && distTrack[j]== INT_MAX)
                            {
                                distTrack[j] = k ; 
                            }
                        }
                    }

                    int maxDist = INT_MIN; 
                    int maxIndex = -1 ; 
                    for(int j = 0 ; j < memoryCapacity ; j++)
                    {
                        if(distTrack[j]>maxDist)
                        {
                            maxDist = distTrack[j];
                            maxIndex = j ; 
                        }
                    }
    
                    recentState[maxIndex] = currPage;
                    
                }

                memoryStates.push_back(recentState);
            }
   
        }
        displayPageStates(memoryStates, pageCount, memoryCapacity);
        cout<<"PageFaults : "<<pageFaultCnt<<endl;
    }

    void MostRecentlyUsed(int pages[], int pageCount, int memoryCapacity)
    {
        vector<vector<int>>memoryStates;
        for(int i = 0 ; i < pageCount ; i++)
        {
            int currPage = pages[i];
            if(memoryStates.empty())
            {
                vector<int>memoryState(memoryCapacity, -1);
                memoryState[0] = currPage ; 
                memoryStates.push_back(memoryState);
            }
            else
            {
                vector<int>recentState = memoryStates.back();
                bool hit = false ; 
                bool isInserted = false ; 
                for(int j = 0 ; j < memoryCapacity ; j++)
                {
                    if(recentState[j] == -1)
                    {
                        recentState[j] = currPage ; 
                        isInserted = true ; 
                        break ;
                    }
                    if(recentState[j] == currPage)
                    {
                        hit = true ; 
                        break ;
                    }
                }

                if(!hit && !isInserted)
                {
                    vector<int>distTrack(memoryCapacity, -1);
                    for(int j = 0 ; j < memoryCapacity ; j++)
                    {
                        int page = recentState[j] ; 
                        for(int k = i-1 ; k>=0 ; k--)
                        {
                            if(pages[k] == page && distTrack[j] == -1)
                            {
                                distTrack[j] = k ;
                            }
                        }
                    }

                    int minDist = INT_MIN ; 
                    int minIndex = -1 ; 
                    for(int j = 0 ; j < memoryCapacity ; j++)
                    {
                        if(distTrack[j] > minDist)
                        {
                            minDist = distTrack[j];
                            minIndex = j ; 
                        }
                    }

                    recentState[minIndex] = currPage;
                    
                }
                memoryStates.push_back(recentState);
            }
        }

        displayPageStates(memoryStates, pageCount, memoryCapacity);
    }

};

void displayArray(int arr[], int n)
{
    for(int i = 0 ; i < n; i++)
    {
        cout<<arr[i]<<"  ";
    }
    cout<<endl;
}

int main()
{
    PageReplacementHandler h ; 
    // int FIFOpages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
    // int FIFOpageCnt = 20 ; 
    // int FIFOmemoryCapacity = 3 ; 
    // h.FIFO(FIFOpages,FIFOpageCnt, FIFOmemoryCapacity);

    // int OptPages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
    // int optPageCnt = 20;
    // int optMemoryCapacity = 3 ; 
    // h.optimalPageReplacementAlgo(OptPages,optPageCnt,optMemoryCapacity);

    // int OptPages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,3};
    // int optPageCnt = 14;
    // int optMemoryCapacity = 4 ; 
    // h.optimalPageReplacementAlgo(OptPages,optPageCnt,optMemoryCapacity);

    int MRUPages[] = {7,0,1,2,0,3,0,4,2,3,0,3,2,3};
    int MRUPgCnt = 14;
    int MRUMemoryCapacity = 4;
    h.MostRecentlyUsed(MRUPages, MRUPgCnt, MRUMemoryCapacity);


    return 0 ; 
}
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class PageManager
{
    private :
    bool checkForHit(vector<int>frameState, int currPage)
    {
        int n = frameState.size();
        for(int i = 0 ; i < n ; i++)
        {
            if(frameState[i] == currPage)
            {
                return true;
            }
        }
        return false;
    }
    
    void displayFrameStates(vector<vector<int>>frameStates)
    {
        int n = frameStates.size();
        int m = frameStates[0].size();

        for(int i = 0 ; i < m ; i++)
        {
            for(int j = 0 ; j < n ; j++)
            {
                cout<<frameStates[j][i]<<"    ";
            }
            cout<<endl;
        }
    }

    int getTargetIndxOpt(vector<int>pageSeq, vector<int>frameState, int currIndx )
    {
        int targetIndx = 0 ;
        int maxDist = -1 ;

        int n = pageSeq.size();
        int frameSize = frameState.size();

        for(int i = 0 ; i < frameSize ; i++)
        {
            int currPg = frameState[i];
            int dist = 100;
            int index = i;
            for(int j = currIndx ; j < n ; j++)
            {
                if(pageSeq[j] == currPg )
                {
                    dist = j;
                }
            }
            if(dist>maxDist)
            {
                maxDist = dist;
                targetIndx = i;
            }
        }

        return targetIndx;
    }

    int getTargetIndxLRU(vector<int>pageSeq, vector<int>frameState, int currIndx)
    {
        int frameSize = frameState.size();
        int targetIndx = 0 ;
        int maxDist = -1;
        for(int i = 0 ; i < frameSize ; i++)
        {
            int dist = 100;
            for(int j = currIndx ; j>=0 ; j-- )
            {
                if(frameState[i] == pageSeq[j])
                {
                    dist = i-j;
                    break;
                }
            }

            if(dist>maxDist)
            {
                maxDist = dist;
                targetIndx = i;
            }
        }

        return targetIndx;
    }

    public : 
    void FIFO(vector<int>pageSeq, int frameSize)
    {
        int n = pageSeq.size();
        vector<vector<int>>frameStates;
        vector<int>frameState(frameSize,-1);
        int faultCnt = 0 ;
        int currIndx = 0 ;
        for(int i = 0 ; i < n ; i++)
        {
            if(!checkForHit(frameState, pageSeq[i]))
            {
                frameState[currIndx] = pageSeq[i];
                currIndx = (currIndx+1)%frameSize;
                faultCnt++;
            }
            frameStates.push_back(frameState);
        }

        displayFrameStates(frameStates);
        cout<<"Total page faults : "<<faultCnt<<endl;
    }

    void optimalPgReplacement(vector<int>pageSeq, int frameSize)
    {
        int n = pageSeq.size();

        vector<vector<int>>frameStates;
        vector<int>frameState(frameSize,-1);
        int indx = 0 ;
        int faultCnt = 0 ;
        for(int i = 0 ; i < n ; i++)
        {
            int currPage = pageSeq[i];

            if(!checkForHit(frameState,currPage))
            {
                if(indx < frameSize)
                {
                    frameState[indx] = currPage;
                    indx++;
                }
                else
                {
                    int targetIndx = getTargetIndxOpt(pageSeq,frameState,i);
                    frameState[targetIndx] = currPage;
                }
                faultCnt++;
            }

            frameStates.push_back(frameState);
        }
        displayFrameStates(frameStates);
        cout<<"Page Faults : "<<faultCnt<<endl;
    }

    void LeastRecentlyUsed(vector<int>pageSeq, int frameSize)
    {
        int n = pageSeq.size();
        vector<vector<int>>frameStates;
        vector<int>frameState(frameSize, -1);
        int indx = 0 ;
        int faultCnt = 0;
        for(int i = 0 ; i < n ; i++)
        {
            int currPage = pageSeq[i];

            if(!checkForHit(frameState, currPage))
            {
                if(indx < frameSize)
                {
                    frameState[indx] = currPage ;
                    indx++;
                }
                else
                {
                    int indx = getTargetIndxLRU(pageSeq,frameState,i);
                    frameState[indx] = currPage;
                }
                faultCnt++;
            }

            frameStates.push_back(frameState);
        }
    
        displayFrameStates(frameStates);
        cout<<"Fault cnt : "<<faultCnt<<endl;
    }
};

int main()
{
    PageManager pm ;
    // vector<int>pageSeq{0, 2, 1, 6, 4, 0, 1, 0, 3, 1, 2, 1};
    // int frameSize = 4;
    // pm.FIFO(pageSeq, frameSize);

    // vector<int>pageSeq{7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 3};
    // int frameSize = 4;
    // pm.optimalPgReplacement(pageSeq,frameSize);

    vector<int>pageSeq{7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 3};
    int frameSize = 4;
    pm.LeastRecentlyUsed(pageSeq,frameSize);

    return 0;
}
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> getWords(string line)
{
    int i = 0 ;
    int len = line.length();
    vector<string>words;
    while(i<len)
    {
        while(i < len && line[i] == ' ')
        {
            i++;
        }

        string word = "";

        while( i < len && line[i] != ' ')
        {
            word = word + line[i];
            i++;
        }

        if(word != "")
        {
            words.push_back(word);
        }
    }

    return words;
}

int getNumber(string strNum)
{
    int len = strNum.length();
    int num = 0 ;
    for(int i = 0 ; i < len ; i++)
    {
        num = num*10 + (strNum[i]-'0');
    }

    return num ;
}

int main()
{


    return 0 ;
}
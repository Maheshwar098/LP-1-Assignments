#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

// Extract Words out of line
vector<string>getWords(string line)
{
    int i = 0 ;
    int len = line.length();
    vector <string> words;
    while(i<len)
    {
        while(i<len && line[i] == ' ')
        {
            i++;
        }

        string word = "";
        int j = i ;

        while(j<len && line[j] != ' ')
        {
            word = word + line[j];
            j++;
        }

        words.push_back(word);

        i = j;
    }

    return words;
}

// define opcode tables
void defineOpcodeTabs(map<string,int>&IS, map<string,int>&AD, map<string,int>&DL)
{
    IS["STOP"] = 0;
    IS["ADD"] = 1;
    IS["SUB"] = 2;
    IS["MULT"] = 3 ;
    IS["MOVER"] = 4;
    IS["MOVEM"] = 5;
    IS["COMP"] = 6;
    IS["BC"] = 7;
    IS["DIV"] = 8 ;
    IS["READ"] = 9 ;
    IS["PRINT"] = 10;

    AD["START"] = 1;
    AD["END"] = 2;
    AD["ORIGIN"] = 3;
    AD["EQU"] = 4;
    AD["LTORG"] = 5;

    DL["DC"] = 1;
    DL["DS"] = 2 ;
}

// Pass 1
void performPass1(string fileName)
{

    map<string,int>IS;
    map<string,int>AD;
    map<string,int>DL;

    defineOpcodeTabs(IS,AD,DL);

    fstream fin;
    fin.open(fileName);

    while(!fin.eof())
    {
        string line;
        getline(fin,line);
        vector<string>words = getWords(line);
        
    }

    fin.close();
}

int main()
{
    string fileName = "assembly_code.txt";
    
    performPass1(fileName);
    
    cout<<"Successfully executed"<<endl;
    return 0;
}
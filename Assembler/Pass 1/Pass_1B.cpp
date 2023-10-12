#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
using namespace std;



class Instruction
{
    public :
    string label;
    string opcode;
    string operand_1;
    string operand_2;

    public:
    Instruction(string label,string opcode,string operand_1,string operand_2)
    {
        this->label = label ;
        this->opcode = opcode ;
        this->operand_1 = operand_1;
        this->operand_2 = operand_2;

    }
};

class ICGenerator
{
    private : 
    map<string,int>IS;
    map<string,int>AD;
    map<string,int>DL;
    map<string,int>R;

    public : 
    ICGenerator()
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

    private:
    vector<string>getWords(string line)
    {
        int len = line.length();
        vector<string>words;
        int i = 0;
        string word = "";
        while(i<len)
        {
            if(line[i]!= ' ')
            {
                word = word+line[i];
            }
            else
            {
                if(word!="")
                {
                    words.push_back(word);
                    word = "";
                }
            }
            i++;
        }

        words.push_back(word);

        return words;
    }
    
    vector<string>readFile(string fileName)
    {
        vector<string>codeLines;

        ifstream fin;
        fin.open("assembly_code.txt");
        while(!(fin.eof()))
        {
            string line;
            getline(fin,line);
            codeLines.push_back(line);
        }
        fin.close();

        return codeLines;

    }

    bool checkForLabel(string word)
    {
        if(IS[word] || AD[word] || DL[word])
        {
            return false;
        }
        if(word == "STOP")
        {
            return false;
        }
        return true;
    }

    Instruction seggregateTokens(vector<string>words)
    {

    }

    vector<Instruction>getPreprocessedInstructions(vector<string>codeLines)
    {
        int noOfLines = codeLines.size();

        for(int i = 0 ; i < noOfLines ; i++)
        {
            string line = codeLines[i];
            vector<string>words = getWords(line);
        }
    }

    public:
    void performPass1(string fileName)
    {
        vector<string>codeLines = readFile(fileName);
        for(int i = 0 ; i < codeLines.size() ; i++)
        {
            // cout<<codeLines[i]<<endl;
        }
        vector<Instruction>preprocessedInstr = getPreprocessedInstructions(codeLines);
    }

};

int main()
{
    string fileName = "assembly_code.txt";
    ICGenerator generator;
    generator.performPass1(fileName);
    return 0 ;
}
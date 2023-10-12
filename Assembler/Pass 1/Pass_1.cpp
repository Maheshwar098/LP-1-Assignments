#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class TableEntry
{
    public : 
    int id;
    string value;
    int address;

    public:
    TableEntry(int id, string value, int address)
    {
        this->id = id ;
        this->value = value;
        this->address = address;
    }
};

class SymbolTableEntry : public TableEntry
{
    public :
    SymbolTableEntry(int id, string value, int address):TableEntry(id,value,address)
    {
        
    }
};

class LiteralTableEntry : public TableEntry
{
    public:
    LiteralTableEntry(int id, string value, int address):TableEntry(id,value,address)
    {
        
    }
};

// check if string is a number
bool isNumber(string str)
{
    for(int i = 0 ; i < str.length() ; i++)
    {
        if(str[i]-'0'>9)
        {
            return false;
        }
    }
    return true;
}

// string to number
int stringToNum(string strNum)
{
    int len = strNum.length();
    int num = 0;
    for(int i = 0 ; i < len ; i++)
    {
        num = num * 10 + (int)(strNum[i] - '0');
    }
    return num;
}

// number to string
string numToStr(int num)
{
    string strNum = "";
    while(num)
    {
        int rem = num % 10;
        int ascii  = rem + (int)'0';
        char digit = ascii;
        strNum = digit + strNum;
        num = num / 10;
    }

    return strNum;
}


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

// define register values tables
void defineRegesterCodes(map<string,int>&R)
{
    R["AREG"] = 1;
    R["BREG"] = 2;
    R["CREG"] = 3;
    R["DREG"] = 4 ;
}

// check for label
bool checkForLabel(map<string,int>IS, map<string,int>AD, map<string,int>DL, string word)
{
    if( IS[word] || AD[word] || DL[word]) 
    {
        return false;
    }

    if(word == "STOP")
    {
        return false;
    }

    return true;
}

// update symbol table
void updateSymTab(string symbol, int address, vector<SymbolTableEntry>&symTab)
{
    int len = symTab.size();
    bool isFound = false;
    for(int i = 0 ; i < len ; i++)
    {
        if(symTab[i].value == symbol)
        {
            symTab[i].address = address;
            isFound = true;
            break;
        }
    }

    if(!isFound)
    {
        int index = symTab.back().id+1;
        SymbolTableEntry symTabEntry(index,symbol,address);
        symTab.push_back(symTabEntry);
    }
}

// update literal Table
void updateLitTab(string literal, int address, vector<LiteralTableEntry>&litTab,vector<int>poolTab)
{
    int j = 0 ;
    int litTabSize = litTab.size();
    while(j<litTab.size() && litTab[j].id != poolTab.back())
    {
        j++;
    }

    for(int i= j ; i < litTab.size() ; i++ )
    {
        if(litTab[i].value == literal)
        {
            return;
        }
    }

    LiteralTableEntry newLitTabEntry(litTab.back().id+1,literal,-1);
    litTab.push_back(newLitTabEntry);

}

// process instruction
void processLiterals(vector<int>&poolTab, vector<LiteralTableEntry>&litTab, vector<string>&pass1Op, int &lc)
{
    int i = 0 ;
    int litTabLen = litTab.size();
    while(i<litTabLen && litTab[i].id<poolTab.back())
    {
        i++;
    }

    for(int j = i ; j < litTabLen  ; j++)
    {
        litTab[j].address = lc;
        string literal = litTab[j].value;
        int literalLen = literal.length();
        string litVal = (literal.substr(2,literalLen-3));
        string processedString = numToStr(lc) + "   (DL,1),(C,"+litVal+")";
        pass1Op.push_back(processedString);    
        lc++;
    }
    int poolTbEntry = litTab.back().id+1;
    poolTab.push_back(poolTbEntry);
}

// equate label addresses
void equateLblAddresses(string lbl1, string lbl2, vector<SymbolTableEntry>symTab)
{
    int symTblen = symTab.size();
    int indx = 0 ;
    for(int j = 0 ; j < symTblen; j++)
    {
        if(symTab[j].value == lbl2)
        {
            indx = j ;
            break;
        }
    }

    for(int j = 0 ; j < symTblen ; j++)
    {
        if(symTab[j].value == lbl1)
        {
            symTab[j].address = symTab[indx].address ;
            break;
        }
    }
}

// get literal id
int getLiteralId(string literal, vector<LiteralTableEntry>litTab)
{
    for(int i = 0 ; i < litTab.size() ; i++)
    {
        if(litTab[i].value == literal)
        {
            return litTab[i].id;
        }
    }

    return -1;
}

// Pass 1
void performPass1(string fileName)  
{

    map<string,int>IS;
    map<string,int>AD;
    map<string,int>DL;
    map<string,int>R;

    vector<SymbolTableEntry>symTab;
    vector<LiteralTableEntry>litTab;
    vector<int>poolTab;

    vector<string>pass1Op;

    poolTab.push_back(1);

    defineOpcodeTabs(IS,AD,DL);

    defineRegesterCodes(R);

    fstream fin;
    fin.open(fileName);

    int lc = 0 ;
    vector<string>pass1Inp ;

    while(!fin.eof())
    {
        string line;
        getline(fin,line);
        pass1Inp.push_back(line);
    }

    int inpLinesCnt = pass1Inp.size();

    for(int i = 0 ; i < inpLinesCnt ; i++)
    {
        string line = pass1Inp[i];
        vector<string>words = getWords(line);
        vector<string>refwords = words;
    
        if(checkForLabel(IS,AD,DL,words[0]))
        {
            string symbol = words[0];
            updateSymTab(symbol, lc, symTab);
            if(words.size() == 3)
            {
                if(words[1] == "EQU")
                {
                    equateLblAddresses(words[0],words[1],symTab);
                    continue;
                }
            }
            words.erase(words.begin());
        }
        
        int len = words.size();

        if(len == 1)
        {
            if(words[0] == "END" || words[0] == "LTORG")
            {
                processLiterals(poolTab,litTab,pass1Op,lc);
            }
            else if(words[0] == "START")
            {
                string processedLine = "(AD,1)";
                pass1Op.push_back(processedLine);
                lc = 0;
            }
            else if (words[0] == "STOP")
            {
                string processedLine = "(IS,0)";
                pass1Op.push_back(processedLine);
                lc++;
            }
            else
            {
                cout<<"Invalid instruction"<<endl;
                break;
            }
            
        }       
        else if (len == 2)
        {
            if(words[0] == "START")
            {
                string processedString = "-     (AD,1),(C,"+words[1]+")";
                pass1Op.push_back(processedString);
                lc = stringToNum(words[1]);
            }
            else if(words[0] == "ORIGIN")
            {
                string processedString = "-     (AD,3),(C,"+words[1]+")";
                pass1Op.push_back(processedString);
                lc = stringToNum(words[1]);
            }
            else if(words[0] == "DC")
            {
                int constlen = words[1].length();
                string processedString = numToStr(lc)+"  (DL,1),(C,"+words[1].substr(1,constlen-2)+")";
                pass1Op.push_back(processedString);
                lc++;
            }
            else if (words[0] == "DS")
            {
                string processedString = numToStr(lc)+"  (DL,2),(C,"+words[1]+")";
                pass1Op.push_back(processedString);
                lc = lc + stringToNum(words[1]);

            }
            else if(IS[words[0]])
            {
                string processedString = numToStr(lc)+"  (IS,"+numToStr(IS[words[0]])+"),";

            }
            string processedString = "";
            if(words[1][0] == '=')
            {
                updateLitTab(words[1],-1,litTab,poolTab);
                int literalId = getLiteralId(words[1],litTab);
                processedString = processedString + "(L,"+numToStr(literalId)+")";
            }
            else if (!isNumber(words[1]))
            {
                updateSymTab(words[1],-1,symTab);
                // processedString = processedString + 
            }
        
        }
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
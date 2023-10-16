#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class ICEntry
{
    public : 
    string lc ; 
    string opcode ;
    string operand_1;
    string operand_2 ;

    public : 
    ICEntry()
    {
        this->lc = "0" ;
        this->opcode = "";
        this->operand_1 = "";
        this->operand_2 = "";
    }

    ICEntry(string lc, string opcode, string operand_1, string operand_2)
    {
        this->lc = lc ;
        this->opcode = opcode ; 
        this->operand_1 = operand_1 ;
        this->operand_2 = operand_2 ;
    }
};

class SymTabEntry
{
    public :
    string id;
    string symbol;
    string address;

    public:
    SymTabEntry()
    {
        this->id = "";
        this->symbol = "";
        this->address = "";
    }
};

class LitTabEntry
{
    public:
    string id;
    string literal;
    string address;

    public:
    LitTabEntry()
    {
        this->id = "";
        this->literal = "";
        this->address = "";
    }
};

class MachineCodeEntry
{
    public :
    string lc;
    string opcode;
    string operand_1;
    string operand_2;

    public:
    MachineCodeEntry()
    {
        this->lc = "";
        this->opcode = "";
        this->operand_1 = "";
        this->operand_2 = "";
    }
};

class MachineCodeGenerator
{
    private : 
    int strToNum(string strNum)
    {
        int len = strNum.length();
        int num = 0;
        for(int i = 0 ; i < len ; i++)
        {
            num = num * 10 + (strNum[i]-'0');
        }
        return num;
    }

    bool isStrNum(string strNum)
    {
        int len = strNum.length();

        for(int i = 0 ; i < len ; i++)
        {
            if(strNum[i]-'0' > 9)
            {
                return false;
            }
        }

        return true ;
    }

    vector<string>getWords(string line)
    {
        int i = 0 ;
        int linelen = line.length();
        string word = "";
        vector<string>words;
        while(i<linelen)
        {
            if(line[i] == ' ')
            {
                if(word != "")
                {
                    words.push_back(word);
                    word = "";
                }
            }
            else
            {
                word = word + line[i];
            }
            i++;
        }

        if(word != "")
        {
            words.push_back(word);
        }

        return words;
        
    }
    
    vector<vector<string>>extractWords(vector<string> lines)
    {
        vector<vector<string>>lineWords ;
        int lineCnt = lines.size();

        for(int i=0 ; i < lineCnt ; i++)
        {
            vector<string>words = getWords(lines[i]);
            if(words.size()>0)
            {
                lineWords.push_back(words);
            } 
        }

        return lineWords;
    }

    vector<ICEntry>getICTab(vector<vector<string>>ICwords)
    {
        int lineCnt = ICwords.size();
        vector<ICEntry>ICTab;
        for(int i = 0 ; i < lineCnt ; i++)
        {
            vector<string>words = ICwords[i];
            int wordslen = words.size();
            ICEntry ICline;
            int j = 0;
            if(!isStrNum(words[0]))
            {
                continue;
            }

            ICline.lc = words[0];
            ICline.opcode = words[1];
            if(wordslen>2)
            {
                ICline.operand_1 = words[2];
            }
            if(wordslen>3)
            {
                ICline.operand_2 = words[3];
            }

            ICTab.push_back(ICline);

        }

        return ICTab;
    }

    vector<SymTabEntry>getSymTab(vector<vector<string>>symTabWords)
    {
        int len = symTabWords.size();
        vector<SymTabEntry>symTab;
        for(int i = 0 ; i < len ; i++)
        {
            vector<string>words = symTabWords[i];
            SymTabEntry symTabLine;
            symTabLine.id = words[0];
            symTabLine.symbol = words[1];
            symTabLine.address = words[2];
            symTab.push_back(symTabLine);
        }

        return symTab;
    }

    vector<LitTabEntry>getLitTab(vector<vector<string>>litTabWords)
    {
        int litTabLen = litTabWords.size();
        vector<LitTabEntry>litTab;
        for(int i = 0 ; i < litTabLen ; i++)
        {
            vector<string>words = litTabWords[i];
            LitTabEntry litTabline ;
            litTabline.id = words[0];
            litTabline.literal = words[1];
            litTabline.address = words[2];
            litTab.push_back(litTabline);
        }

        return litTab;
    }

    vector<string> readFile(string fileName)
    {
        vector<string>fileContents ;
        ifstream fin;
        fin.open(fileName);
        while(!fin.eof())
        {
            string line ;
            getline(fin,line);
            fileContents.push_back(line);
        }

        return fileContents;
    }
    
    string getSymTabAddress(string id, vector<SymTabEntry>symTab)
    {
        int len = symTab.size();

        for(int i = 0 ; i < len ; i++)
        {
            if(symTab[i].id==id)
            {
                return symTab[i].address;
            }
        }

        return "-1";
    }
    
    string getLitTabAddress(string id, vector<LitTabEntry>litTab)
    {
        int len = litTab.size();
        for(int i = 0 ; i < len ; i++)
        {
            if(id == litTab[i].id)
            {
                return litTab[i].address;
            }
        }

        return "-1";
    }

    vector<string>getMachineCodeLines(vector<MachineCodeEntry>machineCode)
    {
        int machineCodeLineCnt = machineCode.size();
        vector<string>machineCodeLines ;
        for(int i = 0 ; i < machineCodeLineCnt ; i++)
        {
            string line = machineCode[i].lc+"      "+machineCode[i].opcode+"     "+machineCode[i].operand_1+"      "+machineCode[i].operand_2;
            machineCodeLines.push_back(line);
        }
        return machineCodeLines;
    }
    public : 
    
    void getMachineCode(string ICFile, string symTabFile, string litTabFile)
    {
        vector<string>IClines = readFile(ICFile); 
        vector<string>symTablines = readFile(symTabFile) ;
        vector<string>litTablines = readFile(litTabFile);

        vector<vector<string>>ICwords = extractWords(IClines);
        vector<vector<string>>symTabWords = extractWords(symTablines);
        vector<vector<string>>litTabWords = extractWords(litTablines);

        vector<ICEntry>ICTab = getICTab(ICwords);
        vector<SymTabEntry>symTab = getSymTab(symTabWords);
        vector<LitTabEntry>litTab = getLitTab(litTabWords);

        vector<MachineCodeEntry>machineCode;

        int ICTabLen = ICTab.size();
        for(int i = 0 ; i < ICTabLen ; i++)
        {
            ICEntry entry = ICTab[i];
            string ICopcode = entry.opcode;
            string ICoperand_1 = entry.operand_1;
            string ICoperand_2 = entry.operand_2;
            MachineCodeEntry machineCodeEnt  ;

            machineCodeEnt.lc = entry.lc;

            string opCodeType = ICopcode.substr(1,2); //(IS,04)
            string opCodeId = ICopcode.substr(4,2);
            if(opCodeType == "DL")
            {
                if(opCodeId == "01")
                {
                    machineCodeEnt.opcode = "00";
                    machineCodeEnt.operand_1 = "00";
                    int temp = ICoperand_1.length();
                    machineCodeEnt.operand_2 = ICoperand_1.substr(3,temp-4);//(C,04)
                }
            }
            else
            {
                machineCodeEnt.opcode = opCodeId;
                int len = ICoperand_1.length();
                if(len>0)
                {
                    string num = ICoperand_1.substr(1,len-2);
                    if(!isStrNum(num))
                    {
                        string address="-1";
                        char type = ICoperand_1[1];
                        if(type == 'S')
                        {
                            address = getSymTabAddress(ICoperand_1.substr(3,len-4),symTab);
                        }
                        else
                        {
                            address = getLitTabAddress(ICoperand_1.substr(3,len-4), litTab);
                        }

                        machineCodeEnt.operand_2 =  address;
                    }
                    else
                    {
                        machineCodeEnt.operand_1 = num;
                    }
                }
                
                len = ICoperand_2.length();
                if(len>0)
                {
                    string num = ICoperand_2.substr(1,len-2);
                    if(!isStrNum(num))
                    {
                        char type = ICoperand_2[1];
                        string address = "-1";
                        if(type == 'S')
                        {
                            address = getSymTabAddress(ICoperand_2.substr(3,len-4),
                            symTab);
                        }
                        else
                        {
                            address = getLitTabAddress(ICoperand_2.substr(3,len-4),litTab);
                        }

                        machineCodeEnt.operand_2 = address;

                    }
                    else
                    {
                        machineCodeEnt.operand_2 = num;
                    }
                }
            
                if(opCodeId == "00")
                {
                    machineCodeEnt.operand_1 = "0";
                    machineCodeEnt.operand_2 = "000";
                }
            }

            machineCode.push_back(machineCodeEnt);

        }

        vector<string>machineCodeLines = getMachineCodeLines(machineCode);
        
        int len = machineCodeLines.size();
        for(int i = 0 ; i < len ; i++)
        {
            cout<<machineCodeLines[i]<<endl;
        }
    }

};


int main()
{
    // string ICfile = "test_1_IC.txt";
    // string symTabFile = "test_1_Symtab.txt";
    // string litTabFile = "test_1_litTab.txt";
    string ICfile = "test_2_IC.txt";
    string symTabFile = "test_2_Symtab.txt";
    string litTabFile = "test_2_litTab.txt";

    MachineCodeGenerator mg;

    mg.getMachineCode(ICfile,symTabFile,litTabFile);

    return 0 ;
}
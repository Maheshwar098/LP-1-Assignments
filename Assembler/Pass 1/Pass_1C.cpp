#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

class LitTabEntry
{
    public : 
    int id;
    string literal;
    int address;

    public:
    LitTabEntry(int id,string literal,int address)
    {
        this->id = id ;
        this->literal = literal;
        this->address = address;
    }
};

class SymTabEntry
{
    public :
    int id;
    string symbol;
    int address;

    public:
    SymTabEntry(int id,string symbol, int address)
    {
        this->id = id ;
        this->symbol = symbol;
        this->address = address;
    }
};

class Instruction
{
    public :
    string label;
    string opcode;
    string operand_1;
    string operand_2;

    public:
    Instruction()
    {
        this->label = "";
        this->opcode = "";
        this->operand_1 = "";
        this->operand_2 = "";
    }
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
    map<string,string>IS;
    map<string,string>AD;
    map<string,string>DL;
    map<string,string>R;

    public : 
    ICGenerator()
    {
        IS["STOP"] = "00";
        IS["ADD"] = "01";
        IS["SUB"] = "02";
        IS["MULT"] = "03" ;
        IS["MOVER"] = "04";
        IS["MOVEM"] = "05";
        IS["COMP"] = "06";
        IS["BC"] = "07";
        IS["DIV"] = "08" ;
        IS["READ"] = "09" ;
        IS["PRINT"] = "10";

        AD["START"] = "01";
        AD["END"] = "02";
        AD["ORIGIN"] = "03";
        AD["EQU"] = "04";
        AD["LTORG"] = "05";

        DL["DC"] = "01";
        DL["DS"] = "02" ;

        R["AREG"] = "01";
        R["BREG"] = "02";
        R["CREG"] = "03";
        R["DREG"] = "04";
    }

    private:

    bool isStrNum(string strNum)
    {
        int len = strNum.length();
        for(int i = 0 ; i < len ; i++)
        {
            if(strNum[i]-'0'>9)
            {
                return false;
            }
        }

        return true;
    }
    
    string numToStr(int num)
    {
        string strNum = "";
        int x =num ;

        while(x)
        {
            int rem = x % 10 ;
            char digit = (char)(rem+'0');
            strNum = digit + strNum;
            x = x / 10 ;
        }
        if(!num)
        {
            return "0";
        }
        return strNum;
    }

    int strToNum(string str)
    {
        int len = str.length();
        int num = 0 ;
        for(int i = 0 ; i < len ; i++)
        {
            num = num * 10 + (str[i]-'0');
        }
        return num;
    }

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
        fin.open(fileName);
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
        if(IS[word] != ""|| AD[word]!= "" || DL[word]!= "")
        {
            return false;
        }
        return true;
    }

    Instruction seggregateTokens(vector<string>words)
    {
        int len = words.size();
        Instruction instr;
        bool isLabel=false;
        if(checkForLabel(words[0]))
        {
            isLabel = true;
            instr.label = words[0];
        }

        int i = 0 ;
        if(isLabel)
        {
            i++;
        }

        if(i<len)
        {
            instr.opcode = words[i];
            i++;
        }

        if(i<len)
        {
            instr.operand_1 = words[i];
            i++;
        }

        if(i<len)
        {
            instr.operand_2 = words[i];
        }

        return instr;
    }

    vector<Instruction>getPreprocessedInstructions(vector<string>codeLines)
    {
        int noOfLines = codeLines.size();
        vector<Instruction>instructions;
        for(int i = 0 ; i < noOfLines ; i++)
        {
            string line = codeLines[i];
            vector<string>words = getWords(line);
            Instruction instr = seggregateTokens(words);
            instructions.push_back(instr);
        }

        return instructions;
    }

    void updateSymTab(string label, int address, vector<SymTabEntry>&symTab)
    {
        int len = symTab.size();

        for(int i = 0 ; i < len ; i++)
        {
            if(symTab[i].symbol == label)
            {
                if(symTab[i].address == -1)
                {
                    symTab[i].address = address;
                }
                
                return;
            }
        }

        SymTabEntry newSymTabEntry(symTab.size()+1, label, address);
        symTab.push_back(newSymTabEntry);
        return ;
    }

    void addOpCode(string &proccStr, string opCode)
    {
        if(opCode == "STOP")
        {
            proccStr = proccStr + "(AD,0)";
            return ;
        }

        if(AD[opCode] != "")
        {
            proccStr = proccStr +"(AD,"+AD[opCode]+")";
        }
        else if(IS[opCode] != "")
        {
            proccStr = proccStr +"(IS,"+IS[opCode]+")";
        }
        else if (DL[opCode] != "")
        {
            proccStr = proccStr + "(DL,"+DL[opCode]+")";
        }
        else 
        {
            cout<<"Invalid opcode"<<endl;
        }
    }

    int getLiteralIndx(string literal,vector<LitTabEntry>litTab)
    {
        int len = litTab.size();

        for(int i = 0 ; i < len ; i++)
        {
            if(litTab[i].literal == literal)
            {
                return litTab[i].id;
            }
        }
        return -1;
    }

    void updateLitTab(string literal,vector<LitTabEntry>&litTab, vector<int>poolTab)
    {
        int i = 0 ; 
        int startId = poolTab.back();
        int litTablen = litTab.size();

        while(i<litTablen && litTab[i].id != startId)
        {
            i++;
        }
        if(i == litTablen)
        {
            LitTabEntry newlitTabEntry(startId,literal,-1);
            litTab.push_back(newlitTabEntry);
            return;
        }
        while(i<litTablen && litTab[i].literal != literal)
        {
            i++;
        }
        if(i == litTablen)
        {
            LitTabEntry newlitTabEntry(litTab.back().id+1,literal,-1);
            litTab.push_back(newlitTabEntry);
            return;
        }

        
    }

    int getSymTabIndx(string symbol,vector<SymTabEntry>symTab)
    {
        int len = symTab.size();
        for(int i = 0 ; i < len ; i++)
        {
            if(symTab[i].symbol == symbol)
            {
                return symTab[i].id;
            }   
        }
        return -1;
    }

    void addOperand(string &proccStr, string operand, vector<LitTabEntry>&litTab, vector<SymTabEntry>&symTab, vector<int>&poolTab)
    {
        if(operand[0] == '\'')
        {
            operand = operand.substr(1,operand.length()-2);
        }
        if(R[operand] != "")
        {
            proccStr = proccStr + "("+R[operand]+")";
            return;
        }
        else if (operand[0] == '=')
        {
            updateLitTab(operand,litTab, poolTab);
            int indx = getLiteralIndx(operand,litTab);
            proccStr = proccStr + "(L," + numToStr(indx) + ")";
        }
        else if (isStrNum(operand))
        {
            proccStr = proccStr + "(C,"+operand+")";
        }
        else
        {
            updateSymTab(operand,-1,symTab);
            int indx = getSymTabIndx(operand,symTab);
            proccStr = proccStr + "(S,"+numToStr(indx)+")";
        }

        return ;
    }

    public:
    void performPass1(string fileName)
    {
        vector<SymTabEntry>symTab;
        vector<LitTabEntry>litTab;
        vector<int>poolTab;
        poolTab.push_back(1);
        vector<string>codeLines = readFile(fileName);
        vector<Instruction>preProccInstr = getPreprocessedInstructions(codeLines);
        int noOfLines = codeLines.size();
        vector<string>IC;
        int lc = 0 ;

        for(int i = 0 ; i < noOfLines ; i++)
        {
            Instruction instr = preProccInstr[i];

            if((instr.label!=""))
            {
                string label = instr.label;
                updateSymTab(label,lc,symTab);
            }

            string proccStr = numToStr(lc) + ".   " ;

            if(instr.opcode != "")
            {
                string opcode = instr.opcode;
                if(opcode == "START")
                {
                    if(instr.operand_1 !="")
                    {
                        lc = strToNum(instr.operand_1);
                        lc--;
                    }
                }
                else if (opcode == "ORIGIN")
                {
                    lc = strToNum(instr.operand_1);
                    lc--;
                }
                else if (opcode == "LTORG" || opcode == "END")
                {
                    int k = 0 ;
                    int litTablen = litTab.size();
                    while(k<litTablen && litTab[k].id !=poolTab.back())
                    {
                        k++;
                    }

                    for(int j = k ; j <litTablen ; j++)
                    {
                        string lit = litTab[j].literal ;
                        proccStr = proccStr +"(DL,01),(C," + lit.substr(2,lit.length()-3)+")";
                        litTab[j].address = lc;
                        IC.push_back(proccStr);
                        lc++;
                        proccStr = numToStr(lc) + ".   " ;
                    }
                    
                    int nextAddr = litTab.size()+1;
                    poolTab.push_back(nextAddr);
                    if(opcode == "END")
                    {
                        IC.push_back("-     (AD,02)");
                    }
                    continue;
                }
                else if (opcode == "EQU")
                {
                    string lbl1 = instr.label ;
                    string lbl2 = instr.operand_1;
                    int lbl2indx = getSymTabIndx(lbl2,symTab);
                    updateSymTab(lbl1,symTab[lbl2indx].address,symTab);
                }
                else if (opcode == "DS")
                {
                    int storageSize = strToNum(instr.operand_1);
                    lc = lc + storageSize;
                    lc--;
                }
                addOpCode(proccStr,opcode);
                 
            }

            if(instr.operand_1 != "")
            {
                string operand_1 = instr.operand_1;
                addOperand(proccStr,operand_1,litTab,symTab,poolTab);
            }

            if(instr.operand_2 != "")
            {
                string operand_2 = instr.operand_2;
                addOperand(proccStr,operand_2,litTab,symTab,poolTab);
            }

            IC.push_back(proccStr);
            lc++;
        }

        int IClen = IC.size();

        for(int i = 0 ; i < IClen ; i++)
        {
            cout<<IC[i]<<endl;
        }

        cout<<"SymTab : "<<endl;
        int symTabLen = symTab.size();
        for(int i = 0 ; i < symTabLen ; i++)
        {
            cout<<symTab[i].id<<"   "<<symTab[i].symbol<<"  "<<symTab[i].address<<endl;
        }

        cout<<"LitTab : "<<endl;
        int litTabLen = litTab.size();
        for(int i = 0 ; i < litTabLen ; i++)
        {
            cout<<litTab[i].id<<"   "<<litTab[i].literal<<"     "<<litTab[i].address<<endl;
        }

        cout<<"PoolTab : "<<endl;
        int poolTabLen = poolTab.size();
        for(int i = 0 ; i < poolTabLen ; i++)
        {
            cout<<poolTab[i]<<endl;
        }

    }

};

int main()
{
    string fileName = "test.txt";
    ICGenerator generator;
    generator.performPass1(fileName);
    return 0 ;
}
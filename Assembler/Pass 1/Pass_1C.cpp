#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
using namespace std;

class SymTabEntry
{
    public:
    int index;
    string symbol;
    int address;

    public:
    SymTabEntry(int index, string symbol, int address)
    {
        this->index = index;
        this->symbol = symbol;
        this->address = address;
    }
};

class LitTabEntry
{
    public:
    int index;
    string literal;
    int address;

    public:
    LitTabEntry(int index, string literal, int address)
    {
        this->index = index;
        this->literal = literal;
        this->address = address;
    }
};

class PoolTabEntry
{
    public:
    int index;
    int address;

    public:
    PoolTabEntry(int index, int address)
    {
        this->index = index;
        this->address = address;
    }
};

class Instruction
{
    public:
    string label;
    string opcode;
    string operand_1;
    string operand_2;

    Instruction()
    {
        this->label = "";
        this->opcode = "";
        this->operand_1 = "";
        this->operand_2 = "";
    }
};

class ICGenerator
{
    private:
    map<string,string>IS;
    map<string,string>AD;
    map<string,string>DL;
    map<string,string>R;
    vector<SymTabEntry>symTab;
    vector<LitTabEntry>litTab;
    vector<PoolTabEntry>poolTab;

    private :
    vector<string> readFile(string fileName)
    {
        vector<string>lines;
        ifstream fin;
        fin.open(fileName);
        string line;
        while(!fin.eof())
        {
            getline(fin,line);
            lines.push_back(line);
        }
        fin.close();
        return lines;
    }

    vector<string> getWords(string line)
    {
        vector<string>words;
        int len = line.length();
        string word = "";
        for(int i = 0 ; i < len ; i++)
        {
            if(line[i] == ' ')
            {
                if(word != "")
                {
                    words.push_back(word);
                }
                word = "";
            }
            else
            {
                word = word + line[i];
            }
        }
        words.push_back(word);
        return words;
    }

    vector<vector<string>> splitLines(vector<string> lines)
    {
        vector<vector<string>>splittedLines;
        int len = lines.size();
        vector<string>words;
        for(int i = 0 ; i < len ; i++)
        {
            words = getWords(lines[i]);
            splittedLines.push_back(words);
        }
        return splittedLines;
    }

    Instruction getInstruction(vector<string>words)
    {
        int len = words.size();
        Instruction instr;
        int i = 0 ;
        if(IS[words[i]]=="" && AD[words[i]]=="" && DL[words[i]]=="")
        {
            instr.label = words[i];
            i++;
        }

        instr.opcode = words[i];
        i++;

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

    vector<Instruction> getInstructions(vector<vector<string>>lines)
    {
        int len = lines.size();
        vector<Instruction>instructions;
        for(int i = 0 ; i < len ; i++)
        {
            Instruction instr = getInstruction(lines[i]);
            instructions.push_back(instr);
        }

        return instructions;
    }

    void updateSymTab(string symbol, int lc)
    {
        int len = symTab.size();
        for(int i = 0 ; i < len ; i++)
        {
            if(symTab[i].symbol == symbol)
            {
                symTab[i].address = lc;
                return;
            }
        }
        SymTabEntry ste(symTab.size()+1, symbol, lc) ;
        symTab.push_back(ste);
    }

    void displaySymTab()
    {
        cout<<"Symtab : "<<endl;
        int len = symTab.size();
        for(int i = 0 ; i < len ; i++)
        {
            cout<<symTab[i].index<<"  "<<symTab[i].symbol<<"  "<<symTab[i].address<<endl;
        }
    }

    void displayPoolTab()
    {
        cout<<"PoolTab:"<<endl;
        int len = poolTab.size();
        for(int i = 0 ; i < len ; i++)
        {
            cout<<poolTab[i].index<<"  "<<poolTab[i].address<<endl;
        }
    }
    
    void displayLitTab()
    {
        cout<<"Littab"<<endl;
        int len = litTab.size();
        for(int i = 0 ; i < len ; i++)
        {
            cout<<litTab[i].index<<"  "<<litTab[i].literal<<"   "<<litTab[i].address<<endl;
        }
    }

    int getSymTabIndex(string symbol)
    {
        int len = symTab.size();
        for(int i = 0 ; i < len ; i++)
        {
            if(symTab[i].symbol == symbol)
            {
                return i;
            }
        }
        return -1;
    }
    
    int evaluateExp(string operand)
    {
        regex r1("[+-]");
        smatch m ; 
        if(regex_search(operand,m,r1))
        {
            string::iterator it = find(operand.begin(), operand.end(), '+');
            if(it == operand.end())
            {
                it = find(operand.begin(),operand.end(),'-');
            }

            int indx = it-operand.begin();
            char oprtor = operand[indx];
            string label = operand.substr(0,indx);
            int change = stoi(operand.substr(indx+1, operand.length()-indx));

            int address = symTab[getSymTabIndex(label)].address;
        
            if(oprtor == '+')
            {
                return (address + change);
            }
            else
            {
                return (address-change);
            }

        }
        else
        {
            return symTab[getSymTabIndex(operand)].address;
        }
    }

    void updateLitTab(string literal)
    {
        int startIndx = poolTab.back().address-1;
        int len = litTab.size();
        for(int i = startIndx ; i < len ; i++)
        {
            if(litTab[i].literal == literal)
            {
                return ;
            }
        }

        litTab.push_back(LitTabEntry(litTab.size()+1, literal, -1));
    }
    
    void processOperand(string operand)
    {
        if(operand == "")
        {
            return;
        }
        if(operand[0] == '=')
        {
            updateLitTab(operand);
        }
        else if(R[operand] == "")
        {
            if(getSymTabIndex(operand) == -1)
            {
                updateSymTab(operand, -1);
            }
            
        }
    }
    
    int getLitTabIndx(string literal)
    {
        int len = litTab.size();
        for(int i = 0 ; i < len ; i++)
        {
            if(litTab[i].literal == literal)
            {
                return i ;
            }
        }

        return -1;
    }

    string addOperandsToString(string ICLine, string operand)
    {
        if(operand == "")
        {
            return ICLine;
        }

        if(R[operand] != "")
        {
            ICLine = ICLine + "(" + R[operand] + ")";
        }
        else if (operand[0] == '=')
        {
            ICLine = ICLine + "(L,"+to_string(getLitTabIndx(operand)+1)+")";
        }
        else if (operand[0] == '\'')
        {
            ICLine = ICLine + "(C," + operand.substr(1,operand.length()-2) + ")";
        }
        else if (regex_match(operand,regex("^[0-9]+$")))
        {
            ICLine = ICLine + "(C," + operand + ")";
        }
        else
        {
            string::iterator it = find(operand.begin(), operand.end() , '+');
            if(it == operand.end())
            {
                it = find(operand.begin(),operand.end(),'-');
            }
            int indx = it-operand.begin();
            string symbol = operand.substr(0,indx);

            ICLine = ICLine + "(S," + to_string(getSymTabIndex(symbol)+1)+")";

            if(it != operand.end())
            {
                ICLine = ICLine + operand.substr(indx,operand.length()-indx);
            }
        }
        return ICLine;
    }
    
    vector<string> processInstr(Instruction instr, int &lc)
    {
        vector<string>ICLines;
        if(instr.label != "")
        {
            updateSymTab(instr.label,lc);
        }

        if(AD[instr.opcode] != "")
        {
            if(instr.opcode == "START" && instr.operand_1 != "")
            {
                lc = stoi(instr.operand_1);
            }
            else if (instr.opcode == "ORIGIN")
            {
                int val = evaluateExp(instr.operand_1);
                lc = val;
            }
            else if (instr.opcode == "LTORG" || instr.opcode == "END")
            {
                int startIndx = poolTab.back().address-1;
                int len = litTab.size();
                for(int i = startIndx ; i < len ; i++)
                {
                    litTab[i].address = lc ; 
                    string literal = litTab[i].literal;
                    string ICLine = to_string(lc) + ".  (DL,01) (C,"+literal.substr(2,literal.length()-3)+")";
                    ICLines.push_back(ICLine);
                    lc++;
                }
                poolTab.push_back(PoolTabEntry(poolTab.size()+1, litTab.size()+1));
            }
            else
            {
                int val = evaluateExp(instr.operand_1);
                symTab[getSymTabIndex(instr.label)].address = val;
            }
        }
        else if (DL[instr.opcode] != "")
        {
            if(instr.opcode == "DS")
            {
                int change = stoi(instr.operand_1);
                lc = lc + change-1;
            }
            
        }
        else
        {
            processOperand(instr.operand_1);
            processOperand(instr.operand_2);
        }
        
        string ICLine = "";
        if(instr.opcode != "LTORG")
        {
            if(IS[instr.opcode] != "")
            {
                ICLine = to_string(lc) + ".  " + "(IS," + IS[instr.opcode] + ")";
            }
            else if(AD[instr.opcode] != "")
            {
                ICLine = "-     (AD,"+AD[instr.opcode]+")";
            }
            else
            {
                ICLine = to_string(lc) + ".  (DL,"+DL[instr.opcode]+")";
            }

            ICLine = addOperandsToString(ICLine, instr.operand_1);
            ICLine = addOperandsToString(ICLine, instr.operand_2);
            ICLines.push_back(ICLine);
        }
        
        if(IS[instr.opcode] != "" || DL[instr.opcode] != "")
        {
            lc++;
        }
        
        return ICLines;
    }

    void displayIC(vector<string>IC)
    {
        int len = IC.size();
        for(int i = 0 ; i < len ; i++)
        {
            cout<<IC[i]<<endl;
        }
    }

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
        R["ANY"] = "06";
        poolTab.push_back(PoolTabEntry(1,1));
    }
   
    void generateIC(string fileName)
    {
        vector<string> lines = readFile(fileName);
        vector<vector<string>> lineWords = splitLines(lines);
        vector<Instruction> instructions = getInstructions(lineWords);
        int len = instructions.size();
        vector<string>IC;
        int lc = 0 ;
        for(int i = 0 ; i < len ; i++)
        {
            vector<string> ICLines = processInstr(instructions[i],lc);
            for(int j= 0 ; j < ICLines.size() ; j++)
            {
                IC.push_back(ICLines[j]);
            }
            
        }

        displayIC(IC);
        displaySymTab();
        displayLitTab();
        displayPoolTab();

        this->symTab.clear();
        this->litTab.clear();
        this->poolTab.clear();
        
    }

};

int main()
{
    string fileName = "test_3.txt";
    ICGenerator generator;
    generator.generateIC(fileName);
    
}

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
        this->lc = "";
        this->opcode = "";
        this->operand_1 = "";
        this->operand_2 = "";
    }
};

class SymTabEntry
{
    public : 
    string index ; 
    string symbol ; 
    string address;

    public : 
    SymTabEntry()
    {
        this->index = "";
        this->symbol = "";
        this->address = "";
    }
};

class LitTabEntry
{
    public : 
    string index; 
    string literal ; 
    string address;
    
    public : 
    LitTabEntry()
    {
        this->index = "";
        this->literal = "";
        this->address = "";
    }
};

class MachineCodeEntry
{
    public : 
    string lc ; 
    string opcode; 
    string operand_1; 
    string operand_2;

    public : 
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
    vector<ICEntry>IC;
    vector<SymTabEntry>symTab;
    vector<LitTabEntry>litTab;
    
    private : 
    bool isNum(string strNum)
    {
        int len = strNum.length();
        for(int i = 0 ; i < len ; i++)
        {
            int val = strNum[i] - '0';
            if(val>9)
            {
                return false ;
            }
        }
        return true ; 
    }
    
    vector<string> readFile(string fileName)
    {
        ifstream fin ; 
        fin.open(fileName);
        vector<string>lines;
        string line;
        while(!fin.eof())
        {
            getline(fin, line);
            if(line != "")
            {
                lines.push_back(line);
            }
        }
        fin.close();
        return lines;
    }

    vector <string> getWords(string line)
    {
        vector<string>words;
        string word = "";
        int len = line.length();
        for(int i = 0 ; i < len ; i++)
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
        }
        words.push_back(word);
        return words;
    }

    vector<vector<string>>splitLines(vector<string>lines)
    {
        vector<vector<string>>splitttedLines ; 
        int len = lines.size();
        vector<string>words;
        for(int i = 0 ; i < len ; i++)
        {
            words = getWords(lines[i]);
            splitttedLines.push_back(words);
        }

        return splitttedLines;
    }

    ICEntry getICEntry(vector<string>ICLine)
    {
        int len = ICLine.size();
        int i = 0 ; 
        ICEntry ice ; 
        if(isNum(ICLine[i]))
        {
            ice.lc = ICLine[i];
            i++;
        }

        ice.opcode = ICLine[i];
        i++;

        if(i<len)
        {
            ice.operand_1 = ICLine[i];
            i++;
        }

        if(i<len)
        {
            ice.operand_2 = ICLine[i];
        }

        return ice;
    }

    void generateIC(vector<vector<string>>ICLines)
    {
        int len = ICLines.size();
        ICEntry ice; 
        for(int i = 0 ; i < len ; i++)
        {
            ice = getICEntry(ICLines[i]);
            IC.push_back(ice);
        }
    }
    
    void generateSymTab(vector<vector<string>>SymTabLines)
    {
        int len = SymTabLines.size();
        SymTabEntry ste  ;
        for(int i = 0 ; i < len ; i++)
        {
            ste.index = SymTabLines[i][0];
            ste.symbol = SymTabLines[i][1];
            ste.address = SymTabLines[i][2];
            symTab.push_back(ste);
        }
    }

    void generateLitTab(vector<vector<string>>LitTabLines)
    {
        int len = LitTabLines.size();
        LitTabEntry lte ; 
        for(int i = 0 ; i < len ; i++)
        {
            lte.index = LitTabLines[i][0];
            lte.literal = LitTabLines[i][1];
            lte.address = LitTabLines[i][2];
            litTab.push_back(lte);
        }

    }
    
    string processOperand(string operand)
    {
        if(operand == "")
        {
            return operand;
        }
        string operandPart = operand.substr(1, operand.length()-2);

        if (isNum(operandPart))
        {
            return operandPart;
        }
        else if (operandPart[0] == 'L')
        {
            int indx = stoi(operandPart.substr(2, operandPart.length()-2));
            string address = litTab[indx-1].address;
            return address;
        }
        else if (operandPart[0] == 'S')
        {
            int indx = stoi(operandPart.substr(2, operandPart.length()-2));
            string address = symTab[indx-1].address;
            return address;
        }
        else if (operandPart[0] == 'C')
        {
            string val =  operandPart.substr(2, operandPart.length()-2);
            return val ; 
        }
        return "-1";
    }

    MachineCodeEntry processICLine(ICEntry ice)
    {
        MachineCodeEntry mce ;
        if(ice.lc == "")
        {
            return mce;
        }

        mce.lc = ice.lc;

        string opcodeType = ice.opcode.substr(1,2);
        string id = ice.opcode.substr(4,2);
        if(opcodeType == "IS")
        {
            mce.opcode = id;
        }
        else if(opcodeType == "DL")
        {
            if(id == "02")
            {
                return mce;
            }
            else
            {
                mce.opcode = "00";
                mce.operand_1 = "0";
                mce.operand_2 = processOperand(ice.operand_1);
                return mce;
            }
        }

        mce.operand_1 = processOperand(ice.operand_1);
        mce.operand_2 = processOperand(ice.operand_2);

        return mce;

    }

    public :
    void generateMachineCode(string ICFile, string litTabFile, string symTabFile)
    {
        vector<string> ICLines = readFile(ICFile);
        vector<string>litTabLines = readFile(litTabFile);
        vector<string>symTabLines = readFile(symTabFile);

        vector<vector<string>>splitICLines = splitLines(ICLines);
        vector<vector<string>>splitLitTabLines  = splitLines(litTabLines);
        vector<vector<string>>splitSymTabLines = splitLines(symTabLines);


        generateIC(splitICLines);
        generateLitTab(splitLitTabLines);
        generateSymTab(splitSymTabLines);

        
        int len = IC.size();
        vector<MachineCodeEntry> machineCode;
        string machineCodeLine ;
        for(int i = 0 ; i < len ; i++)
        {
            MachineCodeEntry mce = processICLine(IC[i]);
            if(mce.lc != "")
            {
                machineCode.push_back(mce);
            }
        }

        len = machineCode.size();
        for(int i = 0 ; i < len ; i++)
        {
            cout<<machineCode[i].lc<<"  "<<machineCode[i].opcode<<"  "<<machineCode[i].operand_1<<"  "<<machineCode[i].operand_2<<endl;
        }

        

    }
};

int main()
{
    // string ICFile = "test_1_IC.txt";
    // string litTabFile = "test_1_litTab.txt";
    // string symTabFile = "test_1_symTab.txt";

    string ICFile = "test_2_IC.txt";
    string litTabFile = "test_2_litTab.txt";
    string symTabFile = "test_2_symTab.txt";

    MachineCodeGenerator mcg ; 
    mcg.generateMachineCode(ICFile, litTabFile, symTabFile);


    return 0 ;
}
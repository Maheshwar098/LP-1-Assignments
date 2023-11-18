#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
using namespace std;

class MacroEntry
{
    public : 
    string opcode ; 
    string operand_1 ; 
    string operand_2 ; 

    public : 
    MacroEntry(string opcode, string operand_1, string operand_2)
    {
        this->opcode = opcode ; 
        this->operand_1 = operand_1 ; 
        this->operand_2 = operand_2 ; 
    }

    MacroEntry()
    {
        this->opcode = "";
        this->operand_1 = "";
        this->operand_2 = "";
    }
};

class MNTEntry
{
    public : 
    int indx ; 
    string macroName ;
    int PP;
    int KP;
    int MDTP;
    int KPDTP;

    public : 
    MNTEntry()
    {
        this->indx = 0 ; 
        this->macroName = "";
        this->PP = 0 ; 
        this->KP = 0 ; 
        this->MDTP = 0 ; 
        this->KPDTP = 0 ;
    }

    MNTEntry(int indx, string macroName, int PP, int KP, int MDTP, int KPDTP)
    {
        this->indx = indx ; 
        this->macroName = macroName; 
        this->PP = PP ; 
        this->KP = KP ; 
        this->MDTP = MDTP ; 
        this->KPDTP = KPDTP ; 
    }
    
};

class KPTEntry
{
    public : 
    int indx ; 
    string parameter ; 
    string val ; 

    public : 
    KPTEntry()
    {
        this->indx = 0 ; 
        this->parameter = "";
        this->val = "";
    }
    
    KPTEntry(int indx, string parameter, string val)
    {
        this->indx = indx ; 
        this->parameter = parameter ; 
        this->val  = val ;
    }
};

class PNTEntry
{
    public : 
    int indx ; 
    string parameter ; 
    
    public : 
    PNTEntry()
    {
        this->indx = 0 ; 
        this->parameter = "";
    }

    PNTEntry(int indx, string parameter)
    {
        this->indx = indx ; 
        this->parameter = parameter ; 
    }
};

class MDTEntry
{
    public : 
    int indx ; 
    string opcode ; 
    string operand_1 ; 
    string operand_2; 

    public : 
    MDTEntry()
    {
        this->indx = 0 ; 
        this->opcode = "";
        this->operand_1 = "";
        this->operand_2 = "";
    }

    MDTEntry(int indx, string opcode, string operand_1, string operand_2)
    {
        this->indx = indx ; 
        this->opcode = opcode; 
        this->operand_1 = operand_1 ; 
        this->operand_2 = operand_2 ; 
    }
};

class MacroProcessor
{
    private : 
    vector<MNTEntry>MNT ; 
    vector<MDTEntry>MDT ; 
    vector<KPTEntry>KPT ;
    vector<vector<PNTEntry>>PNTs ; 

    private : 
    vector<string> readFile(string fileName)
    {
        ifstream fin;
        fin.open(fileName);
        vector<string>fileContent ; 
        string line ; 
        while(!fin.eof())
        {
            getline(fin, line);
            fileContent.push_back(line);
        }
        fin.close();

        return fileContent;
    }

    vector<pair<int,int>> getMacroPositions(vector<string> fileContent)
    {
        vector<pair<int,int>>positions ; 
        int len = fileContent.size();
        int start = -1 ; 
        int end = -1 ; 
        for(int i = 0 ; i < len ; i++)
        {
            if(fileContent[i] == "MACRO")
            {
                start = i ; 
            }
            if(fileContent[i] == "MEND")
            {
                end = i ;
                positions.push_back(make_pair(start,end));
            }
        }

        return positions;
    }
    
    vector<string>extractMacro(vector<string>fileContent, pair<int,int>positions)
    {
        vector<string>macro ; 
        int start = positions.first; 
        int end = positions.second; 
        for(int i = start ; i <= end ; i++)
        {
            macro.push_back(fileContent[i]);
        }

        return macro ; 
    }

    vector<vector<string>>extractAllMacros(vector<string>fileContent, vector<pair<int,int>>positions)
    {
        vector<vector<string>>macros ; 
        int len = positions.size();
        for(int i = 0 ; i < len ; i++)
        {
            vector<string>macro = extractMacro(fileContent, positions[i]);
            macros.push_back(macro);
        }
        return macros;
    }

    vector<string>splitLine(string line)
    {
        int len = line.length();
        string word = "";
        vector<string>words ; 
        for(int i = 0 ; i < len ; i++)
        {
            if(line[i] == ' ')
            {
                if(word != "")
                {
                    words.push_back(word);
                }
                word = "" ;
            }
            else
            {
                word = word + line[i];
            }
        }
        words.push_back(word);

        return words ; 
    }

    vector<vector<string>>splitLines(vector<string>macro)
    {
        vector<vector<string>>splittedLines ; 
        int len = macro.size();
        for(int i = 0 ; i < len ; i++)
        {
            vector<string>words = splitLine(macro[i]);
            splittedLines.push_back(words);
        }
        return splittedLines;
    }

    void processSignature(vector<string>words)
    {
        int indx = MNT.size() + 1 ; 
        int MDTP = MDT.size()+1 ;
        int KPDTP = KPT.size() + 1 ; 
        string macroName = words[0];

        int len = words.size();
        vector<PNTEntry>PNT  ; 
        for(int i = 1 ; i < len ; i++)
        {
            if(words[i] == ",")
            {
                continue;
            }

            if(words[i] != "=" && words[i-1] != "=")
            {
                PNTEntry pnte(PNT.size()+1, words[i]);
                PNT.push_back(pnte);
            }
        }

        int kpCnt= 0 ;

        for(int i = 1 ; i< len ; i++)
        {
            if(words[i] == ",")
            {
                continue;
            }

            if(words[i] == "=")
            {
                kpCnt++;
                string key ; 
                string val ; 
                key = words[i-1];
                if(words[i+1] == ",")
                {
                    val = "-";
                }
                else
                {
                    val = words[i+1];
                }
                KPTEntry kpte(KPT.size()+1, key, val);
                KPT.push_back(kpte);
            }
        }
        MNTEntry mnte(indx, macroName, PNT.size(), kpCnt, MDTP, KPDTP);
        MNT.push_back(mnte);
        PNTs.push_back(PNT);

    }

    int getParameterIndx(vector<PNTEntry>PNT, string operand)
    {
        int len = PNT.size();
        for(int i = 0 ; i < len ; i++)
        {
            if(PNT[i].parameter == operand)
            {
                return PNT[i].indx;
            }
        }
        return -1 ; 
    }

    void processMacro(vector<string> macro)
    {
        vector<vector<string>>macroWords = splitLines(macro);
        processSignature(macroWords[1]);
        vector<PNTEntry>PNT = PNTs.back();

        int len = macroWords.size();
        MDTEntry mdte ; 
        for(int i = 2 ; i < len-1 ; i++)
        {
            vector<string>line = macroWords[i];
            mdte.indx = MDT.size()+1 ; 
            mdte.opcode = line[0];
            int indx = getParameterIndx(PNT, line[1]);
            if(indx == -1)
            {
                mdte.operand_1 = line[1];
            }
            else
            {
                mdte.operand_1 = "(P,"+to_string(indx)+")";
            }
            indx = getParameterIndx(PNT, line[2]);
            if(indx == -1)
            {
                mdte.operand_2 = line[2];
            }
            else
            {
                mdte.operand_2 = "(P,"+to_string(indx)+")";
            }
            MDT.push_back(mdte);
        }
    }

    void displayLine()
    {
        cout<<"---------------------------------------------------------"<<endl;
    }

    public : 
    void generateIC(string fileName)
    {
        vector<string>fileContent = readFile(fileName);
        vector<pair<int,int>>positions = getMacroPositions(fileContent);
        vector<vector<string>> macros = extractAllMacros(fileContent, positions);
        int macroCnt = macros.size();
        for(int i = 0 ; i < macroCnt ; i++)
        {
            processMacro(macros[i]);
        }
        displayLine();
        ofstream fout ; 
        fout.open("MNT.txt");
        for(int i = 0 ; i < MNT.size() ; i++)
        {
            fout<<MNT[i].indx<<"  "<<MNT[i].macroName<<"  "<<MNT[i].PP<<"  "<<MNT[i].KP<<"  "<<MNT[i].MDTP<<"  "<<MNT[i].KPDTP<<endl;
        }
        fout.close();

        fout.open("KPT.txt");
        for(int i = 0 ; i < KPT.size() ; i++)
        {
            fout<<KPT[i].indx<<"  "<<KPT[i].parameter<<"  "<<KPT[i].val<<endl;
        }
        fout.close();

        
        for(int i = 0 ; i < PNTs.size() ; i++)
        {
            fout.open("PNT_"+to_string(i)+".txt");
            for(int j = 0 ; j < PNTs[i].size() ; j++)
            {
                fout<<PNTs[i][j].indx<<"  "<<PNTs[i][j].parameter<<endl;
            }
            fout.close();
        }

        fout.open("MDT.txt");

        for(int i = 0 ; i < MDT.size() ; i++)
        {
            fout<<MDT[i].indx<<"    "<<MDT[i].opcode<<"  "<<MDT[i].operand_1<<"  "<<MDT[i].operand_2<<endl;
        }
        fout.close();
    }
};

int main()
{
    string fileName = "test_1.txt";
    MacroProcessor mp;
    mp.generateIC(fileName);

    return 0 ; 
}
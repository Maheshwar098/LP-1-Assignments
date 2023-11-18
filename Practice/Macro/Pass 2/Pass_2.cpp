#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std; 

class MNTEntry
{
    public : 
    int indx ; 
    string macroName ; 
    int PP; 
    int KP ; 
    int MDTP ; 
    int KPDTP ; 
};

class MDTEntry
{
    public : 
    int indx ; 
    string opcode ; 
    string operand_1 ; 
    string operand_2 ; 
};

class KPDTEntry
{
    public : 
    int indx ; 
    string parameter ; 
    string val ; 
};

class PNTEntry
{
    public : 
    int indx ; 
    string parameter ;
};

class APTEntry
{
    public : 
    int indx ; 
    string parameter ;

    public:
    APTEntry()
    {
        this->indx = 0 ; 
        this->parameter = "";
    }
};

class MacroProccessor
{
    private : 
    vector<MNTEntry> MNT ; 
    vector<MDTEntry> MDT ; 
    vector<KPDTEntry> KPDT ;
    vector<string>output; 

    private : 
    vector<string> readFile(string fileName)
    {
        ifstream fin ; 
        fin.open(fileName);
        vector<string> lines ; 
        string line ; 
        while(!fin.eof())
        {
            getline(fin, line);
            lines.push_back(line);
        }
        fin.close();

        return lines;
    }
  
    vector<string> splitLine(string line)
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
                word = "";
            }
            else
            {
                word = word + line[i];
            }
        }
        words.push_back(word);
        return words ;
    }
    
    vector<vector<string>>splitLines(vector<string>lines)
    {
        vector<vector<string>>splittedLines;
        int len = lines.size();
        for(int i = 0 ; i < len ; i++)
        {
            vector<string>words = splitLine(lines[i]);
            splittedLines.push_back(words);
        }
        return splittedLines; 
    }

    void generateMNT(vector<vector<string>>MNTLines)
    {
        int len = MNTLines.size() ; 
        MNTEntry mnte ; 
        for(int i = 0 ; i < len ; i++)
        {
            mnte.indx = stoi(MNTLines[i][0]);
            mnte.macroName = MNTLines[i][1];
            mnte.PP = stoi(MNTLines[i][2]);
            mnte.KP = stoi(MNTLines[i][3]);
            mnte.MDTP = stoi(MNTLines[i][4]);
            mnte.KPDTP = stoi(MNTLines[i][5]);
            MNT.push_back(mnte);
        }
    }
    
    void generateMDT(vector<vector<string>>MDTLines)
    {
        int len = MDTLines.size();
        MDTEntry mdte ; 
        for(int i = 0 ; i < len ; i++)
        {
            mdte.indx = stoi(MDTLines[i][0]);
            mdte.opcode = MDTLines[i][1];
            mdte.operand_1 = MDTLines[i][2];
            mdte.operand_2 = MDTLines[i][3];
            MDT.push_back(mdte);
        }
    }

    void generateKPDT(vector<vector<string>>KPDTLines)
    {
        int len = KPDTLines.size();
        KPDTEntry kpdte ; 
        for(int i = 0 ; i < len ; i++)
        {
            kpdte.indx = stoi(KPDTLines[i][0]);
            kpdte.parameter = KPDTLines[i][1];
            kpdte.val = KPDTLines[i][2];
            KPDT.push_back(kpdte);
        }

    }
    
    bool checkMacro(string name)
    {
        int len = MNT.size(); 
        for(int i = 0 ; i < len ; i++)
        {
            if(MNT[i].macroName == name)
            {
                return true ; 
            }
        }
        return false ; 
    }

    string getLineFromWords(vector<string>words)
    {
        int len = words.size();
        string line = "";
        for(int i = 0 ; i < len ; i++)
        {
            line = line + words[i] ;
            if(i != len-1)
            {
                line = line + " ";
            }
        }

        return line ;
    }

    int getMNTIndex(string macroName)
    {
        int len = MNT.size();
        for(int i = 0 ; i < len ; i++)
        {
            if(MNT[i].macroName == macroName)
            {
                return MNT[i].indx ;
            }
        }
        return -1 ; 
    }

    vector<PNTEntry> getPNT(string fileName)
    {
        vector<PNTEntry>PNT ; 
        ifstream fin ; 
        fin.open(fileName);
        string line  ;
        PNTEntry pnte; 
        while(!fin.eof())
        {
            getline(fin, line);
            cout<<line<<endl;
            vector<string>words = splitLine(line);
            pnte.indx = stoi(words[0]);
            pnte.parameter = words[1];
            PNT.push_back(pnte);
        }
        fin.close();
        return PNT ; 
    }

    int getPNTIndex(vector<PNTEntry>PNT, string parameter)
    {
        for(int i = 0 ; i < PNT.size() ; i++)
        {
            if(PNT[i].parameter == parameter)
            {
                return PNT[i].indx;
            }
        }
        return -1 ; 
    }

    string getDefaultKPVal(int KPIndex , string parameterKey)
    {
        for(int i = KPIndex-1 ; i<KPDT.size() ; i++ )
        {
            if(KPDT[i].parameter == parameterKey)
            {
                return KPDT[i].val;
            }
        }

        return "";
    }

    void processCodeLine(vector<string>codeLine)
    {
        int len = codeLine.size();
        if(!checkMacro(codeLine[0]))
        {
            output.push_back(getLineFromWords(codeLine));
        }
        else
        {
            string macroName = codeLine[0];
            int indx = getMNTIndex(macroName);
            MNTEntry mnte = MNT[indx-1];
            string PNTFileName = mnte.macroName + ".txt";
            vector<PNTEntry>PNT = getPNT(PNTFileName);
            vector<APTEntry>APT(PNT.size()); 
            int indx1 = 0 ; 
            for(int i = 1 ; i < len ; i++)
            {
                if(codeLine[i] == ",")
                {
                    continue;
                }
                if(i<len-1 && codeLine[i+1] != "=" && codeLine[i-1] != "=")
                {
                    APTEntry apte ; 
                    apte.indx = indx1+1 ; 
                    apte.parameter = codeLine[i];
                    APT[indx1] = apte;
                    indx1++;
                }
                else if(i<len-1 && codeLine[i+1] == "=")
                {
                    int index = getPNTIndex(PNT, codeLine[i]);
                    APT[index-1].indx = index ;
                    APT[index].parameter = codeLine[i+2];
                }

            }

            for(int i = 0 ; i<PNT.size() ; i++)
            {
                if(APT[i].parameter == "")
                {
                    string parameterKey = PNT[i].parameter;
                    string val = getDefaultKPVal(mnte.KP, parameterKey);
                    APT[i].parameter = val ; 
                }
            }

            int mdtIndx = mnte.MDTP ;
            int end = 0 ;
            if(indx == MNT.size())
            {
                end = MDT.size(); 
            }
            else
            {
                end = MNT[indx].MDTP;
            }

            for(int i = mdtIndx-1 ; i < end ; i++)
            {
                MDTEntry mdte = MDT[i];
                string line  = "+   ";
                line = line + mdte.opcode+"   ";
                if(MDT[i].operand_1[1] == 'P')
                {
                    line = line + APT[MDT[i].operand_1[3]-'0'-1].parameter+"  ";
                }
                else
                {
                    line = line + MDT[i].operand_1 + "  ";
                }

                if(MDT[i].operand_2[1] == 'P')
                {
                    
                    line = line + APT[MDT[i].operand_2[3]-'0'-1].parameter+"  ";
                }
                else
                {
                    line = line + MDT[i].operand_2 ; 
                }
                output.push_back(line);
            }



        }
    }
    public : 
    
    void generateFinalCode(string codeFile, string MNTFile, string MDTFile, string KPTFile)
    {
        vector<string>codeLines = readFile(codeFile);
        vector<string>MNTLines = readFile(MNTFile);
        vector<string>MDTLines = readFile(MDTFile);
        vector<string>KPTLines = readFile(KPTFile);

        vector<vector<string>>splitCodeLines = splitLines(codeLines);
        vector<vector<string>>splitMNTLines = splitLines(MNTLines);
        vector<vector<string>>splitMDTLines = splitLines(MDTLines);
        vector<vector<string>>splitKPTLines = splitLines(KPTLines);

        generateMNT(splitMNTLines);
        generateMDT(splitMDTLines);
        generateKPDT(splitKPTLines);

        int len = codeLines.size();
        for(int i = 0 ; i < len ; i++)
        {
            processCodeLine(splitCodeLines[i]);
        }

        for(int i = 0 ; i < output.size() ; i++)
        {
            cout<<output[i]<<endl ; 
        }

    }
};

int main()
{
    string codeFile = "test_1.txt";
    string MNTFile = "MNT.txt";
    string MDTFile = "MDT.txt"; 
    string KPTFile = "KPT.txt";
    MacroProccessor mp ; 
    mp.generateFinalCode(codeFile, MNTFile, MDTFile, KPTFile);
    
cout<<"Done"<<endl;
    return 0 ; 
}
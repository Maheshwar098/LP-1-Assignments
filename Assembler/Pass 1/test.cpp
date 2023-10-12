#include <iostream>
using namespace std;

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

int main()
{
    cout<<numToStr(stringToNum("2341356"));
    return 0;
}
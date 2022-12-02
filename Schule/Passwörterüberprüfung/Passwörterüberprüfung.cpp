#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>
#include <fstream>

using namespace std;

vector<string> explode(const string& delimiter, const string& explodeme);

int main(int argc, char* argv[])
{
    int t = 0;
    string pas, nam,tempp,tempn;
    string namen = "";
    string passw = "";

    ifstream MyReadFile("pas.txt");
    getline(MyReadFile, passw);
    MyReadFile.close();
    ifstream MyReadFilee("nam.txt");
    getline(MyReadFilee, namen);
    MyReadFilee.close();


    cout << "Name: ";
    cin >> nam;
    cout << "Passwort: ";
    cin >> pas;
    vector<string> p = explode("-", passw);
    vector<string> n = explode("-", namen);
    for (int i = 0; i < p.size(); i++)
    {
        tempp = p[i];
        tempn = n[i];
        if ( tempp== pas && tempn == nam) {
            cout << "richtig";
            t = 1;
        }
    }
    if (t == 0)cout << "falsch";
}


vector<string> explode(const string& delimiter, const string& str)
{
    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng == 0)
        return arr;

    int i = 0;
    int k = 0;
    while (i < strleng)
    {
        int j = 0;
        while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
            j++;
        if (j == delleng)
        {
            arr.push_back(str.substr(k, i - k));
            i += delleng;
            k = i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(str.substr(k, i - k));
    return arr;
}
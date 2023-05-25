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
    while (true) {
        system("CLS");
        int chose = 0;
        cout << "System" << endl;
        cout << "Login (1)" << endl;
        cout << "Registration (2)" << endl;
        cout << "Exit (3)" << endl;
        cin >> chose;
        int t = 0;
        string pas, nam, tempp, tempn;

        string namen = "";
        string passw = "";
        string in = "";
        string nua = "", npa = "";
        ifstream MyReadFilee("in.txt");
        getline(MyReadFilee, in);
        MyReadFilee.close();
        vector<string> tall = explode("°", in);
        passw = tall[1];
        namen = tall[0];

        if (chose == 1) {
            system("CLS");
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

                if (tempp == pas && tempn == nam) {
                    cout << "richtig";
                    t = 1;
                }
            }
            if (t == 0)cout << "falsch";
        }
        else if (chose == 2) {
            cout << "Neuer Name: ";
            cin >> nua;
            cout << "Neues Passwort: ";
            cin >> npa;
            namen += nua + "-°" + passw + npa + "-";
            //cout << namen;
            ofstream MyReadFilee("in.txt");
            MyReadFilee << namen;
            MyReadFilee.close();

        }
        else if (chose == 3)return 0;
    }
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
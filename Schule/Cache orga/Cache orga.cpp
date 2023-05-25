
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <iomanip>
#include <fstream>
#include <ctime>

#pragma warning(disable : 4996)

using namespace std;

vector<string> explode(const string& delimiter, const string& explodeme);

int main(int argc, char* argv[])
{
    while (true) {
        int chose = 0,chosee=0;
        cout << "Cache" << endl;
        cout << "Einen neuer eintrag (1)" << endl;
        cout << "Datei auslesen (2)" << endl;
        cout << "Neue Eintraege (3)" << endl;
        cout << "Exit (4)" << endl;
        cin >> chose;
        
        string newe, art,num;
        int jear, month, day,nu;
        string dat,anzahl;
        
        time_t t = time(nullptr);
        tm* now = localtime(&t);

        jear = (now->tm_year + 1900);
        month = (now->tm_mon + 1);
        day = now->tm_mday;
       
        dat = to_string(jear) + '-' + to_string(month) + '-' + to_string(day);
        
        if (chose == 1) {
            ifstream MyReadFilee("in.txt");
            getline(MyReadFilee, anzahl);
            MyReadFilee.close();
            cout << "Bitte art waehlen:" << endl;
            cout << "GC (1)" << endl;
            cout << "Lab (2)" << endl;
            cin >> chosee;
            if (chosee == 1) {
                art = "GC";
            }
            else if (chosee == 2) {
                art = "Lab";
            }

            else {
                cout << "Error bitte Nur 1 oder 2 Eingeben";
                continue;
            }
            cout << "Bitte Nummer eingeben: ";
            cin >> num;
            newe = "\n"+art + num + "," + dat + "T22:30Z,Found it,'" + anzahl + "'";
            cout << newe;
            
            sscanf(anzahl.c_str(), "%d", &nu);
            nu++;
            anzahl = to_string(nu);
            ofstream MyRedFilee("in.txt");
            MyRedFilee << anzahl;
            MyRedFilee.close();

            fstream f("all.txt", ios::app);
            f << newe;

        }

        else if (chose == 3) {
            while (true) {
                ifstream MyReadFilee("in.txt");
                getline(MyReadFilee, anzahl);
                MyReadFilee.close();
                cout << "Bitte art waehlen:" << endl;
                cout << "GC (1)" << endl;
                cout << "Lab (2)" << endl;
                cout << "Dauer GC (3)" << endl;
                cout << "Dauer Lab (4)" << endl;
                cout << "Beenden (5)" << endl;
                cin >> chosee;
                if (chosee == 1) {
                    art = "GC";
                    cout << "Bitte Nummer eingeben: ";

                    cin >> num;

                    newe = "\n" + art + num + "," + dat + "T22:30Z,Found it,'" + anzahl + "'";
                    cout << newe;

                    sscanf(anzahl.c_str(), "%d", &nu);
                    nu++;
                    anzahl = to_string(nu);
                    ofstream MyRedFilee("in.txt");
                    MyRedFilee << anzahl;
                    MyRedFilee.close();

                    fstream f("all.txt", ios::app);
                    f << newe;
                }
                else if (chosee == 2) {
                    art = "Lab";
                    cout << "Bitte Nummer eingeben: ";

                    cin >> num;

                    newe = "\n" + art + num + "," + dat + "T22:30Z,Found it,'" + anzahl + "'";
                    cout << newe;

                    sscanf(anzahl.c_str(), "%d", &nu);
                    nu++;
                    anzahl = to_string(nu);
                    ofstream MyRedFilee("in.txt");
                    MyRedFilee << anzahl;
                    MyRedFilee.close();

                    fstream f("all.txt", ios::app);
                    f << newe;
                }
                else if (chosee == 3) {
                    while (true) {
                        cin >> num;
                        if (num == "EXIT")break;
                        art = "GC";
                        newe = "\n" + art + num + "," + dat + "T22:30Z,Found it,'" + anzahl + "'";
                        cout << newe;

                        sscanf(anzahl.c_str(), "%d", &nu);
                        nu++;
                        anzahl = to_string(nu);
                        ofstream MyRedFilee("in.txt");
                        MyRedFilee << anzahl;
                        MyRedFilee.close();

                        fstream f("all.txt", ios::app);
                        f << newe;
                    }
                }
                else if (chosee == 4) {
                    while (true) {
                        cout << "Eingabe:";
                        cin >> num;
                        cout << endl;
                        if (num == "EXIT")break;
                        art = "Lab";
                        newe = "\n" + art + num + "," + dat + "T22:30Z,Found it,'" + anzahl + "'";
                        cout << newe;

                        sscanf(anzahl.c_str(), "%d", &nu);
                        nu++;
                        anzahl = to_string(nu);
                        ofstream MyRedFilee("in.txt");
                        MyRedFilee << anzahl;
                        MyRedFilee.close();

                        fstream f("all.txt", ios::app);
                        f << newe;
                    }
                }
                
                else if (chosee == 5) {
                    break;
                }
              

                else { //do not function
                    cout << "Error bitte Nur 1 bis 5 Eingeben";
                    continue;
                }
               
            }
        }
        else if (chose == 2) {
            ifstream MyReadFile("all.txt");
            string myText;
            while (getline(MyReadFile, myText)) {
                cout << myText<<endl;

            }
        }
        else if (chose == 4)return 0;
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
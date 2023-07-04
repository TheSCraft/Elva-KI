//Lösung eines MDP(Markov Decision Process) von Simon Seitz
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>



using namespace std;
// Deklaration der globalen Variablen und Funktionen
constexpr auto simo = 100;
double qMatrix[simo][simo], gammaLR = 0.8; // Q-Matrix und der Discount-Faktor gamma (Learning Rate)
int max_index[simo], erlaubteAktion[simo]; // Hilfsarrays für den maximalen Index und verfügbare Aktionen
void ausgabearray(double a[][simo],int felder); // Funktion zur Ausgabe eines Arrays
int zufallszahl(int felder); // Funktion, die eine zufällige Zahl von 0 bis 7 zurückgibt
double update(int punkt, int aktion, double rMatrix[][simo], double qMatrix[][simo],int zeigen,int felder); // Funktion zur Aktualisierung der Q-Matrix
int erlaubteAktionen(int state, int erlaubteAktion[], double rMatrix[][simo],int felder); // Funktion zur Ermittlung verfügbarer Aktionen
void zeigekarte();//zeigt das labyrint
vector<string> explode(const string& delimiter, const string& explodeme);

int main()
{
    int zeigen=2,labyrint,trainings=500;
    int t;//temp
    int ziel = 7;  // Deklaration der Anfangs- und Endzustände
    int punkt, groesemoeglicheraktionen, aktion;  // Deklaration von aktuellen Zustand, Größe der verfügbaren Aktionen und ausgewählter Aktion
    double final_max = 0.0, score = 0.0;  // Deklaration von final_max für das Finden des Maximums und des Punktestands
    double rMatrix[simo][simo];//-1 sind unmögliche verbindungen sinn:[von][nach]
    string fini, datein,finalewerte,in;//zum speichern

    for (int i = 0; i < simo; i++){
        for (int j = 0; j < simo; j++) {
            rMatrix[j][i] = -1;
            qMatrix[i][j] = 0.0;
        }
    }
    int felder = 0, verbunden = 0;

    srand(time(nullptr)); // Initialize random seed

    cout << "Lernen anzeigen? 1 ja  2 nein" << endl;
    cin >> zeigen;//zeigen der einzelnen Lernschritte zum Beschleunigen oder Verlangsamen des Programms
    cout << "Labyrint einlesen oder waehlen? 1 einlesen 2 voreingestellt 3 dateiladen" << endl;
    cin >> labyrint;
    if (labyrint == 1) {
        system("CLS");//Löscht die Konsole
        cout << "Wieviele Felder hat dein Labyrinth?" << endl;
        cout << "Felder: ";
        cin >> felder;//Wieviele Felder das Labyrinth hat
        finalewerte = to_string(felder);
        system("CLS");//Löscht die Konsole
        cout << "Wenn keine weiteren Verbindungen vorhanden mit " << felder  << " Bestätigen" << endl;
        cout << "Um Feld anzuzeigen " << felder + 1 << " eingeben" << endl;

        for (int i = 0; i < felder; i++)
        {
            cout << "Mit welchen Feldern ist " << i << " verbunden?" << endl;//Mit welchem Feld welches Feld verbunden ist
            
            do
            {
                cin >> verbunden;
                if (verbunden == felder)break;
                else if (verbunden > felder) {
                    cout << finalewerte << endl;
                    continue;
                }
                rMatrix[i][verbunden] = 0.0;//moeglich
                rMatrix[verbunden][i] = 0.0;//auch in die andere Richtung
                finalewerte += "|";
                finalewerte += to_string(i) + "-" + to_string(verbunden);
            } while (true);
                
            
        }
 
        cout << "Speichern: 1 ja 2 nein" << endl;
        cin >> t;
        if (t == 1) {
            cout << "Dateiname: ";
            cin >> datein;

            fini = "saves\\";
            fini += datein;
            fini += ".txt";
            ofstream MyReadFile(fini);
            MyReadFile << finalewerte;
            MyReadFile.close();
           
        }
    }
    if (labyrint == 2) {
        felder = 8;
        zeigekarte();

        for (int i = 0; i < felder; i++)
        {
            for (int j = 0; j < felder; j++)
            {
                

                // Festlegen der Belohnungen in der R-Matrix
                if ((i == 0 && j == 1) || (i == 1 && j == 5) || (i == 5 && j == 6) || (i == 5 && j == 4) || (i == 1 && j == 2) || (i == 2 && j == 3) || (i == 2 && j == 7) || (i == 4 && j == 7) || (i == 1 && j == 4) || (i == 2 && j == 7) || (i == 4 && j == 7))
                {
                    rMatrix[i][j] = 0.0;//moeglich
                    rMatrix[j][i] = 0.0;//auch in die andere Richtung
                }

            }
        }
    }

    if (labyrint == 3) {
        system("CLS");//Löscht die Konsole
        cout << "Dateiname: ";
        cin >> datein;
        fini = "saves\\";
        fini += datein;
        fini += ".txt";
        ifstream MyReadFilee(fini);
        getline(MyReadFilee, in);
        MyReadFilee.close();
        cout << in<<endl;
        vector<string> inn = explode("|", in);
        felder = stoi(inn[0]);
        for (int u = 1; u <= size(inn)-1; u++)
        {
            vector<string> innn = explode("-", inn[u]);
            rMatrix[stoi(innn[0])][stoi(innn[1])] = 0.0;
            rMatrix[stoi(innn[1])][stoi(innn[0])] = 0.0;
        }
    }

    cout << "Was soll das Ziel sein:" << endl;
    cin >> ziel;//Defenition des Zielpunktes
 
    for (int i = 0; i < felder; i++)
    {
        for (int j = 0; j < felder; j++)
        {
            if (rMatrix[i][ziel] != -1)rMatrix[i][ziel] = 100.0;//wenn es eine Verbindung zum Ziel gibt
            if (i == j && i == ziel)rMatrix[i][j] = 100.0;//wenn es das Ziel ist
        }
    }
    if (zeigen == 1) {
        // Ausgabe der R-Matrix
        cout << "\nReward Matrix : \n";
        for (int i = 0; i < felder; i++)
        {
            for (int j = 0; j < felder; j++)
            {
                cout << rMatrix[i][j] << "\t";
            }
            cout << "\n";
        }
    }
    cout << "Wie oft soll trainiert werden?" << endl;
        cin >> trainings;
    // Training Q Matrix
    for (int i = 0; i < trainings; i++)
    {
        punkt = zufallszahl(felder);
        groesemoeglicheraktionen = erlaubteAktionen(punkt, erlaubteAktion, rMatrix,felder);
        aktion = erlaubteAktion[zufallszahl(felder) % groesemoeglicheraktionen];

        if (zeigen == 1)cout << "\nNaechster Schritt: " << aktion << "\n";
        score = update(punkt, aktion, rMatrix, qMatrix,zeigen,felder);


        if (zeigen == 1)cout << "\nScore : " << score;
    }

    //Finden des Maximums
    for (int i = 0; i < felder; i++)
    {
        for (int j = 0; j < felder; j++)
        {
            if (final_max < qMatrix[i][j])
            {
                final_max = qMatrix[i][j];
            }
        }
    }

     cout << "\n\nTrained Q Matrix: \n";
    for (int i = 0; i < felder; i++)
    {
        for (int j = 0; j < felder; j++)
        {
             cout << (qMatrix[i][j] / final_max * 100.0) << "\t";
        }
         cout << "\n";
    }

    int punktt;//Anfangspunkt
    bool fehler = false;//für unmoegliche Zuege
    int maxr = 0, maxinde = 0;

    if(labyrint==2)zeigekarte();
    while(true){
        int visited[simo] = { 0 };
        fehler = false;
        maxr = 0;
        maxinde = 0;
     cout << "Eingabe des Anfangspunktes: ";
     cin >> punktt;//Eingabe des Startpunktes von dem zum Ziel navigiert werden soll

    // Ausgabe des Pfads basierend auf der trainierten Q-Matrix
     cout << "Weg: \n";
    while (visited[ziel] != 1)
    {
         cout << punktt << "-> ";
        maxr = 0;
        maxinde = 0;

        for (int i = 0; i < felder; i++)
        {
            if (visited[i] == 0)
            {
                if (qMatrix[punktt][i] > maxr)
                {
                    maxinde = i;
                    maxr = qMatrix[punktt][i];
                }
            }
        }

        punktt = maxinde;
        visited[maxinde] = 1;
        if (maxr == 0)
        {
            fehler = true;
            break;
        }

        if (punktt == ziel)
        {
            break;
        }
    }

    if (fehler)
    {
         cout << " | Da ist kein Weg ab hier\n";
    }
    else
    {
         cout << punktt << " ist der kuerzeste Weg\n";
    }
}
}

int erlaubteAktionen(int state, int erlaubteAktion[], double rMatrix[][simo],int felder)
{
    int k = 0, j = 0;
    while (j < felder)
    {
        if (rMatrix[state][j] >= 0.0)
        {
            erlaubteAktion[k] = j;
            k++;
        }
        j++;
    }
    return k;
}

double update(int punkt, int aktion, double rMatrix[][simo], double qMatrix[][simo],int zeigen,int felder)
{
    int j = 0, k = 0, index_of_max;
    double temp_max = 0.0, max_value = 0.0, sumA = 0.0;

    //Einlesen der Indexe wo Maximum im Punkt ist
    for (int i = 0; i < felder; i++)
    {
        max_index[i] = 0;

        if (temp_max == qMatrix[aktion][i])
        {
            max_index[j] = i;
            j++;
        }
        else if (temp_max < qMatrix[aktion][i])
        {
            j = 0;
            temp_max = qMatrix[aktion][i];
            max_index[j] = i;
            j++;
        }
    }

    //Auswahl einer zufälligen Zahl aus dem Maximum
    int a = zufallszahl(felder) % j;
    index_of_max = max_index[a];

    max_value = qMatrix[aktion][index_of_max];


    //Hauptupdate der Matrix
    qMatrix[punkt][aktion] = rMatrix[punkt][aktion] + (gammaLR * max_value);
    temp_max = 0.0;
    for (int i = 0; i < felder; i++)
    {
        for (int j = 0; j < felder; j++)
        {
            if (qMatrix[i][j] > temp_max)
            {
                temp_max = qMatrix[i][j];
            }
        }
    }
    if (zeigen == 1) {
        cout << "\nQ Max: " << temp_max;
        ausgabearray(qMatrix,felder);
    }
    if (temp_max > 0)
    {
        for (int i = 0; i < felder; i++)
        {
            for (int j = 0; j < felder; j++)
            {
                sumA += (qMatrix[i][j] / temp_max);
            }
        }

        sumA *= 100;
        return sumA;
    }
    else
    {
        return 0.0;
    }
}

int zufallszahl(int felder)
{
    return rand() % felder;
}

void ausgabearray(double a[][simo],int felder)
{

     cout << "\nMatrix: \n";
    for (int i = 0; i < felder; i++)
    {
        for (int j = 0; j < felder; j++)
        {
             cout << a[i][j] << "\t";
        }
         cout << "\n";
    }
}
void zeigekarte() {
    cout << "+---+      +---+" << endl;
    cout << "| 6 |      | 0 | --+" << endl;
    cout << "+---+      +---+   |         +---+" << endl;
    cout << "  |              +---+       | 3 |" << endl;
    cout << "  |  +---+    ---| 1 |----+  +---+" << endl;
    cout << "  +--| 5 |---/   +---+    |    |" << endl;
    cout << "     +---+        /     +---+  |" << endl;
    cout << "       -\\        -      | 2 |--+" << endl;
    cout << "         -\\ +---+       +---+" << endl;
    cout << "           -| 4 |         /" << endl;
    cout << "            +---+       /" << endl;
    cout << "              \\      +---+" << endl;
    cout << "               +-----| 7 |" << endl;
    cout << "                     +---+" << endl;
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
/*
Der Code besteht aus einer main-Funktion und mehreren Hilfsfunktionen. Die main-Funktion enthält den Hauptablauf des Q-Learning-Algorithmus. Es werden eine Q-Matrix, eine R-Matrix (Points Matrix) und andere Variablen initialisiert. Die R-Matrix enthält Belohnungen für den Übergang zwischen Zuständen.

Der Algorithmus trainiert die Q-Matrix durch wiederholtes Auswählen einer zufälligen Startposition, Auswahl einer zufälligen Aktion und Aktualisierung der Q-Matrix basierend auf den erzielten Belohnungen. Dieser Trainingsprozess wird x Mal wiederholt. Nach dem Training wird die trainierte Q-Matrix ausgegeben.

Schließlich wird der Pfad basierend auf der trainierten Q-Matrix gefunden, indem der Zustand schrittweise aktualisiert wird, bis der Zielzustand erreicht ist.

Die Funktionen erlaubteAktionen, update, zufallszahl und ausgabearray sind Hilfsfunktionen, die spezifische Aufgaben erfüllen und im Hauptablauf verwendet werden.

Bitte beachte, dass der Code keine Fehlerbehandlung oder Eingabeüberprüfung enthält. Es wird davon ausgegangen, dass die Eingaben korrekt sind und der Code problemlos ausgeführt werden kann.
*/

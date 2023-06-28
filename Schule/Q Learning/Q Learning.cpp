//Lösung eines MDP(Markov Decision Process) von Simon Seitz
#include <iostream>
#include <cstdlib>

using namespace std;
// Deklaration der globalen Variablen und Funktionen
constexpr auto simo = 8;
double qMatrix[simo][simo], gammaLR = 0.8; // Q-Matrix und der Discount-Faktor gamma (Learning Rate)
int max_index[simo], erlaubteAktion[simo]; // Hilfsarrays für den maximalen Index und verfügbare Aktionen
void ausgabearray(double a[][simo],int felder); // Funktion zur Ausgabe eines Arrays
int zufallszahl(int felder); // Funktion, die eine zufällige Zahl von 0 bis 7 zurückgibt
double update(int punkt, int aktion, double rMatrix[][simo], double qMatrix[][simo],int zeigen,int felder); // Funktion zur Aktualisierung der Q-Matrix
int erlaubteAktionen(int state, int erlaubteAktion[], double rMatrix[][simo],int felder); // Funktion zur Ermittlung verfügbarer Aktionen
void zeigekarte();//zeigt das labyrint

int main()
{
    int zeigen=2,labyrint;
    int t;//temp
    int ziel = 7;  // Deklaration der Anfangs- und Endzustände
    int punkt, groesemoeglicheraktionen, aktion;  // Deklaration von aktuellen Zustand, Größe der verfügbaren Aktionen und ausgewählter Aktion
    double final_max = 0.0, score = 0.0;  // Deklaration von final_max für das Finden des Maximums und des Punktestands
    double rMatrix[simo][simo];//-1 sind unmögliche verbindungen sinn:[von][nach]
    for (int i = 0; i < simo; i++){
        for (int j = 0; j < simo; j++) {
            rMatrix[j][i] = -1;
        }
    }
    int felder = 0, verbunden = 0;

    srand(time(nullptr)); // Initialize random seed

    cout << "Lernen anzeigen? 1 ja  2 nein" << endl;
    cin >> zeigen;//zeigen der einzelnen lern schritte zum verschnellern oder verlangsamen des programs
    cout << "Labyrint einlesen oder wählen? 1 einlesen 2voreingestellt" << endl;
    cin >> labyrint;
    if (labyrint == 1) {
        system("CLS");
        cout << "Wieviele Felder hat dein Labyrint?" << endl;
        cout << "Felder: ";
        cin >> felder;
        for (int i = 0; i < felder; i++)
        {
            cout << "Mit welchen Feldern ist " << i << " verbunden?" << endl;
            
            do
            {
                cin >> verbunden;
                rMatrix[i][verbunden] = 0.0;//moeglich
                rMatrix[verbunden][i] = 0.0;//auch in die andere richtung
            } while (verbunden<felder);
                
            
        }
        cout << "Was soll das ziel sein:" << endl;
        cin >> ziel;//devenition des ziel punktes
        for (int i = 0; i < felder; i++)
        {
            for (int j = 0; j < felder; j++)
            {
                qMatrix[i][j] = 0.0;
                if (rMatrix[i][ziel] == 0.0)rMatrix[i][j] = 100.0;//wen es ne verbindung zum ziel gibt
                if (i == j && i == ziel)rMatrix[i][j] = 100.0;//wen es das zeil ist
            }
        }
    }
    if (labyrint == 2) {
        felder = 8;
        zeigekarte();
        cout << "Was soll das ziel sein:" << endl;
        cin >> ziel;//devenition des ziel punktes
        for (int i = 0; i < felder; i++)
        {
            for (int j = 0; j < felder; j++)
            {
                qMatrix[i][j] = 0.0;

                // Festlegen der Belohnungen in der R-Matrix
                if ((i == 0 && j == 1) || (i == 1 && j == 5) || (i == 5 && j == 6) || (i == 5 && j == 4) || (i == 1 && j == 2) || (i == 2 && j == 3) || (i == 2 && j == 7) || (i == 4 && j == 7) || (i == 1 && j == 4) || (i == 2 && j == 7) || (i == 4 && j == 7))
                {
                    rMatrix[i][j] = 0.0;//moeglich
                    rMatrix[j][i] = 0.0;//auch in die andere richtung
                }
                if (rMatrix[i][ziel] == 0.0)rMatrix[i][j] = 100.0;//wen es ne verbindung zum ziel gibt
                if (i == j && i == ziel)rMatrix[i][j] = 100.0;//wen es das zeil ist

            }
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
    // Training Q Matrix
    for (int i = 0; i < 500; i++)
    {
        punkt = zufallszahl(felder);
        groesemoeglicheraktionen = erlaubteAktionen(punkt, erlaubteAktion, rMatrix,felder);
        aktion = erlaubteAktion[zufallszahl(felder) % groesemoeglicheraktionen];

        if (zeigen == 1)cout << "\nNaechster Schritt: " << aktion << "\n";
        score = update(punkt, aktion, rMatrix, qMatrix,zeigen,felder);


        if (zeigen == 1)cout << "\nScore : " << score;
    }

    //Finden des maximums
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

    int punktt;//anfangspunkt
    bool fehler = false;//für unmoegliche zuege
    int maxr = 0, maxinde = 0;

    if(labyrint==2)zeigekarte();
    while(true){
        int visited[simo] = { 0 };
        fehler = false;
        maxr = 0;
        maxinde = 0;
     cout << "Eingabe des Anfangspunktes: ";
     cin >> punktt;//eingabe des start punktes von wem zum ende navigiert werden soll

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

    //Einlesen der indexe wo max im punkt ist
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

    //Auswahl einer zufälligen Zahl as dem maximum
    int a = zufallszahl(felder) % j;
    index_of_max = max_index[a];

    max_value = qMatrix[aktion][index_of_max];


    //Haupt update der Matrix
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
/*
Der Code besteht aus einer main-Funktion und mehreren Hilfsfunktionen. Die main-Funktion enthält den Hauptablauf des Q-Learning-Algorithmus. Es werden eine Q-Matrix, eine R-Matrix (Points Matrix) und andere Variablen initialisiert. Die R-Matrix enthält Belohnungen für den Übergang zwischen Zuständen.

Der Algorithmus trainiert die Q-Matrix durch wiederholtes Auswählen einer zufälligen Startposition, Auswahl einer zufälligen Aktion und Aktualisierung der Q-Matrix basierend auf den erzielten Belohnungen. Dieser Trainingsprozess wird 500 Mal wiederholt. Nach dem Training wird die trainierte Q-Matrix ausgegeben.

Schließlich wird der Pfad basierend auf der trainierten Q-Matrix gefunden, indem der Zustand schrittweise aktualisiert wird, bis der Zielzustand erreicht ist.

Die Funktionen erlaubteAktionen, update, zufallszahl und ausgabearray sind Hilfsfunktionen, die spezifische Aufgaben erfüllen und im Hauptablauf verwendet werden.

Bitte beachte, dass der Code keine Fehlerbehandlung oder Eingabeüberprüfung enthält. Es wird davon ausgegangen, dass die Eingaben korrekt sind und der Code problemlos ausgeführt werden kann.
*/

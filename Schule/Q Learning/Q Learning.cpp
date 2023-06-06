//L�sung eines MDP(Markov Decision Process) von Simon Seitz
#include <iostream>
#include <cstdlib>

using namespace std;
// Deklaration der globalen Variablen und Funktionen

double qMatrix[8][8], gammaLR = 0.8; // Q-Matrix und der Discount-Faktor gamma (Learning Rate)
int max_index[8], erlaubteAktion[8]; // Hilfsarrays f�r den maximalen Index und verf�gbare Aktionen

void ausgabearray(double a[][8]); // Funktion zur Ausgabe eines Arrays
int zuffalszahl(); // Funktion, die eine zuf�llige Zahl von 0 bis 7 zur�ckgibt
double update(int punkt, int aktion, double rMatrix[][8], double qMatrix[][8]); // Funktion zur Aktualisierung der Q-Matrix
int erlaubteAktionen(int state, int erlaubteAktion[], double rMatrix[][8]); // Funktion zur Ermittlung verf�gbarer Aktionen
void zeigekarte();

int main()
{
    
    int i, j;//temp f�r Schleifen
    int zielPunkt = 7;  // Deklaration der Anfangs- und Endzust�nde
    int punkt, groesemoeglicheraktionen, aktion;  // Deklaration von aktuellen Zustand, Gr��e der verf�gbaren Aktionen und ausgew�hlter Aktion
    double final_max = 0.0, rMatrix[8][8], score = 0.0;  // Deklaration von final_max f�r das Finden des Maximums, der R-Matrix(reward), und des Punktestands

    srand(time(nullptr)); // Initialize random seed



    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            rMatrix[i][j] = -1.0;
            qMatrix[i][j] = 0.0;

            // Festlegen der Belohnungen in der R-Matrix
            if ((i == 0 && j == 1) || (i == 1 && j == 5) || (i == 5 && j == 6) || (i == 5 && j == 4) || (i == 1 && j == 2) || (i == 2 && j == 3) || (i == 2 && j == 7) || (i == 4 && j == 7) || (i == 1 && j == 4))
            {
                rMatrix[i][j] = 0.0;
            }
            // Festlegen der Belohnungen in der R-Matrix (symmetrisch)
            if ((j == 0 && i == 1) || (j == 1 && i == 5) || (j == 5 && i == 6) || (j == 5 && i == 4) || (j == 1 && i == 2) || (j == 2 && i == 3) || (j == 2 && i == 7) || (j == 4 && i == 7) || (j == 1 && i == 4))
            {
                rMatrix[i][j] = 0.0;
            }
            // Festlegen der Belohnungen in der R-Matrix (Zielzust�nde)
            if ((i == 2 && j == 7) || (i == 7 && j == 7) || (i == 4 && j == 7))
            {
                rMatrix[i][j] = 100.0;
            }
        }
    }

    // Ausgabe der R-Matrix
    std::cout << "\nAnfangs Matrix : \n";
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            std::cout << rMatrix[i][j] << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n\n";

    // Training Q Matrix
    for (i = 0; i < 500; i++)
    {
        punkt = zuffalszahl();
        groesemoeglicheraktionen = erlaubteAktionen(punkt, erlaubteAktion, rMatrix);
        aktion = erlaubteAktion[zuffalszahl() % groesemoeglicheraktionen];

        std::cout << "\nNext Step: " << aktion << "\n";
        score = update(punkt, aktion, rMatrix, qMatrix);


        std::cout << "\nScore : " << score;
    }

    //Finden des maximums
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (final_max < qMatrix[i][j])
            {
                final_max = qMatrix[i][j];
            }
        }
    }

    std::cout << "\n\nTrained Q Matrix: \n";
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            std::cout << (qMatrix[i][j] / final_max * 100.0) << "\t";
        }
        std::cout << "\n";
    }

    int punktt;
    bool fehler = false;
    int maxr = 0, maxinde = 0;

    zeigekarte();
    while(true){
        int visited[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        fehler = false;
        maxr = 0;
        maxinde = 0;
    std::cout << "Eingabe des anfangspunktes: ";
    std::cin >> punktt;

    // Ausgabe des Pfads basierend auf der trainierten Q-Matrix
    std::cout << "Weg: \n";
    while (visited[zielPunkt] != 1)
    {
        std::cout << punktt << "-> ";
        maxr = 0;
        maxinde = 0;

        for (i = 0; i < 8; i++)
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

        if (punktt == zielPunkt)
        {
            break;
        }
    }

    if (fehler)
    {
        std::cout << punktt << " | Da ist kein Weg ab hier\n";
    }
    else
    {
        std::cout << punktt << " ist der kuerzeste Weg\n";
    }
}
}

int erlaubteAktionen(int state, int erlaubteAktion[], double rMatrix[][8])
{
    int k = 0, j = 0;
    while (j < 8)
    {
        if (rMatrix[state][j] >= 0.0)
        {
            erlaubteAktion[k] = j;
            k++;
        }
        j++;
    }
    std::cout << "\n";
    return k;
}

double update(int punkt, int aktion, double rMatrix[][8], double qMatrix[][8])
{
    int i = 0, j = 0, k = 0, index_of_max;
    double temp_max = 0.0, max_value = 0.0, sumA = 0.0;

    //Einlesen der indexe wo max im punkt ist
    for (i = 0; i < 8; i++)
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

    //Auswahl einer zuf�lligen Zahl as dem maximum
    int a = zuffalszahl() % j;
    index_of_max = max_index[a];

    max_value = qMatrix[aktion][index_of_max];


    //Haupt update der Matrix
    qMatrix[punkt][aktion] = rMatrix[punkt][aktion] + (gammaLR * max_value);
    temp_max = 0.0;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (qMatrix[i][j] > temp_max)
            {
                temp_max = qMatrix[i][j];
            }
        }
    }

    std::cout << "\nQ Max: " << temp_max;
    ausgabearray(qMatrix);
    if (temp_max > 0)
    {
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                sumA = sumA + (qMatrix[i][j] / temp_max);
            }
        }

        sumA = sumA * 100;
        return sumA;
    }
    else
    {
        return 0.0;
    }
}

int zuffalszahl()
{
    return rand() % 8;
}

void ausgabearray(double a[][8])
{
    int i, j;

    std::cout << "\nMatrix: \n";
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            std::cout << a[i][j] << "\t";
        }
        std::cout << "\n";
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
 Der Code besteht aus einer main-Funktion und mehreren Hilfsfunktionen. Die main-Funktion enth�lt den Hauptablauf des Q-Learning-Algorithmus. Es werden eine Q-Matrix, eine R-Matrix (Points Matrix) und andere Variablen initialisiert. Die R-Matrix enth�lt Belohnungen f�r den �bergang zwischen Zust�nden.

Der Algorithmus trainiert die Q-Matrix durch wiederholtes Ausw�hlen einer zuf�lligen Startposition, Auswahl einer zuf�lligen Aktion und Aktualisierung der Q-Matrix basierend auf den erzielten Belohnungen. Dieser Trainingsprozess wird 500 Mal wiederholt. Nach dem Training wird die trainierte Q-Matrix ausgegeben.

Schlie�lich wird der Pfad basierend auf der trainierten Q-Matrix gefunden, indem der Zustand schrittweise aktualisiert wird, bis der Zielzustand erreicht ist.

Die Funktionen erlaubteAktionen, update, zuffalszahl und ausgabearray sind Hilfsfunktionen, die spezifische Aufgaben erf�llen und im Hauptablauf verwendet werden.

Bitte beachte, dass der Code keine Fehlerbehandlung oder Eingabe�berpr�fung enth�lt. Es wird davon ausgegangen, dass die Eingaben korrekt sind und der Code problemlos ausgef�hrt werden kann.
*/
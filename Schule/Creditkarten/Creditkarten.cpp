#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

bool isNumberString(const string& s) {
    int len = s.length();
    for (int i = 0; i < len; i++) {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

int main() {
    //kommentare sind für mich aus der erklärung kopiert
    string ccNumber;

    cout << "Dieses Programm nutzt den Luth Algorytmus um eine CC Nummer zu pruefen" << endl;
    cout << "Bitte gebe 'exit' ein um das programm zu beenden" << endl;

    while (true) {

        cout << "Bitte gebe nun die zu ueberpruefende CC nummer ein: ";
        cin >> ccNumber;

        if (ccNumber == "exit") break;

        else if (!isNumberString(ccNumber)) {
            cout << "Falsche eingabe! " << endl;
            continue;
        }

        int len = ccNumber.length();
        int doubleEvenSum = 0;

        // Schritt 1 besteht darin, jede zweite Ziffer von rechts beginnend zu verdoppeln. Wenn dies zu einer zweistelligen Zahl führt, addieren Sie beide Ziffern, um eine einstellige Zahl zu erhalten. Summieren Sie schließlich alle Antworten, um das zwischen ergebnis („doubleEvenSum“) zu erhalten.

        for (int i = len - 2; i >= 0; i = i - 2) {
            int dbl = ((ccNumber[i] - 48) * 2);
            if (dbl > 9) {
                dbl = (dbl / 10) + (dbl % 10);
            }
            doubleEvenSum += dbl;
        }

        // Schritt 2 besteht darin, jede ungerade Stelle von rechts zum Wert ('doubleEvenSum') zu addieren

        for (int i = len - 1; i >= 0; i = i - 2) {
            doubleEvenSum += (ccNumber[i] - 48);
        }

        // Schritt 3 besteht darin, zu prüfen, ob der abschließender wert („doubleEvenSum“) ein Vielfaches von 10 ist. Wenn ja, handelt es sich um eine gültige CC-Nummer. Sonst nicht.

       
        if (doubleEvenSum % 10 == 0) {
            cout << "Existiert!";
            cout << endl;
        }
        else {
            cout << "Exestiert nicht!";
            cout << endl;
        }
        continue;
    }

    return 0;
}

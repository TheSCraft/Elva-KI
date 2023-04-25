#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;
vector<string> explode(const string& delimiter, const string& explodeme);
double sigmoid(double x);
double dSigmoid(double x);
double zufallszahl();
void mischen(int* array, size_t n);



constexpr auto anzahleingangsneuronen = 2;
constexpr auto anzahlversteckterneuronen = 2;
constexpr auto anzahlausgangsneuronen = 1;
constexpr auto anzahltraningsdaten = 4;


int main() {
    cout << "Lerngenauichkeit eingeben(0.1)" << endl;
    double lerngenauichkeit = 0.1;
    cin >> lerngenauichkeit;
    double versteckteebene[anzahlversteckterneuronen];
    double ausgangsebene[anzahlausgangsneuronen];
    double versteckteebeneBias[anzahlversteckterneuronen];
    double ausgangsebeneBias[anzahlausgangsneuronen];
    double versteckteWeights[anzahleingangsneuronen][anzahlversteckterneuronen];
    double ausgangsWeights[anzahlversteckterneuronen][anzahlausgangsneuronen];


    double training_inputs[anzahltraningsdaten][anzahleingangsneuronen] = { {0.0,0.0},
                                                                            {1.0,0.0},
                                                                            {0.0,1.0},
                                                                            {1.0,1.0} };
    double training_outputs[anzahltraningsdaten][anzahlausgangsneuronen] = { {0.0},
                                                                             {1.0},
                                                                             {1.0},
                                                                             {0.0} };
    int temp = 0;
    string datein;
    cout << "Manuelle eingabe? 1ja 2nein 3ladevorherige werte"<<endl;
    cin >> temp;
    if (temp == 1) {
        cout << "Versteckte Weights" << endl;
        for (int i = 0; i < anzahleingangsneuronen; i++) {
            for (int j = 0; j < anzahlversteckterneuronen; j++) {
                cin>>versteckteWeights[i][j];
            }
        }
        cout << "versteckte Bias" << endl;

        for (int i = 0; i < anzahlversteckterneuronen; i++) {
            cin>>versteckteebeneBias[i];
          
        }
        cout << "ausgangs Weights" << endl;
        for (int i = 0; i < anzahlversteckterneuronen; i++) {
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
               

                cin >> ausgangsWeights[i][j];
            }
        }
        cout << "ausgangs Bias" << endl;

        for (int i = 0; i < anzahlausgangsneuronen; i++) {
            cin>>ausgangsebeneBias[i];
        }
    }
    if (temp == 2) {
        for (int i = 0; i < anzahleingangsneuronen; i++) {
            for (int j = 0; j < anzahlversteckterneuronen; j++) {
                versteckteWeights[i][j] = zufallszahl();
            }
        }
        for (int i = 0; i < anzahlversteckterneuronen; i++) {
            versteckteebeneBias[i] = zufallszahl();
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                ausgangsWeights[i][j] = zufallszahl();
            }
        }
        for (int i = 0; i < anzahlausgangsneuronen; i++) {
            ausgangsebeneBias[i] = zufallszahl();
        }
    }
    if (temp == 3) {
        cout << "dateiname:";
        cin >> datein;
        datein += ".txt";
        string in;
        int tempp = 0;
        ifstream MyReadFilee(datein);
        getline(MyReadFilee, in);
        MyReadFilee.close();
        vector<string> inn = explode("|", in);
        cout << "Versteckte Weights" << endl;
        for (int i = 0; i < anzahleingangsneuronen; i++) {
            for (int j = 0; j < anzahlversteckterneuronen; j++) {
                 versteckteWeights[i][j]= stod(inn[tempp]);
                 tempp++;
                 cout << versteckteWeights[i][j] << endl;
            }
        }
        cout << "versteckte Bias" << endl;

        for (int i = 0; i < anzahlversteckterneuronen; i++) {
            versteckteebeneBias[i] = stod(inn[tempp]);
            tempp++;
            cout << versteckteebeneBias[i] << endl;


        }
        cout << "ausgangs Weights" << endl;
        for (int i = 0; i < anzahlversteckterneuronen; i++) {
            for (int j = 0; j < anzahlausgangsneuronen; j++) {


                 ausgangsWeights[i][j] = stod(inn[tempp]);
                 tempp++;
                 cout << ausgangsWeights[i][j] << endl;

            }
        }
        cout << "ausgangs Bias" << endl;

        for (int i = 0; i < anzahlausgangsneuronen; i++) {
            ausgangsebeneBias[i] = stod(inn[tempp]);
            tempp++;
            cout << ausgangsebeneBias[i] << endl;

        }
    }
    int reinfolgetrainingsdaten[] = { 0,1,2,3 };

    cout << "Wie oft soll trainiert werden?"<<endl;
    int anzahllernzyclen = 10000;
    cin >> anzahllernzyclen;
    //training
    for (int zyclus = 0; zyclus < anzahllernzyclen; zyclus++) {

        // das training zufällig machen
        mischen(reinfolgetrainingsdaten, anzahltraningsdaten);

        // damit alle tranings gemacht werden
        for (int x = 0; x < anzahltraningsdaten; x++) {

            int i = reinfolgetrainingsdaten[x];

            // Forward pass

            // berechnen der versteckten ebene
            for (int j = 0; j < anzahlversteckterneuronen; j++) {
                double activation = versteckteebeneBias[j];
                for (int k = 0; k < anzahleingangsneuronen; k++) {
                    activation += training_inputs[i][k] * versteckteWeights[k][j];
                }
                versteckteebene[j] = sigmoid(activation);
            }

            // berechnen der ausgangs ebene
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                double activation = ausgangsebeneBias[j];
                for (int k = 0; k < anzahlversteckterneuronen; k++) {
                    activation += versteckteebene[k] * ausgangsWeights[k][j];
                }
                ausgangsebene[j] = sigmoid(activation);
            }

            
            cout << "Input: ";
            for (int w= 0; w < anzahleingangsneuronen; w++)
            {
                cout << training_inputs[i][w] << " ";
            }
            cout << " Output: ";
            for (int q = 0; q < anzahlausgangsneuronen; q++)
            {
                cout << ausgangsebene[0] << " ";
            }
            cout << " Richtiger Output: ";
            for (int w = 0; w < anzahlausgangsneuronen; w++)
            {
                cout << training_outputs[i][w] << " ";
            }
            cout << " |" << (zyclus * 100) / anzahllernzyclen << "%" << endl;


            // Backprop

            // berechnen der änderungen
            double deltaOutput[anzahlausgangsneuronen];
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                double errorOutput = (training_outputs[i][j] - ausgangsebene[j]);
                deltaOutput[j] = errorOutput * dSigmoid(ausgangsebene[j]);
            }

           
            double deltaHidden[anzahlversteckterneuronen];
            for (int j = 0; j < anzahlversteckterneuronen; j++) {
                double errorHidden = 0.0;
                for (int k = 0; k < anzahlausgangsneuronen; k++) {
                    errorHidden += deltaOutput[k] * ausgangsWeights[j][k];
                }
                deltaHidden[j] = errorHidden * dSigmoid(versteckteebene[j]);
            }
            // aendern der werte
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                ausgangsebeneBias[j] += deltaOutput[j] * lerngenauichkeit;
                for (int k = 0; k < anzahlversteckterneuronen; k++) {
                    ausgangsWeights[k][j] += versteckteebene[k] * deltaOutput[j] * lerngenauichkeit;
                }
            }

            for (int j = 0; j < anzahlversteckterneuronen; j++) {
                versteckteebeneBias[j] += deltaHidden[j] * lerngenauichkeit;
                for (int k = 0; k < anzahleingangsneuronen; k++) {
                    versteckteWeights[k][j] += training_inputs[i][k] * deltaHidden[j] * lerngenauichkeit;
                }
            }
        }
    }

    
   
    string finalewete ="";
    cout << "Finale Vesteckte Weights\n[";
    for (int j = 0; j < anzahlversteckterneuronen; j++) {
        
        for (int k = 0; k < anzahleingangsneuronen; k++) {
            cout << "[" << versteckteWeights[k][j] << "]";
            finalewete += to_string(versteckteWeights[k][j]) + '|';
        }
    }

    cout << "]\nFinal Versteckte Biases\n[";
    for (int j = 0; j < anzahlversteckterneuronen; j++) {
        cout << "[" << versteckteebeneBias[j] << "]";
        finalewete += to_string(versteckteebeneBias[j]) + '|';
    }

    cout <<"]\nFinal Output Weights\n";
    for (int j = 0; j < anzahlausgangsneuronen; j++) {
        cout <<"[";
        for (int k = 0; k < anzahlversteckterneuronen; k++) {
           cout << "[" << ausgangsWeights[k][j]<<"]";
           finalewete += to_string(ausgangsWeights[k][j]) + '|';

        }
        cout << "]\n";
    }

    cout << "Final Output Biases\n[";
    for (int j = 0; j < anzahlausgangsneuronen; j++) {
        cout <<"[" << ausgangsebeneBias[j]<<"]";
        finalewete += to_string(ausgangsebeneBias[j]) + '|';
    }

    cout << "]\n";
    cout << "Speichern? 1ja 2nein" << endl;
    cin >> temp;
    if (temp == 1) {
        cout << "dateiname:";
        cin >> datein;
        datein += ".txt";
        cout << finalewete;
        ofstream MyReadFilee(datein);
        MyReadFilee << finalewete;
        MyReadFilee.close();
    }
    return 0;
}

void mischen(int* array, size_t n){
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
double dSigmoid(double x) { return x * (1 - x); }
double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double zufallszahl() { return ((double)rand()) / ((double)RAND_MAX); }
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

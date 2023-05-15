  #include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;
vector<string> explode(const string& delimiter, const string& explodeme);
double sigmoid(double x);
double dSigmoid(double x);
double zufallszahl();
void mischen(int* array, size_t n);



constexpr auto anzahleingangsneuronen = 2;
constexpr auto anzahlversteckterneuroneneins = 4;
constexpr auto anzahlversteckterneuronenzwei = 4;
constexpr auto anzahlausgangsneuronen = 1;
constexpr auto anzahltraningsdaten = 4;


int main() {
    cout << "Lerngenauichkeit eingeben(0.1)" << endl;
    double lerngenauichkeit = 0.1;
    cin >> lerngenauichkeit;
    double versteckteebeneeins[anzahlversteckterneuroneneins]{};
    double versteckteebenezwei[anzahlversteckterneuronenzwei]{};
    double ausgangsebene[anzahlausgangsneuronen]{};
    double versteckteebeneeinsBias[anzahlversteckterneuroneneins]{};
    double versteckteebenezweiBias[anzahlversteckterneuronenzwei]{};
    double ausgangsebeneBias[anzahlausgangsneuronen]{};
    double versteckteWeightseins[anzahleingangsneuronen][anzahlversteckterneuroneneins]{};
    double versteckteWeightszwei[anzahlversteckterneuroneneins][anzahlversteckterneuronenzwei]{};
    double ausgangsWeights[anzahlversteckterneuronenzwei][anzahlausgangsneuronen]{};


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
    cout << "Manuelle eingabe? 1ja 2nein 3ladevorherige werte" << endl;
    cin >> temp;
    
    if (temp == 1) {
        cout << "Versteckte Weights eins" << endl;
        for (int i = 0; i < anzahleingangsneuronen; i++) {
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                cin >> versteckteWeightseins[i][j];
            }
        }
        cout << "versteckte Bias eins" << endl;

        for (int i = 0; i < anzahlversteckterneuroneneins; i++) {
            cin >> versteckteebeneeinsBias[i];

        }

         cout << "Versteckte Weights zwei" << endl;
        for (int i = 0; i < anzahlversteckterneuroneneins; i++) {
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                cin >> versteckteWeightszwei[i][j];
            }
        }
        cout << "versteckte Bias zwei" << endl;

        for (int i = 0; i < anzahlversteckterneuronenzwei; i++) {
            cin >> versteckteebenezweiBias[i];

        }
        cout << "ausgangs Weights" << endl;
        for (int i = 0; i < anzahlversteckterneuronenzwei; i++) {
            for (int j = 0; j < anzahlausgangsneuronen; j++) {


                cin >> ausgangsWeights[i][j];
            }
        }
        cout << "ausgangs Bias" << endl;

        for (int i = 0; i < anzahlausgangsneuronen; i++) {
            cin >> ausgangsebeneBias[i];
        }
    }
    
    if (temp == 2) {
        for (int i = 0; i < anzahleingangsneuronen; i++) {
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                versteckteWeightseins[i][j] = zufallszahl();
            }
        }
        for (int i = 0; i < anzahlversteckterneuroneneins; i++) {
            versteckteebeneeinsBias[i] = zufallszahl();
            
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                versteckteWeightszwei[i][j] = zufallszahl();
            }
        }

        for (int i = 0; i < anzahlversteckterneuronenzwei; i++) {
            versteckteebenezweiBias[i] = zufallszahl();
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
        cout << "Versteckte Weights eins" << endl;
        for (int i = 0; i < anzahlversteckterneuroneneins; i++) {
            for (int j = 0; j < anzahleingangsneuronen; j++) {
                versteckteWeightseins[j][i] = stod(inn[tempp]);
                tempp++;
                cout << fixed <<setprecision(15)<< versteckteWeightseins[j][i] << endl;
            }
        }
        cout << "versteckte Bias eins" << endl;

        for (int i = 0; i < anzahlversteckterneuroneneins; i++) {
            versteckteebeneeinsBias[i] = stod(inn[tempp]);
            tempp++;
            cout << versteckteebeneeinsBias[i] << endl;


        }
        cout << "Versteckte Weights zwei" << endl;
        for (int i = 0; i < anzahlversteckterneuronenzwei; i++) {
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                versteckteWeightszwei[j][i] = stod(inn[tempp]);
                tempp++;
                cout << versteckteWeightszwei[j][i] << endl;
            }
        }
        cout << "versteckte Bias zwei" << endl;

        for (int i = 0; i < anzahlversteckterneuronenzwei; i++) {
            versteckteebenezweiBias[i] = stod(inn[tempp]);
            tempp++;
            cout << versteckteebenezweiBias[i] << endl;


        }

        cout << "ausgangs Weights " << endl;
        for (int i = 0; i < anzahlversteckterneuronenzwei; i++) {
            for (int j = 0; j < anzahlausgangsneuronen; j++) {


                ausgangsWeights[j][i] = stod(inn[tempp]);
                tempp++;
                cout << ausgangsWeights[j][i] << endl;

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
    cout << "Training?  1ja  2nein";
    cin >> temp;
    if (temp == 2) {
        int input[anzahleingangsneuronen]{};
        cout << "Inputs:" <<endl;
        
        for (int k = 0; k < anzahleingangsneuronen; k++) {
            cin >> input[k];
        }
        for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
            double activation = versteckteebeneeinsBias[j];
            for (int k = 0; k < anzahleingangsneuronen; k++) {
                activation += input[k] * versteckteWeightseins[k][j];
            }
            versteckteebeneeins[j] = sigmoid(activation);
        }
        for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
            double activation = versteckteebenezweiBias[j];
            for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
                activation += versteckteebeneeins[k] * versteckteWeightszwei[k][j];
            }
            versteckteebenezwei[j] = sigmoid(activation);
        }

        // berechnen der ausgangs ebene
        for (int j = 0; j < anzahlausgangsneuronen; j++) {
            double activation = ausgangsebeneBias[j];
            for (int k = 0; k < anzahlversteckterneuronenzwei; k++) {
                activation += versteckteebenezwei[k] * ausgangsWeights[k][j];
            }
            ausgangsebene[j] = sigmoid(activation);
        }


        
        cout << " Output: ";
        for (int q = 0; q < anzahlausgangsneuronen; q++)
        {
            cout << ausgangsebene[q] << " ";
        }
       






        return 0;
    }
    cout << "Wie oft soll trainiert werden?" << endl;
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
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                double activation = versteckteebeneeinsBias[j];
                for (int k = 0; k < anzahleingangsneuronen; k++) {
                    activation += training_inputs[i][k] * versteckteWeightseins[k][j];
                }
                versteckteebeneeins[j] = sigmoid(activation);
            }
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                double activation = versteckteebenezweiBias[j];
                for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
                    activation += versteckteebeneeins[k] * versteckteWeightszwei[k][j];
                }
                versteckteebenezwei[j] = sigmoid(activation);
            }

            // berechnen der ausgangs ebene
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                double activation = ausgangsebeneBias[j];
                for (int k = 0; k < anzahlversteckterneuronenzwei; k++) {
                    activation += versteckteebenezwei[k] * ausgangsWeights[k][j];
                }
                ausgangsebene[j] = sigmoid(activation);
            }


            cout << "Input: ";
            for (int w = 0; w < anzahleingangsneuronen; w++)
            {
                cout << fixed << setprecision(2) << training_inputs[i][w] << " ";
            }
            cout << " Output: ";
            for (int q = 0; q < anzahlausgangsneuronen; q++)
            {
                cout << ausgangsebene[q] << " ";
            }
            cout << " Richtiger Output: ";
            for (int w = 0; w < anzahlausgangsneuronen; w++)
            {
                cout << training_outputs[i][w] << " ";
            }
            cout << " |" << (zyclus * 100) / anzahllernzyclen << "%" << endl;


            // Backprop

            // berechnen der änderungen
            double deltaOutput[anzahlausgangsneuronen]{};
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                double errorOutput = (training_outputs[i][j] - ausgangsebene[j]);
                deltaOutput[j] = errorOutput * dSigmoid(ausgangsebene[j]);
            }
            double deltaHiddenzwei[anzahlversteckterneuronenzwei]{};
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                double errorHidden = 0.0;
                for (int k = 0; k < anzahlausgangsneuronen; k++) {
                    errorHidden += deltaOutput[k] * ausgangsWeights[j][k];
                }
                deltaHiddenzwei[j] = errorHidden * dSigmoid(versteckteebenezwei[j]);
            }

            double deltaHiddeneins[anzahlversteckterneuroneneins]{};
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                double errorHidden = 0.0;
                for (int k = 0; k < anzahlversteckterneuronenzwei; k++) {
                    errorHidden += deltaHiddenzwei[k] * versteckteWeightszwei[j][k];
                }
                deltaHiddeneins[j] = errorHidden * dSigmoid(versteckteebeneeins[j]);
            }

            // aendern der werte
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                ausgangsebeneBias[j] += deltaOutput[j] * lerngenauichkeit;
                for (int k = 0; k < anzahlversteckterneuronenzwei; k++) {
                    ausgangsWeights[k][j] += versteckteebenezwei[k] * deltaOutput[j] * lerngenauichkeit;
                }
            }
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                versteckteebenezweiBias[j] += deltaHiddenzwei[j] * lerngenauichkeit;
                for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
                    versteckteWeightszwei[k][j] += versteckteebeneeins[j] * deltaHiddenzwei[j] * lerngenauichkeit;

                }
            }
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                versteckteebeneeinsBias[j] += deltaHiddeneins[j] * lerngenauichkeit;

                for (int k = 0; k < anzahleingangsneuronen; k++) {
                    versteckteWeightseins[k][j] += training_inputs[i][k] * deltaHiddeneins[j] * lerngenauichkeit;
                }
            }
        }
    }
    

   
    stringstream finalewete;
    finalewete << fixed << setprecision(15);

    cout << "Finale Vesteckte Weights erste ebene\n[";
    for (int j = 0; j < anzahlversteckterneuroneneins; j++) {

        for (int k = 0; k < anzahleingangsneuronen; k++) {
            cout << "[" << fixed << setprecision(15) << versteckteWeightseins[k][j] << "]";
            finalewete <<versteckteWeightseins[k][j]<< '|';
        }
    }
    cout << "]\nFinal Versteckte Biases erste ebene\n[";
    for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
        cout << "[" << versteckteebeneeinsBias[j] << "]";
        finalewete <<versteckteebeneeinsBias[j] << '|';
    }

    cout << "]\nFinale Vesteckte Weights zweite ebene\n[";
    for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {

        for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
            cout << "[" << versteckteWeightszwei[k][j] << "]";
            finalewete << versteckteWeightszwei[k][j] << '|';
        }
    }
    cout << "]\nFinal Versteckte Biases zweite ebene\n[";
    for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
        cout << "[" << versteckteebenezweiBias[j] << "]";
        finalewete <<versteckteebenezweiBias[j]<< '|';
    }

    cout << "]\nFinal Output Weights\n";
    for (int j = 0; j < anzahlausgangsneuronen; j++) {
        cout << "[";
        for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
            cout << "[" << ausgangsWeights[k][j] << "]";
            finalewete <<ausgangsWeights[k][j]<< '|';

        }
        cout << "]\n";
    }

    cout << "Final Output Biases\n[";
    for (int j = 0; j < anzahlausgangsneuronen; j++) {
        cout << "[" << ausgangsebeneBias[j] << "]";
        finalewete <<ausgangsebeneBias[j]<< '|';
    }

    cout << "]\n";
    cout << "Speichern? 1ja 2nein" << endl;
    cin >> temp;
    if (temp == 1) {
        cout << "dateiname:";
        cin >> datein;
        datein += ".txt";
        string sf = finalewete.str();
        cout << sf;
        ofstream MyReadFilee(datein);
        MyReadFilee << sf;
        MyReadFilee.close();
    }
    return 0;
}

void mischen(int* array, size_t n) {
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
double zufallszahl() { return (((double)rand()) / ((double)RAND_MAX)); }

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

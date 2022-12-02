#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> explode(const string& delimiter, const string& explodeme);

int main(int argc, char* argv[])
{
    int t = 0;
    string pas,nam;
    string namen = "Simon Seitz-Max Musterman";
    string passw = "123-321";
    cout << "Name: ";
    cin >> nam;
    cout << "Passwort: ";
    cin >> pas;
    vector<string> p = explode("-", passw);
    vector<string> n = explode("-", namen);
    for (int i = 0; i < p.size(); i++)
    {
        if (nam == p[i] && pas == n[i]) {
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
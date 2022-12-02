#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> explode(const string& delimiter, const string& explodeme);

int main(int argc, char* argv[])
{
    string str = "hi";
    string m, t;
    cout << "text eingeben"<<endl;
    cin >> m;
    getline(cin, t);
    str = m + t;
    cout << str << endl;
    vector<string> v = explode(" ", str);
    for (int i = 0; i < v.size(); i++) {
        cout << i << " [" << v[i] << "] " << endl;
    }
    for (int r = 0; r < v.size(); r++) {
        if (v[r] == "hallo") {
            cout << r << "ist hallo";
        }
    }
}


vector<string> explode(const string& delimiter, const string& str)
{
    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng == 0)
        return arr;//no change

    int i = 0;
    int k = 0;
    while (i < strleng)
    {
        int j = 0;
        while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
            j++;
        if (j == delleng)//found delimiter
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


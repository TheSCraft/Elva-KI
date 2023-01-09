//hi
#include <iostream> 
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <fstream>
#include <iomanip>
#pragma warning(disable : 4996)
using namespace std;

void game();      //fast wichtiger als die main haha
void zeigebild();   //ausgabe des bildes
void roeren();   //erschaft und bewegt die hindernisse
void voegelchen();    // bewegung von vogel is logisch
bool gameover();  //gugt ob vogel tot sein sollte
void ehoeungscore();  //erhöt den score
void hilfe();     //wunderbare erklärung wenn man es nicht checkt
void menubild();   //wunderschoenes menue
void endebild();  //wunderschönes bild fuer game over
void creditsbild();  //ist logisch
ifstream in;  
ofstream outp;
char c[30][21]; //pixel(x y)
int n[30][21];  //ueberpruefung weil wichtig
int highscore;
int taste = 0, score = 0, t = 0, langeweilevogel= 0, vogelx = 0, vogely = 0; 
bool err; 
int main()
{
    srand(time(0));  
    in.open("nichtcheaten.txt");  
    if (in.is_open()) 
    {
        in >> highscore;
        in.close();
        err = false;  
    }
    else
    {
        highscore = 0; 
        err = true;
    }

    int a = 0, b;
    char wahl; 
    while (1) 
    {
        if (a == 0) goto play;
        if (a > 0)               //wenn man nicht das erstemal spielt
        {
            score = 0;
            cout << "Zurueck zum Menu? [j/n] oder nochmal [d] ";
            cin >> wahl;
            if (wahl == 'j') goto play;
            if (wahl == 'n') goto exit;
            if (wahl == 'd') game();
        }
    play:
        menubild();
        cin >> wahl;
        switch (wahl) 
        {
        case '1':
        {
            game(); 
            break;
        }
        case '2': 
        {
            hilfe();
            goto play;
            break;
        }
        case '3':
        {
            creditsbild();
            goto play;
            break;
        }
        case '4':
        {
            goto exit; 
            break;
        }
        default:
        {
            goto play;
            break;
        }
        }
        a++; //wieoft gespielt
    }
exit:
    {
        cout << "Dann halt Tschuess. ):";
    }

    return 0;
}

void game()  
{
    int x, y;
    char s;
    for (y = 0; y < 21; y++)  //bildschirm initialisirung
    {
        for (x = 0; x < 30; x++)
        {
            if (y < 20)
            {
                c[x][y] = ' ';
                n[x][y] = 0;
            }
            if (y == 20)
            {
                c[x][y] = '-';
                n[x][y] = 2;
            }
        }
    }
    c[10][10] = 'V';  //start vom fogel
    zeigebild();      
    while (1)       //eigentliches game
    {
        s = '~';  
        Sleep(0.2 * 1000);  //hier ändern wenn zulagsam oder zu schnell
        t++;              //wielange läuft das spiel
        if (kbhit()) //abfrage tastegedrückt
        {
            s = getch();        //welche taste
            if (s != '~') taste = 1; //wenn es eine taste ist 
        }
        for (x = 0; x < 30; x++) //boden
        {
            c[x][20] = '-';
            n[x][20] = 2;
        }
        voegelchen();                       
        ehoeungscore();                 
        if (gameover() == true) goto gameEnd;   
        roeren();                            
        if (score > highscore) highscore = score;  
        zeigebild();                            

        if (taste > 0) taste++;           //wenn mann zuschnell drückt damit man auch chansen hat
        if (taste == 3) taste = 0;          //ansonsten kann man cheaten
    }
gameEnd:   
    {
        if (score > highscore) highscore = score;
        if (err == false)              // wenn es nen hiscore gibt
        {
            outp.open("nichtcheaten.txt");
            outp << highscore;
            outp.close();
        }
        zeigebild();    
        endebild();
        return;
    }
}

void zeigebild()   
{
    int x, y;
    system("cls");    
    for (y = 0; y < 21; y++) 
    {
        for (x = 0; x < 30; x++)
        {
            if (x < 29) cout << c[x][y];
            if (x == 29) cout << c[x][y] << endl;
        }
    }
    cout << "" << endl;
    cout << "Your Score: " << score;
}

void roeren()  
{
    int x, y, r;
    if (t == 10)   //alle 10 durchläufe kommt ne neue 
    {
        r = (rand() % 11) + 5;  //wo das loch ist
        for (y = 0; y < 20; y++)  
        {
            c[29][y] = 'H';  
            n[29][y] = 2;    
        }
        c[29][r - 1] = ' ';  //erstelung des loches
        c[29][r] = ' ';
        c[29][r + 1] = ' ';
        n[29][r - 1] = 0;
        n[29][r] = 0;
        n[29][r + 1] = 0;
        t = 0;
        goto bewege; 
    }
    else goto bewege;
bewege:                
    {
        for (y = 0; y < 20; y++)  
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == 'H')  //alle 1 nach links
                {
                    if (x > 0)
                    {
                        c[x - 1][y] = 'H';
                        n[x - 1][y] = 2;
                        c[x][y] = ' ';
                        n[x][y] = 0;
                    }
                    if (x == 0)  //muss weil sonst error hat lange gedauert haha
                    {
                        c[x][y] = ' ';
                        n[x][y] = 0;
                    }
                }
            }
        }
    }
}

void voegelchen()   
{
    int x, y;
    if (taste > 0) //wenn taste vogel hoch
    {
        langeweilevogel= 0;
        for (y = 0; y < 20; y++)   //wo is das voegelchen
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == 'V')
                {
                    if (y > 0)
                    {
                        c[x][y - 1] = 'V';  //vogel hoch
                        c[x][y] = ' ';
                        vogelx = x;       
                        vogely = y - 1;      
                        return;         //zurück zur game funktion
                    }
                }
            }
        }
    }
    else   //wenn nichts vogel stuertzt
    {
        langeweilevogel++;
        for (y = 0; y < 20; y++)
        {
            for (x = 0; x < 30; x++)
            {
                if (c[x][y] == 'V')
                {
                    if (y < 20)  //wenn der vogel nicht am boden ist so wie ich nach 3 stunden coden haha
                    {
                        if (langeweilevogel< 3)   //wenn erst 3 vergangen 1 pixel schnell
                        {
                            c[x][y + 1] = 'V';
                            c[x][y] = ' ';
                            vogelx = x;
                            vogely = y + 1;
                            return;
                        }
                        else if (langeweilevogel> 2 && langeweilevogel< 5)  //wenn mehr vogel fällt schneller
                        {
                            c[x][y + 2] = 'V';
                            c[x][y] = ' ';
                            vogelx = x;
                            vogely = y + 2;
                            return;
                        }
                        else if (langeweilevogel> 4)//wenn noch mehr zeit vogel macht sturzflug haha
                        {
                            c[x][y + 3] = 'V';
                            c[x][y] = ' ';
                            vogelx = x;
                            vogely = y + 3;
                            return;
                        }
                    }
                    else
                    {
                        return;  //wenn boden gameover 
                    }
                }
            }
        }
    }
}



void ehoeungscore()  
{
    int y;
    for (y = 0; y < 20; y++)
    {
        if (c[vogelx][y] == 'H')  //wenn der vogel zwischen einer roere ist 
        {
            score++;
            return;
        }
    }
}

bool gameover()  //checks if bird has hit something
{
    int x, y, f = 0;
    if (vogely > 19) //ist vogel auf dem boden(der tatsachen)
    {
        c[vogelx][19] = 'V';  //zwingt den vogel auf dem boden zu bleiben sonst kann man cheaten 
        c[vogelx][20] = '-';
        f = 1;           //return true
        goto quit;
    }
    else
    {     //ist der vogel gegen ne röhre geflogen
        if (n[vogelx][vogely] > 0 && (c[vogelx][vogely] == 'H' || c[vogelx][vogely] == 'V'))
        {
            c[vogelx][vogely] = 'H';
            c[vogelx - 1][19] = 'V';
            f = 1;
            goto quit;
        }
    }
quit:
    if (f == 1) return true;
    else return false;
}

void endebild() 
{
    system("cls");
     
    cout << "" << endl << endl;
    cout << " ------------------------------------------------------------------------- " << endl;
    cout << "|    *****      *     *       * ******       ****  *       ****** ****    |" << endl;
    cout << "|   *          * *    * *   * * *           *    *  *     * *     *   *   |" << endl;
    cout << "|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |" << endl;
    cout << "|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |" << endl;
    cout << "|    *****  *       * *       * ******       ****      *    ***** *   *   |" << endl;
    cout << " ------------------------------------------------------------------------- " << endl;
    cout << "" << endl << endl;
    cout << "                        Y O U R   S C O R E : " << score << endl << endl;
    cout << "                        H I G H   S C O R E : " << highscore << endl;
    cout << "" << endl << endl;
}

void menubild() 
{
    system("cls");
    cout << "" << endl;
    cout << " --------------------------------------------------------  " << endl;
    cout << "|                                                        | " << endl;
    cout << "|   **** *    **** **** **** *   *    ***  * ***  ***    | " << endl;
    cout << "|   *    *    *  * *  * *  * *   *    *  * * *  * *  *   | " << endl;
    cout << "|   ***  *    **** **** **** *****    ***  * ***  *  *   | " << endl;
    cout << "|   *    *    *  * *    *      *      *  * * *  * *  *   | " << endl;
    cout << "|   *    **** *  * *    *      *      ***  * *  * ***    | " << endl;
    cout << "|                                                  v 1.0 | " << endl;
    cout << " --------------------------------------------------------  " << endl;
    cout << "" << endl << endl;
    cout << "                  High Score:  " << highscore << endl << endl;
    cout << "" << endl << endl;
    cout << "                     M E N U:    " << endl << endl;
    cout << "                  1: Start Game  " << endl << endl;
    cout << "                  2: Hilfe       " << endl << endl;
    cout << "                  3: Credits     " << endl << endl;
    cout << "                  4: Exit        " << endl << endl;
}

void creditsbild()
{
    char sel;
    system("cls");
    while (true)
    {
        cout << "" << endl << endl;
        cout << "               Programierer: Simon Seitz " << endl << endl;
        cout << "               Designer: Simon Seitz " << endl << endl;
        cout << "               Tester: Menschen " << endl << endl;
        cout << "               Version: 1.0 " << endl << endl << endl;
        cout << "Zurueck? [y/n]  ";
        cin >> sel;
        if (sel == 'y') return;
        else system("cls");
    }
}

void hilfe()
{
    char sel;
    system("cls");
    while (true)
    {
        cout << "" << endl << endl;
        cout << "                Drueck ne Taste um hoch zu fliegen            " << endl;
        cout << "            Ziel: Fliege zwischen den Roehren hindurch        " << endl;
        cout << "           Wenn du das schaffst, kriegst du nen Punkt         " << endl;
        cout << "         Versuche so viele zu durchfliegen wie moeglich       " << endl;
        cout << "           Aber triff nicht den Boden oder die Roehren        " << endl << endl;
        cout << "                          Bist du bereit?                     " << endl << endl << endl;
        cout << "Zurueck? [y/n]  ";
        cin >> sel;
        if (sel == 'y') return;
        else system("cls");
    }
}
//wer das liest kann lesen haha


#include <iostream> 
#include <random>
#include <iostream> 
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <fstream> 
#include <iomanip>
using namespace std;

constexpr auto COMPUTER = 1;
constexpr auto MENSCH = 2;
constexpr auto FELDGR = 3;
constexpr auto COMPUTERZEICHEN = 'O';
constexpr auto MENSCHZEICHEN = 'X';
int SIMULATIONEN = 0;

int comeins = 0;
int comzwei = 0;
int unentschieden = 0;
int zuegein = 0;
int tee = 0;
int vuu = 0;

void gebefeldaus(char feldinhatl[][FELDGR]);
void erklaerunganzeigen();
void erstellefeld(char feldinhatl[][FELDGR]);
void werhatgewonnen(int werdran);
bool zeile(char feld[][FELDGR]);
bool spalte(char feld[][FELDGR]);
bool diagonalen(char feld[][FELDGR]);
bool spielende(char feldinhatl[][FELDGR]);
int minimax(char feldinhatl[][FELDGR], int zug, bool botamzug);
int bestMove(char feldinhatl[][FELDGR], int zugnummer);
int bestMovee(char feldinhatl[][FELDGR], int zugnummer);
void gamemain(int wer);
int st(int v);

int main()
{
	int x = 0;
	cout << endl << endl <<endl ;
	cout << "1 Com 1 beginnt" << endl;
	cout << "2 Com 2 beginnt" << endl;
	cout << "3 Randoam" << endl;
	cin >> x;
	cout << "Wieoft";
	cin >> SIMULATIONEN;
	cout << "Zeigen 1ja 2nein";
	cin >> tee;
	cout << "Erster zug zufaulig 1ja 2nein" << endl;
	for (int i = 0; i < SIMULATIONEN; i++)
	{
		st(x);
			}
	cout << "Bot 1 gewonnen rate:" << endl;
	cout << ((double)comeins / SIMULATIONEN)*100<<"%"<< endl;
	cout << "Anzahl:" << endl;
	cout << comeins << endl;
	cout << "Bot 2 gewonnen rate:" << endl;
	cout << ((double)comzwei / SIMULATIONEN) * 100 << "%" << endl;
	cout << "Anzahl:" << endl;
	cout << comzwei << endl;
	cout << "Unentschieden rate" << endl;
	cout << ((double)unentschieden / SIMULATIONEN) * 100 << "%" << endl;
	cout << "Anzahl:" << endl;
	cout << unentschieden << endl;
	cout << "Zuege" << endl;
	cout << ((double)zuegein / SIMULATIONEN) << "Durchschnitlich" << endl;
	cout << zuegein << endl;
	return 0;
}
int st(int v)
{

	if (v == 3) {
		random_device rd;
		default_random_engine eng(rd());
		uniform_int_distribution<int> distr(1, 2);
		v = distr(eng);
	}
	cout << v << "beginnt" << endl;
	gamemain(v);
	return 0;
}
void gebefeldaus(char feldinhatl[][FELDGR])
{
	for (int i = 0; i < FELDGR; i++)
	{
		cout << "\t\t\t ";
		for (int c = 0; c < FELDGR; c++)
		{
			cout << feldinhatl[i][c];
			if (c != 2)cout << " | ";
		}
		if (i != 2)cout << ("\n\t\t\t-----------\n");
		if (i == 2)cout << "\n";

	}
	cout << "\n";
	cout << ("\n------------------------\n\n");
}
void erklaerunganzeigen()
{
	cout << ("\nW\x84\hle die nummer des feldes in welches du setzen m\x94\chtst\n\n");

	cout << ("\t\t\t 1 | 2 | 3 \n");
	cout << ("\t\t\t-----------\n");
	cout << ("\t\t\t 4 | 5 | 6 \n");
	cout << ("\t\t\t-----------\n");
	cout << ("\t\t\t 7 | 8 | 9 \n\n");
}
void erstellefeld(char feldinhatl[][FELDGR])
{
	for (int i = 0; i < FELDGR; i++)
	{
		for (int j = 0; j < FELDGR; j++)
			feldinhatl[i][j] = ' ';
	}
}
void werhatgewonnen(int werdran)
{
	if (werdran == COMPUTER) {
		cout << ("Com 2 hat gewonnen\n");
		comzwei++;
	}
	else {
		cout << ("Com 1 hat gewonnen\n");
		comeins++;
	}
}
bool zeile(char feld[][FELDGR])
{
	for (int i = 0; i < FELDGR; i++)
	{
		if (feld[i][0] == feld[i][1] && feld[i][1] == feld[i][2] && feld[i][0] != ' ')return (true);
	}
	return(false);
}
bool spalte(char feld[][FELDGR])
{
	for (int i = 0; i < FELDGR; i++)
	{
		if (feld[0][i] == feld[1][i] && feld[1][i] == feld[2][i] && feld[0][i] != ' ')return (true);
	}
	return(false);
}
bool diagonalen(char feld[][FELDGR])
{
	if (feld[0][0] == feld[1][1] && feld[1][1] == feld[2][2] && feld[0][0] != ' ')return(true);
	if (feld[0][2] == feld[1][1] && feld[1][1] == feld[2][0] && feld[0][2] != ' ')return(true);
	return(false);


}
bool spielende(char feldinhatl[][FELDGR])
{
	return(zeile(feldinhatl) || spalte(feldinhatl) || diagonalen(feldinhatl));
}
int minimax(char feldinhatl[][FELDGR], int zug, bool botamzug)
{

	int score = 0;
	int bestScore = 0;
	if (spielende(feldinhatl) == true)
	{
		if (botamzug == true)
			return -1;
		if (botamzug == false)
			return +1;
	}
	else
	{
		if (zug < 9)
		{
			if (botamzug == true)
			{
				bestScore = -999;
				for (int i = 0; i < FELDGR; i++)
				{
					for (int j = 0; j < FELDGR; j++)
					{
						if (feldinhatl[i][j] == ' ')
						{
							feldinhatl[i][j] = COMPUTERZEICHEN;
							score = minimax(feldinhatl, zug + 1, false);
							feldinhatl[i][j] = ' ';
							if (score > bestScore)
							{
								bestScore = score;
							}
						}
					}
				}
				return bestScore;
			}
			else
			{
				bestScore = 999;
				for (int i = 0; i < FELDGR; i++)
				{
					for (int j = 0; j < FELDGR; j++)
					{
						if (feldinhatl[i][j] == ' ')
						{
							feldinhatl[i][j] = MENSCHZEICHEN;
							score = minimax(feldinhatl, zug + 1, true);
							feldinhatl[i][j] = ' ';
							if (score < bestScore)
							{
								bestScore = score;
							}
						}
					}
				}
				return bestScore;
			}
		}
		else
		{
			return 0;
		}
	}
}
int bestMove(char feldinhatl[][FELDGR], int zugnummer)
{
	int x = -1, y = -1;
	int score = 0, bestScore = -999;
	for (int i = 0; i < FELDGR; i++)
	{
		for (int j = 0; j < FELDGR; j++)
		{
			if (feldinhatl[i][j] == ' ')
			{
				feldinhatl[i][j] = COMPUTERZEICHEN;
				score = minimax(feldinhatl, zugnummer + 1, true);
				feldinhatl[i][j] = ' ';
				if (score > bestScore)
				{
					bestScore = score;
					x = i;
					y = j;
				}
			}
		}
	}
	return x * FELDGR + y;
}
int bestMovee(char feldinhatl[][FELDGR], int zugnummer)
{
	int x = -1, y = -1;
	int score = 0, bestScore = -999;
	for (int i = 0; i < FELDGR; i++)
	{
		for (int j = 0; j < FELDGR; j++)
		{
			if (feldinhatl[i][j] == ' ')
			{
				feldinhatl[i][j] = MENSCHZEICHEN;
				score = minimax(feldinhatl, zugnummer + 1, false);
				feldinhatl[i][j] = ' ';
				if (score > bestScore)
				{
					bestScore = score;
					x = i;
					y = j;
				}
			}
		}
	}
	return x * FELDGR + y;
}
void gamemain(int wer)
{
	char feldinhatl[FELDGR][FELDGR];
	int zugnummer = 0, x = 0, y = 0;

	erstellefeld(feldinhatl);

	if (vuu == 1) {
		random_device rd;
		default_random_engine eng(rd());
		uniform_int_distribution<int> distr(0, 8);
		int v = distr(eng);
		x = v / FELDGR;
		y = v % FELDGR;


		if (wer == COMPUTER) {
			wer = MENSCH;
			feldinhatl[x][y] = COMPUTERZEICHEN;
		}

		else if (wer == MENSCH) {
			wer = COMPUTER;
			feldinhatl[x][y] = MENSCHZEICHEN;
		}
		if (tee == 1)gebefeldaus(feldinhatl);
		zugnummer = 1;
		zuegein++;
	}
	
	while (spielende(feldinhatl) == false && zugnummer != FELDGR * FELDGR)
	{
		int n;
		if (wer == COMPUTER)
		{
			n = bestMove(feldinhatl, zugnummer);
			x = n / FELDGR;
			y = n % FELDGR;
			feldinhatl[x][y] = COMPUTERZEICHEN;
			cout << ("Com2 hat %c in feld %d eingegeben\n\n", COMPUTERZEICHEN, n + 1);
			if(tee==1)gebefeldaus(feldinhatl);
			//zugnummer++;
			zuegein++;
			wer = MENSCH;
		}

		else if (wer == MENSCH)
		{

			n = bestMovee(feldinhatl, zugnummer);
			x = n / FELDGR;
			y = n % FELDGR;
			if (feldinhatl[x][y] == ' ' && n < 9 && n >= 0)
			{
				feldinhatl[x][y] = MENSCHZEICHEN;
				cout << ("\nCom1 hat %c in feld %d eingegeben\n\n", MENSCHZEICHEN, n + 1);
				if(tee==1)gebefeldaus(feldinhatl);
				//zugnummer++;
				zuegein++;
				wer = COMPUTER;
			}


		}
		zugnummer++;
		
	}

	// unentschieden
	if (spielende(feldinhatl) == false && zugnummer == FELDGR * FELDGR) {
		cout << "Unentschieden\n";
		unentschieden++;
	}
	else
	{

		if (wer == COMPUTER)
			wer = MENSCH;
		else if (wer == MENSCH)
			wer = COMPUTER;

		werhatgewonnen(wer);
	}
}

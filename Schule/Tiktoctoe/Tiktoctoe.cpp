#include <iostream> 
using namespace std;

constexpr auto COMPUTER = 1;
constexpr auto MENSCH = 2;
constexpr auto FELDGR = 3;
constexpr auto COMPUTERZEICHEN = 'O';
constexpr auto MENSCHZEICHEN = 'X';

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
void gamemain(int wer);

int main()
{
	cout << ("\n\t\t\t Tik Tok (Toe)\n");
	cout << ("\n\t\t\t---------------\n\n");
	int auswahl = 0;
	do {

		cout << ("Spiel Starten du als erster: 1") << endl;
		cout << ("Spiel Starten computer als erster: 2") << endl;
		cout << ("Beenden: 3") << endl;
		cin >> auswahl;

		switch (auswahl)
		{
		case 2:
			gamemain(COMPUTER);
			break;
		case 1:
			gamemain(MENSCH);
			break;
		case 3:
			break;
		default:
			cout << ("Geht nicht\n");
		}



	} while (auswahl != 3);
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
	if (werdran == COMPUTER)
		cout << ("Computer hat gewonnen\n");
	else
		cout << ("Du hast gewonnen\n");
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
	erklaerunganzeigen();


	while (spielende(feldinhatl) == false && zugnummer != FELDGR * FELDGR)
	{
		int n;
		if (wer == COMPUTER)
		{
			n = bestMove(feldinhatl, zugnummer);
			x = n / FELDGR;
			y = n % FELDGR;
			feldinhatl[x][y] = COMPUTERZEICHEN;
			cout << ("COMPUTER hat %c in feld %d eingegeben\n\n", COMPUTERZEICHEN, n + 1);
			gebefeldaus(feldinhatl);
			zugnummer++;
			wer = MENSCH;
		}

		else if (wer == MENSCH)
		{
			cout << "Folgendepositionen sind m\x94\glich : ";
			for (int i = 0; i < FELDGR; i++)
				for (int j = 0; j < FELDGR; j++)
					if (feldinhatl[i][j] == ' ')
						cout << (i * 3 + j) + 1;
			cout << "\n\nBitte position eingeben = ";
			cin >> n;
			n--;
			x = n / FELDGR;
			y = n % FELDGR;
			if (feldinhatl[x][y] == ' ' && n < 9 && n >= 0)
			{
				feldinhatl[x][y] = MENSCHZEICHEN;
				cout << ("\nMensch hat %c in feld %d eingegeben\n\n", MENSCHZEICHEN, n + 1);
				gebefeldaus(feldinhatl);
				zugnummer++;
				wer = COMPUTER;
			}
			else if (feldinhatl[x][y] != ' ' && n < 9 && n >= 0)
			{
				cout << "\nPosition ist bereits besetzt bitte wähle eine andere\n\n";
			}
			else if (n < 0 || n>8)
			{
				cout << "NEIN!\n";
			}
		}
	}

	// unentschieden
	if (spielende(feldinhatl) == false && zugnummer == FELDGR * FELDGR)
		cout << "Unentschieden\n";
	else
	{

		if (wer == COMPUTER)
			wer = MENSCH;
		else if (wer == MENSCH)
			wer = COMPUTER;

		werhatgewonnen(wer);
	}
}
/*Battaglia Navale V3*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <windows.h>
#include <fstream>
#include <cstdlib>
using namespace std;

//Costanti
#define VITAMAX 20											//Vita massima
#define DIM 10												//Dimensione Mappa
const char ACQUA = '.';										//Skin dell'acqua
const char NAVI = '@';										//Skin delle navi

//Variabili Globali
bool xyCOM[DIM][DIM];										//Matrice di bool che segna se una casella ? occupato o meno da una nave
string vecUtente;
int posiz;

//Struct
struct profiliPunteggio										//Serve a memorizzare i partecipanti
{
	string nomeU;
	int vitto, sconfitte;
};

//-----------------------------------------------------|Funzioni|-----------------------------------------------------\\
//Animazioni
void loading(int); 											//Animazione di caricamento
void animeCOM(int); 										//Animazione attacco COM
char occhi();												//Gestisce il cambio delgli occhi dell'animazione "animeCOM"

//Posizionamento e controllo navi
void posizNavi(char [][DIM], bool, bool);  					//Posiziona le navi
bool orientamento(string);									//Gestisce la scelta dell'orientamento della navi da parte dell'utente
bool controllaCoord(string, int&, bool);					//Controlla se le coordinate rispettano i requisiti
bool controllaPos(char [][DIM], bool, int, int, int);		//Centro di controllo per la posizione delle coordinate della matrice
bool cenOriz(char [][DIM], int, int, int);					//Controlla i centri della navi orizzontali
bool cenVert(char [][DIM], int, int, int);					//Controlla i centri delle navi verticali
bool attornoOriz(char [][DIM], int, int, int);				//Controlla attorno alle navi orizzontali
bool attornoVert(char [][DIM], int, int, int);				//Controlla attorno alla navi verticali
bool coordAttacCom(int, int);								//Controlla se le coordinate generate sono gi? state utilizzate

//MainGame
bool mainGame(int&);										//Corpo del gioco
bool salvaGame(char [][DIM]);								//Dati alcuni bug, serve a salvare il gioco!
void attaccoPG(char [][DIM], char [][DIM], int&, int&);		//Gestisce l'attacco del PG
void attaccoCOM(char [][DIM], char [][DIM], int&, int&);	//Gestisce l'attacco del COM

//Utenti e Menu
bool controlloRispMenu(string, int&);						//Controlla le risposte del menu
void menu(string&);											//Gestisce il menu
void mostraVecProfili();									//Mostra gli eventuali vecchi utenti e i loro punteggi
string creaProfile(); 										//Permette di creare un nuovo profilo
void classifica();											//Gestisce la classifica
void ordinaClassifica(profiliPunteggio[], int);				//Ordina i valori del file e li mostra
void visualClassifica(profiliPunteggio[], int);				//Visualizza i dati dopo il riordinamento
void aggiornaRisultati(bool);								//Aggiorna i risultati in base al risultato del gioco

//Varie
bool contrRisp(string&);									//Controlla se la risposta rispetta i requisiti
void tutorial();											//Gestisce il tutorial del gioco
void setFontSize(int);										//Modifica la grandezza dei caratteri della console
void continua();											//Alternativa al "sistem("Pause")"
void popolaMat(char [][DIM], bool, int&);					//Popola la mappa con il carattere all'interno di ACQUA
void visualMat(char [][DIM], bool);							//Visualizza la mappa
void popola_xyCOM(); 										//Serve a popolare una matrice bool

int main()
{	
	//Serve a modificare il nome della console
	HWND titoloConsole = GetForegroundWindow();
	SetWindowText(titoloConsole,"Battaglia Navale");
	
	srand(time(0));
	//Definizione Varibili
	string spaziV = "\n\n\n\n\n\n\n\n\n", spaziO = "		   ", risp, nome;
	bool contr, vincitore;
	int turni = 0;
	
	//Mette la grandezza del font a 28
	setFontSize(28);
	
	//Intro
	system("cls");
	cout << spaziV << spaziO <<  "Benvenuto Nella Terza Versione Della Battaglia Navale" << endl;
	cout << spaziO.substr(0,1) << "In questa versione avrai la possibilita' di giocare con navi grandi: 4, 3, 2 e 1 " << endl;
	cout << spaziO + spaziO.substr(0,1) << "   ";
	system("pause");
	menu(nome);

	system("cls");
	loading(8); 
	
	setFontSize(28);
	
	vincitore = mainGame(turni);
	system("cls");
	setFontSize(28);
	
	if(vincitore == true) 
		cout << spaziV << spaziO << "COMPLIMENTI! Sei riuscito a vincere!";
	else
		cout << spaziV << spaziO << "Mi dispiace! Hai perso, il COM ti ha battuto";
		
	cout << endl << spaziO << "Hai impiegato " << turni << " turni per riuscire a completare il gioco";
	cout << endl << endl << spaziO << "Torna a trovarci!";
	
	aggiornaRisultati(vincitore);
	
	cout << spaziV << spaziO;
	system("pause");
	
	return 0;
}

void setFontSize(int FontSize) 
{
    //CONSOLE_FONT_INFOEX info = {0};
    //info.cbSize = sizeof(info);
    //info.dwFontSize.Y = FontSize; 
    // SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

void loading(int load)
{
	system("color 30");
	setFontSize(18);
	
	string posY = "\n\n\n\n\n\n\n\n", posX = "							";
	string spazi = "", acqua = "~~~~~~~~~~";
	char medio = 95, sinitro = 92, alto = 193, bandiera = 30;
	for(int i = 0; i < load; i++)
	{
		cout << posY << posX << spazi << "  " << bandiera << endl;
		cout << posX << spazi << medio << medio << alto << medio << medio << endl;
		cout << posX << spazi << sinitro << medio << medio << medio << "/" << endl;
		
		cout << posX << acqua.append("~")<< endl;
		spazi = spazi.append(" ");
		Sleep(500);
		system("cls");
	}
	setFontSize(0);
	system("color 0F");
}

bool contrRisp(string& risp)
{
	//Rendete tutti i caratteri della risposta maiuscoli
	for(int i = 0; i < risp.size(); i++)
		risp[i] = toupper(risp[i]);
	
	if(risp == "SI")
		return true;
	if(risp == "NO")
		return false;
	//Se nella risposta ? presente una S e la lunghezza ? simile a quella di "SI" allora 
	//viene preso come uno sbaglio
	if(risp.size() == 3 or risp.size() == 2 or risp.size() == 1)
	{
		for(int i = 0; i < risp.size(); i++)
			if(risp[i] == 'S')
			{
				risp = "MISSCLICK";
				break;
			}
	}
		
	return false;
}

void popolaMat(char map[][DIM], bool tutor, int& cont)
{	
	char oriz = 196, vert = 124;	
	for(int i = 0; i < DIM; i++) 
	{
		for(int j = 0; j < DIM; j++)
		{
			map[i][j] = ACQUA;
			//Se si tratta del tutorial allora la matrice viene popolata in maniera diversa
			if(tutor == true)
			{
				if(i == 0 or i == DIM-2)
					map[i][j] = oriz;
				if(j == 0 or j == DIM-2)
					map[i][j] = vert;
			}
		}
	}

	//Serve a posizionare delle navi predefinite sulla matrice
	if(tutor == true)
	{	
		if(cont == 1)
			map[4][7] = NAVI;	
			
		if(cont == 2)
		{
			for(int j = 0; j < 4; j++)
			{
				map[2+j][7] = NAVI;
				map[7][2+j] = NAVI;
			}
		}
		cont++;
	}
}

void visualMat(char map[][DIM], bool tutor)
{
	string spazi = "					";
	int cont = 0;
	
	cout << endl << spazi.substr(0,3) << "     ----------------------------------------" << endl;
	
	for(int i = 0; i < DIM-1; i++) 
	{
		cout << spazi; //Per sistemare l'output al centro dello schermo
		for(int j = 0; j < DIM-1; j++)
		{	
			if(cont == 0) 
			{
				for(int h = 0; h < DIM-1; h++) cout << h << " "; //Serve per la facilitare la scelta all'utente
				cout << endl << spazi;
				cont++;	
			}
			cout << map[i][j] << " ";
		}
		cout << i << " "; //Serve per la facilitare la scelta all'utente
		cout << endl;
	}
	cout << spazi.substr(0,3) << "     ----------------------------------------" << endl;
}

void continua()
{
	string risp, spaziO = "			";
	bool avanti;
	
	while(true)
	{
		cout << endl << endl << spaziO.substr(0,3) << "\t";
		cout << "Se vuoi continuare scrivi 'SI': ";
		getline(cin, risp);
			
		avanti = contrRisp(risp);

		if(risp == "MISSCLICK")
		{
			cout << endl << endl << spaziO << spaziO.substr(0,2) << "Volevi scrivere 'SI' ?";
			getline(cin, risp);
			avanti = contrRisp(risp);
			for(int i = 0; i < risp.size(); i++)
				risp[i] = toupper(risp[i]);
			if(risp == "SI")
				break;
		}
		else
		{
			if(avanti == true)
				break;
			else
			{
				cout << spaziO << spaziO.substr(0,2) << "  Aspettiamo...";
				Sleep(1000);	
			}
		}
	}
}

bool orientamento(string risp)
{
	string vertO = "\n\n\n\n\n\n\n\n\n", orizO = "                  ";
	
	for(int i = 0; i < risp.size(); i++)
		risp[i] = toupper(risp[i]);
	
	if(risp.size() == 0)
	{
		do
		{
			system("cls");
			cout << vertO << orizO << "Cosa volevi scrivere: ";
			getline(cin, risp);
		}while(risp.size() == 0);
	}
	
	if(risp == "V") return false;
	if(risp == "O") return true;
	if(risp.size() > 1)
	{
		for(int i = 0; i < risp.size();i++)
		{
			if(risp[i] == 'V')
			{
				do
				{
					cout << vertO << orizO << "Volevi scrivere 'Verticale' (SI, NO): ";
					getline(cin, risp);
					contrRisp(risp);
					if(risp == "SI") return false;
					if(risp == "NO") return true;
				}while(risp == "MISSCLICK");
			}
			else if(risp[i] == 'O')
			{
				do
				{
					cout << vertO << orizO << "Volevi scrivere 'Orizzontale' (SI, NO): ";
					getline(cin, risp);
					contrRisp(risp);
					if(risp == "SI") return true;
					if(risp == "NO") return false;	
				}while(risp == "MISSCLICK");
			}
		}
	}
}

bool controllaCoord(string x1, int& x, bool tutor)
{
	char appX[x1.length()];   
    //Controlla se x1 ? composto solo da numeri    
    for(char const &appX1 : x1)
        if(isdigit(appX1) == 0) return false;
        
    //Converte la stringa x1 in char
    for(int i = 0; i < sizeof(appX); i++)
        appX[i] = x1[i];
	x = atoi(appX);
	
	if(x == 111) return true;
	
	if(tutor == false) if(x <= -1 or x > DIM) return false;
	else if(tutor == true) if(x < 2 or x > DIM-2) return false;
		
	return true;
}

bool controllaPos(char map[][DIM], bool orienta, int coorI, int coorJ, int tipoN)
{
	bool oriz = false, latiO = false, latiV = false, vert = false;
	
	if(coorI < 0 or coorJ < 0) return false;
	
	if(orienta == true) 
	{
		oriz = cenOriz(map, coorI, coorJ, tipoN);	//Controlla se i/l punti/o sorgente rispettano i requisiti
		if(oriz == true)
			latiO = attornoOriz(map, coorI, coorJ, tipoN);	//Controlla attorno a i/l punti/o sorgente
		else return false;
		
		if(oriz == true && latiO == true)return true;
	}
	else
	{
		vert = cenVert(map, coorI, coorJ, tipoN); //Controlla i punti d'origine
		if(vert == true)
			latiV = attornoVert(map, coorI, coorJ, tipoN); //Controlla i punti attorno
		else return false;
		
		if(vert == true && latiV == true)return true;
	} 
	
	return false;

}

bool cenOriz(char map[][DIM], int coorI, int coorJ, int tipoN)
{
	for(int i = 0; i < tipoN; i++)
	{
		if(coorJ+i >= DIM-1)
			return false;
		if(map[coorI][coorJ+i] != ACQUA) return false;
	}
	
	return true;
}

bool cenVert(char map[][DIM], int coorI, int coorJ, int tipoN)
{
	for(int i = 0; i < tipoN; i++)
	{
		if(coorI+i >= DIM-1)
			return false;
		if(map[coorI+i][coorJ] != ACQUA) return false;
	}
	
	return true;
}

bool attornoOriz(char map[][DIM], int coorI, int coorJ, int tipoN)
{
	char vert = 124, oriz = 196;
	bool ab = false;
	int altoBasso = 0;
	
	if(coorI == 0)		
	{
		for(int i = 0; i < tipoN; i++)
		{
			if(coorJ+i >= DIM-1)
				return false;
			if(map[coorI+1][coorJ+i] == ACQUA)altoBasso++;
			else return false;
		}
	}
	else if(coorI == DIM-1)
	{
		for(int i = 0; i < tipoN; i++)
		{
			if(coorJ+i >= DIM-1)
				return false;
			if(map[coorI-1][coorJ+i] == ACQUA)altoBasso++;
			else return false;
		}
	}
	else
	{
		for(int i = 0; i < tipoN; i++)
		{
			if(coorJ+i >= DIM-1)
				return false;
			if(map[coorI+1][coorJ+i] == ACQUA and map[coorI-1][coorJ+i] == ACQUA)altoBasso++;
			else return false;
		}
	}
	
	if(map[coorI][coorJ-1] == ACQUA or map[coorI][coorJ-1] == vert)  //Controlla se gli estremi sono liberi
		if(map[coorI][coorJ+tipoN] == ACQUA or map[coorI][coorJ-1] == vert) ab = true;	
	if(ab == false)return false;
		
	if(altoBasso == tipoN) return true;
	else return false;
}

bool attornoVert(char map[][DIM], int coorI, int coorJ, int tipoN)
{
	char oriz = 196, vert = 124;
	bool sopSot = false;
	int lati = 0;
	
	if(coorI == 0)
	{
		for(int i = 0; i < tipoN; i++)
		{
			if(coorI+i >= DIM-1)
				return false;
			if(map[coorI+i][coorJ+1] == ACQUA)lati++;
			else return false;
		}
		
		if(map[coorI+tipoN][coorJ] == ACQUA or map[coorI-1][coorJ] == oriz) sopSot = true;  //Controlla se gli estremi sono liberi
		if(sopSot == false)return false;
	}
	else if(coorJ == DIM-1)
	{
		for(int i = 0; i < tipoN; i++)
		{
			if(coorI+i >= DIM-1)
				return false;
			if(map[coorI+i][coorJ-1] == ACQUA)lati++;
			else return false;
		}
		
		if(map[coorI-1][coorJ] == ACQUA or map[coorI-1][coorJ] == oriz)  sopSot = true;  //Controlla se gli estremi sono liberi 
		if(sopSot == false)return false;
	}
	else
	{
		for(int i = 0; i < tipoN; i++)
		{
			if(coorI+i >= DIM-1)
				return false;
			if(map[coorI+i][coorJ+1] == ACQUA and map[coorI+i][coorJ-1] == ACQUA)lati++;
			else return false;
		}
		if(map[coorI-1][coorJ] == ACQUA or map[coorI-1][coorJ] == oriz)  //Controlla se gli estremi sono liberi
			if(map[coorI+tipoN][coorJ] == ACQUA or map[coorI-1][coorJ] == oriz) sopSot = true;
		if(sopSot == false)return false;	
	}
	 
	if(lati == tipoN)return true;
	else return false;
}

void tutorial()
{
	int cont = 0;
	bool tutor = true, games = false, avanti; 
	char map[DIM][DIM];
	string spaziO = "			", spaziV = "\n\n\n\n\n\n\n\n\n", risp;
	
	system("cls");
	
	popolaMat(map, tutor, cont);
	visualMat(map, tutor);
	
	cout << endl << spaziO << "     Questo che vedi e' il campo di battaglia";
	cout << endl << spaziO.substr(0,2) << "Su di esso verranno disposte una serie di navi piu' o meno grandi";
	cout << endl << spaziO.substr(0,2) << " Potrai posizionare 10 navi, 4 da una sola cella, 3 da due celle ";
	cout << endl << spaziO << "  	2 da tre celle e 1 da quattro celle.";
	
	continua();
	system("cls");
	popolaMat(map, tutor, cont);
	visualMat(map, tutor);
	
	cout << endl << spaziO << "\t   Quella che vedi e' una nave!";
	cout << endl << spaziO << "Sia tu che il COM avrete delle navi da abbattere";
	cout << endl << spaziO.substr(0,2) << "   chi abbatte per primo tutte le navi dell'avversario vince! ";
	
	continua();
	system("cls");
	popolaMat(map, tutor, cont);
	visualMat(map, tutor);
	
	cout << endl << spaziO << "Come puoi vedere le navi possono essere posizionate";
	cout << endl << spaziO << "\tSia in orizzontale che in verticale";
	cout << endl << spaziO.substr(0,2) << "Ricordati che una nave non potra' mai essere attaccata ad un'altra";
	
	continua();
	system("cls");
	
	cout << endl << spaziV << spaziO.substr(0,1) << "  Ok, ora faremo una prova per vedere se hai capito come si posizionano le navi!";
	continua();
	system("cls");
	
	popolaMat(map, false, cont);
	cont = 0;
	posizNavi(map, true, false);	
	
	system("cls");
	cout << spaziV << spaziO.substr(0,2) << "  Benissimo, hai finito il tutorial. Ora sei pronto per giocare!";
	continua();
}

void posizNavi(char map[][DIM], bool tutor, bool games)
{
	string spazi = "			", spaziV = "\n\n\n\n\n\n\n\n\n";
	int cont = 0;
	
	if(tutor == false and games == false)
	{
		bool orienta = false, posN;
		int irienta, tipoN = 4, coorI, coorJ, cont = 0, ripet = 1;
		
		do
		{
			
			if(tipoN != 1) //Se tipoNavi ? diverso da 1 allora viene generata la posizione della navi (true = orizzontale / false = verticale)
			{
				irienta = rand()%2;
				if(irienta == 0)orienta = true;
				else orienta = false;
			}
			
			do //Vengono generate le coordinate del punto d'origine della nave
			{
				posN = false;	
				coorI = rand()%DIM-1; //Viene generata la Y
				coorJ = rand()%DIM-1; //Viene generata la X
			    posN = controllaPos(map, orienta, coorI, coorJ, tipoN); //Viene controllato se le coordinate rispettano i requisiti
			}while(posN == false);
			
			
			if(orienta == true)
			{
				for(int i = 0; i < ripet; i++)
					for(int j = 0; j < tipoN; j++) map[coorI][coorJ+j] = NAVI;
						
				if(cont == ripet-1) tipoN--;
				if(cont == ripet)
				{
					cont = 0;
					ripet++;
				}
				cont++;
			}
			else
			{
				for(int i = 0; i < ripet; i++)
					for(int j = 0; j < tipoN; j++) map[coorI+j][coorJ] = NAVI;
							
					if(cont == ripet-1) tipoN--;
					if(cont == ripet)
					{
						cont = 0;
						ripet++;
					}
					cont++;
			}
		//	cout << "COORDINATE: " << endl << "X: " << coorI << endl << "Y: " << coorJ << endl;
		//	visualMat(map, false);
		//	system("pause");
		}while(tipoN != 0);
	}
	else
	{
		int x = 0, y = 0, cont = 0, iter = 3, tipoN = 4, ripet = 1, n, conta = 1, quattro = 0;
		bool posNavi, orienta = false, coor; 
		string x1, y1, risp, outOrient, navi;
		
		if(games == true)
		{
			while(cont < 4) 
			{
				system("cls");
				
				if(ripet == 5) break;
				if(tipoN != 1)
				{
					cout << spaziV << spazi << "Inserisci l'orientamento della nave (V, O): ";
					getline(cin, risp);
					orienta = orientamento(risp);
				}
				
				if(orienta == true) outOrient = "Orizzontale";
				else outOrient = "Verticale";
				coor = false;
						
				do //Si controlla che la coordinata X rispetti tutte le restrizioni
				{
					system("cls");
					visualMat(map, tutor);
					if(tipoN == 1) cout << spazi << "  Inserisci le coordinate per la tua nave: ";
					else cout << spazi.substr(0,2) << "  Inserisci le coordinate per la tua nave ( Orientamento: " << outOrient << " )";
					cout << endl << spazi << spazi.substr(0,1) << "  Prima coordinata ( .. - .. ): ";
					getline(cin, x1);
					coor = controllaCoord(x1, x, false);
					cout << "Coor: " << coor << endl << endl;
				}while(coor != true);
					
				coor = false;
				
				do //Si controlla che la coordinata Y rispetti tutte le restrizioni
				{
					system("cls");
					visualMat(map, tutor);
					cout << spazi.substr(0,2) << "  Inserisci le coordinate per la tua nave ( Orientamento: " << outOrient << " )" << endl;
					cout << spazi << spazi.substr(0,1) << "  Seconda coordinata ( " << x << " - .." << " ): ";
					getline(cin, y1);
					coor = controllaCoord(y1, y, false);
				}while(coor != true );
				
				if(conta == 0) navi = " nave";
				else navi = " navi";
				
				//Vengono controllate le coordinate dell'utente
				posNavi = controllaPos(map, orienta, x, y, tipoN);
					
				if(posNavi == false)
				{
					//Se le coordinate non sono valide viene avvisato l'utente e viene esortato a riprovare
					cout << endl << spazi.substr(0,2) << "    Mi dispiace, ma le coordinate che hai inserito non vanno bene";
					continua();
					continue;
				}
				else
				{
					system("cls");
					cout << spazi << "\t Hai posizionato: " << conta << navi << ", su 10" << endl;
					cout << spazi << "   Coordinate: " << x << " - " << y << "( Orientamento: " << outOrient << " )" << endl;
					
					if(orienta == true)
					{
					//	cout << "CONT: " << cont << endl << "RIPET: " << ripet << endl << "TIPON: " << tipoN;
						for(int i = 0; i < ripet; i++)
							for(int j = 0; j < tipoN; j++) map[x][y+j] = NAVI;
						
						if(cont == ripet-1) tipoN--;
						if(cont == ripet)
						{
							cont = 0;
							ripet++;
						}
						cont++;
						conta++;
					}
					else
					{
					//	cout << "CONT: " << cont << endl << "RIPET: " << ripet << endl << "TIPON: " << tipoN;
						for(int i = 0; i < ripet; i++)
							for(int j = 0; j < tipoN; j++) map[x+j][y] = NAVI;
						
						if(cont == ripet-1) tipoN--;
						if(cont == ripet)
						{
							cont = 0;
							ripet++;
						}
						cont++;
						conta++;
					}
					
					visualMat(map, tutor);
					continua();
					
					if(ripet == 4) 
					{
						if(quattro == 4) break;
						else quattro++;
					}
				}
			}	
		}
		
		else
		{
			for(int i = 1; i <= iter; i++)
			{
				system("cls");
				
				//Viene generato l'orientamento della navi (true = orizzontale / false = verticale)
				if(i % 2 == 0)
					orienta = true;
				else
					orienta = false;
				
				do //Si controlla che la coordinata X rispetti tutte le restrizioni
				{
					system("cls");
					visualMat(map, tutor);
					cout << spazi << "     Inserisci le coordinate per la tua nave";
					cout << endl << spazi << spazi.substr(0,2) << "Prima coordinata: ";
					getline(cin, x1);
					coor = controllaCoord(x1, x, tutor);
				}while(coor != true); 
				
				coor = false;
				
				do //Si controlla che la coordinata Y rispetti tutte le restrizioni
				{
					system("cls");
					visualMat(map, tutor);
					cout << spazi << spazi.substr(0,1) << "  Seconda coordinata ( " << x << " -" << "  ): ";
					getline(cin, y1);
					coor = controllaCoord(y1, y, tutor);
				}while(coor != true);
				
				system("cls");
				cout << spazi << "Coordinate: " << x << " - " << y << endl;				
			
				//Vengono controllate le coordinate dell'utente
				posNavi = controllaPos(map, orienta, x, y, i);
							
				if(posNavi == false)
				{
					//Se le coordinate non sono valide viene avvisato l'utente e viene esortato a riprovare
					cout << spazi << "Mi dispiace, ma le coordinate che hai inserito non vanno bene";
					i--;
				}
				else
				{
					//Si controlla l'orientamento e poi si posizionano le navi in base alle coordinate inserite dall'utente
					if(orienta == true)
					{
						if(i != 1)
							for(int j = 0; j < i; j++) map[x][y+j] = NAVI;
					}
					else
					{
						if(i != 1)
							for(int j = 0; j < i; j++) map[x+j][y] = NAVI;
					}
					
					if(i == 1)
						map[x][y] = NAVI;
				}
				//Si visualizza la mappa aggiornata e si chiede se si vuole continuare
				visualMat(map, tutor);
				continua();
			}
		}
	}
}

void attaccoPG(char mapPG[][DIM], char mapCOM[][DIM], int& vitaPG, int& vitaCOM)
{
	string spaziO = "			", spaziV = "\n\n\n\n\n\n\n\n\n";
	string x1, y1;
	int x, y;
	bool coor = false;
	
	do
	{
		system("cls");
		cout << spaziO + spaziO.substr(0,2) << "Hai affondato " << VITAMAX - vitaCOM << " navi" << endl;
		cout << spaziO + spaziO.substr(0,2) << "A te rimangono " << vitaPG << " navi" << endl;
		cout << spaziO + spaziO.substr(0,1) << "       Questo e' il tuo campo: " << spaziV.substr(0, 2);
		visualMat(mapPG, true);
		cout << spaziO << "Dammi le coordinate della nave che vuoi affondare: " << endl;
		cout << spaziO << "Prima coordinata: ";	
		getline(cin, x1);
		coor = controllaCoord(x1, x, false);
	}while(coor == false);	
	coor = false;
		
	do
	{
		system("cls");
		cout << spaziO + spaziO.substr(0,2) << "Hai affondato " << VITAMAX - vitaCOM << " navi" << endl;
		cout << spaziO + spaziO.substr(0,2) << "A te rimangono " << vitaPG << " navi" << endl;
		cout << spaziO + spaziO.substr(0,1) << "       Questo e' il tuo campo: " << spaziV.substr(0, 2);
		visualMat(mapPG, true);
		cout << spaziO << "Dammi le coordinate della nave che vuoi affondare: " << endl;
		cout << spaziO << "Seconda coordinata ( " << x << " - .. ): ";	
		getline(cin, y1);
		coor = controllaCoord(y1, y, false);
	}while(coor == false);
		
	if(mapCOM[x][y] == NAVI)
	{
		mapCOM[x][y] = ACQUA;
		system("cls");
		cout << spaziV << spaziO << "   Complimenti hai affondato una nave al COM";
		vitaCOM--;
	}
	else
	{
		system("cls");
		cout << spaziV << spaziO << "   Mi dispiace non hai affondato nessuna nave";
	}	

	continua();
}

void attaccoCOM(char mapPG[][DIM], char mapCOM[][DIM], int& vitaPG, int& vitaCOM)
{
	string spaziO = "			", spaziV = "\n\n\n\n\n\n\n\n\n";
	int x, y, anime;
	bool coor = false;
	
	do
	{
		x = rand()%DIM;
		y = rand()%DIM;
		
		coor = coordAttacCom(x, y);
	}while(x < 0 or x > DIM-1 or y < 0 or y > DIM-1 or coor == false);
	
	anime = rand()%4+1;
	animeCOM(anime);
	
	if(mapPG[x][y] == NAVI)
	{
		mapPG[x][y] = ACQUA;
		system("cls");
		cout << spaziV << spaziO << "Mi dispiace il COM ti ha distrutto una nave!!" << endl;
		cout << spaziO << "Ti ha distrutto la nave a coordinate X: " << x << "\tY: " << y;
		vitaPG--;
	}
	else
	{
		system("cls");
		cout << endl << spaziV << spaziO << " Che fortuna! Il COM non e' riuscito ad abbarterti le navi";
	}
}

bool mainGame(int& turni)
{
	string spaziO = "			", spaziV = "\n\n\n\n\n\n\n\n\n", risp;
	char mapCOM[DIM][DIM], mapPG[DIM][DIM];
	bool tutor = true, games = true, controlla;
	int cont, load = 8, vitaPG = VITAMAX, vitaCOM = VITAMAX, provCam = 20;
	turni = 0;
	
	popola_xyCOM();
	
	system("cls");
	setFontSize(28);
	loading(load);
	system("cls");
	setFontSize(28);
	
	cout << spaziV << spaziO << "     Perfetto, cominciamo la nostra partita!";
	continua();
	
	system("cls");
	cout << spaziV << spaziO << "\tOra dovrai posizionare le tue navi!";
	continua();
	
	popolaMat(mapPG, false, cont);
	posizNavi(mapPG, tutor, games); 
		
	popolaMat(mapCOM, false, cont);
	posizNavi(mapCOM, false, false);
	
//	visualMat(mapCOM, false);
//	system("pause");	                  Permettono di vedere la mappa del COM durante le fasi di test
//	system("Cls");
	
	while(true)
	{
		attaccoPG(mapPG, mapCOM, vitaPG, vitaCOM);
		attaccoCOM(mapPG, mapCOM, vitaPG, vitaCOM);
		turni++;
		continua();
		if(turni > provCam)
		{
			bool corner = salvaGame(mapCOM);
			if(corner == true) break;
			else provCam += 10;
		}
		
//		cout << "VitaPG: " << vitaPG << endl << "VitaCOM: " << vitaCOM << endl;
//		system("pause");
		if(vitaPG <= -1)
			if(controlla = salvaGame(mapPG) == false) break;
		else if(vitaCOM <= -1) 
			if(controlla = salvaGame(mapCOM) == false) break;
	}
	
	load -= 2;
	loading(load);
	if(vitaPG <= 0) return false;
	else return true;	
}

void animeCOM(int anime)
{
	string posY = "\n\n", posX = "\t     "; 
	char piatto = 22, lati = 179, sDA = 191, sSA = 218, sDB = 217, sSB = 192, punto, occhioA = 155;
	
	setFontSize(75);
	system("color 2");
	for(int i = 0; i < anime; i++)
	{
		punto = 63;
		system("cls");
		cout << posY << posX << sSA << piatto << piatto << piatto << piatto << piatto  << sDA << "" << punto << endl;
		occhioA = occhi();
		
		cout << posX << lati << " " << occhioA;
		occhioA = occhi();
		cout << " " << occhioA << " " << lati << endl;
		cout << posX << lati << " " << piatto << piatto << piatto << " " << lati << endl; 	
		cout << posX << sSB << 	piatto << piatto << piatto << piatto << piatto  << sDB << endl;
		Sleep(400);

		
		punto = 168;
		system("cls");
		cout << posY << posX << sSA << piatto << piatto << piatto << piatto << piatto  << sDA << "" << punto << endl;
		occhioA = occhi();
		cout << posX << lati << " " << occhioA;
		occhioA = occhi();
		cout << " " << occhioA << " " << lati << endl;
		cout << posX << lati << " " << piatto << '^' << piatto << " " << lati << endl; 	
		cout << posX << sSB << 	piatto << piatto << piatto << piatto << piatto  << sDB << endl;
		Sleep(395);
		system("cls");
	}
	system("cls");
	system("color 0F");
	setFontSize(28);
}

char occhi()
{
	int tipoOc;
	char occhio;
	
	tipoOc = rand()%8;
	
	switch(tipoOc)
	{
		case 0:
			occhio = 207;
			break;
		case 1:
			occhio = 246;
			break;
		case 2:
			occhio = 155;
			break;
		case 3:
			occhio = 11;
			break;
		case 4:
			occhio = 3;
			break;
		case 5:
			occhio = 169;
			break;
		case 6:
			occhio = 4;
			break;
		case 7:
			occhio = 184;
			break;
		default:
			occhio = 207;
	}
	
	return occhio;
}

bool salvaGame(char mapCOM[][DIM])
{
	bool navi = false;
	
	for(int i = 0; i < DIM; i++)
	{
		for(int j = 0; j < DIM; j++)
		{
			if(mapCOM[i][j] == NAVI) 
			{
				navi = true;	
				break;
			}
		}
	}
	
	return navi;
}

bool coordAttacCom(int x, int y)
{
	if(xyCOM[x][y] == false)
	{
		xyCOM[x][y] = true;
		return true;
	}
	else return false;
}

void popola_xyCOM()
{
	for(int i = 0; i < DIM-1; i++)
	{
		for(int j = 0; j < DIM-1; j++)
			xyCOM[i][j] = false;
	}
}

void menu(string& nome)
{
	string spaziO = "			    ", spaziV = "\n\n\n\n\n\n\n", risp, nomeUt;
	int rispS;
	bool cRisp;
	
	system("cls");
	
	do
	{
		cout << spaziV << spaziO << "*-*-*-*-*-*-*-*-*-MENU'-*-*-*-*-*-*-*-*-*-*" << endl;
		cout << spaziO <<           "*                                         *" << endl;
		cout << spaziO <<           "*           Nuovo Utente    (1)           *" << endl;
		cout << spaziO <<           "*           Vecchi Utenti   (2)           *" << endl;
		cout << spaziO <<           "*           Tutorial        (3)           *" << endl;
		cout << spaziO <<           "*           Classifica      (4)           *" << endl;
		cout << spaziO <<           "*           Esci            (5)           *" << endl;
		cout << spaziO <<           "*                                         *" << endl;
		cout << spaziO <<           "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		cout << endl << endl << spaziO << "Cosa vuoi fare: ";
		cin >> risp;
		cRisp = controlloRispMenu(risp,rispS);
		system("cls");
	}while(cRisp == false or rispS > 5);
	
	switch(rispS)
	{
		case 1:
			nome = creaProfile();
			do
			{
				system("cls");
				cout << spaziV << spaziO << "Vuoi fare il tutorial (si, no): ";
				cin >> risp;
				cRisp = contrRisp(risp);
			}while(risp == "MISSCLICK");
			getchar();
			if(cRisp == true) tutorial();
			break;
		case 2:
			mostraVecProfili();
			break;
		case 3:
			getchar();
			tutorial();
			menu(nome);
			break;
		case 4:
			classifica();
			getchar();
			continua();
			menu(nome);
			break;
		case 5:
			cout << spaziV << spaziO << spaziO.substr(0,2) << "Grazie e presto!" << spaziV << spaziV.substr(0, 4);
			Sleep(2000);
			exit(5);
	}
}

bool controlloRispMenu(string risp, int& rispS)
{
	if(risp.size() > 1 or risp.size() == 0) return false;
	
	char appRisp[risp.length()];
	for(char const &appRisp : risp)
        if(isdigit(appRisp) == 0) return false;
        
    for(int i = 0; i < sizeof(appRisp); i++)
        appRisp[i] = risp[i];
	 rispS = atoi(appRisp);
	 
	if(rispS < 0) return false;
	 
	return true;
}

string creaProfile()
{
	string spaziO = "			 ", spaziV = "\n\n\n\n\n\n\n\n", risp;
	profiliPunteggio profi;
	int cont;
	bool cRisp;
	ifstream utenteIn("Profili.txt");
	system("cls");
	
	utenteIn >> cont;
	
	int v[cont], s[cont];
	string ps[cont];
	for(int i = 0; i < cont; i++) utenteIn >> ps[i] >> v[i] >> s[i];
	
	utenteIn.close();
	
	do
	{
		getchar();
		system("cls");
		cout << spaziV << spaziO << "Quale e' il tuo Nickname: ";
		getline(cin, profi.nomeU);
		cout << spaziO << "Sei sicuro di voler scegliere questo nome? ( " << profi.nomeU << " )";
		cin >> risp;
		
		cRisp = contrRisp(risp);
	}while(cRisp == false or risp == "MISSCLICK");
	
	profi.sconfitte = 0;
	profi.vitto = 0;
	
	system("cls");
	ofstream utenteOut("Profili.txt");
	utenteOut << cont+1 << endl;
	
	for(int i = 0; i < cont; i++)
		utenteOut << ps[i] << endl << v[i] << endl << s[i] << endl;
		
	utenteOut << profi.nomeU << endl << profi.vitto << endl << profi.sconfitte << endl;
	utenteOut.close();
	
	vecUtente = profi.nomeU;
	return profi.nomeU;
}

void mostraVecProfili()
{
	getchar();
	int cont = 0, rispS;
	bool cRisp;
	string risp, nome, spaziO = "			    ", spaziV = "\n\n\n\n\n\n\n";
	ifstream utenteIn("Profili.txt");
	system("cls");
	
	utenteIn >> cont;
	if(cont == 0) 
	{
		cout << spaziV << spaziO << "Mi dispiace, non ci sono vecchi giocatori";
		continua();
		menu(nome);
	}
	else
	{
		profiliPunteggio uteC[cont];
	
		do
		{
			system("cls");
			cout << spaziV << spaziO << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
			cout << spaziO <<           "*                                         *" << endl;
			for(int i = 0; i < cont; i++)
			{
				utenteIn >> uteC[i].nomeU;
				utenteIn >> uteC[i].vitto;
				utenteIn >> uteC[i].sconfitte;
				
				cout << spaziO << spaziO.substr(0,2) << i+1 << ") " << uteC[i].nomeU << " V = " << uteC[i].vitto << " S = " << uteC[i].sconfitte << endl;
			}
			
			cout << spaziO <<           "*                                         *" << endl;
			cout << spaziO <<           "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
			cout << spaziO << "Quale utente vuoi scegliere: ";
			getline(cin, risp);
			cRisp = controlloRispMenu(risp, rispS);
		}while(cRisp == false or rispS-1 > cont);
		
		vecUtente = uteC[rispS-1].nomeU;
		posiz = rispS-1;
	}
	
	continua();
}

void classifica()
{
	int cont;
	string spaziO = "			    ", spaziV = "\n\n\n\n\n\n\n";
	ifstream utenteIn("Profili.txt");
	
	utenteIn >> cont;
	if(cont == 0) cout << spaziV << spaziO << "Mi dispaice, non ci sono vecchi giocatori";
	else
	{
		profiliPunteggio uteC[cont];
	
		for(int i = 0; i < cont; i++)
		{
			utenteIn >> uteC[i].nomeU;
			utenteIn >> uteC[i].vitto;
			utenteIn >> uteC[i].sconfitte;
		}
		
		ordinaClassifica(uteC, cont);
		visualClassifica(uteC, cont);	
	}
}

void ordinaClassifica(profiliPunteggio uteC[], int cont)
{
	int app;
	string appS;
	
	for(int i = 0; i < cont-1; i++)
	{
		for(int j = 0; j < cont-1; j++)
		{
			if(uteC[j].vitto < uteC[j+1].vitto)
			{
				appS = uteC[j].nomeU;
				uteC[j].nomeU = uteC[j+1].nomeU;
				uteC[j+1].nomeU = appS;
				
				app = uteC[j].vitto;
				uteC[j].vitto = uteC[j+1].vitto;
				uteC[j+1].vitto = app;
				
				app = uteC[j].sconfitte;
				uteC[j].sconfitte = uteC[j+1].sconfitte;
				uteC[j+1].sconfitte = app;
			}
		}
	}
}

void visualClassifica(profiliPunteggio uteC[], int cont)
{
	string spaziO = "			    ", spaziV = "\n\n\n\n\n\n\n";
	cout << spaziV << spaziO << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
	cout << spaziO <<           "*                                         *" << endl;
	
	for(int i = 0; i < cont; i++)
	{
		cout << spaziO << spaziO.substr(0,2) << uteC[i].nomeU;
		cout << " V: " << uteC[i].vitto ;
		cout << " S: " << uteC[i].sconfitte << endl;
	}
	
	cout << spaziO <<           "*                                         *" << endl;
	cout << spaziO <<           "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
}

void aggiornaRisultati(bool vittorie)
{
	int cont;
	ifstream utenteIn("Profili.txt");
	utenteIn >> cont;
	profiliPunteggio uteC[cont];
	
	for(int i = 0; i < cont; i++)
	{
		utenteIn >> uteC[i].nomeU;
		utenteIn >> uteC[i].vitto;
		utenteIn >> uteC[i].sconfitte;
	}
	
	utenteIn.close();
	ofstream utenteOut("Profili.txt");
	for(int i = 0; i < cont; i++)
	{
		if(uteC[i].nomeU == vecUtente)
		{
			if(vittorie == true) uteC[i].vitto += 1;
			else uteC[i].sconfitte += 1;
			break;
		}
	}
	utenteOut << cont << endl;
	for(int i = 0; i < cont; i++)
		utenteOut << uteC[i].nomeU << endl << uteC[i].vitto << endl << uteC[i].sconfitte << endl;
}


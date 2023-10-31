//gra w GO - Juliusz Radziszewski, Informatyka, 193504//

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
#include <cstdlib>

using namespace std;
#define MENU_LEFT_BORDER 2
#define MENU_TOP_BORDER 2
#define BOARD_TOP_BORDER 4
#define BOARD_LEFT_BORDER 64
#define UP 2
#define DOWN 2
#define LEFT 4
#define RIGHT 4

void printmenu() { //funkcja odpowiada za wypisanie menu z instrukcją
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER);
	cputs("INSTRUKCJA");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 2);
	cputs("strzalki : poruszanie kursorem na planszy;");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 3);
	cputs("q: zamkniecie programu");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 4);
	cputs("n: rozpoczecie nowej gry");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 5);
	cputs("enter: potwierdzenie wyboru i zakonczenie tury gracza");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 6);
	cputs("esc: anulowanie obecnej akcji");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 7);
	cputs("i: polozenie kamienia na planszy");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 8);
	cputs("s: zapis stanu gry");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 9);
	cputs("l: wczytanie stanu gry");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 11);
	cputs("@Copyrights Juliusz Radziszewski 193504");

	for (int i = MENU_TOP_BORDER; i < MENU_TOP_BORDER + 15; i++) {
		gotoxy(MENU_LEFT_BORDER - 1, i);
		cputs("|");
		gotoxy(MENU_LEFT_BORDER + 53, i);
		cputs("|");
	}
	for (int y = MENU_LEFT_BORDER - 1; y < MENU_LEFT_BORDER + 54; y++) {
		gotoxy(y, MENU_TOP_BORDER - 1);
		cputs("_");
		gotoxy(y, MENU_TOP_BORDER + 15);
		cputs("-");
	}
}
int sizeChoosing(int size) { //funkcja odpowiadająca za wybór wielkości planszy
	char sizeChoice = '0';
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 16);
	cputs("Wybierz wielkosc planszy: ");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 17);
	cputs("type 1 if 9x9;  type 2 if 13x13;");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 18);
	cputs("type 3 if 19x19; type 4 if other;");
	if (sizeChoice == '0') {
		while (sizeChoice != '1' && sizeChoice != '2' && sizeChoice != '3' && sizeChoice != '4') { //trzy wartości domyślne, i ostatnia pozwalająca wybrać swoją własną wielkość
			sizeChoice = getch();
			if (sizeChoice == '1') {
				size = 9;
				break;
			}
			if (sizeChoice == '2') {
				size = 13;
				break;
			}
			if (sizeChoice == '3') {
				size = 19;
				break;
			}
			if (sizeChoice == '4') {
				while (size != 0 || size < 44) { //jako, że konsola na pełnym ekranie poprawnie wyświetla plansze o max. wielkości 43, tworzę ograniczenia by nie móc wybrać więcej niż 43
					gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 16);
					cputs("choose the size between 1 and 43:");
					char tab[2];
					for (int z = 0; z < 2; z++) { //użytkownik wpisuje w 2-elementową tablicę charów wielkość planszy
						tab[z] = getch();
						gotoxy(MENU_LEFT_BORDER + 34 + z, MENU_TOP_BORDER + 16);
						putch(tab[z]);
					}
					size = atoi(tab); //string->int
					if (size > 43) {
						gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 19);
						puts("Za duzy rozmiar, max. wielkosc to 43");
					}
					if (size < 44)
						break;
				}
			}
			else { //użytkownik musi początkowo wybrać wartości pomiędzy 1 a 4, inaczej nie może wykonać reszty akcji
				gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 16);
				cputs("enter proper value");
			}
		}
	}
	return size;
}

void printBoard(int size) { //funkcja drukująca tablicę o wybranym rozmiarze size
	int x = BOARD_LEFT_BORDER, y = BOARD_TOP_BORDER;

	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size; j++) {
			gotoxy(x, y);
			cputs(" ---");
			y += 2;
		}
		y = BOARD_TOP_BORDER;
		x += 4;
	}
	y = BOARD_TOP_BORDER;
	x = BOARD_LEFT_BORDER;

	for (int k = 0; k < size; k++) {
		for (int l = 0; l < size - 1; l++) {
			gotoxy(x, y + 1);
			cputs("|");
			y += 2;
		}
		y = BOARD_TOP_BORDER;
		x += 4;
	}
	y = BOARD_TOP_BORDER;
	x = BOARD_LEFT_BORDER;

	for (int m = 0; m < 2 * (size + 1) - 3; m++) {
		gotoxy(x - 2, y);
		putch(0xBA);
		y++;
	}
	y = BOARD_TOP_BORDER;
	x = BOARD_LEFT_BORDER;
	for (int n = 0; n < 2 * (size + 1) - 3; n++) {
		gotoxy(x + (size - 1) * LEFT + 2, y);
		putch(0xBA);
		y++;
	}
	y = BOARD_TOP_BORDER;
	x = BOARD_LEFT_BORDER;
	for (int o = 0; o < 4 * (size - 1) + 3; o++) {
		gotoxy(x - 1, y - 1);
		putch(0xCD);
		x++;
	}
	y = BOARD_TOP_BORDER;
	x = BOARD_LEFT_BORDER;
	for (int o = 0; o < 4 * (size - 1) + 3; o++) {
		gotoxy(x - 1, y + (size - 1) * 2 + 1);
		putch(0xCD);
		x++;
	}
	y = BOARD_TOP_BORDER;
	x = BOARD_LEFT_BORDER;
	gotoxy(BOARD_LEFT_BORDER - 2, BOARD_TOP_BORDER - 1);
	putch(0xC9);
	gotoxy(BOARD_LEFT_BORDER - 2, BOARD_TOP_BORDER + (size - 1) * 2 + 1);
	putch(0xC8);
	gotoxy(BOARD_LEFT_BORDER + (size - 1) * 4 + 2, BOARD_TOP_BORDER + (size - 1) * 2 + 1);
	putch(0xBC);
	gotoxy(BOARD_LEFT_BORDER + (size - 1) * 4 + 2, BOARD_TOP_BORDER - 1);
	putch(0xBB);
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 16);
	cputs("                                             ");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 17);
	cputs("                                             ");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 18);
	cputs("                                             ");

	return;
}

int checkBreath(int boardX, int boardY, char **board, int size, char a) { //funkcja sprawdzająca liczbę zajętych oddechów dla danego kamienia
	int breaths = 0;
	if (boardX<size-1)
	if (board[boardX + 1][boardY] == a) { //jeśli dookoła znajdują się przeciwne kamienie lub granica planszy, to zwiększamy zmienną breaths - max wartość 4
		breaths++;
	}
	if (boardX>0)
	if (board[boardX - 1][boardY] == a) {
		breaths++;
	}
	if (boardY<size-1)
	if (board[boardX][boardY + 1] == a) {
		breaths++;
	}
	if (boardY>0)
	if (board[boardX][boardY - 1] == a) {
		breaths++;
	}
	if (boardX == 0 || boardX == size - 1) {
		breaths++;
	}
	if (boardY == 0 || boardY == size - 1) {
		breaths++;
	}
	return breaths; //zwracamy tą wartość i wykorzystujemy ją w innych funkcjach
}

int Kill_stone(int boardX, int boardY, char **board, int points, int x, int y, int size, char my, char enemy) { //funkcja zbijająca ewentualne kamienie bez pozostałych oddechów po postawieniu przeciwnego kamienia
	if (boardX<size-1)
	if (board[boardX + 1][boardY] == enemy) {
		int breaths = 0;
		breaths = checkBreath(boardX + 1, boardY, board, size, my); //jesli jest na danym polu kamien, to sprawdzamy jego oddechy
		if (breaths == 4) { //jeśli zmienna breaths to 4, to znaczy, ze nie ma oddechow, tym samym zmieniamy wartosc tego pola na neutralne i dodajemy punkty dla gracza zbijącego
			board[boardX + 1][boardY] = ' ';
			gotoxy(x + 4, y);
			putch(' ');
			points++;
		}
	}
	if (boardX>0)
	if (board[boardX - 1][boardY] == enemy) { //niżej analogicznie, sprawdzamy pola w inne strony
		int breaths = 0;
		breaths = checkBreath(boardX - 1, boardY, board, size, my);
		if (breaths == 4) {
			board[boardX - 1][boardY] = ' ';
			gotoxy(x - 4, y);
			putch(' ');
			points++;
		}
	}
	if (boardY<size-1)
	if (board[boardX][boardY + 1] == enemy) {
		int breaths = 0;
		breaths = checkBreath(boardX, boardY + 1, board, size, my);
		if (breaths == 4) {
			board[boardX][boardY + 1] = ' ';
			gotoxy(x, y + 2);
			putch(' ');
			points++;
		}
	}
	if (boardY > 0)
	if (board[boardX][boardY - 1] == enemy) {
		int breaths = 0;
		breaths = checkBreath(boardX, boardY - 1, board, size, my);
		if (breaths == 4) {
			board[boardX][boardY - 1] = ' ';
			gotoxy(x, y - 2);
			putch(' ');
			points++;
		}
	}
	return points; //zwracamy liczbę punktów gracza stawiającego kamień zaktualizowaną o ewentualne zdobyte teraz punkty
}

int checkKill_stone(int boardX, int boardY, char **board, int size, int x, int y, int points, int& tempBlockX, int& tempBlockY, char my, char enemy) { //sprawdzenie, czy mimo braku oddechów zbijamy kamień przeciwnika
	int counter = 0;    //jesli tak, to gra pozwoli nam na taki ruch, chyba ze łamie to zasadę KO
	if (boardX<size-1)
	if (board[boardX + 1][boardY] == enemy) {
		int breaths = 0;
		breaths = checkBreath(boardX + 1, boardY, board, size, my);
		if (breaths == 3) { //jak zmienna przed ruchem wynosi 3, to znaczy ze po postawieniu kamienia wynosilaby 4, czyli dojdzie do zbicia
			board[boardX + 1][boardY] = ' '; //zbijamy kamien na tym polu
			gotoxy(x + 4, y);
			putch(' ');
			points++;
			counter++; //liczymy ilosc zbić
			tempBlockX = boardX + 1; //zapisujemy wspolrzedne, na ktorych nie moze zostac postawiony kamien w nastepnym ruchu
			tempBlockY = boardY;
		}
	}
	if(boardX>0)
	if (board[boardX - 1][boardY] == enemy) {
		int breaths = 0;
		breaths = checkBreath(boardX - 1, boardY, board, size, my);
		if (breaths == 3) {
			board[boardX - 1][boardY] = ' ';
			gotoxy(x - 4, y);
			putch(' ');
			points++;
			counter++;
			tempBlockX = boardX - 1;
			tempBlockY = boardY;
		}
	}
	if(boardY<size-1)
	if (board[boardX][boardY + 1] == enemy) {
		int breaths = 0;
		breaths = checkBreath(boardX, boardY + 1, board, size, my);
		if (breaths == 3) {
			board[boardX][boardY + 1] = ' ';
			gotoxy(x, y + 2);
			putch(' ');
			points++;
			counter++;
			tempBlockX = boardX;
			tempBlockY = boardY + 1;
		}
	}
	if(boardY>0)
	if (board[boardX][boardY - 1] == enemy) {
		int breaths = 0;
		breaths = checkBreath(boardX, boardY - 1, board, size, my);
		if (breaths == 3) {
			board[boardX][boardY - 1] = ' ';
			gotoxy(x, y - 2);
			putch(' ');
			points++;
			counter++;
			tempBlockX = boardX;
			tempBlockY = boardY - 1;
		}
	}
	if (counter != 0) { //jeśli doszło do co najmniej jednego zbicia, to stawiamy kamień
		gotoxy(x, y);
		putch(my);
		board[boardX][boardY] = my;
	}
	return points; //zwracamy liczbe zdobytych punktow
}

char handicapStart(char handicap) {  //dajemy mozliwosc czarnemu graczowi zaczecia z handicapem
	gotoxy(MENU_LEFT_BORDER + 60, 5);
	puts("Graczu czarny, czy chcesz zaczac rozgrywke z handicapem?");
	gotoxy(MENU_LEFT_BORDER + 60, 6);
	puts("TAK - type t, NIE - type n"); //jesli gracz wybierze n, to gra zacznie sie standardowo
	char wybor = getch();				//jesli jednak wybierze t, to licznik tury zostanie zatrzymany, czarny bedzie mogl klasc swoje kamienie
	if (wybor == 't') {					//tym samym oddajac za kazdego 0.5pkt dla gracza bialego
		handicap = 't';					//w kazdym momencie czarny gracz moze przerwac stawianie handicapow i zaczac rozgrywke przyciskiem b
		return handicap;
	}
	else if (wybor == 'n') {
		handicap = 'n';
		return handicap;
	}
	else {
		handicap = '0';
		handicapStart(handicap);
	}
}

void saveGame(int size, char**board, int pointsWhite, int pointsBlack, int tura, int handicapCount, int tempBlockX, int tempBlockY) { //funkcja zapisujaca stan gry z wszystkimi potrzebnymi wartościami
	FILE* plik;

	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 18);
	puts("Wpisz nazwe pliku: ");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 19);
	char name[20];
	char znak;
	int counter = 0;
	for (int i = 0; i < 20; i++) { //wpisujemy nazwę pliku i zatwierdzamy ją enterem
		gotoxy(MENU_LEFT_BORDER + i, MENU_TOP_BORDER + 19);
		znak = getche();
		if (znak == 0x0d) {
			break;
		}
		name[i] = znak;
		counter++;
	}
	name[counter] = '\0';

	plik = fopen(name, "w");

	fprintf(plik, "%3d", size);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fprintf(plik, "%c", board[i][j]);
		}
	}

	fprintf(plik, "%4d", tura); //wszystkie zmienne lądują w pliku o podanej nazwie
	fprintf(plik, "%4d", pointsWhite);
	fprintf(plik, "%4d", pointsBlack);
	fprintf(plik, "%2d", handicapCount);
	fprintf(plik, "%3d", tempBlockX);
	fprintf(plik, "%3d", tempBlockY);


	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 20);
	puts("Pomyslnie zapisano plik!");

	fclose(plik);
}

char** resumeGame(int& size, int& pointsWhite, int& pointsBlack, int& tura, int& handicapCount, int& tempBlockX, int& tempBlockY) { //wczytujemy dane z pliku
	FILE* plik;
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 18);
	puts("Wpisz nazwe pliku: ");
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 19);
	char name[20];
	char znak;
	int counter = 0;
	for (int i = 0; i < 20; i++) { //otwieramy plik o podanej przez nas nazwie, zatwierdzamy enterem
		gotoxy(MENU_LEFT_BORDER + i, MENU_TOP_BORDER + 19);
		znak = getche();
		if (znak == 0x0d) {
			break;
		}
		name[i] = znak;
		counter++;
	}
	name[counter] = '\0';

	plik = fopen(name, "r");
	if (plik == NULL) { //jak nie ma takiego pliku jak podaliśmy, to otrzymujemy stosowny komunikat
		gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 20);
		puts("Nie udalo sie otworzyc pliku!");
		resumeGame(size, pointsWhite, pointsBlack, tura, handicapCount, tempBlockX, tempBlockY);
	}
	else {
		fscanf(plik, "%3d", &size);

		char** board = new char* [size];
		for (int j = 0; j < size; j++) {
			board[j] = new char[size];
			for (int k = 0; k < size; k++) {
				fscanf(plik, "%c", &board[j][k]);
			}
		}

		fscanf(plik, "%4d", &tura); //zczytujemy wszystkie dane do zmiennych
		fscanf(plik, "%4d", &pointsWhite);
		fscanf(plik, "%4d", &pointsBlack);
		fscanf(plik, "%2d", &handicapCount);
		fscanf(plik, "%3d", &tempBlockX);
		fscanf(plik, "%3d", &tempBlockY);

		fclose(plik);
		gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 20); //komunikat o zczytaniu pliku
		puts("                                      ");
		puts("Pomyslnie zczytano plik!");
		return board;
	}
}

void whiteTurn(int boardX, int boardY, char** board, int size, int& tura, int& pointsWhite, int x, int y, int& tempBlockX, int& tempBlockY) {
	if (board[boardX][boardY] == ' ') {
		int breaths = 0;
		breaths = checkBreath(boardX, boardY, board, size, '*'); //szukamy ile oddechów zostało zablokowanych dla danego pola
		if (breaths != 4) { //jeśli jest wolny oddech to stawiamy kamień
			gotoxy(x, y);
			putch('o');
			board[boardX][boardY] = 'o';
			tura++;  //jeśli uda się postawić kamień, to dodajemy turę
			pointsWhite = Kill_stone(boardX, boardY, board, pointsWhite, x, y, size, 'o', '*'); //szukamy, czy zostały zbite jakieś kamienie, jeśli tak, to zostaną dodane punkty
		}
		else if (breaths == 4) { //jeśli w danym miejscu są 4 oddechy zajęte, to sprawdzamy możliwość czy przy owym ruchu zostanie zbity przeciwny kamień
			if (tempBlockX != boardX && tempBlockY != boardY) { //nie pozwalamy na ruch łamiący zasadę KO
				int whiteTemp = pointsWhite;
				pointsWhite = checkKill_stone(boardX, boardY, board, size, x, y, pointsWhite, tempBlockX, tempBlockY, 'o', '*'); //dodajemy punkty jeśli po postawieniu kamienia zostanie zbity przeciwny
				//funkcja zwraca domyślną wartość punktów jeśli liczba zajętych oddechów przeciwnego kamienia
				//jest różna od 3 - tylko w takim przypadku rozpatrujemy możliwość postawienia 'samobójczego'
				if (whiteTemp != pointsWhite) //porównujemy stan punktów przed funkcją i po niej, jeśli liczba punktów pozostała taka sama, to do ruchu nie doszło, tak więc tura nie przeskakuje
					tura++;
			}
		}
	}
}

void blackTurn(int boardX, int boardY, char **board, int size, int& tura, int& pointsBlack, int x, int y, int& tempBlockX, int& tempBlockY, char handicap, float& startingPointsWhite, int& handicapCount) { //analogicznie jak wyżej
	if (board[boardX][boardY] == ' ') {
		int breaths = 0;
		breaths = checkBreath(boardX, boardY, board, size, 'o');
		if (breaths != 4) {
			gotoxy(x, y);
			putch('*');
			board[boardX][boardY] = '*';
			if (handicap == 'n') { //jak handicap jest wyłączony, to tury przeskakują normalnie
				tura++;
			}
			if (handicap == 't') {
				startingPointsWhite = startingPointsWhite + 0.5; //jesli jest tryb handicap, to za kazdy postawiony kamien bialy dostaje 0.5pkt
				handicapCount++;								//rozpatrujemy tylko stawianie kamieni gdy liczba zblokowanych oddechów jest mniejsza od 4, bo maksymalna wartość zblokowanych oddechów na początku rozgrywki wynosi 2
			}
			pointsBlack = Kill_stone(boardX, boardY, board, pointsBlack, x, y, size, '*', 'o');
		}
		else if (breaths == 4) {
			if (tempBlockX != boardX && tempBlockY != boardY) {
				int blackTemp = pointsBlack;
				pointsBlack = checkKill_stone(boardX, boardY, board, size, x, y, pointsBlack, tempBlockX, tempBlockY, '*', 'o');
				if (blackTemp != pointsBlack) {
					tura++;
				}
			}
		}
	}
}

void beginning(char *buffer, int size, int pointsWhite, float startingPointsWhite, int pointsBlack) {
	float lastPointsBlack = pointsBlack + 0.0;
	float lastPointsWhite = pointsWhite + startingPointsWhite;
	gotoxy(MENU_LEFT_BORDER + 60, 3);
	puts("                ");
	gotoxy(MENU_LEFT_BORDER + 60, 4);
	puts("                ");
	gotoxy(MENU_LEFT_BORDER + 60, 5);
	puts("                                                                ");
	gotoxy(MENU_LEFT_BORDER + 60, 6);
	puts("                                             "); //czyścimy nieaktualne komentarze

	printBoard(size); //wywołujemy funkcję drukującą planszę
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 19);
	puts("                                            ");

	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 13); //pokazujemy stan punktowy graczy w tym miejscu, by był też widoczny od razu po wczytaniu rozgrywki z pliku
	snprintf(buffer, 50, "Liczba punktow gracza bialego:  %4.1f", lastPointsWhite);
	puts(buffer);
	gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 14);
	snprintf(buffer, 50, "Liczba punktow gracza czarnego:  %4.1f", lastPointsBlack);
	puts(buffer);

	gotoxy(BOARD_LEFT_BORDER, BOARD_TOP_BORDER);
}

char** starting(int &size, int &pointsWhite, int &pointsBlack, int &tura, int &handicapCount, int &tempBlockX, int &tempBlockY, char &handicap, float &startingPointsWhite, char *buffer) {
	char** board;
	printmenu(); //drukujemy menu
	gotoxy(MENU_LEFT_BORDER + 60, 3);
	puts("l - wczytaj gre");
	gotoxy(MENU_LEFT_BORDER + 60, 4);
	puts("n - nowa gra");
	char choice = 'a';
	while (choice != 'l' && choice != 'n' && choice != 'q') {
		choice = getch();
	} //wybieramy, czy zaczynamy nową grę, czy ją wczytujemy. rozpatrujemy również możliwość całkowitego wyjścia z programu
	if (choice == 'n') {
		size = sizeChoosing(size);
		handicap = handicapStart(handicap);
		if (handicap == 't') {
			gotoxy(MENU_LEFT_BORDER + 12, MENU_TOP_BORDER);
			cputs("type b to end handicap");
		}
	}
	if (choice == 'l') {
		board = resumeGame(size, pointsWhite, pointsBlack, tura, handicapCount, tempBlockX, tempBlockY); //wczytujemy wartości z pliku
		handicap = 'n';
		startingPointsWhite = handicapCount * 0.5;
	}
	else {
		board = new char* [size];
		for (int j = 0; j < size; j++) {
			board[j] = new char[size];
			for (int k = 0; k < size; k++) {
				board[j][k] = ' ';
			}
		}
	}
	if (choice == 'q') {
		gotoxy(1, 60);
		return 0;
	}
	beginning(buffer, size, pointsWhite, startingPointsWhite, pointsBlack);
	return board;
}

int main() {
#ifndef __cplusplus
	Conio2_Init();
#endif
	settitle("Juliusz, Radziszewski, 193504");
	_setcursortype(_NOCURSOR);
	textbackground(BLACK);
	textcolor(15);
	int size = 0;
	int command = 0;
	int tura = 1;
	int x = BOARD_LEFT_BORDER, y = BOARD_TOP_BORDER;
	int boardX;
	int boardY;
	int tempBlockX = -1;
	int tempBlockY = -1;
	char buffer[50];
	char handicap = '0';
	int pointsWhite = 0, pointsBlack = 0;
	float startingPointsWhite = 0;
	int handicapCount = 0;
	char** board;

	board = starting(size, pointsWhite, pointsBlack, tura, handicapCount, tempBlockX, tempBlockY, handicap, startingPointsWhite, buffer);
	
	while (command != 'q') { //dopóki program nie natrafi na komendę kończącą program, będzie wykonywał treść pętli

		boardX = (x - BOARD_LEFT_BORDER) / 4; //zmienne odpowiadające wartości pola na planszy wskazywanego przez kursor - współrzędna X i Y
		boardY = (y - BOARD_TOP_BORDER) / 2;

		for (int i = 0; i < size; i++) { //w planszę drukujemy odpowiadające polom wartości w tablicy
			for (int j = 0; j < size; j++) {
				gotoxy(BOARD_LEFT_BORDER + (i * 4), BOARD_TOP_BORDER + (j * 2));
				putch(board[i][j]);
			}
		}

		gotoxy(x, y);
		textbackground(LIGHTBLUE);
		putch(board[boardX][boardY]); //kursor podświetlający obecnie wskazywane pole

		textbackground(BLACK);
		gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 12);
		snprintf(buffer, 50, "Tura: %d", tura); //licznik tury
		puts(buffer);
		gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 1);
		snprintf(buffer, 25, "wspolrzedne: %d x %d ", (x - BOARD_LEFT_BORDER) / 4, (y - BOARD_TOP_BORDER) / 2); //licznik współrzędnych
		puts(buffer);

		command = getch();
		//X-poziomo  Y-pionowo - kierowanie kursorem
		if (command == 0x48) {     //do góry

			gotoxy(x, y);
			textbackground(0);
			putch(board[boardX][boardY]);

			if (y > BOARD_TOP_BORDER) {
				y -= UP;
			}
		}
		else if (command == 0x50) {  //w dół

			gotoxy(x, y);
			textbackground(0);
			putch(board[boardX][boardY]);

			if (y < BOARD_TOP_BORDER + DOWN * size - 2) {
				y += DOWN;
			}
		}
		else if (command == 0x4b) {  //w lewo

			gotoxy(x, y);
			textbackground(0);
			putch(board[boardX][boardY]);

			if (x > BOARD_LEFT_BORDER) {
				x -= LEFT;
			}
		}
		else if (command == 0x4d) { //w prawo

			gotoxy(x, y);
			textbackground(0);
			putch(board[boardX][boardY]);

			if (x < BOARD_LEFT_BORDER + RIGHT * size - 4) {
				x += RIGHT;
			}
		}
		else if (command == 'n') { //czyścimy output, pozwalamy użytkownikowi zacząć nową grę
			textbackground(0);
			clrscr();
			for (int i = 0; i < size; i++)
				delete[] board[i]; //zwalniamy pamięć z nieużywanej, poprzedniej tablicy dynamicznej
			delete[] board;

			board = starting(size, pointsWhite, pointsBlack, tura, handicapCount, tempBlockX, tempBlockY, handicap, startingPointsWhite, buffer);
		}
		else if (command == 'i') { //stawianie kamieni
			gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 20);
			puts("                               ");
			if (board[boardX][boardY] == ' ') { //możemy stawiać kamień na plansze tylko w puste miejsce
				int continuation;
				continuation = getch();
				if (continuation == 13 || continuation != 27) {//zatwierdzenie enterem//0x0A
					while (continuation != 13 && continuation != 27) { //dzięki temu możemy pomylić się w naciskaniu entera i naciskając go ponownie nie musimy ponownie naciskać i
						continuation = getch();
					}
					textcolor(15);
					if (tura % 2 == 0) { //ruch białych
						whiteTurn(boardX, boardY, board, size, tura, pointsWhite, x, y, tempBlockX, tempBlockY);
					}
					else if (tura % 2 == 1) { //ruch czarnych - analogicznie jak wyżej
						blackTurn(boardX, boardY, board, size, tura, pointsBlack, x, y, tempBlockX, tempBlockY, handicap, startingPointsWhite, handicapCount);
					}
					else if (continuation == 27) { //anulowanie escapem
						continuation = getch();
					}

					if (handicap == 'n') { //czyścimy niepotrzebny napis gdy tryb z handicapem jest wyłączony
						gotoxy(MENU_LEFT_BORDER + 12, MENU_TOP_BORDER);
						cputs("                         ");
					}
				}
			}
			gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 13); //aktualizujemy licznik punktów
			snprintf(buffer, 50, "Liczba punktow gracza bialego:  %4.1f", pointsWhite + startingPointsWhite);
			puts(buffer);
			gotoxy(MENU_LEFT_BORDER, MENU_TOP_BORDER + 14);
			snprintf(buffer, 50, "Liczba punktow gracza czarnego:  %4.1f", pointsBlack + 0.0);
			puts(buffer);
		}
		else if (command == 's') { //wywołanie funkcji do zapisu stanu gry
			saveGame(size,board, pointsWhite, pointsBlack, tura, handicapCount, tempBlockX, tempBlockY);
		}
		else if (command == 'b') { //kończenie handicapu
			handicap = 'n';
			tura++;
		}
	}
	if (command == 'q') { //zakończenie programu
		gotoxy(1, 60);
		return 0;
	}
}
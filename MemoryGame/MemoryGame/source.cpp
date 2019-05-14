#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>


using namespace std;


const int wymiar8 = 8; //rozmiar planszy 8x8
const int wymiar12 = 12; // rozmiar planszy 12x12
const char wartosc_p = ' '; //wartosc wpisana na poczatku w kazde z pol planszy
const char X = 'X'; // znak nieodkrytego pola
const int ascii = 93; //liczba mozliwych roznych znakow drukowanych na planszy(liczone od 33 wlacznie)
const int ascii_poczatek = 33;//od tego znaku zaczynaja sie symbole "drukowane"

void powitanie();
int rozmiar(); //wybor rozmiaru planszy na jakiej chce grac gracz
void tworz_plansze(int wymiary, int* index_znak, char** tablica);
void tryb_gry(int wymiary, char** tablicaX, char** tablica);
void g_vs_g(int wymiary, char** tablicaX, char** tablica);
void g_vs_k(int wymiary, char** tablicaX, char** tablica);
void wypisz_plansze(int wymiary, char** tablicaX, char** tablica);
int spr_liczba(int& liczba); //funkcja sprawdzajaca czy to co podajemy na wejscie jest liczba
int zakres_wsp(int wymiary, int& wsp); //funkcja sprawdzajaca czy podana wspolrzedna miesci sie w zakresie rozmiaru planszy


int main() {

	powitanie();

	int rozmiar_p;
	rozmiar_p = rozmiar();
	//tworzenie planszy o rozmiarze wybranym przez gracza
	char** plansza;
	plansza = new char* [rozmiar_p];
	for (int i = 0; i < rozmiar_p; i++) {
		plansza[i] = new char[rozmiar_p];
	}
	//przypisanie kazdemu polu planszy pustego pola
	for (int i = 0; i < rozmiar_p; i++) {
		for (int j = 0; j < rozmiar_p; j++) {
			plansza[i][j] = wartosc_p;
		}
	}
	//tworzenie planszy o rozmiarze wybranym wczesniej przez gracza, bedzie ona widoczna w konsoli
	int* index_znak = 0;  //tablica przechowujaca znaki z tablicy ASCII zapisane dziesietnie
	tworz_plansze(rozmiar_p, index_znak, plansza);
	char** planszaX;
	planszaX = new char* [rozmiar_p];
	for (int i = 0; i < rozmiar_p; i++) {
		planszaX[i] = new char[rozmiar_p];
	}
	//wypelnienie calej planszy X
	for (int i = 0; i < rozmiar_p; i++) {
		for (int j = 0; j < rozmiar_p; j++) {
			planszaX[i][j] = X;
		}
	}
	//wybor gry "vs gracz" lub "vs komputer"
	tryb_gry(rozmiar_p, planszaX, plansza);

	//zwalnianie pamieci
	for (int i = 0; i < rozmiar_p; i++)
		delete[] plansza[i];
	delete[] plansza;
	for (int i = 0; i < rozmiar_p; i++)
		delete[] planszaX[i];
	delete[] planszaX;
	return 0;
}

void powitanie() {
	cout << " Witaj w grze Memory!" << endl << endl;
	cout << "\n\n Gra polega na wyszukiwaniu takich samych par znakow, \n\n ktore znajduja sie pod zakrytymi polami.";
	cout << "\n\n Kazdy z graczy wprowadza wspolrzedne dwoch pol. \n\n Pola ktore wybral gracz sa odkrywane. Jezeli znaki, ktore sie pod nimi";
	cout << "\n\n znajdujda sa takie same, to graczowi zostaje przyznany punkt \n\n i kontynuuje on rozgrywke.\n\n Jezeli pola odkryte przez gracza nie sa takie same,";
	cout << "\n\n wtedy nastepuje ruch przeciwnika.\n\n Zycze udanej zabawy! \n\n\n\n Wcisnij ENTER aby rozpoczac gre..." << endl;
	getchar();
	system("cls");

}

int rozmiar() {
	int wybor;
	int rozmiar;
	bool warunek = true;
	//menu wyboru rozmiaru planszy
	cout << "Wybierz rozmiar planszy:\n1.8x8\n2.12x12" << endl;
	do {
		cin >> wybor;
		wybor = spr_liczba(wybor);
		switch (wybor) {
		case 1:
			system("cls");
			rozmiar = wymiar8;
			warunek = false;
			break;
		case 2:
			system("cls");
			rozmiar = wymiar12;
			warunek = false;
			break;
		default:
			system("cls");
			cout << "Wybrano nieistniejaca opcje. Prosze wybrac rozmiar ponownie:\n1.8x8\n2.12x12" << endl;
			break;
		}

	} while (warunek);
	return rozmiar;
}

void tworz_plansze(int wymiary, int* index_znak, char** tablica) {
	index_znak = new int[wymiary * wymiary / 2];
	srand(time(NULL));
	int losowanie = wymiary * wymiary / 2;
	//tworzenie tablicy przechowywujacej znaki z tablicy ASCII zapisane dziesietnie
	for (int i = 0; i < losowanie; i++) {
		index_znak[i] = (rand() % ascii) + ascii_poczatek;
	}
	int licznik = 0;
	bool wartosc = true;
	int wsp_11, wsp_12, wsp_21, wsp_22;
	do {
		//losowanie wspolrzednych pierwszego i drugiego pola
		do {
			wsp_11 = (rand() % wymiary);
			wsp_12 = (rand() % wymiary);
			wsp_21 = (rand() % wymiary);
			wsp_22 = (rand() % wymiary);
		} while (wsp_11 == wsp_21 && wsp_12 == wsp_22);
		//przypisanie tego samego losowego znaku z wylosowanych znakow ASCII do dwoch pol, jesli oba sa niewypelnione zadnnym znakiem
		if ((tablica[wsp_11][wsp_12] == wartosc_p) && (tablica[wsp_21][wsp_22] == wartosc_p)) {
			int index = rand() % (losowanie);
			int znak = index_znak[index];
			tablica[wsp_11][wsp_12] = (char)znak;
			tablica[wsp_21][wsp_22] = (char)znak;
			licznik++;
		}
		//warunek wyjscia z petli- zadne z pol nie zostalo puste
		for (int i = 0; i < wymiary; i++) {
			for (int j = 0; j < wymiary; j++) {
				if ((licznik == losowanie) && (tablica[i][j] != wartosc_p)) {
					wartosc = false;
				}
			}
		}
	} while (wartosc);
	//zwalnianie pamieci
	delete[] index_znak;
}

void tryb_gry(int wymiary, char** tablicaX, char** tablica) {
	int wybor_t;
	bool warunek = true;
	//wybor trybu gry - "vs gracz" lub "vs komputer"
	cout << "W jakim trybie chcesz grac?\n1.Gracz vs Gracz\n2.Gracz vs Komputer" << endl;
	do {
		cin >> wybor_t;
		wybor_t = spr_liczba(wybor_t);
		switch (wybor_t) {
		case 1:
			system("cls");
			g_vs_g(wymiary, tablicaX, tablica);
			warunek = false;
			break;
		case 2:
			system("cls");
			g_vs_k(wymiary, tablicaX, tablica);
			warunek = false;
			break;
		default:
			system("cls");
			cout << "Wybrano nieistniejaca opcje. Prosze wybrac tryb ponownie:\n1.Gracz vs Gracz\n2.Gracz vs Komputer" << endl;
			break;
		}
	} while (warunek);
	getchar();
}

void g_vs_g(int wymiary, char** tablicaX, char** tablica) {
	//mechanizm dzialania trybu gracz vs gracz
	int x1, y1, x2, y2;
	int ruch = 1;
	int pkt1 = 0, pkt2 = 0;
	while (pkt1 + pkt2 < wymiary * wymiary / 2) {
		do {
			//wypisanie planszy
			system("cls");
			wypisz_plansze(wymiary, tablicaX, tablica);

			cout << "Gracz " << ruch << ". Podaj wiersz pierwszego pola [1," << wymiary << "]" << endl;
			cin >> x1;
			x1 = spr_liczba(x1);
			x1 = zakres_wsp(wymiary, x1);

			cout << "Gracz " << ruch << ". Podaj kolumne pierwszego pola [1," << wymiary << "]" << endl;
			cin >> y1;
			y1 = spr_liczba(y1);
			y1 = zakres_wsp(wymiary, y1);

			//sprawdzenie czy pole nie jest juz puste-tzn ze przyznany juz zostal za nie punkt
			while (tablicaX[x1 - 1][y1 - 1] == ' ') {
				cout << "To pole zostalo juz dopasowane z innym. Wybierz wiersz innego pola [1," << wymiary << "]" << endl;
				cin >> x1;
				x1 = spr_liczba(x1);
				x1 = zakres_wsp(wymiary, x1);

				cout << "Gracz " << ruch << ". Podaj kolumne innego pola [1," << wymiary << "]" << endl;
				cin >> y1;
				y1 = spr_liczba(y1);
				y1 = zakres_wsp(wymiary, y1);
			}

			//wyswietlenie znaku jaki kryje sie pod wspolrzednymi wybranymi przez gracza
			tablicaX[x1 - 1][y1 - 1] = tablica[x1 - 1][y1 - 1];
			system("cls");
			wypisz_plansze(wymiary, tablicaX, tablica);

			cout << "Gracz " << ruch << ". Podaj wiersz drugiego pola [1," << wymiary << "]" << endl;
			cin >> x2;
			x2 = spr_liczba(x2);
			x2 = zakres_wsp(wymiary, x2);

			cout << "Gracz " << ruch << ". Podaj kolumne drugiego pola [1," << wymiary << "]" << endl;
			cin >> y2;
			y2 = spr_liczba(y2);
			y2 = zakres_wsp(wymiary, y2);

			//sprawdzenie czy wspolrzedne pierwszego pola nie sa takie same jak wspolrzedne drugiego
			while (x1 == x2 && y1 == y2) {

				cout << "Nie mozesz wybrac tego samego pola. \nWybierz wiersz drugiego pola ponownie [1," << wymiary << "]" << endl;
				cin >> x2;
				x2 = spr_liczba(x2);
				x2 = zakres_wsp(wymiary, x2);

				cout << "Gracz " << ruch << ". Podaj kolumne drugiego pola ponownie [1," << wymiary << "]" << endl;
				cin >> y2;
				y2 = spr_liczba(y2);
				y2 = zakres_wsp(wymiary, y2);

			};

			//sprawdzenie czy pole nie jest juz puste-tzn ze przyznany juz zostal za nie punkt
			while (tablicaX[x2 - 1][y2 - 1] == ' ') {

				cout << "To pole zostalo juz dopasowane z innym. Wybierz wiersz drugiego pola ponownie [1," << wymiary << "]" << endl;
				cin >> x2;
				x2 = spr_liczba(x2);
				x2 = zakres_wsp(wymiary, x2);

				cout << "Gracz " << ruch << ". Podaj kolumne drugiego pola ponownie [1," << wymiary << "]" << endl;
				cin >> y2;
				y2 = spr_liczba(y2);
				y2 = zakres_wsp(wymiary, y2);

				//sprawdzenie czy wspolrzedne pierwszego pola nie sa takie same jak wspolrzedne drugiego
				while (x1 == x2 && y1 == y2) {
					cout << "Nie mozesz wybrac tego samego pola. \nWybierz wiersz drugiego pola ponownie [1," << wymiary << "]" << endl;
					cin >> x2;
					x2 = spr_liczba(x2);
					x2 = zakres_wsp(wymiary, x2);

					cout << "Gracz " << ruch << ". Podaj kolumne drugiego pola ponownie [1," << wymiary << "]" << endl;
					cin >> y2;
					y2 = spr_liczba(y2);
					y2 = zakres_wsp(wymiary, y2);
				};
			}

			//wyswietlenie znaku jaki kryje sie pod wspolrzednymi wybranymi przez gracza
			tablicaX[x2 - 1][y2 - 1] = tablica[x2 - 1][y2 - 1];
			system("cls");
			wypisz_plansze(wymiary, tablicaX, tablica);
			//system przyznawania punktow
			if (tablicaX[x1 - 1][y1 - 1] == tablicaX[x2 - 1][y2 - 1]) {
				tablicaX[x1 - 1][y1 - 1] = ' ';
				tablicaX[x2 - 1][y2 - 1] = ' ';
				if (ruch == 1) {
					pkt1++;
					if (pkt1 + pkt2 < wymiary * wymiary / 2) {
						cout << "+punkt dla gracza nr 1! Aktualna liczba punktow to " << pkt1 << endl << endl;
						cout << "Gracz pierwszy kontynuuje rozgrywke." << endl;
						system("ping 127.0.0.1 -n 5 > nul");
					}
				}
				else {
					pkt2++;
					if (pkt1 + pkt2 < wymiary * wymiary / 2) {
						cout << "+punkt dla gracza nr 2! Aktualna liczba punktow to " << pkt2 << endl << endl;
						cout << "Gracz drugi kontynuuje rozgrywke." << endl;
						system("ping 127.0.0.1 -n 5 > nul");
					}
				}

			}
			else {
				tablicaX[x1 - 1][y1 - 1] = X;
				tablicaX[x2 - 1][y2 - 1] = X;
				cout << "Pola sa rozne, brak punktu!" << endl << endl;
				if (ruch == 1)cout << "Ruch gracza 2";
				if (ruch == 2) cout << "Ruch gracza 1";
				system("ping 127.0.0.1 -n 5 > nul");
			}
		} while (tablica[x1 - 1][y1 - 1] == tablica[x2 - 1][y2 - 1] && pkt1 + pkt2 < wymiary * wymiary / 2);
		if (ruch == 1) ruch = 2;
		else ruch = 1;
	}
	//wyswietlnie wynikow na koniec gry
	if (pkt1 > pkt2) cout << "Wygrywa gracz 1 zdobywajac " << pkt1 << " punktow. Gracz 2 zdobyl " << pkt2 << " punktow.";
	if (pkt1 < pkt2) cout << "Wygrywa gracz 2 zdobywajac " << pkt2 << " punktow. Gracz 1 zdobyl " << pkt1 << " punktow.";
	if (pkt1 == pkt2) cout << "Remis!";
	getchar();
}

void g_vs_k(int wymiary, char** tablicaX, char** tablica) {
	//mechanizm dzialania trybu gracz vs komputer
	int x1, y1, x2, y2;
	int ruch = 0; //1-ruch gracza 2-ruch komputera
	int pkt1 = 0, pkt2 = 0;
	int poczatek = 0;
	bool warunek = true;
	//menu wyboru kto rozpoczyna rozgrywke
	cout << "Wybierz kto ma rozpoczac:\n1.Gracz\n2.Komputer" << endl;
	do {
		cin >> poczatek;
		poczatek = spr_liczba(poczatek);
		system("cls");
		switch (poczatek) {
		case 1:
			ruch = 1;
			warunek = false;
			break;
		case 2:
			ruch = 2;
			warunek = false;
			break;
		default:
			cout << "Wybrano nieisniejaca opcje. Prosze wybrac kto zaczyna ponownie:\n1.Gracz\n2.Komputer" << endl;
			break;
		}
	} while (warunek);
	while (pkt1 + pkt2 < wymiary * wymiary / 2) {
		do {
			srand(time(NULL));
			system("cls");
			wypisz_plansze(wymiary, tablicaX, tablica);

			if (ruch == 2) {
				cout << "Ruch komputera." << endl;
				x1 = (rand() % wymiary) + 1;
			}
			else {
				cout << "Gracz. Podaj wiersz pierwszego pola [1," << wymiary << "]" << endl;
				cin >> x1;
				x1 = spr_liczba(x1);
				x1 = zakres_wsp(wymiary, x1);
			}

			if (ruch == 2) {
				y1 = (rand() % wymiary) + 1;
			}
			else {
				cout << "Gracz. Podaj kolumne pierwszego pola [1," << wymiary << "]" << endl;
				cin >> y1;
				y1 = spr_liczba(y1);
				y1 = zakres_wsp(wymiary, y1);
			}

			/*sprawdzenie czy pole nie jest juz puste-tzn ze przyznany juz zostal za nie punkt, dla gracza analogiczne sprawdzanie
			czy nie wybral kolejnego pola spoza planszy oraz czy ponownie wybrane pole nie jest juz puste*/
			while (tablicaX[x1 - 1][y1 - 1] == ' ') {
				if (ruch == 2) {
					x1 = (rand() % wymiary) + 1;
					y1 = (rand() % wymiary) + 1;
				}
				else {
					cout << "To pole zostalo juz dopasowane z innym. Wybierz wiersz innego pola [1," << wymiary << "]" << endl;
					cin >> x1;
					x1 = spr_liczba(x1);
					x1 = zakres_wsp(wymiary, x1);
					cout << "To pole zostalo juz dopasowane z innym. Wybierz kolumne innego pola [1," << wymiary << "]" << endl;
					cin >> y1;
					y1 = spr_liczba(y1);
					y1 = zakres_wsp(wymiary, y1);
				}
			};
			//wyswietlenie znaku jaki kryje sie pod wspolrzednymi wybranymi przez gracza
			tablicaX[x1 - 1][y1 - 1] = tablica[x1 - 1][y1 - 1];
			if (ruch == 2)	system("ping 127.0.0.1 -n 5 > nul");
			system("cls");
			wypisz_plansze(wymiary, tablicaX, tablica);
			if (ruch == 2) cout << "Komputer wybral wspolrzedne pierwszego pola [" << x1 << "," << y1 << "]." << endl << endl;
			if (ruch == 2) {
				x2 = (rand() % wymiary) + 1;
			}
			else {
				cout << "Gracz. Podaj wiersz drugiego pola [1," << wymiary << "]" << endl;
				cin >> x2;
				x2 = spr_liczba(x2);
				x2 = zakres_wsp(wymiary, x2);
			}

			if (ruch == 2) {
				y2 = (rand() % wymiary) + 1;
			}
			else {
				cout << "Gracz. Podaj kolumne drugiego pola [1," << wymiary << "]" << endl;
				cin >> y2;
				y2 = spr_liczba(y2);
				y2 = zakres_wsp(wymiary, y2);
			}

			//sprawdzenie czy wspolrzedne pierwszego pola nie sa takie same jak wspolrzedne drugiego
			while (x1 == x2 && y1 == y2) {
				if (ruch == 1) {
					cout << "Nie mozesz wybrac tego samego pola. \nPodaj wiersz drugiego pola ponownie [1," << wymiary << "]" << endl;
					cin >> x2;
					x2 = spr_liczba(x2);
					x2 = zakres_wsp(wymiary, x2);
					cout << "\nPodaj kolumne drugiego pola ponownie [1," << wymiary << "]" << endl;
					cin >> y2;
					y2 = spr_liczba(y2);
					y2 = zakres_wsp(wymiary, y2);
				}
				if (ruch == 2) {
					x2 = (rand() % wymiary) + 1;
					y2 = (rand() % wymiary) + 1;
				}
				while (tablicaX[x2 - 1][y2 - 1] == ' ') {
					if (ruch == 2) {
						x2 = (rand() % wymiary) + 1;
						y2 = (rand() % wymiary) + 1;
					}
					else {
						cout << "To pole zostalo juz dopasowane z innym. Wybierz wiersz innego pola [1," << wymiary << "]" << endl;
						cin >> x2;
						x2 = spr_liczba(x2);
						x2 = zakres_wsp(wymiary, x2);
						cout << "To pole zostalo juz dopasowane z innym. Wybierz kolumne innego pola [1," << wymiary << "]" << endl;
						cin >> y2;
						y2 = spr_liczba(y2);
						y2 = zakres_wsp(wymiary, y2);
					}
				}
			}


			/*sprawdzenie czy pole nie jest juz puste-tzn ze przyznany juz zostal za nie punkt, dla gracza analogiczne sprawdzanie
			czy nie wybral kolejnego pola spoza planszy oraz czy ponownie wybrane pole nie jest juz puste*/
			while (tablicaX[x2 - 1][y2 - 1] == ' ') {
				if (ruch == 2) {
					x2 = (rand() % wymiary) + 1;
					y2 = (rand() % wymiary) + 1;
				}
				else {
					cout << "To pole zostalo juz dopasowane z innym. Wybierz wiersz innego pola [1," << wymiary << "]" << endl;
					cin >> x2;
					x2 = spr_liczba(x2);
					x2 = zakres_wsp(wymiary, x2);
					cout << "To pole zostalo juz dopasowane z innym. Wybierz kolumne innego pola [1," << wymiary << "]" << endl;
					cin >> y2;
					y2 = spr_liczba(y2);
					y2 = zakres_wsp(wymiary, y2);
				}
				while (x1 == x2 && y1 == y2) {
					if (ruch == 1) {
						cout << "Nie mozesz wybrac tego samego pola. \nPodaj wiersz drugiego pola ponownie [1," << wymiary << "]" << endl;
						cin >> x2;
						x2 = spr_liczba(x2);
						x2 = zakres_wsp(wymiary, x2);
						cout << "\nPodaj kolumne drugiego pola ponownie [1," << wymiary << "]" << endl;
						cin >> y2;
						y2 = spr_liczba(y2);
						y2 = zakres_wsp(wymiary, y2);
					}
					if (ruch == 2) {
						x2 = (rand() % wymiary) + 1;
						y2 = (rand() % wymiary) + 1;
					}
				}
			};

			//wyswietlenie znaku jaki kryje sie pod wspolrzednymi wybranymi przez gracza
			tablicaX[x2 - 1][y2 - 1] = tablica[x2 - 1][y2 - 1];
			if (ruch == 2)	system("ping 127.0.0.1 -n 5 > nul");
			system("cls");
			wypisz_plansze(wymiary, tablicaX, tablica);
			if (ruch == 2) cout << "Komputer wybral wspolrzedne drugiego pola [" << x2 << "," << y2 << "]." << endl << endl;
			//system przyznawania punktow
			if (tablicaX[x1 - 1][y1 - 1] == tablicaX[x2 - 1][y2 - 1]) {
				tablicaX[x1 - 1][y1 - 1] = ' ';
				tablicaX[x2 - 1][y2 - 1] = ' ';
				if (ruch == 1) {
					pkt1++;
					if (pkt1 + pkt2 < wymiary * wymiary / 2) {
						cout << "+punkt dla gracza! Aktualna liczba punktow to " << pkt1 << endl << endl;
						cout << "Gracz kontynuuje rozgrywke." << endl;
						system("ping 127.0.0.1 -n 5 > nul");
					}
				}
				else {
					pkt2++;
					if (pkt1 + pkt2 < wymiary * wymiary / 2) {
						cout << "+punkt dla komputera! Aktualna liczba punktow to " << pkt2 << endl << endl;
						cout << "Komputer kontynuuje rozgrywke." << endl;
						system("ping 127.0.0.1 -n 5 > nul");
					}
				}

			}
			else {
				tablicaX[x1 - 1][y1 - 1] = X;
				tablicaX[x2 - 1][y2 - 1] = X;
				cout << "Pola sa rozne, brak punktu!" << endl << endl;
				if (ruch == 2) cout << "Ruch gracza";
				system("ping 127.0.0.1 -n 5 > nul");
			}
		} while (tablica[x1 - 1][y1 - 1] == tablica[x2 - 1][y2 - 1] && pkt1 + pkt2 < wymiary * wymiary / 2);
		if (ruch == 1) ruch = 2;
		else ruch = 1;

	}
	//wyswietlanie wynikow na koniec gry
	if (pkt1 > pkt2) cout << "Wygrywa gracz  zdobywajac " << pkt1 << " punktow. Komputer zdobyl " << pkt2 << " punktow.";
	if (pkt1 < pkt2) cout << "Wygrywa komputer zdobywajac " << pkt2 << " punktow. Gracz zdobyl " << pkt1 << " punktow.";
	if (pkt1 == pkt2) cout << "Remis!";
	getchar();
}

void wypisz_plansze(int wymiary, char** tablicaX, char** tablica) {
	//numerowanie poziomo
	int  *numery_poziom = new int[wymiary];
	for (int j = 0; j < wymiary; j++) {
		numery_poziom[j] = j + 1;
	}
	cout << "  ";
	for (int j = 0; j < wymiary; j++) {
		cout << setw(3) << numery_poziom[j] << "  ";
	}
	cout << endl << endl;
	for (int i = 0; i < wymiary; i++) {
		//numerowanie pionowo
		cout << setw(2) << i + 1;
		//wypisanie talbicy
		for (int j = 0; j < wymiary; j++) {
			cout << setw(3) << tablicaX[i][j] << " |";
		}
		cout << endl;
		cout << "    ";
		for (int i = 0; i < 4.75 * wymiary; i++) {
			if (i % 2 == 0) cout << "_";
			if (i % 2 == 1) cout << " ";
		}
		cout << endl << endl;
	}
}

int spr_liczba(int& liczba) {
	bool spr = cin.good();
	if (spr == false) {
		do {
			cout << "Podaj liczbe, a nie znak: " << endl;
			cin.clear();
			cin.sync();
			cin >> liczba;
			spr = cin.good();

		} while (spr == false);
	}
	return liczba;
}

int zakres_wsp(int wymiary, int& wsp) {
	//sprawdzenie czy podana wspolrzedna pola jest w zakresie wczesniej wybranej planszy
	while (wsp > wymiary || wsp < 1) {
		cout << "Podano wspolrzedna spoza zakresu. \nWprowadz ja jeszcze raz[1," << wymiary << "]." << endl;
		cin >> wsp;
		wsp = spr_liczba(wsp);
	};
	return wsp;
}

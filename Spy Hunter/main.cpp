#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#define ENEMY_SPEED 0.9
#define ALLY_SPEED 0.75
#define BISHOP_SPEED 1.1
#define NUMBER_OF_VEHICLES 6
#define SAFE_TIME 15
#define SIDE_WIDTH 200
#define NUMBER_OF_BULLETS 3
#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720
#define SAVENAME_LENGTH 26
#define NUMBER_OF_SLOTS 5
#define DESTROY_POINTS 50

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>  

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

struct Vehicle { //struktura zawieraj¹ca informacje o pojeŸdzie przeciwnika/neutralnym
	double x;
	double y;
	int enemy;
	int destroyed;
	char direction;
};

struct Bullet { //struktura zawieraj¹ca informacje o pocisku
	double x;
	double y;
	int fired;
};
//funkcje z template
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};
//koniec funkcji z template
void setMenu(int zielony, int niebieski, SDL_Surface* screen, char text[128], SDL_Surface* charset, double worldTime, double fps, int points, double speed, int paused, double worldTimeSave, int pointsStoppedAt, int pointsBanned, int ended, int gameplayPoints) {
	DrawRectangle(screen, 4, 4, (SCREEN_WIDTH / 5), 20, zielony, niebieski); //funkcja odpowiadaj¹ca za wyrysowanie menu, wyœwietlanie czasu, punktów, fps'ów i prêdkoœci
	sprintf(text, "Juliusz Radziszewski - 193504");
	DrawString(screen, 6, 10, text, charset);
	DrawRectangle(screen, SCREEN_WIDTH - 265, 4, (SCREEN_WIDTH / 6) + 40, 58, zielony, niebieski);
	if (paused == 0)
		sprintf(text, "Czas trwania rozgrywki: %.1f s", worldTime);
	if (paused == 1||ended==1) //gdy czas jest zapauzowany, to wyœwietlamy zapisan¹ wartoœæ w momencie zapauzowania, po odpauzowaniu g³ówna wartoœæ jest nadpisywana przez ni¹
		sprintf(text, "Czas trwania rozgrywki: %.1f s", worldTimeSave);
	DrawString(screen, SCREEN_WIDTH - 260, 10, text, charset);
	sprintf(text, "%.0lf klatek/s", fps);
	DrawString(screen, SCREEN_WIDTH - 260, 22, text, charset);
	if (pointsBanned==0)
	sprintf(text, "Liczba punktow: %6d", points);
	if (pointsBanned==1) //gdy gracz ma na³o¿on¹ blokadê punktów, to pokazujemy wartoœæ punktów w momencie na³o¿enia blokady, po jej up³ywie g³ówna wartoœæ jest nadpisywana przez t¹
		sprintf(text, "Liczba punktow: %6d", pointsStoppedAt);
	DrawString(screen, SCREEN_WIDTH - 260, 34, text, charset);
	sprintf(text, "Predkosc: %1lf", speed);
	DrawString(screen, SCREEN_WIDTH - 260, 46, text, charset);
	DrawRectangle(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 40), SCREEN_HEIGHT - 155, SCREEN_WIDTH / 5 - 50, 150, zielony, niebieski);
	sprintf(text, "Zaimplementowane punkty:");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 150, text, charset);
	sprintf(text, "a b c d e f g i j k l m");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 135, text, charset);
	sprintf(text, "esc - wyscie z gry");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 120, text, charset);
	sprintf(text, "n - nowa gra");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 105, text, charset);
	sprintf(text, "strzalki - poruszanie sie");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 90, text, charset);
	sprintf(text, "s - zapis stanu gry");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 75, text, charset);
	sprintf(text, "l - wczytanie stanu gry");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 60, text, charset);
	sprintf(text, "p - pauza/kontynuuj");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 45, text, charset);
	sprintf(text, "f - zakonczenie gry");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 30, text, charset);
	sprintf(text, "space - strzelanie");
	DrawString(screen, SCREEN_WIDTH - ((SCREEN_WIDTH / 5) - 45), SCREEN_HEIGHT - 15, text, charset);
};

void saveCounter(int counter) { //funkcja odpowiadaj¹ca za zapis w pliku informacji o iloœci save'ów
	FILE* plik2;
	plik2 = fopen("saveCounter", "w+");
	fprintf(plik2, "%2d", counter);
	fclose(plik2);
}

int loadCounter() { //funkcja odczytuj¹ca informacjê z pliku o iloœci zapisanych save'ów
	int counter=0;
	FILE* plik2;
	plik2 = fopen("saveCounter", "r+");
	if (plik2 != NULL) {
		fscanf(plik2, "%2d", &counter);
		fclose(plik2);
	}
	return counter;
}
void saveSave(char save[SAVENAME_LENGTH], char name[16]) { //funkcja odpowiadaj¹ca za zapis nazwy save'u w odpowiednim 'slocie' na save'y - jest ich ³¹cznie 5
	FILE* plik;
	plik = fopen(name, "w+");
	for (int i = 0; i < SAVENAME_LENGTH; i++)
		fprintf(plik, "%c", save[i]);
	fclose(plik);
}

char* loadSave(char name[], char save[SAVENAME_LENGTH+1]) { //funkcja zwracaj¹ca nazwê save'u, który umiejscowiony jest na danym 'slocie'
	FILE* plik;
	plik = fopen(name, "r+");
	if (plik != NULL) {
		for (int i = 0; i < SAVENAME_LENGTH; i++)
			fscanf(plik, "%26s", save);
		fclose(plik);
	}
	else { //jeœli nie ma takiego pliku, to znaczy, ¿e slot jest pusty, tak wiêc zwracamy pust¹ nazwê
		for (int i = 0; i < SAVENAME_LENGTH; i++)
			save[i] = ' ';
	}
		return save;
}

void save(int positionX, int points, double worldTime, double distance, double distanceOnRoad, double sidePosition, int counter, int bulletsNum, int hearts, int pointsBanned, int gameplayPoints, int pointsStoppedAt, double worldTimeSave, double worldTimeBantime, double worldTimeFlag, Vehicle *vehicle, Bullet bullet, int heartGainedAt) {
	FILE* plik; //funkcja odpowiadaj¹ca za zapis wszystkich wa¿nych informacji do save'u
	time_t czas = 0;
	time(&czas);
	char saves[SAVENAME_LENGTH], *save1=0, *save2=0, *save3=0, *save4=0, *save5=0;
	strftime(saves, sizeof(saves), "Saves/%F_%H.%M.%S", localtime(&czas)); //nazwa zale¿na od obecnego czasu
	plik = fopen(saves, "w+");
	fwrite(&positionX, sizeof(positionX), 1, plik);
	fwrite(&points, sizeof(points), 1, plik);
	fwrite(&worldTime, sizeof(worldTime), 1, plik);
	fwrite(&distance, sizeof(distance), 1, plik);
	fwrite(&distanceOnRoad, sizeof(distanceOnRoad), 1, plik);
	fwrite(&sidePosition, sizeof(sidePosition), 1, plik);
	fwrite(&bulletsNum, sizeof(bulletsNum), 1, plik);
	fwrite(&hearts, sizeof(hearts), 1, plik);
	fwrite(&pointsBanned, sizeof(pointsBanned), 1, plik);
	fwrite(&gameplayPoints, sizeof(gameplayPoints), 1, plik);
	fwrite(&pointsStoppedAt, sizeof(pointsStoppedAt), 1, plik);
	fwrite(&worldTimeSave, sizeof(worldTimeSave), 1, plik);
	fwrite(&worldTimeBantime, sizeof(worldTimeBantime), 1, plik);
	fwrite(&worldTimeFlag, sizeof(worldTimeFlag), 1, plik);
	fwrite(&heartGainedAt, sizeof(heartGainedAt), 1, plik);

	for (int i = 0; i < NUMBER_OF_VEHICLES; i++) { //zapisujemy równie¿ struktury
		fwrite(&vehicle[i], sizeof(Vehicle), 1, plik);
	}
	fwrite(&bullet, sizeof(Bullet), 1, plik);

	counter = loadCounter(); //doliczamy +1 do pliku z liczb¹ zapisanych save'ów
	counter++;

	if (counter % NUMBER_OF_SLOTS == 1) { //tu, na podstawie liczby zapisanych save'ów, przydzielamy odpowiedniego 'slota' do zapisu. Plik najstarszy jest nadpisywany przez obecny, chyba ¿e nie wszystkie sloty s¹ zape³nione.
		save1 = saves;
		saveSave(save1, "SaveNames/save1");
	}
	if (counter % NUMBER_OF_SLOTS == 2) {
		save2 = saves;
		saveSave(save2, "SaveNames/save2");
	}
	if (counter % NUMBER_OF_SLOTS == 3) {
		save3 = saves;
		saveSave(save3, "SaveNames/save3");
	}
	if (counter % NUMBER_OF_SLOTS == 4) {
		save4 = saves;
		saveSave(save4, "SaveNames/save4");
	}
	if (counter % NUMBER_OF_SLOTS == 0) {
		save5 = saves;
		saveSave(save5, "SaveNames/save5");
	}

	saveCounter(counter); //edytujemy plik zliczaj¹cy iloœæ save'ów
	fclose(plik);

};
void loadAction(char* saveName, int& positionX, int& points, int& direction, int& tryb, double& worldTime, double& distance, double& distanceOnRoad, double& sidePosition, int& bulletsNum, int& hearts, int& pointsBanned, int& gameplayPoints, int& pointsStoppedAt, double& worldTimeSave, double& worldTimeBantime, double& worldTimeFlag, float&enemySpeed, float&allySpeed, float&bishopSpeed, Vehicle *vehicle, Bullet &bullet, double &worldTimeSave2, double &worldTimeSave3, int &heartGainedAt, int&paused, int&loading, double&boomX, double&boomY, double&boomTime) {
	FILE* plik; //funkcja odpowiadaj¹ca za zczytanie informacji o zapisanej grze z pliku
	plik = fopen(saveName, "r+");
	fread(&positionX, sizeof(positionX), 1, plik);
	fread(&points, sizeof(points), 1, plik);
	fread(&worldTime, sizeof(worldTime), 1, plik);
	fread(&distance, sizeof(distance), 1, plik);
	fread(&distanceOnRoad, sizeof(distanceOnRoad), 1, plik);
	fread(&sidePosition, sizeof(sidePosition), 1, plik);
	fread(&bulletsNum, sizeof(bulletsNum), 1, plik);
	fread(&hearts, sizeof(hearts), 1, plik);
	fread(&pointsBanned, sizeof(pointsBanned), 1, plik);
	fread(&gameplayPoints, sizeof(gameplayPoints), 1, plik);
	fread(&pointsStoppedAt, sizeof(pointsStoppedAt), 1, plik);
	fread(&worldTimeSave, sizeof(worldTimeSave), 1, plik);
	fread(&worldTimeBantime, sizeof(worldTimeBantime), 1, plik);
	fread(&worldTimeFlag, sizeof(worldTimeFlag), 1, plik);
	fread(&heartGainedAt, sizeof(heartGainedAt), 1, plik);

	for (int i = 0; i < NUMBER_OF_VEHICLES; i++) {
		fread(&vehicle[i], sizeof(Vehicle), 1, plik);
	}
	fread(&bullet, sizeof(Bullet), 1, plik);
	fclose(plik);

	enemySpeed = ENEMY_SPEED, bishopSpeed = BISHOP_SPEED, allySpeed = ALLY_SPEED; tryb = 0; direction = 0; worldTimeSave2 = 0; worldTimeSave3 = 0; paused = 0; loading = 0; boomX = -100; boomY = 0; boomTime = 0;//przydzielamy równie¿ podstawowe wartoœci, aby rozgrywka wczyta³a siê poprawnie
}

void manageDestroyed(SDL_Surface* screen, double& x, double& y, char& direction, int& destroyed, int& gameplayPoints, int& pointsBanned, int enemy, int &bulletsNum, double &worldTimeBantime, double &worldTime, int &pointsStoppedAt, int points, int positionX, double delta) {
	if (direction == 'l') { //funkcja odpowiadaj¹ca za zepchniête pojazdy
		x = x - (2100*delta);
	}
	if (direction == 'r') {
		x = x + (2100 *delta);
	}
	if (x <= SIDE_WIDTH || x >= SCREEN_WIDTH-SIDE_WIDTH) {	//gdy zepchniêty pojazd wyjedzie na trawê
		destroyed = 0; 
		direction = 0;
		if (pointsBanned == 0) { //jeœli nie mamy blokady na punkty i zepchniêtym pojazdem jest przeciwnik, to dostajemy 50 punktów i dodatkowy nabój
			if (enemy == 1) {
				if (positionX >= SIDE_WIDTH && positionX <= SCREEN_WIDTH-SIDE_WIDTH)
				gameplayPoints += DESTROY_POINTS;
				if (bulletsNum < NUMBER_OF_BULLETS)
					bulletsNum++;
			}
		}
		if (enemy == 0) { //jeœli zepchniêtym pojazdem jest pojazd neutralny, to odejmujemy 50 punktów, dostajemy blokadê na punkty i tracimy ewentualny dodatkowy nabój
			gameplayPoints -= DESTROY_POINTS;
			worldTimeBantime = worldTime;
			if (pointsBanned == 0) {
				pointsStoppedAt = points - DESTROY_POINTS;
			}
			if (pointsBanned == 1) {
				pointsStoppedAt = pointsStoppedAt - DESTROY_POINTS;
			}
			if (bulletsNum >= 1)
				bulletsNum--;
			pointsBanned = 1;
		}
		x = rand() % (SCREEN_WIDTH - (2 * SIDE_WIDTH)) + SIDE_WIDTH; //na samym koñcu resetujemy po³o¿enie zepchniêtego pojazdu
		y = rand() % 500 - 550;
	}
}

void manageLifeloss(int &hearts, double& x, double& y, double &worldTimeSave3, int &worldTimeFlag, double worldTime, double &boomX, double &boomY,double &boomTime, int &positionX, int &pointsStoppedAt, int points, double &worldTimeBantime, int &pointsBanned, int &gameplayPoints, int &enemy) {
	worldTimeBantime = worldTime; //funkcja odpowiadaj¹ca za sytuacjê, w której naje¿d¿aj¹c na ty³ pojazdu tracimy ¿ycie
	if (enemy == 0) { //rozpatrujemy sytuacjê, gdzie naje¿d¿amy na pojazd neutralny, zabieramy punkty, przydzielamy karê
		worldTimeSave3 = worldTime;
		gameplayPoints -= DESTROY_POINTS;
		if (pointsBanned == 0) {
			pointsStoppedAt = points - DESTROY_POINTS;
		}
		if (pointsBanned == 1) {
			pointsStoppedAt = pointsStoppedAt - DESTROY_POINTS;
		}
		pointsBanned = 1;
	}
	if (worldTimeFlag == 0) { //zapamiêtujemy miejsce zderzenia, potem przyda siê to do przedstawienia wybuchu, zmienna worldTimeFlag po to, by tylko raz siê to wydarzy³o w przeci¹gu danego okresu czasu
		boomX = x;
		boomY = y;
		boomTime = worldTime;
		if(worldTime> SAFE_TIME) //gdy czas przekroczy³ 15 sekund, czyli czas kiedy jest nieskoñczona iloœæ aut, to tracimy ¿ycie
		hearts--;
	}
	x = rand() % (SCREEN_WIDTH - (2 * SIDE_WIDTH)) + SIDE_WIDTH; //po zderzeniu pojazd ma resetowane po³o¿enie
	y = rand() % 500 - 550;
	positionX = SCREEN_WIDTH / 2; //nasz pojazd trafia na œrodek
	worldTimeFlag = 1;
	}

void bulletHit(int& bullet_fired, double& bullet_x, double& bullet_y, double& x, double& y, int& gameplayPoints, double& boomTime, double worldTime, double&boomX, double&boomY,int &worldTimeFlag, int enemy, int &pointsBanned, double &worldTimeBantime, int &pointsStoppedAt, int points, int positionX) {
	bullet_fired = 0; //funkcja odpowiadaj¹ca za sytuacjê, gdy nasz pocisk trafi w pojazd
	bullet_x = 0; bullet_y = 0; //resetujemy informacje o pocisku po uderzeniu
	if (enemy == 1) { //jeœli trafionym pojazdem jest wróg, to dostajemy 50 punktów, gdy nie mamy blokady
		if (pointsBanned == 0) {
			if (positionX>=SIDE_WIDTH&&positionX<=SCREEN_WIDTH-SIDE_WIDTH)
			gameplayPoints += DESTROY_POINTS;
		}
	}
	if (enemy == 0) { //jeœli trafiamy sojusznika, to tracimy 50 punktów, otrzymujemy blokadê
		gameplayPoints -= DESTROY_POINTS;
		worldTimeBantime = worldTime;
		if (pointsBanned == 0) { //gdy wielokrotnie strzelimy do sojusznika, to punkty podczas blokady bêd¹ aktualizowane
			pointsStoppedAt = points- DESTROY_POINTS;
		}
		if (pointsBanned == 1) {
			pointsStoppedAt = pointsStoppedAt - DESTROY_POINTS;
		}
		pointsBanned = 1;
	}
	if (worldTimeFlag == 0) { //informacje do wyœwietlenia wybuchu
		boomX = x; boomY = y;
		boomTime = worldTime;
		worldTimeFlag = 1;
	}
	x = rand() % (SCREEN_WIDTH - (2 * SIDE_WIDTH)) + SIDE_WIDTH; //resetujemy po³o¿enie pojazdu
	y = rand() % 500 - 550;
}

void manageVehicles(double& x, double& y, int enemy, SDL_Surface* screen, SDL_Surface* enemy_car, SDL_Surface* ally_car, SDL_Surface* bishop, int &positionX, int& pointsBanned, int& pointsStoppedAt, int &points, double worldTime, double& worldTimeBantime, int paused, int ended, int& hearts, int &destroyed, char &direction, int &gameplayPoints, double 	&worldTimeSave3, int worldTimeFlag, double& boomX, double& boomY, double& boomTime,double &bullet_x,double &bullet_y, int &bullet_fired, int &bulletsNum, double delta) {
		if (enemy == 1||enemy==0) { //g³owna funkcja odpowiadaj¹ca za pojazdy, tu znajduje siê ca³a matematyka
			if (destroyed == 0) { //przeciwnik
				if (y > SCREEN_HEIGHT + 200|| y < -550) { //gdy pojazd wyjedzie daleko poza zasiêg widzenia, resetujemy jego po³o¿enie
					x = rand() % (SCREEN_WIDTH - (2 * SIDE_WIDTH)) + SIDE_WIDTH;
					y = rand() % 500 - 550;
				}
				if (enemy == 1) {
					if (y > 0 && y < SCREEN_HEIGHT / 2) { //gdy wrogi pojazd znajdzie siê w obszarze widzenia, to bêdzie chcia³ zajechaæ nam drogê
						if (paused == 0 && ended == 0) {
							if (positionX > x) {
								double random = rand() % 100 + 5;
								x = x + (random / 200)*delta*800;
							}
							if (positionX < x) {
								double random = rand() % 100 + 5;
								x = x - (random / 200)*delta* 800;
							}
						}
					}
				}
			}
			if (y<(SCREEN_HEIGHT / 2) + 100 && y>(SCREEN_HEIGHT / 2) - 100 && (positionX - x) <= 38 && (positionX - x) >= 28 || (destroyed == 1 && direction == 'l')) { //gdy uderzymy pojazd od jego prawej strony
				direction = 'l';
				destroyed = 1;
				manageDestroyed(screen, x, y, direction, destroyed, gameplayPoints, pointsBanned, enemy, bulletsNum, worldTimeBantime, worldTime, pointsStoppedAt, points, positionX, delta);//left
			}
			if (y<(SCREEN_HEIGHT / 2) + 100 && y>(SCREEN_HEIGHT / 2) - 100 && (x - positionX) <= 38 && (x - positionX) >= 28 || (destroyed == 1 && direction == 'r')) { //gdy uderzymy pojazd od jego lewej strony
				direction = 'r';
				destroyed = 1;
				manageDestroyed(screen, x, y, direction, destroyed, gameplayPoints, pointsBanned, enemy, bulletsNum, worldTimeBantime, worldTime, pointsStoppedAt, points, positionX, delta);//right
			}
			if (y>=(SCREEN_HEIGHT / 2) - 100 &&y<=(SCREEN_HEIGHT/2)+100&& (positionX - x)<28 && (positionX - x) > -28 ) { //gdy uderzymy pojazd od ty³u/przodu
				manageLifeloss(hearts, x, y, worldTimeSave3, worldTimeFlag, worldTime, boomX, boomY, boomTime, positionX, pointsStoppedAt, points, worldTimeBantime, pointsBanned, gameplayPoints, enemy);
			}
			if (bullet_x >x-25&&bullet_x<x+25&&bullet_y>y-20&&bullet_y<y+20) { //gdy pojazd natrafi na pocisk
				bulletHit(bullet_fired, bullet_x, bullet_y, x, y, gameplayPoints, boomTime, worldTime, boomX, boomY, worldTimeFlag, enemy, pointsBanned, worldTimeBantime, pointsStoppedAt, points, positionX);
			}
		}
		if (enemy == 1) { //pojazd wrogi
			DrawSurface(screen, enemy_car, x, y); //zale¿nie od zmian w powy¿szych funkcjach rysujemy pojazd w danym miejscu
		}
		if (enemy == 0) { //pojazd neutralny
			DrawSurface(screen, ally_car, x, y);
		}
		if (enemy == 2) { //pojazd 'goniec' - gdy nas dogoni, tracimy ¿ycie i resetuje swoje po³o¿enie
			if (y > SCREEN_HEIGHT + 300) {
				y = SCREEN_HEIGHT + 300;
			}
			if (y <= (SCREEN_HEIGHT / 2) + 40) {
				if (worldTime > SAFE_TIME) {
					hearts--;
				}
				positionX = SCREEN_WIDTH / 2;
				y = SCREEN_HEIGHT + 300;
			}
			DrawSurface(screen, bishop, x, y);
		}
		if (positionX <= 30 || positionX >= SCREEN_WIDTH-30) {
			if (worldTime>SAFE_TIME)
			hearts--;
			positionX = SCREEN_WIDTH / 2;
		}
}
void banCountdown(int &pointsBanned, double worldTime, double worldTimeBantime, int paused) { //funkcja zliczaj¹ca 5 sekund blokady na punkty
	if (paused == 0) {
		if (worldTime - worldTimeBantime > 5) {
			pointsBanned = 0;
		}
	}
}		
void free_surfaces(SDL_Surface* charset, SDL_Renderer* renderer, SDL_Texture* scrtex, SDL_Window* window, SDL_Surface* screen, SDL_Surface* car, SDL_Surface* map, SDL_Surface* side, SDL_Surface* pause, SDL_Surface* enemy_car, SDL_Surface* ally_car,SDL_Surface* bishop, SDL_Surface* heart, SDL_Surface* inf_heart, SDL_Surface* sad_heart, SDL_Surface* game_lost, SDL_Surface* boom, SDL_Surface* bullets) {
	//funkcja odpowiadaj¹ca za czyszczenie wszystkich Surface, wywo³ana, gdy przy ³adowaniu napotkany zostaje b³¹d i na koñcu programu
	SDL_FreeSurface(screen);
	SDL_FreeSurface(charset);
	SDL_FreeSurface(map);
	SDL_FreeSurface(car);
	SDL_FreeSurface(pause);
	SDL_FreeSurface(bishop);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}
void loadingScreen(SDL_Surface* screen, SDL_Surface* charset, char text[128], int zielony, int niebieski, char* save1, char* save2, char* save3, char* save4, char* save5, int &counter) {
	counter = loadCounter(); //pokazanie dostêpnych zapisów do wczytania
	DrawRectangle(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 8), SCREEN_HEIGHT / 2 - 200, (SCREEN_WIDTH / 4), 100, zielony, niebieski);
	sprintf(text, "WYBIERZ ZAPIS");
	DrawString(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 8) + 4, SCREEN_HEIGHT / 2 - 195, text, charset);
	if (counter>0) {
		sprintf(text, "1. %s", save1);
		DrawString(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 8) + 4, SCREEN_HEIGHT / 2 - 180, text, charset);
	}
	if (counter > 1) {
		sprintf(text, "2. %s", save2);
		DrawString(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 8) + 4, SCREEN_HEIGHT / 2 - 165, text, charset);
	}
	if (counter > 2) {
		sprintf(text, "3. %s", save3);
		DrawString(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 8) + 4, SCREEN_HEIGHT / 2 - 150, text, charset);
	}
	if (counter > 3) {
		sprintf(text, "4. %s", save4);
		DrawString(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 8) + 4, SCREEN_HEIGHT / 2 - 135, text, charset);
	}
	if (counter > 4) {
		sprintf(text, "5. %s", save5);
		DrawString(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 8) + 4, SCREEN_HEIGHT / 2 - 120, text, charset);
	}
}

int loadImages(SDL_Renderer*& renderer, SDL_Texture*& scrtex, SDL_Window*& window, SDL_Surface*& screen,SDL_Surface*& car, SDL_Surface*& map, SDL_Surface*& charset, SDL_Surface*& enemy_car, SDL_Surface*& ally_car, SDL_Surface*& side, SDL_Surface*& pause, SDL_Surface*& bishop, SDL_Surface*& heart, SDL_Surface*& inf_heart, SDL_Surface*& sad_heart, SDL_Surface*& game_lost, SDL_Surface*& boom, SDL_Surface*& bullets) {
	charset = SDL_LoadBMP("./cs8x8.bmp"); //funkcja odpowiadaj¹ca za wczytanie plików BMP, gdy zostanie napotkany b³¹d, odpalana jest funkcja free_surfaces
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	map = SDL_LoadBMP("./map.bmp");
	if (map == NULL) {
		printf("SDL_LoadBMP(map.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};

	car = SDL_LoadBMP("./car.bmp");
	if (car == NULL) {
		printf("SDL_LoadBMP(car.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	enemy_car = SDL_LoadBMP("./enemy_car.bmp");
	if (enemy_car == NULL) {
		printf("SDL_LoadBMP(enemy_car.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	ally_car = SDL_LoadBMP("./ally_car.bmp");
	if (ally_car == NULL) {
		printf("SDL_LoadBMP(ally_car.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};

	side = SDL_LoadBMP("./side.bmp");
	if (side == NULL) {
		printf("SDL_LoadBMP(side.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	pause = SDL_LoadBMP("./pause.bmp");
	if (pause == NULL) {
		printf("SDL_LoadBMP(pause.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	bishop = SDL_LoadBMP("./bishop.bmp");
	if (bishop == NULL) {
		printf("SDL_LoadBMP(bishop.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	heart = SDL_LoadBMP("./heart.bmp");
	if (heart == NULL) {
		printf("SDL_LoadBMP(heart.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	inf_heart = SDL_LoadBMP("./inf_heart.bmp");
	if (inf_heart == NULL) {
		printf("SDL_LoadBMP(inf_heart.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	sad_heart = SDL_LoadBMP("./sad_heart.bmp");
	if (sad_heart == NULL) {
		printf("SDL_LoadBMP(sad_heart.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	game_lost = SDL_LoadBMP("./game_lost.bmp");
	if (game_lost == NULL) {
		printf("SDL_LoadBMP(game_lost.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	boom = SDL_LoadBMP("./boom.bmp");
	if (boom == NULL) {
		printf("SDL_LoadBMP(boom.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	bullets = SDL_LoadBMP("./bullet.bmp");
	if (bullets == NULL) {
		printf("SDL_LoadBMP(bullet.bmp) error: %s\n", SDL_GetError());
		free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);
		return 1;
	};
	return 0;
}

void gameEnded(SDL_Surface* screen, SDL_Surface* game_lost, int& ended, float &allySpeed, float& enemySpeed, float& bishopSpeed, double& speed, double& worldTime) {
	ended = 1; //gdy gra jest zakoñczona, odpowiednie zmienne przyjmuj¹ dane wartoœci
	allySpeed = 0; enemySpeed = 0; bishopSpeed = 0; speed = 0;
}

int manageHearts(double &worldTime, int &hearts, SDL_Surface* screen, SDL_Surface* heart, SDL_Surface* inf_heart, SDL_Surface* sad_heart, SDL_Surface* game_lost, int points, int &paused, int &ended, float& allySpeed, float& enemySpeed, float& bishopSpeed, double& speed, double &worldTimeSave, int gameplayPoints, int &heartGainedAt) {
	//funkcja odpowiadaj¹ca za zarz¹dzanie ¿yciami/dostêpnymi autami
	if (paused == 0) {
		if (hearts == -1) {
			gameEnded(screen, game_lost, ended, allySpeed, enemySpeed, bishopSpeed, speed, worldTime);
		}
		if (worldTime < SAFE_TIME) { //do 15 sekundy posiadamy nieskoñczon¹ iloœæ ¿yæ
			DrawSurface(screen, inf_heart, 60, SCREEN_HEIGHT - 60);
		}
		else {
			if (hearts == 0) { //jeœli mamy 0 ¿yæ, równie¿ zwracamy funkcjê gameEnded
				gameEnded(screen, game_lost, ended, allySpeed, enemySpeed, bishopSpeed, speed, worldTime);
			}
			if (hearts == 1) {
				DrawSurface(screen, heart, 60, SCREEN_HEIGHT - 60);
				DrawSurface(screen, sad_heart, 60, SCREEN_HEIGHT - 180);
				DrawSurface(screen, sad_heart, 60, SCREEN_HEIGHT - 300);
			}
			if (hearts == 2) {
				DrawSurface(screen, heart, 60, SCREEN_HEIGHT - 60);
				DrawSurface(screen, heart, 60, SCREEN_HEIGHT - 180);
				DrawSurface(screen, sad_heart, 60, SCREEN_HEIGHT - 300);
			}
			if (hearts == 3) {
				DrawSurface(screen, heart, 60, SCREEN_HEIGHT - 60);
				DrawSurface(screen, heart, 60, SCREEN_HEIGHT - 180);
				DrawSurface(screen, heart, 60, SCREEN_HEIGHT - 300);
			}
		}
		if (worldTime >= SAFE_TIME) { //regenerujemy ¿ycie co 1500 zdobytych punktów
			if (points / heartGainedAt>= 1) {
					if (hearts < 3) {
						heartGainedAt += 1500;
							return hearts + 1;
					}
			}
		}
	}
	return hearts;
}

void fireBullet(double &x, double &y,int& fired, int &bulletsNum, int positionX) { //gdy posiadamy pociski, wystrzelamy jeden z nich
	if (bulletsNum >= 1) {
		fired = 1;
		bulletsNum--;
		x = positionX;
		y = SCREEN_HEIGHT / 2;
	}
}
void drawBullet(double& x, double& y, int& fired, SDL_Surface* screen, SDL_Surface* bullets, double delta) { //rysujmemy wystrzelony pocisk, gdy wychodzi poza strefê widzenia nie trafiaj¹c nikogo, resetuje siê i mo¿emy wystrzeliæ kolejny pocisk
	if (fired == 1) {
		y = y - (5*delta*500);
		if (y <= -50) {
			y = 0; x = 0; fired = 0;
		}
		DrawSurface(screen, bullets, x, y);
	}
}
void manageBullets(int bulletsNum, SDL_Surface* screen, SDL_Surface* bullets) { //funkcja odpowiadaj¹ca za zilustrowanie iloœci posiadanych przez nas kul - max. iloœæ to 3
		if (bulletsNum == 1) {
			DrawSurface(screen, bullets, SCREEN_WIDTH/2, SCREEN_HEIGHT - 50);
		}
		if (bulletsNum == 2) {
			DrawSurface(screen, bullets, SCREEN_WIDTH / 2-20, SCREEN_HEIGHT - 50);
			DrawSurface(screen, bullets, SCREEN_WIDTH / 2+20, SCREEN_HEIGHT - 50);
		}
		if (bulletsNum == 3) {
			DrawSurface(screen, bullets, SCREEN_WIDTH / 2-40, SCREEN_HEIGHT - 50);
			DrawSurface(screen, bullets, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50);
			DrawSurface(screen, bullets, SCREEN_WIDTH / 2+40, SCREEN_HEIGHT - 50);
		}
}

int starting(int &rc, SDL_Window*& window, SDL_Renderer*& renderer, SDL_Surface*& screen, SDL_Texture*& scrtex) { //funkcja inicjuj¹ca, przy ew. b³êdach program bêdzie zatrzymany

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}


	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "SPY HUNTER - Juliusz Radziszewski");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);
	return 0;
}

void colorHandling(int &czarny, int &zielony, int &czerwony, int &niebieski, SDL_Surface* charset, SDL_Surface* screen) { //funkcja inicjalizuj¹ca kolory do póŸniejszego u¿ytku
	SDL_SetColorKey(charset, true, 0x000000);
	czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

}

void structHandling(Vehicle *vehicle) { //przydzielamy wartoœci pocz¹tkowe tablicy struktur Vehicle
	vehicle[0].enemy = 1;	vehicle[1].enemy = 1;	vehicle[2].enemy = 1;	vehicle[3].enemy = 0;	vehicle[4].enemy = 0; vehicle[5].enemy = 2;
	for (int i = 0; i < NUMBER_OF_VEHICLES - 1; i++) {
		vehicle[i].x = rand() % 880 + SIDE_WIDTH;
		vehicle[i].y = rand() % 500 - 600;
		vehicle[i].destroyed = 0;
		vehicle[i].direction = 0;
	};
	vehicle[5].x = SCREEN_WIDTH / 2; vehicle[5].y = SCREEN_HEIGHT + 300;
}

void variableHandling(int &t1, int &t2, double &delta, double &worldTime, double &distance, double speed, int positionX, int pointsBanned, double &distanceOnRoad, int &points, double &worldTimeFlag, int gameplayPoints, double worldTimeSave3, double &sidePosition, float enemySpeed, float allySpeed, float bishopSpeed, Vehicle* vehicle) {
	t2 = SDL_GetTicks(); //przydzielamy wartoœci zmiennym potrzebnym w poprawnym dzia³aniu programu
	delta = (t2 - t1) * 0.001; //ró¿nica czasowa miêdzy tickami
	t1 = t2;

	worldTime += delta; //czas rozgrywki
	distance += speed * delta; //dystans zale¿ny od naszej prêdkoœci i delty
	if (positionX >= SIDE_WIDTH && positionX <= SCREEN_WIDTH - SIDE_WIDTH) {//gdy nie mamy blokady na punkty naliczane s¹ punkty za jazdê po trasie
		if (pointsBanned == 0) {
			distanceOnRoad += speed * delta;
			points = distanceOnRoad * 20 + gameplayPoints;
		}
	}
	if (worldTimeFlag == 1) { //ogranicznik 1-sekundowy na jedn¹ z akcji gracza
		if (worldTime - worldTimeSave3 > 1) {
			worldTimeFlag = 0;
		}
	}
	sidePosition = sidePosition + speed*delta*500; //poruszanie siê pobocza zgodne z nasz¹ prêdkoœci¹
	for (int i = 0; i < NUMBER_OF_VEHICLES; i++) { //poruszanie siê pojazdów zale¿ne od naszej i ich prêdkoœci
		if (vehicle[i].enemy == 1)
			vehicle[i].y = vehicle[i].y - (enemySpeed - speed)*delta*500;
		else if (vehicle[i].enemy == 0)
			vehicle[i].y = vehicle[i].y - (allySpeed - speed) * delta * 500;
		else
			vehicle[i].y = vehicle[i].y - (bishopSpeed - speed) * delta * 500;
	}

}
void manageSpeed(int tryb, double &speed, int &positionX, int direction, double delta) { //zarz¹dzanie prêdkoœci¹ i kierunkiem jazdy zale¿nie od obecnie wciœniêtego przycisku zarz¹dzaj¹cego zmienn¹
	if (tryb == 1) {
		speed = 2;
	}
	if (tryb == -1) {
		speed = 0.3;
	}
	if (tryb == 0) {
		speed = 1;
	}
	if (tryb == -2) {
		speed = 0;
	}
	if (direction == 1) {
		if (positionX <= SCREEN_WIDTH)
			positionX = positionX + (1* delta *500);
	}
	if (direction == -1) {
		if (positionX >= 0)
			positionX = positionX - (1 * delta * 500);
	}
}
void drawMap(SDL_Surface* screen, int czarny, SDL_Surface* map, double &sidePosition, SDL_Surface* side) { //rysowanie mapy i pobocza
	SDL_FillRect(screen, NULL, czarny);
	DrawSurface(screen, map, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	if (sidePosition / 1080.0 > 1) {
		sidePosition = 360;
	}
	DrawSurface(screen, side, SIDE_WIDTH/2, sidePosition);
	DrawSurface(screen, side, SCREEN_WIDTH-SIDE_WIDTH/2, sidePosition);
	DrawSurface(screen, side, SIDE_WIDTH/2, sidePosition - SCREEN_HEIGHT);
	DrawSurface(screen, side, SCREEN_WIDTH - SIDE_WIDTH / 2, sidePosition - SCREEN_HEIGHT);
}
void drawSaveInfo(int &saved, double worldTime, double worldTimeSave2, SDL_Surface* screen, int zielony, int niebieski, SDL_Surface* charset, char text[128]) { //informacja o zapisaniu gry
	if (saved == 1 && worldTime - worldTimeSave2 < 5) {
		DrawRectangle(screen, 4, SCREEN_HEIGHT - 25, (SCREEN_WIDTH / 6) + 5, 20, zielony, niebieski);
		sprintf(text, "Pomyslnie zapisano stan gry");
		DrawString(screen, 5, SCREEN_HEIGHT - 23, text, charset);
	}
	else {
		saved = 0;
	}
}
void drawAllyInfo(int &pointsBanned, double worldTime, double worldTimeBantime, SDL_Surface* screen, int zielony, int niebieski, char text[128], SDL_Surface* charset, int paused) { //informacja ostrzegwacza o na³o¿onej karze
	if (pointsBanned == 1) {
		banCountdown(pointsBanned, worldTime, worldTimeBantime, paused);
		DrawRectangle(screen, 4, SCREEN_HEIGHT - 25, (SCREEN_WIDTH / 6) + 35, 20, zielony, niebieski);
		sprintf(text, "Nie atakuj neutralnych pojazdow");
		DrawString(screen, 5, SCREEN_HEIGHT - 23, text, charset);
	}
}
void boomManage(double boomTime, double &boomY, double boomX, double speed, double worldTime, int hearts, SDL_Surface* screen, SDL_Surface* boom, double delta, int paused) { //rysowanie wybuchu po zderzeniu z pojazdem lub trafieniu pojazdu
	if (boomTime != 0) {
		if (worldTime - boomTime < 1) {
			if (paused == 0) {
				if (hearts >= 1)
					boomY = boomY + (speed * delta * 500);
				DrawSurface(screen, boom, boomX, boomY);
			}
		}
	}
}
void pausedManage(int paused, SDL_Surface* screen, SDL_Surface* pause, double &speed, int &now) { //wyœwietlanie menu pauzy
	if (paused == 1) {
		DrawSurface(screen, pause, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		speed = 0;
	}
	now = 0;
}
void endedManage(int ended, SDL_Surface* screen, int zielony, int niebieski, int pointsBanned, int points, int pointsStoppedAt, char text[128], SDL_Surface* charset, double &speed, SDL_Surface* game_lost) { //wyœwietlanie menu skoñczonej gry z punktami
	if (ended == 1) {
		DrawRectangle(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 10), (SCREEN_HEIGHT / 2) - 130, (SCREEN_WIDTH / 5), 28, zielony, niebieski);
		if (pointsBanned == 0)
			sprintf(text, "Final score: %d", points);
		if (pointsBanned == 1)
			sprintf(text, "Final score: %d", pointsStoppedAt);
		DrawString(screen, SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 10) + 30, (SCREEN_HEIGHT / 2) - 120, text, charset);
		DrawSurface(screen, game_lost, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		speed = 0;
	}
}
void fpsManage(double &fpsTimer, double delta, double &fps, int &frames) { //zarz¹dzanie klatkami
	fpsTimer += delta;
	if (fpsTimer > 0.5) {
		fps = frames * 2;
		frames = 0;
		fpsTimer -= 0.5;
	};
}
void keyboardAction(SDL_Event event,int &quit,int &paused,int &ended,Bullet &bullet,int &bulletsNum,int &positionX,float &enemySpeed,float &bishopSpeed,float &allySpeed,int &points,int&frames,int&pointsBanned,int&hearts,int&gameplayPoints,int&pointsStoppedAt,double&worldTime,double&distance, double&speed,double&distanceOnRoad,double&worldTimeSave,double&worldTimeBantime,double&worldTimeFlag,double&worldTimeSave2,double&worldTimeSave3, Vehicle *vehicle, int&now, int&loading, double&sidePosition,int&counter,int&saved,int&direction,int&tryb, char *&save1, char *&save2, char *&save3, char *&save4, char *&save5, int &heartGainedAt, double &boomX, double &boomY, double &boomTime) {
	if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1; //wyjœcie z gry                          //zarz¹dzanie wejœciem od gracza - wszystkie mo¿liwe akcje gracza
	if (paused == 0 && ended == 0) {
		if (event.key.keysym.sym == SDLK_SPACE) { //strzelanie, gdy gra nie jest zapauzowana i skoñczona
			if (bullet.fired == 0)
				fireBullet(bullet.x, bullet.y, bullet.fired, bulletsNum, positionX);
		};
	}
	if (event.key.keysym.sym == SDLK_n) { //nowa gra - zresetowanie zmiennych, przydzielenie im wartoœci pocz¹tkowych nadanych na starcie gry
		enemySpeed = ENEMY_SPEED, bishopSpeed = BISHOP_SPEED, allySpeed = ALLY_SPEED; ended = 0; paused = 0; frames = 0; points = 0; positionX = SCREEN_WIDTH / 2; pointsBanned = 0; bulletsNum = 1; hearts = 1; gameplayPoints = 0; pointsStoppedAt = 0; worldTime = 0; distance = 0; speed = 1; distanceOnRoad = 0; worldTimeSave = 0; worldTimeBantime = 0; worldTimeFlag = 0; worldTimeSave2 = 0; worldTimeSave3 = 0; loading = 0; heartGainedAt = 1500; boomX = -100; boomY = 0; boomTime = 0;
		structHandling(vehicle);
		bullet.x = 0;		bullet.y = 0;		bullet.fired = 0;
	}
	if (event.key.keysym.sym == SDLK_p) { //pauzowanie gry - wszystkie dynamiczne zmienne s¹ zatrzymane, czas jest zapisany w zmiennej, by potem go przywróciæ do stanu sprzed pauzy
		if (paused == 0 && now == 0) {
			worldTimeSave = worldTime;
			paused = 1;
			now = 1;
			enemySpeed = 0; allySpeed = 0; bishopSpeed = 0;
		}
		if (paused == 1 && now == 0) { //odpauzowywanie gry
			worldTime = worldTimeSave;
			speed = 1;
			paused = 0;
			now = 1;
			loading = 0;
			enemySpeed = ENEMY_SPEED, bishopSpeed = BISHOP_SPEED, allySpeed = ALLY_SPEED;
		}
	}
	if (ended == 0) { //póki gra siê nie skoñczy³a, mo¿emy dokonaæ zapisu stanu gry
		if (event.key.keysym.sym == SDLK_s) {
			worldTimeSave2 = worldTime;
			save(positionX, points, worldTime, distance, distanceOnRoad, sidePosition, counter, bulletsNum, hearts, pointsBanned, gameplayPoints, pointsStoppedAt, worldTimeSave, worldTimeBantime, worldTimeFlag, vehicle, bullet, heartGainedAt);
			saved = 1;
			worldTime = worldTimeSave2;
		}
	}
	if (event.key.keysym.sym == SDLK_f) { //przycisk zakoñczenia gry
		hearts = -1;
		worldTimeSave = worldTime;
	}
	if (loading == 1) { //mo¿liwe do klikniêcia tylko wtedy, gdy ukazane jest menu wczytania gry, 5 save'ów do wyboru, zmienne s¹ zczytywane z pliku, po wczytaniu gra jest automatycznie odpauzowana
		if (counter > 0) {
			if (event.key.keysym.sym == SDLK_1) {
				loadAction(save1, positionX, points, direction, tryb, worldTime, distance, distanceOnRoad, sidePosition, bulletsNum, hearts, pointsBanned, gameplayPoints, pointsStoppedAt, worldTimeSave, worldTimeBantime, worldTimeFlag, enemySpeed, allySpeed, bishopSpeed, vehicle, bullet, worldTimeSave2, worldTimeSave3, heartGainedAt, paused, loading, boomX, boomY, boomTime);
				loading = 0; paused = 0; ended = 0;
			}
		}
		if (counter > 1) {
			if (event.key.keysym.sym == SDLK_2) {
				loadAction(save2, positionX, points, direction, tryb, worldTime, distance, distanceOnRoad, sidePosition, bulletsNum, hearts, pointsBanned, gameplayPoints, pointsStoppedAt, worldTimeSave, worldTimeBantime, worldTimeFlag, enemySpeed, allySpeed, bishopSpeed, vehicle, bullet, worldTimeSave2, worldTimeSave3, heartGainedAt, paused, loading, boomX, boomY, boomTime);
				loading = 0; paused = 0; ended = 0;
			}
		}
		if (counter > 2) {
			if (event.key.keysym.sym == SDLK_3) {
				loadAction(save3, positionX, points, direction, tryb, worldTime, distance, distanceOnRoad, sidePosition, bulletsNum, hearts, pointsBanned, gameplayPoints, pointsStoppedAt, worldTimeSave, worldTimeBantime, worldTimeFlag, enemySpeed, allySpeed, bishopSpeed, vehicle, bullet, worldTimeSave2, worldTimeSave3, heartGainedAt, paused, loading, boomX, boomY, boomTime);
				loading = 0; paused = 0; ended = 0;
			}
		}
		if (counter > 3) {
			if (event.key.keysym.sym == SDLK_4) {
				loadAction(save4, positionX, points, direction, tryb, worldTime, distance, distanceOnRoad, sidePosition, bulletsNum, hearts, pointsBanned, gameplayPoints, pointsStoppedAt, worldTimeSave, worldTimeBantime, worldTimeFlag, enemySpeed, allySpeed, bishopSpeed, vehicle, bullet, worldTimeSave2, worldTimeSave3, heartGainedAt, paused, loading, boomX, boomY, boomTime);
				loading = 0; paused = 0; ended = 0;
			}
		}
		if (counter > 4) {
			if (event.key.keysym.sym == SDLK_5) {
				loadAction(save5, positionX, points, direction, tryb, worldTime, distance, distanceOnRoad, sidePosition, bulletsNum, hearts, pointsBanned, gameplayPoints, pointsStoppedAt, worldTimeSave, worldTimeBantime, worldTimeFlag, enemySpeed, allySpeed, bishopSpeed, vehicle, bullet, worldTimeSave2, worldTimeSave3, heartGainedAt, paused, loading, boomX, boomY, boomTime);
				loading = 0; paused = 0; ended = 0;
			}
		}
	}
};
void keyboardLoading(int& loading, float& enemySpeed, float& allySpeed, float& bishopSpeed, double& worldTimeSave, double& worldTime, int& paused) { //zmienne po naciœniêciu przycisku do wczytania gry
	if (loading == 0) {
		enemySpeed = 0; allySpeed = 0; bishopSpeed = 0;
		worldTimeSave = worldTime;
		paused = 1;
		loading = 1;
	}
}
void keyUpEvent(SDL_Event event, double &speed, int&tryb, int&direction) { //odciœniêcie przycisków - poruszanie siê
	if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
		speed = 1.0;
		tryb = 0;
	}
	if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
		direction = 0;
	}
}
void scancodeEvent(int paused, int ended, const Uint8 *currentKeyState, double&speed,int&tryb,int&direction,int&frames) { //naciskanie przycisków - poruszanie siê
	if (paused == 0 && ended == 0) {
		if (currentKeyState[SDL_SCANCODE_UP]) {
			speed = 2.0;
			tryb = 1;
		}
		if (currentKeyState[SDL_SCANCODE_DOWN]) {
			speed = 0.3;
			tryb = -1;
		}
		if (currentKeyState[SDL_SCANCODE_RIGHT]) {
			direction = 1;
		}
		if (currentKeyState[SDL_SCANCODE_LEFT]) {
			direction = -1;
		}
	}
	frames++;
}

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {// main
	int t1, t2, quit = 0, frames = 0, rc, points = 0, positionX = SCREEN_WIDTH / 2, tryb = 0, direction = 0, paused = 0, now = 0, saved = 0, counter = 0, loading = 0, pointsBanned = 0, ended = 0, bulletsNum = 1, hearts = 1, gameplayPoints = 0, pointsStoppedAt = 0, czarny = 0, zielony = 0, czerwony = 0, niebieski = 0, heartGainedAt = 1500;
	double delta, worldTime=0, fpsTimer=0, fps=0, distance=0, speed=1, distanceOnRoad=0, sidePosition= SCREEN_HEIGHT / 2, worldTimeSave=0, worldTimeSave2=0, worldTimeBantime = 0, worldTimeFlag = 0, worldTimeSave3 = 0, boomX = -100, boomY = 0, boomTime=0;
	char* save1, * save2, * save3, * save4, * save5, *saveNumber = 0;
	float enemySpeed = ENEMY_SPEED, bishopSpeed = BISHOP_SPEED, allySpeed = ALLY_SPEED;
	char text[128];
	SDL_Event event;
	SDL_Surface* screen, * charset,* car=0, * map=0, * side=0, * pause=0, *enemy_car=0, *ally_car=0, *bishop=0, *inf_heart=0, *sad_heart=0, *heart=0, *game_lost=0, *boom=0, *bullets=0;
	SDL_Texture* scrtex;
	SDL_Window* window=0;
	SDL_Renderer* renderer=0;
	Bullet bullet; //deklarujemy i przydzielamy wartoœci odpowiednim zmiennym
	bullet.x = 0;	bullet.y = 0;	bullet.fired = 0;
	Vehicle vehicle[NUMBER_OF_VEHICLES];
	structHandling(vehicle);

	srand(time(NULL));
	int ifError=starting(rc, window, renderer, screen, scrtex); //inicjalizujemy program, gdy wyst¹pi b³¹d to return 1
	if (ifError == 1)
		return 1;
	int decision = loadImages(renderer, scrtex, window, screen, car, map, charset, enemy_car, ally_car, side, pause, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets); //wczytujemy zdjêcia, gdy wyst¹pi b³¹d to return 1
	if (decision == 1)
		return 1;
	colorHandling(czarny, zielony, czerwony, niebieski, charset, screen); //inicjalizacja kolorów do póŸniejszego u¿ytku

	t1 = SDL_GetTicks();
	while (!quit) { //pêtla gry
		variableHandling(t1, t2, delta, worldTime, distance, speed, positionX, pointsBanned, distanceOnRoad, points, worldTimeFlag, gameplayPoints, worldTimeSave3, sidePosition, enemySpeed, allySpeed, bishopSpeed, vehicle); //ustawienie odpowiednich wartoœci zmiennych
		drawMap(screen, czarny, map, sidePosition, side); //rysujemy drogê i pobocze
		drawSaveInfo(saved, worldTime, worldTimeSave2, screen, zielony, niebieski, charset, text); //informacja o pomyœlnym zapisie gry
		drawAllyInfo(pointsBanned, worldTime, worldTimeBantime, screen, zielony, niebieski, text, charset, paused); //informacja o kolizji z neutralnym pojazdem
		manageSpeed(tryb, speed, positionX, direction, delta); //zarz¹dzanie prêdkoœci¹ i kierunkiem jazdy
		DrawSurface(screen, car, positionX, SCREEN_HEIGHT / 2); //rysowanie auta
		for (int i = 0; i < NUMBER_OF_VEHICLES; i++) { //zarz¹dzanie pojazdami - zderzenia, kolizje, postrzelenia, respienie itd.
			manageVehicles(vehicle[i].x,vehicle[i].y,vehicle[i].enemy, screen, enemy_car, ally_car, bishop, positionX, pointsBanned, pointsStoppedAt, points, worldTime, worldTimeBantime, paused, ended, hearts, vehicle[i].destroyed, vehicle[i].direction, gameplayPoints, worldTimeSave3, worldTimeFlag, boomX, boomY, boomTime, bullet.x, bullet.y, bullet.fired, bulletsNum, delta);
		}
		if (loading == 1)
			loadingScreen(screen, charset, text, zielony, niebieski, save1, save2, save3, save4, save5, counter); //wyœwietlanie tabelki z pokazanymi save'ami do wczytania
		boomManage(boomTime,boomY,boomX,speed,worldTime,hearts,screen,boom,delta, paused); //rysowanie wybuchu
		pausedManage(paused, screen, pause, speed, now); //rysowanie menu pauzy
		endedManage(ended, screen, zielony, niebieski, pointsBanned, points, pointsStoppedAt, text, charset, speed, game_lost); //rysowanie menu po skoñczonej grze
		drawBullet(bullet.x, bullet.y, bullet.fired, screen, bullets, delta); //rysowanie wystrzelonego pocisku
		fpsManage(fpsTimer, delta, fps, frames); //zarz¹dzanie klatkami
		setMenu(zielony, niebieski, screen, text, charset, worldTime, fps, points, speed, paused, worldTimeSave, pointsStoppedAt, pointsBanned, ended, gameplayPoints); //rysowanie menu
		hearts=manageHearts(worldTime,hearts, screen, heart, inf_heart, sad_heart, game_lost, points, paused, ended, allySpeed, enemySpeed, bishopSpeed, speed, worldTimeSave, gameplayPoints, heartGainedAt); //zarz¹dzanie ¿yciami
		manageBullets(bulletsNum, screen, bullets); //zarz¹dzanie pociskami - ich iloœci¹, czy mo¿na strzeliæ itd.

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);

		while (SDL_PollEvent(&event)!=0) { //pêtla naszego dzia³ania - zczytywanie z klawiatury
			switch (event.type) {
			case SDL_KEYDOWN: //zczytujemy wciœniête klawisze
				keyboardAction(event, quit, paused, ended, bullet, bulletsNum, positionX, enemySpeed, bishopSpeed, allySpeed, points, frames, pointsBanned, hearts, gameplayPoints, pointsStoppedAt, worldTime, distance, speed, distanceOnRoad, worldTimeSave, worldTimeBantime, worldTimeFlag, worldTimeSave2, worldTimeSave3, vehicle, now, loading, sidePosition, counter, saved, direction, tryb, save1, save2, save3, save4, save5, heartGainedAt, boomX, boomY, boomTime);
				if (event.key.keysym.sym == SDLK_l) { //akcja wczytywania zapisu
					counter = loadCounter();
					char save[NUMBER_OF_SLOTS][SAVENAME_LENGTH+1];
					save1 = loadSave("SaveNames/save1", save[0]); save2 = loadSave("SaveNames/save2", save[1]); save3 = loadSave("SaveNames/save3", save[2]); save4 = loadSave("SaveNames/save4", save[3]); save5 = loadSave("SaveNames/save5", save[4]);
					keyboardLoading(loading, enemySpeed, allySpeed, bishopSpeed, worldTimeSave, worldTime, paused);
				}
				break;
				if (paused == 0 && ended == 0) {
			case SDL_KEYUP: //zczytujemy 'odklikniête' przyciski, mechanizm poruszania siê
				keyUpEvent(event,speed,tryb,direction);
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};
		scancodeEvent(paused,ended,currentKeyState,speed,tryb,direction,frames); //kontynuacja mechanizmu poruszania siê i strzelania
	};
	free_surfaces(charset, renderer, scrtex, window, screen, car, map, side, pause, enemy_car, ally_car, bishop, heart, inf_heart, sad_heart, game_lost, boom, bullets);	//zwolnienie powierzchni po wyœciu z gry

	return 0;
};
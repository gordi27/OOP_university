#include <windows.h>
#include <iostream>
#include "Header.h"
// определение кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
// объявление контекста устройства
HDC hdc;
// указатель на консольное окно
HWND GetConcolWindow();
Objects* objects[4];
bool Collision(Asteroid2* asteroid, Objects* Object)
{
	int xl = Object->Get_xr();
	int yl = Object->Get_yr();
	int xr = Object->Get_xl();
	int yr = Object->Get_yl();
	//определение столкновения слева
	bool left = xl <= asteroid->Get_xr() && xr >= asteroid->Get_xr()
		&& (yl <= asteroid->Get_yr() && yl >= asteroid->Get_yl()
			|| yr >= asteroid->Get_yl() && yr <= asteroid->Get_yr());
	//определение столкновений справа
	bool right = xr >= asteroid->Get_xl() && xl <= asteroid->Get_xl()
		&& (yl <= asteroid->Get_yr() && yl >= asteroid->Get_yl()
			|| yr >= asteroid->Get_yl() && yr <= asteroid->Get_yr());
	//определение столкновений сверху
	bool top = yl <= asteroid->Get_yl() && yr >= asteroid->Get_yl()
		&& (xl <= asteroid->Get_xr() && xl >= asteroid->Get_xl()
			|| xl >= asteroid->Get_xl() && xr <= asteroid->Get_xr());
	//определение столкновений снизу
	bool bottom = yr >= asteroid->Get_yr() && yl <= asteroid->Get_yr()
		&& (xl <= asteroid->Get_xr() && xl >= asteroid->Get_xl()
			|| xr >= asteroid->Get_xl() && xr <= asteroid->Get_xr());

	//определение того, находится ли препятствие "внутри" фигуры
	bool inside = xl >= asteroid->Get_xl() && yl >= asteroid->Get_yl() &&
		xr <= asteroid->Get_xr() && yr <= asteroid->Get_yr();
	//если хоть одно есть, то столкновение есть
	return left || right || top || bottom || inside;
}
int main()
{
	setlocale(LC_ALL, "Russian");
	// получение дескриптора консольного окна
	HWND hwnd = GetConcolWindow();
	hdc = GetDC(GetConsoleWindow());
	int matr_per[4][4] = {
		// Asteroid   FireAsteroid   DamagedAsteroid   MagicAsteroid
		{ 3,          3,             3,                3 },    // UFO
		{ 2,          2,             2,                2 },    // Planet
		{ 1,          1,             1,                1 },    // Star
		{ 0,          0,             0,                0 }     // MiniAsteroid
	};
	/*
	При столкновении с UFO (индекс 0) появляется MagicAsteroid (индекс 3)
	При столкновении с Planet (индекс 1) появляется DamagedAsteroid (индекс 2)
	При столкновении с Star (индекс 2) появляется FireAsteroid (индекс 1) // FireFine
	При столкновении с MiniAsteroid (индекс 3) появляется Asteroid (индекс 0)
	*/
	//Point BPoint(650, 650);
	if (hwnd != NULL)
	{
		if (hdc != 0)
		{
			int X = 650;
			int Y = 500;
			int delta = 150;
			Asteroid2 AAsteroid(X, Y, delta);
			FireAsteroid2 FAsteroid(X, Y, delta);
			DamagedAsteroid2 DAsteroid(X, Y, delta);
			MagicAsteroid2 MAsteroid(X, Y, delta);
			//FireFine FIAsteroid(X, Y, delta);
			// массив указателей на астероиды
			Asteroid2* asteroids[4] = { &AAsteroid, &FAsteroid, &DAsteroid, &MAsteroid };
			Asteroid2* pAsteroid;
			pAsteroid = &AAsteroid;
			UFO AUFO(1350, 650);
			Planet APlanet(200, 550, 100);
			Star AStar(700, 170, 53);
			MiniAsteroid AMiniAsteroid(700, 800, 50);
			objects[0] = &AUFO;
			objects[1] = &APlanet;
			objects[2] = &AStar;
			objects[3] = &AMiniAsteroid;
			int Step = 40;
			int FigX, FigY;
			while (1)
			{
				Sleep(100);
				FigX = pAsteroid->GetX();
				FigY = pAsteroid->GetY();
				pAsteroid->Show();
				for (int i = 0; i < 4; i++)
				{
					objects[i]->Show();
				}
				if (KEY_DOWN(VK_ESCAPE))
				{
					break;
				}
				if (KEY_DOWN(VK_LEFT))
				{
					FigX -= Step;
					pAsteroid->MoveTo(FigX, FigY);
				}
				if (KEY_DOWN(VK_RIGHT))
				{
					FigX += Step;
					pAsteroid->MoveTo(FigX, FigY);

				}
				if (KEY_DOWN(VK_DOWN))
				{
					FigY += Step;
					pAsteroid->MoveTo(FigX, FigY);
				}
				if (KEY_DOWN(VK_UP))
				{
					FigY -= Step;
					pAsteroid->MoveTo(FigX, FigY);
				}
				for (int i = 0; i < 4; i++)
				{
					if (Collision(pAsteroid, objects[i]))
					{
						pAsteroid->Hide();
						pAsteroid = asteroids[matr_per[i][pAsteroid->getIndex()]]; // переход к новому объекту
						pAsteroid->MoveTo(X, Y);  // переход в исходную точку
					}
				}
			}
		}
	}
}
HWND GetConcolWindow()
{
	char str[128];
	// новая версия Windows
	LPWSTR title = (LPWSTR)"xxxxxxxxxxxxxxxxxx";
	// получение заголовка окна
	GetConsoleTitle((LPWSTR)str, sizeof((LPWSTR)str));
	// установка нового заголовка окна
	SetConsoleTitle(title);
	Sleep(100);
	// определение дескриптора окна
	HWND hwnd = FindWindow(NULL, (LPWSTR)title);
	//возвращение прежнего заголовка
	SetConsoleTitle((LPWSTR)str);
	// возвращение дескриптора окна
	return hwnd;
}


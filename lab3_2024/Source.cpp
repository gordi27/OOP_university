#include <windows.h>
#include "Header.h"
#include <cmath>
// глобальная переменная контекста устройства для рисования
extern HDC hdc;
// определение кода нажатой клавиши
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
// размерность массива конфликтных объектов
#define ARR_SIZE 4 
extern Objects* objects[4];
// ------------------------- class Location: методы -------------------------
// конструктор и деструктор
Location::Location(int InitX, int InitY)
{
	// координаты
	X = InitX;
	Y = InitY;
};
Location::~Location(void)
{
};
// получение координат x и y точки
int Location::GetX(void)
{
	return X;
};
int Location::GetY(void)
{
	return Y;
};
// -------------------------- class Point: методы --------------------------
// конструктор и деструктор
Point::Point(int InitX, int InitY) : Location(InitX, InitY)
{
	// скрытие точки изначально
	Visible = false;
}
Point::~Point(void)
{
}
// показ точки
void Point::Show(void)
{
	Visible = true;
	// рисование точки
	for (int dx = -1; dx <= 3; ++dx)
	{
		for (int dy = -1; dy <= 3; ++dy)
		{
			SetPixel(hdc, X + dx, Y + dy, RGB(255, 255, 255));
		}
	}
}
// скрытие точки 
void Point::Hide(void)
{
	Visible = false;
	// рисование точки
	for (int dx = -1; dx <= 3; ++dx)
	{
		for (int dy = -1; dy <= 3; ++dy)
		{
			SetPixel(hdc, X + dx, Y + dy, RGB(12, 12, 12));
		}
	}
}
// светимость точки
bool Point::IsVisible(void)
{
	return Visible;
};
// перемещение точки
void Point::MoveTo(int NewX, int NewY)
{
	// скрытие точки
	Hide();
	// замена координат точки
	X = NewX;
	Y = NewY;
	// показ точки на новом месте
	Show();
};
// перемещение по стрелкам
void Point::Drag(int Step)
{
	// новые координаты фигуры
	int FigX, FigY;
	// начальные координаты фигуры
	FigX = GetX();
	FigY = GetY();
	// бесконечный цикл перемещения точки
	while (1)
	{
		if (KEY_DOWN(VK_ESCAPE)) // esc - выход
			break;
		// выбор направления движения фигуры
		if (KEY_DOWN(VK_LEFT)) // стрелка влево
		{
			FigX = FigX - Step;
			MoveTo(FigX, FigY);
			Sleep(500); // задержка экрана на 500 милисекунд
		}
		if (KEY_DOWN(VK_RIGHT)) // стрелка вправо
		{
			FigX = FigX + Step;
			MoveTo(FigX, FigY);
			Sleep(500);
		}
		if (KEY_DOWN(VK_DOWN)) // стрелка вниз
		{
			FigY = FigY + Step;
			MoveTo(FigX, FigY);
			Sleep(500);
		}
		if (KEY_DOWN(VK_UP)) // стрелка вверх
		{
			FigY = FigY - Step;
			MoveTo(FigX, FigY);
			Sleep(500);
		}
	}
}
// -------------------------- class Asteroid: методы --------------------------
// конструктор и деструктор
Asteroid::Asteroid(int InitX, int InitY, int d) : Point(InitX, InitY)
{
	index = 0;
	delta = d;
	xr = InitX - delta;
	yr = InitY - delta;
	xl = InitX + delta * 3;
	yl = InitY + delta;
}

Asteroid::~Asteroid()
{
}
// показ фигуры
void Asteroid::Show()
{
	Visible = true;
	xr = X + delta * 3;
	yr = Y + delta;
	xl = X - delta;
	yl = Y - delta;
	// белая ручка
	HPEN white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, white_pen);
	// белая заливка 
	HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, white_brush);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// черная ручка
	HPEN black_pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hdc, black_pen);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(white_pen);
	DeleteObject(black_pen);
	DeleteObject(white_brush);
}
// скрытие фигуры
void Asteroid::Hide()
{
	Visible = false;
	// создание кисти с цветом фона RGB(12,12,12) для заливки
	HBRUSH background_brush = CreateSolidBrush(RGB(12, 12, 12));
	SelectObject(hdc, background_brush);
	// создание ручки с цветом фона RGB(12,12,12) для обводки
	HPEN background_pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	SelectObject(hdc, background_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	// удаление созданных объектов кисти и пера
	DeleteObject(background_brush);
	DeleteObject(background_pen);
}
// получение индекса
int Asteroid::getIndex()
{
	return index;
}
// получение крайних координат
int Asteroid::Get_xr()
{
	return xr;
}
int Asteroid::Get_yr()
{
	return yr;
}
int Asteroid::Get_xl()
{
	return xl;
}
int Asteroid::Get_yl()
{
	return yl;
}
// ------------------------ class FireAsteroid: методы ------------------------
// конструктор и деструктор
FireAsteroid::FireAsteroid(int InitX, int InitY, int d) : Asteroid(InitX, InitY, d)
{
	delta = d;
	index = 1;
}
FireAsteroid::~FireAsteroid()
{
}
// показ фигуры
void FireAsteroid::Show()
{
	Visible = true;
	xr = X + delta * 3;
	yr = Y + delta;
	xl = X - delta;
	yl = Y - delta;
	// красная заливка 
	HBRUSH red_brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, red_brush);
	// красная ручка
	HPEN red_pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	SelectObject(hdc, red_pen);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// доп. треугольники
	POINT points_4[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 150}, {X + 2 * delta, Y - delta + 75} };
	Polygon(hdc, points_4, 3);

	POINT points_5[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 180}, {X + 2 * delta, Y + delta - 70} };
	Polygon(hdc, points_5, 3);
	// белая заливка 
	HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, white_brush);
	// белая ручка
	HPEN white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, white_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// черная ручка
	HPEN black_pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hdc, black_pen);
	SelectObject(hdc, white_brush);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(black_pen);
	DeleteObject(white_pen);
	DeleteObject(white_brush);
	DeleteObject(red_pen);
	DeleteObject(red_brush);
}
// скрытие фигуры
void FireAsteroid::Hide()
{
	Visible = false;
	// создание кисти с цветом фона RGB(12,12,12) для заливки
	HBRUSH background_brush = CreateSolidBrush(RGB(12, 12, 12));
	SelectObject(hdc, background_brush);
	// создание ручки с цветом фона RGB(12,12,12) для обводки
	HPEN background_pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	SelectObject(hdc, background_pen);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// доп. треугольники
	POINT points_4[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 150}, {X + 2 * delta, Y - delta + 75} };
	Polygon(hdc, points_4, 3);
	POINT points_5[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 180}, {X + 2 * delta, Y + delta - 70} };
	Polygon(hdc, points_5, 3);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);

	DeleteObject(background_pen);
	DeleteObject(background_brush);
}


// ----------------------- class DamagedAsteroid: методы -----------------------
// конструктор и деструктор
DamagedAsteroid::DamagedAsteroid(int InitX, int InitY, int d) : Asteroid(InitX, InitY, d)
{
	delta = d;
	index = 2;
}
DamagedAsteroid::~DamagedAsteroid()
{
}
// показ фигуры
void DamagedAsteroid::Show()
{
	Visible = true;
	xr = X + delta * 3;
	yr = Y + delta;
	xl = X - delta;
	yl = Y - delta;
	// белая заливка фигур
	HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, white_brush);
	// белая ручка
	HPEN white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, white_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// черная ручка
	HPEN black_pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hdc, black_pen);
	// зигзаги - трещины
	POINT points1[] =
	{
		{X - delta + 125, Y - delta + 10},
		{X - delta + 110, Y - delta + 40},
		{X - delta + 120, Y - delta + 70},
		{X - delta + 100, Y - delta + 90},
	};
	Polyline(hdc, points1, sizeof(points1) / sizeof(points1[0]));
	POINT points2[] =
	{
		{X + delta - 115, Y + delta - 10},
		{X + delta - 100, Y + delta - 30},
		{X + delta - 120, Y + delta - 40},
		{X + delta - 100, Y + delta - 60},
	};
	Polyline(hdc, points2, sizeof(points2) / sizeof(points2[0]));
	POINT points3[] =
	{
		{X - delta + 43, Y + delta - 50},
		{X - delta + 70, Y + delta - 65},
		{X - delta + 50, Y + delta - 95},
		{X - delta + 80, Y + delta - 115},
	};
	Polyline(hdc, points3, sizeof(points3) / sizeof(points3[0]));
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(white_pen);
	DeleteObject(white_brush);
	DeleteObject(black_pen);
}
// скрытие фигуры
void DamagedAsteroid::Hide()
{
	Visible = false;
	// создание кисти с цветом фона RGB(12,12,12) для заливки
	HBRUSH background_brush = CreateSolidBrush(RGB(12, 12, 12));
	SelectObject(hdc, background_brush);
	// создание ручки с цветом фона RGB(12,12,12) для обводки
	HPEN background_pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	SelectObject(hdc, background_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// зигзаги - трещины
	POINT points1[] =
	{
		{X - delta + 125, Y - delta + 10},
		{X - delta + 110, Y - delta + 40},
		{X - delta + 120, Y - delta + 70},
		{X - delta + 100, Y - delta + 90},
	};
	Polyline(hdc, points1, sizeof(points1) / sizeof(points1[0]));
	POINT points2[] =
	{
		{X + delta - 115, Y + delta - 10},
		{X + delta - 100, Y + delta - 30},
		{X + delta - 120, Y + delta - 40},
		{X + delta - 100, Y + delta - 60},
	};
	Polyline(hdc, points2, sizeof(points2) / sizeof(points2[0]));
	POINT points3[] =
	{
		{X - delta + 43, Y + delta - 50},
		{X - delta + 70, Y + delta - 65},
		{X - delta + 50, Y + delta - 95},
		{X - delta + 80, Y + delta - 115},
	};
	Polyline(hdc, points3, sizeof(points3) / sizeof(points3[0]));
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(background_pen);
	DeleteObject(background_brush);
}

// ----------------------- class MagicAsteroid: методы -----------------------

// конструктор и деструктор
MagicAsteroid::MagicAsteroid(int InitX, int InitY, int d) : Asteroid(InitX, InitY, d)
{
	delta = d;
	index = 3;
}
MagicAsteroid::~MagicAsteroid()
{
}
// показ фигуры
void MagicAsteroid::Show()
{
	Visible = true;
	xr = X + delta * 3;
	yr = Y + delta;
	xl = X - delta;
	yl = Y - delta;
	HPEN purple_pen = CreatePen(PS_SOLID, 2, RGB(193, 157, 250));
	SelectObject(hdc, purple_pen);
	HBRUSH purple_brush = CreateSolidBrush(RGB(193, 157, 250));
	SelectObject(hdc, purple_brush);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// розовая ручка и заливка
	HPEN pink_pen = CreatePen(PS_SOLID, 2, RGB(140, 41, 120));
	SelectObject(hdc, pink_pen);
	HBRUSH pink_brush = CreateSolidBrush(RGB(245, 66, 158));
	SelectObject(hdc, pink_brush);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(purple_pen);
	DeleteObject(purple_brush);
	DeleteObject(pink_pen);
	DeleteObject(pink_brush);
}
// скрытие фигуры
void MagicAsteroid::Hide()
{
	Visible = false;
	// создание кисти с цветом фона RGB(12,12,12) для заливки
	HBRUSH background_brush = CreateSolidBrush(RGB(12, 12, 12));
	SelectObject(hdc, background_brush);
	// создание ручки с цветом фона RGB(12,12,12) для обводки
	HPEN background_pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	SelectObject(hdc, background_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };

	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);

	DeleteObject(background_pen);
	DeleteObject(background_brush);
}
// --------------------- class Objects: методы ---------------------
// формальный конструктор по умолчанию для создания массива
Objects::Objects() : Point(0, 0)
{
}
// конструктор и деструктор
Objects::Objects(int InitX, int InitY) : Point(InitX, InitY)
{
}
Objects::~Objects()
{
}
int Objects::getIndex()
{
	return index;
}
int Objects::Get_xr()
{
	return xr;
}
int Objects::Get_yr()
{
	return yr;
}
int Objects::Get_xl()
{
	return xl;
}
int Objects::Get_yl()
{
	return yl;
}
void Objects::Draw(HPEN pen)
{
}
void Objects::Hide()
{
	HPEN white = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	Draw(white);
}

void Objects::Show()
{
	HPEN black = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	Draw(black);
}
// ----------------------- class UFO: методы -----------------------
// конструктор и деструктор
UFO::UFO(int InitX, int InitY) : Objects(InitX, InitY)
{
	index = 0;
	// расчет крайних координат
	xr = InitX - 100 + 100;
	yr = InitY - 200 + 100;
	xl = InitX + 150 + 100;
	yl = InitY - 35 + 100;
}
UFO::~UFO()
{
}
// показ фигуры
void UFO::Draw(HPEN Pen)
{
	// темно-зеленая ручка
	HPEN dark_green_pen = CreatePen(PS_SOLID, 3, RGB(26, 84, 42));
	SelectObject(hdc, dark_green_pen);
	// светло-зеленая кисть
	HBRUSH light_green_brush = CreateSolidBrush(RGB(109, 191, 133));
	SelectObject(hdc, light_green_brush);
	// круг сверху
	Ellipse(hdc, X + 80, Y - 100, X - 30, Y - 200);
	// зеленая кисть
	HBRUSH green_brush = CreateSolidBrush(RGB(86, 184, 113));
	SelectObject(hdc, green_brush);
	// овал снизу
	Ellipse(hdc, X - 100, Y - 150, X + 150, Y - 60);
	// ножки корабля
	POINT points1[] =
	{
		{X - 35, Y - 65},
		{X - 40, Y - 45},
	};
	Polyline(hdc, points1, sizeof(points1) / sizeof(points1[0]));
	POINT points2[] =
	{
		{X + 25, Y - 60},
		{X + 25, Y - 37},
	};
	Polyline(hdc, points2, sizeof(points2) / sizeof(points2[0]));
	POINT points3[] =
	{
		{X + 85, Y - 65},
		{X + 90, Y - 45},
	};
	Polyline(hdc, points3, sizeof(points3) / sizeof(points3[0]));
	DeleteObject(dark_green_pen);
	DeleteObject(green_brush);
	DeleteObject(light_green_brush);
}
// ---------------------- class Planet: методы ----------------------
// конструктор и деструктор
Planet::Planet(int InitX, int InitY, int r) : Objects(InitX, InitY)
{
	radius = r;
	index = 1;
	// расчет крайних координат
	xr = InitX - radius - 30;
	yr = InitY - radius - 30;
	xl = InitX + radius - 30;
	yl = InitY + radius - 30;
}
Planet::~Planet()
{
}
// показ фигуры
void Planet::Draw(HPEN Pen)
{
	// темно красная ручка
	HPEN dark_red_pen = CreatePen(PS_SOLID, 3, RGB(175, 76, 41));
	// красная кисть
	HBRUSH red_brush = CreateSolidBrush(RGB(209, 104, 68));
	SelectObject(hdc, dark_red_pen);
	SelectObject(hdc, red_brush);

	// большой круг
	Ellipse(hdc, X + radius, Y + radius, X - radius, Y - radius);
	// кольцо планеты
	POINT points1[] =
	{
		{X + radius, Y + radius},
		{X - radius, Y - radius},
	};
	Polyline(hdc, points1, sizeof(points1) / sizeof(points1[0]));
	DeleteObject(dark_red_pen);
	DeleteObject(red_brush);
}
// ----------------------- class Star: методы -----------------------
// конструктор и деструктор
Star::Star(int InitX, int InitY, int r) : Objects(InitX, InitY)
{
	radius = r;
	index = 2;
	// расчет крайних координат
	xr = InitX - radius - 20;
	yr = InitY - radius - 20;
	xl = InitX + radius - 20;
	yl = InitY + radius - 20;
}
Star::~Star()
{
}
// показ фигуры
void Star::Draw(HPEN Pen)
{
	// желтая ручка
	HPEN yellow_pen = CreatePen(PS_SOLID, 2, RGB(255, 214, 8));
	SelectObject(hdc, yellow_pen);
	// желтая заливка
	HBRUSH yellow_brush = CreateSolidBrush(RGB(255, 214, 8));
	SelectObject(hdc, yellow_brush);
	// координаты вершин звезды
	POINT points[10];
	double angle = 0;
	double angleStep = 2 * 3.14159265358979323846 / 5; // угол между вершинами
	for (int i = 0; i < 10; ++i)
	{
		double radius = (i % 2 == 0) ? 100 : 100 / 2; // чередующиеся радиусы для вершин
		points[i].x = X + radius * cos(angle);
		points[i].y = Y + radius * sin(angle);
		angle += angleStep;
	}
	// заполнение звезды с помощью круга
	Ellipse(hdc, X + radius, Y + radius, X - radius - 2, Y - radius - 2);
	// рисование звезды
	Polygon(hdc, points, 10);
	DeleteObject(yellow_pen);
	DeleteObject(yellow_brush);
}
// -------------------- class MiniAsteroid: методы --------------------
// конструктор и деструктор
MiniAsteroid::MiniAsteroid(int InitX, int InitY, int r) : Objects(InitX, InitY)
{
	radius = r;
	index = 3;
	// расчет крайних координат
	xr = InitX - radius + 20;
	yr = InitY - radius + 20;
	xl = InitX + radius + 20;
	yl = InitY + radius + 20;
}
MiniAsteroid::~MiniAsteroid()
{
}

// показ фигуры
void MiniAsteroid::Draw(HPEN Pen)
{
	// цвет мини астероида
	HBRUSH gray_brush = CreateSolidBrush(RGB(153, 169, 194));
	SelectObject(hdc, gray_brush);
	// ручка для обводки
	HPEN dark_pen = CreatePen(PS_SOLID, 2, RGB(68, 82, 102));
	SelectObject(hdc, dark_pen);
	// большой круг
	Ellipse(hdc, X - radius, Y - radius, X + radius, Y + radius);
	// цвет углублений
	HBRUSH dark_gray_brush = CreateSolidBrush(RGB(118, 133, 156));
	SelectObject(hdc, dark_gray_brush);
	// маленькие круги
	Ellipse(hdc, X + 30, Y + 30, X, Y);
	Ellipse(hdc, X - 30, Y - 15, X - 10, Y + 5);
	Ellipse(hdc, X, Y - 25, X + 15, Y - 10);
	// удаление созданных объектов кисти и пера
	DeleteObject(gray_brush);
	DeleteObject(dark_gray_brush);
	DeleteObject(dark_pen);
}
// -------------------------- интерфйес: главная ---------------------------------
Asteroid2::Asteroid2(int InitX, int InitY, int d) : Point(InitX, InitY)
{
	delta = d;
	index = 0;
	xr = InitX + delta * 3;
	yr = InitY + delta;
	xl = InitX - delta;
	yl = InitY - delta;
}
Asteroid2::~Asteroid2()
{
}
void Asteroid2::Show()
{
	Visible - true;
	tail();
	body();
	holes();
	xr = X + delta * 3;
	yr = Y + delta;
	xl = X - delta;
	yl = Y - delta;
}
// скрытие фигуры
void Asteroid2::Hide()
{
	Visible = false;
	// создание кисти с цветом фона RGB(12,12,12) для заливки
	HBRUSH background_brush = CreateSolidBrush(RGB(12, 12, 12));
	SelectObject(hdc, background_brush);
	// создание ручки с цветом фона RGB(12,12,12) для обводки
	HPEN background_pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	SelectObject(hdc, background_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	// удаление созданных объектов кисти и пера
	DeleteObject(background_brush);
	DeleteObject(background_pen);
}
void Asteroid2::body()
{
	// белая ручка
	HPEN white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, white_pen);
	// белая заливка 
	HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, white_brush);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	DeleteObject(white_pen);
	DeleteObject(white_brush);
}
void Asteroid2::tail()
{
	// белая ручка
	HPEN white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, white_pen);
	// белая заливка 
	HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, white_brush);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	DeleteObject(white_pen);
	DeleteObject(white_brush);
}
void Asteroid2::holes()
{
	// белая заливка 
	HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, white_brush);
	// черная ручка
	HPEN black_pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hdc, black_pen);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(black_pen);
	DeleteObject(white_brush);
}
//функции получения значений координат внешнего прямоугольника вокруг фигуры
int Asteroid2::Get_xr() {
	return xr;
}

int Asteroid2::Get_yr() {
	return yr;
}
int Asteroid2::Get_xl() {
	return xl;
}
int Asteroid2::Get_yl() {
	return yl;
}
int Asteroid2::getIndex() {
	return index;
}
// ----------------------- интерфейс: производные --------------------------
// конструктор и деструктор
FireAsteroid2::FireAsteroid2(int InitX, int InitY, int d) : Asteroid2(InitX, InitY, d)
{
	delta = d;
	index = 1;
}
FireAsteroid2::~FireAsteroid2()
{
}
// скрытие фигуры
void FireAsteroid2::Hide()
{
	Visible = false;
	// создание кисти с цветом фона RGB(12,12,12) для заливки
	HBRUSH background_brush = CreateSolidBrush(RGB(12, 12, 12));
	SelectObject(hdc, background_brush);
	// создание ручки с цветом фона RGB(12,12,12) для обводки
	HPEN background_pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	SelectObject(hdc, background_pen);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// доп. треугольники
	POINT points_4[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 150}, {X + 2 * delta, Y - delta + 75} };
	Polygon(hdc, points_4, 3);
	POINT points_5[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 180}, {X + 2 * delta, Y + delta - 70} };
	Polygon(hdc, points_5, 3);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(background_pen);
	DeleteObject(background_brush);
}
void FireAsteroid2::tail()
{
	// красная заливка 
	HBRUSH red_brush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, red_brush);
	// красная ручка
	HPEN red_pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	SelectObject(hdc, red_pen);

	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// доп. треугольники
	POINT points_4[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 150}, {X + 2 * delta, Y - delta + 75} };
	Polygon(hdc, points_4, 3);
	POINT points_5[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 180}, {X + 2 * delta, Y + delta - 70} };
	Polygon(hdc, points_5, 3);

	DeleteObject(red_pen);
	DeleteObject(red_brush);
}
// конструктор и деструктор
DamagedAsteroid2::DamagedAsteroid2(int InitX, int InitY, int d) : Asteroid2(InitX, InitY, d)
{
	delta = d;
	index = 2;
}
DamagedAsteroid2::~DamagedAsteroid2()
{
}

// скрытие фигуры
void DamagedAsteroid2::Hide()
{
	Visible = false;
	// создание кисти с цветом фона RGB(12,12,12) для заливки
	HBRUSH background_brush = CreateSolidBrush(RGB(12, 12, 12));
	SelectObject(hdc, background_brush);

	// создание ручки с цветом фона RGB(12,12,12) для обводки
	HPEN background_pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	SelectObject(hdc, background_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };

	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// зигзаги - трещины
	POINT points1[] =
	{
		{X - delta + 125, Y - delta + 10},
		{X - delta + 110, Y - delta + 40},
		{X - delta + 120, Y - delta + 70},
		{X - delta + 100, Y - delta + 90},
	};
	Polyline(hdc, points1, sizeof(points1) / sizeof(points1[0]));
	POINT points2[] =
	{
		{X + delta - 115, Y + delta - 10},
		{X + delta - 100, Y + delta - 30},
		{X + delta - 120, Y + delta - 40},
		{X + delta - 100, Y + delta - 60},
	};
	Polyline(hdc, points2, sizeof(points2) / sizeof(points2[0]));
	POINT points3[] =
	{
		{X - delta + 43, Y + delta - 50},
		{X - delta + 70, Y + delta - 65},
		{X - delta + 50, Y + delta - 95},
		{X - delta + 80, Y + delta - 115},
	};
	Polyline(hdc, points3, sizeof(points3) / sizeof(points3[0]));
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(background_pen);
	DeleteObject(background_brush);
}
void DamagedAsteroid2::body()
{
	// белая заливка фигур
	HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, white_brush);
	// белая ручка
	HPEN white_pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	SelectObject(hdc, white_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// черная ручка
	HPEN black_pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	SelectObject(hdc, black_pen);
	// зигзаги - трещины
	POINT points1[] =
	{
		{X - delta + 125, Y - delta + 10},
		{X - delta + 110, Y - delta + 40},
		{X - delta + 120, Y - delta + 70},
		{X - delta + 100, Y - delta + 90},
	};
	Polyline(hdc, points1, sizeof(points1) / sizeof(points1[0]));
	POINT points2[] =
	{
		{X + delta - 115, Y + delta - 10},
		{X + delta - 100, Y + delta - 30},
		{X + delta - 120, Y + delta - 40},
		{X + delta - 100, Y + delta - 60},
	};
	Polyline(hdc, points2, sizeof(points2) / sizeof(points2[0]));
	POINT points3[] =
	{
		{X - delta + 43, Y + delta - 50},
		{X - delta + 70, Y + delta - 65},
		{X - delta + 50, Y + delta - 95},
		{X - delta + 80, Y + delta - 115},
	};
	Polyline(hdc, points3, sizeof(points3) / sizeof(points3[0]));
	DeleteObject(white_pen);
	DeleteObject(white_brush);
	DeleteObject(black_pen);
}
// конструктор и деструктор
MagicAsteroid2::MagicAsteroid2(int InitX, int InitY, int d) : Asteroid2(InitX, InitY, d)
{
	delta = d;
	index = 3;
}
MagicAsteroid2::~MagicAsteroid2()
{
}
// скрытие фигуры
void MagicAsteroid2::Hide()
{
	Visible = false;
	// создание кисти с цветом фона RGB(12,12,12) для заливки
	HBRUSH background_brush = CreateSolidBrush(RGB(12, 12, 12));
	SelectObject(hdc, background_brush);
	// создание ручки с цветом фона RGB(12,12,12) для обводки
	HPEN background_pen = CreatePen(PS_SOLID, 2, RGB(12, 12, 12));
	SelectObject(hdc, background_pen);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);
	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(background_pen);
	DeleteObject(background_brush);
}
void MagicAsteroid2::body()
{
	HPEN purple_pen = CreatePen(PS_SOLID, 2, RGB(193, 157, 250));
	SelectObject(hdc, purple_pen);
	HBRUSH purple_brush = CreateSolidBrush(RGB(193, 157, 250));
	SelectObject(hdc, purple_brush);
	// большой круг
	Ellipse(hdc, X - delta, Y - delta, X + delta, Y + delta);
	// вершины треугольников
	POINT points_1[] = { {X + delta - 75, Y - delta + 50}, {X + 1.5 * delta - 100, Y - delta + 120}, {X + 2 * delta, Y - delta} };
	POINT points_2[] = { {X + delta - 50, Y + 60}, {X + 1.5 * delta - 85, Y - 50}, {X + 2 * delta + 140, Y} };
	POINT points_3[] = { {X + delta - 100, Y + delta - 55}, {X + 1.5 * delta - 140, Y + delta - 150}, {X + 2 * delta, Y + delta} };
	// треугольники
	Polygon(hdc, points_1, 3);
	Polygon(hdc, points_2, 3);
	Polygon(hdc, points_3, 3);

	DeleteObject(purple_pen);
	DeleteObject(purple_brush);
}
void MagicAsteroid2::holes()
{
	// розовая ручка и заливка
	HPEN pink_pen = CreatePen(PS_SOLID, 2, RGB(140, 41, 120));
	SelectObject(hdc, pink_pen);
	HBRUSH pink_brush = CreateSolidBrush(RGB(245, 66, 158));
	SelectObject(hdc, pink_brush);

	// маленькие круги
	Ellipse(hdc, X - 50, Y - 50, X + 20, Y + 20);
	Ellipse(hdc, X + 70, Y + 70, X + 30, Y + 30);
	Ellipse(hdc, X - 45, Y + 55, X + 5, Y + 105);
	Ellipse(hdc, X + 30, Y - 115, X + 55, Y - 90);
	Ellipse(hdc, X - 120, Y - 15, X - 95, Y + 10);
	DeleteObject(pink_pen);
	DeleteObject(pink_brush);
}

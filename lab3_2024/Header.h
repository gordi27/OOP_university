#pragma once
// типы данных
class Location // местоположение
{
	// предоставляет доступ производным классам к частным данным
protected:
	// координаты
	int X;
	int Y;
	// данные методы доступны извне всем в программе
public:
	// конструктор и деструктор
	Location(int InitX, int InitY);
	~Location();

	// получение координат X и Y точки
	int GetX();
	int GetY();
};

class Point : public Location // точка - производный класс от Location
{
	// классам, производным от Point, потребуется доступ public
protected:
	// светимость точки
	bool Visible;
public:
	// конструктор и деструктор
	Point(int InitX, int InitY);
	~Point();
	// светимость точки
	bool IsVisible();
	// перемещение фигуры
	void MoveTo(int NewX, int NewY);
	// движение фигуры по стрелкам
	void Drag(int Step);
	// позднее связывание
	//void Show();
	//void Hide();
	// показ точки
	virtual void Show();
	// скрытие точки
	virtual void Hide();
};
class Asteroid : public Point // астероид - производный от класса Point
{
protected:
	int delta;
	// крайние координаты
	int xr;
	int yr;
	int xl;
	int yl;
	int index;
public:
	// конструктор и деструктор
	Asteroid(int InitX, int InitY, int delta);
	~Asteroid();
	//// перемещение по стрелкам
	//void Drag(int step);
	// показ фигуры
	virtual void Show();
	// скрытие фигуры
	virtual void Hide();
	// void MoveTo(int NewX, int NewY);
	int Get_xr();
	int Get_yr();
	int Get_xl();
	int Get_yl();
	int getIndex();
};
// --------------------------- интерфейс: главная ----------------------------------
class IAsteroid1
{
public:
	virtual void tail() = 0;
	virtual void body() = 0;
	virtual void holes() = 0;
};
class Asteroid2 : public Point, IAsteroid1
{
protected:
	int delta;
	// крайние координаты
	int xr;
	int yr;
	int xl;
	int yl;
	int index;
public:
	// конструктор и деструктор
	Asteroid2(int InitX, int InitY, int delta);
	~Asteroid2();
	//// перемещение по стрелкам
	//void Drag(int step);
	// показ фигуры
	virtual void Show();
	// скрытие фигуры
	virtual void Hide();
	// void MoveTo(int NewX, int NewY);
	int Get_xr();
	int Get_yr();
	int Get_xl();
	int Get_yl();
	int getIndex();
	virtual void tail();
	virtual void body();
	virtual void holes();
	//bool Collision(Asteroid2& asteroid);
};
class Objects : public Point // объекты - производный от класса Location
{
protected:
	// крайние координаты
	int xr;
	int yr;
	int xl;
	int yl;
	int index;
protected:
	int radius;
public:
	Objects();
	Objects(int InitX, int InitY);
	~Objects();

	int Get_xr();
	int Get_yr();
	int Get_xl();
	int Get_yl();
	int getIndex();
	// определитель столкновения
	bool Collision(Asteroid2& asteroid, Objects* Object);
	void Hide();
	void Show();
	virtual void Draw(HPEN Pen);
};
class FireAsteroid : public Asteroid // огненный астероид - производный от Asteroid
{
public:
	FireAsteroid(int InitX, int InitY, int delta);
	~FireAsteroid();
	// показ фигуры
	virtual void Show();
	// скрытие фигуры
	virtual void Hide();
};
class DamagedAsteroid : public Asteroid // поврежденный астероид - производный от Asteroid
{
public:
	DamagedAsteroid(int InitX, int InitY, int delta);
	~DamagedAsteroid();
	// показ фигуры
	virtual void Show();
	// скрытие фигуры
	virtual void Hide();
};
class MagicAsteroid : public Asteroid // волшебный астероид - производный от Asteroid
{
public:
	MagicAsteroid(int InitX, int InitY, int delta);
	~MagicAsteroid();
	// показ фигуры
	virtual void Show();
	// скрытие фигуры
	virtual void Hide();
};
// -------------------------- объекты --------------------------------
class UFO : public Objects // НЛО - производный от Objects
{
public:
	UFO(int InitX, int InitY);
	~UFO();
	void Draw(HPEN pen);
};
class Planet : public Objects // планета - производный от Objects
{
	//protected:
	//	int radius;
public:
	Planet(int InitX, int InitY, int r);
	~Planet();
	void Draw(HPEN pen);
};
class Star : public Objects // звезда - производный от Objects
{
	//protected:
	//	int radius;
public:
	Star(int InitX, int InitY, int r);
	~Star();
	void Draw(HPEN pen);
};
class MiniAsteroid : public Objects // мини астероид - производный от Objects
{
	//protected:
	//	int radius;
public:
	MiniAsteroid(int InitX, int InitY, int r);
	~MiniAsteroid();
	void Draw(HPEN pen);
};
// ------------------------------ интерфейс: производные ----------------
class FireAsteroid2 : public Asteroid2 // огненный астероид - производный от Asteroid
{
public:
	FireAsteroid2(int InitX, int InitY, int delta);
	~FireAsteroid2();

	//// показ фигуры
	//virtual void Show();
	// скрытие фигуры
	virtual void Hide();

	// принудительный полиморфизм
	virtual void tail(); // красный огонь
};
class FireFine : public FireAsteroid2
{
public:
	FireFine(int InitX, int InitY, int delta);
	~FireFine();
	virtual void Hide();
	// принудительный полиморфизм
	virtual void holes(); // красные круги
};
class DamagedAsteroid2 : public Asteroid2 // поврежденный астероид - производный от Asteroid
{
public:
	DamagedAsteroid2(int InitX, int InitY, int delta);
	~DamagedAsteroid2();
	//// показ фигуры
	//virtual void Show();
	// скрытие фигуры
	virtual void Hide();
	// принудительный полиморфизм
	virtual void body(); // трещины
};
class MagicAsteroid2 : public Asteroid2 // волшебный астероид - производный от Asteroid
{
public:
	MagicAsteroid2(int InitX, int InitY, int delta);
	~MagicAsteroid2();
	//// показ фигуры
	//virtual void Show();
	// скрытие фигуры
	virtual void Hide();
	// принудительный полиморфизм
	virtual void body(); // тело
	virtual void holes(); // отверстия
};


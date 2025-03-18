#pragma once
// ���� ������
class Location // ��������������
{
	// ������������� ������ ����������� ������� � ������� ������
protected:
	// ����������
	int X;
	int Y;
	// ������ ������ �������� ����� ���� � ���������
public:
	// ����������� � ����������
	Location(int InitX, int InitY);
	~Location();

	// ��������� ��������� X � Y �����
	int GetX();
	int GetY();
};

class Point : public Location // ����� - ����������� ����� �� Location
{
	// �������, ����������� �� Point, ����������� ������ public
protected:
	// ���������� �����
	bool Visible;
public:
	// ����������� � ����������
	Point(int InitX, int InitY);
	~Point();
	// ���������� �����
	bool IsVisible();
	// ����������� ������
	void MoveTo(int NewX, int NewY);
	// �������� ������ �� ��������
	void Drag(int Step);
	// ������� ����������
	//void Show();
	//void Hide();
	// ����� �����
	virtual void Show();
	// ������� �����
	virtual void Hide();
};
class Asteroid : public Point // �������� - ����������� �� ������ Point
{
protected:
	int delta;
	// ������� ����������
	int xr;
	int yr;
	int xl;
	int yl;
	int index;
public:
	// ����������� � ����������
	Asteroid(int InitX, int InitY, int delta);
	~Asteroid();
	//// ����������� �� ��������
	//void Drag(int step);
	// ����� ������
	virtual void Show();
	// ������� ������
	virtual void Hide();
	// void MoveTo(int NewX, int NewY);
	int Get_xr();
	int Get_yr();
	int Get_xl();
	int Get_yl();
	int getIndex();
};
// --------------------------- ���������: ������� ----------------------------------
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
	// ������� ����������
	int xr;
	int yr;
	int xl;
	int yl;
	int index;
public:
	// ����������� � ����������
	Asteroid2(int InitX, int InitY, int delta);
	~Asteroid2();
	//// ����������� �� ��������
	//void Drag(int step);
	// ����� ������
	virtual void Show();
	// ������� ������
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
class Objects : public Point // ������� - ����������� �� ������ Location
{
protected:
	// ������� ����������
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
	// ������������ ������������
	bool Collision(Asteroid2& asteroid, Objects* Object);
	void Hide();
	void Show();
	virtual void Draw(HPEN Pen);
};
class FireAsteroid : public Asteroid // �������� �������� - ����������� �� Asteroid
{
public:
	FireAsteroid(int InitX, int InitY, int delta);
	~FireAsteroid();
	// ����� ������
	virtual void Show();
	// ������� ������
	virtual void Hide();
};
class DamagedAsteroid : public Asteroid // ������������ �������� - ����������� �� Asteroid
{
public:
	DamagedAsteroid(int InitX, int InitY, int delta);
	~DamagedAsteroid();
	// ����� ������
	virtual void Show();
	// ������� ������
	virtual void Hide();
};
class MagicAsteroid : public Asteroid // ��������� �������� - ����������� �� Asteroid
{
public:
	MagicAsteroid(int InitX, int InitY, int delta);
	~MagicAsteroid();
	// ����� ������
	virtual void Show();
	// ������� ������
	virtual void Hide();
};
// -------------------------- ������� --------------------------------
class UFO : public Objects // ��� - ����������� �� Objects
{
public:
	UFO(int InitX, int InitY);
	~UFO();
	void Draw(HPEN pen);
};
class Planet : public Objects // ������� - ����������� �� Objects
{
	//protected:
	//	int radius;
public:
	Planet(int InitX, int InitY, int r);
	~Planet();
	void Draw(HPEN pen);
};
class Star : public Objects // ������ - ����������� �� Objects
{
	//protected:
	//	int radius;
public:
	Star(int InitX, int InitY, int r);
	~Star();
	void Draw(HPEN pen);
};
class MiniAsteroid : public Objects // ���� �������� - ����������� �� Objects
{
	//protected:
	//	int radius;
public:
	MiniAsteroid(int InitX, int InitY, int r);
	~MiniAsteroid();
	void Draw(HPEN pen);
};
// ------------------------------ ���������: ����������� ----------------
class FireAsteroid2 : public Asteroid2 // �������� �������� - ����������� �� Asteroid
{
public:
	FireAsteroid2(int InitX, int InitY, int delta);
	~FireAsteroid2();

	//// ����� ������
	//virtual void Show();
	// ������� ������
	virtual void Hide();

	// �������������� �����������
	virtual void tail(); // ������� �����
};
class FireFine : public FireAsteroid2
{
public:
	FireFine(int InitX, int InitY, int delta);
	~FireFine();
	virtual void Hide();
	// �������������� �����������
	virtual void holes(); // ������� �����
};
class DamagedAsteroid2 : public Asteroid2 // ������������ �������� - ����������� �� Asteroid
{
public:
	DamagedAsteroid2(int InitX, int InitY, int delta);
	~DamagedAsteroid2();
	//// ����� ������
	//virtual void Show();
	// ������� ������
	virtual void Hide();
	// �������������� �����������
	virtual void body(); // �������
};
class MagicAsteroid2 : public Asteroid2 // ��������� �������� - ����������� �� Asteroid
{
public:
	MagicAsteroid2(int InitX, int InitY, int delta);
	~MagicAsteroid2();
	//// ����� ������
	//virtual void Show();
	// ������� ������
	virtual void Hide();
	// �������������� �����������
	virtual void body(); // ����
	virtual void holes(); // ���������
};


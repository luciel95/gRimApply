
// gRimApply.h: PROJECT_NAME 애플리케이션에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

class Circle
{
private :
	int _x;
	int _y;
	double _radius;
public:
	Circle()
	{
		_x = 0;
		_y = 0;
		_radius = 0.0;
	}
	void SetCircle(int x, int y, double radius)
	{
		_x = x;
		_y = y;
		_radius = radius;
	}
	int GetX()
	{
		return _x;
	}
	int GetY()
	{
		return _y;
	}
};

const double DOT_RAD = 2.0;
class Dots
{
private:
	int _dotCount;
	byte _dotColor;
	Circle _dot[3];

public:
	Dots()
	{
		_dotColor = 0x80;
		_dotCount = 0;
	}
	
	int GetCount()
	{
		return _dotCount;
	}
	Circle GetDot(int index)
	{
		if (index < 0 || index >= 3)
			return Circle();
		return _dot[index];
	}
	bool AddDot(int x, int y) 
	{
		if (_dotCount == 3)
			return false;
		_dot[_dotCount++].SetCircle(x, y, DOT_RAD);
		if (_dotCount == 3)
			return false; // limit reached.
		else
			return true;
	}
	void Clear()
	{
		_dotColor = 0x80;
		_dotCount = 0;
		_dot[0].SetCircle(0, 0, DOT_RAD);
		_dot[1].SetCircle(0, 0, DOT_RAD);
		_dot[2].SetCircle(0, 0, DOT_RAD);
	}
};
/// @brief 
class LineCircle : public Circle 
{
private:
	int _thick;
public:
	LineCircle()
	{
		_thick = 2;
		SetCircle(0, 0, 0.0);
	}
	LineCircle(int x, int y, double radius, int thick = 2)
	{
		_thick = thick;
		SetCircle(x, y, radius);
	}
	void SetThickness(int value) 
	{
		_thick = value;
	}

};

// CgRimApplyApp:
// 이 클래스의 구현에 대해서는 gRimApply.cpp을(를) 참조하세요.
//
class CgRimApplyApp : public CWinApp
{
public:
	CgRimApplyApp();

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.
	Dots _dots;
	LineCircle* _circle;

	bool AddDot(int x, int y);
	void CreateCircle(int x, int y, double radius, int thick);
	void DeleteCircle();

	DECLARE_MESSAGE_MAP()
};

extern CgRimApplyApp theApp;
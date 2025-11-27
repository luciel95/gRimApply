
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
		_x = 0;
		_y = 0;
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
	int _dotCount = 0;
	byte _dotColor;
	Circle Dot[3];

public:
	Dots()
	{
		;
	}

};

class LineCircle : public Circle 
{
private:
	int _thick;
public:
	LineCircle()
	{

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

	DECLARE_MESSAGE_MAP()
};

extern CgRimApplyApp theApp;
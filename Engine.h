#pragma once

#include "resource.h"
#include "Gun.h"
#include "Missile.h"
#include "Bomb.h"
#include "Target.h"

#define RESOLUTION_X 800
#define RESOLUTION_Y 600

class Engine
{
public:
	Engine();
	~Engine();

	HRESULT InitializeD2D(HWND m_hwnd);
	void KeyUp(WPARAM wParam);
	void KeyDown(WPARAM wParam);
	void MousePosition(int x, int y);
	void MouseButtonUp(bool left, bool right);
	void MouseButtonDown(bool left, bool right);
	void Logic(double elapsedTime);
	HRESULT Draw();

private:
	Gun* gun;
	Missile* missiles[100];
	int missileCount;

	double bombTimer;
	double bombDelay;
	int bombCount;
	double bombSpeed;
	Bomb* bombs[100];

	int targetCount;
	Target* targets[6];

	Point2D mousePos;
	bool leftClick;

	bool gameOver;

	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pWhiteBrush;
};


#pragma once

#include "resource.h"
#include "Point2D.h"

class Bomb
{
public:
	Bomb(double bSpeed);
	~Bomb();

	void InitializeD2D(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Advance(double elapsedTime);
	bool IsDead();

	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);

	Point2D GetPosition();
private:
	Point2D initial;
	Point2D position;
	Point2D destination;
	Point2D speed;

	ID2D1SolidColorBrush* m_pBlueBrush;
};
#pragma once

#include "resource.h"
#include "Point2D.h"

#define MISSILE_SPEED 400
#define MISSILE_EXPLOSION_SIZE 100

class Missile
{
public:
	Missile(Point2D destination);
	~Missile();

	void InitializeD2D(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Advance(double elapsedTime);
	bool IsDead();
	bool IsInsideExplosion(Point2D point);

	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);

	Point2D GetPosition();
private:
	Point2D position;
	Point2D destination;
	Point2D speed;

	int lifeStage;
	double explosionTime;

	ID2D1SolidColorBrush* m_pRedBrush;
};
#pragma once

#include "resource.h"
#include "Point2D.h"

class Target
{
public:
	Target(float positionX);
	~Target();

	void InitializeD2D(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Advance(double elapsedTime);
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Kill();

	bool IsAlive();
	float GetXPosition();
private:
	float position;
	boolean lifeStage;
	float explosionTime;

	ID2D1SolidColorBrush* m_pGreenBrush;
	ID2D1SolidColorBrush* m_pYellowBrush;
};
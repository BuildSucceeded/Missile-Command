#pragma once

#include "resource.h"
#include "Point2D.h"

class Gun
{
public:
	Gun();
	~Gun();

	void InitializeD2D(ID2D1HwndRenderTarget* m_pRenderTarget);
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget);
private:

	ID2D1SolidColorBrush* m_pGreenBrush;
};
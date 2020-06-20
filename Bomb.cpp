#include "framework.h"
#include "Point2D.h"
#include "Engine.h"
#include "Bomb.h"

Bomb::Bomb(double bSpeed) : m_pBlueBrush(NULL)
{
	int initialX = rand() % RESOLUTION_X;
	int finalX = rand() % 6;
	
	initial.x = initialX;
	initial.y = 0;
	
	position.x = initialX;
	position.y = 0;

	float step = RESOLUTION_X / 6;
	destination.x = finalX * step + step / 2;
	destination.y = RESOLUTION_Y;

	double distance = sqrt(pow(destination.x - position.x, 2) + pow(destination.y - position.y, 2));

	speed.x = bSpeed * (destination.x - position.x) / distance;
	speed.y = bSpeed * (destination.y - position.y) / distance;
}

Bomb::~Bomb()
{
	SafeRelease(&m_pBlueBrush);
}

void Bomb::InitializeD2D(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a red brush for drawing the bomb
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&m_pBlueBrush
	);
}

void Bomb::Advance(double elapsedTime)
{
	position.y += speed.y * elapsedTime;
	position.x += speed.x * elapsedTime;
}

bool Bomb::IsDead()
{
	return position.y > destination.y;
}

void Bomb::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{

	D2D1_POINT_2F point0 = D2D1::Point2F(initial.x, initial.y);
	D2D1_POINT_2F point1 = D2D1::Point2F(position.x, position.y);
	m_pRenderTarget->DrawLine(point0, point1, m_pBlueBrush, 3.0);


}

Point2D Bomb::GetPosition()
{
	return position;
}
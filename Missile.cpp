#include "framework.h"
#include "Point2D.h"
#include "Engine.h"
#include "Missile.h"

Missile::Missile(Point2D dest) : m_pRedBrush(NULL)
{
	// Resets the ship in the center of the screen
	position.x = RESOLUTION_X / 2;
	position.y = RESOLUTION_Y - 30;

	destination = dest;

	double distance = sqrt(pow(destination.x - position.x, 2) + pow(destination.y - position.y, 2));

	speed.x = MISSILE_SPEED * (destination.x - position.x) / distance;
	speed.y = MISSILE_SPEED * (destination.y - position.y) / distance;

	lifeStage = 1;
}

Missile::~Missile()
{
	SafeRelease(&m_pRedBrush);
}

void Missile::InitializeD2D(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a red brush for drawing the missile / explosion
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pRedBrush
	);
}

void Missile::Advance(double elapsedTime)
{
	if (lifeStage == 1) // If the missle is on its way to the destination
	{
		position.y += speed.y * elapsedTime;
		position.x += speed.x * elapsedTime;
		if (position.y < destination.y)
		{
			lifeStage = 2;
			explosionTime = 0;
		}
	}
	if (lifeStage == 2) // If the missle is exploding (growing)
	{
		explosionTime += 0.5 * elapsedTime;
		if (explosionTime > 0.5)
		{
			lifeStage = 3;
		}
	}
	if (lifeStage == 3) // If the missle is exploding (shrinking)
	{
		explosionTime -= 0.5 * elapsedTime;
		if (explosionTime < 0)
		{
			lifeStage = 4;
		}
	}
}

bool Missile::IsDead()
{
	return lifeStage > 3;
}

bool Missile::IsInsideExplosion(Point2D point)
{
	if (lifeStage == 2 || lifeStage == 3)
	{
		double distance = sqrt(pow(point.x - position.x, 2) + pow(point.y - position.y, 2));
		if (distance < explosionTime * MISSILE_EXPLOSION_SIZE)
		{
			return true;
		}
	}
	return false;
}

void Missile::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	if (lifeStage == 1)
	{
		D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
			D2D1::Point2F(position.x, position.y),
			4, 4
		);
		m_pRenderTarget->FillEllipse(&ellipseBall, m_pRedBrush);
	}

	if (lifeStage == 2 || lifeStage == 3)
	{
		D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
			D2D1::Point2F(position.x, position.y),
			explosionTime * MISSILE_EXPLOSION_SIZE, explosionTime * MISSILE_EXPLOSION_SIZE
		);
		m_pRenderTarget->FillEllipse(&ellipseBall, m_pRedBrush);
	}

}

Point2D Missile::GetPosition()
{
	return position;
}
#include "framework.h"
#include "Point2D.h"
#include "Engine.h"
#include "Target.h"

Target::Target(float positionX) : m_pGreenBrush(NULL), m_pYellowBrush(NULL)
{
	position = positionX;
	lifeStage = 1;
}

Target::~Target()
{
	SafeRelease(&m_pGreenBrush);
	SafeRelease(&m_pYellowBrush);
}

void Target::InitializeD2D(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a green brush for drawing the target
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::LightGreen),
		&m_pGreenBrush
	);
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow),
		&m_pYellowBrush
	);
}

void Target::Advance(double elapsedTime) {
	if (lifeStage == 2)
	{
		explosionTime += elapsedTime;
		if (explosionTime > 0.5)
		{
			lifeStage = 3; // Dead mode
		}
	}
}

void Target::Kill()
{
	lifeStage = 2; // Set it to "explosion mode"
	explosionTime = 0;
}

void Target::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	if (lifeStage == 1)
	{
		D2D1_POINT_2F p1 = D2D1::Point2F(position - 20, RESOLUTION_Y - 20);
		D2D1_POINT_2F p2 = D2D1::Point2F(position - 20, RESOLUTION_Y);
		D2D1_POINT_2F p3 = D2D1::Point2F(position + 20, RESOLUTION_Y);
		D2D1_POINT_2F p4 = D2D1::Point2F(position + 20, RESOLUTION_Y - 20);

		ID2D1PathGeometry* clPath;
		ID2D1Factory* factory;
		m_pRenderTarget->GetFactory(&factory);
		factory->CreatePathGeometry(&clPath);

		ID2D1GeometrySink* pclSink;
		clPath->Open(&pclSink);
		pclSink->SetFillMode(D2D1_FILL_MODE_WINDING);
		pclSink->BeginFigure(p1, D2D1_FIGURE_BEGIN_FILLED);
		pclSink->AddLine(p2);
		pclSink->AddLine(p3);
		pclSink->AddLine(p4);
		pclSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		pclSink->Close();
		m_pRenderTarget->FillGeometry(clPath, m_pGreenBrush);

		SafeRelease(&pclSink);
		SafeRelease(&clPath);
	}
	if (lifeStage == 2)
	{
		// In case of an explosion, we draw 9 points moving away from the center
		int angleStep = 360 / 18;
		for (int i = 0; i < 18; i++)
		{
			D2D1_ELLIPSE ellipseBall = D2D1::Ellipse(
				D2D1::Point2F(position + (explosionTime * 100 * sin(i * angleStep * 3.14 / 180)), RESOLUTION_Y - (explosionTime * 100  * cos(i * angleStep * 3.14 / 180))),
				4, 4
			);
			m_pRenderTarget->FillEllipse(&ellipseBall, m_pYellowBrush);
		}
	}

}

bool Target::IsAlive()
{
	return lifeStage == 1;
}

float Target::GetXPosition()
{
	return position;
}
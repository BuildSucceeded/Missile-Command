#include "framework.h"
#include "Point2D.h"
#include "Engine.h"
#include "Gun.h"

Gun::Gun() : m_pGreenBrush(NULL)
{
}

Gun::~Gun()
{
	SafeRelease(&m_pGreenBrush);
}

void Gun::InitializeD2D(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// Creates a green brush for drawing the ship
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Green),
		&m_pGreenBrush
	);
}

void Gun::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	
	// Calculate the head position and the 2 sides based on position and rotation
	D2D1_POINT_2F headPoint = D2D1::Point2F(RESOLUTION_X / 2, RESOLUTION_Y - 30);
	D2D1_POINT_2F leftPoint = D2D1::Point2F(RESOLUTION_X / 2 - 20, RESOLUTION_Y);
	D2D1_POINT_2F rightPoint = D2D1::Point2F(RESOLUTION_X / 2 + 20, RESOLUTION_Y);

	ID2D1PathGeometry* clPath;
	ID2D1Factory* factory;
	m_pRenderTarget->GetFactory(&factory);
	factory->CreatePathGeometry(&clPath);

	ID2D1GeometrySink* pclSink;
	clPath->Open(&pclSink);
	pclSink->SetFillMode(D2D1_FILL_MODE_WINDING);
	pclSink->BeginFigure(headPoint, D2D1_FIGURE_BEGIN_FILLED);
	pclSink->AddLine(leftPoint);
	pclSink->AddLine(rightPoint);
	pclSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	pclSink->Close();
	m_pRenderTarget->FillGeometry(clPath, m_pGreenBrush);

	SafeRelease(&pclSink);
	SafeRelease(&clPath);

}
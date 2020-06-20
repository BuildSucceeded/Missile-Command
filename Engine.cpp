#include "framework.h"
#include "Gun.h"
#include "Engine.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Windowscodecs.lib")

Engine::Engine() : m_pDirect2dFactory(NULL), m_pRenderTarget(NULL)
{
    // Constructor
    // Initialize your game elements here

    gun = new Gun();

    missileCount = 0;

    bombCount = 0;
    bombTimer = 0;
    bombDelay = 2;
    bombSpeed = 30;

    targetCount = 6;
    float step = RESOLUTION_X / 6;
    for (int i = 0; i < 6; i++) {
        float posX = i * step + step / 2;
        targets[i] = new Target(posX);
    }
}

Engine::~Engine()
{
    // Destructor

    SafeRelease(&m_pDirect2dFactory);
    SafeRelease(&m_pRenderTarget);

    // Safe-release your game elements here
    delete gun;
}

HRESULT Engine::InitializeD2D(HWND m_hwnd)
{
    // Initializes Direct2D, for drawing
    D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
    m_pDirect2dFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
        &m_pRenderTarget
    );

    // Initialize text writing factory and format
    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(m_pDWriteFactory),
        reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
    );

    m_pDWriteFactory->CreateTextFormat(
        L"Verdana",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        60,
        L"", //locale
        &m_pTextFormat
    );

    m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

    m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    m_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::White),
        &m_pWhiteBrush
    );

    // Initialize the D2D part of your game elements here
    gun->InitializeD2D(m_pRenderTarget);

    for (int i = 0; i < 6; i++) {
        targets[i]->InitializeD2D(m_pRenderTarget);
    }

    return S_OK;
}

void Engine::KeyUp(WPARAM wParam)
{
    // If keyup, un-set the keys flags
    // Don't do any logic here, you want to control the actual logic in the Logic method below
}

void Engine::KeyDown(WPARAM wParam)
{
    // If keyup, set the keys flags
    // Don't do any logic here, you want to control the actual logic in the Logic method below
}

void Engine::MousePosition(int x, int y)
{
    // Campture mouse position when the mouse moves
    // Don't do any logic here, you want to control the actual logic in the Logic method below
    mousePos.x = x;
    mousePos.y = y;
}

void Engine::MouseButtonUp(bool left, bool right)
{
    // If mouse button is released, set the mouse flags
    // Don't do any logic here, you want to control the actual logic in the Logic method below
    if (left)
    {
        leftClick = true;
    }
}

void Engine::MouseButtonDown(bool left, bool right)
{
    // If mouse button is pressed, set the mouse flags
    // Don't do any logic here, you want to control the actual logic in the Logic method below
}

void Engine::Logic(double elapsedTime)
{
    // This is the logic part of the engine. Add your game logic here.
    // It runs every frame and it receives the elapsed time between the last frame and this one, in seconds.
    // Use this value for a smooth and consistent movement, regardless of the CPU or graphics speed

    // Firing
    if (leftClick)
    {
        if (missileCount < 100)
        {
            Missile* missile = new Missile(mousePos);
            missile->InitializeD2D(m_pRenderTarget);
            missiles[missileCount] = missile;
            missileCount++;
        }
        leftClick = false;
    }

    // Target logic
    for (int i = 0; i < 6; i++) {
        targets[i]->Advance(elapsedTime);
    }

    // Missile logic
    for (int i = 0; i < missileCount; i++)
    {
        missiles[i]->Advance(elapsedTime);
        if (missiles[i]->IsDead())
        {
            Missile* missile = missiles[i];
            for (int j = i; j < missileCount - 1; j++)
            {
                missiles[j] = missiles[j + 1];
            }
            missileCount--;
            delete missile;
            i--;
        }
    }

    // Bomb logic
    bombTimer += elapsedTime;
    if (bombTimer > bombDelay)
    {
        bombTimer = 0;
        Bomb* bomb = new Bomb(bombSpeed);
        bomb->InitializeD2D(m_pRenderTarget);
        bombs[bombCount] = bomb;
        bombCount++;
    }
    for (int i = 0; i < bombCount; i++)
    {
        bombs[i]->Advance(elapsedTime);
        if (bombs[i]->IsDead())
        {
            Bomb* bomb = bombs[i];
            for (int j = i; j < bombCount - 1; j++)
            {
                bombs[j] = bombs[j + 1];
            }
            bombCount--;
            delete bomb;
            i--;
        }
    }

    // Bomb - missile interaction
    for (int i = 0; i < missileCount; i++)
    {
        Missile* missile = missiles[i];
        for (int j = 0; j < bombCount; j++)
        {
            Bomb* bomb = bombs[j];

            if (missile->IsInsideExplosion(bomb->GetPosition()))
            {
                for (int k = j; k < bombCount - 1; k++)
                {
                    bombs[k] = bombs[k + 1];
                }
                bombCount--;
                delete bomb;
                j--;
                bombDelay *= 0.99;
                bombSpeed *= 1.01;
            }
        }
    }

    // Bomb - target interaction
    for (int i = 0; i < 6; i++)
    {
        Target* target = targets[i];
        if (target->IsAlive()) // Only if the target is not already dead
        {
            for (int j = 0; j < bombCount; j++)
            {
                Bomb* bomb = bombs[j];

                if (bomb->GetPosition().y > RESOLUTION_Y - 20) // If the bomb is near the target on he X axys
                {
                    if (abs(bomb->GetPosition().x - target->GetXPosition()) < 20) // If the bomb is near the target on he Y axys
                    {
                        target->Kill();
                        targetCount--;

                        if (targetCount <= 0) {
                            gameOver = true;
                        }
                    }
                }
            }
        }
    }
}

HRESULT Engine::Draw()
{
    // This is the drawing method of the engine.
    // It runs every frame

    // Draws the elements in the game using Direct2D
    HRESULT hr;

    m_pRenderTarget->BeginDraw();

    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


    m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

    // Below you can add drawing logic for your game elements
    gun->Draw(m_pRenderTarget);

    for (int i = 0; i < bombCount; i++)
    {
        bombs[i]->Draw(m_pRenderTarget);
    }

    for (int i = 0; i < missileCount; i++)
    {
        missiles[i]->Draw(m_pRenderTarget);
    }

    for (int i = 0; i < 6; i++) {
        targets[i]->Draw(m_pRenderTarget);
    }

    if (gameOver) {
        D2D1_RECT_F rectangle2 = D2D1::RectF(0, 0, RESOLUTION_X, RESOLUTION_X);

        
        m_pRenderTarget->DrawText(
            L"Game Over!",
            10,
            m_pTextFormat,
            rectangle2,
            m_pWhiteBrush
        );
    }
    
    hr = m_pRenderTarget->EndDraw();

    return S_OK;
}
#ifndef UNICODE
#define UNICODE
#endif 

#include "main.h"

uint32_t Debug_Color = 0xFFFFFF;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Engie";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"I miss the old kanye",
        WS_OVERLAPPEDWINDOW,

        CW_USEDEFAULT, CW_USEDEFAULT, SCRW, SCRH,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.\

    bool exit = false;

    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory
    );

    initialize();
    last = std::chrono::high_resolution_clock::now();
    ab = last;
    MSG msg;

    while (!exit)
    {
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) exit = true;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //ab = std::chrono::high_resolution_clock::now();

        // display our screenbuffer
        /*BITMAPINFOHEADER bmi = { sizeof(bmi), SCRW, -SCRH, 1, 32 };
        HDC dc = GetDC(hwnd);
        SetDIBitsToDevice(dc, 0, 0, SCRW, SCRH, 0, 0, 0u, SCRH, scrbuf,
            (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
        ReleaseDC(hwnd, dc);*/

        /*update(Debug_Color);*/
        //last = ab;
    }

    SAFE_RELEASE(pD2DFactory);

    return 0;
}

bool rotate;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        SCRW = LOWORD(lParam);
        SCRH = HIWORD(lParam);
        resize(SCRW, SCRH);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        ab = std::chrono::high_resolution_clock::now();
        deltaTime = (float)std::chrono::duration<double>(ab - last).count();
        last = ab;

        if (rotate) {
            fYaw += 0.25f * deltaTime;
        }

        RECT rc;
        GetClientRect(hwnd, &rc);
        ID2D1HwndRenderTarget* pRT = NULL;
        HRESULT hr = pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                hwnd,
                D2D1::SizeU(
                    rc.right - rc.left,
                    rc.bottom - rc.top)
            ),
            &pRT
        );

        if (SUCCEEDED(hr)) 
        {
            pRT->BeginDraw();
            pRT->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

            render(pRT, Debug_Color);

            hr = pRT->EndDraw();

            SAFE_RELEASE(pRT);
        }

        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        Debug_Color = 0xFF0000;
        return 0;
    }
    case WM_LBUTTONUP:
    {
        Debug_Color = 0xFFFFFF;
        return 0;
    }
    case WM_CHAR:
    {
        /*switch (wParam)
        {
        default:
            break;
        }*/
        
        return 0;
    }
    case WM_KEYDOWN:
    {
        double bcs;
        double d;

        vec3d vForward = Vector_Mul(vLookDir, 32.0f * deltaTime);

        switch (wParam)
        {
        case VK_DOWN:
            vCamera.y -= 16.0f * deltaTime;
            break;
        case VK_UP:
            vCamera.y += 16.0f * deltaTime;
            break;
        case VK_LEFT:
            vCamera.x -= 16.0f * deltaTime;
            break;
        case VK_RIGHT:
            vCamera.x += 16.0f * deltaTime;
            break;
        case 'a':
        case 'A':
            fYaw -= 2.0f * deltaTime;
            break;
        case 'd':
        case 'D':
            fYaw += 2.0f * deltaTime;
            break;
        case 'w':
        case 'W':
            vCamera = Vector_Add(vCamera, vForward);
            break;
        case 's':
        case 'S':
            vCamera = Vector_Sub(vCamera, vForward);
            break;
        case 'R':
            rotate = !rotate;
            break;
        default:
            break;
        }
        return 0;
    }
    case WM_KEYUP:
    {
        Debug_Color = 0xFFFFFF;
        return 0;
    }
    default: 
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

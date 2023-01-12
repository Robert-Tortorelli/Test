#include <Windows.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.Foundation.h>

using namespace winrt;
using namespace Windows::UI::Composition;
using namespace Windows::Foundation;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Initialize the WinRT runtime
    init_apartment();

    // Register the Window Class
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = DefWindowProcW;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"WinUI3Win32Sample";
    RegisterClassExW(&wcex);

    // Create the Window
    auto hwnd = CreateWindowExW(0, L"WinUI3Win32Sample", L"WinUI 3 Win32 DirectX Sample",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr,
        hInstance, nullptr);
    ShowWindow(hwnd, nCmdShow);

    // Create the Compositor
    Compositor compositor;
    auto visual = compositor.CreateSpriteVisual();
    visual.Size(Size(800, 600));
    visual.Brush(compositor.CreateColorBrush(Color{ 0xFF, 0x00, 0x00, 0xFF }));

    // Get the Window's target and attach the visual
    auto target = hwnd.as<IDesktopWindowTarget>();
    target.Compositor().CreateTargetForHwnd(hwnd).Root(visual);

    // Run the Message Loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

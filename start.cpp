#include <windows.h>
#include <gdiplus.h>
#include <iostream>

#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

int GetScreenWhiteColorPercent() {
    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 获取屏幕尺寸
    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);

    // 创建屏幕截图
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenX, screenY);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, screenX, screenY, hScreenDC, 0, 0, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

    // 分析屏幕截图中的白色像素
    Bitmap* bmp = new Bitmap(hBitmap, (HPALETTE)NULL);
    Color pixelColor;
    int whitePixels = 0;
    int totalPixels = screenX * screenY;

    for (int x = 0; x < bmp->GetWidth(); x++) {
        for (int y = 0; y < bmp->GetHeight(); y++) {
            bmp->GetPixel(x, y, &pixelColor);
            if (pixelColor.GetR() == 255 && pixelColor.GetG() == 255 && pixelColor.GetB() == 255) {
                whitePixels++;
            }
        }
    }

    delete bmp;
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
    GdiplusShutdown(gdiplusToken);

    std::cout << "Screen checked. White color percentage: " << (whitePixels * 100) / totalPixels << "%\n";
    return (whitePixels * 100) / totalPixels;
}

void StartProgramIfWhiteExceeds90Percent(bool& programStarted) {
    int whitePercent = GetScreenWhiteColorPercent();
    if (whitePercent > 90 && !programStarted) {
        // 白色占比超过90%，尝试启动程序
        std::cout << "White color exceeds 90%. Starting the program...\n";
        system(".\\Ginshin_start.exe");
        programStarted = true; // 标记程序已启动
        std::cout << "Program started!\n";
    }
    else {
        std::cout << "White color does not exceed 90%.\n";
    }
}

int main() {
    bool programStarted = false; // 用于记录程序是否已经启动
    while (true) {
        bool programStarted = false;
        StartProgramIfWhiteExceeds90Percent(programStarted);
        Sleep(500); // 暂停0.5秒
    }
    return 0;
}

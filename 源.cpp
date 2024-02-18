#include <windows.h>
#include <gdiplus.h>
#include <iostream>

#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

int GetScreenWhiteColorPercent() {
    // ��ʼ��GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // ��ȡ��Ļ�ߴ�
    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);

    // ������Ļ��ͼ
    HDC hScreenDC = GetDC(NULL);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenX, screenY);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, screenX, screenY, hScreenDC, 0, 0, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

    // ������Ļ��ͼ�еİ�ɫ����
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
        // ��ɫռ�ȳ���90%��������������
        std::cout << "White color exceeds 90%. Starting the program...\n";
        system(".\\Ginshin_start.exe");
        programStarted = true; // ��ǳ���������
        std::cout << "Program started!\n";
    }
    else {
        std::cout << "White color does not exceed 90%.\n";
    }
}

int main() {
    bool programStarted = false; // ���ڼ�¼�����Ƿ��Ѿ�����
    while (true) {
        bool programStarted = false;
        StartProgramIfWhiteExceeds90Percent(programStarted);
        Sleep(500); // ��ͣ0.5��
    }
    return 0;
}
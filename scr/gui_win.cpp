#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#pragma comment(lib, "Comctl32.lib")

#include <string>
#include <vector>
#include <sstream>

#include "stackcalc.h"
#include "history.h"

static const char* APP_CLASS = "CalcAppWnd";
static HWND hEditA, hEditB, hEditC, hEditD; // четыре поля для кватерниона (a,b,c,d), первые два же используются для комплексного (re,im)
static HWND hList, hStatus;
static StackCalc calc;
static HistorySet hist;

static std::string getText(HWND h) {
    int len = GetWindowTextLengthA(h);
    std::string s(len, '\0');
    if (len > 0) GetWindowTextA(h, s.data(), len + 1);
    return s;
}
static void setStatus(const std::string& m) { SendMessageA(hStatus, SB_SETTEXTA, 0, (LPARAM)m.c_str()); }
static void refreshList() {
    SendMessageA(hList, LB_RESETCONTENT, 0, 0);
    const auto& d = calc.data();
    for (size_t i = 0; i < d.size(); ++i) {
        std::ostringstream ss; ss << (i + 1) << "  " << d[i]->str();
        SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)ss.str().c_str());
    }
}

enum {
    ID_PUSH_COMPLEX = 1001,
    ID_PUSH_QUAT,
    ID_ADD, ID_SUB, ID_MUL, ID_DIV,
    ID_SAVE, ID_LOAD, ID_CLEAR
};

static void doPushComplex() {
    double re = 0, im = 0;
    try {
        re = std::stod(getText(hEditA)); // A = Re
        im = std::stod(getText(hEditB)); // B = Im
    } catch (...) { setStatus("Введите числа для Re и Im"); return; }
    calc.pushComplex(re, im);
    refreshList();
    if (!calc.lastError().empty()) setStatus(calc.lastError()); else setStatus("OK");
}

static void doPushQuat() {
    double a = 0, b = 0, c = 0, d = 0;
    try {
        a = std::stod(getText(hEditA));
        b = std::stod(getText(hEditB));
        c = std::stod(getText(hEditC));
        d = std::stod(getText(hEditD));
    } catch (...) { setStatus("Введите числа для a,b,c,d"); return; }
    calc.pushQuaternion(a, b, c, d);
    refreshList();
    if (!calc.lastError().empty()) setStatus(calc.lastError()); else setStatus("OK");
}

static void doBin(void (StackCalc::*op)()) {
    (calc.*op)();
    refreshList();
    if (!calc.lastError().empty()) setStatus(calc.lastError()); else setStatus("OK");
}

static void doSave() {
    char file[260] = { 0 };
    OPENFILENAMEA ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = "History (*.bin)\0*.bin\0All\0*.*\0";
    ofn.lpstrFile = file;
    ofn.nMaxFile = 260;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    if (!GetSaveFileNameA(&ofn)) return;
    std::ofstream f(file, std::ios::binary);
    if (!f) { setStatus("Не открыть файл"); return; }
    StackSnapshot snap;
    for (auto& p : calc.data()) snap.items.push_back(p->clone());
    snap.serialize(f);
    setStatus("Сохранено");
}

static void doLoad() {
    char file[260] = { 0 };
    OPENFILENAMEA ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = "History (*.bin)\0*.bin\0All\0*.*\0";
    ofn.lpstrFile = file;
    ofn.nMaxFile = 260;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (!GetOpenFileNameA(&ofn)) return;
    std::ifstream f(file, std::ios::binary);
    if (!f) { setStatus("Не открыть файл"); return; }
    StackSnapshot snap;
    if (!snap.deserialize(f)) { setStatus("Ошибка чтения"); return; }
    calc.clear();
    for (auto& p : snap.items) {
        if (p->identify() == "complex") {
            auto& z = static_cast<Complex&>(*p);
            calc.pushComplex(z.re(), z.im());
        } else {
            auto& q = static_cast<Quaternion&>(*p);
            calc.pushQuaternion(q.re(), q.i(), q.j(), q.k());
        }
    }
    refreshList();
    setStatus("Загружено");
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        INITCOMMONCONTROLSEX icc{ sizeof(icc), ICC_BAR_CLASSES };
        InitCommonControlsEx(&icc);

        // ряд 1: четыре подписи + четыре поля
        CreateWindowA("STATIC", "Re/a:", WS_CHILD | WS_VISIBLE, 10, 10, 50, 20, hWnd, nullptr, nullptr, nullptr);
        hEditA = CreateWindowA("EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER, 60, 10, 90, 22, hWnd, nullptr, nullptr, nullptr);

        CreateWindowA("STATIC", "Im/b:", WS_CHILD | WS_VISIBLE, 160, 10, 50, 20, hWnd, nullptr, nullptr, nullptr);
        hEditB = CreateWindowA("EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER, 210, 10, 90, 22, hWnd, nullptr, nullptr, nullptr);

        CreateWindowA("STATIC", "c:", WS_CHILD | WS_VISIBLE, 310, 10, 20, 20, hWnd, nullptr, nullptr, nullptr);
        hEditC = CreateWindowA("EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER, 330, 10, 90, 22, hWnd, nullptr, nullptr, nullptr);

        CreateWindowA("STATIC", "d:", WS_CHILD | WS_VISIBLE, 430, 10, 20, 20, hWnd, nullptr, nullptr, nullptr);
        hEditD = CreateWindowA("EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER, 450, 10, 90, 22, hWnd, nullptr, nullptr, nullptr);

        // ряд 2: кнопки действий
        CreateWindowA("BUTTON", "Push Complex", WS_CHILD | WS_VISIBLE, 10, 40, 120, 24, hWnd, (HMENU)ID_PUSH_COMPLEX, nullptr, nullptr);
        CreateWindowA("BUTTON", "Push Quaternion (a,b,c,d)", WS_CHILD | WS_VISIBLE, 140, 40, 200, 24, hWnd, (HMENU)ID_PUSH_QUAT, nullptr, nullptr);

        CreateWindowA("BUTTON", "ADD", WS_CHILD | WS_VISIBLE, 350, 40, 60, 24, hWnd, (HMENU)ID_ADD, nullptr, nullptr);
        CreateWindowA("BUTTON", "SUB", WS_CHILD | WS_VISIBLE, 415, 40, 60, 24, hWnd, (HMENU)ID_SUB, nullptr, nullptr);
        CreateWindowA("BUTTON", "MUL", WS_CHILD | WS_VISIBLE, 480, 40, 60, 24, hWnd, (HMENU)ID_MUL, nullptr, nullptr);
        CreateWindowA("BUTTON", "DIV", WS_CHILD | WS_VISIBLE, 545, 40, 60, 24, hWnd, (HMENU)ID_DIV, nullptr, nullptr);

        CreateWindowA("BUTTON", "Save", WS_CHILD | WS_VISIBLE, 610, 40, 60, 24, hWnd, (HMENU)ID_SAVE, nullptr, nullptr);
        CreateWindowA("BUTTON", "Load", WS_CHILD | WS_VISIBLE, 675, 40, 60, 24, hWnd, (HMENU)ID_LOAD, nullptr, nullptr);
        CreateWindowA("BUTTON", "Clear", WS_CHILD | WS_VISIBLE, 740, 40, 60, 24, hWnd, (HMENU)ID_CLEAR, nullptr, nullptr);

        // список стека
        hList = CreateWindowA("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
            10, 70, 790, 360, hWnd, nullptr, nullptr, nullptr);

        // статус-бар
        hStatus = CreateWindowExA(0, STATUSCLASSNAMEA, "", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, nullptr, nullptr, nullptr);
        int parts[2] = { 500, -1 };
        SendMessageA(hStatus, SB_SETPARTS, 2, (LPARAM)parts);
        setStatus("Готово");
        return 0;
    }
    case WM_SIZE: {
        SendMessageA(hStatus, WM_SIZE, 0, 0);
        RECT rc; GetClientRect(hWnd, &rc);
        int sh = 22;
        MoveWindow(hList, 10, 70, rc.right - 20, rc.bottom - 70 - sh, TRUE);
        return 0;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_PUSH_COMPLEX: doPushComplex(); break;
        case ID_PUSH_QUAT:    doPushQuat();    break;
        case ID_ADD:          doBin(&StackCalc::add); break;
        case ID_SUB:          doBin(&StackCalc::sub); break;
        case ID_MUL:          doBin(&StackCalc::mul); break;
        case ID_DIV:          doBin(&StackCalc::div); break;
        case ID_SAVE:         doSave(); break;
        case ID_LOAD:         doLoad(); break;
        case ID_CLEAR:        calc.clear(); refreshList(); setStatus("Очищено"); break;
        }
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    WNDCLASSA wc{};
    wc.lpszClassName = APP_CLASS;
    wc.hInstance = hInst;
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    if (!RegisterClassA(&wc)) return 1;

    HWND hWnd = CreateWindowA(APP_CLASS, "StackCalc (Complex & Quaternion)", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 830, 520, nullptr, nullptr, hInst, nullptr);
    if (!hWnd) return 2;

    MSG msg;
    while (GetMessageA(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return (int)msg.wParam;
}

#include <windows.h>
#include <string>
#include <iostream>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "--gui") {
        return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLineA(), SW_SHOWDEFAULT);
    }
    std::cout << "Run with --gui\n";
    return 0;
}

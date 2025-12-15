# StackCalc (Комплексные числа и кватернионы) — GUI + CLI

Учебный стек-калькулятор для операций над **комплексными числами** и **кватернионами**.  
Два режима: **GUI на Win32** и **CLI**. Поддерживается сохранение/загрузка снимка стека.

---

## Сборка и запуск (Windows, MSVC, CMake)

### Быстрая команда (PowerShell)
```powershell
cd C:\programs\lab1; if (Test-Path build) { Remove-Item -Recurse -Force build }; cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release; cmake --build build --config Release; .\build\Release\app.exe --gui
````

### Запуск CLI

```powershell
cd C:\programs\lab1\build\Release; .\app.exe
```

> Проект автоматически линкует WinAPI-библиотеки (`Comctl32.lib`, `Comdlg32.lib`). Нужны: Visual Studio 2022 + Windows 10 SDK.

---

## Как пользоваться

### GUI

* Поля ввода:

  * Для комплексного: **Re** (действительная), **Im** (мнимая).
  * Для кватерниона: **a, b, c, d** (скаляра и три векторные компоненты).
* Кнопки:

  * **Push Complex** — добавить `Re + i·Im`.
  * **Push Quaternion (a,b,c,d)** — добавить `a + b·i + c·j + d·k`.
  * **ADD / SUB / MUL / DIV** — бинарные операции над вершинами стека (тип к типу).
  * **Save / Load** — сохранить / загрузить снимок стека (одним файлом).
  * **Clear** — очистить стек.
* Статус-бар внизу показывает результат/ошибку.

### CLI (команды)

```
pushc <re> <im>          ; добавить комплексное число
pushq <a> <b> <c> <d>    ; добавить кватернион
add | sub | mul | div    ; бинарные операции
top                      ; показать верх стека
list                     ; вывести весь стек
save <file.bin>          ; сохранить стек
load <file.bin>          ; загрузить стек
clear                    ; очистить стек
exit                     ; выход
```

---

## Мини-тест (пошагово)

### Комплексные

1. Сложение:

```
pushc 4 -3
pushc 23 -2
add
top
```

Ожидаем: **27 − 5i**

2. Умножение:

```
clear
pushc 1 2
pushc 3 4
mul
top
```

Ожидаем: **−5 + 10i**

3. Деление:

```
clear
pushc 1 1
pushc 2 -1
div
top
```

Ожидаем: **0.2 + 0.6i**

### Кватернионы

1. Сложение:

```
clear
pushq 1 2 3 4
pushq 5 6 7 8
add
top
```

Ожидаем: **6 + 8i + 10j + 12k**

2. Умножение (не коммутативно!):

```
clear
pushq 1 2 3 4
pushq 5 6 7 8
mul
top
```

Ожидаем: **−60 + 12i + 30j + 24k**

3. Деление (через обратный):

```
clear
pushq 1 0 0 0
pushq 2 0 0 0
div
top
```

Ожидаем: **0.5 + 0i + 0j + 0k**

---

## Формат файла истории (Save/Load)

`*.bin`:

* `uint64` — количество элементов
* для каждого элемента:

  * `char` — тип: `'c'` (complex) или `'q'` (quaternion)
  * `double` значения:

    * complex: `re`, `im`
    * quaternion: `a`, `b`, `c`, `d`

---

## Структура проекта

```
include/
  number.h        — базовый интерфейс Number (clone/str/identify/equals)
  complex.h       — класс Complex
  quaternion.h    — класс Quaternion
  stackcalc.h     — стек и операции (add/sub/mul/div, валидация)
  history.h       — StackSnapshot (глубокое копирование и (де)сериализация)
  assocset.h      — упрощённая ассоц. обёртка (ключ→снимок)

scr/
  complex.cpp
  quaternion.cpp
  stackcalc.cpp
  history.cpp
  gui_win.cpp     — Win32 GUI
  main.cpp        — CLI + опция `--gui`
```

---

## Частые ошибки и решения

* **C2280 / unique_ptr copy ctor deleted** — нельзя копировать `std::unique_ptr`.
  Исправлено: в `StackSnapshot` глубокое копирование через `clone()`, в остальных местах — перемещение/клонирование.

* **OPENFILENAMEA / GetOpenFileNameA / OFN_... не найден** — добавлен `<commdlg.h>` и линковка `Comdlg32.lib`.

* **STATUSCLASSNAMEA / SB_... не найден** — добавлен `<commctrl.h>`, вызван `InitCommonControlsEx`, линковка `Comctl32.lib`.

* **C4312 (int→HMENU)** — допустимое приведение для Win32. При желании подавить: `reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID))`.

* **Деление “ничего не выводит”** — валидация запрещает делить на нулевой модуль; смотри сообщение в статус-баре/CLI.

---

## План улучшений

* Контекстное меню списка: удалить/дублировать/переместить.
* Автоповтор последней операции (RPN-стиль).
* Юнит-тесты кватернионов (GoogleTest).
* Сериализация набора снимков по ключам.

```
```

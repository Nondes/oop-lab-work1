cmake_minimum_required(VERSION 3.16)
project(lab_build_any CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Папки (если какой-то нет — просто игнорируем)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(SCR_DIR "${CMAKE_SOURCE_DIR}/scr")
set(INC_DIR "${CMAKE_SOURCE_DIR}/include")

# Собираем все .cpp из src и scr
file(GLOB_RECURSE SRC_CPP
     "${SRC_DIR}/*.cpp"
     "${SCR_DIR}/*.cpp"
)

if(NOT SRC_CPP)
  message(FATAL_ERROR "Не найдено ни одного .cpp в папках src/ или scr/. Проверь имена папок и файлов.")
endif()

# Инклуды: include + scr (на случай, если заголовки лежат в scr)
include_directories(${INC_DIR} ${SCR_DIR})

# Главный exe
add_executable(app ${SRC_CPP})

# Если есть gui_win.cpp и мы на Windows — долинкуем comctl32
if(WIN32)
  foreach(f ${SRC_CPP})
    if(f MATCHES "gui_win\\.cpp$")
      target_link_libraries(app PRIVATE comctl32)
    endif()
  endforeach()
endif()

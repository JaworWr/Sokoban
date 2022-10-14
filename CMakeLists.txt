cmake_minimum_required(VERSION 3.23)
project(Sokoban C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(Sokoban
        Gtk_dialogs.c
        Level.c
        Level_draw.c
        Level_file.c
        main.c)

target_include_directories(Sokoban PUBLIC . ${GTK3_INCLUDE_DIRS})
target_link_libraries(Sokoban PUBLIC ${GTK3_LIBRARIES})
target_compile_options(Sokoban PUBLIC ${GTK3_CFLAGS_OTHER})

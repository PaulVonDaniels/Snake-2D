cmake_minimum_required(VERSION 3.10)
project(RaylibProject LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск raylib
find_package(raylib REQUIRED)

file(GLOB_RECURSE C_SOURCES 
    "*.c"
    "*.C"
)

file(GLOB_RECURSE CPP_SOURCES 
    "*.cpp" 
    "*.cxx"
    "*.cc"
    "*.c++"
    "*.C++"
)

# Исключаем файлы из папки build если она существует
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/build")
    list(FILTER C_SOURCES EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/build/.*")
    list(FILTER CPP_SOURCES EXCLUDE REGEX "${CMAKE_CURRENT_SOURCE_DIR}/build/.*")
endif()

# Выводим информацию о найденных файлах
message(STATUS "Found C sources: ${C_SOURCES}")
message(STATUS "Found C++ sources: ${CPP_SOURCES}")

# Автоматически добавляем все директории проекта в include path
file(GLOB_RECURSE INCLUDE_DIRS LIST_DIRECTORIES true "*")
list(FILTER INCLUDE_DIRS INCLUDE REGEX ".*/.*")
foreach(dir ${INCLUDE_DIRS})
    if(IS_DIRECTORY ${dir})
        include_directories(${dir})
    endif()
endforeach()

# Альтернативный простой способ - добавить корневую директорию и все поддиректории
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Проверяем, какие файлы найдены и создаем исполняемый файл
if(C_SOURCES OR CPP_SOURCES)
    if(C_SOURCES AND NOT CPP_SOURCES)
        # Только C файлы
        message(STATUS "Building as C project")
        add_executable(main ${C_SOURCES})
    elseif(CPP_SOURCES AND NOT C_SOURCES)
        # Только C++ файлы
        message(STATUS "Building as C++ project")
        add_executable(main ${CPP_SOURCES})
    else()
        # Смешанный проект
        message(STATUS "Building as mixed C/C++ project")
        add_executable(main ${C_SOURCES} ${CPP_SOURCES})
    endif()
else()
    message(FATAL_ERROR "No source files found in project directory")
endif()

# Линковка с raylib
target_link_libraries(main raylib)

# Для macOS: добавляем необходимые фреймворки
if(APPLE)
    target_link_libraries(main 
        "-framework OpenGL"
        "-framework Cocoa" 
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()
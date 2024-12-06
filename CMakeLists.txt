# CMake version minimum requise
cmake_minimum_required(VERSION 3.16)

# Projet principal C++ (HIVE)
project(HIVE)

set(CMAKE_CXX_STANDARD 14)

# Inclure les répertoires nécessaires
include_directories(.)
add_executable(HIVE
        main.cpp
        src/utils/hiveException.h
        src/utils/utils.cpp
        src/utils/utils.h
        src/features/insect.cpp
        src/features/insect.h
        src/features/map.cpp
        src/features/map.h
        src/features/deck.h
        src/features/inputs.h
        src/features/inputs.cpp
        src/inputsManager.h
        src/solver.h
        src/renderer.h
        src/hive.h
        src/hive.cpp
        src/features/inputsManager.cpp
)

# Projet Qt (TestQt)
project(TestQt VERSION 0.1 LANGUAGES CXX)

# Spécifier le chemin où le sous-module Qt a été cloné
set(CMAKE_PREFIX_PATH "Qt/CmakeQT")
# Configurer Qt pour le projet
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver Qt et ses composants nécessaires
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Fichiers sources pour le projet Qt
set(PROJECT_SOURCES
        Qt/main.cpp
        Qt/hiverenderer.cpp
        Qt/hiverenderer.h
        Qt/hiverenderer.ui
        Qt/hexagonalbutton.cpp
        Qt/hexagonalbutton.h
)

# Créer l'exécutable pour Qt
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TestQt
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
    )
else()
    add_executable(TestQt ${PROJECT_SOURCES})
endif()

# Lier Qt avec le projet Qt
target_link_libraries(TestQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Configuration des propriétés pour le bundle (macOS/Windows)
set_target_properties(TestQt PROPERTIES
        WIN32_EXECUTABLE TRUE
)

# Ajouter les étapes d'installation pour Qt
include(GNUInstallDirs)
install(TARGETS TestQt
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finaliser l'exécutable si nécessaire (Qt6)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TestQt)
endif()

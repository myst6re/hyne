###############################################################################
## Copyright (C) 2009-2024 Arzel Jérôme <myst6re@gmail.com>
## Copyright (C) 2020 Julian Xhokaxhiu <https://julianxhokaxhiu.com>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0010 NEW)
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0091 NEW)

option(ENABLE_JP_ENCODING "Enable JP encoding (needs QtCore5Compat)" ON)

set(VCPKG_DISABLE_COMPILER_TRACKING 1)
set(VCPKG_INSTALL_OPTIONS "--clean-after-build")
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

set(RELEASE_NAME "Hyne")
if(NOT PRERELEASE_STRING)
    set(PRERELEASE_STRING "")
endif()

project("Hyne" VERSION 1.11.4 LANGUAGES CXX
    DESCRIPTION "Final Fantasy VIII savegame editor"
    HOMEPAGE_URL "https://github.com/myst6re/hyne"
)

# Try to autodetect Qt path if not specified
include(qt.cmake)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_compile_definitions(
    QT_DISABLE_DEPRECATED_UP_TO=0x060000
    QT_DISABLE_DEPRECATED_BEFORE=0x060000
    #QT_RESTRICTED_CAST_FROM_ASCII
    HYNE_NAME="${PROJECT_NAME}"
    HYNE_DESCRIPTION="${PROJECT_DESCRIPTION}"
    HYNE_COMPANY="myst6re"
    HYNE_VERSION="${PROJECT_VERSION}${PRERELEASE_STRING}"
    HYNE_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    HYNE_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    HYNE_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    HYNE_VERSION_TWEAK=0
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
if(ENABLE_JP_ENCODING)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core5Compat REQUIRED)
endif()
find_package(ZLIB REQUIRED)

set(LANGS
    en
    ja
    zh)
list(TRANSFORM LANGS REPLACE ".+" "translations/${RELEASE_NAME}_\\0.ts" OUTPUT_VARIABLE TS_FILES)

set(PROJECT_SOURCES
    "src/Aes.cpp"
    "src/Aes.h"
    "src/Config.cpp"
    "src/Config.h"
    "src/CryptographicHash.cpp"
    "src/CryptographicHash.h"
    "src/Data.cpp"
    "src/Data.h"
    "src/Editor.cpp"
    "src/Editor.h"
    "src/FF8Installation.cpp"
    "src/FF8Installation.h"
    "src/FF8Text.cpp"
    "src/FF8Text.h"
    "src/FF8text_caract.cpp"
    "src/GZIP.cpp"
    "src/GZIP.h"
    "src/HeaderDialog.cpp"
    "src/HeaderDialog.h"
    "src/HelpWidget.cpp"
    "src/HelpWidget.h"
    "src/LZS.cpp"
    "src/LZS.h"
    "src/main.cpp"
    "src/MapWidget.cpp"
    "src/MapWidget.h"
    "src/Metadata.cpp"
    "src/Metadata.h"
    "src/MetadataDialog.cpp"
    "src/MetadataDialog.h"
    "src/PageWidgets/AllEditor.cpp"
    "src/PageWidgets/AllEditor.h"
    "src/PageWidgets/BattleEditor.cpp"
    "src/PageWidgets/BattleEditor.h"
    "src/PageWidgets/ConfigEditor.cpp"
    "src/PageWidgets/ConfigEditor.h"
    "src/PageWidgets/CWEditor.cpp"
    "src/PageWidgets/CWEditor.h"
    "src/PageWidgets/DrawPointEditor.cpp"
    "src/PageWidgets/DrawPointEditor.h"
    "src/PageWidgets/FieldEditor.cpp"
    "src/PageWidgets/FieldEditor.h"
    "src/PageWidgets/GfEditor.cpp"
    "src/PageWidgets/GfEditor.h"
    "src/PageWidgets/ItemEditor.cpp"
    "src/PageWidgets/ItemEditor.h"
    "src/PageWidgets/MiscEditor.cpp"
    "src/PageWidgets/MiscEditor.h"
    "src/PageWidgets/PageWidget.cpp"
    "src/PageWidgets/PageWidget.h"
    "src/PageWidgets/PartyEditor.cpp"
    "src/PageWidgets/PartyEditor.h"
    "src/PageWidgets/PersoEditor.cpp"
    "src/PageWidgets/PersoEditor.h"
    "src/PageWidgets/PreviewEditor.cpp"
    "src/PageWidgets/PreviewEditor.h"
    "src/PageWidgets/ShopEditor.cpp"
    "src/PageWidgets/ShopEditor.h"
    "src/PageWidgets/TTriadEditor.cpp"
    "src/PageWidgets/TTriadEditor.h"
    "src/PageWidgets/WorldmapEditor.cpp"
    "src/PageWidgets/WorldmapEditor.h"
    "src/QHexEdit/commands.cpp"
    "src/QHexEdit/commands.h"
    "src/QHexEdit/qhexedit.cpp"
    "src/QHexEdit/qhexedit.h"
    "src/QHexEdit/qhexedit_p.cpp"
    "src/QHexEdit/qhexedit_p.h"
    "src/QHexEdit/xbytearray.cpp"
    "src/QHexEdit/xbytearray.h"
    "src/QTaskBarButton.cpp"
    "src/QTaskBarButton.h"
    "src/SavecardData.cpp"
    "src/SavecardData.h"
    "src/SavecardView.cpp"
    "src/SavecardView.h"
    "src/SavecardWidget.cpp"
    "src/SavecardWidget.h"
    "src/SaveData.cpp"
    "src/SaveData.h"
    "src/SaveIcon.cpp"
    "src/SaveIcon.h"
    "src/SCHeaderDialog.cpp"
    "src/SCHeaderDialog.h"
    "src/SelectSavesDialog.cpp"
    "src/SelectSavesDialog.h"
    "src/Sha1.cpp"
    "src/Sha1.h"
    "src/SpinBox.cpp"
    "src/SpinBox.h"
    "src/SpinBoxDelegate.cpp"
    "src/SpinBoxDelegate.h"
    "src/StartWidget.cpp"
    "src/StartWidget.h"
    "src/TimeWidget.cpp"
    "src/TimeWidget.h"
    "src/UserDirectory.cpp"
    "src/UserDirectory.h"
    "src/Window.cpp"
    "src/Window.h"
)

if(APPLE)
    list(APPEND PROJECT_SOURCES
        "src/QTaskBarButton.mm"
    )
endif()

set(RESOURCES "src/qt/${RELEASE_NAME}.qrc")

if(APPLE)
    set(ICON_FILE "deploy/macosx/${RELEASE_NAME}.icns")
    set(EXTRA_RESOURCES_GUI ${ICON_FILE})

    set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
elseif (WIN32)
    set(ICON_FILE "src/qt/images/hyne.ico")
    set(EXTRA_RESOURCES_GUI "src/qt/${RELEASE_NAME}.rc")
endif()

qt_add_executable(${RELEASE_NAME} MANUAL_FINALIZATION MACOSX_BUNDLE WIN32 ${PROJECT_SOURCES} ${QM_FILES} ${RESOURCES} ${EXTRA_RESOURCES_GUI})
target_include_directories(${RELEASE_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(${RELEASE_NAME} PRIVATE
    Qt::Widgets
    ZLIB::ZLIB
)

if(${QT_VERSION_MAJOR} EQUAL 6)
    if(ENABLE_JP_ENCODING)
        target_link_libraries(${RELEASE_NAME} PRIVATE Qt6::Core5Compat)

        target_compile_definitions(${RELEASE_NAME}
            PRIVATE HYNE_ENABLE_JP_ENCODING=1
        )
    endif()

    target_compile_definitions(${RELEASE_NAME}
        PRIVATE TASKBAR_BUTTON=1
    )
endif()

set_target_properties(${RELEASE_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME "Hyne"
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    MACOSX_BUNDLE_GUI_IDENTIFIER org.myst6re.hyne
    MACOSX_BUNDLE_ICON_FILE ${RELEASE_NAME}.icns
)

if(APPLE OR WIN32)
    set_target_properties(${RELEASE_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
endif()

if (WIN32)
    target_compile_options(
        ${RELEASE_NAME}
        PRIVATE /Qpar
        PRIVATE /MP
    )
endif()

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR}/src ${TS_FILES} OPTIONS -no-ui-lines -locations none)
# Force lupdate
add_custom_target(app_translations ALL DEPENDS ${QM_FILES})

include(GNUInstallDirs)

if(APPLE)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "/Applications" CACHE PATH "..." FORCE)
    endif()
    if(NOT QT_DEPLOY_TMP_DIR)
        set(QT_DEPLOY_TMP_DIR "${CMAKE_BINARY_DIR}" CACHE PATH "Directory to run deployqt scripts")
    endif()
elseif(WIN32)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT AND ENV{ProgramFiles})
        # Force Program Files for x64 programs (CMake uses x86 by default)
        set(CMAKE_INSTALL_PREFIX "$ENV{ProgramFiles}/${PROJECT_NAME}" CACHE PATH "..." FORCE)
    endif()
    if(NOT QT_DEPLOY_TMP_DIR)
        set(QT_DEPLOY_TMP_DIR "${CMAKE_BINARY_DIR}/win32-deploy" CACHE PATH "Directory to run deployqt scripts")
    endif()
    install(TARGETS ${RELEASE_NAME} DESTINATION "${QT_DEPLOY_TMP_DIR}")
endif()

add_subdirectory(deploy)

if(APPLE)
    install(TARGETS ${RELEASE_NAME} BUNDLE DESTINATION ".")
elseif(WIN32)
    install(TARGETS ${RELEASE_NAME} RUNTIME DESTINATION ".")
    install(DIRECTORY "${QT_DEPLOY_TMP_DIR}/" DESTINATION ".")
    install(CODE "file(REMOVE_RECURSE \"${QT_DEPLOY_TMP_DIR}\")")
else()
    install(FILES ${CMAKE_SOURCE_DIR}/src/qt/images/Hyne.png DESTINATION share/icons/hicolor/256x256/apps RENAME io.github.myst6re.hyne.png)
    install(FILES ${CMAKE_SOURCE_DIR}/deploy/linux/io.github.myst6re.hyne.desktop DESTINATION share/applications)
    install(FILES ${QM_FILES} DESTINATION share/hyne/translations)
    install(TARGETS ${RELEASE_NAME} RUNTIME)
endif()

qt_finalize_executable(${RELEASE_NAME})

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CPACK_SYSTEM_NAME "macos")
    set(CPACK_GENERATOR "DragNDrop")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CPACK_GENERATOR "ZIP")
else()
    set(CPACK_GENERATOR "TXZ")
endif()

set(CPACK_DEBIAN_PACKAGE_NAME "hyne")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "myst6re@gmail.com")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt6-qpa-plugins")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_PACKAGE_NAME hyne)
set(CPACK_SOURCE_IGNORE_FILES ".git/*;*.user;*.yml;*.github/*")
set(CPACK_SOURCE_GENERATOR "ZIP;TGZ")

include(CPack)

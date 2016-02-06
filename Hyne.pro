TEMPLATE = app
win32 {
    TARGET = Hyne
} else {
    TARGET = hyne
}

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}
lessThan(QT_MAJOR_VERSION, 5) {
    INCLUDEPATH += compat
}

# include zlib
!win32 {
    LIBS += -lz
} else {
    exists($$[QT_INSTALL_PREFIX]/include/QtZlib) {
        INCLUDEPATH += $$[QT_INSTALL_PREFIX]/include/QtZlib
    } else {
        # INCLUDEPATH += zlib
        LIBS += -lz
    }
}

# QTaskbarButton
greaterThan(QT_MAJOR_VERSION, 4):qtHaveModule(winextras) {
    QT += winextras
} else {
    DEFINES += HYNE_TASKBAR_FAKE

    win32 {
        message(Hyne: Taskbar button overlay icon is only available with Qt Windows Extras)
    }
}

win32 {
    # Regedit features
    LIBS += -ladvapi32 -lshell32
}

# Input
HEADERS += PageWidgets/ConfigEditor.h \
    PageWidgets/MiscEditor.h \
    PageWidgets/GfEditor.h \
    PageWidgets/ItemEditor.h \
    PageWidgets/PersoEditor.h \
    PageWidgets/TTriadEditor.h \
    PageWidgets/CWEditor.h \
    PageWidgets/AllEditor.h \
    Data.h \
    Editor.h \
    FF8Text.h \
    LZS.h \
    Parameters.h \
    SavecardWidget.h \
    SaveIcon.h \
    Window.h \
    HeaderDialog.h \
    Config.h \
    SpinBoxDelegate.h \
    QHexEdit/qhexedit.h \
    QHexEdit/qhexedit_p.h \
    QHexEdit/xbytearray.h \
    QHexEdit/commands.h \
    SaveData.h \
    PageWidgets/PageWidget.h \
    PageWidgets/DrawPointEditor.h \
    PageWidgets/BattleEditor.h \
    PageWidgets/ShopEditor.h \
    SelectSavesDialog.h \
    PageWidgets/WorldmapEditor.h \
    MapWidget.h \
    PageWidgets/FieldEditor.h \
    StartWidget.h \
    PageWidgets/PartyEditor.h \
    GZIP.h \
    SavecardData.h \
    SavecardView.h \
    QTaskbarButton.h \
    PageWidgets/PreviewEditor.h \
    TimeWidget.h \
    HelpWidget.h \
    Metadata.h \
    UserDirectory.h \
    FF8Installation.h \
    MetadataDialog.h \
    SpinBox.h
SOURCES += PageWidgets/ConfigEditor.cpp \
    PageWidgets/MiscEditor.cpp \
    PageWidgets/GfEditor.cpp \
    PageWidgets/ItemEditor.cpp \
    PageWidgets/PersoEditor.cpp \
    PageWidgets/TTriadEditor.cpp \
    PageWidgets/CWEditor.cpp \
    PageWidgets/AllEditor.cpp \
    Data.cpp \
    Editor.cpp \
    FF8Text.cpp \
    LZS.cpp \
    main.cpp \
    SavecardWidget.cpp \
    SaveIcon.cpp \
    Window.cpp \
    HeaderDialog.cpp \
    Config.cpp \
    FF8text_caract.cpp \
    SpinBoxDelegate.cpp \
    QHexEdit/qhexedit.cpp \
    QHexEdit/qhexedit_p.cpp \
    QHexEdit/xbytearray.cpp \
    QHexEdit/commands.cpp \
    SaveData.cpp \
    PageWidgets/PageWidget.cpp \
    PageWidgets/DrawPointEditor.cpp \
    PageWidgets/BattleEditor.cpp \
    PageWidgets/ShopEditor.cpp \
    SelectSavesDialog.cpp \
    PageWidgets/WorldmapEditor.cpp \
    MapWidget.cpp \
    PageWidgets/FieldEditor.cpp \
    StartWidget.cpp \
    PageWidgets/PartyEditor.cpp \
    GZIP.cpp \
    SavecardData.cpp \
    SavecardView.cpp \
    PageWidgets/PreviewEditor.cpp \
    TimeWidget.cpp \
    HelpWidget.cpp \
    Metadata.cpp \
    UserDirectory.cpp \
    FF8Installation.cpp \
    MetadataDialog.cpp \
    SpinBox.cpp
RESOURCES += Hyne.qrc
TRANSLATIONS += hyne_en.ts \
    hyne_ja.ts
CODECFORTR = UTF-8
CODECFORSRC = UTF-8

# Only for static compilation
# QTPLUGIN += qjpcodecs
# CONFIG += static

# Icons
macx {
    ICON = images/hyne.icns
}
win32 {
    RC_FILE = Hyne.rc
}

OTHER_FILES += Hyne.rc \
    deploy.bat \
    compat/QtWidgets

# only on linux/unix (for package creation and other deploys)
unix:!macx:!symbian {

    system(lrelease Hyne.pro)

    target.path = /usr/bin

    langfiles.files = *.qm
    langfiles.path = /usr/share/hyne/

    icon.files = images/Hyne.png
    icon.path = /usr/share/pixmaps

    desktop.files = Hyne.desktop
    desktop.path = /usr/share/applications

    INSTALLS += target langfiles icon desktop
}

DISTFILES += Hyne.desktop \
    README.md

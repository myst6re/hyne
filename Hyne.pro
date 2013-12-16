TEMPLATE = app
!win32 {
	TARGET = hyne
}
win32 {
	TARGET = Hyne
}
DEPENDPATH += .
INCLUDEPATH += .

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
	QTaskBarButton.h \
    PageWidgets/PreviewEditor.h \
    TimeWidget.h \
	HelpWidget.h \
	Metadata.h \
    UserDirectory.h \
    FF8Installation.h
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
    FF8text_caract_utf8.cpp \
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
	QTaskBarButton.cpp \
    PageWidgets/PreviewEditor.cpp \
    TimeWidget.cpp \
    HelpWidget.cpp \
    Metadata.cpp \
    UserDirectory.cpp \
    FF8Installation.cpp
RESOURCES += Hyne.qrc
TRANSLATIONS += hyne_en.ts \
    hyne_ja.ts

# Only for static compilation
# QTPLUGIN += qjpcodecs
# CONFIG += static

macx {
	ICON = images/hyne.icns
}
!win32 {
	LIBS += -lz
}
win32 {
	RC_FILE = Hyne.rc
	LIBS += -lole32
	HEADERS += shobjidl.h # Windows 7 taskBarButton
}

OTHER_FILES += Hyne.rc \
	Hyne.desktop

#only on linux/unix (for package creation and other deploys)
unix:!macx:!symbian {
	system(lrelease Hyne.pro)
}

Hyne
====

Hyne is a complete savegame editor for Final Fantasy VIII.

## How to compile

Pease refer to the Qt documentation for more information about
how to compile a Qt project.

### With Qt Creator

Just open Hyne.pro with Qt Creator, that's easy to edit
and compile the project with it!

### Manually

In the project directory:

    qmake
    make

If you are on Windows, you can compile with mingw32:

    qmake
    mingw32-make

### Dependencies

Hyne needs Qt4.7 at least to be compiled. If your
version is lower than Qt5, you may need the GZIP library
(zlib package).

## About taskbar button feature (Windows 7+)

This feature is only available with Qt5.2.

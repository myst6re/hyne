# Hyne

[![Build Status](https://travis-ci.org/myst6re/hyne.svg?branch=develop)](https://travis-ci.org/myst6re/hyne)
[![Build status](https://ci.appveyor.com/api/projects/status/hqwk4fcapv28kyrq/branch/master?svg=true)](https://ci.appveyor.com/project/myst6re/hyne/branch/master)

Hyne is a complete savegame editor for Final Fantasy VIII.

## How to compile

Pease refer to the Qt documentation for more information about
how to compile a Qt project.

### Dependencies

Hyne needs Qt5 at least to be compiled. If your Qt installation does not have
QtZlib, you may need the GZIP library (zlib package).

#### zlib on Windows

Execute the following commands to fill the zlib directory (if needed):

    git submodule update --init

#### About taskbar button feature (Windows 7+)

This feature is only available with Qt5.2+.

### Compile with Qt Creator

Just open Hyne.pro with Qt Creator, that's easy to edit
and compile the project with it!

### Compile manually

In the project directory:

    qmake
    make

If you are on Windows, you can compile with mingw32:

    qmake
    mingw32-make

### Deploy

Change the version number in `Hyne.pro` (all targets) and `Hyne.desktop` (Unix
targets).

On Windows, you can use `deploy.bat` to deploy DLLs, translation files
and compress files.

`.travis.yml` and `appveyor.yml` are configuration files used to automatically
deploy new versions (tagged in the repository).

## How to contribute

Pull requests are welcome on [GitHub](https://github.com/myst6re/hyne).
Ensure that you have commited in `develop` branch, otherwise I will not accept
your request.

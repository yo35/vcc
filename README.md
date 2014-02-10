Virtual Chess Clock
===================

Virtual Chess Clock is a chess clock software: it can be used to simulate a
chess clock on computer (typically a laptop). Virtual Chess Clock is designed
to be easy to use. It is free, open-source and multi-platform.

http://vchessclock.sourceforge.net/  
https://github.com/yo35/vcc (developer link)



Features
--------

* Two active key areas on the keyboard (left and right) to operate the clock.
* Support for exotic time control modes such as Fischer, Bronstein, hourglass,
  byo-yomi.
* Time resolution of 1 millisecond.
* Inactivation of most key shortcuts defined by the OS (such as the Windows key
  that triggers the "Start" menu on Windows) when the software is in use.

If you encounter some bugs with this program, or if you wish to get new features
in the future versions, you can report/propose them in the GitHub bug tracker at
https://github.com/yo35/vcc/issues. You can also send an email to the author:
yo35@melix.net.



Translation
-----------

Currently, Virtual Chess Clock is available in the following languages:

* English,
* French,
* German,
* Spanish.

If you are interested in translating this program into your language, please
contact the author. Do not be afraid: you don't need to have any special
proficiency in programming or software development to do so.



Installation
------------

### Installing from a binary package ###

Depending your operating system, you may consider installing Virtual Chess Clock
using one of the following pre-compiled binary package:

* Windows installer (for Windows),
* Debian package (for Debian-based Linux distributions).

These packages are available for download at
http://vchessclock.sourceforge.net/download/.


### Building from sources ###

To build Virtual Chess Clock from the source, you need:

1. A C++ compiler that support the C++11 standard (typically, GCC or MinGW >= 4.7).
2. The [CMake](http://www.cmake.org/) build framework (at least version 2.8.11).
3. The [boost libraries](http://www.boost.org/) (at least version 1.49).
4. The [Qt library](http://qt-project.org/) (at least version 5.0).

When all these required component are ready, you need to download the VCC source
code:

* either by cloning the git repository (from https://github.com/yo35/vcc.git),
* or by downloading and unpacking the ZIP archive with the sources, available
  for download at http://vchessclock.sourceforge.net/download/.

Finally, open a terminal, and type the following commands:

```
cd /path/to/VCC/sources
./configure
make
sudo make install
```

On Windows, this process becomes:

```
cd x:\path\to\VCC\sources
configure.bat
mingw32-make
mingw32-make install
```

Type `./configure --help` to get more details about the available installation
options.

Virtual Chess Clock
===================

Virtual Chess Clock is a [chess clock](http://en.wikipedia.org/wiki/Chess_clock) software:
it can be used to simulate a chess clock on computer (typically a laptop).
Virtual Chess Clock is designed to be easy to use. It is free, open-source and multi-platform.

https://sourceforge.net/projects/vchessclock/



Download
--------

https://sourceforge.net/projects/vchessclock/files/



Features
--------

* Two active key areas on the keyboard (left and right) to operate the clock.
* Support for exotic time control modes such as Fischer, Bronstein, hourglass,
  byo-yomi.
* Time resolution of 1 millisecond.
* Inactivation of most key shortcuts defined by the OS (such as the Windows key
  that triggers the "Start" menu on Windows) when the software is in use.

If you encounter some bugs with this program, or if you wish to get new features
in the future versions, you can report/propose them
in the [GitHub bug tracker](https://github.com/yo35/vcc/issues),
or send an [email to the author](mailto:yo35@melix.net).



Screenshots
-----------

<img alt="Main window" src="screenshot_main_window.png" width="600" />  
The main window.

<img alt="Time control set-up" src="screenshot_time_control.png" width="600" />  
Time control configuration dialog.

<img alt="Keyboard set-up" src="screenshot_keyboard_setup.png" width="600" />  
Keyboard set-up dialog. The function of each key can be finely tuned.



Translation
-----------

Currently, Virtual Chess Clock is available in the following languages:

* English,
* French,
* German,
* Spanish.

If you are interested in translating this program into your language,
please [contact the author](mailto:yo35@melix.net).



Build from source
-----------------

### Required components ###

1. A C++ compiler that supports the C++11 standard (typically, GCC or MinGW >= 4.7).
2. The [CMake](http://www.cmake.org/) build framework (at least version 2.8.11).
3. The [boost libraries](http://www.boost.org/) (at least version 1.49).
4. The [Qt library](http://qt-project.org/) (at least version 5.0).

Making all these things work together is likely to be the harder part of the job! ;-)


### Download the source code ###

The source code can be retrieved by cloning the current GitHub repository:

```
git clone https://github.com/yo35/vcc.git --branch release
```

However, if you are not familiar with the Git tool, you can download a
[ZIP archive](https://sourceforge.net/projects/vchessclock/files/2.0-latest/virtual-chess-clock-2.0.0-Source.zip/download)
containing the latest (stable) source from the
[SourceForge repository](https://sourceforge.net/projects/vchessclock/).


### Build VCC ###

When the required components and the VCC source are ready, open a terminal,
and type the following commands:

```
cd /path/to/VCC/source
./configure
make
sudo make install
```

On Windows, using the MinGW compiler, this process becomes:

```
cd x:\path\to\VCC\sources
configure.bat
mingw32-make
mingw32-make install
```

Additional details about the available installation options can be obtained
with the command `./configure --help` (or `configure.bat --help` on Windows).
Should you encounter any problems, please report them
[here](https://github.com/yo35/vcc/issues).

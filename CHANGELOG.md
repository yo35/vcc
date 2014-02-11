Virtual Chess Clock
===================


Version 2.0 (February 11, 2014)
-------------------------------

* Major code refactoring
* Switch to the Qt GUI library (instead of GTKmm, which is hardly maintained on Windows)
* German translation (thanks to Tobias Küchel)
* Refactor the key detection system to make it less dependent of the actual keyboard layout
* Full-screen feature
* Additional display options


Version 1.6 (October 5, 2012)
-----------------------------

* Add an option to display the names of the players
* Improved keyboard shortcuts to pause the game


Version 1.5 (August 16, 2012)
-----------------------------

* Add an option to pause the game through the keyboard


Version 1.4 (September 29, 2011)
--------------------------------

* Spanish translation (thanks to Santiago Paz)
* Option for switching players with the mouse buttons


Version 1.3 (May 22, 2011)
--------------------------

* Implementation of byo-yomi time control mode (used for go)
* Keyboard layout description files available for Lithuanian, Russian and
  Ukrainian keyboards


Version 1.2 (March 8, 2011)
---------------------------

* Status bar showing the time control mode currently in use
* Improved time visualization in Bronstein mode (more suited to Backgammon).
  Reverting to the former visualization mode is possible through the corresponding
  display option.
* Keyboard layout description files available for Turkish, Czech, Slovak keyboards


Version 1.1 (January 24, 2011)
------------------------------

* The keyboard combo display an icon for each keyboard layout
* Keyboard layout description files available for Irish, Spanish, Mexican keyboards
* Various bugs fixed


Version 1.0.1 (December 6, 2010)
--------------------------------

* Windows bugs solved
* A 'configure' script is now provided, both for Unix and Windows


Version 1.0 (November 27, 2010)
-------------------------------

* The active keys on the keyboard are now managed through a geometric description
  of the keyboard layout, so that VCC can work well even if the final have not
  the same keyboard layout configuration than myself (I'm using a French keyboard
  layout). Layout description file are available for the Belgian, Brazilian,
  Canadian, German, French, Italian, Portuguese, Swedish, English and American
  keyboard layouts.
* Clock reset can now be performed by pushing some keys on the keyboard
* Customization is now available through the 'Preferences' dialog


Version 0.999 (October 24, 2010)
--------------------------------

* The last time control selected is now recorded when VCC closes
* Credits and license dialog
* Strict separation between algorithm and GUI in the source code


Version 0.99 (October 10, 2010)
-------------------------------

* Adding support for internationalization
* French translation


Version 0.9 (September 20, 2010)
--------------------------------

* First official release

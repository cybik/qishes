# Ramblings

## Using aqt is possible, requires craftiness

The precompiled Qt is possible but a bit shit.

* All the libs must now be pulled into the output directory manually
* Bunch of plugin shit needs to also be packed into the repo (redist might be an ass)
* A "qt external browser runtime" might need to get packed as well, unless I abandon Web display
  * Honestly probably for the best
  * this also pullsd in "resources". Again, massive hassle for a bloody web widget I may deprecate

## xdg-utils-cxx-dev

???

# Packages needed, probably

* qt6-base-private-dev
* qt6-svg-dev

## Other things

### CPack

* cmake-fedora
* debaux
* git-buildpackage
* git-buildpackage-rpm
* 

## BAD: qt5
* qtbase5-private-dev
* qtwayland5-private-dev
* libqt5svg5-dev
* libqt5waylandcompositor5-dev
* libqt5waylandclient5-dev
* libqt5xdg-dev
  * This is from lxqt
  * Unused, for now. May drop?
# Gcodetools


Gcodetools is a set of Inkscape extensions for generating [G-Code](https://en.wikipedia.org/wiki/G-code) and other performing computer-aided manufacturing related tasks. It serves as an interface for working with plotters, laser cutters, engravers and other CNC machines based on Inkscape.

## Maintainer wanted!

The extension is currently unmaintained. Help with the following tasks is greatly appreciated:
 - Collecting feedback from maker spaces and other users 
 - Compiling documentation of the various features
 - Fixing bugs and improving the overall code quality
 - Adding [unit tests](#testing) and expanding the test framework
 - Using new API functionality added since Inkscape 1.0 to improve the robustness of the extensions

Do you want to contribute? Join the community on [chat.inkscape.org](chat.inkscape.org) (channel #inkscape_extensions)!

## Installation

These scripts should be installed with an Inkscape package already (if you have 
installed Inkscape). For packagers or people testing newer releases, you can 
install the `*.inx` and `*.py` files into `/usr/share/inkscape/extensions` or 
`~/.config/inkscape/extensions` .

## Testing
<a name="testing"></a>

You must install the program `pytest` in order to run these tests. You may run all tests by omitting any other parameters or select tests by adding the test filename that you want to run.

    pytest
    pytest tests/test_gcodetools.py

See [extensions/TESTING.md](https://gitlab.com/inkscape/extensions/-/blob/master/TESTING.md) for further details.

Increasing the test coverage of gcodetools is one of the primary development objectives at the moment.
## Development

The gcodetools extension requires the inkex library, developed [here](https://gitlab.com/inkscape/extensions). Clone the main extensions repository and add the cloned folder to your PYTHONPATH, e.g. by creating a .pth file in your virtualenv's `site-packages`. 

The branches of gcodetools are parallel to the branches of inkex, i.e. the 1.1.x branch uses only functionality that is included in the inkex-1.1.x branch (corresponding to the Inkscape 1.1.x release series). This is also ensured by the CI pipeline.

Apart from this, the same rules as in the main extensions repo apply with respect to code quality.
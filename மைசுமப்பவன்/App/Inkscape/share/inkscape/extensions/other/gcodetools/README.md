# Gcodetools

This folder contains gcodetools extension. They require the Inkscape extensions API inkex, see https://gitlab.com/inkscape/extensions.

## Installation

These scripts should be installed with an Inkscape package already (if you have 
installed Inkscape). For packagers or people testing newer releases, you can 
install the *.inx and *.py files into /usr/share/inkscape/extensions or 
~/.config/inkscape/extensions .

## Testing

These extensions are designed to have good test coverage for python 3.6 and above.

You must install the program `pytest` in order to run these tests. You may run all tests by omitting any other parameters or select tests by adding the test filename that you want to run.

    pytest
    pytest tests/test_gcodetools.py

See TESTING.md for further details.

## Extension description

Each *.inx file describes an extension, listing its name, purpose,
prerequisites, location within the menu, etc. These files are read by
Inkscape on launch. Other files are the scripts themselves (Perl,
Python, and Ruby are supported, as well as shell scripts).

## Development

Development of both the core inkex modules, tests and each of the extensions
contained within the core inkscape extensions repository should follow these
basic rules of quality assurance:

 * Use python3.6 or later, no python2 code would be used here.
 * Use pylint to ensure code is written consistantly
 * Have tests so that each line of an extension is covered in the coverage report
 * Not cross streams between extensions, so your extension should import from
   a module and not from another extension.
 * Use translations on text for display to users using get text.
 * Should not require external programs to work (with some exceptions)

Also join the community on chat.inkscape.org channel #inkscape_extensions with any
doubts or problems.
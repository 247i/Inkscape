# Inkscape Extensions

This folder contains the stock Inkscape extensions, i.e. the scripts that
implement some commands that you can use from within Inkscape. Most of
these commands are in the Extensions menu.

It also contains the Python package `inkex` which powers the Inkscape extensions.
The package has a [separate readme here](package-readme.md).

This readme concerns the development of the core extensions and `inkex` itself.

## Installation

These scripts should be installed with an Inkscape package already (if you have 
installed Inkscape). For packagers or people testing newer releases, you can 
install the files into `/usr/share/inkscape/extensions` or 
`~/.config/inkscape/extensions` .

## Testing

These extensions are designed to have good test coverage for Python 3.7 and above.

You must install the program `pytest` in order to run these tests. You may run
all tests by omitting any other parameters or select tests by adding the test
filename that you want to run.

    pytest
    pytest tests/test_my_extension.py

See [TESTING.md](TESTING.md) for further details.

## Extension description

Each `*.inx` file describes an extension, listing its name, purpose,
prerequisites, location within the menu, etc. These files are read by
Inkscape on launch. Other files are the scripts themselves (Perl,
Python, and Ruby are supported, as well as shell scripts).

## Development

Development of both the core inkex modules, tests and each of the extensions
contained within the core inkscape extensions repository should follow these
basic rules of quality assurance:

* Use Python 3.7 or later, no Python 2 code would be used here.
* Use [Black](https://black.readthedocs.io/en/stable/) to ensure code is written
  consistantly.
* Write tests so that each line of an extension is covered in the coverage report.
* Do not cross streams between extensions, so your extension should import from
  a module and not from another extension.
* Use translations on text for display to users using get text.
* Do not introduce dependencies to external programs (with some exceptions).

Also join the community on [Inkscape's RocketChat](https://chat.inkscape.org),
specifically the
[#inkscape_extensions](https://chat.inkscape.org/channel/inkscape_extensions)
channel with any doubts or problems.

## Building Docs

If you improve the documentation, you might like to compile it to check what it looks like.
This section should get you set up.

1. Install [Poetry](https://pypi.org/project/poetry/) and the dependencies.
   ```
   pip3 install poetry
   poetry install
   poetry run sphinx-apidoc -e -P -o docs/source/ inkex */deprecated.py
   ```
2. Build the documentation
   ```
   cd docs
   poetry run make html
   ```
3. Open the documentation in the `build/html` directory.
   ```
   firefox ../build/html/index.html
   ``` 

If that does not work, please have a look at the [.gitlab-ci.yml](.gitlab-ci.yml) and update this documentation!

All documentation should be included __inside__ of each python module.

The latest documentation for master branch can be found
[here](https://inkscape.gitlab.io/extensions/documentation/).

## License Requirements

Only include extensions here which are GPL-compatible.  This includes
Apache-2.0, MPL-1.1, certain Creative Commons licenses, and more. See the GNU
project's page
[Various Licenses and Comments about Them](https://www.gnu.org/licenses/license-list.html.en)
for guidance.

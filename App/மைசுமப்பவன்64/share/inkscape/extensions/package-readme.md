# inkex & Inkscape extensions

This package supports Inkscape extensions. 

It provides 
 - a simplification layer for SVG manipulation through lxml
 - base classes for common types of Inkscape extensions
 - simplified testing of those extensions
 - a user interface library based on GTK3

At its core, Inkscape extensions take in a file, and output a file.
- For effect extensions, those two files are SVG files.
- For input extensions, the input file may be any arbitrary file and the output is an SVG.
- For output extensions, the input is an SVG file while the output is an arbitrary file.
- Some extensions (e.g. the extensions manager) don't manipulate files.

This folder also contains the stock Inkscape extensions, i.e. the scripts that
implement some commands that you can use from within Inkscape. Most of
these commands are in the Extensions menu, or in the Open / Save dialogs.

## Documentation

The latest documentation for how to develop Inkscape extensions can be found at
https://inkscape.gitlab.io/extensions/documentation/.

## Installation

```
pip install inkex
```

Inkex releases are currently synchronous with Inkscape releases and share the same version number.

## Publishing an extension on the Inkscape website

Follow the guide on https://inkscape.gitlab.io/extensions/documentation/authors/submit.html#i-want-to-submit-my-extension.
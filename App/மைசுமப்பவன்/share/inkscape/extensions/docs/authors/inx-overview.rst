INX extension descriptor format
===============================


.. highlight:: xml

Introduction
------------

In order for Inkscape to make use of an external script or program, you
must describe that script to inkscape using an INX file. The INX file is
an XML file with Inkscape-specific content that can be edited in a
plain-text editor.

The INX file allows the author to:

-  Specify what type of extension it is, for example input, output, or
   effect
-  Identify :ref:`which interpreter <supported_interpreters>` should be used to run the extension
-  List dependencies; files or other extensions required for operation
-  Define parameters that can be set in the extension
-  Create :ref:`a GUI with control widgets <inx-widgets>` for those parameters
-  Add a submenu to the Extensions menu for the extension to reside in
-  Label strings for translation
-  Chain extensions
-  Etc

Nothing beats a working example, and Inkscape includes a great number of
extensions with INX files that you can read. To find the location of
your Inkscape extensions directory, where included extensions and their
INX files are located, look in the System pane of Inkscape Preferences,
under "Inkscape extensions".

.. _translation_of_extensions:

Translation of extensions
-------------------------

Extension dialog windows, described in INX files, can be prepared for
translation or localisation by adding an ``_`` (underscore) to the XML
tags or attributes. Only add underscores when text needs to be
translated (not numeric values, for example!).

Example::

   <_name>Some translatable extension name</_name>

Or::

   <param name="..." type="..." _gui-text="Some translatable label text">

When extensions are included in the `Inkscape Extensions Repository`_,
various scripts will scan each INX file for translatable text and
prepare `translation files`_ for others to translate.

See also: `Ted's blog`_.

.. _attributes_description:

Attributes description
----------------------

+---------------------------+-------------------------+
| Attribute name            | Allowed values          |
+===========================+=========================+
| ``implements-custom-gui`` | ``"true"`` |            |
|                           | ``"false"`` (default)   |
| .. versionadded:: 1.0     |                         |
+---------------------------+-------------------------+
| If set to ``true`` **requires** an effect           |
| extension to implement custom GUI.                  |
|                                                     |
| .. hint::                                           |
|    *Implementation detail:* The "extension is       |
|    working" window is not shown for this kind of    |
|    extensions. This means user interaction with the |
|    Inkscape interface is blocked until the          |
|    extension returns, with no way for the user to   |
|    abort the running extension! It is therefore     |
|    **absolutely essential** that your extension     |
|    provides the necessary visual feedback for the   |
|    user and has proper error handling, to rule out  |
|    any dead-locking behavior.                       |
+---------------------------+-------------------------+
| ``needs-document``        | ``"true"`` (default) |  |
|                           | ``"false"``             |
| .. versionadded:: 1.0     |                         |
+---------------------------+-------------------------+
| If set to ``false`` an effect extension will not be |
| passed a document nor will a document be read back  |
| ("no-op" effect). This is currently a hack to make  |
| extension manager work and will likely be           |
| removed/replaced in future, so use at your          |
| **own risk**!                                       |
+---------------------------+-------------------------+
| ``needs-live-preview``    | ``"true"`` (default)    |
|                           | ``"false"``             |
+---------------------------+-------------------------+
| If set to ``true`` in an effect extension, it will  |
| offer a "Live preview" checkbox in its GUI. When    |
| the user checks that box, it will run the extension |
| in a "preview mode", visually showing the effect of |
| the extension, but not making any changes to the    |
| SVG document, unless the user clicks the Apply      |
| button. While "Live preview" is checked in the GUI, |
| any changes that the user makes to parameters       |
| accessible in the GUI will generate an updated      |
| preview.                                            |
+---------------------------+-------------------------+
| ``savecopyonly``          | ``"true"`` |            |
|                           | ``"false"`` (default)   |
| .. versionadded:: 1.2     |                         |
+---------------------------+-------------------------+
| If set to ``true`` in an **output** extension, it   |
| will limit the extension to being available only    |
| in the "Save a Copy" menu.                          |
+---------------------------+-------------------------+


Example
-------

::

   <?xml version="1.0" encoding="UTF-8"?>
   <inkscape-extension xmlns="http://www.inkscape.org/namespace/inkscape/extension">
     <_name>{Friendly Extension Name}</_name>
     <id>{org.domain.sub-domain.extension-name}</id>
     <dependency type="executable" location="[extensions|path|plugins|{location}]">program.ext</dependency>
     <param name="tab" type="notebook">  
       <page name="controls" _gui-text="Controls">
         <param name="{argumentName}" type="[int|float|string|bool]" min="{number}" max="{number}"
           _gui-text="{Friendly Argument Name}">{default value}</param>
       </page>
       <page name="help" _gui-text="Help">
         <param name="help_text" type="description">{Friendly Extension Help}</param>
       </page>
     </param>
     <effect>
       <object-type>[all|{element type}]</object-type>
         <effects-menu>
           <submenu _name="{Extension Group Name}"/>
         </effects-menu>
     </effect>
     <script>
       <command reldir="extensions" interpreter="[python|perl|ruby|bash|{some other}]">program.ext</command>
     </script>
   </inkscape-extension>

More example INX files are available in the Inkscape distribution, which
takes its files from the `Inkscape Extensions Repository`_.

For a full list of currently supported interpreters, please see 
:ref:`supported_interpreters`.

.. _dtd_xml_schema:

DTD XML schema
--------------

.. warning:: This section contains slightly outdated information.

The following XML schema may not fully describe the current INX file
structure. The actual XML schema used is described in the :ref:`next
paragraph <relax_ng_xml_schema>`.

::

    <!ELEMENT inkscape-extension (name, id, dependency*, param*,(input|output|effect),(script|plugin))>
    <!ELEMENT input (extension, mimetype, filetype, filetypetooltip, output_extension?)>
    <!ELEMENT output (extension, mimetype, filetype, filetypetooltip, dataloss?)>
    <!ELEMENT effect (object-type|submenu?)>
    <!ELEMENT script (command, helper_extension*, check*)>
    <!ELEMENT plugin (name)>
    <!ELEMENT name (#PCDATA)>
    <!ELEMENT id (#PCDATA)>
    <!ELEMENT item (#PCDATA)>
    <!ELEMENT option (#PCDATA)>
    <!ELEMENT dependency (#PCDATA)>
    <!ELEMENT param (#PCDATA|page|item|option)*>
    <!ELEMENT page (#PCDATA, param*)>
    <!ELEMENT extension (#PCDATA)>
    <!ELEMENT mimetype (#PCDATA)>
    <!ELEMENT filetype (#PCDATA)>
    <!ELEMENT filetooltip (#PCDATA)>
    <!ELEMENT object-type (#PCDATA)>
    <!ELEMENT command (#PCDATA)>
    <!ELEMENT check (#PCDATA)>
    <!ELEMENT dataloss (#PCDATA)>
    <!ELEMENT helper_extension (#PCDATA)>
    <!ELEMENT output_extension (#PCDATA)>
    <!ELEMENT menu-tip (#PCDATA)>
    
    <!ATTLIST check reldir (absolute|path|extensions|plugins) #REQUIRED>
    <!ATTLIST command reldir (absolute|path|extensions|plugins) #REQUIRED>
    <!ATTLIST command interpreter CDATA #REQUIRED>
    <!ATTLIST dependency type (executable|extension) #REQUIRED>
    <!ATTLIST dependency location (absolute|path|extensions|plugins) #IMPLIED>
    <!ATTLIST dependency description CDATA #IMPLIED>
    <!ATTLIST effect needs-live-preview (true|false) #REQUIRED>
    <!ATTLIST effect implements-custom-gui (true|false) #IMPLIED>
    <!ATTLIST effect needs-document (true|false) #IMPLIED>
    <!ATTLIST page name CDATA #REQUIRED>
    <!ATTLIST page gui-text CDATA #IMPLIED>
    <!ATTLIST param name CDATA #REQUIRED>
    <!ATTLIST param type (int|float|string|bool|notebook|path|optiongroup|color) #REQUIRED>
    <!ATTLIST param min CDATA #IMPLIED>
    <!ATTLIST param max CDATA #IMPLIED>
    <!ATTLIST param max_length CDATA #IMPLIED>
    <!ATTLIST param precision CDATA #IMPLIED>
    <!ATTLIST param gui-text CDATA #IMPLIED>
    <!ATTLIST param gui-tip CDATA #IMPLIED>
    <!ATTLIST param gui-description CDATA #IMPLIED>
    <!ATTLIST param scope CDATA #IMPLIED>
    <!ATTLIST param gui-hidden CDATA #IMPLIED>
    <!ATTLIST param appearance (minimal|) "">
    <!ATTLIST submenu name CDATA #REQUIRED>

.. _relax_ng_xml_schema:

RELAX NG XML schema
-------------------

The XML schema for INX files is available in the `Inkscape extensions
Git repository`_. This is a `RELAX NG schema`_.

.. _see_also:

.. _next paragraph: INX_extension_descriptor_format#RELAX_NG_XML_schema
.. _Inkscape extensions Git repository: https://gitlab.com/inkscape/extensions/-/blob/master/inkscape.extension.rng
.. _RELAX NG schema: http://www.relaxng.org/
.. _INX Parameters: Extensions:_INX_widgets_and_parameters
.. _ScriptingHOWTO: ScriptingHOWTO

.. _Inkscape Extensions Repository: https://gitlab.com/inkscape/extensions
.. _a GUI with control widgets: Extensions:_INX_widgets_and_parameters
.. _translation files: https://gitlab.com/inkscape/inkscape/-/tree/master/po
.. _Ted's blog: http://gould.cx/ted/blog/Translating_Custom_XML
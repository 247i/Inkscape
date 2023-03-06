#
# Copyright 2018-2022 Martin Owens <doctormo@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>
#
"""Information about a package"""

import os
import sys

from inkex.gui import ChildWindow, TreeView


class ExtensionTreeItem(object):
    """Shows the name of the item in the extensions tree"""

    def __init__(self, name, kind="debug", parent=None):
        self.kind = kind
        self.name = str(name)


class ExtensionTreeView(TreeView):
    """A list of extensions (inx file based)"""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.parents = {}
        self.menus = {}

    def setup(self, *args, **kwargs):
        self.ViewColumn("Name", expand=True, text="name")
        self.ViewSort(data=lambda item: item.name, ascending=True)

    def get_menu(self, parent, *remain):
        menu_id = "::".join([str(r) for r in remain])
        if menu_id in self.menus:
            return self.menus[menu_id]

        if remain[:-1]:
            parent = self.get_menu(parent, *remain[:-1])

        menu = self._add_item([ExtensionTreeItem(remain[-1])], parent=parent)
        self.menus[menu_id] = menu
        return menu

    def add_item(self, item, parent=None):
        if not item or not item.name:
            return None
        if item.kind not in self.parents:
            tree_item = ExtensionTreeItem(item.kind.title())
            self.parents[item.kind] = self._add_item([tree_item], parent=None)
        parent = self.parents[item.kind]
        if item.kind == "effect" and len(item.menu) > 1:
            parent = self.get_menu(parent, *item.menu[:-1])
        return self._add_item([item], parent)


class MoreInformation(ChildWindow):
    """Show further information for an installed package"""

    name = "info"

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.inx = ExtensionTreeView(self.widget("inx"), selected=self.select_inx)

    def load_widgets(self, pixmaps, item):
        """Initialise the information"""
        try:
            self.widget("info_website").show()
            self.widget("info_website").set_uri(item.link)
            self.widget("info_website").set_label(item.link)
        except Exception:
            self.widget("info_website").hide()

        self.pixmaps = pixmaps
        self.window.set_title(item.name)
        self.widget("info_name").set_label(item.name)
        self.widget("info_desc").set_label(item.summary)
        self.widget("info_version").set_label(item.version)
        self.widget("info_license").set_label(item.license)
        self.widget("info_author").set_label(f"{item.author}")
        try:
            self.widget("info_image").set_from_pixbuf(pixmaps.get(item.get_icon()))
        except Exception:
            pass

        self.inx.clear()
        if hasattr(item, "get_files"):
            for fn in item.get_files(filters=("*.inx",)):
                self.inx.add([ExtensionTreeItem(fn, kind="lost-and-found")])

    def select_inx(self, item):
        pass

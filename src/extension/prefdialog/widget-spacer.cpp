// SPDX-License-Identifier: GPL-2.0-or-later
/** @file
 * Spacer widget for extensions
 *//*
 * Authors:
 *   Patrick Storz <eduard.braun2@gmx.de>
 *
 * Copyright (C) 2019 Authors
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include "widget-spacer.h"

#include <gtkmm/box.h>

#include "xml/node.h"
#include "extension/extension.h"

namespace Inkscape {
namespace Extension {


WidgetSpacer::WidgetSpacer(Inkscape::XML::Node *xml, Inkscape::Extension::Extension *ext)
    : InxWidget(xml, ext)
{
    // get size
    const char *size = xml->attribute("size");
    if (size) {
        _size = strtol(size, nullptr, 0);
        if (_size == 0) {
            if (!strcmp(size, "expand")) {
                _expand = true;
            } else {
                g_warning("Invalid value ('%s') for size spacer in extension '%s'", size, _extension->get_id());
            }
        }
    }
}

/** \brief  Create a label for the description */
Gtk::Widget *WidgetSpacer::get_widget(sigc::signal<void ()> *changeSignal)
{
    if (_hidden) {
        return nullptr;
    }

    auto const spacer = Gtk::make_managed<Gtk::Box>();
    spacer->property_margin().set_value(_size/2);

    if (_expand) {
        spacer->set_hexpand();
        spacer->set_vexpand();
    }

    spacer->set_visible(true);
    return spacer;
}

}  /* namespace Extension */
}  /* namespace Inkscape */

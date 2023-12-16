// SPDX-License-Identifier: GPL-2.0-or-later
/* Authors:
 *   Thomas Holder
 *
 * Copyright (C) 2020 authors
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef SEEN_INKSCAPE_UI_WIDGET_SCROLL_UTILS_H
#define SEEN_INKSCAPE_UI_WIDGET_SCROLL_UTILS_H

#include <gdk/gdk.h>

namespace Gtk {
class Widget;
} // namespace Gtk

namespace Inkscape::UI::Widget {

Gtk::Widget       *get_scrollable_ancestor(Gtk::Widget       *widget);
Gtk::Widget const *get_scrollable_ancestor(Gtk::Widget const *widget);

bool scrolling_allowed(Gtk::Widget    const *widget,
                       GdkEventScroll const *event = nullptr);

} // namespace Inkscape::UI::Widget

#endif // SEEN_INKSCAPE_UI_WIDGET_SCROLL_UTILS_H

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :

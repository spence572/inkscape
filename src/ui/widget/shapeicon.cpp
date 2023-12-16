// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Authors:
 *   Martin Owens
 *
 * Copyright (C) 2020 Authors
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include "ui/widget/shapeicon.h"

#include <algorithm>
#include <utility>
#include <glibmm/ustring.h>
#include <gdkmm/general.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/enums.h>
#include <gtkmm/stylecontext.h>
#include <gtkmm/widget.h>

#include "color.h"
#include "ui/icon-loader.h"
#include "ui/util.h"

namespace Inkscape::UI::Widget {

void CellRendererItemIcon::set_icon_name()
{
    std::string shape_type = _property_shape_type.get_value();
    if (shape_type == "-") { // "-" is an explicit request not to draw any icon
        property_icon_name().set_value({});
        return;
    }

    auto color = _property_color.get_value();
    if (color == 0 && _widget_color) {
        color = *_widget_color;
    }

    auto [icon_name, color_class] = get_shape_icon(shape_type, color);
    property_icon_name().set_value(icon_name);
    _color_class = std::move(color_class);
}

void CellRendererItemIcon::render_vfunc(const Cairo::RefPtr<Cairo::Context>& cr,
                                        Gtk::Widget &widget,
                                        const Gdk::Rectangle &background_area,
                                        const Gdk::Rectangle &cell_area,
                                        Gtk::CellRendererState flags)
{
    if (property_icon_name().get_value().empty()) {
        return;
    }

    auto const style_context = widget.get_style_context();
    // CSS color might have changed, so refresh if so:
    if (auto const color = to_guint32(get_foreground_color(style_context));
        _widget_color != color)
    {
        _widget_color = color;
        set_icon_name();
    }
    // GTK4 will not let us recolor symbolic icons any other way I can find, so…
    style_context->add_class(_color_class);
    Gtk::CellRendererPixbuf::render_vfunc(cr, widget, background_area, cell_area, flags);
    style_context->remove_class(_color_class);

    int clipmask = _property_clipmask.get_value();
    if (clipmask <= 0) return;

    // Create an overlay icon
    // …somewhat sneakily, by temporarily changing our :icon-name & re-rendering
    auto icon_name = property_icon_name().get_value();
    if (clipmask == OVERLAY_CLIP) {
        property_icon_name().set_value("overlay-clip");
    } else if (clipmask == OVERLAY_MASK) {
        property_icon_name().set_value("overlay-mask");
    } else if (clipmask == OVERLAY_BOTH) {
        property_icon_name().set_value("overlay-clipmask");
    }
    Gtk::CellRendererPixbuf::render_vfunc(cr, widget, background_area, cell_area, flags);
    property_icon_name().set_value(std::move(icon_name));
}

bool CellRendererItemIcon::activate_vfunc(GdkEvent *event,
                                          Gtk::Widget &widget,
                                          const Glib::ustring &path,
                                          const Gdk::Rectangle &background_area,
                                          const Gdk::Rectangle &cell_area,
                                          Gtk::CellRendererState flags) {
    _signal_activated.emit(path);
    return true;
}

} // namespace Inkscape::UI::Widget

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

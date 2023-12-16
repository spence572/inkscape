// SPDX-License-Identifier: GPL-2.0-or-later
/**
 * A simple gradient preview
 *
 * Author:
 *   Lauris Kaplinski <lauris@kaplinski.com>
 *
 * Copyright (C) 2001-2002 Lauris Kaplinski
 * Copyright (C) 2001 Ximian, Inc.
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include "gradient-image.h"

#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>
#include <sigc++/functors/mem_fun.h>

#include "display/cairo-utils.h"
#include "object/sp-gradient.h"
#include "object/sp-stop.h"

namespace Inkscape::UI::Widget {

GradientImage::GradientImage(SPGradient *gradient)
    : _drawing_area{Gtk::make_managed<Gtk::DrawingArea>()}
{
    set_name("GradientImage");

    _drawing_area->set_visible(true);
    _drawing_area->signal_draw().connect(sigc::mem_fun(*this, &GradientImage::on_drawing_area_draw));
    _drawing_area->property_expand() = true; // DrawingArea fills self Box,
    property_expand() = false;               // but the Box doesnʼt expand.
    add(*_drawing_area);

    set_gradient(gradient);
}

bool
GradientImage::on_drawing_area_draw(Cairo::RefPtr<Cairo::Context> const &cr)
{
    auto const width = _drawing_area->get_width(), height = _drawing_area->get_height();
    auto ct = cr->cobj();
    sp_gradient_draw(_gradient, width, height, ct);
    return true;
}

void
GradientImage::set_gradient(SPGradient *gradient)
{
    if (_gradient == gradient) return;

    if (_gradient) {
        _release_connection.disconnect();
        _modified_connection.disconnect();
    }

    _gradient = gradient;

    if (gradient) {
        _release_connection = gradient->connectRelease(sigc::mem_fun(*this, &GradientImage::gradient_release));
        _modified_connection = gradient->connectModified(sigc::mem_fun(*this, &GradientImage::gradient_modified));
    }

    _drawing_area->queue_draw();
}

void
GradientImage::gradient_release(SPObject const * /*object*/)
{
    set_gradient(nullptr);
}

void
GradientImage::gradient_modified(SPObject const * /*object*/, guint /*flags*/)
{
    _drawing_area->queue_draw();
}

} // namespace Inkscape::UI::Widget

void
sp_gradient_draw(SPGradient * const gr, int const width, int const height,
                 cairo_t * const ct)
{
    cairo_pattern_t *check = ink_cairo_pattern_create_checkerboard();
    cairo_set_source(ct, check);
    cairo_paint(ct);
    cairo_pattern_destroy(check);

    if (gr) {
        cairo_pattern_t *p = gr->create_preview_pattern(width);
        cairo_set_source(ct, p);
        cairo_paint(ct);
        cairo_pattern_destroy(p);
    }
}

GdkPixbuf *
sp_gradient_to_pixbuf (SPGradient *gr, int width, int height)
{
    cairo_surface_t *s = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *ct = cairo_create(s);
    sp_gradient_draw(gr, width, height, ct);
    cairo_destroy(ct);
    cairo_surface_flush(s);

    // no need to free s - the call below takes ownership
    GdkPixbuf *pixbuf = ink_pixbuf_create_from_cairo_surface(s);
    return pixbuf;
}

Glib::RefPtr<Gdk::Pixbuf>
sp_gradient_to_pixbuf_ref (SPGradient *gr, int width, int height)
{
    return Glib::wrap(sp_gradient_to_pixbuf(gr, width, height));
}

Glib::RefPtr<Gdk::Pixbuf>
sp_gradstop_to_pixbuf_ref (SPStop *stop, int width, int height)
{
    cairo_surface_t *s = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *ct = cairo_create(s);

    /* Checkerboard background */
    cairo_pattern_t *check = ink_cairo_pattern_create_checkerboard();
    cairo_rectangle(ct, 0, 0, width, height);
    cairo_set_source(ct, check);
    cairo_fill_preserve(ct);
    cairo_pattern_destroy(check);

    if (stop) {
        /* Alpha area */
        cairo_rectangle(ct, 0, 0, width/2, height);
        ink_cairo_set_source_rgba32(ct, stop->get_rgba32());
        cairo_fill(ct);

        /* Solid area */
        cairo_rectangle(ct, width/2, 0, width, height);
        ink_cairo_set_source_rgba32(ct, stop->get_rgba32() | 0xff);
        cairo_fill(ct);
    }

    cairo_destroy(ct);
    cairo_surface_flush(s);

    Cairo::RefPtr<Cairo::Surface> sref = Cairo::RefPtr<Cairo::Surface>(new Cairo::Surface(s));
    Glib::RefPtr<Gdk::Pixbuf> pixbuf =
        Gdk::Pixbuf::create(sref, 0, 0, width, height);

    cairo_surface_destroy(s);

    return pixbuf;
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8 :

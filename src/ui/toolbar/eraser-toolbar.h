// SPDX-License-Identifier: GPL-2.0-or-later
/**
 * @file
 * Eraser toolbar
 */
/* Authors:
 *   MenTaLguY <mental@rydia.net>
 *   Lauris Kaplinski <lauris@kaplinski.com>
 *   bulia byak <buliabyak@users.sf.net>
 *   Frank Felfe <innerspace@iname.com>
 *   John Cliff <simarilius@yahoo.com>
 *   David Turner <novalis@gnu.org>
 *   Josh Andler <scislac@scislac.com>
 *   Jon A. Cruz <jon@joncruz.org>
 *   Maximilian Albert <maximilian.albert@gmail.com>
 *   Tavmjong Bah <tavmjong@free.fr>
 *   Abhishek Sharma
 *   Kris De Gussem <Kris.DeGussem@gmail.com>
 *   Vaibhav Malik <vaibhavmalik2018@gmail.com>
 *
 * Copyright (C) 2004 David Turner
 * Copyright (C) 2003 MenTaLguY
 * Copyright (C) 1999-2011 authors
 * Copyright (C) 2001-2002 Ximian, Inc.
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef SEEN_INKCAPE_UI_ERASERTOOLBAR_H
#define SEEN_INKCAPE_UI_ERASERTOOLBAR_H

#include <glibmm/refptr.h>

#include "toolbar.h"

namespace Gtk {
class Builder;
class ToggleButton;
} // namespace Gtk

class SPDesktop;

namespace Inkscape {
namespace UI {
class SimplePrefPusher;

namespace Tools {
enum class EraserToolMode;
} // namespace Tools

namespace Widget {
class SpinButton;
} // namespace Widget

namespace Toolbar {

class EraserToolbar final : public Toolbar
{
public:
    EraserToolbar(SPDesktop *desktop);
    ~EraserToolbar() override;

private:
    using ValueChangedMemFun = void (EraserToolbar::*)();

    Glib::RefPtr<Gtk::Builder> _builder;
    UI::Widget::SpinButton &_width_item;
    UI::Widget::SpinButton &_thinning_item;
    UI::Widget::SpinButton &_cap_rounding_item;
    UI::Widget::SpinButton &_tremor_item;
    UI::Widget::SpinButton &_mass_item;
    Gtk::ToggleButton *_usepressure_btn = nullptr;
    Gtk::ToggleButton &_split_btn;

    std::unique_ptr<SimplePrefPusher> _pressure_pusher;

    bool _freeze = false;

    void setup_derived_spin_button(UI::Widget::SpinButton &btn, Glib::ustring const &name, double default_value,
                                   ValueChangedMemFun value_changed_mem_fun);
    static unsigned _modeAsInt(Tools::EraserToolMode mode);
    void mode_changed(int mode);
    void set_eraser_mode_visibility(unsigned eraser_mode);
    void width_value_changed();
    void mass_value_changed();
    void velthin_value_changed();
    void cap_rounding_value_changed();
    void tremor_value_changed();
    static void update_presets_list(gpointer data);
    void toggle_break_apart();
    void usepressure_toggled();
};

} // namespace Toolbar
} // namespace UI
} // namespace Inkscape

#endif // SEEN_INKCAPE_UI_ERASERTOOLBAR_H

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

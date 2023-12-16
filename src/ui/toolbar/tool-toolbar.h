// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Authors:
 *   Tavmjong Bah
 *
 * Copyright (C) 2023 Tavmjong Bah
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef SEEN_TOOLBAR_TOOL_H
#define SEEN_TOOLBAR_TOOL_H

#include <memory>
#include <glibmm/refptr.h>
#include <glibmm/ustring.h>
#include <gtkmm/box.h>

#include "preferences.h"

namespace Gtk {
class Button;
class Builder;
} // namespace Gtk

class InkscapeWindow;
class SPDesktop;

namespace Inkscape::UI {

namespace Widget {
class PopoverMenu;
} // namespace Widget

namespace Toolbar {

class ToolToolbar : public Gtk::Box
{
public:
    ToolToolbar(InkscapeWindow *window);
    ~ToolToolbar() override;

    void set_visible_buttons();
    static Glib::ustring get_tool_visible_button_path(const Glib::ustring& button_action_name);

private:
    std::unique_ptr<UI::Widget::PopoverMenu> makeContextMenu(InkscapeWindow *window);
    void showContextMenu(InkscapeWindow *window,
                         Gtk::Button &button, Glib::ustring const &tool_name);
    void attachHandlers(Glib::RefPtr<Gtk::Builder> builder, InkscapeWindow *window);

    static constexpr const char* tools_button_path = "/toolbox/tools/buttons";

    std::unique_ptr<UI::Widget::PopoverMenu> _context_menu;
    Glib::ustring _context_menu_tool_name;

    Inkscape::PrefObserver buttons_pref_observer;
};

} // namespace Toolbar

} // namespace Inkscape::UI

#endif /* SEEN_TOOLBAR_TOOL_H */

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

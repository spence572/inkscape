// SPDX-License-Identifier: GPL-2.0-or-later
/** @file
 * @brief A simple dialog for previewing icon representation.
 */
/* Authors:
 *   Jon A. Cruz
 *   Bob Jamison
 *   Other dudes from The Inkscape Organization
 *
 * Copyright (C) 2004,2005 The Inkscape Organization
 * Copyright (C) 2010 Jon A. Cruz
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef SEEN_ICON_PREVIEW_H
#define SEEN_ICON_PREVIEW_H

#include <memory>
#include <vector>
#include <glibmm/ustring.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/paned.h>

#include "helper/auto-connection.h"
#include "ui/dialog/dialog-base.h"

class SPObject;

namespace Glib {
class Timer;
} // namespace Glib

namespace Gtk {
class ToggleButton;
} // namespace Gtk

namespace Inkscape {

class Drawing;

namespace UI::Dialog {

/**
 * A panel that displays an icon preview
 */
class IconPreviewPanel final : public DialogBase
{
public:
    IconPreviewPanel();
    ~IconPreviewPanel() final;

    void selectionModified(Selection *selection, guint flags) override;
    void documentReplaced() override;

    void refreshPreview();
    void modeToggled();

private:
    std::unique_ptr<Drawing> drawing;
    SPDocument *drawing_doc;
    unsigned int visionkey;
    std::unique_ptr<Glib::Timer> timer;
    std::unique_ptr<Glib::Timer> renderTimer;
    bool pending;
    gdouble minDelay;

    Gtk::Box        iconBox;
    Gtk::Paned      splitter;
    Glib::ustring targetId;
    int hot;
    std::vector<int> sizes;

    Gtk::Image      magnified;
    Gtk::Label      magLabel;

    Gtk::ToggleButton     *selectionButton;

    std::vector<std::vector<unsigned char>> pixMem;
    std::vector<Gtk::Image *> images;
    std::vector<Glib::ustring> labels;
    std::vector<Gtk::ToggleButton *> buttons;
    auto_connection docModConn;
    auto_connection docDesConn;

    void setDocument( SPDocument *document );
    void removeDrawing();
    void on_button_clicked(int which);
    void renderPreview( SPObject* obj );
    void updateMagnify();
    void queueRefresh();
    bool refreshCB();
};

} // namespace UI::Dialog
} // namespace Inkscape

#endif // SEEN_ICON_PREVIEW_H

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

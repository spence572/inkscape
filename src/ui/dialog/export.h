// SPDX-License-Identifier: GPL-2.0-or-later
/* Authors:
 *   Lauris Kaplinski <lauris@kaplinski.com>
 *   bulia byak <buliabyak@users.sf.net>
 *   Johan Engelen <j.b.c.engelen@ewi.utwente.nl>
 *   Anshudhar Kumar Singh <anshudhar2001@gmail.com>
 *
 * Copyright (C) 1999-2007, 2021 Authors
 * Copyright (C) 2001-2002 Ximian, Inc.
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef SP_EXPORT_H
#define SP_EXPORT_H

#include <map>
#include <string>
#include <vector>

#include <glibmm/refptr.h>

#include "helper/auto-connection.h"
#include "ui/dialog/dialog-base.h"

namespace Gtk {
class Box;
class Builder;
class Notebook;
} // namespace Gtk

class SPObject;
class SPPage;

namespace Inkscape {

class Preferences;

namespace Extension {
class Output;
} // namespace Extension

namespace UI::Dialog {

class SingleExport;
class BatchExport;

enum notebook_page
{
    SINGLE_IMAGE = 0,
    BATCH_EXPORT
};

void set_export_bg_color(SPObject* object, guint32 color);
guint32 get_export_bg_color(SPObject* object, guint32 default_color);

class Export final : public DialogBase
{
public:
    Export();
    ~Export() final;

private:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Box      &container;       // Main Container
    Gtk::Notebook &export_notebook; // Notebook Container for single and batch export
    SingleExport  &single_image;
    BatchExport   &batch_export;

    Inkscape::Preferences *prefs = nullptr;

    // setup default values of widgets
    void setDefaultNotebookPage();
    std::map<notebook_page, int> pages;

    auto_connection notebook_signal;

    // signals callback
    void onNotebookPageSwitch(Widget *page, unsigned page_number);
    void documentReplaced() override;
    void desktopReplaced() override;
    void selectionChanged(Inkscape::Selection *selection) override;
    void selectionModified(Inkscape::Selection *selection, unsigned flags) override;

public:
    static std::string absolutizePath(SPDocument *doc, const std::string &filename);
    static bool unConflictFilename(SPDocument *doc, std::string &filename, std::string const extension);
    static std::string filePathFromObject(SPDocument *doc, SPObject *obj, const std::string &file_entry_text);
    static std::string filePathFromId(SPDocument *doc, Glib::ustring id, const std::string &file_entry_text);
    static std::string defaultFilename(SPDocument *doc, std::string &filename_entry_text, std::string extension);
    static bool checkOrCreateDirectory(std::string const &filename);

    static bool exportRaster(
        Geom::Rect const &area, unsigned long int const &width, unsigned long int const &height,
        float const &dpi, guint32 bg_color, Glib::ustring const &filename, bool overwrite,
        unsigned (*callback)(float, void *), void *data,
        Inkscape::Extension::Output *extension, std::vector<SPItem const *> *items = nullptr);
  
    static bool exportVector(
        Inkscape::Extension::Output *extension, SPDocument *doc, Glib::ustring const &filename,
        bool overwrite, const std::vector<SPItem const *> &items, SPPage const *page);
    static bool exportVector(
        Inkscape::Extension::Output *extension, SPDocument *doc, Glib::ustring const &filename,
        bool overwrite, const std::vector<SPItem const *> &items, const std::vector<SPPage const *> &pages);
};

} // namespace UI::Dialog

} // namespace Inkscape

#endif // SP_EXPORT_H

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

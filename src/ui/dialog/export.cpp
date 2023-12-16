// SPDX-License-Identifier: GPL-2.0-or-later
/* Authors:
 *   Lauris Kaplinski <lauris@kaplinski.com>
 *   bulia byak <buliabyak@users.sf.net>
 *   Johan Engelen <j.b.c.engelen@ewi.utwente.nl>
 *   Peter Bostrom
 *   Jon A. Cruz <jon@joncruz.org>
 *   Abhishek Sharma
 *   Kris De Gussem <Kris.DeGussem@gmail.com>
 *   Anshudhar Kumar Singh <anshudhar2001@gmail.com>
 *
 * Copyright (C) 1999-2007, 2012, 2021 Authors
 * Copyright (C) 2001-2002 Ximian, Inc.
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include "export.h"

#include <set>

#include <glibmm/convert.h>
#include <glibmm/i18n.h>
#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/notebook.h>

#include "desktop.h"
#include "document.h"
#include "inkscape.h"
#include "preferences.h"
#include "message.h"                        // for MessageType
#include "message-stack.h"

#include "color/color-conv.h"
#include "extension/output.h"
#include "helper/png-write.h"
#include "io/resource.h"
#include "io/sys.h"                         // for sanitizeString, file_test
#include "object/object-set.h"              // for ObjectSet
#include "object/sp-item.h"                 // for SPItem
#include "object/sp-object.h"               // for SPObject
#include "object/sp-page.h"                 // for SPPage
#include "object/sp-root.h"                 // for SPRoot
#include "object/weakptr.h"                 // for SPWeakPtr
#include "ui/builder-utils.h"
#include "ui/dialog/export-batch.h"
#include "ui/dialog/export-single.h"
#include "ui/interface.h"

namespace Inkscape::UI::Dialog {

Export::Export()
    : DialogBase("/dialogs/export/", "Export")
    , builder(create_builder("dialog-export.glade"))
    , container       (get_widget<Gtk::Box>            (builder, "export-box"))
    , export_notebook (get_widget<Gtk::Notebook>       (builder, "export-notebook"))
    // Initialise Single Export and its objects
    , single_image    (get_derived_widget<SingleExport>(builder, "single-image"))
    // Initialise Batch Export and its objects
    , batch_export    (get_derived_widget<BatchExport> (builder, "batch-export"))
{
    prefs = Inkscape::Preferences::get();

    add(container);
    show_all_children();

    container.signal_realize().connect([=, this]() {
        setDefaultNotebookPage();
        notebook_signal = export_notebook.signal_switch_page().connect(sigc::mem_fun(*this, &Export::onNotebookPageSwitch));
    });
    container.signal_unrealize().connect([this]() {
        notebook_signal.disconnect();
    });
}

Export::~Export() = default;

// Set current page based on preference/last visited page
void Export::setDefaultNotebookPage()
{
    pages[BATCH_EXPORT] = export_notebook.page_num(*batch_export.get_parent());
    pages[SINGLE_IMAGE] = export_notebook.page_num(*single_image.get_parent());
    export_notebook.set_current_page(pages[SINGLE_IMAGE]);
}

void Export::documentReplaced()
{
    single_image.setDocument(getDocument());
    batch_export.setDocument(getDocument());
}

void Export::desktopReplaced()
{
    single_image.setDesktop(getDesktop());
    single_image.setApp(getApp());
    batch_export.setDesktop(getDesktop());
    batch_export.setApp(getApp());
    // Called previously, but we need post-desktop call too
    documentReplaced();
}

void Export::selectionChanged(Inkscape::Selection *selection)
{
    auto current_page = export_notebook.get_current_page();
    if (current_page == pages[SINGLE_IMAGE]) {
        single_image.selectionChanged(selection);
    }
    if (current_page == pages[BATCH_EXPORT]) {
        batch_export.selectionChanged(selection);
    }
}
void Export::selectionModified(Inkscape::Selection *selection, unsigned flags)
{
    auto current_page = export_notebook.get_current_page();
    if (current_page == pages[SINGLE_IMAGE]) {
        single_image.selectionModified(selection, flags);
    }
    if (current_page == pages[BATCH_EXPORT]) {
        batch_export.selectionModified(selection, flags);
    }
}

void Export::onNotebookPageSwitch(Widget *page, unsigned page_number)
{
    auto desktop = getDesktop();
    if (desktop) {
        auto selection = desktop->getSelection();

        if (page_number == pages[SINGLE_IMAGE]) {
            single_image.selectionChanged(selection);
        }
        if (page_number == pages[BATCH_EXPORT]) {
            batch_export.selectionChanged(selection);
        }
    }
}

std::string Export::absolutizePath(SPDocument *doc, const std::string &filename)
{
    std::string path;
    // Make relative paths go from the document location, if possible:
    if (!Glib::path_is_absolute(filename) && doc->getDocumentFilename()) {
        auto dirname = Glib::path_get_dirname(doc->getDocumentFilename());
        if (!dirname.empty()) {
            path = Glib::build_filename(dirname, filename);
        }
    }
    if (path.empty()) {
        path = filename;
    }
    return path;
}

bool Export::unConflictFilename(SPDocument *doc, std::string &filename, std::string const extension)
{
    std::string path = absolutizePath(doc, filename);
    std::string test_filename = path + extension;
    if (!Inkscape::IO::file_test(test_filename.c_str(), G_FILE_TEST_EXISTS)) {
        filename = test_filename;
        return true;
    }
    for (int i = 1; i <= 100; i++) {
        test_filename = path + "_copy_" + std::to_string(i) + extension;
        if (!Inkscape::IO::file_test(test_filename.c_str(), G_FILE_TEST_EXISTS)) {
            filename = test_filename;
            return true;
        }
    }
    return false;
}

// Checks if the directory exists and if not, tries to create the directory and if failed, displays an error message.
bool Export::checkOrCreateDirectory(std::string const &filename)
{
    SPDesktop *desktop = SP_ACTIVE_DESKTOP;
    if (!desktop) {
        return false;
    }

    std::string path = absolutizePath(desktop->getDocument(), filename);
    std::string dirname = Glib::path_get_dirname(path);

    if (dirname.empty() || !Inkscape::IO::file_test(dirname.c_str(), (GFileTest)(G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))) {
        if (g_mkdir_with_parents(dirname.c_str(), S_IRWXU) != 0) {
            Glib::ustring safeDir = Inkscape::IO::sanitizeString(dirname.c_str());
            Glib::ustring error = g_strdup_printf(_("Directory <b>%s</b> does not exist and can't be created.\n"), safeDir.c_str());
            desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, error.c_str());
            sp_ui_error_dialog(error.c_str());
            return false;
        }
    }
    return true;
}

bool Export::exportRaster(
        Geom::Rect const &area, unsigned long int const &width, unsigned long int const &height,
        float const &dpi, guint32 bg_color, Glib::ustring const &filename, bool overwrite,
        unsigned (*callback)(float, void *), void *data,
        Inkscape::Extension::Output *extension, std::vector<SPItem const *> *items)
{
    SPDesktop *desktop = SP_ACTIVE_DESKTOP;
    if (!desktop)
        return false;
    SPDocument *doc = desktop->getDocument();

    if (area.hasZeroArea() || width == 0 || height == 0) {
        desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, _("The chosen area to be exported is invalid."));
        sp_ui_error_dialog(_("The chosen area to be exported is invalid"));
        return false;
    }
    if (filename.empty()) {
        desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, _("You have to enter a filename."));
        sp_ui_error_dialog(_("You have to enter a filename"));
        return false;
    }

    if (!extension || !extension->is_raster()) {
        desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, _("Raster Export Error"));
        sp_ui_error_dialog(_("Raster export Method is used for NON RASTER EXTENSION"));
        return false;
    }

    float pHYs = extension->get_param_float("png_phys", dpi);
    if (pHYs < 0.01) pHYs = dpi;

    bool use_interlacing = extension->get_param_bool("png_interlacing", false);
    int antialiasing = extension->get_param_int("png_antialias", 2); // Cairo anti aliasing
    int zlib = extension->get_param_int("png_compression", 1); // Default is 6 for png, but 1 for non-png
    auto val = extension->get_param_int("png_bitdepth", 99); // corresponds to RGBA 8

    int bit_depth = pow(2, (val & 0x0F));
    int color_type = (val & 0xF0) >> 4;

    std::string path = absolutizePath(doc, Glib::filename_from_utf8(filename));

    // Do the over-write protection now, since the png is just a temp file.
    if (!overwrite && !sp_ui_overwrite_file(path)) {
        return false;
    }

    auto fn = Glib::path_get_basename(path);
    auto png_filename = path;
    {
        // Select the extension and set the filename to a temporary file
        int tempfd_out = Glib::file_open_tmp(png_filename, "ink_ext_");
        close(tempfd_out);
    }

    // Export Start Here
    std::vector<SPItem const *> selected;
    if (items && items->size() > 0) {
        selected = *items;
    }

    ExportResult result = sp_export_png_file(desktop->getDocument(), png_filename.c_str(), area, width, height, pHYs,
                                             pHYs, // previously xdpi, ydpi.
                                             bg_color, callback, data, true, selected,
                                             use_interlacing, color_type, bit_depth, zlib, antialiasing);

    bool failed = result == EXPORT_ERROR; // || prog_dialog->get_stopped();

    if (failed) {
        Glib::ustring safeFile = Inkscape::IO::sanitizeString(path.c_str());
        Glib::ustring error = g_strdup_printf(_("Could not export to filename <b>%s</b>.\n"), safeFile.c_str());

        desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, error.c_str());
        sp_ui_error_dialog(error.c_str());
        return false;
    } else if (result == EXPORT_OK) {
        // Don't ask for preferences on every run.
        try {
            extension->export_raster(doc, png_filename, path.c_str(), false);
        } catch (Inkscape::Extension::Output::save_failed &e) {
            return false;
        }
    } else {
        // Extensions have their own error popup, so this only tracks failures in the png step
        desktop->messageStack()->flash(Inkscape::INFORMATION_MESSAGE, _("Export aborted."));
        return false;
    }

    Glib::ustring safeFile = Inkscape::IO::sanitizeString(path.c_str());
    desktop->messageStack()->flashF(Inkscape::INFORMATION_MESSAGE, _("Drawing exported to <b>%s</b>."),
                                    safeFile.c_str());

    unlink(png_filename.c_str());
    return true;
}

bool Export::exportVector(
        Inkscape::Extension::Output *extension, SPDocument *doc,
        Glib::ustring const &filename,
        bool overwrite, const std::vector<SPItem const *> &items, SPPage const *page)
{
    std::vector<SPPage const *> pages;
    if (page)
        pages.push_back(page);
    return exportVector(extension, doc, filename, overwrite, items, pages);
}

bool Export::exportVector(
        Inkscape::Extension::Output *extension, SPDocument *copy_doc,
        Glib::ustring const &filename,
        bool overwrite, const std::vector<SPItem const *> &items, const std::vector<SPPage const *> &pages)
{
    SPDesktop *desktop = SP_ACTIVE_DESKTOP;
    if (!desktop)
        return false;

    if (filename.empty()) {
        desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, _("You have to enter a filename."));
        sp_ui_error_dialog(_("You have to enter a filename"));
        return false;
    }

    if (!extension || extension->is_raster()) {
        desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, _("Vector Export Error"));
        sp_ui_error_dialog(_("Vector export Method is used for RASTER EXTENSION"));
        return false;
    }

    std::string path = absolutizePath(copy_doc, Glib::filename_from_utf8(filename));
    Glib::ustring safeFile = Inkscape::IO::sanitizeString(path.c_str());

    // Do the over-write protection now
    if (!overwrite && !sp_ui_overwrite_file(path)) {
        return false;
    }
    copy_doc->ensureUpToDate();

    std::vector<SPItem const *> objects = items;
    std::set<std::string> obj_ids;
    std::set<std::string> page_ids;
    for (auto page : pages) {
        if (auto _id = page->getId()) {
            page_ids.insert(std::string(_id));
        }
        // If page then our item set is limited to the overlapping items
        auto page_items = page->getOverlappingItems(true, true);

        if (items.empty()) {
            // Items is page_items, remove all items not in this page.
            objects.insert(objects.end(), page_items.begin(), page_items.end());
        } else {
            for (auto &item : page_items) {
                item->getIds(obj_ids);
            }
        }
    }

    // Delete any pages not specified, delete all pages if none specified
    auto &pm = copy_doc->getPageManager();

    // Make weak pointers to pages, since deletePage() can delete more than just the requested page.
    std::vector<SPWeakPtr<SPPage>> copy_pages;
    copy_pages.reserve(pm.getPageCount());
    for (auto *page : pm.getPages()) {
        copy_pages.emplace_back(page);
    }

    // We refuse to delete anything if everything would be deleted.
    for (auto &page : copy_pages) {
        if (page) {
            auto _id = page->getId();
            if (_id && page_ids.find(_id) == page_ids.end()) {
                pm.deletePage(page.get(), false);
            }
        }
    }

    // Page export ALWAYS restricts, even if nothing would be on the page.
    if (!objects.empty() || !pages.empty()) {
        std::vector<SPObject *> objects_to_export;
        Inkscape::ObjectSet object_set(copy_doc);
        for (auto &object : objects) {
            auto _id = object->getId();
            if (!_id || (!obj_ids.empty() && obj_ids.find(_id) == obj_ids.end())) {
                // This item is off the page so can be ignored for export
                continue;
            }

            SPObject *obj = copy_doc->getObjectById(_id);
            if (!obj) {
                Glib::ustring error = g_strdup_printf(_("Could not export to filename <b>%s</b>. (missing object)\n"), safeFile.c_str());

                desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, error.c_str());
                sp_ui_error_dialog(error.c_str());

                return false;
            }
            copy_doc->ensureUpToDate();

            object_set.add(obj, true);
            objects_to_export.push_back(obj);
        }

        copy_doc->getRoot()->cropToObjects(objects_to_export);

        if (pages.empty()) {
            object_set.fitCanvas(true, true);
        }
    }

    // Remove all unused definitions
    copy_doc->vacuumDocument();

    try {
        extension->save(copy_doc, path.c_str());
    } catch (Inkscape::Extension::Output::save_failed &e) {
        Glib::ustring safeFile = Inkscape::IO::sanitizeString(path.c_str());
        Glib::ustring error = g_strdup_printf(_("Could not export to filename <b>%s</b>.\n"), safeFile.c_str());

        desktop->messageStack()->flash(Inkscape::ERROR_MESSAGE, error.c_str());
        sp_ui_error_dialog(error.c_str());

        return false;
    }

    desktop->messageStack()->flashF(Inkscape::INFORMATION_MESSAGE, _("Drawing exported to <b>%s</b>."),
                                    safeFile.c_str());
    return true;
}

std::string Export::filePathFromObject(SPDocument *doc, SPObject *obj, const std::string &file_entry_text)
{
    Glib::ustring id = _("bitmap");
    if (obj && obj->getId()) {
        id = obj->getId();
    }
    return filePathFromId(doc, id, file_entry_text);
}

std::string Export::filePathFromId(SPDocument *doc, Glib::ustring id, const std::string &file_entry_text)
{
    g_assert(!id.empty());

    std::string directory;

    if (!file_entry_text.empty()) {
        directory = Glib::path_get_dirname(file_entry_text);
    }

    if (directory.empty()) {
        /* Grab document directory */
        const gchar *docFilename = doc->getDocumentFilename();
        if (docFilename) {
            directory = Glib::path_get_dirname(docFilename);
        }
    }

    if (directory.empty()) {
        directory = Inkscape::IO::Resource::homedir_path();
    }

    return Glib::build_filename(directory, Glib::filename_from_utf8(id));
}

std::string Export::defaultFilename(SPDocument *doc, std::string &filename_entry_text, std::string extension)
{
    std::string filename;
    if (doc && doc->getDocumentFilename()) {
        filename = doc->getDocumentFilename();
        //appendExtensionToFilename(filename, extension);
    } else if (doc) {
        filename = filePathFromId(doc, _("bitmap"), filename_entry_text);
        filename = filename + extension;
    }
    return filename;
}

void set_export_bg_color(SPObject* object, guint32 color) {
    if (object) {
        object->setAttribute("inkscape:export-bgcolor", Inkscape::Util::rgba_color_to_string(color).c_str());
    }
}

guint32 get_export_bg_color(SPObject* object, guint32 default_color) {
    if (object) {
        if (auto color = Inkscape::Util::string_to_rgba_color(object->getAttribute("inkscape:export-bgcolor"))) {
            return *color;
        }
    }
    return default_color;
}

} // namespace Inkscape::UI::Dialog

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

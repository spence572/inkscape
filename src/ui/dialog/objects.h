// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * A simple dialog for objects UI.
 *
 * Authors:
 *   Theodore Janeczko
 *   Tavmjong Bah
 *
 * Copyright (C) Theodore Janeczko 2012 <flutterguy317@gmail.com>
 *               Tavmjong Bah 2017
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef SEEN_OBJECTS_PANEL_H
#define SEEN_OBJECTS_PANEL_H

#include <optional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <glibmm/refptr.h>
#include <gtk/gtk.h> // GtkEventControllerKey
#include <gtkmm/box.h>
#include <gtkmm/gesture.h> // Gtk::EventSequenceState
#include <gtkmm/treemodel.h>
#include <gtkmm/treerowreference.h>

#include "color-rgba.h"
#include "helper/auto-connection.h"
#include "preferences.h"
#include "selection.h"
#include "style-enums.h"
#include "ui/dialog/dialog-base.h"
#include "ui/widget/color-picker.h"
#include "ui/widget/preferences-widget.h"
#include "xml/node-observer.h"

namespace Gtk {
class Builder;
class GestureMultiPress;
class Popover;
class RadioButton;
class Scale;
class SearchEntry;
class TreeStore;
} // namespace Gtk

class SPItem;
class SPObject;

namespace Inkscape::UI {

namespace Widget { class ImageToggler; }

namespace Dialog {

class ObjectsPanel;
class ObjectWatcher;

enum {COL_LABEL, COL_VISIBLE, COL_LOCKED};

using SelectionState = int;
enum SelectionStates : SelectionState {
    SELECTED_NOT = 0,     // Object is NOT in desktop's selection
    SELECTED_OBJECT = 1,  // Object is in the desktop's selection
    LAYER_FOCUSED = 2,    // This layer is the desktop's focused layer
    LAYER_FOCUS_CHILD = 4 // This object is a child of the focused layer
};

/**
 * A panel that displays objects.
 */
class ObjectsPanel final : public DialogBase
{
public:
    ObjectsPanel();
    ~ObjectsPanel() final;

    class ModelColumns;

protected:
    void desktopReplaced() final;
    void documentReplaced() final;
    void layerChanged(SPObject *obj);
    void selectionChanged(Selection *selected) final;
    ObjectWatcher *unpackToObject(SPObject *item);

    // Accessed by ObjectWatcher directly (friend class)
    SPObject* getObject(Inkscape::XML::Node *node);
    ObjectWatcher* getWatcher(Inkscape::XML::Node *node);
    ObjectWatcher *getRootWatcher() const { return root_watcher.get(); };
    bool showChildInTree(SPItem *item);

    Inkscape::XML::Node *getRepr(Gtk::TreeModel::Row const &row) const;
    SPItem *getItem(Gtk::TreeModel::Row const &row) const;
    std::optional<Gtk::TreeRow> getRow(SPItem *item) const;

    bool isDummy(Gtk::TreeModel::Row const &row) const { return getRepr(row) == nullptr; }
    bool hasDummyChildren(Gtk::TreeModel::Row const &row) const;
    bool removeDummyChildren(Gtk::TreeModel::Row const &row);
    bool cleanDummyChildren(Gtk::TreeModel::Row const &row);

    Glib::RefPtr<Gtk::TreeStore> _store;
    std::unique_ptr<ModelColumns> _model;

    void setRootWatcher();

private:
    Glib::RefPtr<Gtk::Builder> _builder;
    Inkscape::PrefObserver _watch_object_mode;
    std::unique_ptr<ObjectWatcher> root_watcher;
    SPItem *current_item = nullptr;

    Inkscape::auto_connection layer_changed;
    SPObject *_layer;
    Gtk::TreeModel::RowReference _hovered_row_ref;

    //Show icons in the context menu
    bool _show_contextmenu_icons;

    bool _is_editing;
    bool _scroll_lock = false;

    std::vector<Gtk::Widget*> _watching;
    std::vector<Gtk::Widget*> _watchingNonTop;
    std::vector<Gtk::Widget*> _watchingNonBottom;

    Gtk::TreeView _tree;
    Gtk::CellRendererText *_text_renderer;
    Gtk::TreeView::Column *_name_column;
    Gtk::TreeView::Column *_blend_mode_column = nullptr;
    Gtk::TreeView::Column *_eye_column = nullptr;
    Gtk::TreeView::Column *_lock_column = nullptr;
    Gtk::TreeView::Column *_color_tag_column = nullptr;
    Gtk::Box _buttonsRow;
    Gtk::Box _buttonsPrimary;
    Gtk::Box _buttonsSecondary;
    Gtk::SearchEntry& _searchBox;
    Gtk::ScrolledWindow _scroller;
    Gtk::Box _page;
    Inkscape::auto_connection _tree_style;
    Inkscape::UI::Widget::ColorPicker _color_picker;
    Gtk::TreeRow _clicked_item_row;

    Gtk::Button *_addBarButton(char const* iconName, char const* tooltip, char const *action_name);

    void _activateAction(const std::string& layerAction, const std::string& selectionAction);

    bool blendModePopup(int x, int y, Gtk::TreeModel::Row row);
    bool toggleVisible(unsigned int state, Gtk::TreeModel::Row row);
    bool toggleLocked(unsigned int state, Gtk::TreeModel::Row row);

    enum class EventType {pressed, released};
    Gtk::EventSequenceState on_click(Gtk::GestureMultiPress const &gesture,
                                     int n_press, double x, double y,
                                     EventType);
    bool on_tree_key_pressed   (GtkEventControllerKey const *controller,
                                unsigned keyval, unsigned keycode, GdkModifierType state);
    bool on_window_key_pressed (GtkEventControllerKey const *controller,
                                unsigned keyval, unsigned keycode, GdkModifierType state);
    bool on_window_key_released(GtkEventControllerKey const *controller,
                                unsigned keyval, unsigned keycode, GdkModifierType state);
    bool on_window_key         (GtkEventControllerKey const *controller,
                                unsigned keyval, unsigned keycode, GdkModifierType state,
                                EventType);
    void on_motion_enter (GtkEventControllerMotion const *controller, double x, double y);
    void on_motion_motion(GtkEventControllerMotion const *controller, double x, double y);
    void on_motion_leave (GtkEventControllerMotion const *controller);

    void _searchActivated();
    void _searchChanged();
    
    void _handleEdited(const Glib::ustring& path, const Glib::ustring& new_text);
    void _handleTransparentHover(bool enabled);
    void _generateTranslucentItems(SPItem *parent);

    bool select_row( Glib::RefPtr<Gtk::TreeModel> const & model, Gtk::TreeModel::Path const & path, bool b );

    bool on_drag_motion(const Glib::RefPtr<Gdk::DragContext> &, int, int, guint) final;
    bool on_drag_drop(const Glib::RefPtr<Gdk::DragContext> &, int, int, guint) final;
    void on_drag_start(const Glib::RefPtr<Gdk::DragContext> &);
    void on_drag_end(const Glib::RefPtr<Gdk::DragContext> &) final;

    bool selectCursorItem(unsigned int state);
    SPItem *_getCursorItem(Gtk::TreeViewColumn *column);

    friend class ObjectWatcher;

    SPItem *_solid_item;
    std::vector<SPItem *> _translucent_items;
    int _msg_id;
    Gtk::Popover& _settings_menu;
    Gtk::Popover& _object_menu;
    Gtk::Scale& _opacity_slider;
    std::map<SPBlendMode, Gtk::RadioButton *> _blend_items;
    std::map<SPBlendMode, Glib::ustring> _blend_mode_names;
    Inkscape::UI::Widget::ImageToggler* _item_state_toggler;

    // Special column dragging mode
    Gtk::TreeViewColumn* _drag_column = nullptr;

    UI::Widget::PrefCheckButton& _setting_layers;
    UI::Widget::PrefCheckButton& _setting_track;
    bool _drag_flip;

    bool _selectionChanged();
    auto_connection _idle_connection;
};

} //namespace Dialog

} //namespace Inkscape::UI

#endif // SEEN_OBJECTS_PANEL_H

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

// SPDX-License-Identifier: GPL-2.0-or-later
/** @file
 * @brief New node tool with support for multiple path editing
 */
/* Authors:
 *   Krzysztof Kosiński <tweenk@gmail.com>
 *
 * Copyright (C) 2009 Authors
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef INKSCAPE_UI_TOOLS_NODE_TOOL_H
#define INKSCAPE_UI_TOOLS_NODE_TOOL_H

#include <map>
#include <memory>
#include <vector>
#include <sigc++/connection.h>

#include "ui/tools/tool-base.h"

namespace Inkscape {
    namespace Display {
        class TemporaryItem;
    }

    namespace UI {
        class MultiPathManipulator;
        class ControlPointSelection;
        class Selector;
        class ControlPoint;

        struct PathSharedData;
    }

    class Selection;
    class Rubberband;
    class CanvasItemGroup;
    struct ButtonReleaseEvent;
}

namespace Inkscape::UI::Tools {

class NodeTool : public ToolBase
{
public:
    NodeTool(SPDesktop *desktop);
    ~NodeTool() override;

    Inkscape::UI::ControlPointSelection* _selected_nodes = nullptr;
    Inkscape::UI::MultiPathManipulator* _multipath = nullptr;
    std::vector<Inkscape::Display::TemporaryItem *> _helperpath_tmpitem;
    std::map<SPItem *, std::unique_ptr<ShapeEditor>> _shape_editors;

    bool edit_clipping_paths = false;
    bool edit_masks = false;

    void set(Preferences::Entry const &val) override;
    bool root_handler(CanvasEvent const &event) override;
    bool item_handler(SPItem *item, CanvasEvent const &event) override;
    void deleteSelected();

private:
    Inkscape::Rubberband *get_rubberband() const;

    sigc::connection _selection_changed_connection;
    sigc::connection _mouseover_changed_connection;

    SPItem *flashed_item = nullptr;

    Inkscape::Display::TemporaryItem *flash_tempitem = nullptr;
    Inkscape::UI::Selector* _selector = nullptr;
    Inkscape::UI::PathSharedData* _path_data = nullptr;
    Inkscape::CanvasItemGroup *_transform_handle_group = nullptr;
    SPItem *_last_over = nullptr;

    bool cursor_drag = false;
    bool show_handles = false;
    bool show_outline =false;
    bool live_outline = false;
    bool live_objects = false;
    bool show_path_direction = false;
    bool show_transform_handles = false;
    bool single_node_transform_handles = false;

    std::vector<SPItem*> _current_selection;
    std::vector<SPItem*> _previous_selection;

    void selection_changed(Inkscape::Selection *sel);

    void select_area(Geom::Path const &path, ButtonReleaseEvent const &event);
    void select_point(ButtonReleaseEvent const &event);
    void mouseover_changed(Inkscape::UI::ControlPoint *p);
    void update_tip(CanvasEvent const &event);
    void update_tip();
    void handleControlUiStyleChange();
};

void sp_update_helperpath(SPDesktop *desktop);

} // namespace Inkscape::UI::Tools

// Todo: Remove
inline bool INK_IS_NODE_TOOL(Inkscape::UI::Tools::ToolBase const *obj) { return dynamic_cast<Inkscape::UI::Tools::NodeTool const *>(obj); }

#endif // INKSCAPE_UI_TOOLS_NODE_TOOL_H

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

// SPDX-License-Identifier: GPL-2.0-or-later
/** @file
 * The context in which a single CanvasItem tree exists. Holds the root node and common state.
 */

#include "canvas-item-context.h"

#include "canvas-item-group.h"

namespace Inkscape {

CanvasItemContext::CanvasItemContext(UI::Widget::Canvas *canvas)
    : _canvas(canvas)
    , _root(new CanvasItemGroup(this))
{
}

CanvasItemContext::~CanvasItemContext()
{
    delete _root;
}

void CanvasItemContext::snapshot()
{
    assert(!_snapshotted);
    _snapshotted = true;
}

void CanvasItemContext::unsnapshot()
{
    assert(_snapshotted);
    _snapshotted = false;
    _funclog();
}

} // namespace Inkscape

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4 :

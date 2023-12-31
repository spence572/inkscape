// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef SNAPENUMS_H_
#define SNAPENUMS_H_
/*
 * Authors:
 *   Diederik van Lierop <mail@diedenrezi.nl>
 *
 * Copyright (C) 2010 - 2012 Authors
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

namespace Inkscape {

/**
 * enumerations of snap source types and snap target types.
 */
enum SnapSourceType { // When adding source types here, then also update Inkscape::SnapPreferences::source2target!
    SNAPSOURCE_UNDEFINED = 0,
    //-------------------------------------------------------------------
    // Bbox points can be located at the edge of the stroke (for visual bboxes); they will therefore not snap
    // to nodes because these are always located at the center of the stroke
    SNAPSOURCE_BBOX_CATEGORY = 16, // will be used as a flag and must therefore be a power of two. Also,
                                   // must be larger than the largest number of targets in a single group
    SNAPSOURCE_BBOX_CORNER,
    SNAPSOURCE_BBOX_MIDPOINT,
    SNAPSOURCE_BBOX_EDGE_MIDPOINT,

    // Allow PAGES to be moved as BBOX (enabled alignment snapping)
    SNAPSOURCE_PAGE_CENTER,
    SNAPSOURCE_PAGE_CORNER,

    //-------------------------------------------------------------------
    // For the same reason, nodes will not snap to bbox points
    SNAPSOURCE_NODE_CATEGORY = 32, // will be used as a flag and must therefore be a power of two
    SNAPSOURCE_NODE_SMOOTH, // Symmetrical nodes are also considered to be smooth; there's no dedicated type for symm. nodes
    SNAPSOURCE_NODE_CUSP,
    SNAPSOURCE_LINE_MIDPOINT,
    SNAPSOURCE_PATH_INTERSECTION,
    SNAPSOURCE_RECT_CORNER, // of a rectangle, so at the center of the stroke
    SNAPSOURCE_CONVEX_HULL_CORNER,
    SNAPSOURCE_ELLIPSE_QUADRANT_POINT,
    SNAPSOURCE_NODE_HANDLE, // eg. nodes in the path editor, handles of stars or rectangles, etc. (tied to a stroke)
    //-------------------------------------------------------------------
    // Other points (e.g. guides) will snap to both bounding boxes and nodes
    SNAPSOURCE_DATUMS_CATEGORY = 64, // will be used as a flag and must therefore be a power of two
    SNAPSOURCE_GUIDE,
    SNAPSOURCE_GUIDE_ORIGIN,
    //-------------------------------------------------------------------
    // Other points (e.g. gradient knots, image corners) will snap to both bounding boxes and nodes
    SNAPSOURCE_OTHERS_CATEGORY = 128, // will be used as a flag and must therefore be a power of two
    SNAPSOURCE_ROTATION_CENTER,
    SNAPSOURCE_OBJECT_MIDPOINT, // midpoint of rectangles, ellipses, polygon, etc.
    SNAPSOURCE_IMG_CORNER,
    SNAPSOURCE_TEXT_ANCHOR,
    SNAPSOURCE_OTHER_HANDLE, // eg. the handle of a gradient or of a connector (ie not being tied to a stroke)
    SNAPSOURCE_GRID_PITCH, // eg. when pasting or alt-dragging in the selector tool; not really a snap source

    //-------------------------------------------------------------------
    // Alignment snapping
    SNAPSOURCE_ALIGNMENT_CATEGORY = 256,
    SNAPSOURCE_ALIGNMENT_BBOX_CORNER,
    SNAPSOURCE_ALIGNMENT_BBOX_MIDPOINT,
    SNAPSOURCE_ALIGNMENT_BBOX_EDGE_MIDPOINT,
    SNAPSOURCE_ALIGNMENT_PAGE_CENTER,
    SNAPSOURCE_ALIGNMENT_PAGE_CORNER,
    SNAPSOURCE_ALIGNMENT_HANDLE
};

enum SnapTargetType {
    SNAPTARGET_UNDEFINED = 0,
    //-------------------------------------------------------------------
    SNAPTARGET_BBOX_CATEGORY = 16, // will be used as a flag and must therefore be a power of two. Also,
                                   // must be larger than the largest number of targets in a single group
                                   // i.e > 15 because that's the number of targets in the "others" group
    SNAPTARGET_BBOX_CORNER,
    SNAPTARGET_BBOX_EDGE,
    SNAPTARGET_BBOX_EDGE_MIDPOINT,
    SNAPTARGET_BBOX_MIDPOINT,
    //-------------------------------------------------------------------
    SNAPTARGET_NODE_CATEGORY = 32, // will be used as a flag and must therefore be a power of two
    SNAPTARGET_NODE_SMOOTH,
    SNAPTARGET_NODE_CUSP,
    SNAPTARGET_LINE_MIDPOINT,
    SNAPTARGET_PATH,    // If path targets are added here, then also add them to the list in findBestSnap()
    SNAPTARGET_PATH_PERPENDICULAR,
    SNAPTARGET_PATH_TANGENTIAL,
    SNAPTARGET_PATH_INTERSECTION,
    SNAPTARGET_PATH_GUIDE_INTERSECTION,
    SNAPTARGET_PATH_CLIP,
    SNAPTARGET_PATH_MASK,
    SNAPTARGET_ELLIPSE_QUADRANT_POINT, // this corner is at the center of the stroke
    SNAPTARGET_RECT_CORNER, // of a rectangle, so this corner is at the center of the stroke
    //-------------------------------------------------------------------
    SNAPTARGET_DATUMS_CATEGORY = 64, // will be used as a flag and must therefore be a power of two
    SNAPTARGET_GRID,
    SNAPTARGET_GRID_LINE,
    SNAPTARGET_GRID_INTERSECTION,
    SNAPTARGET_GRID_PERPENDICULAR,
    SNAPTARGET_GUIDE,
    SNAPTARGET_GUIDE_INTERSECTION,
    SNAPTARGET_GUIDE_ORIGIN,
    SNAPTARGET_GUIDE_PERPENDICULAR,
    SNAPTARGET_GRID_GUIDE_INTERSECTION,
    SNAPTARGET_PAGE_EDGE_BORDER,
    SNAPTARGET_PAGE_EDGE_CENTER,
    SNAPTARGET_PAGE_EDGE_CORNER,
    SNAPTARGET_PAGE_MARGIN_BORDER,
    SNAPTARGET_PAGE_MARGIN_CENTER,
    SNAPTARGET_PAGE_MARGIN_CORNER,
    SNAPTARGET_PAGE_BLEED_BORDER,
    SNAPTARGET_PAGE_BLEED_CORNER,
    //-------------------------------------------------------------------
    SNAPTARGET_OTHERS_CATEGORY = 128, // will be used as a flag and must therefore be a power of two
    SNAPTARGET_OBJECT_MIDPOINT,
    SNAPTARGET_IMG_CORNER,
    SNAPTARGET_ROTATION_CENTER,
    SNAPTARGET_TEXT_ANCHOR,
    SNAPTARGET_TEXT_BASELINE,
    SNAPTARGET_CONSTRAINED_ANGLE,
    SNAPTARGET_CONSTRAINT,

    //-------------------------------------------------------------------
    // Alignment snapping
    SNAPTARGET_ALIGNMENT_CATEGORY = 256, // will be used as a flag and must therefore be a power of two
    SNAPTARGET_ALIGNMENT_BBOX_CORNER,
    SNAPTARGET_ALIGNMENT_BBOX_MIDPOINT,
    SNAPTARGET_ALIGNMENT_BBOX_EDGE_MIDPOINT,
    SNAPTARGET_ALIGNMENT_PAGE_EDGE_CENTER,
    SNAPTARGET_ALIGNMENT_PAGE_EDGE_CORNER,
    SNAPTARGET_ALIGNMENT_PAGE_MARGIN_CENTER,
    SNAPTARGET_ALIGNMENT_PAGE_MARGIN_CORNER,
    SNAPTARGET_ALIGNMENT_PAGE_BLEED_CORNER,
    SNAPTARGET_ALIGNMENT_HANDLE,
    SNAPTARGET_ALIGNMENT_INTERSECTION,

    //-------------------------------------------------------------------
    // Distribution snapping
    SNAPTARGET_DISTRIBUTION_CATEGORY = 512, // will be used as a flag and must therefore be a power of two
    SNAPTARGET_DISTRIBUTION_X,
    SNAPTARGET_DISTRIBUTION_Y,
    SNAPTARGET_DISTRIBUTION_RIGHT,
    SNAPTARGET_DISTRIBUTION_LEFT,
    SNAPTARGET_DISTRIBUTION_UP,
    SNAPTARGET_DISTRIBUTION_DOWN,
    SNAPTARGET_DISTRIBUTION_XY,

    //-------------------------------------------------------------------
    SNAPTARGET_MAX_ENUM_VALUE
};

// simple snapping UI hides variety of choices behind a few categories
enum class SimpleSnap {
    BBox = 0,   // bounding box category
    Nodes,      // nodes, paths
    Alignment,  // alignment and distribution snaps
    Rest,       // all the rest
    _MaxEnumValue
};

}
#endif /* SNAPENUMS_H_ */

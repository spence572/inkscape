// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef INKSCAPE_LPEGROUPBBOX_H
#define INKSCAPE_LPEGROUPBBOX_H

/*
 * Inkscape::LivePathEffect_group_bbox
 *
 * Copyright (C) Steren Giannini 2008 <steren.giannini@gmail.com>
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

class SPLPEItem;

#include <2geom/affine.h>    // for identity, Affine (ptr only)
#include <2geom/interval.h>  // for Interval

namespace Inkscape {
namespace LivePathEffect {

class GroupBBoxEffect {
protected:
    // Bounding box of the item the path effect is applied on
    Geom::Interval boundingbox_X;
    Geom::Interval boundingbox_Y;

    //This sets boundingbox_X and boundingbox_Y
    Geom::OptRect clip_mask_bbox(SPLPEItem * item, Geom::Affine transform);
    void original_bbox(SPLPEItem const* lpeitem, bool absolute = false, bool clip_mask = false, Geom::Affine base_transform = Geom::identity());
};

}; //namespace LivePathEffect
}; //namespace Inkscape

#endif

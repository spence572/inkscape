// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) Steren Giannini 2008 <steren.giannini@gmail.com>
 *   Abhishek Sharma
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include "lpegroupbbox.h"

#include "object/sp-clippath.h"
#include "object/sp-mask.h"
#include "object/sp-item-group.h"
#include "object/sp-lpe-item.h"

namespace Inkscape {
namespace LivePathEffect {

/**
 * Updates the \c boundingbox_X and \c boundingbox_Y values from the geometric bounding box of \c lpeitem.
 *
 * @pre   lpeitem must have an existing geometric boundingbox (usually this is guaranteed when: \code cast<SPShape>(lpeitem)->curve != NULL \endcode )
 *        It's not possible to run LPEs on items without their original-d having a bbox.
 * @param lpeitem   This is not allowed to be NULL.
 * @param absolute  Determines whether the bbox should be calculated of the untransformed lpeitem (\c absolute = \c false)
 *                  or of the transformed lpeitem (\c absolute = \c true) using sp_item_i2doc_affine.
 * @post Updated values of boundingbox_X and boundingbox_Y. These intervals are set to empty intervals when the precondition is not met.
 */

Geom::OptRect
GroupBBoxEffect::clip_mask_bbox(SPLPEItem *item, Geom::Affine transform)
{
    Geom::OptRect bbox;
    Geom::Affine affine = transform * item->transform;
    SPClipPath *clip_path = item->getClipObject();
    if(clip_path) {
        bbox.unionWith(clip_path->geometricBounds(affine));
    }
    SPMask * mask_path = item->getMaskObject();
    if(mask_path) {
        bbox.unionWith(mask_path->visualBounds(affine));
    }
    auto group = cast<SPGroup>(item);
    if (group) {
        std::vector<SPItem*> item_list = group->item_list();
        for (auto iter : item_list) {
            auto subitem = cast<SPLPEItem>(iter);
            if (subitem) {
                bbox.unionWith(clip_mask_bbox(subitem, affine));
            }
        }
    }
    return bbox;
}

void GroupBBoxEffect::original_bbox(SPLPEItem const* lpeitem, bool absolute, bool clip_mask, Geom::Affine base_transform)
{
    // Get item bounding box
    Geom::Affine transform;
    if (absolute) {
        transform = lpeitem->i2doc_affine();
    }
    else {
        transform = base_transform;
    }
    
    Geom::OptRect bbox = lpeitem->geometricBounds(transform);
    if (clip_mask) {
        SPLPEItem * item = const_cast<SPLPEItem *>(lpeitem);
        bbox.unionWith(clip_mask_bbox(item, transform * item->transform.inverse()));
    }
    if (bbox) {
        boundingbox_X = (*bbox)[Geom::X];
        boundingbox_Y = (*bbox)[Geom::Y];
    } else {
        boundingbox_X = Geom::Interval();
        boundingbox_Y = Geom::Interval();
    }
}

} // namespace LivePathEffect
} /* namespace Inkscape */

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

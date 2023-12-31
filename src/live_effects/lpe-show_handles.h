// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef INKSCAPE_LPE_SHOW_HANDLES_H
#define INKSCAPE_LPE_SHOW_HANDLES_H

/*
 * Authors:
 *   Jabier Arraiza Cenoz
 *
 * Copyright (C) Jabier Arraiza Cenoz 2014 <jabier.arraiza@marker.es>
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */
#include "helper/geom-nodetype.h"
#include "live_effects/effect.h"
#include "live_effects/lpegroupbbox.h"
#include "live_effects/parameter/bool.h"

namespace Inkscape {
namespace LivePathEffect {

class LPEShowHandles : public Effect , GroupBBoxEffect {

public:
    LPEShowHandles(LivePathEffectObject *lpeobject);
    ~LPEShowHandles() override = default;

    void doOnApply(SPLPEItem const* lpeitem) override;

    void doBeforeEffect (SPLPEItem const* lpeitem) override;

    virtual void generateHelperPath(Geom::PathVector result);

    virtual void drawNode(Geom::Point p, Geom::NodeType nodetype);

    virtual void drawHandle(Geom::Point p);

    virtual void drawHandleLine(Geom::Point p,Geom::Point p2);

protected:

    Geom::PathVector doEffect_path (Geom::PathVector const & path_in) override;

private:

    BoolParam nodes;
    BoolParam handles;
    BoolParam original_path;
    BoolParam original_d;
    BoolParam show_center_node;
    ScalarParam scale_nodes_and_handles;
    double stroke_width;

    Geom::PathVector outline_path;

    LPEShowHandles(const LPEShowHandles &) = delete;
    LPEShowHandles &operator=(const LPEShowHandles &) = delete;

};

}; //namespace LivePathEffect
}; //namespace Inkscape
#endif

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4 :

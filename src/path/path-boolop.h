// SPDX-License-Identifier: GPL-2.0-or-later
/** @file
 * Boolean operations.
 *//*
 * Authors: see git history
 *
 * Copyright (C) 2018 Authors
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */
#ifndef PATH_BOOLOP_H
#define PATH_BOOLOP_H

#include <vector>

#include <2geom/forward.h>

#include "livarot/LivarotDefs.h" // FillRule, BooleanOp

/// Flatten a pathvector according to the given fill rule.
Geom::PathVector flattened(Geom::PathVector const &pathv, FillRule fill_rule);
void sp_flatten(Geom::PathVector &pathv, FillRule fill_rule);

/// Cut a pathvector along a collection of lines into several smaller pathvectors.
std::vector<Geom::PathVector> pathvector_cut(Geom::PathVector const &pathv, Geom::PathVector const &lines);

/// Perform a boolean operation on two pathvectors.
Geom::PathVector sp_pathvector_boolop(Geom::PathVector const &pathva, Geom::PathVector const &pathvb, BooleanOp bop,
                                      FillRule fra, FillRule frb, bool livarotonly, bool flattenbefore, bool &error);
Geom::PathVector sp_pathvector_boolop(Geom::PathVector const &pathva, Geom::PathVector const &pathvb, BooleanOp bop,
                                      FillRule fra, FillRule frb, bool livarotonly = false, bool flattenbefore = true);

#endif // PATH_BOOLOP_H

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

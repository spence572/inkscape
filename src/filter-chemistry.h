// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Various utility methods for filters
 *
 * Authors:
 *   Hugo Rodrigues
 *   bulia byak
 *   Niko Kiirala
 *
 * Copyright (C) 2006,2007 authors
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#ifndef SEEN_SP_FILTER_CHEMISTRY_H
#define SEEN_SP_FILTER_CHEMISTRY_H

#include "display/nr-filter-types.h"
#include "style-enums.h"

class SPDocument;
class SPFilter;
class SPFilterPrimitive;
class SPItem;
class SPObject;

SPFilterPrimitive *filter_add_primitive(SPFilter *filter, Inkscape::Filters::FilterPrimitiveType);
SPFilter *new_filter (SPDocument *document);
SPFilter *new_filter_gaussian_blur (SPDocument *document, double stdDeviation, double expansion);
SPFilter *new_filter_simple_from_item (SPDocument *document, SPItem *item, const char *mode, double stdDeviation);
SPFilter *modify_filter_gaussian_blur_from_item (SPDocument *document, SPItem *item, double stdDeviation);
void remove_filter (SPObject *item, bool recursive);
void remove_hidder_filter (SPObject *item);
bool has_hidder_filter(SPObject const *item);
void remove_filter_gaussian_blur (SPObject *item);
void remove_filter_legacy_blend(SPObject *item);
SPBlendMode filter_get_legacy_blend(SPObject *item);
bool filter_is_single_gaussian_blur(SPFilter *filter);
double get_single_gaussian_blur_radius(SPFilter *filter);
bool set_blend_mode(SPItem* item, SPBlendMode mode);

#endif

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

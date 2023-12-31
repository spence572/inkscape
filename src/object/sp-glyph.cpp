// SPDX-License-Identifier: GPL-2.0-or-later
#ifdef HAVE_CONFIG_H
#endif

/*
 * SVG <glyph> element implementation
 *
 * Author:
 *   Felipe C. da S. Sanches <juca@members.fsf.org>
 *   Abhishek Sharma
 *
 * Copyright (C) 2008, Felipe C. da S. Sanches
 *
 * Released under GNU GPL v2+, read the file 'COPYING' for more information.
 */

#include "sp-glyph.h"

#include "attributes.h"        // for SPAttr
#include "object/sp-object.h"  // for SP_OBJECT_MODIFIED_FLAG, SPObject, SPC...
#include "xml/document.h"      // for Document
#include "xml/node.h"          // for Node

class SPDocument;

SPGlyph::SPGlyph()
    : SPObject() 
//TODO: correct these values:
    , d(nullptr)
    , orientation(GLYPH_ORIENTATION_BOTH)
    , arabic_form(GLYPH_ARABIC_FORM_INITIAL)
    , lang(nullptr)
    , horiz_adv_x(0)
    , vert_origin_x(0)
    , vert_origin_y(0)
    , vert_adv_y(0)
{
}

void SPGlyph::build(SPDocument *document, Inkscape::XML::Node *repr)
{
    SPObject::build(document, repr);

    this->readAttr(SPAttr::UNICODE);
    this->readAttr(SPAttr::GLYPH_NAME);
    this->readAttr(SPAttr::D);
    this->readAttr(SPAttr::ORIENTATION);
    this->readAttr(SPAttr::ARABIC_FORM);
    this->readAttr(SPAttr::LANG);
    this->readAttr(SPAttr::HORIZ_ADV_X);
    this->readAttr(SPAttr::VERT_ORIGIN_X);
    this->readAttr(SPAttr::VERT_ORIGIN_Y);
    this->readAttr(SPAttr::VERT_ADV_Y);
}

void SPGlyph::release() {
    SPObject::release();
}

static glyphArabicForm sp_glyph_read_arabic_form(gchar const *value){
    if (!value) {
    	return GLYPH_ARABIC_FORM_INITIAL; //TODO: verify which is the default default (for me, the spec is not clear)
    }
    
    switch(value[0]){
        case 'i':
            if (strncmp(value, "initial", 7) == 0) {
            	return GLYPH_ARABIC_FORM_INITIAL;
            }
            
            if (strncmp(value, "isolated", 8) == 0) {
            	return GLYPH_ARABIC_FORM_ISOLATED;
            }
            break;
        case 'm':
            if (strncmp(value, "medial", 6) == 0) {
            	return GLYPH_ARABIC_FORM_MEDIAL;
            }
            break;
        case 't':
            if (strncmp(value, "terminal", 8) == 0) {
            	return GLYPH_ARABIC_FORM_TERMINAL;
            }
            break;
    }
    
    return GLYPH_ARABIC_FORM_INITIAL; //TODO: VERIFY DEFAULT!
}

static glyphOrientation sp_glyph_read_orientation(gchar const *value)
{
    if (!value) {
    	return GLYPH_ORIENTATION_BOTH;
    }
    
    switch(value[0]){
        case 'h':
            return GLYPH_ORIENTATION_HORIZONTAL;
            break;
        case 'v':
            return GLYPH_ORIENTATION_VERTICAL;
            break;
    }
    
//ERROR? TODO: VERIFY PROPER ERROR HANDLING
    return GLYPH_ORIENTATION_BOTH;
}

void SPGlyph::set(SPAttr key, const gchar *value)
{
    switch (key) {
        case SPAttr::UNICODE:
        {
            this->unicode.clear();
            
            if (value) {
            	this->unicode.append(value);
            }
            
            this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            break;
        }
        case SPAttr::GLYPH_NAME:
        {
            this->glyph_name.clear();
            
            if (value) {
            	this->glyph_name.append(value);
            }
            
            this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            break;
        }
        case SPAttr::D:
        {
            if (this->d) {
            	g_free(this->d);
            }
            
            this->d = g_strdup(value);
            this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            break;
        }
        case SPAttr::ORIENTATION:
        {
            glyphOrientation orient = sp_glyph_read_orientation(value);
            
            if (this->orientation != orient){
                this->orientation = orient;
                this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            }
            break;
        }
        case SPAttr::ARABIC_FORM:
        {
            glyphArabicForm form = sp_glyph_read_arabic_form(value);
            
            if (this->arabic_form != form){
                this->arabic_form = form;
                this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            }
            break;
        }
        case SPAttr::LANG:
        {
            if (this->lang) {
            	g_free(this->lang);
            }
            
            this->lang = g_strdup(value);
            this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            break;
        }
        case SPAttr::HORIZ_ADV_X:
        {
            double number = value ? g_ascii_strtod(value, nullptr) : 0;
            
            if (number != this->horiz_adv_x){
                this->horiz_adv_x = number;
                this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            }
            break;
        }
        case SPAttr::VERT_ORIGIN_X:
        {
            double number = value ? g_ascii_strtod(value, nullptr) : 0;
            
            if (number != this->vert_origin_x){
                this->vert_origin_x = number;
                this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            }
            break;
        }
        case SPAttr::VERT_ORIGIN_Y:
        {
            double number = value ? g_ascii_strtod(value, nullptr) : 0;
            
            if (number != this->vert_origin_y){
                this->vert_origin_y = number;
                this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            }
            break;
        }
        case SPAttr::VERT_ADV_Y:
        {
            double number = value ? g_ascii_strtod(value, nullptr) : 0;
            
            if (number != this->vert_adv_y){
                this->vert_adv_y = number;
                this->requestModified(SP_OBJECT_MODIFIED_FLAG);
            }
            break;
        }
        default:
        {
        	SPObject::set(key, value);
            break;
        }
    }
}

/**
 * Receives update notifications.
 */
void SPGlyph::update(SPCtx *ctx, guint flags)
{
    if (flags & SP_OBJECT_MODIFIED_FLAG) {
        /* do something to trigger redisplay, updates? */
        this->readAttr(SPAttr::UNICODE);
        this->readAttr(SPAttr::GLYPH_NAME);
        this->readAttr(SPAttr::D);
        this->readAttr(SPAttr::ORIENTATION);
        this->readAttr(SPAttr::ARABIC_FORM);
        this->readAttr(SPAttr::LANG);
        this->readAttr(SPAttr::HORIZ_ADV_X);
        this->readAttr(SPAttr::VERT_ORIGIN_X);
        this->readAttr(SPAttr::VERT_ORIGIN_Y);
        this->readAttr(SPAttr::VERT_ADV_Y);
    }

    SPObject::update(ctx, flags);
}

#define COPY_ATTR(rd,rs,key) (rd)->setAttribute((key), rs->attribute(key));

Inkscape::XML::Node* SPGlyph::write(Inkscape::XML::Document *xml_doc, Inkscape::XML::Node *repr, guint flags)
{
    if ((flags & SP_OBJECT_WRITE_BUILD) && !repr) {
        repr = xml_doc->createElement("svg:glyph");
    }

    /* I am commenting out this part because I am not certain how does it work. I will have to study it later. Juca
    repr->setAttribute("unicode", glyph->unicode);
    repr->setAttribute("glyph-name", glyph->glyph_name);
    repr->setAttribute("d", glyph->d);
    repr->setAttributeSvgDouble("orientation", (double) glyph->orientation);
    repr->setAttributeSvgDouble("arabic-form", (double) glyph->arabic_form);
    repr->setAttribute("lang", glyph->lang);
    repr->setAttributeSvgDouble("horiz-adv-x", glyph->horiz_adv_x);
    repr->setAttributeSvgDouble("vert-origin-x", glyph->vert_origin_x);
    repr->setAttributeSvgDouble("vert-origin-y", glyph->vert_origin_y);
    repr->setAttributeSvgDouble("vert-adv-y", glyph->vert_adv_y);
    */

    if (repr != this->getRepr()) {
        // All the COPY_ATTR functions below use
        // XML Tree directly while they shouldn't.
        COPY_ATTR(repr, this->getRepr(), "unicode");
        COPY_ATTR(repr, this->getRepr(), "glyph-name");
        COPY_ATTR(repr, this->getRepr(), "d");
        COPY_ATTR(repr, this->getRepr(), "orientation");
        COPY_ATTR(repr, this->getRepr(), "arabic-form");
        COPY_ATTR(repr, this->getRepr(), "lang");
        COPY_ATTR(repr, this->getRepr(), "horiz-adv-x");
        COPY_ATTR(repr, this->getRepr(), "vert-origin-x");
        COPY_ATTR(repr, this->getRepr(), "vert-origin-y");
        COPY_ATTR(repr, this->getRepr(), "vert-adv-y");
    }

    SPObject::write(xml_doc, repr, flags);

    return repr;
}

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

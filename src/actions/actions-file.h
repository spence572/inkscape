// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Gio::Actions for file handling tied to the application and without GUI.
 *
 * Copyright (C) 2020 Tavmjong Bah
 *
 * The contents of this file may be used under the GNU General Public License Version 2 or later.
 *
 */

#ifndef INK_ACTIONS_FILE_H
#define INK_ACTIONS_FILE_H

class InkscapeApplication;

void add_actions_file(InkscapeApplication* app);

#endif // INK_ACTIONS_FILE_H

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

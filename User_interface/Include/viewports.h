/* ----------------------------------------------------------------------------
@COPYRIGHT  :
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

#ifndef  DEF_VIEWPORTS
#define  DEF_VIEWPORTS

typedef  enum
{
    Main_menu_viewport,
    RMS_error_viewport,
    Volume_1_tags_viewport,
    Volume_2_tags_viewport,
    Tag_names_viewport,
    Volume_1_menu_viewport,
    Volume_2_menu_viewport,
    Merged_menu_viewport,
    Main_volume_1_separator_viewport,
    Volume_1_2_separator_viewport,
    Volume_2_merged_separator_viewport,
    Slice_1_2_separator_viewport,
    Slice_2_3_separator_viewport,
    Slice_3_menu_separator_viewport,
    Tag_volume_menu_separator_viewport,
    N_UI_viewports,
    Whole_window_event_viewport = N_UI_viewports,
    N_VIEWPORT_TYPES
} Viewport_types;

#endif

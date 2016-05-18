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

#define Main_menu_viewport 0
#define RMS_error_viewport 1
#define Volume_1_tags_viewport 2
#define Volume_1_menu_viewport (Volume_1_tags_viewport + N_VOLUMES_DISPLAYED)
#define Volume_1_horz_sep_viewport (Volume_1_menu_viewport + N_VOLUMES_DISPLAYED)
#define Slice_1_vert_sep_viewport (Volume_1_horz_sep_viewport + N_VOLUMES_DISPLAYED)
#define Whole_window_event_viewport (Slice_1_vert_sep_viewport + N_VIEWS + 1)
#define N_VIEWPORT_TYPES (Whole_window_event_viewport+1)

#define Tag_names_viewport (Volume_1_tags_viewport + MERGED_VOLUME_INDEX)
#define Merged_menu_viewport (Volume_1_menu_viewport + MERGED_VOLUME_INDEX)

typedef unsigned int Viewport_types;
#if 0
/* Old viewport definitions */
typedef  enum
{
    Main_menu_viewport,
    RMS_error_viewport,
    /* tags, per volume */
    Volume_1_tags_viewport,
    Volume_2_tags_viewport,
    Volume_3_tags_viewport,
    Tag_names_viewport,
    /* menu, per volume */
    Volume_1_menu_viewport,
    Volume_2_menu_viewport,
    Volume_3_menu_viewport,
    Merged_menu_viewport,
    /* separators, per volume */
    Main_volume_1_separator_viewport,
    Volume_1_2_separator_viewport,
    Volume_2_merged_separator_viewport,
    Volume_3_merged_separator_viewport,
    /* separators, per slice view */
    Slice_1_2_separator_viewport,
    Slice_2_3_separator_viewport,
    Slice_3_menu_separator_viewport,
    Tag_volume_menu_separator_viewport,
    Whole_window_event_viewport,
    N_VIEWPORT_TYPES
} Viewport_types;
#endif

extern unsigned int N_UI_viewports;

#endif

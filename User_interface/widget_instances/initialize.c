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

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/widget_instances/initialize.c,v 1.10 1998-06-29 15:02:03 david Exp $";
#endif

#include  <user_interface.h>

public  void  initialize_UI_widgets( UI_struct  *ui_info )
{
    Viewport_types   viewport_index;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        initialize_widget_list( &ui_info->widget_list[viewport_index] );

#ifndef DEBUG
    add_main_widgets( ui_info );
#endif

    add_volume_widgets( ui_info, Volume_1_menu_viewport );
    add_volume_widgets( ui_info, Volume_2_menu_viewport );

    add_merged_widgets( ui_info, Merged_menu_viewport );

    ui_info->tag_points.n_tags_displayed = Default_n_tag_points;
    ui_info->tag_points.first_tag_displayed = 0;
    ui_info->tag_points.current_tag_index = 0;

#ifndef DEBUG
    add_tag_point_widgets( ui_info, ui_info->tag_points.n_tags_displayed,
                           RMS_error_viewport, Volume_1_tags_viewport,
                           Volume_2_tags_viewport, Tag_names_viewport );
#endif
}

public  void  delete_UI_widgets( UI_struct  *ui_info )
{
    Viewport_types   viewport_index;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        delete_widget_list( &ui_info->widget_list[viewport_index] );

#ifndef DEBUG
    delete_tag_point_widgets_indices();
#endif
}

public  void  update_all_widget_colours( UI_struct *ui_info )
{
    Viewport_types   viewport_index;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        update_widget_list_colours( &ui_info->widget_list[viewport_index] );
}

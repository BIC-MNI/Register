/**
 * \file User_interface/widget_instances/initialize.c
 * \brief Start widget creation.
 *
 * \copyright
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
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif //HAVE_CONFIG_H

#include  <user_interface.h>

  void  initialize_UI_widgets( UI_struct  *ui_info )
{
    Viewport_types   viewport_index;
    int i;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        initialize_widget_list( &ui_info->widget_list[viewport_index] );

#ifndef DEBUG
    add_main_widgets( ui_info );
#endif

    for (i = 0; i < ui_info->n_volumes_displayed; i++)
    {
      viewport_index = get_volume_menu_viewport_index( i );
      if (i == MERGED_VOLUME_INDEX)
          add_merged_widgets( ui_info, viewport_index );
      else
          add_volume_widgets( ui_info, viewport_index );
    }


    ui_info->tag_points.n_tags_displayed = Default_n_tag_points;
    ui_info->tag_points.first_tag_displayed = 0;
    ui_info->tag_points.current_tag_index = 0;

#ifndef DEBUG
    add_tag_point_widgets( ui_info, ui_info->tag_points.n_tags_displayed,
                           RMS_error_viewport, Volume_1_tags_viewport,
                           -1, Tag_names_viewport );
#endif
}

  void  delete_UI_widgets( UI_struct  *ui_info )
{
    Viewport_types   viewport_index;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        delete_widget_list( &ui_info->widget_list[viewport_index] );

#ifndef DEBUG
    delete_tag_point_widgets_indices();
#endif
}

  void  update_all_widget_colours( UI_struct *ui_info )
{
    Viewport_types   viewport_index;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        update_widget_list_colours( &ui_info->widget_list[viewport_index] );
}

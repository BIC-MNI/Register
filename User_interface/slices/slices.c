/**
 * \file User_interface/slices/slices.c
 * \brief Handle some basic UI tasks for the slice views.
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

  void  update_position_counters(
    UI_struct  *ui_info,
    int        volume_index )
{
    VIO_Real   voxel_position[VIO_N_DIMENSIONS];
    VIO_Real   original_world_position[VIO_N_DIMENSIONS];
    VIO_Real   tpos;
    int    axis;

    IF_get_volume_voxel_position( volume_index, voxel_position );
    IF_get_volume_original_world_position( volume_index,
                                           original_world_position );

    for_less( axis, 0, VIO_N_DIMENSIONS )
    {
        set_volume_voxel_text( ui_info, volume_index, axis,
                               voxel_position[axis] );
        set_volume_world_text( ui_info, volume_index, axis,
                               original_world_position[axis] );
    }

    IF_get_volume_time_position(volume_index, &tpos);
    set_volume_time_text(ui_info, volume_index, tpos);

    update_volume_readout( ui_info, volume_index );
}

  void  update_other_volume_positions(
    UI_struct   *ui_info,
    int         volume_index )
{
    int   i;
    VIO_Real  position[VIO_N_DIMENSIONS];

    if( ui_info->volumes_synced && IF_volume_is_loaded(volume_index) )
    {
        IF_get_volume_world_position( volume_index, position );

        for_less( i, 0, ui_info->n_volumes_displayed )
        {
            if( i != volume_index && IF_volume_is_loaded( i ) )
            {
                IF_set_volume_world_position( i, position );

                update_position_counters( ui_info, i );
            }
        }
    }
}

  void  ui_set_volume_voxel_position(
    UI_struct  *ui_info,
    int        volume_index,
    VIO_Real   position[] )
{
    IF_set_volume_voxel_position( volume_index, position );

    update_position_counters( ui_info, volume_index );

    update_other_volume_positions( ui_info, volume_index );
}


 void ui_set_volume_time_position(UI_struct *ui_info,
                                        int volume_index,
                                        VIO_Real tpos)
{
    IF_set_volume_time_position( volume_index, tpos );

    update_position_counters( ui_info, volume_index );
}


  void  ui_set_volume_original_world_position(
    UI_struct  *ui_info,
    int        volume_index,
    VIO_Real   position[] )
{
    IF_set_volume_original_world_position( volume_index, position );

    update_position_counters( ui_info, volume_index );

    update_other_volume_positions( ui_info, volume_index );
}

  Viewport_types  get_volume_menu_viewport_index(
    int   volume_index )
{
    if (volume_index <= MERGED_VOLUME_INDEX)
      return Volume_1_menu_viewport + volume_index;
    return Volume_1_menu_viewport;
}

  int  get_viewport_volume_index(
    Viewport_types   viewport_index )
{
    return (viewport_index - Volume_1_menu_viewport);
}

  void  set_merged_activity(
    UI_struct  *ui_info,
    VIO_BOOL    activity )
{
    VIO_BOOL  changed;

    changed = ( activity != IF_get_merged_slice_visibility());

    IF_set_merged_slice_visibility( activity );

    if( changed )
    {
        set_merged_widgets_activity( ui_info, activity );

        if( activity )
        {
            install_slice_events( &ui_info->graphics_window.event_viewports,
                                  MERGED_VOLUME_INDEX );

            update_position_counters( ui_info, MERGED_VOLUME_INDEX );
        }
        else
            remove_slice_events( &ui_info->graphics_window.event_viewports,
                                 MERGED_VOLUME_INDEX );
    }
}

  VIO_BOOL   get_voxel_under_mouse(
    UI_struct        *ui_info,
    Viewport_types   event_viewport_index,
    int              *volume,
    VIO_Real             voxel_position[VIO_N_DIMENSIONS] )
{
    int        view, x_mouse, y_mouse;

    get_viewport_mouse_position( &ui_info->graphics_window,
                                 event_viewport_index,
                                 &x_mouse, &y_mouse );

    ui_get_volume_view_index( event_viewport_index, volume, &view);

    return( IF_convert_pixel_to_voxel( *volume, view, x_mouse, y_mouse,
                                       voxel_position ) );
}

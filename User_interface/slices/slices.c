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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/slices/slices.c,v 1.5 1996-12-09 20:21:57 david Exp $";
#endif

#include  <user_interface.h>

public  void  update_position_counters(
    UI_struct  *ui_info,
    int        volume_index )
{
    Real   voxel_position[N_DIMENSIONS];
    Real   original_world_position[N_DIMENSIONS];
    int    axis;

    IF_get_volume_voxel_position( volume_index, voxel_position );
    IF_get_volume_original_world_position( volume_index,
                                           original_world_position );

    for_less( axis, 0, N_DIMENSIONS )
    {
        set_volume_voxel_text( ui_info, volume_index, axis,
                               voxel_position[axis] );
        set_volume_world_text( ui_info, volume_index, axis,
                               original_world_position[axis] );
    }

    update_volume_readout( ui_info, volume_index );
}

public  void  update_other_volume_positions(
    UI_struct   *ui_info,
    int         volume_index )
{
    int   i;
    Real  position[N_DIMENSIONS];

    if( ui_info->volumes_synced && IF_volume_is_loaded(volume_index) )
    {
        IF_get_volume_world_position( volume_index, position );

        for_less( i, 0, N_VOLUMES_DISPLAYED )
        {
            if( i != volume_index && IF_volume_is_loaded( i ) )
            {
                IF_set_volume_world_position( i, position );

                update_position_counters( ui_info, i );
            }
        }
    }
}

public  void  ui_set_volume_voxel_position(
    UI_struct  *ui_info,
    int        volume_index,
    Real       position[N_DIMENSIONS] )
{
    IF_set_volume_voxel_position( volume_index, position );

    update_position_counters( ui_info, volume_index );

    update_other_volume_positions( ui_info, volume_index );
}

public  void  ui_set_volume_original_world_position(
    UI_struct  *ui_info,
    int        volume_index,
    Real       position[N_DIMENSIONS] )
{
    IF_set_volume_original_world_position( volume_index, position );

    update_position_counters( ui_info, volume_index );

    update_other_volume_positions( ui_info, volume_index );
}

public  Viewport_types  get_volume_menu_viewport_index(
    int   volume_index )
{
    switch( volume_index )
    {
    case 0:    return( Volume_1_menu_viewport );
    case 1:    return( Volume_2_menu_viewport );
    case 2:    return( Merged_menu_viewport );
    default:   return( Volume_1_menu_viewport );
    }
}

public  int  get_viewport_volume_index(
    Viewport_types   viewport_index )
{
    switch( viewport_index )
    {
    case Volume_1_menu_viewport:    return( 0 );
    case Volume_2_menu_viewport:    return( 1 );
    case Merged_menu_viewport:      return( 2 );
    default:                        return( 0 );
    }
}

public  void  set_merged_activity(
    UI_struct  *ui_info,
    BOOLEAN    activity )
{
    BOOLEAN  changed;

    changed = ( activity != IF_get_merged_slice_visibility());

    IF_set_merged_slice_visibility( activity );

    if( changed )
    {
        set_merged_widgets_activity( ui_info, activity );

        if( activity )
        {
            install_slice_events( &ui_info->graphics_window.event_viewports, 2);

            update_position_counters( ui_info, MERGED_VOLUME_INDEX );
        }
        else
            remove_slice_events( &ui_info->graphics_window.event_viewports, 2 );
    }
}

public  BOOLEAN   get_voxel_under_mouse(
    UI_struct        *ui_info,
    Viewport_types   event_viewport_index,
    int              *volume,
    Real             voxel_position[N_DIMENSIONS] )
{
    int        view, x_mouse, y_mouse;

    get_viewport_mouse_position( &ui_info->graphics_window,
                                 event_viewport_index,
                                 &x_mouse, &y_mouse );

    ui_get_volume_view_index( event_viewport_index, volume, &view);

    return( IF_convert_pixel_to_voxel( *volume, view, x_mouse, y_mouse,
                                       voxel_position ) );
}

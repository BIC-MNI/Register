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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/UI_calls_F/UI_calls_F.c,v 1.18 1996-04-11 19:01:38 david Exp $";
#endif

#include  <UI_calls_F.h>

public  void  IF_initialize_register(
    Gwindow  window,
    STRING   executable_name )
{
    (void) initialize_register( window, executable_name );
}

public  Status   UI_set_global_variable(
    STRING  variable_name,
    STRING  value_to_set )
{
    set_functional_global_variable( variable_name, value_to_set );
}

public  void  IF_terminate_register()
{
    terminate_register();
}

public  Status  IF_start_loading_volume(
    int     volume_index,
    STRING  filename )
{
    return( start_loading_volume( get_main_struct(), volume_index, filename ) );
}

public  BOOLEAN  IF_load_more_of_volume(
    int     volume_index,
    Real    max_time,
    Real    *fraction_done )
{
    return( load_more_of_volume( get_main_struct(), volume_index, max_time,
                                 fraction_done ) );
}

public  void  IF_cancel_loading_volume(
    int     volume_index )
{
    cancel_loading_volume( get_main_struct(), volume_index );
}

public  BOOLEAN  IF_volume_is_loaded(
    int            volume_index )
{
    return( is_volume_active( get_main_struct(), volume_index ) );
}

public  BOOLEAN  IF_is_an_rgb_volume(
    int            volume_index )
{
    return( is_volume_rgb( get_main_struct(), volume_index ) );
}

public  BOOLEAN  IF_is_resampled_volume_loaded()
{
    return( is_resampled_volume_loaded( get_main_struct() ) );
}

public  Status  IF_do_resampling(
    STRING   resampled_filename )
{
    return( resample_the_volume( get_main_struct(), resampled_filename ) );
}

public  STRING  IF_get_volume_filename(
    int  volume_index )
{
    return( get_volume_filename( get_main_struct(), volume_index ) );
}

public  void  IF_save_image(
    int   volume,
    int   view )
{
    save_image( get_main_struct(), volume, view );
}

public  BOOLEAN  IF_get_merged_slice_visibility()
{
    return( get_merged_volume_activity(get_main_struct()) );
}

public  void  IF_set_merged_slice_visibility(
    BOOLEAN  visible )
{
    set_merged_volume_activity( get_main_struct(), visible );
}

public  void  IF_set_volume(
    int     volume_index,
    STRING  filename )
{
    set_register_volume( get_main_struct(), volume_index, filename );
}

public  void  IF_set_resampled_volume(
    int                    volume_index,
    STRING                 filename,
    STRING                 original_filename,
    General_transform      *resampling_transform )
{
    set_register_resampled_volume( get_main_struct(), volume_index, filename,
                                   original_filename, resampling_transform );
}

public  Real  IF_get_voxel_value(
    int     volume_index,
    Real    x_voxel,
    Real    y_voxel,
    Real    z_voxel )
{
    return( get_voxel_value( get_main_struct(), volume_index,
            x_voxel, y_voxel, z_voxel ) );
}

public  void  IF_get_volume_value_range(
    int     volume_index,
    Real    *min_value,
    Real    *max_value )
{
    get_volume_value_range( get_main_struct(), volume_index,
                            min_value, max_value );
}

public  void  IF_delete_volume(
    int            volume_index )
{
    delete_register_volume( get_main_struct(), volume_index );
}

public  BOOLEAN  IF_get_interpolation_flag()
{
    return( get_interpolation_mode( get_main_struct() ) );
}

public  void  IF_set_interpolation_flag( BOOLEAN  flag )
{
    set_interpolation_mode( get_main_struct(), flag );
}

public  void  IF_set_recreate_slice_flag(
    int    volume,
    int    view )
{
    set_recreate_slice_flag( get_main_struct(), volume, view );
}

public  void  IF_reset_slice_view(
    int             volume,
    int             view )
{
    initialize_slice_view( get_main_struct(), volume, view );
}

public  void  IF_set_update_slice_viewport_flag(
    int             volume,
    int             view,
    Bitplane_types  bitplane )
{
    set_update_slice_viewport_flag( get_main_struct(), volume, view,
                                    bitplane );
}

public  BOOLEAN  IF_slices_to_be_updated(
    int   current_buffer )
{
    return( slices_to_be_updated( get_main_struct(), current_buffer ) );
}

public  BOOLEAN  IF_redraw_slices(
    int             current_buffer )
{
    return( update_slice_display( get_main_struct(), current_buffer ) );
}

public  void  IF_set_volume_voxel_position(
    int       volume,
    Real      position[] )
{
    set_volume_voxel_position( get_main_struct(), volume, position );
}

public  void  IF_get_volume_voxel_position(
    int       volume,
    Real      position[] )
{
    get_volume_voxel_position( get_main_struct(), volume, position );
}

public  void  IF_set_volume_world_position(
    int       volume,
    Real      position[] )
{
    set_volume_world_position( get_main_struct(), volume, position );
}

public  void  IF_get_volume_world_position(
    int       volume,
    Real      position[] )
{
    get_volume_world_position( get_main_struct(), volume, position );
}

public  void  IF_set_volume_original_world_position(
    int       volume,
    Real      position[] )
{
    set_volume_original_world_position( get_main_struct(), volume, position );
}

public  void  IF_get_volume_original_world_position(
    int       volume,
    Real      position[] )
{
    get_volume_original_world_position( get_main_struct(), volume, position );
}

public  int   IF_get_slice_axis(
    int   view )
{
    return( get_slice_axis( view ) );
}

public  void  IF_translate_slice(
    int       volume,
    int       view,
    int       x_translation,
    int       y_translation )
{
    translate_slice( get_main_struct(), volume, view,
                     x_translation, y_translation );
}

public  void  IF_scale_slice(
    int       volume,
    int       view,
    Real      scale_factor )
{
    scale_slice( get_main_struct(), volume, view, scale_factor );
}

public  void  IF_set_slice_viewport(
    int       volume,
    int       view,
    int       x_min,
    int       x_max,
    int       y_min,
    int       y_max )
{
    set_slice_viewport( get_main_struct(), volume, view,
                        x_min, x_max, y_min, y_max );
}

public  BOOLEAN  IF_convert_pixel_to_voxel(
    int    volume,
    int    view,
    int    x_pixel,
    int    y_pixel,
    Real   voxel_position[] )
{
    return( convert_pixel_to_voxel( get_main_struct(), volume, view,
                                    x_pixel, y_pixel, voxel_position ) );
}

public  BOOLEAN  IF_can_switch_colour_modes()
{
    return( can_switch_colour_modes( get_main_struct() ) );
}

public  void  IF_colour_mode_has_toggled(
    int  start_index )
{
    colour_mode_has_toggled( get_main_struct(), start_index );
}

public  void  IF_set_under_colour(
    int        volume_index,
    Colour     colour )
{
    set_volume_under_colour( get_main_struct(), volume_index, colour );
}

public  void  IF_set_merge_method(
    Merge_methods     method )
{
    set_merged_method( get_main_struct(), method );
}

public  Merge_methods  IF_get_merge_method()
{
    return( get_merged_method( get_main_struct() ) );
}

public  void  IF_set_over_colour(
    int        volume_index,
    Colour     colour )
{
    set_volume_over_colour( get_main_struct(), volume_index, colour );
}

public  void  IF_set_volume_colour_coding_type(
    int                   volume_index,
    Colour_coding_types   type )
{
    set_volume_colour_coding_type( get_main_struct(), volume_index,
                                   type );
}

public  Colour_coding_types  IF_get_colour_coding_type(
    int    volume_index )
{
    return( get_volume_colour_coding_type( get_main_struct(), volume_index ) );
}

public  void  IF_set_colour_coding_limits(
    int    volume_index,
    Real   min_value,
    Real   max_value )
{
    set_volume_colour_coding_limits( get_main_struct(), volume_index,
                                     min_value, max_value );
}

public  void  IF_get_colour_coding_limits(
    int    volume_index,
    Real   *min_value,
    Real   *max_value )
{
    get_volume_colour_coding_limits( get_main_struct(), volume_index,
                                     min_value, max_value );
}

public  void  IF_set_merged_volume_opacity(
    int    which_volume,
    Real   opacity )
{
    set_merged_volume_opacity( get_main_struct(), which_volume, opacity );
}

public  Real  IF_get_merged_volume_opacity(
    int    which_volume )
{
    return( get_merged_volume_opacity( get_main_struct(), which_volume ) );
}

public  void  IF_set_slice_filter_type(
    int           volume_index,
    int           view_index,
    Filter_types  filter_type )
{
    set_slice_filter_type( get_main_struct(), volume_index, view_index,
                           filter_type );
}

public  Filter_types  IF_get_slice_filter_type(
    int           volume_index,
    int           view_index )
{
    return( get_slice_filter_type(get_main_struct(), volume_index, view_index));
}

public  void  IF_set_slice_filter_width(
    int      volume_index,
    int      view_index,
    Real     filter_width )
{
    set_slice_filter_width( get_main_struct(), volume_index, view_index,
                            filter_width );
}

public  Real  IF_get_slice_filter_width(
    int           volume_index,
    int           view_index )
{
    return(get_slice_filter_width(get_main_struct(), volume_index, view_index));
}

/* ----------------------- tag points -------------------------------- */

public  BOOLEAN  IF_get_tags_visibility( void )
{
    return( get_tags_visibility(get_main_struct()) );
}

public  void  IF_set_tags_visibility( BOOLEAN  visibility )
{
    set_tags_visibility( get_main_struct(), visibility );
}

public  void  IF_create_new_tag_point( void )
{
    create_new_tag_point( get_main_struct() );
}

public  void  IF_delete_tag_point(
    int      ind )
{
    delete_tag_point( get_main_struct(), ind );
}

public  int  IF_get_n_tag_points()
{
    return( get_n_tag_points( get_main_struct() ) );
}

public  BOOLEAN  IF_get_tag_point_position(
    int              ind,
    int              volume_index,
    Real             position[] )
{
    return( get_tag_point_position( get_main_struct(),
                                    ind, volume_index, position ) );
}

public  void  IF_set_tag_point_position(
    int              ind,
    int              volume_index,
    Real             position[] )
{
    set_tag_point_position( get_main_struct(),
                            ind, volume_index, position );
}

public  STRING  IF_get_tag_point_name(
    int              ind )
{
    return( get_tag_point_name( get_main_struct(), ind ) );
}

public  void  IF_set_tag_point_name(
    int              ind,
    STRING           name )
{
    set_tag_point_name( get_main_struct(), ind, name );
}

public  BOOLEAN  IF_get_tag_point_rms_error(
    int              ind,
    Real             *rms_error )
{
    return( get_tag_point_rms_error( get_main_struct(), ind, rms_error ) );
}

public  BOOLEAN  IF_get_tag_point_avg_rms_error(
    Real             *avg_rms_error )
{
    return( get_tag_point_avg_rms_error( get_main_struct(), avg_rms_error ) );
}

public  BOOLEAN  IF_get_tag_point_activity(
    int              ind )
{
    return( get_tag_point_activity( get_main_struct(), ind ) );
}

public  void  IF_set_tag_point_activity(
    int              ind,
    BOOLEAN          activity )
{
    set_tag_point_activity( get_main_struct(), ind, activity );
}

public  void  IF_save_tags_file(
    STRING   filename )
{
    (void) save_tag_points( get_main_struct(), filename );
}

public  void  IF_load_tags_file(
    STRING   filename )
{
    (void) load_tag_points( get_main_struct(), filename );
}

public  void  IF_save_transform(
    STRING   filename )
{
    (void) save_transform( get_main_struct(), filename );
}

public  BOOLEAN  IF_does_transform_exist()
{
    return( get_tag_point_transform( get_main_struct(),
                                     (General_transform **) NULL ) );
}

public  BOOLEAN  IF_get_resampling_transform(
    General_transform  **transform )
{
    return( get_tag_point_transform( get_main_struct(), transform ) );
}

public  BOOLEAN  IF_tag_points_have_been_saved()
{
    return( get_tag_points_saved( get_main_struct() ) );
}

public  Trans_type  IF_get_transform_type()
{
    return( get_tag_transform_type( get_main_struct() ) );
}

public  void  IF_set_transform_type(
    Trans_type  type )
{
    set_tag_transform_type( get_main_struct(), type );
}

public  BOOLEAN  IF_get_cursor_visibility( void )
{
    return( get_cursor_visibility(get_main_struct()) );
}

public  void  IF_set_cursor_visibility( BOOLEAN  visibility )
{
    set_cursor_visibility( get_main_struct(), visibility );
}


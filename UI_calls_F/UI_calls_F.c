#include  <def_UI_calls_F.h>

public  void  IF_initialize_register( window_struct *window )
{
    (void) initialize_register( window );
}

public  void  IF_terminate_register()
{
    terminate_register();
}

public  Boolean  IF_volume_is_loaded(
    int            volume_index )
{
    return( is_volume_active( get_main_struct(), volume_index ) );
}

public  Boolean  IF_is_resampled_volume_loaded()
{
    return( is_resampled_volume_loaded( get_main_struct() ) );
}

public  Status  IF_do_resampling(
    char   resampled_filename[] )
{
    return( resample_the_volume( get_main_struct(), resampled_filename ) );
}

public  char  *IF_get_volume_filename(
    int  volume_index )
{
    return( get_volume_filename( get_main_struct(), volume_index ) );
}

public  Boolean  IF_get_merged_slice_visibility()
{
    return( get_merged_volume_activity(get_main_struct()) );
}

public  void  IF_set_merged_slice_visibility(
    Boolean  visible )
{
    set_merged_volume_activity( get_main_struct(), visible );
}

public  void  IF_set_volume(
    int            volume_index,
    volume_struct  *volume )
{
    set_register_volume( get_main_struct(), volume_index, volume );
}

public  void  IF_set_resampled_volume(
    volume_struct  *volume,
    char           original_filename[],
    Transform      *resampling_transform )
{
    set_register_resampled_volume( get_main_struct(), volume, original_filename,
                                   resampling_transform );
}

public  void  IF_delete_volume(
    int            volume_index )
{
    delete_register_volume( get_main_struct(), volume_index );
}

public  void  IF_set_interpolation_flag( Boolean  flag )
{
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

public  Boolean  IF_redraw_slices(
    window_struct   *window,
    int             current_buffer )
{
    return( update_slice_display( get_main_struct(), window, current_buffer ) );
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

public  Boolean  IF_convert_pixel_to_voxel(
    int    volume,
    int    view,
    int    x_pixel,
    int    y_pixel,
    Real   voxel_position[] )
{
    return( convert_pixel_to_voxel( get_main_struct(), volume, view,
                                    x_pixel, y_pixel, voxel_position ) );
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

/* ----------------------- tag points -------------------------------- */

public  Boolean  IF_get_tags_visibility( void )
{
    return( get_tags_visibility(get_main_struct()) );
}

public  void  IF_set_tags_visibility( Boolean  visibility )
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

public  Boolean  IF_get_tag_point_position(
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

public  char  *IF_get_tag_point_name(
    int              ind )
{
    return( get_tag_point_name( get_main_struct(), ind ) );
}

public  void  IF_set_tag_point_name(
    int              ind,
    char             *name )
{
    set_tag_point_name( get_main_struct(), ind, name );
}

public  Boolean  IF_get_tag_point_rms_error(
    int              ind,
    Real             *rms_error )
{
    return( get_tag_point_rms_error( get_main_struct(), ind, rms_error ) );
}

public  Boolean  IF_get_tag_point_avg_rms_error(
    Real             *avg_rms_error )
{
    return( get_tag_point_avg_rms_error( get_main_struct(), avg_rms_error ) );
}

public  Boolean  IF_get_tag_point_activity(
    int              ind )
{
    return( get_tag_point_activity( get_main_struct(), ind ) );
}

public  void  IF_set_tag_point_activity(
    int              ind,
    Boolean          activity )
{
    set_tag_point_activity( get_main_struct(), ind, activity );
}

public  void  IF_save_tags_file(
    char   filename[] )
{
    (void) save_tag_points( get_main_struct(), filename );
}

public  void  IF_load_tags_file(
    char   filename[] )
{
    (void) load_tag_points( get_main_struct(), filename );
}

public  void  IF_save_transform(
    char   filename[] )
{
    (void) save_transform( get_main_struct(), filename );
}

public  Boolean  IF_does_transform_exist()
{
    return( get_tag_point_transform( get_main_struct(), (Transform **) NULL,
                                     (Transform **) NULL ) );
}

public  Boolean  IF_get_resampling_transform(
    Transform  **transform )
{
    return( get_tag_point_transform( get_main_struct(), transform,
                                     (Transform **) NULL ) );
}

public  Boolean  IF_tag_points_have_been_saved()
{
    return( get_tag_points_saved( get_main_struct() ) );
}

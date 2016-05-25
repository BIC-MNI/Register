#ifndef  DEF_UI_calls_F_prototypes
#define  DEF_UI_calls_F_prototypes

/**
 * \defgroup UItoF Functionality API for UI
 * @{
 */
void  IF_initialize_register(
    Gwindow  window,
    VIO_STR   executable_name,
    int      n_volumes);

VIO_Status   UI_set_global_variable(
    VIO_STR  variable_name,
    VIO_STR  value_to_set );

void  IF_terminate_register( void );

VIO_Status  IF_start_loading_volume(
    int     volume_index,
    VIO_STR  filename );

VIO_BOOL  IF_load_more_of_volume(
    int     volume_index,
    VIO_Real    max_time,
    VIO_Real    *fraction_done );

void  IF_cancel_loading_volume(
    int     volume_index );

VIO_BOOL  IF_volume_is_loaded(
    int            volume_index );

VIO_BOOL  IF_is_an_rgb_volume(
    int            volume_index );

VIO_BOOL  IF_volume_has_time_axis(
    int            volume_index );

VIO_BOOL  IF_is_resampled_volume_loaded( void );

VIO_Status  IF_do_resampling(
    VIO_STR   resampled_filename );

VIO_STR  IF_get_volume_filename(
    int  volume_index );

void  IF_save_image(
    int   volume,
    int   view );

VIO_BOOL  IF_get_merged_slice_visibility( void );

void  IF_set_merged_slice_visibility(
    VIO_BOOL  visible );

void  IF_set_volume(
    int     volume_index,
    VIO_STR  filename );

void  IF_set_resampled_volume(
    int                    volume_index,
    VIO_STR                 filename,
    VIO_STR                 original_filename,
    VIO_General_transform      *resampling_transform );

VIO_Real  IF_get_voxel_value(
    int     volume_index,
    VIO_Real    x_voxel,
    VIO_Real    y_voxel,
    VIO_Real    z_voxel );

void  IF_get_volume_value_range(
    int     volume_index,
    VIO_Real    *min_value,
    VIO_Real    *max_value );

void  IF_delete_volume(
    int            volume_index );

VIO_BOOL  IF_get_interpolation_flag( void );

void  IF_set_interpolation_flag( VIO_BOOL  flag );

void  IF_set_recreate_slice_flag(
    int    volume,
    int    view );

void  IF_reset_slice_view(
    int             volume,
    int             view );

void  IF_set_update_slice_viewport_flag(
    int             volume,
    int             view,
    Bitplane_types  bitplane );

VIO_BOOL  IF_redraw_slices(
    int             current_buffer );

void  IF_set_volume_voxel_position(
    int       volume,
    VIO_Real      position[] );

void  IF_get_volume_voxel_position(
    int       volume,
    VIO_Real      position[] );

void  IF_set_volume_world_position(
    int       volume,
    VIO_Real      position[] );

void  IF_get_volume_world_position(
    int       volume,
    VIO_Real      position[] );

void  IF_set_volume_original_world_position(
    int       volume,
    VIO_Real      position[] );

void  IF_get_volume_original_world_position(
    int       volume,
    VIO_Real      position[] );

int   IF_get_slice_axis(
    int   view );

void  IF_translate_slice(
    int       volume,
    int       view,
    int       x_translation,
    int       y_translation );

void  IF_scale_slice(
    int       volume,
    int       view,
    VIO_Real      scale_factor );

void  IF_set_slice_viewport(
    int       volume,
    int       view,
    int       x_min,
    int       x_max,
    int       y_min,
    int       y_max );

VIO_BOOL  IF_convert_pixel_to_voxel(
    int    volume,
    int    view,
    int    x_pixel,
    int    y_pixel,
    VIO_Real   voxel_position[] );

void  IF_set_under_colour(
    int        volume_index,
    VIO_Colour     colour );

void  IF_set_over_colour(
    int        volume_index,
    VIO_Colour     colour );

void  IF_set_volume_colour_coding_type(
    int                   volume_index,
    Colour_coding_types   type );

Colour_coding_types  IF_get_colour_coding_type(
    int    volume_index );

void  IF_set_colour_coding_limits(
    int    volume_index,
    VIO_Real   min_value,
    VIO_Real   max_value );

void  IF_get_colour_coding_limits(
    int    volume_index,
    VIO_Real   *min_value,
    VIO_Real   *max_value );

void  IF_set_merged_volume_opacity(
    int    which_volume,
    VIO_Real   opacity );

VIO_Real  IF_get_merged_volume_opacity(
    int    which_volume );

void IF_set_volume_merge_method( int which_volume, Merge_methods method );

Merge_methods IF_get_volume_merge_method( int which_volume );

void  IF_set_slice_filter_type(
    int           volume_index,
    int           view_index,
    VIO_Filter_types  filter_type );

VIO_Filter_types  IF_get_slice_filter_type(
    int           volume_index,
    int           view_index );

void  IF_set_slice_filter_width(
    int      volume_index,
    int      view_index,
    VIO_Real     filter_width );

VIO_Real  IF_get_slice_filter_width(
    int           volume_index,
    int           view_index );

VIO_BOOL  IF_get_tags_visibility( void );

void  IF_set_tags_visibility( VIO_BOOL  visibility );

void  IF_create_new_tag_point( void );

void  IF_delete_tag_point(
    int      ind );

int  IF_get_n_tag_points( void );

VIO_BOOL  IF_get_tag_point_position(
    int              ind,
    int              volume_index,
    VIO_Real             position[] );

void  IF_set_tag_point_position(
    int              ind,
    int              volume_index,
    VIO_Real             position[] );

VIO_STR  IF_get_tag_point_name(
    int              ind );

void  IF_set_tag_point_name(
    int              ind,
    VIO_STR           name );

VIO_BOOL  IF_get_tag_point_rms_error(
    int              ind,
    VIO_Real             *rms_error );

VIO_BOOL  IF_get_tag_point_avg_rms_error(
    VIO_Real             *avg_rms_error );

VIO_BOOL  IF_get_tag_point_activity(
    int              ind );

void  IF_set_tag_point_activity(
    int              ind,
    VIO_BOOL          activity );

void  IF_save_tags_file(
    VIO_STR   filename );

void  IF_load_tags_file(
    VIO_STR   filename );

void  IF_save_transform(
    VIO_STR   filename );

VIO_BOOL  IF_does_transform_exist( void );

VIO_BOOL  IF_get_resampling_transform(
    VIO_General_transform  **transform );

VIO_BOOL  IF_tag_points_have_been_saved( void );

Trans_type  IF_get_transform_type( void );

void  IF_set_transform_type(
    Trans_type  type );

VIO_BOOL  IF_get_cursor_visibility( void );

void  IF_set_cursor_visibility( VIO_BOOL  visibility );

void IF_get_volume_time_position(int volume_index, VIO_Real *tpos_ptr);

void IF_set_volume_time_position(int volume_index, VIO_Real tpos);

void IF_load_volume_colour_coding( int volume_index, VIO_STR filename );
/** @} */
#endif

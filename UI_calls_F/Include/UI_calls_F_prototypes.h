#ifndef  DEF_UI_calls_F_prototypes
#define  DEF_UI_calls_F_prototypes

public  void  IF_initialize_register(
    Gwindow  window,
    STRING   executable_name );

public  Status   UI_set_global_variable(
    STRING  variable_name,
    STRING  value_to_set );

public  void  IF_terminate_register( void );

public  Status  IF_start_loading_volume(
    int     volume_index,
    STRING  filename );

public  BOOLEAN  IF_load_more_of_volume(
    int     volume_index,
    Real    max_time,
    Real    *fraction_done );

public  void  IF_cancel_loading_volume(
    int     volume_index );

public  BOOLEAN  IF_volume_is_loaded(
    int            volume_index );

public  BOOLEAN  IF_is_an_rgb_volume(
    int            volume_index );

public  BOOLEAN  IF_is_resampled_volume_loaded( void );

public  Status  IF_do_resampling(
    STRING   resampled_filename );

public  STRING  IF_get_volume_filename(
    int  volume_index );

public  void  IF_save_image(
    int   volume,
    int   view );

public  BOOLEAN  IF_get_merged_slice_visibility( void );

public  void  IF_set_merged_slice_visibility(
    BOOLEAN  visible );

public  void  IF_set_volume(
    int     volume_index,
    STRING  filename );

public  void  IF_set_resampled_volume(
    int                    volume_index,
    STRING                 filename,
    STRING                 original_filename,
    General_transform      *resampling_transform );

public  Real  IF_get_voxel_value(
    int     volume_index,
    Real    x_voxel,
    Real    y_voxel,
    Real    z_voxel );

public  void  IF_get_volume_value_range(
    int     volume_index,
    Real    *min_value,
    Real    *max_value );

public  void  IF_delete_volume(
    int            volume_index );

public  BOOLEAN  IF_get_interpolation_flag( void );

public  void  IF_set_interpolation_flag( BOOLEAN  flag );

public  void  IF_set_recreate_slice_flag(
    int    volume,
    int    view );

public  void  IF_reset_slice_view(
    int             volume,
    int             view );

public  void  IF_set_update_slice_viewport_flag(
    int             volume,
    int             view,
    Bitplane_types  bitplane );

public  BOOLEAN  IF_slices_to_be_updated(
    int   current_buffer );

public  BOOLEAN  IF_redraw_slices(
    int             current_buffer );

public  void  IF_set_volume_voxel_position(
    int       volume,
    Real      position[] );

public  void  IF_get_volume_voxel_position(
    int       volume,
    Real      position[] );

public  void  IF_set_volume_world_position(
    int       volume,
    Real      position[] );

public  void  IF_get_volume_world_position(
    int       volume,
    Real      position[] );

public  void  IF_set_volume_original_world_position(
    int       volume,
    Real      position[] );

public  void  IF_get_volume_original_world_position(
    int       volume,
    Real      position[] );

public  int   IF_get_slice_axis(
    int   view );

public  void  IF_translate_slice(
    int       volume,
    int       view,
    int       x_translation,
    int       y_translation );

public  void  IF_scale_slice(
    int       volume,
    int       view,
    Real      scale_factor );

public  void  IF_set_slice_viewport(
    int       volume,
    int       view,
    int       x_min,
    int       x_max,
    int       y_min,
    int       y_max );

public  BOOLEAN  IF_convert_pixel_to_voxel(
    int    volume,
    int    view,
    int    x_pixel,
    int    y_pixel,
    Real   voxel_position[] );

public  BOOLEAN  IF_can_switch_colour_modes( void );

public  void  IF_colour_mode_has_toggled(
    int  start_index );

public  void  IF_set_under_colour(
    int        volume_index,
    Colour     colour );

public  void  IF_set_merge_method(
    Merge_methods     method );

public  Merge_methods  IF_get_merge_method( void );

public  void  IF_set_over_colour(
    int        volume_index,
    Colour     colour );

public  void  IF_set_volume_colour_coding_type(
    int                   volume_index,
    Colour_coding_types   type );

public  Colour_coding_types  IF_get_colour_coding_type(
    int    volume_index );

public  void  IF_set_colour_coding_limits(
    int    volume_index,
    Real   min_value,
    Real   max_value );

public  void  IF_get_colour_coding_limits(
    int    volume_index,
    Real   *min_value,
    Real   *max_value );

public  void  IF_set_merged_volume_opacity(
    int    which_volume,
    Real   opacity );

public  Real  IF_get_merged_volume_opacity(
    int    which_volume );

public  void  IF_set_slice_filter_type(
    int           volume_index,
    int           view_index,
    Filter_types  filter_type );

public  Filter_types  IF_get_slice_filter_type(
    int           volume_index,
    int           view_index );

public  void  IF_set_slice_filter_width(
    int      volume_index,
    int      view_index,
    Real     filter_width );

public  Real  IF_get_slice_filter_width(
    int           volume_index,
    int           view_index );

public  BOOLEAN  IF_get_tags_visibility( void );

public  void  IF_set_tags_visibility( BOOLEAN  visibility );

public  void  IF_create_new_tag_point( void );

public  void  IF_delete_tag_point(
    int      ind );

public  int  IF_get_n_tag_points( void );

public  BOOLEAN  IF_get_tag_point_position(
    int              ind,
    int              volume_index,
    Real             position[] );

public  void  IF_set_tag_point_position(
    int              ind,
    int              volume_index,
    Real             position[] );

public  STRING  IF_get_tag_point_name(
    int              ind );

public  void  IF_set_tag_point_name(
    int              ind,
    STRING           name );

public  BOOLEAN  IF_get_tag_point_rms_error(
    int              ind,
    Real             *rms_error );

public  BOOLEAN  IF_get_tag_point_avg_rms_error(
    Real             *avg_rms_error );

public  BOOLEAN  IF_get_tag_point_activity(
    int              ind );

public  void  IF_set_tag_point_activity(
    int              ind,
    BOOLEAN          activity );

public  void  IF_save_tags_file(
    STRING   filename );

public  void  IF_load_tags_file(
    STRING   filename );

public  void  IF_save_transform(
    STRING   filename );

public  BOOLEAN  IF_does_transform_exist( void );

public  BOOLEAN  IF_get_resampling_transform(
    General_transform  **transform );

public  BOOLEAN  IF_tag_points_have_been_saved( void );

public  Trans_type  IF_get_transform_type( void );

public  void  IF_set_transform_type(
    Trans_type  type );

public  BOOLEAN  IF_get_cursor_visibility( void );

public  void  IF_set_cursor_visibility( BOOLEAN  visibility );

public void IF_get_volume_time_position(int volume_index, Real *tpos_ptr);

public void IF_set_volume_time_position(int volume_index, Real tpos);

#endif

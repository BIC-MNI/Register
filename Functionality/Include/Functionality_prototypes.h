#ifndef  DEF_Functionality_prototypes
#define  DEF_Functionality_prototypes

public  main_struct  *get_main_struct( void );

public  Status   initialize_register(
    Gwindow   window,
    STRING    executable_name );

public  void   terminate_register( void );

public  Status  set_functional_global_variable(
    STRING  variable_name,
    STRING  value_to_set );

public  void   update_colour_maps(
    main_struct  *main,
    int          volume );

public  void  repartition_colour_maps(
    main_struct  *main );

public  void  colour_mode_has_toggled(
    main_struct  *main,
    int          start_index );

public  void   set_volume_colour_coding_type( 
    main_struct          *main,
    int                  volume_index,
    Colour_coding_types  type );

public  Colour_coding_types   get_volume_colour_coding_type( 
    main_struct          *main,
    int                  volume_index );

public  void  set_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    Real                 min_value,
    Real                 max_value );

public  void  get_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    Real                 *min_value,
    Real                 *max_value );

public  void  set_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume,
    Real                 opacity );

public  Real  get_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume );

public  void  set_merged_method(
    main_struct       *main,
    Merge_methods     method );

public  Merge_methods  get_merged_method(
    main_struct       *main );

public  void   set_volume_under_colour( 
    main_struct          *main,
    int                  volume_index,
    Colour               colour );

public  void   set_volume_over_colour( 
    main_struct          *main,
    int                  volume_index,
    Colour               colour );

public  void  composite_merged_pixels(
    main_struct          *main,
    pixels_struct        *pixels1,
    pixels_struct        *pixels2,
    pixels_struct        *result );

public  BOOLEAN  can_switch_colour_modes(
    main_struct          *main );

public  object_struct  *create_cursor(
    main_struct  *main,
    int          volume_index,
    int          view_index );

public  void  position_cursor(
    lines_struct   *lines,
    int            x,
    int            y,
    int            offset,
    int            length );

public  void  update_cursor_colours(
    main_struct    *main,
    int            volume,
    int            view );

public  void  update_volume_cursor(
    main_struct   *main,
    int           volume_index,
    int           view_index );

public  void  update_volume_cursors(
    main_struct   *main,
    int           volume_index );

public  BOOLEAN  get_cursor_visibility(
    main_struct  *main );

public  void  set_cursor_visibility(
    main_struct  *main,
    BOOLEAN      state );

public  void  create_slice_pixels(
    main_struct   *main,
    int           volume_index,
    int           view );

public  void  create_merged_pixels(
    main_struct   *main,
    int           view );

public  BOOLEAN   convert_pixel_to_voxel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    int             x_pixel,
    int             y_pixel,
    Real            voxel_position[N_DIMENSIONS] );

public  void   convert_voxel_to_pixel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    Real            voxel_position[N_DIMENSIONS],
    Real            *x_pixel,
    Real            *y_pixel );

public  void  translate_slice(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_translation_offset,
    int          y_translation_offset );

public  void  scale_slice(
    main_struct  *main,
    int          volume,
    int          view,
    Real         scale_factor );

public  void  resize_slice(
    main_struct  *main,
    int          volume_index,
    int          view );

public  void  initialize_slice_view(
    main_struct  *main,
    int          volume_index,
    int          view );

public  void  set_slice_interpolation(
    main_struct  *main,
    int          degrees_continuity );

public  void  initialize_slices( main_struct  *main );

public  void  terminate_slices( main_struct  *main );

public  Status  resample_the_volume(
    main_struct  *main,
    STRING       resampled_filename );

public  void  save_image(
    main_struct   *main_info,
    int           volume_index,
    int           view_index );

public  BOOLEAN  is_volume_active(
    main_struct    *main,
    int            volume_index );

public  BOOLEAN  is_resampled_volume_loaded(
    main_struct    *main );

public  STRING  get_volume_filename(
    main_struct    *main,
    int            volume_index );

public  void   set_register_volume(
    main_struct    *main,
    int            volume_index,
    STRING         filename );

public  void   set_register_resampled_volume(
    main_struct            *main,
    int                    volume_index,
    STRING                 filename,
    STRING                 original_filename,
    General_transform      *resampling_transform );

public  void  delete_register_volume(
    main_struct    *main,
    int            volume_index );

public  void  set_merged_volume_activity(
    main_struct    *main,
    BOOLEAN        activity );

public  BOOLEAN  get_merged_volume_activity(
    main_struct    *main );

public  Status  start_loading_volume(
    main_struct    *main,
    int            volume_index,
    STRING         filename );

public  BOOLEAN  load_more_of_volume(
    main_struct    *main,
    int            volume_index,
    Real           max_time,
    Real           *fraction_done );

public  void  cancel_loading_volume(
    main_struct    *main,
    int            volume_index );

public  BOOLEAN  is_volume_rgb(
    main_struct    *main,
    int            volume_index );

public  void  get_slice_axes(
    int   view_index,
    int   *x_axis_index,
    int   *y_axis_index );

public  void  get_slice_axes_flip(
    int       view_index,
    BOOLEAN   *x_axis_flip,
    BOOLEAN   *y_axis_flip );

public  int  get_slice_axis(
    int   view_index );

public  void  get_slice_plane(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          origin[],
    Real          x_axis[],
    Real          y_axis[] );

public  int  get_slice_viewport_index( int volume, int view );

public  void  get_volume_view_index(
    int   viewport_index,
    int   *volume,
    int   *view );

public  void  get_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_min,
    int          *x_max,
    int          *y_min,
    int          *y_max );

public  void  get_slice_viewport_size(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_size,
    int          *y_size );

public  void  set_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_min,
    int          x_max,
    int          y_min,
    int          y_max );

public  void  set_volume_voxel_position(
    main_struct    *main,
    int            volume_index,
    Real           position[N_DIMENSIONS] );

public  void  get_volume_voxel_position(
    main_struct   *main,
    int           volume,
    Real          position[N_DIMENSIONS] );

public  void  convert_original_world_to_world(
    main_struct    *main,
    int            volume_index,
    Real           x_original,
    Real           y_original,
    Real           z_original,
    Real           *x_world,
    Real           *y_world,
    Real           *z_world );

public  void  convert_world_to_original_world(
    main_struct    *main,
    int            volume_index,
    Real           x_world,
    Real           y_world,
    Real           z_world,
    Real           *x_original,
    Real           *y_original,
    Real           *z_original );

public  void  set_volume_world_position(
    main_struct    *main,
    int            volume_index,
    Real           world_position[N_DIMENSIONS] );

public  void  get_volume_world_position(
    main_struct   *main,
    int           volume_index,
    Real          world_position[N_DIMENSIONS] );

public  void  convert_original_world_to_voxel(
    main_struct    *main,
    int            volume_index,
    Real           x_original,
    Real           y_original,
    Real           z_original,
    Real           *x_voxel,
    Real           *y_voxel,
    Real           *z_voxel );

public  void  convert_voxel_to_original_world(
    main_struct    *main,
    int            volume_index,
    Real           x_voxel,
    Real           y_voxel,
    Real           z_voxel,
    Real           *x_original,
    Real           *y_original,
    Real           *z_original );

public  void  set_volume_original_world_position(
    main_struct    *main,
    int            volume_index,
    Real           original_world_position[N_DIMENSIONS] );

public  void  get_volume_original_world_position(
    main_struct   *main,
    int           volume_index,
    Real          original_world_position[N_DIMENSIONS] );

public  Volume  get_slice_volume(
    main_struct   *main,
    int           volume_index );

public  void  get_volume_range_of_voxels(
    main_struct   *main,
    int           volume_index,
    int           *min_value,
    int           *max_value );

public  void  get_volume_value_range(
    main_struct   *main,
    int           volume_index,
    Real          *min_value,
    Real          *max_value );

public  Real  *get_volume_cursor(
    main_struct   *main,
    int           volume_index );

public  slice_struct  *get_slice_struct(
    main_struct   *main,
    int           volume_index,
    int           view );

public  void  get_slice_transform(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          *x_trans,
    Real          *y_trans,
    Real          *x_scale,
    Real          *y_scale );

public  void  set_slice_translation(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          x_trans,
    Real          y_trans );

public  void  set_slice_scale(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          x_scale,
    Real          y_scale );

public  Real  get_voxel_value(
    main_struct   *main,
    int           volume_index,
    Real          x_voxel,
    Real          y_voxel,
    Real          z_voxel );

public  void  set_slice_filter_type(
    main_struct   *main,
    int           volume_index,
    int           view,
    Filter_types  filter_type );

public  Filter_types  get_slice_filter_type(
    main_struct   *main,
    int           volume_index,
    int           view );

public  void  set_slice_filter_width(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          filter_width );

public  Real  get_slice_filter_width(
    main_struct   *main,
    int           volume_index,
    int           view );

public  void  set_interpolation_mode(
    main_struct   *main,
    BOOLEAN       smooth_flag );

public  BOOLEAN  get_interpolation_mode(
    main_struct   *main );

public  void  set_recreate_3_slices_flags(
    main_struct  *main,
    int          volume_index );

public  void  set_recreate_slice_flag(
    main_struct  *main,
    int          volume,
    int          view );

public  void  set_update_slice_viewport_flag(
    main_struct     *main,
    int             volume,
    int             view,
    Bitplane_types  bitplane );

public  void  create_tag_objects(
    main_struct        *main,
    tag_point_struct   *tag );

public  void  update_slice_tag_colours(
    main_struct        *main,
    int                volume,
    int                view,
    tag_point_struct   *tag );

public   void  update_tag_colours(
    main_struct        *main,
    tag_point_struct   *tag );

public   void  update_all_tag_colours(
    main_struct        *main );

public  void  update_tag_object(
    main_struct       *main,
    int               volume,
    int               view,
    tag_point_struct  *tag );

public  void  update_tag_objects(
    main_struct       *main,
    tag_point_struct  *tag );

public  void  update_slice_tag_objects(
    main_struct       *main,
    int               volume,
    int               view );

public   void  update_volume_tag_objects(
    main_struct        *main,
    int                volume );

public   void  update_all_tag_objects(
    main_struct        *main );

public  void  delete_tag_objects(
    main_struct        *main,
    tag_point_struct   *tag );

public  Status   save_tag_points(
    main_struct   *main,
    STRING        filename );

public  Status   load_tag_points(
    main_struct   *main,
    STRING        filename );

public  Status   save_transform(
    main_struct   *main,
    STRING        filename );

public  void  initialize_tag_points(
    main_struct      *main );

public  BOOLEAN  get_tags_visibility(
    main_struct      *main );

public  void  set_tags_visibility(
    main_struct      *main,
    BOOLEAN          visibility );

public  void  delete_tag_points(
    main_struct      *main );

public  void  create_new_tag_point(
    main_struct      *main );

public  void  delete_tag_point(
    main_struct      *main,
    int              ind );

public  int  get_n_tag_points(
    main_struct      *main );

public  BOOLEAN  get_tag_point_position(
    main_struct      *main,
    int              ind,
    int              volume_index,
    Real             position[] );

public  void  set_tag_point_position(
    main_struct      *main,
    int              ind,
    int              volume_index,
    Real             position[] );

public  STRING  get_tag_point_name(
    main_struct      *main,
    int              ind );

public  void  set_tag_point_name(
    main_struct      *main,
    int              ind,
    STRING           name );

public  BOOLEAN  get_tag_point_avg_rms_error(
    main_struct      *main,
    Real             *error );

public  BOOLEAN  get_tag_point_rms_error(
    main_struct      *main,
    int              ind,
    Real             *error );

public  void  set_tag_point_rms_error(
    main_struct      *main,
    int              ind,
    Real             rms_error );

public  void  set_tag_point_activity(
    main_struct      *main,
    int              ind,
    BOOLEAN          activity );

public  BOOLEAN  get_tag_point_activity(
    main_struct      *main,
    int              ind );

public  BOOLEAN  get_tag_point_transform(
    main_struct           *main,
    General_transform     **transform );

public  BOOLEAN  get_tag_points_saved(
    main_struct   *main );

public  Trans_type  get_tag_transform_type(
    main_struct  *main );

public  void  set_tag_transform_type(
    main_struct  *main,
    Trans_type   type );

public  void  recompute_tag_transform(
    tag_list_struct   *tags );

public  BOOLEAN  update_slice_display(
    main_struct     *main,
    int             current_buffer );

public  BOOLEAN  slices_to_be_updated(
    main_struct     *main,
    int             current_buffer );

public void get_volume_time_position(main_struct *main,
                                     int volume_index,
                                     Real *tpos_ptr);

public void set_volume_time_position(main_struct *main,
                                     int volume_index,
                                     Real tpos);
#endif

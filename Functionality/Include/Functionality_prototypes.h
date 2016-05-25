#ifndef  DEF_Functionality_prototypes
#define  DEF_Functionality_prototypes

  main_struct  *get_main_struct( void );

  VIO_Status   initialize_register(
    Gwindow   window,
    VIO_STR    executable_name,
    int        n_volumes);

  void   terminate_register( void );

  VIO_Status  set_functional_global_variable(
    VIO_STR  variable_name,
    VIO_STR  value_to_set );

  void   update_colour_maps(
    main_struct  *main,
    int          volume );

  void   set_volume_colour_coding_type( 
    main_struct          *main,
    int                  volume_index,
    Colour_coding_types  type );

  Colour_coding_types   get_volume_colour_coding_type( 
    main_struct          *main,
    int                  volume_index );

  void  set_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    VIO_Real                 min_value,
    VIO_Real                 max_value );

  void  get_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    VIO_Real                 *min_value,
    VIO_Real                 *max_value );

void load_volume_colour_coding( main_struct *main, int volume_index,
                                VIO_STR filename );

  void  set_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume,
    VIO_Real                 opacity );

  VIO_Real  get_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume );

  void set_volume_merge_activity( 
    main_struct          *main,
    int                  volume_index,
    VIO_BOOL             active );

  VIO_BOOL get_volume_merge_activity(
    main_struct          *main,
    int                  volume_index );

void  set_volume_merge_method( main_struct *main, int volume_index,
                               Merge_methods     method );

Merge_methods get_volume_merge_method( main_struct *main, int volume_index );

  void   set_volume_under_colour( 
    main_struct          *main,
    int                  volume_index,
    VIO_Colour               colour );

  void   set_volume_over_colour( 
    main_struct          *main,
    int                  volume_index,
    VIO_Colour               colour );

  void  composite_merged_pixels(
    main_struct          *main,
    pixels_struct        pixels[],
    pixels_struct        *result );

  VIO_BOOL  can_switch_colour_modes(
    main_struct          *main );

  object_struct  *create_cursor(
    main_struct  *main,
    int          volume_index,
    int          view_index );

  void  position_cursor(
    lines_struct   *lines,
    int            x,
    int            y,
    int            offset,
    int            length );

  void  update_cursor_colours(
    main_struct    *main,
    int            volume,
    int            view );

  void  update_volume_cursor(
    main_struct   *main,
    int           volume_index,
    int           view_index );

  void  update_volume_cursors(
    main_struct   *main,
    int           volume_index );

  VIO_BOOL  get_cursor_visibility(
    main_struct  *main );

  void  set_cursor_visibility(
    main_struct  *main,
    VIO_BOOL      state );

  void  create_slice_pixels(
    main_struct   *main,
    int           volume_index,
    int           view );

  void  create_merged_pixels(
    main_struct   *main,
    int           view );

  VIO_BOOL   convert_pixel_to_voxel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    int             x_pixel,
    int             y_pixel,
    VIO_Real            voxel_position[VIO_N_DIMENSIONS] );

  void   convert_voxel_to_pixel(
    main_struct     *main,
    int             volume_index,
    int             view_index,
    VIO_Real            voxel_position[VIO_N_DIMENSIONS],
    VIO_Real            *x_pixel,
    VIO_Real            *y_pixel );

  void  translate_slice(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_translation_offset,
    int          y_translation_offset );

  void  scale_slice(
    main_struct  *main,
    int          volume,
    int          view,
    VIO_Real         scale_factor );

  void  resize_slice(
    main_struct  *main,
    int          volume_index,
    int          view );

  void  initialize_slice_view(
    main_struct  *main,
    int          volume_index,
    int          view );

  void  set_slice_interpolation(
    main_struct  *main,
    int          degrees_continuity );

  void  initialize_slices( main_struct  *main );

  void  terminate_slices( main_struct  *main );

  VIO_Status  resample_the_volume(
    main_struct  *main,
    VIO_STR       resampled_filename );

  void  save_image(
    main_struct   *main_info,
    int           volume_index,
    int           view_index );

  VIO_BOOL  is_volume_active(
    main_struct    *main,
    int            volume_index );

  VIO_BOOL  is_resampled_volume_loaded(
    main_struct    *main );

  VIO_STR  get_volume_filename(
    main_struct    *main,
    int            volume_index );

  void   set_register_volume(
    main_struct    *main,
    int            volume_index,
    VIO_STR         filename );

  void   set_register_resampled_volume(
    main_struct            *main,
    int                    volume_index,
    VIO_STR                 filename,
    VIO_STR                 original_filename,
    VIO_General_transform      *resampling_transform );

  void  delete_register_volume(
    main_struct    *main,
    int            volume_index );

  void  set_merged_volume_activity(
    main_struct    *main,
    VIO_BOOL        activity );

  VIO_BOOL  get_merged_volume_activity(
    main_struct    *main );

  VIO_Status  start_loading_volume(
    main_struct    *main,
    int            volume_index,
    VIO_STR         filename );

  VIO_BOOL  load_more_of_volume(
    main_struct    *main,
    int            volume_index,
    VIO_Real           max_time,
    VIO_Real           *fraction_done );

  void  cancel_loading_volume(
    main_struct    *main,
    int            volume_index );

  VIO_BOOL  is_volume_rgb(
    main_struct    *main,
    int            volume_index );

  VIO_BOOL  volume_has_time_axis(
    main_struct    *main,
    int            volume_index );

  void  get_slice_axes(
    int   view_index,
    int   *x_axis_index,
    int   *y_axis_index );

  void  get_slice_axes_flip(
    int       view_index,
    VIO_BOOL   *x_axis_flip,
    VIO_BOOL   *y_axis_flip );

  int  get_slice_axis(
    int   view_index );

  void  get_slice_plane(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          origin[],
    VIO_Real          x_axis[],
    VIO_Real          y_axis[] );

  int  get_slice_viewport_index( int volume, int view );

  void  get_volume_view_index(
    int   viewport_index,
    int   *volume,
    int   *view );

  void  get_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_min,
    int          *x_max,
    int          *y_min,
    int          *y_max );

  void  get_slice_viewport_size(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_size,
    int          *y_size );

  void  set_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_min,
    int          x_max,
    int          y_min,
    int          y_max );

  void  set_volume_voxel_position(
    main_struct    *main,
    int            volume_index,
    VIO_Real           position[VIO_N_DIMENSIONS] );

  void  get_volume_voxel_position(
    main_struct   *main,
    int           volume,
    VIO_Real          position[VIO_N_DIMENSIONS] );

  void  convert_original_world_to_world(
    main_struct    *main,
    int            volume_index,
    VIO_Real           x_original,
    VIO_Real           y_original,
    VIO_Real           z_original,
    VIO_Real           *x_world,
    VIO_Real           *y_world,
    VIO_Real           *z_world );

  void  convert_world_to_original_world(
    main_struct    *main,
    int            volume_index,
    VIO_Real           x_world,
    VIO_Real           y_world,
    VIO_Real           z_world,
    VIO_Real           *x_original,
    VIO_Real           *y_original,
    VIO_Real           *z_original );

  void  set_volume_world_position(
    main_struct    *main,
    int            volume_index,
    VIO_Real           world_position[VIO_N_DIMENSIONS] );

  void  get_volume_world_position(
    main_struct   *main,
    int           volume_index,
    VIO_Real          world_position[VIO_N_DIMENSIONS] );

  void  convert_original_world_to_voxel(
    main_struct    *main,
    int            volume_index,
    VIO_Real           x_original,
    VIO_Real           y_original,
    VIO_Real           z_original,
    VIO_Real           *x_voxel,
    VIO_Real           *y_voxel,
    VIO_Real           *z_voxel );

  void  convert_voxel_to_original_world(
    main_struct    *main,
    int            volume_index,
    VIO_Real           x_voxel,
    VIO_Real           y_voxel,
    VIO_Real           z_voxel,
    VIO_Real           *x_original,
    VIO_Real           *y_original,
    VIO_Real           *z_original );

  void  set_volume_original_world_position(
    main_struct    *main,
    int            volume_index,
    VIO_Real           original_world_position[VIO_N_DIMENSIONS] );

  void  get_volume_original_world_position(
    main_struct   *main,
    int           volume_index,
    VIO_Real          original_world_position[VIO_N_DIMENSIONS] );

  VIO_Volume  get_slice_volume(
    main_struct   *main,
    int           volume_index );

  void  get_volume_range_of_voxels(
    main_struct   *main,
    int           volume_index,
    int           *min_value,
    int           *max_value );

  void  get_volume_value_range(
    main_struct   *main,
    int           volume_index,
    VIO_Real          *min_value,
    VIO_Real          *max_value );

  VIO_Real  *get_volume_cursor(
    main_struct   *main,
    int           volume_index );

  slice_struct  *get_slice_struct(
    main_struct   *main,
    int           volume_index,
    int           view );

  void  get_slice_transform(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          *x_trans,
    VIO_Real          *y_trans,
    VIO_Real          *x_scale,
    VIO_Real          *y_scale );

  void  set_slice_translation(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          x_trans,
    VIO_Real          y_trans );

  void  set_slice_scale(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          x_scale,
    VIO_Real          y_scale );

  VIO_Real  get_voxel_value(
    main_struct   *main,
    int           volume_index,
    VIO_Real          x_voxel,
    VIO_Real          y_voxel,
    VIO_Real          z_voxel );

  void  set_slice_filter_type(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Filter_types  filter_type );

  VIO_Filter_types  get_slice_filter_type(
    main_struct   *main,
    int           volume_index,
    int           view );

  void  set_slice_filter_width(
    main_struct   *main,
    int           volume_index,
    int           view,
    VIO_Real          filter_width );

  VIO_Real  get_slice_filter_width(
    main_struct   *main,
    int           volume_index,
    int           view );

  void  set_interpolation_mode(
    main_struct   *main,
    VIO_BOOL       smooth_flag );

  VIO_BOOL  get_interpolation_mode(
    main_struct   *main );

  void  set_recreate_3_slices_flags(
    main_struct  *main,
    int          volume_index );

  void  set_recreate_slice_flag(
    main_struct  *main,
    int          volume,
    int          view );

  void  set_update_slice_viewport_flag(
    main_struct     *main,
    int             volume,
    int             view,
    Bitplane_types  bitplane );

  void  create_tag_objects(
    main_struct        *main,
    tag_point_struct   *tag );

  void  update_slice_tag_colours(
    main_struct        *main,
    int                volume,
    int                view,
    tag_point_struct   *tag );

   void  update_tag_colours(
    main_struct        *main,
    tag_point_struct   *tag );

   void  update_all_tag_colours(
    main_struct        *main );

  void  update_tag_object(
    main_struct       *main,
    int               volume,
    int               view,
    tag_point_struct  *tag );

  void  update_tag_objects(
    main_struct       *main,
    tag_point_struct  *tag );

  void  update_slice_tag_objects(
    main_struct       *main,
    int               volume,
    int               view );

   void  update_volume_tag_objects(
    main_struct        *main,
    int                volume );

   void  update_all_tag_objects(
    main_struct        *main );

  void  delete_tag_objects(
    main_struct        *main,
    tag_point_struct   *tag );

  VIO_Status   save_tag_points(
    main_struct   *main,
    VIO_STR        filename );

  VIO_Status   load_tag_points(
    main_struct   *main,
    VIO_STR        filename );

  VIO_Status   save_transform(
    main_struct   *main,
    VIO_STR        filename );

  void  initialize_tag_points(
    main_struct      *main );

  VIO_BOOL  get_tags_visibility(
    main_struct      *main );

  void  set_tags_visibility(
    main_struct      *main,
    VIO_BOOL          visibility );

  void  delete_tag_points(
    main_struct      *main );

  void  create_new_tag_point(
    main_struct      *main );

  void  delete_tag_point(
    main_struct      *main,
    int              ind );

  int  get_n_tag_points(
    main_struct      *main );

  VIO_BOOL  get_tag_point_position(
    main_struct      *main,
    int              ind,
    int              volume_index,
    VIO_Real             position[] );

  void  set_tag_point_position(
    main_struct      *main,
    int              ind,
    int              volume_index,
    VIO_Real             position[] );

  VIO_STR  get_tag_point_name(
    main_struct      *main,
    int              ind );

  void  set_tag_point_name(
    main_struct      *main,
    int              ind,
    VIO_STR           name );

  VIO_BOOL  get_tag_point_avg_rms_error(
    main_struct      *main,
    VIO_Real             *error );

  VIO_BOOL  get_tag_point_rms_error(
    main_struct      *main,
    int              ind,
    VIO_Real             *error );

  void  set_tag_point_rms_error(
    main_struct      *main,
    int              ind,
    VIO_Real             rms_error );

  void  set_tag_point_activity(
    main_struct      *main,
    int              ind,
    VIO_BOOL          activity );

  VIO_BOOL  get_tag_point_activity(
    main_struct      *main,
    int              ind );

  VIO_BOOL  get_tag_point_transform(
    main_struct           *main,
    VIO_General_transform     **transform );

  VIO_BOOL  get_tag_points_saved(
    main_struct   *main );

  Trans_type  get_tag_transform_type(
    main_struct  *main );

  void  set_tag_transform_type(
    main_struct  *main,
    Trans_type   type );

  void  recompute_tag_transform(
    tag_list_struct   *tags );

  VIO_BOOL  update_slice_display(
    main_struct     *main,
    int             current_buffer );

 void get_volume_time_position(main_struct *main,
                                     int volume_index,
                                     VIO_Real *tpos_ptr);

 void set_volume_time_position(main_struct *main,
                                     int volume_index,
                                     VIO_Real tpos);
#endif

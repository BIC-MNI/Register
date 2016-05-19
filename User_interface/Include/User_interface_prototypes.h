#ifndef  DEF_User_interface_prototypes
#define  DEF_User_interface_prototypes
/**
 * \defgroup RegisterUI User interface
 * @{
 */
  UI_struct  *get_ui_struct( void );

  VIO_Status   initialize_user_interface(
    UI_struct  *ui,
    const VIO_STR     executable_name,
    int n_volumes );

  void   terminate_user_interface(
    UI_struct  *ui );

  void  popup_colour_selection(
    UI_struct   *ui,
    int         volume,
    int         over_or_under );

  void  popup_delete_tags(
    UI_struct   *ui );

  void  save_current_viewport(
    UI_struct        *ui,
    Viewport_types   viewport );

  DEFINE_EVENT_FUNCTION( slice_key_down_callback );

  void  install_slice_events(
    event_viewports_struct   *event_table,
    int                     volume );

  void  remove_slice_events(
    event_viewports_struct   *event_table,
    int                     volume );

  void  install_tag_events(
    UI_struct                *ui );

  void  start_interaction(
    UI_struct              *ui_info,
    Viewport_types         event_viewport_index,
    Event_types            terminate_event,
    event_function_type    terminate_callback,
    event_function_type    no_event_callback );

  void  terminate_interaction(
    Event_types            terminate_event,
    event_function_type    terminate_callback,
    event_function_type    no_event_callback );

  void  restore_mouse_position(
    UI_struct   *ui );

  void  get_viewport_mouse_position(
    graphics_window_struct    *graphics_window,
    Viewport_types            event_viewport_index,
    int                       *x_mouse,
    int                       *y_mouse );

  void  install_window_events(
    UI_struct  *ui );

  void  initialize_event_table( event_callback_list_struct  table[] );

  void  delete_event_table( event_callback_list_struct  table[] );

  void  set_event_callback_enabled(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data,
    VIO_BOOL                      enabled );

  void  add_event_callback_function(
    event_callback_list_struct   *callback_list,
    int                          x_min,
    int                          x_max,
    int                          y_min,
    int                          y_max,
    event_function_type          callback_function,
    Event_modifiers              modifier,
    void                         *callback_data );

  void  set_event_callback_viewport(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data,
    int                          x_min,
    int                          x_max,
    int                          y_min,
    int                          y_max );

  void  remove_event_callback_function(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data );

  VIO_BOOL  execute_event_callback_functions(
    VIO_BOOL                      shift_state,
    event_callback_list_struct   *callback_list,
    int                          mouse_x,
    int                          mouse_y,
    Viewport_types               event_viewport_index,
    int                          key_pressed );

  void  set_quit_program_flag( void );

  void  event_loop( void );

  void   handle_event(
    Event_types       event_type,
    Gwindow           event_window,
    int               key_pressed );

  void  initialize_event_viewports(
    event_viewports_struct  *event_viewports );

  void  delete_event_viewports(
    event_viewports_struct  *event_viewports );

  void  set_event_viewport(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    int                     x_min,
    int                     x_max,
    int                     y_min,
    int                     y_max );

  void  get_event_viewport(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    int                     *x_min,
    int                     *x_max,
    int                     *y_min,
    int                     *y_max );

  void  set_event_viewport_callback_enabled(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data,
    VIO_BOOL                 enabled );

  void  set_event_viewport_callback_viewport(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data,
    int                     x_min,
    int                     x_max,
    int                     y_min,
    int                     y_max );

  void  add_event_viewport_callback(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    Event_types             event_type,
    int                     x_min,
    int                     x_max,
    int                     y_min,
    int                     y_max,
    event_function_type     callback_function,
    Event_modifiers         modifier,
    void                    *callback_data );

  void  remove_event_viewport_callback(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data );

VIO_BOOL execute_event_viewport_events(
    VIO_BOOL                 shift_state,
    event_viewports_struct  *event_viewports,
    int                     x_mouse,
    int                     y_mouse,
    Event_types             event_type,
    int                     key_pressed );

  VIO_BOOL  find_viewport_containing_mouse(
    event_viewports_struct  *event_viewports,
    int                     x_mouse,
    int                     y_mouse,
    Viewport_types          *viewport_index );

  VIO_BOOL  event_is_allowable(
    Event_types     event_type );

  void  set_interaction_in_progress(
    VIO_BOOL  state );

  VIO_BOOL  mouse_must_be_in_window(
    Event_types     event_type );

  void  add_global_event_callback(
    Event_types               event_type,
    event_function_type       callback,
    Event_modifiers           modifier,
    void                      *callback_data );

  void  remove_global_event_callback(
    Event_types               event_type,
    event_function_type       callback,
    void                      *callback_data );

  VIO_BOOL  execute_global_event_callbacks(
    VIO_BOOL                 shift_state,
    Event_types             event_type,
    int                     key_pressed );

  void  initialize_global_events( void );

  void  delete_global_events( void );

  void  set_window_event_callbacks(
    graphics_window_struct    *window );

  void  popup_filter_selection(
    UI_struct   *ui,
    int         volume );

  VIO_Status  initialize_loading_volume(
    UI_struct  *ui_info,
    int        volume,
    VIO_STR     filename,
    VIO_BOOL    this_is_resampled_volume );

  void  cancel_loading(
    load_struct    *data );

  void  initialize_load_popup(
    load_struct   *load_data,
    int           x_position,
    int           y_position,
    VIO_STR        filename );

  void  delete_load_popup(
    load_struct   *load_data );

  void  set_load_popup_meter(
    load_struct   *load_data,
    VIO_Real          fraction_done );

  void  initialize_ui_colours( void );

  void  set_start_colour_table(
    UI_struct  *ui );

  int  get_ui_colour_index(
    UI_colours  colour_name );

  VIO_Colour  get_ui_rgb_colour(
    UI_colours  colour_name );

  VIO_Colour  get_ui_colour(
    VIO_BOOL        colour_map_state,
    UI_colours     colour_name );

  void  set_ui_colour(
    UI_struct    *ui,
    UI_colours   colour_name,
    VIO_Colour       colour );

  void  set_window_colours( window_struct *window );

  void  colour_map_state_has_changed( UI_struct  *ui );

  void  initialize_layout( UI_struct  *ui_info );

  void  resize_layout( UI_struct  *ui_info );

  VIO_BOOL  is_slice_viewport(
    Viewport_types   viewport );

  void  ui_get_volume_view_index(
    Viewport_types   view_index,
    int              *volume,
    int              *view );

  Viewport_types  ui_get_slice_viewport_index(
    int   volume,
    int   view );

  void  initialize_print_popup( void );

  void  disable_print_popup( void );

  void  popup_quit_confirm(
    UI_struct   *ui );

  void  do_resampling(
    UI_struct           *ui,
    VIO_General_transform   *resampling_transform,
    VIO_STR              resampled_filename );

  void  update_position_counters(
    UI_struct  *ui_info,
    int        volume_index );

  void  update_other_volume_positions(
    UI_struct   *ui_info,
    int         volume_index );

  void  ui_set_volume_time_position(
    UI_struct  *ui_info,
    int        volume_index,
    VIO_Real       tpos);

  void  ui_set_volume_voxel_position(
    UI_struct  *ui_info,
    int        volume_index,
    VIO_Real       position[VIO_N_DIMENSIONS] );

  void  ui_set_volume_original_world_position(
    UI_struct  *ui_info,
    int        volume_index,
    VIO_Real       position[VIO_N_DIMENSIONS] );

  Viewport_types  get_volume_menu_viewport_index(
    int   volume_index );

  int  get_viewport_volume_index(
    Viewport_types   viewport_index );

  void  set_merged_activity(
    UI_struct  *ui_info,
    VIO_BOOL    activity );

  VIO_BOOL   get_voxel_under_mouse(
    UI_struct        *ui_info,
    Viewport_types   event_viewport_index,
    int              *volume,
    VIO_Real             voxel_position[VIO_N_DIMENSIONS] );

  void  popup_transform_dialog(
    UI_struct   *ui );

  void  update_volume_readout(
    UI_struct  *ui,
    int        volume_index );

  void  install_readout_update_event(
    UI_struct   *ui );

  int  add_colour_bar_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               x,
    int               y,
    int               *height );

  void  set_colour_bar_widgets_activity(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               start_widget_index,
    VIO_BOOL           activity );

  void  set_over_under_colour_activity(
    UI_struct         *ui_info,
    int               volume_index,
    int               over_or_under,
    VIO_BOOL           activity );

  void  set_over_under_colour(
    UI_struct         *ui_info,
    int               volume_index,
    int               over_or_under,
    VIO_Colour            colour );

  void  set_colour_bar_limits(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_Real              min_value,
    VIO_Real              max_value );

  void  set_colour_bar_values(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_Real              min_value,
    VIO_Real              max_value );

  void  initialize_UI_widgets( UI_struct  *ui_info );

  void  delete_UI_widgets( UI_struct  *ui_info );

  void  update_all_widget_colours( UI_struct *ui_info );

  void  load_tags_file(
    UI_struct  *ui,
    VIO_STR     filename );

  void  record_tag(
    UI_struct  *ui,
    int        tag_index );

  void  add_main_widgets(
    UI_struct         *ui_info );

  void  position_main_widgets(
    UI_struct         *ui_info );

  void  set_transform_buttons_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity );

  void  set_quit_button_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity );

  void  set_resample_button_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity );

  void  set_delete_tags_button_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity );

  void  set_transform_type_button_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity );

  void  update_avg_rms_error(
    UI_struct         *ui_info );

  void  add_merged_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index );

  void  set_merged_widgets_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity );

  widget_struct  *get_merged_readout_widget(
    UI_struct     *ui_info,
    int           volume );

widget_struct  *get_merged_blend_widget( UI_struct     *ui_info );

  void  initialize_meter(
    popup_struct     *popup,
    int              x_size,
    int              y_size,
    VIO_Colour           background_colour,
    VIO_Colour           colour,
    polygons_struct  **meter_background,
    polygons_struct  **meter );

  void  set_meter_position(
    popup_struct      *popup,
    polygons_struct   *meter_background,
    polygons_struct   *meter,
    VIO_Real              fraction_done );

  int  add_cursor_position_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               *height,
    VIO_BOOL          show_time );

  void  set_voxel_position_widgets_activity(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               start_widget_index,
    VIO_BOOL          activity,
    VIO_BOOL          has_time_axis);

  void  set_volume_voxel_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    VIO_Real              value );

  void  set_volume_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    VIO_Real              value );

  void  set_volume_time_text(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_Real              value );

  void  previous_current_tag_point( UI_struct *ui );

  void  advance_current_tag_point( UI_struct  *ui );

  void  set_tag_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               tag_index,
    int               axis_index,
    VIO_Real              value );

  void  add_tag_point_widgets(
    UI_struct         *ui_info,
    int               n_tag_points,
    Viewport_types    rms_viewport_index,
    Viewport_types    volume1_viewport_index,
    Viewport_types    volume2_viewport_index,
    Viewport_types    names_viewport_index );

  void  delete_tag_point_widgets_indices( void );

  void  update_tag_widgets(
    UI_struct         *ui_info,
    int               tag );

  void  update_all_tag_widgets(
    UI_struct         *ui_info );

  void  set_current_tag_index(
    UI_struct      *ui_info,
    int            tag_index );

  int  get_current_tag(
    UI_struct      *ui_info );

  int  get_current_tag_index(
    UI_struct      *ui_info );

  void  delete_current_tag_point(
    UI_struct  *ui );

  void  delete_all_tag_points(
    UI_struct  *ui );

  DEFINE_WIDGET_CALLBACK( reset_view_callback );

  DEFINE_WIDGET_CALLBACK( popup_filter_callback );

  void  add_volume_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index );

  void  set_load_activity(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_BOOL           state );

  void  set_load_filename(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_STR            filename );

  void  set_volume_widgets_activity(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_BOOL          activity,
    VIO_BOOL          is_rgb_volume,
    VIO_BOOL          has_time_axis);

  int  get_colour_bar_start_index( void );

  void  set_resampled_label_activity(
    UI_struct         *ui_info,
    VIO_BOOL           state );

  widget_struct  *get_volume_readout_widget(
    UI_struct     *ui_info,
    int           volume );

  void  set_filter_popup_activity(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_BOOL           activity );

widget_struct *get_volume_weight_widget( UI_struct *ui_info, int volume_index );
void set_volume_blend_value( UI_struct *ui_info, VIO_Real value );

  void  define_radio_buttons(
    int            n_widgets,
    widget_struct  *widgets[] );

  void  update_button_activity(
    widget_struct           *widget );

  void  update_button_colours(
    widget_struct   *widget );

  void  set_button_active_colour(
    widget_struct   *widget,
    VIO_Colour          col );

  VIO_Colour  get_button_colour(
    widget_struct   *widget );

  void  position_button(
    widget_struct                 *widget,
    int                           x,
    int                           y );

  void  set_toggle_button_state(
    widget_struct    *widget,
    int              state );

  void  set_button_text(
    widget_struct    *widget,
    VIO_STR           text_string );

  void  delete_button(
    widget_struct  *widget );

  widget_struct *create_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_STR                     label,
    VIO_BOOL                    initial_activity,
    VIO_BOOL                    use_ui_colours,
    VIO_Colour                     active_colour,
    VIO_Colour                     selected_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    Font_types                 text_font,
    VIO_Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data );

  widget_struct  *create_toggle_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_STR                     off_text,
    VIO_STR                     on_text,
    VIO_BOOL                    initial_state,
    VIO_BOOL                    initial_activity,
    VIO_BOOL                    use_ui_colours,
    VIO_Colour                     active_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    Font_types                 text_font,
    VIO_Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data );

  widget_struct  *create_multi_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    int                        n_states,
    VIO_STR                    state_text[],
    int                        initial_state,
    VIO_BOOL                   initial_activity,
    VIO_BOOL                   use_ui_colours,
    VIO_Colour                 active_colour,
    VIO_Colour                 inactive_colour,
    VIO_Colour                 text_colour,
    Font_types                 text_font,
    VIO_Real                   font_size,
    widget_callback_type       push_callback,
    void                       *callback_data );

  void  update_slider_colours(
    widget_struct   *widget );

  void  get_slider_values(
    widget_struct           *widget,
    VIO_Real                    *low_value,
    VIO_Real                    *high_value );

  void  set_slider_values(
    widget_struct           *widget,
    VIO_Real                    low_value,
    VIO_Real                    high_value );

  void  set_slider_limits(
    widget_struct  *widget,
    VIO_Real           min_value,
    VIO_Real           max_value );

  void  update_slider_activity(
    widget_struct           *widget );

  void  position_slider(
    widget_struct                 *widget,
    int                           x,
    int                           y );

  void  delete_slider(
    widget_struct  *widget );

  widget_struct  *create_slider(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_Real                       initial_value,
    VIO_Real                       min_value,
    VIO_Real                       max_value,
    VIO_STR                     format_string,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     peg_colour,
    widget_callback_type       value_changed_callback,
    void                       *value_changed_data );

  widget_struct  *create_colour_bar_slider(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_Real                       initial_low_value,
    VIO_Real                       initial_high_value,
    VIO_Real                       min_value,
    VIO_Real                       max_value,
    VIO_STR                     format_string,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     peg_colour,
    widget_callback_type       lower_value_changed_callback,
    void                       *lower_value_callback_data,
    widget_callback_type       upper_value_changed_callback,
    void                       *upper_value_callback_data );

  void  position_text_entry(
    widget_struct                 *widget,
    int                           x,
    int                           y );

  VIO_STR  get_text_entry_string(
    widget_struct  *widget );

  void  set_text_entry_string(
    widget_struct  *widget,
    VIO_STR         string );

  void  restore_text_entry_string(
    widget_struct  *widget );

  void  update_text_entry_colours(
    widget_struct  *widget );

  void  update_text_entry_activity(
    widget_struct           *widget );

  void  delete_text_entry(
    widget_struct  *widget );

  widget_struct  *create_text_entry(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_BOOL                    clear_text_when_selected,
    VIO_STR                     initial_text,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     selected_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    VIO_Colour                     edit_colour,
    VIO_Colour                     text_edit_colour,
    VIO_Colour                     cursor_colour,
    Font_types                 text_font,
    VIO_Real                       font_size,
    widget_callback_type       hit_return_callback,
    void                       *callback_data );

  widget_struct  *create_label(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_STR                     initial_text,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     selected_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     text_colour,
    Font_types                 text_font,
    VIO_Real                       font_size );

  object_struct  *create_rectangle(
    VIO_Colour       colour );

  void  position_rectangle(
    polygons_struct    *polygons,
    int                x,
    int                y,
    int                x_size,
    int                y_size );

  object_struct  *create_text(
    VIO_Colour           colour,
    Font_types       text_font,
    VIO_Real             font_size );

  void  position_text(
    text_struct   *text,
    int           x,
    int           y,
    int           y_size );

  void  position_text_centred(
    text_struct    *text,
    int            x,
    int            y,
    int            x_size,
    int            y_size );

  VIO_BOOL  get_toggle_button_state(
    widget_struct  *widget );

  VIO_BOOL  get_text_entry_real_value(
    widget_struct  *widget,
    VIO_Real           *value );

  void  set_text_entry_real_value(
    widget_struct     *widget,
    VIO_STR            format,
    VIO_Real              value );

  void  set_button_int_value(
    widget_struct     *widget,
    VIO_STR            format,
    int               value );

  void  initialize_widget_list(
    widgets_struct   *widget_list );

  void  delete_widget_list(
    widgets_struct   *widget_list );

  int  add_widget_to_list(
    widgets_struct   *widget_list,
    widget_struct    *widget );

  void  delete_nth_widget_from_list(
    widgets_struct   *widget_list,
    int              index );

  widget_struct  *create_widget(
    Widget_types            type,
    int                     x,
    int                     y,
    int                     x_size,
    int                     y_size,
    VIO_BOOL                 initial_activity,
    VIO_BOOL                 use_ui_colours,
    graphics_window_struct  *graphics,
    Viewport_types          viewport_index );

  int  get_widget_height(
    widget_struct   *widget );

  void  position_widget(
    widget_struct           *widget,
    int                     x,
    int                     y );

  button_struct  *get_widget_button(
    widget_struct  *widget );

  text_entry_struct  *get_widget_text_entry(
    widget_struct  *widget );

  slider_struct  *get_widget_slider(
    widget_struct  *widget );

  void  set_widget_activity(
    widget_struct  *widget,
    VIO_BOOL        activity );

  void  set_widget_selected(
    widget_struct  *widget,
    VIO_BOOL        selected );

  void  update_widget_colours(
    widget_struct  *widget );

  void  update_widget_list_colours(
    widgets_struct      *widget_list );

  void  delete_widget(
    widget_struct  *widget );

  void  record_graphics_window(
    graphics_window_struct   *graphics_window );

  void  unrecord_graphics_window(
    graphics_window_struct   *graphics_window );

  VIO_BOOL   lookup_event_viewports(
    window_struct           *window,
    event_viewports_struct  **event_viewports );

  void  update_window(
    graphics_window_struct   *window );

  void  make_windows_up_to_date( void );

  void  delete_all_graphics_windows( void );

  void   create_popup_window(
    popup_struct          *popup,
    VIO_STR                title,
    int                   x_position,
    int                   y_position,
    int                   x_size,
    int                   y_size,
    event_function_type   quit_popup_callback,
    void                  *quit_callback_data );

  void  delete_popup_window(
    popup_struct   *popup );

  VIO_BOOL  make_window_up_to_date(
    window_struct    *window,
    graphics_struct  *graphics,
    int              current_buffer );

  void  set_clear_and_update_flags( UI_struct  *ui_struct );

  void  set_recreate_all_slice_flags( void );

  void  xs_display(UI_struct *ui_info,
                         Viewport_types viewport, int force);
/**
 * @}
 */
#endif

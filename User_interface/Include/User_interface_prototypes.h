#ifndef  DEF_User_interface_prototypes
#define  DEF_User_interface_prototypes

public  UI_struct  *get_ui_struct( void );

public  Status   initialize_user_interface(
    UI_struct  *ui,
    STRING     executable_name );

public  void   terminate_user_interface(
    UI_struct  *ui );

public  void  popup_colour_selection(
    UI_struct   *ui,
    int         volume,
    int         over_or_under );

public  void  popup_delete_tags(
    UI_struct   *ui );

public  void  save_current_viewport(
    UI_struct        *ui,
    Viewport_types   viewport );

public  DEFINE_EVENT_FUNCTION( slice_key_down_callback );

public  void  install_slice_events(
    event_viewports_struct   *event_table,
    int                     volume );

public  void  remove_slice_events(
    event_viewports_struct   *event_table,
    int                     volume );

public  void  install_tag_events(
    UI_struct                *ui );

public  void  start_interaction(
    UI_struct              *ui_info,
    Viewport_types         event_viewport_index,
    Event_types            terminate_event,
    event_function_type    terminate_callback,
    event_function_type    no_event_callback );

public  void  terminate_interaction(
    Event_types            terminate_event,
    event_function_type    terminate_callback,
    event_function_type    no_event_callback );

public  void  restore_mouse_position(
    UI_struct   *ui );

public  void  get_viewport_mouse_position(
    graphics_window_struct    *graphics_window,
    Viewport_types            event_viewport_index,
    int                       *x_mouse,
    int                       *y_mouse );

public  void  install_window_events(
    UI_struct  *ui );

public  void  initialize_event_table( event_callback_list_struct  table[] );

public  void  delete_event_table( event_callback_list_struct  table[] );

public  void  set_event_callback_enabled(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data,
    BOOLEAN                      enabled );

public  void  add_event_callback_function(
    event_callback_list_struct   *callback_list,
    int                          x_min,
    int                          x_max,
    int                          y_min,
    int                          y_max,
    event_function_type          callback_function,
    Event_modifiers              modifier,
    void                         *callback_data );

public  void  set_event_callback_viewport(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data,
    int                          x_min,
    int                          x_max,
    int                          y_min,
    int                          y_max );

public  void  remove_event_callback_function(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data );

public  BOOLEAN  execute_event_callback_functions(
    BOOLEAN                      shift_state,
    event_callback_list_struct   *callback_list,
    int                          mouse_x,
    int                          mouse_y,
    Viewport_types               event_viewport_index,
    int                          key_pressed );

public  void  set_quit_program_flag( void );

public  void  event_loop( void );

public  void   handle_event(
    Event_types       event_type,
    Gwindow           event_window,
    int               key_pressed );

public  void  initialize_event_viewports(
    event_viewports_struct  *event_viewports );

public  void  delete_event_viewports(
    event_viewports_struct  *event_viewports );

public  void  set_event_viewport(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    int                     x_min,
    int                     x_max,
    int                     y_min,
    int                     y_max );

public  void  get_event_viewport(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    int                     *x_min,
    int                     *x_max,
    int                     *y_min,
    int                     *y_max );

public  void  set_event_viewport_callback_enabled(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data,
    BOOLEAN                 enabled );

public  void  set_event_viewport_callback_viewport(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data,
    int                     x_min,
    int                     x_max,
    int                     y_min,
    int                     y_max );

public  void  add_event_viewport_callback(
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

public  void  remove_event_viewport_callback(
    event_viewports_struct  *event_viewports,
    Viewport_types          event_viewport_index,
    Event_types             event_type,
    event_function_type     callback_function,
    void                    *callback_data );

public  void  execute_event_viewport_events(
    BOOLEAN                 shift_state,
    event_viewports_struct  *event_viewports,
    int                     x_mouse,
    int                     y_mouse,
    Event_types             event_type,
    int                     key_pressed );

public  BOOLEAN  find_viewport_containing_mouse(
    event_viewports_struct  *event_viewports,
    int                     x_mouse,
    int                     y_mouse,
    Viewport_types          *viewport_index );

public  BOOLEAN  event_is_allowable(
    Event_types     event_type );

public  void  set_interaction_in_progress(
    BOOLEAN  state );

public  BOOLEAN  mouse_must_be_in_window(
    Event_types     event_type );

public  void  add_global_event_callback(
    Event_types               event_type,
    event_function_type       callback,
    Event_modifiers           modifier,
    void                      *callback_data );

public  void  remove_global_event_callback(
    Event_types               event_type,
    event_function_type       callback,
    void                      *callback_data );

public  BOOLEAN  execute_global_event_callbacks(
    BOOLEAN                 shift_state,
    Event_types             event_type,
    int                     key_pressed );

public  void  initialize_global_events( void );

public  void  delete_global_events( void );

public  void  set_window_event_callbacks(
    graphics_window_struct    *window );

public  void  popup_filter_selection(
    UI_struct   *ui,
    int         volume );

public  Status  initialize_loading_volume(
    UI_struct  *ui_info,
    int        volume,
    STRING     filename,
    BOOLEAN    this_is_resampled_volume );

public  void  cancel_loading(
    load_struct    *data );

public  void  initialize_load_popup(
    load_struct   *load_data,
    int           x_position,
    int           y_position,
    STRING        filename );

public  void  delete_load_popup(
    load_struct   *load_data );

public  void  set_load_popup_meter(
    load_struct   *load_data,
    Real          fraction_done );

public  void  initialize_ui_colours( void );

public  void  set_start_colour_table(
    UI_struct  *ui );

public  int  get_ui_colour_index(
    UI_colours  colour_name );

public  Colour  get_ui_rgb_colour(
    UI_colours  colour_name );

public  Colour  get_ui_colour(
    BOOLEAN        colour_map_state,
    UI_colours     colour_name );

public  void  set_ui_colour(
    UI_struct    *ui,
    UI_colours   colour_name,
    Colour       colour );

public  void  set_window_colours( window_struct *window );

public  void  colour_map_state_has_changed( UI_struct  *ui );

public  void  initialize_layout( UI_struct  *ui_info );

public  void  resize_layout( UI_struct  *ui_info );

public  BOOLEAN  is_slice_viewport(
    Viewport_types   viewport );

public  void  ui_get_volume_view_index(
    Viewport_types   view_index,
    int              *volume,
    int              *view );

public  Viewport_types  ui_get_slice_viewport_index(
    int   volume,
    int   view );

public  void  initialize_print_popup( void );

public  void  disable_print_popup( void );

public  void  popup_quit_confirm(
    UI_struct   *ui );

public  void  do_resampling(
    UI_struct           *ui,
    General_transform   *resampling_transform,
    STRING              resampled_filename );

public  void  update_position_counters(
    UI_struct  *ui_info,
    int        volume_index );

public  void  update_other_volume_positions(
    UI_struct   *ui_info,
    int         volume_index );

public  void  ui_set_volume_time_position(
    UI_struct  *ui_info,
    int        volume_index,
    Real       tpos);

public  void  ui_set_volume_voxel_position(
    UI_struct  *ui_info,
    int        volume_index,
    Real       position[N_DIMENSIONS] );

public  void  ui_set_volume_original_world_position(
    UI_struct  *ui_info,
    int        volume_index,
    Real       position[N_DIMENSIONS] );

public  Viewport_types  get_volume_menu_viewport_index(
    int   volume_index );

public  int  get_viewport_volume_index(
    Viewport_types   viewport_index );

public  void  set_merged_activity(
    UI_struct  *ui_info,
    BOOLEAN    activity );

public  BOOLEAN   get_voxel_under_mouse(
    UI_struct        *ui_info,
    Viewport_types   event_viewport_index,
    int              *volume,
    Real             voxel_position[N_DIMENSIONS] );

public  void  popup_transform_dialog(
    UI_struct   *ui );

public  void  update_volume_readout(
    UI_struct  *ui,
    int        volume_index );

public  void  install_readout_update_event(
    UI_struct   *ui );

public  int  add_colour_bar_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               x,
    int               y,
    int               *height );

public  void  set_colour_bar_widgets_activity(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               start_widget_index,
    BOOLEAN           activity );

public  void  set_over_under_colour_activity(
    UI_struct         *ui_info,
    int               volume_index,
    int               over_or_under,
    BOOLEAN           activity );

public  void  set_over_under_colour(
    UI_struct         *ui_info,
    int               volume_index,
    int               over_or_under,
    Colour            colour );

public  void  set_colour_bar_limits(
    UI_struct         *ui_info,
    int               volume_index,
    Real              min_value,
    Real              max_value );

public  void  set_colour_bar_values(
    UI_struct         *ui_info,
    int               volume_index,
    Real              min_value,
    Real              max_value );

public  void  initialize_UI_widgets( UI_struct  *ui_info );

public  void  delete_UI_widgets( UI_struct  *ui_info );

public  void  update_all_widget_colours( UI_struct *ui_info );

public  void  load_tags_file(
    UI_struct  *ui,
    STRING     filename );

public  void  record_tag(
    UI_struct  *ui,
    int        tag_index );

public  void  add_main_widgets(
    UI_struct         *ui_info );

public  void  position_main_widgets(
    UI_struct         *ui_info );

public  void  set_transform_buttons_activity(
    UI_struct         *ui_info,
    BOOLEAN           activity );

public  void  set_quit_button_activity(
    UI_struct         *ui_info,
    BOOLEAN           activity );

public  void  set_resample_button_activity(
    UI_struct         *ui_info,
    BOOLEAN           activity );

public  void  set_delete_tags_button_activity(
    UI_struct         *ui_info,
    BOOLEAN           activity );

public  void  set_transform_type_button_activity(
    UI_struct         *ui_info,
    BOOLEAN           activity );

public  void  update_avg_rms_error(
    UI_struct         *ui_info );

public  void  update_colour_map_toggle_activity(
    UI_struct         *ui_info );

public  void  add_merged_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index );

public  void  set_merged_widgets_activity(
    UI_struct         *ui_info,
    BOOLEAN           activity );

public  widget_struct  *get_merged_readout_widget(
    UI_struct     *ui_info,
    int           volume );

public  void  initialize_meter(
    popup_struct     *popup,
    int              x_size,
    int              y_size,
    Colour           background_colour,
    Colour           colour,
    polygons_struct  **meter_background,
    polygons_struct  **meter );

public  void  set_meter_position(
    popup_struct      *popup,
    polygons_struct   *meter_background,
    polygons_struct   *meter,
    Real              fraction_done );

public  int  add_cursor_position_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               *height );

public  void  set_voxel_position_widgets_activity(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               start_widget_index,
    BOOLEAN           activity );

public  void  set_volume_voxel_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    Real              value );

public  void  set_volume_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    Real              value );

public  void  set_volume_time_text(
    UI_struct         *ui_info,
    int               volume_index,
    Real              value );

public  void  previous_current_tag_point( UI_struct *ui );

public  void  advance_current_tag_point( UI_struct  *ui );

public  void  set_tag_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               tag_index,
    int               axis_index,
    Real              value );

public  void  add_tag_point_widgets(
    UI_struct         *ui_info,
    int               n_tag_points,
    Viewport_types    rms_viewport_index,
    Viewport_types    volume1_viewport_index,
    Viewport_types    volume2_viewport_index,
    Viewport_types    names_viewport_index );

public  void  delete_tag_point_widgets_indices( void );

public  void  update_tag_widgets(
    UI_struct         *ui_info,
    int               tag );

public  void  update_all_tag_widgets(
    UI_struct         *ui_info );

public  void  set_current_tag_index(
    UI_struct      *ui_info,
    int            tag_index );

public  int  get_current_tag(
    UI_struct      *ui_info );

public  int  get_current_tag_index(
    UI_struct      *ui_info );

public  void  delete_current_tag_point(
    UI_struct  *ui );

public  void  delete_all_tag_points(
    UI_struct  *ui );

public  DEFINE_WIDGET_CALLBACK( reset_view_callback );

public  DEFINE_WIDGET_CALLBACK( popup_filter_callback );

public  void  add_volume_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index );

public  void  set_load_activity(
    UI_struct         *ui_info,
    int               volume_index,
    BOOLEAN           state );

public  void  set_load_filename(
    UI_struct         *ui_info,
    int               volume_index,
    STRING            filename );

public  void  set_volume_widgets_activity(
    UI_struct         *ui_info,
    int               volume_index,
    BOOLEAN           activity,
    BOOLEAN           is_rgb_volume );

public  int  get_colour_bar_start_index( void );

public  void  set_resampled_label_activity(
    UI_struct         *ui_info,
    BOOLEAN           state );

public  widget_struct  *get_volume_readout_widget(
    UI_struct     *ui_info,
    int           volume );

public  void  set_filter_popup_activity(
    UI_struct         *ui_info,
    int               volume_index,
    BOOLEAN           activity );

public  void  define_radio_buttons(
    int            n_widgets,
    widget_struct  *widgets[] );

public  void  update_button_activity(
    widget_struct           *widget );

public  void  update_button_colours(
    widget_struct   *widget );

public  void  set_button_active_colour(
    widget_struct   *widget,
    Colour          col );

public  Colour  get_button_colour(
    widget_struct   *widget );

public  void  position_button(
    widget_struct                 *widget,
    int                           x,
    int                           y );

public  void  set_toggle_button_state(
    widget_struct    *widget,
    BOOLEAN          state );

public  void  set_button_text(
    widget_struct    *widget,
    STRING           text_string );

public  void  delete_button(
    widget_struct  *widget );

public  widget_struct *create_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    STRING                     label,
    BOOLEAN                    initial_activity,
    BOOLEAN                    use_ui_colours,
    Colour                     active_colour,
    Colour                     selected_colour,
    Colour                     inactive_colour,
    Colour                     text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data );

public  widget_struct  *create_toggle_button(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    STRING                     off_text,
    STRING                     on_text,
    BOOLEAN                    initial_state,
    BOOLEAN                    initial_activity,
    BOOLEAN                    use_ui_colours,
    Colour                     active_colour,
    Colour                     inactive_colour,
    Colour                     text_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       push_callback,
    void                       *callback_data );

public  void  update_slider_colours(
    widget_struct   *widget );

public  void  get_slider_values(
    widget_struct           *widget,
    Real                    *low_value,
    Real                    *high_value );

public  void  set_slider_values(
    widget_struct           *widget,
    Real                    low_value,
    Real                    high_value );

public  void  set_slider_limits(
    widget_struct  *widget,
    Real           min_value,
    Real           max_value );

public  void  update_slider_activity(
    widget_struct           *widget );

public  void  position_slider(
    widget_struct                 *widget,
    int                           x,
    int                           y );

public  void  delete_slider(
    widget_struct  *widget );

public  widget_struct  *create_slider(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    Real                       initial_value,
    Real                       min_value,
    Real                       max_value,
    STRING                     format_string,
    BOOLEAN                    initial_activity,
    Colour                     active_colour,
    Colour                     inactive_colour,
    Colour                     peg_colour,
    widget_callback_type       value_changed_callback,
    void                       *value_changed_data );

public  widget_struct  *create_colour_bar_slider(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    Real                       initial_low_value,
    Real                       initial_high_value,
    Real                       min_value,
    Real                       max_value,
    STRING                     format_string,
    BOOLEAN                    initial_activity,
    Colour                     active_colour,
    Colour                     inactive_colour,
    Colour                     peg_colour,
    widget_callback_type       lower_value_changed_callback,
    void                       *lower_value_callback_data,
    widget_callback_type       upper_value_changed_callback,
    void                       *upper_value_callback_data );

public  void  position_text_entry(
    widget_struct                 *widget,
    int                           x,
    int                           y );

public  STRING  get_text_entry_string(
    widget_struct  *widget );

public  void  set_text_entry_string(
    widget_struct  *widget,
    STRING         string );

public  void  restore_text_entry_string(
    widget_struct  *widget );

public  void  update_text_entry_colours(
    widget_struct  *widget );

public  void  update_text_entry_activity(
    widget_struct           *widget );

public  void  delete_text_entry(
    widget_struct  *widget );

public  widget_struct  *create_text_entry(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    BOOLEAN                    clear_text_when_selected,
    STRING                     initial_text,
    BOOLEAN                    initial_activity,
    Colour                     active_colour,
    Colour                     selected_colour,
    Colour                     inactive_colour,
    Colour                     text_colour,
    Colour                     edit_colour,
    Colour                     text_edit_colour,
    Colour                     cursor_colour,
    Font_types                 text_font,
    Real                       font_size,
    widget_callback_type       hit_return_callback,
    void                       *callback_data );

public  widget_struct  *create_label(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    STRING                     initial_text,
    BOOLEAN                    initial_activity,
    Colour                     active_colour,
    Colour                     selected_colour,
    Colour                     inactive_colour,
    Colour                     text_colour,
    Font_types                 text_font,
    Real                       font_size );

public  object_struct  *create_rectangle(
    Colour       colour );

public  void  position_rectangle(
    polygons_struct    *polygons,
    int                x,
    int                y,
    int                x_size,
    int                y_size );

public  object_struct  *create_text(
    Colour           colour,
    Font_types       text_font,
    Real             font_size );

public  void  position_text(
    text_struct   *text,
    int           x,
    int           y,
    int           y_size );

public  void  position_text_centred(
    text_struct    *text,
    int            x,
    int            y,
    int            x_size,
    int            y_size );

public  BOOLEAN  get_toggle_button_state(
    widget_struct  *widget );

public  BOOLEAN  get_text_entry_real_value(
    widget_struct  *widget,
    Real           *value );

public  void  set_text_entry_real_value(
    widget_struct     *widget,
    STRING            format,
    Real              value );

public  void  set_button_int_value(
    widget_struct     *widget,
    STRING            format,
    int               value );

public  void  initialize_widget_list(
    widgets_struct   *widget_list );

public  void  delete_widget_list(
    widgets_struct   *widget_list );

public  int  add_widget_to_list(
    widgets_struct   *widget_list,
    widget_struct    *widget );

public  void  delete_nth_widget_from_list(
    widgets_struct   *widget_list,
    int              index );

public  widget_struct  *create_widget(
    Widget_types            type,
    int                     x,
    int                     y,
    int                     x_size,
    int                     y_size,
    BOOLEAN                 initial_activity,
    BOOLEAN                 use_ui_colours,
    graphics_window_struct  *graphics,
    Viewport_types          viewport_index );

public  int  get_widget_height(
    widget_struct   *widget );

public  void  position_widget(
    widget_struct           *widget,
    int                     x,
    int                     y );

public  button_struct  *get_widget_button(
    widget_struct  *widget );

public  text_entry_struct  *get_widget_text_entry(
    widget_struct  *widget );

public  slider_struct  *get_widget_slider(
    widget_struct  *widget );

public  void  set_widget_activity(
    widget_struct  *widget,
    BOOLEAN        activity );

public  void  set_widget_selected(
    widget_struct  *widget,
    BOOLEAN        selected );

public  void  update_widget_colours(
    widget_struct  *widget );

public  void  update_widget_list_colours(
    widgets_struct      *widget_list );

public  void  delete_widget(
    widget_struct  *widget );

public  void  record_graphics_window(
    graphics_window_struct   *graphics_window );

public  void  unrecord_graphics_window(
    graphics_window_struct   *graphics_window );

public  BOOLEAN   lookup_event_viewports(
    window_struct           *window,
    event_viewports_struct  **event_viewports );

public  void  update_window(
    graphics_window_struct   *window );

public  void  make_windows_up_to_date( void );

public  void  delete_all_graphics_windows( void );

public  void   create_popup_window(
    popup_struct          *popup,
    STRING                title,
    int                   x_position,
    int                   y_position,
    int                   x_size,
    int                   y_size,
    event_function_type   quit_popup_callback,
    void                  *quit_callback_data );

public  void  delete_popup_window(
    popup_struct   *popup );

public  BOOLEAN  make_window_up_to_date(
    window_struct    *window,
    graphics_struct  *graphics,
    int              current_buffer );

public  void  set_clear_and_update_flags( UI_struct  *ui_struct );

public  void  set_recreate_all_slice_flags( void );

public  void  xs_display(UI_struct *ui_info,
                         Viewport_types viewport, int force);
#endif

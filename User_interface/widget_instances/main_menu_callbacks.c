#include  <def_user_interface.h>

typedef  enum
{
    QUIT_BUTTON,
    RESAMPLE_BUTTON,
    SYNC_VOLUMES_BUTTON,
    INTERPOLATION_BUTTON,
    COLOUR_MODE_BUTTON,
    DOUBLE_BUFFER_BUTTON,
    LOAD_TAGS_BUTTON,
    TAGS_FILENAME_ENTRY,
    SAVE_TAGS_BUTTON,
    SAVE_TRANSFORM_BUTTON,
    TRANSFORM_FILENAME_ENTRY,
    RECORD_TAG_BUTTON,
    DELETE_TAG_BUTTON,
    N_MAIN_WIDGETS
} Main_widgets;

private  int  widget_indices[N_MAIN_WIDGETS];

private  DEFINE_WIDGET_CALLBACK( quit_button_callback ) /* ARGSUSED */
{
    popup_quit_confirm( get_ui_struct() );
}

private  DEFINE_WIDGET_CALLBACK( resample_and_load_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( sync_volumes_button_callback ) /* ARGSUSED */
{
    get_ui_struct()->volumes_synced = !get_ui_struct()->volumes_synced;

    if( get_ui_struct()->volumes_synced )
    {
        update_other_volume_positions( get_ui_struct(), 0 );
    }
}

private  DEFINE_WIDGET_CALLBACK( interpolation_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( colour_mode_button_callback ) /* ARGSUSED */
{
    int            state;

    state = get_toggle_button_state( widget );

    G_set_colour_map_state( get_ui_struct()->graphics_window.window, state );

    colour_map_state_has_changed( get_ui_struct() );

    set_clear_and_update_flags( get_ui_struct() );
}

private  DEFINE_WIDGET_CALLBACK( double_buffer_button_callback ) /* ARGSUSED */
{
    int            state;

    state = get_toggle_button_state( widget );

    G_set_double_buffer_state( get_ui_struct()->graphics_window.window, state );

    colour_map_state_has_changed( get_ui_struct() );

    set_clear_and_update_flags( get_ui_struct() );
}

private  DEFINE_WIDGET_CALLBACK( load_tags_button_callback ) /* ARGSUSED */
{
    char   *filename;

    filename = get_text_entry_string(
                     get_ui_struct()->widget_list[Main_menu_viewport].widgets
                                 [widget_indices[TAGS_FILENAME_ENTRY]] );

    IF_load_tags_file( filename );

    get_ui_struct()->tag_points.first_tag_displayed = 0;   
    set_current_tag_index( get_ui_struct(), 0 );
}

private  DEFINE_WIDGET_CALLBACK( tags_filename_hit_return_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( save_tags_button_callback ) /* ARGSUSED */
{
    char   *filename;

    filename = get_text_entry_string(
                     get_ui_struct()->widget_list[Main_menu_viewport].widgets
                                 [widget_indices[TAGS_FILENAME_ENTRY]] );

    IF_save_tags_file( filename );
}

private  DEFINE_WIDGET_CALLBACK( transform_filename_hit_return_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( save_transform_button_callback ) /* ARGSUSED */
{
    char   *filename;

    filename = get_text_entry_string(
                     get_ui_struct()->widget_list[Main_menu_viewport].widgets
                                 [widget_indices[TRANSFORM_FILENAME_ENTRY]] );

    IF_save_transform( filename );
}

private  DEFINE_WIDGET_CALLBACK( record_tag_button_callback ) /* ARGSUSED */
{
    int      volume, tag_index;
    Real     position[N_DIMENSIONS];

    tag_index = get_current_tag_index( get_ui_struct() );

    if( tag_index >= IF_get_n_tag_points() )
        IF_create_new_tag_point();

    for_less( volume, 0, N_VOLUMES )
    {
        if( IF_volume_is_loaded( volume ) )
        {
            IF_get_volume_world_position( volume, position );
            IF_set_tag_point_position( tag_index, volume, position );
        }
    }

    if( tag_index == IF_get_n_tag_points() - 1 )
        ++tag_index;

    set_current_tag_index( get_ui_struct(), tag_index );
}

private  DEFINE_WIDGET_CALLBACK( delete_tag_button_callback ) /* ARGSUSED */
{
    int      tag_index;

    tag_index = get_current_tag_index( get_ui_struct() );

    if( tag_index < IF_get_n_tag_points() )
    {
        IF_delete_tag_point( tag_index );
        if( tag_index > IF_get_n_tag_points() )
            --tag_index;
        set_current_tag_index( get_ui_struct(), tag_index );
    }
}

public  void  add_main_widgets(
    UI_struct         *ui_info )
{
    widget_indices[QUIT_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Quit",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   quit_button_callback, (void *) 0 ) );

    widget_indices[RESAMPLE_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Resample", 
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   resample_and_load_button_callback, (void *) 0 ) );

    widget_indices[SYNC_VOLUMES_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_toggle_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Not Synced",
                   "Synced",
                   ui_info->volumes_synced, ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   sync_volumes_button_callback, (void *) 0 ) );

    widget_indices[INTERPOLATION_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_toggle_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Flat", "Smooth", FALSE,
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   interpolation_button_callback, (void *) 0 ) );

    widget_indices[COLOUR_MODE_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_toggle_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "RGB", "Colour Map",
                   G_get_colour_map_state(ui_info->graphics_window.window),
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   colour_mode_button_callback, (void *) 0 ) );

    widget_indices[DOUBLE_BUFFER_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_toggle_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Single", "Double",
                   G_get_double_buffer_state(ui_info->graphics_window.window),
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   double_buffer_button_callback, (void *) 0 ) );

    widget_indices[LOAD_TAGS_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Load Tags",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   load_tags_button_callback, (void *) 0 ) );

    widget_indices[TAGS_FILENAME_ENTRY] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_text_entry( &ui_info->graphics_window,
                       Main_menu_viewport, 
                       0, 0, Tags_filename_width, Text_entry_height,
                       "", ON,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       tags_filename_hit_return_callback, (void *) 0 ) );

    widget_indices[SAVE_TAGS_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Save Tags",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   save_tags_button_callback, (void *) 0 ) );

    widget_indices[SAVE_TRANSFORM_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Save Transform",
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   save_transform_button_callback, (void *) 0 ) );

    widget_indices[TRANSFORM_FILENAME_ENTRY] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_text_entry( &ui_info->graphics_window,
                       Main_menu_viewport, 
                       0, 0, Tags_filename_width, Text_entry_height,
                       "", ON,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       transform_filename_hit_return_callback, (void *) 0 ) );

    widget_indices[RECORD_TAG_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Record Tag",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   record_tag_button_callback, (void *) 0 ) );

    widget_indices[DELETE_TAG_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Delete Tag",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   delete_tag_button_callback, (void *) 0 ) );

    position_main_widgets( ui_info );
}

public  void  position_main_widgets(
    UI_struct         *ui_info )
{
    int           x, y_pos, x_pos, x_min, x_max, y_min, y_max;
    Main_widgets  widget;

    get_graphics_viewport( &ui_info->graphics_window.graphics,
                           Main_menu_viewport,
                           &x_min, &x_max, &y_min, &y_max );

    x = Main_menu_x_offset;
    y_pos = y_max - y_min - 1 - Main_menu_y_offset;

    for_enum( widget, N_MAIN_WIDGETS, Main_widgets )
    {
        if( widget == TAGS_FILENAME_ENTRY ||
            widget == TRANSFORM_FILENAME_ENTRY )
            x_pos = Tags_filename_x_offset;
        else
            x_pos = x;

        position_widget( ui_info->
                         widget_list[Main_menu_viewport].widgets[widget],
                         x_pos, y_pos );

        y_pos -= get_widget_height( ui_info->widget_list[Main_menu_viewport].
                                                      widgets[widget] ) +
                 Interface_y_spacing;
    }
}

public  void  set_transform_buttons_activity(
    UI_struct         *ui_info,
    Boolean           activity )
{
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[SAVE_TRANSFORM_BUTTON]], activity );
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[RESAMPLE_BUTTON]], activity );
}

public  void  set_quit_button_activity(
    UI_struct         *ui_info,
    Boolean           activity )
{
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[QUIT_BUTTON]], activity );
}

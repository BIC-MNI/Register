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
    RECORD_TAG_BUTTON,
    DELETE_TAG_BUTTON,
    GOTO_END_TAGS_BUTTON,
    N_MAIN_WIDGETS
} Main_widgets;

private  int  widget_indices[N_MAIN_WIDGETS];

private  DEFINE_WIDGET_CALLBACK( quit_button_callback ) /* ARGSUSED */
{
    set_quit_program_flag();
}

private  DEFINE_WIDGET_CALLBACK( resample_and_load_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( sync_volumes_button_callback ) /* ARGSUSED */
{
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
}

private  DEFINE_WIDGET_CALLBACK( tags_filename_hit_return_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( save_tags_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( record_tag_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( delete_tag_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( goto_end_tags_button_callback ) /* ARGSUSED */
{
}

public  void  add_main_widgets(
    UI_struct         *ui_info )
{
    widget_indices[QUIT_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Quit",
                   ON, BUTTON_ACTIVE_COLOUR,
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
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   resample_and_load_button_callback, (void *) 0 ) );

    widget_indices[SYNC_VOLUMES_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Sync Volumes", 
                   OFF, BUTTON_ACTIVE_COLOUR,
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
                   OFF, BUTTON_ACTIVE_COLOUR,
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
                   ON, BUTTON_ACTIVE_COLOUR,
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
                   ON, BUTTON_ACTIVE_COLOUR,
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
                   ON, BUTTON_ACTIVE_COLOUR,
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
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   save_tags_button_callback, (void *) 0 ) );

    widget_indices[RECORD_TAG_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Record Tag",
                   OFF, BUTTON_ACTIVE_COLOUR,
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
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   delete_tag_button_callback, (void *) 0 ) );

    widget_indices[GOTO_END_TAGS_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Goto End Tags",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   goto_end_tags_button_callback, (void *) 0 ) );

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
        if( widget == TAGS_FILENAME_ENTRY )
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

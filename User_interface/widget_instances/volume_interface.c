#include  <def_user_interface.h>

typedef  enum
{
    RESET_VIEW_BUTTON,
    LOAD_BUTTON,
    LOAD_FILENAME_TEXT,
    VOXEL_LABEL,

    N_VOLUME_WIDGETS
}
Volume_widgets;

static  int  widget_indices[N_VOLUME_WIDGETS];
static  int  colour_bar_start_index;

public  DEFINE_WIDGET_CALLBACK( reset_view_callback ) /* ARGSUSED */
{
    int   volume_index, view_index;

    volume_index = get_viewport_volume_index(widget->viewport_index);

    for_less( view_index, 0, N_VIEWS )
        IF_reset_slice_view( volume_index, view_index );
}

private  DEFINE_WIDGET_CALLBACK( load_volume_callback ) /* ARGSUSED */
{
    char       *filename;
    int        viewport_index;

    viewport_index = widget->viewport_index;
    filename = get_text_entry_string( 
                     get_ui_struct()->widget_list[viewport_index].widgets
                                 [widget_indices[LOAD_FILENAME_TEXT]] );

    (void) initialize_loading_volume( get_ui_struct(),
                                      get_viewport_volume_index(viewport_index),
                                      filename, FALSE );
}

private  DEFINE_WIDGET_CALLBACK( volume_filename_callback ) /* ARGSUSED */
{
}

public  void  add_volume_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index )
{
    int            x, y, height, volume_index;

    volume_index = get_viewport_volume_index( viewport_index );

    x = Volume_menu_x_offset;
    y = Volume_menu_y_offset;

    colour_bar_start_index = add_colour_bar_widgets( ui_info, viewport_index,
                                                     x, y, &height );

    y += height + Interface_y_spacing;

    widget_indices[RESET_VIEW_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x, y, Volume_button_width, Volume_button_height,
                   "Reset View",
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   reset_view_callback, (void *) NULL ) );

    y += Volume_button_height + Interface_y_spacing;

    widget_indices[LOAD_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x, y, Volume_button_width, Volume_button_height,
                   "Load",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR, BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   load_volume_callback, (void *) NULL ) );

    widget_indices[LOAD_FILENAME_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_text_entry( &ui_info->graphics_window,
                       viewport_index, 
                       x + Volume_button_width + Interface_x_spacing, y,
                       Load_filename_width, Text_entry_height,
                       FALSE, "", ON,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       volume_filename_callback, (void *) NULL ) );

    ui_info->position_text_start_index[volume_index] =
                             add_cursor_position_widgets( ui_info,
                                   viewport_index, &height );
}

public  void  set_load_activity(
    UI_struct         *ui_info,
    int               volume_index,
    Boolean           state )
{
    Viewport_types   viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_widget_activity( ui_info->widget_list
                                 [viewport_index].widgets
                                 [widget_indices[LOAD_BUTTON]],
                         state );
    set_widget_activity( ui_info->widget_list
                                 [viewport_index].widgets
                                 [widget_indices[LOAD_FILENAME_TEXT]],
                         state );
}

public  void  set_load_filename(
    UI_struct         *ui_info,
    int               volume_index,
    char              filename[] )
{
    Viewport_types   viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_text_entry_string( ui_info->widget_list[viewport_index].widgets
                             [widget_indices[LOAD_FILENAME_TEXT]],
                           filename );
}

public  void  set_volume_widgets_activity(
    UI_struct         *ui_info,
    int               volume_index,
    Boolean           activity )
{
    Viewport_types       viewport_index;
    Volume_widgets       widget_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    for_enum( widget_index, N_VOLUME_WIDGETS, Volume_widgets )
    {
        set_widget_activity( ui_info->widget_list[viewport_index].widgets
                                            [widget_indices[widget_index]],
                             activity );
    }

    set_colour_bar_widgets_activity( ui_info, viewport_index,
                           colour_bar_start_index, activity );

    set_voxel_position_widgets_activity( ui_info, viewport_index,
                           ui_info->position_text_start_index[volume_index],
                           activity );
}

public  int  get_colour_bar_start_index()
{
    return( colour_bar_start_index );
}

#include  <def_user_interface.h>

static  int  avg_rms_index;

typedef enum
{
    RMS_ERROR,
    VOXEL_POSITION_LABEL,
    VOXEL_X_POSITION_TEXT1,
    VOXEL_Y_POSITION_TEXT1,
    VOXEL_Z_POSITION_TEXT1,
    WORLD_POSITION_LABEL1,
    WORLD_X_POSITION_TEXT1,
    WORLD_Y_POSITION_TEXT1,
    WORLD_Z_POSITION_TEXT1,
    VOXEL_X_POSITION_TEXT2,
    VOXEL_Y_POSITION_TEXT2,
    VOXEL_Z_POSITION_TEXT2,
    WORLD_POSITION_LABEL2,
    WORLD_X_POSITION_TEXT2,
    WORLD_Y_POSITION_TEXT2,
    WORLD_Z_POSITION_TEXT2,
    TAG_ACTIVITY_BUTTON,
    TAG_POINT_NAME,
    N_TAG_POINT_WIDGETS
} Tag_point_widgets;

static  int  first_rms_tag_index;
static  int  **widget_indices;

public  DEFINE_WIDGET_CALLBACK( reset_view_callback ) /* ARGSUSED */
{
    int   volume_index, view_index;

    volume_index = get_viewport_volume_index(widget->viewport_index);

    for_less( view_index, 0, N_DIMENSIONS )
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
                              filename );
}

private  DEFINE_WIDGET_CALLBACK( volume_filename_callback ) /* ARGSUSED */
{
}

public  void  add_tag_point_widgets(
    UI_struct         *ui_info,
    int               n_tag_points,
    Viewport_types    rms_viewport_index,
    Viewport_types    volume1_viewport_index,
    Viewport_types    volume2_viewport_index,
    Viewport_types    names_viewport_index )
{
    int            x, y, height;

    ALLOC2D( widget_indices, n_tag_points, N_TAG_POINT_WIDGETS );

    x = Interface_x_spacing;
    y = Interface_y_spacing;

    first_rms_tag_index = create_label( ui_info, viewport_index,
                  x, y, Avg_rms_label_width, Text_entry_height,
                  "V:", OFF, LABEL_ACTIVE_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  Label_text_font, Label_text_font_size );

    y += Volume_button_height + Interface_y_spacing;

    widget_indices[LOAD_BUTTON] = create_button( ui_info, viewport_index, 
                   x, y, Volume_button_width, Volume_button_height,
                   "Load",
                   ON, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   load_volume_callback );

    widget_indices[LOAD_FILENAME_TEXT] = create_text_entry( ui_info,
                       viewport_index, 
                       x + Volume_button_width + Interface_x_spacing, y,
                       Load_filename_width, Text_entry_height,
                       "",
                       ON,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       volume_filename_callback );

    y += Volume_button_height + Interface_y_spacing;

    ui_info->position_text_start_index[volume_index] =
                      add_cursor_position_widgets(
                               ui_info, viewport_index, x, y, &height );
}

public  void  set_load_activity(
    UI_struct         *ui_info,
    int               volume_index,
    Boolean           state )
{
    Viewport_types   viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_widget_activity_and_update( ui_info,
                         ui_info->widget_list
                                 [viewport_index].widgets
                                 [widget_indices[LOAD_BUTTON]],
                         state );
    set_widget_activity_and_update( ui_info,
                         ui_info->widget_list
                                 [viewport_index].widgets
                                 [widget_indices[LOAD_FILENAME_TEXT]],
                         state );

    set_viewport_update_flag( &ui_info->graphics_window.graphics,
                              viewport_index, NORMAL_PLANES );

}

public  void  set_load_filename(
    UI_struct         *ui_info,
    int               volume_index,
    char              filename[] )
{
    Viewport_types   viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_text_entry_string( ui_info,
                           ui_info->widget_list[viewport_index].widgets
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
        set_widget_activity_and_update( ui_info,
                             ui_info->widget_list[viewport_index].widgets
                                            [widget_indices[widget_index]],
                             activity );
    }

    set_viewport_update_flag( &ui_info->graphics_window.graphics,
                              viewport_index, NORMAL_PLANES );

    set_colour_bar_widgets_activity( ui_info, viewport_index,
                           ui_info->colour_bar_start_index[volume_index],
                           activity );

    set_voxel_position_widgets_activity( ui_info, viewport_index,
                           ui_info->position_text_start_index[volume_index],
                           activity );
}

#include  <def_user_interface.h>

typedef enum
{
    RMS_ERROR_BUTTON,
    RMS_ERROR_NUMBER,
    N_RMS_WIDGETS
} Rms_widgets;

typedef enum
{
    WORLD_POSITION_BUTTON,
    WORLD_X_POSITION_TEXT,
    WORLD_Y_POSITION_TEXT,
    WORLD_Z_POSITION_TEXT,
    N_POSITION_WIDGETS
} Position_widgets;

typedef enum
{
    TAG_NUMBER_BUTTON,
    TAG_ACTIVITY_BUTTON,
    TAG_POINT_NAME,
    N_TAG_NAME_WIDGETS
} Tag_name_widgets;

static  int  rms_label_index;
static  int  rms_error_index;
static  int  **rms_widget_indices;
static  int  ***position_widgets_indices;
static  int  **tag_name_widget_indices;

private  void   type_in_world_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            volume_index,
    int            axis );

private  DEFINE_WIDGET_CALLBACK( world_x_position1_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 0, X );
}

private  DEFINE_WIDGET_CALLBACK( world_y_position1_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 0, Y );
}

private  DEFINE_WIDGET_CALLBACK( world_z_position1_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 0, Z );
}

private  DEFINE_WIDGET_CALLBACK( world_x_position2_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 1, X );
}

private  DEFINE_WIDGET_CALLBACK( world_y_position2_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 1, Y );
}

private  DEFINE_WIDGET_CALLBACK( world_z_position2_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 1, Z );
}

private  DEFINE_WIDGET_CALLBACK( tag_number_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( tag_activity_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( world1_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( world2_button_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( rms_error_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( tag_name_callback ) /* ARGSUSED */
{
}

private  Viewport_types  get_tag_menu_viewport_index(
    int   volume_index )
{
    switch( volume_index )
    {
    case 0:   return( Volume_1_tags_viewport );
    case 1:   return( Volume_2_tags_viewport );
    default:  return( Volume_1_tags_viewport );
    }
}

public  void  set_tag_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               tag_index,
    int               axis_index,
    Real              value )
{
    String          string;
    Viewport_types  viewport_index;

    viewport_index = get_tag_menu_viewport_index( volume_index );

    (void) sprintf( string, Position_values_format, value );

    set_text_entry_string( ui_info->widget_list
                           [viewport_index].widgets
                           [position_widgets_indices[volume_index][tag_index]
                            [WORLD_X_POSITION_TEXT+axis_index]],
                           string );
}

private  void   type_in_world_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            volume_index,
    int            axis )
{
    int    start_widget_index;
    Real   value;
    Real   position[N_DIMENSIONS];

    volume_index = get_viewport_volume_index( widget->viewport_index );
    start_widget_index = ui_info->position_text_start_index[volume_index];

    if( get_text_entry_real_value( ui_info->widget_list
                                     [widget->viewport_index].widgets
                                     [start_widget_index+
                                      widget_indices[X_VOXEL_TEXT+axis]],
                                   &value ) )
    {
        (void) IF_get_tag_position( volume_index, position );
        position[axis] = value;
        IF_set_tag_position( volume_index, position );
    }
}

public  void  add_tag_point_widgets(
    UI_struct         *ui_info,
    int               n_tag_points,
    Viewport_types    rms_viewport_index,
    Viewport_types    volume1_viewport_index,
    Viewport_types    volume2_viewport_index,
    Viewport_types    names_viewport_index )
{
    int      tag, x, y, x_left, y_top, x_min, x_max, y_min, y_max;

    ALLOC2D( rms_widget_indices, n_tag_points, N_RMS_WIDGETS );
    ALLOC3D( position_widgets_indices, 2, n_tag_points, N_POSITION_WIDGETS );
    ALLOC2D( tag_name_widget_indices, n_tag_points, N_TAG_NAME_WIDGETS );

    get_graphics_viewport( &ui_info->graphics_window.graphics,
                           rms_viewport_index,
                           &x_min, &x_max, &y_min, &y_max );

    x_left = Interface_x_spacing;
    y_top = y_max - y_min - 1 - Interface_y_spacing;

    x = x_left;
    y = y_top;

    rms_label_index = add_widget_to_list(
                  &ui_info->widget_list[rms_viewport_index],
                  create_label( &ui_info->graphics_window, rms_viewport_index,
                  x, y, Avg_rms_label_width, Text_entry_height,
                  "Avg:", OFF, LABEL_ACTIVE_COLOUR, LABEL_SELECTED_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  Label_text_font, Label_text_font_size ) );

    rms_label_index = add_widget_to_list(
                  &ui_info->widget_list[rms_viewport_index],
                  create_label( &ui_info->graphics_window, rms_viewport_index,
                  x + Interface_x_spacing + Avg_rms_label_width, y,
                  Avg_rms_number_width, Text_entry_height,
                  "", OFF, LABEL_ACTIVE_COLOUR, LABEL_SELECTED_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  Label_text_font, Label_text_font_size ) );

    y -= Volume_button_height + Interface_y_spacing;

    for_less( tag, 0, n_tag_points )
    {
        x = x_left;

        rms_widget_indices[tag][RMS_ERROR_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[rms_viewport_index],
                   create_button( &ui_info->graphics_window, rms_viewport_index,
                   x, y, Rms_button_width, Tag_point_height,
                   "RMS:",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   rms_error_callback, (void *) NULL ) );

        rms_widget_indices[tag][RMS_ERROR_NUMBER] =
                   add_widget_to_list(
                   &ui_info->widget_list[rms_viewport_index],
                   create_label( &ui_info->graphics_window,
                   rms_viewport_index,
                   x + Interface_x_spacing + Rms_label_width, y,
                   Rms_number_width, Tag_point_height,
                   "", OFF, LABEL_ACTIVE_COLOUR, LABEL_SELECTED_COLOUR,
                   LABEL_INACTIVE_COLOUR,
                   LABEL_TEXT_COLOUR,
                   Label_text_font, Label_text_font_size ) );

        position_widgets_indices[0][tag][WORLD_POSITION_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume1_viewport_index],
                   create_button( &ui_info->graphics_window,
                   volume1_viewport_index,
                   x, y, Tag_world_button_width, Tag_point_height,
                   "World:",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   world1_button_callback, (void *) NULL ) );


        x += Tag_position_label_width + Position_values_separation;

        position_widgets_indices[0][tag][WORLD_X_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume1_viewport_index],
                   create_text_entry(&ui_info->graphics_window,
                       volume1_viewport_index, x, y,
                       Tag_position_width, Tag_point_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       world_x_position1_callback, (void *) NULL ) );

        x += Tag_position_width + Position_values_separation;

        position_widgets_indices[0][tag][WORLD_Y_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume1_viewport_index],
                   create_text_entry(&ui_info->graphics_window,
                       volume1_viewport_index, x, y,
                       Tag_position_width, Tag_point_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       world_y_position1_callback, (void *) NULL ) );

        x += Tag_position_width + Position_values_separation;

        position_widgets_indices[0][tag][WORLD_Z_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume1_viewport_index],
                   create_text_entry(&ui_info->graphics_window,
                       volume1_viewport_index, x, y,
                       Tag_position_width, Tag_point_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       world_z_position1_callback, (void *) NULL ) );

        x = x_left;
        position_widgets_indices[1][tag][WORLD_POSITION_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume2_viewport_index],
                   create_button( &ui_info->graphics_window,
                   volume2_viewport_index,
                   x, y, Tag_world_button_width, Tag_point_height,
                   "World:",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   world2_button_callback, (void *) NULL ) );



        x += Tag_position_label_width + Position_values_separation;

        position_widgets_indices[1][tag][WORLD_X_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume2_viewport_index],
                   create_text_entry(&ui_info->graphics_window,
                       volume2_viewport_index, x, y,
                       Tag_position_width, Tag_point_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       world_x_position2_callback, (void *) NULL ) );

        x += Tag_position_width + Position_values_separation;

        position_widgets_indices[1][tag][WORLD_Y_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume2_viewport_index],
                   create_text_entry(&ui_info->graphics_window,
                       volume2_viewport_index, x, y,
                       Tag_position_width, Tag_point_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       world_y_position2_callback, (void *) NULL ) );

        x += Tag_position_width + Position_values_separation;

        position_widgets_indices[1][tag][WORLD_Z_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume2_viewport_index],
                   create_text_entry(&ui_info->graphics_window,
                       volume2_viewport_index, x, y,
                       Tag_position_width, Tag_point_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       world_z_position2_callback, (void *) NULL ) );

        x = x_left;

        position_widgets_indices[1][tag][TAG_NUMBER_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Tag_number_button_width, Tag_point_height,
                   "",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   tag_number_button_callback, (void *) NULL ) );


        x += Tag_number_width + Interface_x_spacing;

        tag_name_widget_indices[tag][TAG_ACTIVITY_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_toggle_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Tag_activity_width, Tag_point_height,
                   "Ignore", "On",
                   ON,
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   tag_activity_callback, (void *) NULL ) );

        x += Tag_activity_width + Interface_x_spacing;

        tag_name_widget_indices[tag][TAG_POINT_NAME] =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_text_entry( &ui_info->graphics_window,
                       names_viewport_index, x, y,
                       Tag_name_width, Tag_point_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       tag_name_callback, (void *) NULL ) );

        y -= Tag_point_height + Interface_y_spacing;
    }
}

public  void  delete_tag_point_widgets_indices()
{
    FREE2D( rms_widget_indices );
    FREE3D( position_widgets_indices );
    FREE2D( tag_name_widget_indices );
}

public  void  set_tag_widgets_activity(
    UI_struct         *ui_info,
    Boolean           activity )
{
    int         widget_index, tag;

    set_widget_activity( ui_info->widget_list[RMS_error_viewport].widgets
                                         [rms_label_index], activity );
    set_widget_activity( ui_info->widget_list[RMS_error_viewport].widgets
                                         [rms_error_index], activity );

    for_less( tag, 0, ui_info->tag_points.n_tag_points )
    {
        for_enum( widget_index, N_RMS_WIDGETS, Rms_widgets )
        {
            set_widget_activity( 
                   ui_info->widget_list[RMS_error_viewport].widgets
                            [rms_widget_indices[tag][widget_index]], activity );
        }

        for_enum( widget_index, N_POSITION_WIDGETS, Position_widgets )
        {
            set_widget_activity(
                   ui_info->widget_list[Volume_1_tags_viewport].widgets
                   [position_widgets_indices[0][tag][widget_index]], activity );

            set_widget_activity(
                   ui_info->widget_list[Volume_2_tags_viewport].widgets
                   [position_widgets_indices[1][tag][widget_index]], activity );
        }

        for_enum( widget_index, N_TAG_NAME_WIDGETS, Tag_name_widgets )
        {
            set_widget_activity(
                   ui_info->widget_list[Tag_names_viewport].widgets
                            [rms_widget_indices[tag][widget_index]], activity );
        }
    }
}

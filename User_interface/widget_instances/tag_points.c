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
static  int  prev_tag_widget_index;
static  int  next_tag_widget_index;
static  int  end_tags_widget_index;

private  int  get_tag_index(
    UI_struct      *ui_info,
    int            tag );

private  void   type_in_world_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            tag,
    int            volume_index,
    int            axis );

private  DEFINE_WIDGET_CALLBACK( world_x_position1_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     (int) callback_data, 0, X );
}

private  DEFINE_WIDGET_CALLBACK( world_y_position1_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     (int) callback_data, 0, Y );
}

private  DEFINE_WIDGET_CALLBACK( world_z_position1_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     (int) callback_data, 0, Z );
}

private  DEFINE_WIDGET_CALLBACK( world_x_position2_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     (int) callback_data, 1, X );
}

private  DEFINE_WIDGET_CALLBACK( world_y_position2_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     (int) callback_data, 1, Y );
}

private  DEFINE_WIDGET_CALLBACK( world_z_position2_callback ) /* ARGSUSED */
{
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     (int) callback_data, 1, Z );
}

private  void  set_current_tag_from_button(
    void           *callback_data )
{
    set_current_tag_index( get_ui_struct(),
                           get_tag_index(get_ui_struct(), (int)callback_data) );
}

private  DEFINE_WIDGET_CALLBACK( tag_name_callback ) /* ARGSUSED */
{
    int   tag_index;
    char  *name;

    name = get_text_entry_string( widget );
    tag_index = get_tag_index( get_ui_struct(), (int) callback_data );

    IF_set_tag_point_name( tag_index, name );
    set_current_tag_index( get_ui_struct(), tag_index );
}

private  DEFINE_WIDGET_CALLBACK( tag_number_button_callback ) /* ARGSUSED */
{
    set_current_tag_from_button( callback_data );
}

private  DEFINE_WIDGET_CALLBACK( tag_activity_callback ) /* ARGSUSED */
{
}

private  DEFINE_WIDGET_CALLBACK( world1_button_callback ) /* ARGSUSED */
{
    set_current_tag_from_button( callback_data );
}

private  DEFINE_WIDGET_CALLBACK( world2_button_callback ) /* ARGSUSED */
{
    set_current_tag_from_button( callback_data );
}

private  DEFINE_WIDGET_CALLBACK( rms_error_callback ) /* ARGSUSED */
{
    set_current_tag_from_button( callback_data );
}

private  DEFINE_WIDGET_CALLBACK( prev_tag_button_callback ) /* ARGSUSED */
{
    int  tag_index;

    tag_index =  get_current_tag_index( get_ui_struct() );

    if( tag_index > 0 )
    {
        --tag_index;
        set_current_tag_index( get_ui_struct(), tag_index );
    }
}

private  DEFINE_WIDGET_CALLBACK( next_tag_button_callback ) /* ARGSUSED */
{
    int  tag_index;

    tag_index =  get_current_tag_index( get_ui_struct() );

    if( tag_index < IF_get_n_tag_points() )
    {
        ++tag_index;
        set_current_tag_index( get_ui_struct(), tag_index );
    }
}

private  DEFINE_WIDGET_CALLBACK( end_tags_button_callback ) /* ARGSUSED */
{
    int  tag_index;

    tag_index =  get_current_tag_index( get_ui_struct() );

    if( tag_index < IF_get_n_tag_points() )
    {
        set_current_tag_index( get_ui_struct(), IF_get_n_tag_points() );
    }
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
    Viewport_types  viewport_index;

    viewport_index = get_tag_menu_viewport_index( volume_index );

    set_text_entry_real_value( ui_info->widget_list
                           [viewport_index].widgets
                           [position_widgets_indices[volume_index][tag_index]
                            [WORLD_X_POSITION_TEXT+axis_index]],
                            Position_values_format, value );
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
                  "Avg:", ON, LABEL_ACTIVE_COLOUR, LABEL_SELECTED_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  Label_text_font, Label_text_font_size ) );

    rms_error_index = add_widget_to_list(
                  &ui_info->widget_list[rms_viewport_index],
                  create_label( &ui_info->graphics_window, rms_viewport_index,
                  x + Interface_x_spacing + Avg_rms_label_width, y,
                  Avg_rms_number_width, Text_entry_height,
                  "", ON, LABEL_ACTIVE_COLOUR, LABEL_SELECTED_COLOUR,
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
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   rms_error_callback, (void *) tag ) );

        rms_widget_indices[tag][RMS_ERROR_NUMBER] =
                   add_widget_to_list(
                   &ui_info->widget_list[rms_viewport_index],
                   create_label( &ui_info->graphics_window,
                   rms_viewport_index,
                   x + Interface_x_spacing + Rms_button_width, y,
                   Rms_number_width, Tag_point_height,
                   "", OFF, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
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
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   world1_button_callback, (void *) tag ) );


        x += Tag_world_button_width + Position_values_separation;

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
                       world_x_position1_callback, (void *) tag ) );

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
                       world_y_position1_callback, (void *) tag ) );

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
                       world_z_position1_callback, (void *) tag ) );

        x = x_left;
        position_widgets_indices[1][tag][WORLD_POSITION_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume2_viewport_index],
                   create_button( &ui_info->graphics_window,
                   volume2_viewport_index,
                   x, y, Tag_world_button_width, Tag_point_height,
                   "World:",
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   world2_button_callback, (void *) tag ) );



        x += Tag_world_button_width + Position_values_separation;

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
                       world_x_position2_callback, (void *) tag ) );

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
                       world_y_position2_callback, (void *) tag ) );

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
                       world_z_position2_callback, (void *) tag ) );

        x = x_left;

        tag_name_widget_indices[tag][TAG_NUMBER_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Tag_number_button_width, Tag_point_height,
                   "",
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   tag_number_button_callback, (void *) tag ) );

        x += Tag_number_button_width + Interface_x_spacing;

        tag_name_widget_indices[tag][TAG_ACTIVITY_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_toggle_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Tag_activity_width, Tag_point_height,
                   "Ignore", "On",
                   ON,
                   OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   tag_activity_callback, (void *) tag ) );

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
                       tag_name_callback, (void *) tag ) );

        x += Tag_name_width + Interface_x_spacing;

        if( tag == 0 )
        {
            prev_tag_widget_index =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Advance_tags_button_width, Tag_point_height,
                   "^",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   prev_tag_button_callback, (void *) NULL ) );
        }
        else if( tag == 1 )
        {
            next_tag_widget_index =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Advance_tags_button_width, Tag_point_height,
                   "v",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   next_tag_button_callback, (void *) NULL ) );
        }
        else if( tag == n_tag_points-1 )
        {
            end_tags_widget_index =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Advance_tags_button_width, Tag_point_height,
                   "End",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   end_tags_button_callback, (void *) NULL ) );
        }

        y -= Tag_point_height + Interface_y_spacing;
    }

    update_all_tag_widgets( ui_info );
}

public  void  delete_tag_point_widgets_indices()
{
    FREE2D( rms_widget_indices );
    FREE3D( position_widgets_indices );
    FREE2D( tag_name_widget_indices );
}

private  void  update_avg_rms_error(
    UI_struct         *ui_info )
{
    Real           avg_rms;
    widget_struct  *widget;

    widget = ui_info->widget_list[RMS_error_viewport].widgets[rms_error_index];

    if( IF_get_tag_point_avg_rms_error( &avg_rms ) )
    {
        set_text_entry_real_value( widget, Rms_error_format, avg_rms );
    }
    else
    {
        set_text_entry_string( widget, "" );
    }
}

private  void  update_rms_error(
    UI_struct         *ui_info,
    int               tag )
{
    Real           rms;
    widget_struct  *widget;

    widget = ui_info->widget_list[RMS_error_viewport].widgets
                   [rms_widget_indices[tag][RMS_ERROR_NUMBER]];

    if( get_tag_index(ui_info,tag) < IF_get_n_tag_points() &&
        IF_get_tag_point_rms_error( get_tag_index(ui_info,tag), &rms ) )
    {
        set_text_entry_real_value( widget, Rms_error_format, rms );
    }
    else
    {
        set_text_entry_string( widget, "" );
    }
}

private  void  update_tag_position(
    UI_struct         *ui_info,
    int               volume_index,
    int               tag )
{
    Boolean        exists;
    int            i;
    Real           position[N_DIMENSIONS];
    widget_struct  *widget;

    exists = get_tag_index(ui_info,tag) < IF_get_n_tag_points() &&
             IF_get_tag_point_position( get_tag_index(ui_info,tag),
                                        volume_index, position );

    for_less( i, 0, N_DIMENSIONS )
    {
        widget = ui_info->widget_list[Volume_1_tags_viewport+volume_index].
                 widgets[position_widgets_indices[volume_index][tag]
                        [WORLD_X_POSITION_TEXT+i]];

        if( exists )
        {
            set_text_entry_real_value( widget, Position_values_format,
                                       position[i] );
        }
        else
        {
            set_text_entry_string( widget, "" );
        }
    }
}

private  void  update_tag_name(
    UI_struct         *ui_info,
    int               tag )
{
    char           *name;
    int            tag_index;
    Boolean        exists;
    widget_struct  *widget;

    tag_index = get_tag_index(ui_info,tag);

    exists = tag_index < IF_get_n_tag_points();

    widget = ui_info->widget_list[Tag_names_viewport].widgets
                    [tag_name_widget_indices[tag][TAG_NUMBER_BUTTON]];

    if( exists )
    {
        set_button_int_value( widget, Tag_number_format, tag_index+1 );
    }
    else
    {
        set_button_text( widget, "" );
    }

    widget = ui_info->widget_list[Tag_names_viewport].widgets
                          [tag_name_widget_indices[tag][TAG_ACTIVITY_BUTTON]];

    set_widget_activity( widget, exists );

    if( exists )
        set_toggle_button_state( widget, IF_get_tag_point_activity(tag_index) );

    widget = ui_info->widget_list[Tag_names_viewport].widgets
                 [tag_name_widget_indices[tag][TAG_POINT_NAME]];

    if( exists )
        name = IF_get_tag_point_name( tag_index );
    else
        name = "";

    set_text_entry_string( widget, name );
}

private  void  set_widget_activity_and_selected(
    widget_struct  *widget,
    Boolean        activity,
    Boolean        selected )
{
    set_widget_activity( widget, activity );
    set_widget_selected( widget, selected );
}

public  void  update_tag_widgets(
    UI_struct         *ui_info,
    int               tag )
{
    int      widget_index, tag_index;
    Boolean  exists, selected;

    exists = (ui_info->tag_points.first_tag_displayed + tag <=
              IF_get_n_tag_points());

    tag_index = get_tag_index(ui_info,tag);
    selected = tag_index == ui_info->tag_points.current_tag_index;

    for_enum( widget_index, N_RMS_WIDGETS, Rms_widgets )
    {
        set_widget_activity_and_selected(
                ui_info->widget_list[RMS_error_viewport].widgets
                             [rms_widget_indices[tag][widget_index]], exists,
                 selected );
    }

    for_enum( widget_index, N_POSITION_WIDGETS, Position_widgets )
    {
        set_widget_activity_and_selected(
              ui_info->widget_list[Volume_1_tags_viewport].
              widgets[position_widgets_indices[0][tag][widget_index]], exists,
              selected );
        set_widget_activity_and_selected(
              ui_info->widget_list[Volume_2_tags_viewport].
              widgets[position_widgets_indices[1][tag][widget_index]], exists,
              selected );
    }

    for_enum( widget_index, N_TAG_NAME_WIDGETS, Tag_name_widgets )
    {
        set_widget_activity_and_selected(
              ui_info->widget_list[Tag_names_viewport].widgets
                     [tag_name_widget_indices[tag][widget_index]], exists,
              selected );
    }

    update_rms_error( ui_info, tag );
    update_tag_position( ui_info, 0, tag );
    update_tag_position( ui_info, 1, tag );
    update_tag_name( ui_info, tag );

    set_transform_buttons_activity( ui_info, IF_does_transform_exist() );
}

public  void  update_all_tag_widgets(
    UI_struct         *ui_info )
{
    int  tag;

    for_less( tag, 0, ui_info->tag_points.n_tags_displayed )
        update_tag_widgets( ui_info, tag );
    update_avg_rms_error( ui_info );
}

private  void   type_in_world_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            tag,
    int            volume_index,
    int            axis )
{
    int    tag_index;
    Real   value;
    Real   position[N_DIMENSIONS];

    if( get_text_entry_real_value( widget, &value ) )
    {
        tag_index = get_tag_index( ui_info, tag );

        (void) IF_get_tag_point_position( tag_index, volume_index, position );
        position[axis] = value;
        IF_set_tag_point_position( tag_index, volume_index, position );
        set_current_tag_index( ui_info, tag_index );
    }
}

public  void  set_current_tag_index(
    UI_struct      *ui_info,
    int            tag_index )
{
    ui_info->tag_points.current_tag_index = tag_index;

    while( tag_index < ui_info->tag_points.first_tag_displayed )
        --ui_info->tag_points.first_tag_displayed;

    while( tag_index >= ui_info->tag_points.first_tag_displayed +
                        ui_info->tag_points.n_tags_displayed )
        ++ui_info->tag_points.first_tag_displayed;

    update_all_tag_widgets( ui_info );
}

private  int  get_tag_index(
    UI_struct      *ui_info,
    int            tag )
{
    return( ui_info->tag_points.first_tag_displayed + tag );
}

public  int  get_current_tag(
    UI_struct      *ui_info )
{
    return( ui_info->tag_points.current_tag_index -
            ui_info->tag_points.first_tag_displayed );
}

public  int  get_current_tag_index(
    UI_struct      *ui_info )
{
    return( ui_info->tag_points.current_tag_index );
}

#include  <def_user_interface.h>

private  void    set_voxel_position_callback( UI_struct *, widget_struct *,
                                              int );
private  void    set_world_position_callback( UI_struct *, widget_struct *,
                                              int );

typedef  enum
{
    VOXEL_LABEL,
    X_VOXEL_TEXT,
    Y_VOXEL_TEXT,
    Z_VOXEL_TEXT,
    WORLD_LABEL,
    X_WORLD_TEXT,
    Y_WORLD_TEXT,
    Z_WORLD_TEXT,

    N_POSITION_WIDGETS
}
Position_widgets;

static  int  widget_indices[N_POSITION_WIDGETS];

private  DEFINE_WIDGET_CALLBACK( pos_x_voxel_callback ) /* ARGSUSED */
{
    set_voxel_position_callback( get_ui_struct(), widget, X );
}

private  DEFINE_WIDGET_CALLBACK( pos_y_voxel_callback ) /* ARGSUSED */
{
    set_voxel_position_callback( get_ui_struct(), widget, Y );
}

private  DEFINE_WIDGET_CALLBACK( pos_z_voxel_callback ) /* ARGSUSED */
{
    set_voxel_position_callback( get_ui_struct(), widget, Z );
}

private  DEFINE_WIDGET_CALLBACK( pos_x_world_callback ) /* ARGSUSED */
{
    set_world_position_callback( get_ui_struct(), widget, X );
}

private  DEFINE_WIDGET_CALLBACK( pos_y_world_callback ) /* ARGSUSED */
{
    set_world_position_callback( get_ui_struct(), widget, Y );
}

private  DEFINE_WIDGET_CALLBACK( pos_z_world_callback ) /* ARGSUSED */
{
    set_world_position_callback( get_ui_struct(), widget, Z );
}

public  int  add_cursor_position_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               x,
    int               y,
    int               *height )
{
    int    start_index, x_start, dx;

    start_index = create_label( ui_info, viewport_index, 
                  x, y, Position_label_width, Text_entry_height,
                  "V:", OFF, LABEL_ACTIVE_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  Label_text_font, Label_text_font_size );

    widget_indices[VOXEL_LABEL] = 0;

    x_start = x + Position_label_width + Position_values_separation;

    dx = Position_values_width + Position_values_separation;

    widget_indices[X_VOXEL_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       pos_x_voxel_callback ) - start_index;

    widget_indices[Y_VOXEL_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start + dx, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       pos_y_voxel_callback ) - start_index;

    widget_indices[Z_VOXEL_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start + 2 * dx, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       pos_z_voxel_callback ) - start_index;

    widget_indices[WORLD_LABEL] = create_label( ui_info, viewport_index, 
                  x_start + 3 * dx + Position_values_separation, y,
                  Position_label_width, Text_entry_height,
                  "W:", OFF, LABEL_ACTIVE_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  Label_text_font, Label_text_font_size ) - start_index;

    x_start = x_start + 3 * dx + Position_values_separation +
              Position_label_width + Position_values_separation;

    widget_indices[X_WORLD_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       pos_x_world_callback ) - start_index;

    widget_indices[Y_WORLD_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start + dx, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       pos_y_world_callback ) - start_index;

    widget_indices[Z_WORLD_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start + 2 * dx, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Text_entry_font, Text_entry_font_size,
                       pos_z_world_callback ) - start_index;

    *height = Text_entry_height;

    return( start_index );
}

public  void  set_voxel_position_widgets_activity(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               start_widget_index,
    Boolean           activity )
{
    Position_widgets       widget_index;

    for_enum( widget_index, N_POSITION_WIDGETS, Position_widgets )
    {
        set_widget_activity_and_update( ui_info,
                             ui_info->widget_list[viewport_index].widgets
                             [start_widget_index+widget_indices[widget_index]],
                             activity );
    }
}

public  void  set_volume_voxel_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    int               start_widget_index,
    Real              value )
{
    String          string;
    Viewport_types  viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    (void) sprintf( string, Position_values_format, value );

    set_text_entry_string( ui_info,
                           ui_info->widget_list
                           [viewport_index].widgets
                           [start_widget_index+
                            widget_indices[X_VOXEL_TEXT+view_index]],
                           string );
}

public  void  set_volume_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    int               start_widget_index,
    Real              value )
{
    String          string;
    Viewport_types  viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    (void) sprintf( string, Position_values_format, value );

    set_text_entry_string( ui_info,
                           ui_info->widget_list
                           [viewport_index].widgets
                           [start_widget_index+
                            widget_indices[X_WORLD_TEXT+view_index]],
                           string );
}

private  void   set_voxel_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            axis )
{
    int    volume_index;
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
        IF_get_volume_voxel_position( volume_index, position );
        position[axis] = value;
        IF_set_volume_voxel_position( volume_index, position );
    }

    update_position_counters( ui_info, volume_index );
}

private  void   set_world_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            axis )
{
    int    volume_index;
    Real   value;
    int    start_widget_index;
    Real   position[N_DIMENSIONS];

    volume_index = get_viewport_volume_index( widget->viewport_index );
    start_widget_index = ui_info->position_text_start_index[volume_index];

    volume_index = get_viewport_volume_index( widget->viewport_index );

    if( get_text_entry_real_value( ui_info->widget_list
                                     [widget->viewport_index].widgets
                                     [start_widget_index+
                                      widget_indices[X_WORLD_TEXT+axis]],
                                   &value ) )
    {
        IF_get_volume_world_position( volume_index, position );
        position[axis] = value;
        IF_set_volume_world_position( volume_index, position );
    }

    update_position_counters( ui_info, volume_index );
}

#include  <def_user_interface.h>

private  void    set_voxel_position_callback( UI_struct *, widget_struct *,
                                              int );
private  void    set_world_position_callback( UI_struct *, widget_struct *,
                                              int );
private  void   set_colour_coding_buttons_active( UI_struct *, int );

typedef  enum
{
    GRAY_SCALE_BUTTON,
    HOT_METAL_BUTTON,
    SPECTRAL_BUTTON,
    RED_BUTTON,
    GREEN_BUTTON,
    BLUE_BUTTON,
    RESET_VIEW_BUTTON,
    LOAD_BUTTON,
    LOAD_FILENAME_TEXT,
    VOXEL_LABEL,
    X_VOXEL_TEXT,
    Y_VOXEL_TEXT,
    Z_VOXEL_TEXT,
    WORLD_LABEL,
    X_WORLD_TEXT,
    Y_WORLD_TEXT,
    Z_WORLD_TEXT,

    N_VOLUME_WIDGETS
}
Volume_widgets;

static  int  widget_indices[N_VOLUME_WIDGETS];

private  int  get_colour_coding_widget_index(
    Colour_coding_types  type )
{
    switch( type )
    {
    case GRAY_SCALE:        return( GRAY_SCALE_BUTTON );
    case HOT_METAL:         return( HOT_METAL_BUTTON );
    case SPECTRAL:          return( SPECTRAL_BUTTON );
    case RED_COLOUR_MAP:    return( RED_BUTTON );
    case GREEN_COLOUR_MAP:  return( GREEN_BUTTON );
    case BLUE_COLOUR_MAP:   return( BLUE_BUTTON );
    default:                return( GRAY_SCALE_BUTTON );
    }
}

public  void  ui_set_volume_colour_coding_type(
    UI_struct            *ui,
    int                  volume,
    Colour_coding_types  type )
{
    IF_set_volume_colour_coding_type( volume, type );

    set_colour_coding_buttons_active( ui, volume );

    set_widget_activity( ui,
           ui->widget_list[Volume_1_menu_viewport + volume].widgets
                [widget_indices[get_colour_coding_widget_index(type)]], OFF );
}

private  void  set_colour_coding(
    widget_struct        *widget,
    Colour_coding_types  type )
{
    int   volume_index;

    volume_index = widget->viewport_index - Volume_1_menu_viewport;

    IF_set_volume_colour_coding_type( volume_index, type );

    set_colour_coding_buttons_active( get_ui_struct(), volume_index );

    set_widget_activity( get_ui_struct(), widget, OFF );
}

private  DEFINE_WIDGET_CALLBACK( gray_scale_callback ) /* ARGSUSED */
{
    set_colour_coding( widget, GRAY_SCALE );
}

private  DEFINE_WIDGET_CALLBACK( hot_metal_callback ) /* ARGSUSED */
{
    set_colour_coding( widget, HOT_METAL );
}

private  DEFINE_WIDGET_CALLBACK( spectral_callback ) /* ARGSUSED */
{
    set_colour_coding( widget, SPECTRAL );
}

private  DEFINE_WIDGET_CALLBACK( red_callback ) /* ARGSUSED */
{
    set_colour_coding( widget, RED_COLOUR_MAP );
}

private  DEFINE_WIDGET_CALLBACK( green_callback ) /* ARGSUSED */
{
    set_colour_coding( widget, GREEN_COLOUR_MAP );
}

private  DEFINE_WIDGET_CALLBACK( blue_callback ) /* ARGSUSED */
{
    set_colour_coding( widget, BLUE_COLOUR_MAP );
}

private  DEFINE_WIDGET_CALLBACK( reset_view_callback ) /* ARGSUSED */
{
    int   volume_index, view_index;

    volume_index = widget->viewport_index - Volume_1_menu_viewport;

    for_less( view_index, 0, N_DIMENSIONS )
        IF_reset_slice_view( volume_index, view_index );
}

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

private  DEFINE_WIDGET_CALLBACK( load_volume_callback ) /* ARGSUSED */
{
    char       *filename;
    int        viewport_index;

    viewport_index = widget->viewport_index;
    filename = get_text_entry_string( 
                     get_ui_struct()->widget_list[viewport_index].widgets
                                 [widget_indices[LOAD_FILENAME_TEXT]] );

    (void) initialize_loading_volume( get_ui_struct(),
                                      viewport_index - Volume_1_menu_viewport,
                                      filename );
}

private  DEFINE_WIDGET_CALLBACK( volume_filename_callback ) /* ARGSUSED */
{
}

public  void  add_volume_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index )
{
    int   x, x_start, y, dx;

    x = Volume_menu_x_offset;
    y = Volume_menu_y_offset;

    dx = Colour_bar_button_width + Colour_bar_button_spacing;

    widget_indices[GRAY_SCALE_BUTTON] = create_button( ui_info, viewport_index, 
                   x, y, Colour_bar_button_width, Volume_button_height,
                   "Gray",
                   OFF, get_ui_colour_index(BUTTON_ACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_INACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_TEXT_COLOUR),
                   Button_text_font, Button_text_font_size,
                   gray_scale_callback );

    widget_indices[HOT_METAL_BUTTON] = create_button( ui_info, viewport_index, 
                   x + dx, y, Colour_bar_button_width, Volume_button_height,
                   "Hot",
                   OFF, get_ui_colour_index(BUTTON_ACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_INACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_TEXT_COLOUR),
                   Button_text_font, Button_text_font_size,
                   hot_metal_callback );

    widget_indices[SPECTRAL_BUTTON] = create_button( ui_info, viewport_index, 
                   x + 2 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Spect",
                   OFF, get_ui_colour_index(BUTTON_ACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_INACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_TEXT_COLOUR),
                   Button_text_font, Button_text_font_size,
                   spectral_callback );

    widget_indices[RED_BUTTON] = create_button( ui_info, viewport_index, 
                   x + 3 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Red",
                   OFF, get_ui_colour_index(BUTTON_ACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_INACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_TEXT_COLOUR),
                   Button_text_font, Button_text_font_size,
                   red_callback );

    widget_indices[GREEN_BUTTON] = create_button( ui_info, viewport_index, 
                   x + 4 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Green",
                   OFF, get_ui_colour_index(BUTTON_ACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_INACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_TEXT_COLOUR),
                   Button_text_font, Button_text_font_size,
                   green_callback );

    widget_indices[BLUE_BUTTON] = create_button( ui_info, viewport_index, 
                   x + 5 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Blue",
                   OFF, get_ui_colour_index(BUTTON_ACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_INACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_TEXT_COLOUR),
                   Button_text_font, Button_text_font_size,
                   blue_callback );

    y += Volume_button_height + Interface_y_spacing;

    widget_indices[RESET_VIEW_BUTTON] = create_button( ui_info, viewport_index, 
                   x, y, Volume_button_width, Volume_button_height,
                   "Reset View",
                   OFF, get_ui_colour_index(BUTTON_ACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_INACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_TEXT_COLOUR),
                   Button_text_font, Button_text_font_size,
                   reset_view_callback );

    y += Volume_button_height + Interface_y_spacing;

    widget_indices[LOAD_BUTTON] = create_button( ui_info, viewport_index, 
                   x, y, Volume_button_width, Volume_button_height,
                   "Load",
                   ON, get_ui_colour_index(BUTTON_ACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_INACTIVE_COLOUR),
                   get_ui_colour_index(BUTTON_TEXT_COLOUR),
                   Button_text_font, Button_text_font_size,
                   load_volume_callback );

    widget_indices[LOAD_FILENAME_TEXT] = create_text_entry( ui_info,
                       viewport_index, 
                       x + Volume_button_width + Interface_x_spacing, y,
                       Load_filename_width, Text_entry_height,
                       "",
                       ON, get_ui_colour_index(TEXT_ENTRY_ACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_INACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_CURSOR_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_TEXT_COLOUR),
                       Text_entry_font, Text_entry_font_size,
                       volume_filename_callback );

    y += Volume_button_height + Interface_y_spacing;

    widget_indices[VOXEL_LABEL] = create_label( ui_info, viewport_index, 
                  x, y, Position_label_width, Text_entry_height,
                  "V:", OFF, get_ui_colour_index(LABEL_ACTIVE_COLOUR),
                  get_ui_colour_index(LABEL_INACTIVE_COLOUR),
                  get_ui_colour_index(LABEL_TEXT_COLOUR),
                  Label_text_font, Label_text_font_size );

    x_start = x + Position_label_width + Position_values_separation;

    dx = Position_values_width + Position_values_separation;

    widget_indices[X_VOXEL_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       get_ui_colour_index(TEXT_ENTRY_ACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_INACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_CURSOR_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_TEXT_COLOUR),
                       Text_entry_font, Text_entry_font_size,
                       pos_x_voxel_callback );

    widget_indices[Y_VOXEL_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start + dx, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       get_ui_colour_index(TEXT_ENTRY_ACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_INACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_CURSOR_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_TEXT_COLOUR),
                       Text_entry_font, Text_entry_font_size,
                       pos_y_voxel_callback );

    widget_indices[Z_VOXEL_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start + 2 * dx, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       get_ui_colour_index(TEXT_ENTRY_ACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_INACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_CURSOR_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_TEXT_COLOUR),
                       Text_entry_font, Text_entry_font_size,
                       pos_z_voxel_callback );

    widget_indices[WORLD_LABEL] = create_label( ui_info, viewport_index, 
                  x_start + 3 * dx + Position_values_separation, y,
                  Position_label_width, Text_entry_height,
                  "W:", OFF, get_ui_colour_index(LABEL_ACTIVE_COLOUR),
                  get_ui_colour_index(LABEL_INACTIVE_COLOUR),
                  get_ui_colour_index(LABEL_TEXT_COLOUR),
                  Label_text_font, Label_text_font_size );

    x_start = x_start + 3 * dx + Position_values_separation +
              Position_label_width + Position_values_separation;

    widget_indices[X_WORLD_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       get_ui_colour_index(TEXT_ENTRY_ACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_INACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_CURSOR_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_TEXT_COLOUR),
                       Text_entry_font, Text_entry_font_size,
                       pos_x_world_callback );

    widget_indices[Y_WORLD_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start + dx, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       get_ui_colour_index(TEXT_ENTRY_ACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_INACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_CURSOR_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_TEXT_COLOUR),
                       Text_entry_font, Text_entry_font_size,
                       pos_y_world_callback );

    widget_indices[Z_WORLD_TEXT] = create_text_entry( ui_info, viewport_index,
                       x_start + 2 * dx, y,
                       Position_values_width, Text_entry_height,
                       "", OFF,
                       get_ui_colour_index(TEXT_ENTRY_ACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_INACTIVE_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_CURSOR_COLOUR),
                       get_ui_colour_index(TEXT_ENTRY_TEXT_COLOUR),
                       Text_entry_font, Text_entry_font_size,
                       pos_z_world_callback );
}

public  void  set_load_activity(
    UI_struct         *ui_info,
    int               volume_index,
    Boolean           state )
{
    set_widget_activity( ui_info,
                         ui_info->widget_list
                                 [Volume_1_menu_viewport + volume_index].widgets
                                 [widget_indices[LOAD_BUTTON]],
                         state );
    set_widget_activity( ui_info,
                         ui_info->widget_list
                                 [Volume_1_menu_viewport + volume_index].widgets
                                 [widget_indices[LOAD_FILENAME_TEXT]],
                         state );
}

public  void  set_load_filename(
    UI_struct         *ui_info,
    int               volume_index,
    char              filename[] )
{
    set_text_entry_string( ui_info,
                           ui_info->widget_list
                           [Volume_1_menu_viewport + volume_index].widgets
                           [widget_indices[LOAD_FILENAME_TEXT]],
                           filename );
}

public  void  set_volume_widgets_activity(
    UI_struct         *ui_info,
    int               volume_index,
    Boolean           activity )
{
    Volume_widgets  widget_index;

    for_enum( widget_index, N_VOLUME_WIDGETS, Volume_widgets )
    {
        set_widget_activity( ui_info,
                         ui_info->widget_list
                                 [Volume_1_menu_viewport + volume_index].widgets
                                 [widget_indices[widget_index]],
                         activity );
    }
}

public  void  set_volume_voxel_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    Real              value )
{
    String   string;

    (void) sprintf( string, Position_values_format, value );

    set_text_entry_string( ui_info,
                           ui_info->widget_list
                           [Volume_1_menu_viewport + volume_index].widgets
                           [widget_indices[X_VOXEL_TEXT+view_index]],
                           string );
}

public  void  set_volume_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    Real              value )
{
    String   string;

    (void) sprintf( string, Position_values_format, value );

    set_text_entry_string( ui_info,
                           ui_info->widget_list
                           [Volume_1_menu_viewport + volume_index].widgets
                           [widget_indices[X_WORLD_TEXT+view_index]],
                           string );
}

private  void   set_voxel_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            axis )
{
    int    volume_index;
    Real   value;
    Real   position[N_DIMENSIONS];

    volume_index = widget->viewport_index - Volume_1_menu_viewport;

    if( get_text_entry_real_value( ui_info->widget_list
                                     [widget->viewport_index].widgets
                                     [widget_indices[X_VOXEL_TEXT+axis]],
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
    Real   position[N_DIMENSIONS];

    volume_index = widget->viewport_index - Volume_1_menu_viewport;

    if( get_text_entry_real_value( ui_info->widget_list
                                     [widget->viewport_index].widgets
                                     [widget_indices[X_WORLD_TEXT+axis]],
                                   &value ) )
    {
        IF_get_volume_world_position( volume_index, position );
        position[axis] = value;
        IF_set_volume_world_position( volume_index, position );
    }

    update_position_counters( ui_info, volume_index );
}

private  void   set_colour_coding_buttons_active(
    UI_struct      *ui_info,
    int            volume_index )
{
    int    i, viewport_index;

    viewport_index = Volume_1_menu_viewport + volume_index;

    for_inclusive( i, (int) GRAY_SCALE_BUTTON, (int) BLUE_BUTTON )
    {
        set_widget_activity( ui_info,
                             ui_info->widget_list
                                     [viewport_index].widgets
                                     [widget_indices[i]], ON );
    }
}

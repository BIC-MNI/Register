#include  <def_user_interface.h>

private  void   set_colour_coding_buttons_active( UI_struct *, int,
                                                  Colour_coding_types );

typedef  enum
{
    GRAY_SCALE_BUTTON,
    HOT_METAL_BUTTON,
    SPECTRAL_BUTTON,
    RED_BUTTON,
    GREEN_BUTTON,
    BLUE_BUTTON,
    COLOUR_BAR_SLIDER,

    N_COLOUR_BAR_WIDGETS
}
Colour_bar_widgets;

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

static  int  widget_indices[N_COLOUR_BAR_WIDGETS];

private  void  set_colour_coding(
    widget_struct        *widget,
    Colour_coding_types  type )
{
    int   volume_index;

    volume_index = get_viewport_volume_index(widget->viewport_index);

    IF_set_volume_colour_coding_type( volume_index, type );
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

private  void  change_limits(
    widget_struct  *widget )
{
    Real   min_val, max_val;
    int    volume_index;

    volume_index = get_viewport_volume_index( widget->viewport_index );

    get_slider_values( widget, &min_val, &max_val );

    IF_set_colour_coding_limits( volume_index, min_val, max_val );
}

private  DEFINE_WIDGET_CALLBACK( lower_limit_callback ) /* ARGSUSED */
{
    change_limits( widget );
}

private  DEFINE_WIDGET_CALLBACK( upper_limit_callback ) /* ARGSUSED */
{
    change_limits( widget );
}

public  int  add_colour_bar_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               x,
    int               y,
    int               *height )
{
    int              start_index, dx;
    widget_struct    *radio_widgets[6];

    dx = Colour_bar_button_width + Colour_bar_button_spacing;

    start_index = create_button( ui_info, viewport_index, 
                   x, y, Colour_bar_button_width, Volume_button_height,
                   "Gray",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   gray_scale_callback );

    widget_indices[GRAY_SCALE_BUTTON] = 0;

    widget_indices[HOT_METAL_BUTTON] = create_button( ui_info, viewport_index, 
                   x + dx, y, Colour_bar_button_width, Volume_button_height,
                   "Hot",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   hot_metal_callback ) - start_index;

    widget_indices[SPECTRAL_BUTTON] = create_button( ui_info, viewport_index, 
                   x + 2 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Spect",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   spectral_callback ) - start_index;

    widget_indices[RED_BUTTON] = create_button( ui_info, viewport_index, 
                   x + 3 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Red",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   red_callback ) - start_index;

    widget_indices[GREEN_BUTTON] = create_button( ui_info, viewport_index, 
                   x + 4 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Green",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   green_callback ) - start_index;

    widget_indices[BLUE_BUTTON] = create_button( ui_info, viewport_index, 
                   x + 5 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Blue",
                   OFF, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   blue_callback ) - start_index;

    y += Volume_button_height + Interface_y_spacing;

    widget_indices[COLOUR_BAR_SLIDER] = create_colour_bar_slider( ui_info,
                   viewport_index, x, y, Colour_bar_slider_width,
                   Colour_bar_slider_height,
                   0.0, 255.0, 0.0, 255.0,
                   OFF,
                   SLIDER_ACTIVE_COLOUR, SLIDER_INACTIVE_COLOUR,
                   SLIDER_PEG_COLOUR,
                   lower_limit_callback,
                   upper_limit_callback ) - start_index;

    *height = y + Colour_bar_slider_height;

    radio_widgets[0] = ui_info->widget_list[viewport_index].widgets
                               [widget_indices[start_index+GRAY_SCALE_BUTTON]];
    radio_widgets[1] = ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[start_index+HOT_METAL_BUTTON]];
    radio_widgets[2] = ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[start_index+SPECTRAL_BUTTON]];
    radio_widgets[3] = ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[start_index+RED_BUTTON]];
    radio_widgets[4] = ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[start_index+GREEN_BUTTON]];
    radio_widgets[5] = ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[start_index+BLUE_BUTTON]];

    define_radio_buttons( 6, radio_widgets );

    return( start_index );
}

public  void  set_colour_bar_widgets_activity(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               start_widget_index,
    Boolean           activity )
{
    Colour_coding_types  type;
    Colour_bar_widgets   widget_index;

    for_enum( widget_index, N_COLOUR_BAR_WIDGETS, Colour_bar_widgets )
    {
        set_widget_activity_and_update( ui_info,
                             ui_info->widget_list[viewport_index].widgets
                                   [start_widget_index +
                                    widget_indices[widget_index]],
                             activity );
    }

    if( activity )
    {
        type = IF_get_colour_coding_type(
                          get_viewport_volume_index(viewport_index) );
        set_widget_activity_and_update( ui_info,
                     ui_info->widget_list[viewport_index].widgets
                     [start_widget_index + widget_indices[
                           get_colour_coding_widget_index(type)]],
                     OFF );
    }
}

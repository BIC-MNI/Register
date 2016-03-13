/* ----------------------------------------------------------------------------
@COPYRIGHT  :
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

#include  <user_interface.h>

typedef  enum
{
    GRAY_SCALE_BUTTON,
    HOT_METAL_BUTTON,
    SPECTRAL_BUTTON,
    RED_BUTTON,
    GREEN_BUTTON,
    BLUE_BUTTON,
    UNDER_BUTTON,
    OVER_BUTTON,
    COLOUR_BAR_SLIDER,

    N_COLOUR_BAR_WIDGETS
}
Colour_bar_widgets;

static  int  get_colour_coding_widget_index(
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

static  void  set_colour_coding(
    widget_struct        *widget,
    Colour_coding_types  type )
{
    int   volume_index;

    volume_index = get_viewport_volume_index(widget->viewport_index);

    IF_set_volume_colour_coding_type( volume_index, type );

    IF_set_volume_colour_coding_type( MERGED_VOLUME_INDEX + volume_index,
                                      type );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( gray_scale_callback )
{
    set_colour_coding( widget, GRAY_SCALE );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( hot_metal_callback )
{
    set_colour_coding( widget, HOT_METAL );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( spectral_callback )
{
    set_colour_coding( widget, SPECTRAL );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( red_callback )
{
    set_colour_coding( widget, RED_COLOUR_MAP );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( green_callback )
{
    set_colour_coding( widget, GREEN_COLOUR_MAP );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( blue_callback )
{
    set_colour_coding( widget, BLUE_COLOUR_MAP );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( under_button_callback )
{
    int   volume_index;

    volume_index = get_viewport_volume_index(widget->viewport_index);

    set_widget_activity( widget, FALSE );
    popup_colour_selection( get_ui_struct(), volume_index, 0 );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( over_button_callback )
{
    int   volume_index;

    volume_index = get_viewport_volume_index(widget->viewport_index);

    set_widget_activity( widget, FALSE );
    popup_colour_selection( get_ui_struct(), volume_index, 1 );
}

static  void  change_limits(
    widget_struct  *widget )
{
    VIO_Real   min_val, max_val;
    int    volume_index;

    volume_index = get_viewport_volume_index( widget->viewport_index );

    get_slider_values( widget, &min_val, &max_val );

    IF_set_colour_coding_limits( volume_index, min_val, max_val );
    IF_set_colour_coding_limits( MERGED_VOLUME_INDEX + volume_index,
                                 min_val, max_val );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( lower_limit_callback )
{
    change_limits( widget );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( upper_limit_callback )
{
    change_limits( widget );
}

  int  add_colour_bar_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               x,
    int               y,
    int               *height )
{
    int              start_index, dx, volume;
    widget_struct    *radio_widgets[6];

    volume = get_viewport_volume_index( viewport_index );

    dx = Colour_bar_button_width + Colour_bar_button_spacing;

    start_index = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x, y, Colour_bar_button_width, Volume_button_height,
                   "Gray",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   gray_scale_callback, (void *) NULL ) );

    widget_indices[GRAY_SCALE_BUTTON] = 0;

    widget_indices[HOT_METAL_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x + dx, y, Colour_bar_button_width, Volume_button_height,
                   "Hot",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   hot_metal_callback, (void *) NULL ) ) - start_index;

    widget_indices[SPECTRAL_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x + 2 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Spect",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   spectral_callback, (void *) NULL ) ) - start_index;

    widget_indices[RED_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x + 3 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Red",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   red_callback, (void *) NULL ) ) - start_index;

    widget_indices[GREEN_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x + 4 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Green",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   green_callback, (void *) NULL ) ) - start_index;

    widget_indices[BLUE_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x + 5 * dx, y, Colour_bar_button_width, Volume_button_height,
                   "Blue",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   blue_callback, (void *) NULL ) ) - start_index;

    y += Volume_button_height + Interface_y_spacing;

    widget_indices[UNDER_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x, y, Colour_bar_button_width, Volume_button_height,
                   "Under",
                   FALSE, TRUE,
                   (VIO_Colour) (VOLUME1_UNDER_COLOUR + volume),
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   under_button_callback, (void *) NULL ) ) - start_index;

    x += Colour_bar_button_width + Interface_x_spacing;

    widget_indices[COLOUR_BAR_SLIDER] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_colour_bar_slider( &ui_info->graphics_window,
                   viewport_index, x, y, Colour_bar_slider_width,
                   Colour_bar_slider_height,
                   0.0, 255.0, 0.0, 255.0, Colour_bar_text_format,
                   FALSE,
                   SLIDER_ACTIVE_COLOUR, SLIDER_INACTIVE_COLOUR,
                   SLIDER_PEG_COLOUR,
                   lower_limit_callback, (void *) NULL,
                   upper_limit_callback, (void *) NULL ) ) - start_index;

    x += Colour_bar_slider_width + Interface_x_spacing;

    widget_indices[OVER_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x, y, Colour_bar_button_width, Volume_button_height,
                   "Over",
                   FALSE, TRUE,
                   (VIO_Colour) (VOLUME1_OVER_COLOUR + volume),
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   over_button_callback, (void *) NULL ) ) - start_index;

    *height = y + Colour_bar_slider_height;

    radio_widgets[0] = ui_info->widget_list[viewport_index].widgets
                               [start_index+widget_indices[GRAY_SCALE_BUTTON]];
    radio_widgets[1] = ui_info->widget_list[viewport_index].widgets
                                 [start_index+widget_indices[HOT_METAL_BUTTON]];
    radio_widgets[2] = ui_info->widget_list[viewport_index].widgets
                                 [start_index+widget_indices[SPECTRAL_BUTTON]];
    radio_widgets[3] = ui_info->widget_list[viewport_index].widgets
                                 [start_index+widget_indices[RED_BUTTON]];
    radio_widgets[4] = ui_info->widget_list[viewport_index].widgets
                                 [start_index+widget_indices[GREEN_BUTTON]];
    radio_widgets[5] = ui_info->widget_list[viewport_index].widgets
                                 [start_index+widget_indices[BLUE_BUTTON]];

    define_radio_buttons( 6, radio_widgets );

    return( start_index );
}

  void  set_colour_bar_widgets_activity(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               start_widget_index,
    VIO_BOOL           activity )
{
    Colour_coding_types  type;
    Colour_bar_widgets   widget_index;

    for_enum( widget_index, N_COLOUR_BAR_WIDGETS, Colour_bar_widgets )
    {
        set_widget_activity( ui_info->widget_list[viewport_index].widgets
                                   [start_widget_index +
                                    widget_indices[widget_index]],
                             activity );
    }

    if( activity )
    {
        type = IF_get_colour_coding_type(
                          get_viewport_volume_index(viewport_index) );
        set_widget_selected(
                     ui_info->widget_list[viewport_index].widgets
                     [start_widget_index + widget_indices[
                           get_colour_coding_widget_index(type)]], TRUE );
    }
}

  void  set_over_under_colour_activity(
    UI_struct         *ui_info,
    int               volume_index,
    int               over_or_under,
    VIO_BOOL           activity )
{
    Viewport_types      viewport;
    Colour_bar_widgets  widget_index;

    viewport = get_volume_menu_viewport_index( volume_index );

    if( over_or_under == 0 )
        widget_index = UNDER_BUTTON;
    else
        widget_index = OVER_BUTTON;

    set_widget_activity( ui_info->widget_list[viewport].widgets
                           [get_colour_bar_start_index() +
                            widget_indices[widget_index]],
                             activity );
}

  void  set_over_under_colour(
    UI_struct         *ui_info,
    int               volume_index,
    int               over_or_under,
    VIO_Colour            colour )
{
    Viewport_types      viewport;
    Colour_bar_widgets  widget_index;
    UI_colours          colour_name;

    viewport = get_volume_menu_viewport_index( volume_index );

    if( over_or_under == 0 )
    {
        widget_index = UNDER_BUTTON;
        IF_set_under_colour( volume_index, colour );
        IF_set_under_colour( MERGED_VOLUME_INDEX + volume_index, colour );
    }
    else
    {
        widget_index = OVER_BUTTON;
        IF_set_over_colour( volume_index, colour );
        IF_set_over_colour( MERGED_VOLUME_INDEX + volume_index, colour );
    }

    if (over_or_under == 0)
      colour_name = VOLUME1_UNDER_COLOUR + volume_index;
    else
      colour_name = VOLUME1_OVER_COLOUR + volume_index;
 
    set_ui_colour( ui_info, colour_name, colour );

    update_widget_colours( ui_info->widget_list[viewport].widgets
                                 [get_colour_bar_start_index() +
                                  widget_indices[widget_index]] );
}

  void  set_colour_bar_limits(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_Real              min_value,
    VIO_Real              max_value )
{
    Viewport_types      viewport;

    viewport = get_volume_menu_viewport_index( volume_index );

    set_slider_limits( ui_info->widget_list[viewport].widgets
                                 [get_colour_bar_start_index() +
                                  widget_indices[COLOUR_BAR_SLIDER]],
                       min_value, max_value );
}

  void  set_colour_bar_values(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_Real              min_value,
    VIO_Real              max_value )
{
    Viewport_types      viewport;

    viewport = get_volume_menu_viewport_index( volume_index );

    set_slider_values( ui_info->widget_list[viewport].widgets
                                 [get_colour_bar_start_index() +
                                  widget_indices[COLOUR_BAR_SLIDER]],
                       min_value, max_value );
}

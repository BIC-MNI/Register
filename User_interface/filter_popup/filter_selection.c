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

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/filter_popup/filter_selection.c,v 1.6 1996-12-09 20:22:07 david Exp $";
#endif

#include  <user_interface.h>

typedef struct
{
    int      volume;
    int      view;
} callback_struct;

typedef  struct
{
    int                     volume;
    popup_struct            popup_window;

    callback_struct         callback_data[N_VIEWS];
} filter_selection_struct;

typedef  enum
{
    NEAREST_BUTTON,
    LINEAR_BUTTON,
    BOX_BUTTON,
    TRIANGLE_BUTTON,
    GAUSSIAN_BUTTON,
    FILTER_WIDTH_LABEL,
    FILTER_WIDTH_TEXT,

    N_FILTER_WIDGETS
}
Filter_widgets;

static   int  widget_indices[N_FILTER_WIDGETS];

private  int  get_filter_widget_index(
    Filter_types  type )
{
    switch( type )
    {
    case NEAREST_NEIGHBOUR:        return( NEAREST_BUTTON );
    case LINEAR_INTERPOLATION:     return( LINEAR_BUTTON );
    case BOX_FILTER:               return( BOX_BUTTON );
    case TRIANGLE_FILTER:          return( TRIANGLE_BUTTON );
    case GAUSSIAN_FILTER:          return( GAUSSIAN_BUTTON );
    default:                       return( NEAREST_BUTTON );
    }
}

private  void  set_filter(
    void            *callback_data,
    Filter_types    type )
{
    callback_struct  *callback;

    callback = (callback_struct *) callback_data;

    IF_set_slice_filter_type( callback->volume, callback->view, type );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( nearest_callback )
{
    set_filter( callback_data, NEAREST_NEIGHBOUR );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( linear_callback )
{
    set_filter( callback_data, LINEAR_INTERPOLATION );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( box_callback )
{
    set_filter( callback_data, BOX_FILTER );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( triangle_callback )
{
    set_filter( callback_data, TRIANGLE_FILTER );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( gaussian_callback )
{
    set_filter( callback_data, GAUSSIAN_FILTER );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( filter_width_callback )
{
    Real             value;
    callback_struct  *callback;

    if( get_text_entry_real_value( widget, &value ) && value >= 0.0 )
    {
        callback = (callback_struct *) callback_data;
        
        IF_set_slice_filter_width( callback->volume, callback->view, value );
    }
    else
        restore_text_entry_string( widget );
}

private  int  add_filter_widgets(
    popup_struct      *popup,
    int               x,
    int               y,
    callback_struct   *callback,
    int               *height )
{
    int              start_index, y_start, dx;
    widget_struct    *radio_widgets[5];

    y_start = y;
    dx = Filter_button_width + Interface_x_spacing;

    start_index = add_widget_to_list(
                   &popup->widgets,
                   create_button( &popup->graphics, 0, 
                   x, y, Filter_button_width, Filter_button_height,
                   "Nearest",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   nearest_callback, (void *) callback ) );

    widget_indices[NEAREST_BUTTON] = 0;

    widget_indices[LINEAR_BUTTON] = add_widget_to_list(
                   &popup->widgets,
                   create_button( &popup->graphics, 0, 
                   x + dx, y, Filter_button_width, Filter_button_height,
                   "Linear",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   linear_callback, (void *) callback ) ) - start_index;

    widget_indices[BOX_BUTTON] = add_widget_to_list(
                   &popup->widgets,
                   create_button( &popup->graphics, 0, 
                   x + 2 * dx, y, Filter_button_width, Filter_button_height,
                   "Box",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   box_callback, (void *) callback ) ) - start_index;

    widget_indices[TRIANGLE_BUTTON] = add_widget_to_list(
                   &popup->widgets,
                   create_button( &popup->graphics, 0, 
                   x + 3 * dx, y, Filter_button_width, Filter_button_height,
                   "Triangle",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   triangle_callback, (void *) callback ) ) - start_index;

    widget_indices[GAUSSIAN_BUTTON] = add_widget_to_list(
                   &popup->widgets,
                   create_button( &popup->graphics, 0, 
                   x + 4 * dx, y, Filter_button_width, Filter_button_height,
                   "Gaussian",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   gaussian_callback, (void *) callback ) ) - start_index;

    y -= Filter_button_height + Interface_y_spacing;

    widget_indices[FILTER_WIDTH_LABEL] = add_widget_to_list(
                   &popup->widgets,
                   create_label( &popup->graphics, 0,
                   x, y, Full_width_label_width, Filter_button_height,
                   "Full Width Half Max", ON, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR, LABEL_INACTIVE_COLOUR,
                   LABEL_TEXT_COLOUR, (Font_types) Label_text_font,
                   Label_text_font_size ) )
                      - start_index;

    widget_indices[FILTER_WIDTH_TEXT] = add_widget_to_list(
                   &popup->widgets,
                   create_text_entry( &popup->graphics, 0,
                   x + Full_width_label_width + Interface_x_spacing, y,
                   Full_width_text_width, Filter_button_height,
                   FALSE, "", ON,
                   TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                   TEXT_ENTRY_INACTIVE_COLOUR, TEXT_ENTRY_TEXT_COLOUR,
                   TEXT_ENTRY_EDIT_COLOUR, TEXT_ENTRY_EDIT_TEXT_COLOUR,
                   TEXT_ENTRY_CURSOR_COLOUR,
                   (Font_types) Text_entry_font, Text_entry_font_size,
                   filter_width_callback, (void *) callback ) ) - start_index;

    *height = y_start - y;

    radio_widgets[0] = popup->widgets.widgets
                               [start_index+widget_indices[NEAREST_BUTTON]];
    radio_widgets[1] = popup->widgets.widgets
                                 [start_index+widget_indices[LINEAR_BUTTON]];
    radio_widgets[2] = popup->widgets.widgets
                                 [start_index+widget_indices[BOX_BUTTON]];
    radio_widgets[3] = popup->widgets.widgets
                                 [start_index+widget_indices[TRIANGLE_BUTTON]];
    radio_widgets[4] = popup->widgets.widgets
                                 [start_index+widget_indices[GAUSSIAN_BUTTON]];

    define_radio_buttons( 5, radio_widgets );

    return( start_index );
}

/* ------------------------------------------------------------------------ */

private  void  delete_filter_selection(
    filter_selection_struct          *popup )
{
    delete_popup_window( &popup->popup_window );

    set_filter_popup_activity( get_ui_struct(), popup->volume, ON );

    FREE( popup );
}

/* ARGSUSED */

private  DEFINE_WIDGET_CALLBACK( close_callback )
{
    delete_filter_selection( (filter_selection_struct *) callback_data );
}

/* ARGSUSED */

private  DEFINE_EVENT_FUNCTION( quit_window_callback )
{
    delete_filter_selection( (filter_selection_struct *) callback_data );
}

public  void  popup_filter_selection(
    UI_struct   *ui,
    int         volume )
{
    int                       x, y, view_index;
    filter_selection_struct   *popup;
    int                       view_start_index, height;
    Filter_types              filter_type;
    Real                      filter_width;
    char                      window_name[EXTREMELY_LARGE_STRING_SIZE];
    static STRING             view_names[3];

    view_names[0] = Filter_name1;
    view_names[1] = Filter_name2;
    view_names[2] = Filter_name3;

    set_filter_popup_activity( ui, volume, OFF );

    ALLOC( popup, 1 );

    popup->volume = volume;

    G_get_mouse_screen_position( &x, &y );

    (void) sprintf( window_name, "Volume %d Filter Selection", volume + 1 );

    create_popup_window( &popup->popup_window, window_name, x, y,
                         Filter_selection_x_size, Filter_selection_y_size,
                         quit_window_callback, (void *) popup );

    initialize_widget_list( &popup->popup_window.widgets );

    x = Interface_x_spacing;
    y = Filter_selection_y_size - 1 - Interface_y_spacing -
        Filter_button_height;

    for_less( view_index, 0, N_VIEWS )
    {
        (void) add_widget_to_list( &popup->popup_window.widgets,
                   create_label( &popup->popup_window.graphics, 0,
                   x, y, Filter_view_label_width, Filter_button_height,
                   view_names[view_index], ON, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR, LABEL_INACTIVE_COLOUR,
                   LABEL_TEXT_COLOUR,
                   (Font_types) Label_text_font, Label_text_font_size ) );

        y -= Filter_button_height + Filter_y_spacing;

        popup->callback_data[view_index].volume = volume;
        popup->callback_data[view_index].view = view_index;
        view_start_index = add_filter_widgets( &popup->popup_window, x, y,
                                 &popup->callback_data[view_index], &height );

        y -= height + Filter_button_height + 5 * Filter_y_spacing;

        filter_type = IF_get_slice_filter_type( volume, view_index );
        set_widget_selected(
                     popup->popup_window.widgets.widgets
                     [view_start_index + widget_indices[
                           get_filter_widget_index(filter_type)]], ON );

        filter_width = IF_get_slice_filter_width( volume, view_index );
        set_text_entry_real_value( popup->popup_window.widgets.widgets
           [view_start_index + widget_indices[FILTER_WIDTH_TEXT]],
           "%g", filter_width );
    }

    (void) add_widget_to_list( &popup->popup_window.widgets,
                               create_button( &popup->popup_window.graphics, 0,
                                x, y, Button_width, Button_height,
                                "Close", ON, TRUE, BUTTON_ACTIVE_COLOUR,
                                BUTTON_SELECTED_COLOUR,
                                BUTTON_INACTIVE_COLOUR,
                                BUTTON_TEXT_COLOUR,
                                (Font_types) Button_text_font,
                                Button_text_font_size,
                                close_callback, (void *) popup ) );
}

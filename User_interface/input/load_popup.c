/**
 * \file User_interface/input/load_popup.c
 * \brief Implement the popup shown during file loading.
 *
 * \copyright

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
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif //HAVE_CONFIG_H

#include  <user_interface.h>

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( cancel_button_callback )
{
    cancel_loading( (load_struct *) callback_data );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( quit_window_callback )
{
    cancel_loading( (load_struct *) callback_data );
}

  void  initialize_load_popup(
    load_struct   *load_data,
    int           x_position,
    int           y_position,
    VIO_STR        filename )
{
    widget_struct  *widget;

    create_popup_window( &load_data->popup, filename,
                         x_position, y_position,
                         Load_popup_x_size, Load_popup_y_size,
                         quit_window_callback, (void *) load_data );

    initialize_meter( &load_data->popup,
                      Load_meter_x_size, Load_meter_y_size,
                      (VIO_Colour) Load_meter_background_colour,
                      (VIO_Colour) Load_meter_colour,
                      &load_data->meter_background,
                      &load_data->meter );

    widget = create_button( &load_data->popup.graphics,
                   (Viewport_types) 0, Interface_x_spacing, Interface_y_spacing,
                   Button_width, Button_height,
                   "Cancel",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   cancel_button_callback, (void *) load_data );

    (void) add_widget_to_list( &load_data->popup.widgets, widget );
}

  void  delete_load_popup(
    load_struct   *load_data )
{
    delete_popup_window( &load_data->popup );
}

  void  set_load_popup_meter(
    load_struct   *load_data,
    VIO_Real          fraction_done )
{
    set_meter_position( &load_data->popup, load_data->meter_background,
                        load_data->meter, fraction_done );
}

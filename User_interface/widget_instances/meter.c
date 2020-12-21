/**
 * \file User_interface/widget_instances/meter.c
 * \brief Create progress bar used during file load or resampling.
 *
 * Probably this belongs in widgets??
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

  void  initialize_meter(
    popup_struct     *popup,
    int              x_size,
    int              y_size,
    VIO_Colour           background_colour,
    VIO_Colour           colour,
    polygons_struct  **meter_background,
    polygons_struct  **meter )
{
    int                     x, y, window_x_size, window_y_size;
    object_struct           *object;

    /* create the background of the meter */

    object = create_rectangle( background_colour );

    *meter_background = get_polygons_ptr( object );

    G_get_window_size( popup->graphics.window, &window_x_size, &window_y_size );

    x = (window_x_size - x_size) / 2;
    y = Interface_y_spacing + Button_height +
          (window_y_size - Interface_y_spacing - Button_height - y_size) / 2;

    position_rectangle( *meter_background, x, y, x_size, y_size );
    
    add_object_to_viewport( &popup->graphics.graphics, 0,
                            NORMAL_PLANES, object );

    /* create the meter */

    object = create_rectangle( colour );

    *meter = get_polygons_ptr( object );

    position_rectangle( *meter, x, y, 0, 0 );

    add_object_to_viewport( &popup->graphics.graphics, 0,
                            NORMAL_PLANES, object );
}

  void  set_meter_position(
    popup_struct      *popup,
    polygons_struct   *meter_background,
    polygons_struct   *meter,
    VIO_Real              fraction_done )
{
    int   x, y, x_size, y_size, new_pos;

    x = (int) Point_x(meter_background->points[0]);
    y = (int) Point_y(meter_background->points[0]);
    x_size = (int) Point_x(meter_background->points[1]) - x + 1;
    y_size = (int) Point_y(meter_background->points[2]) - y + 1;

    new_pos = VIO_ROUND( fraction_done * (VIO_Real) x_size );

    if( new_pos != (int) Point_x(meter->points[0]) )
    {
        position_rectangle( meter, x, y, new_pos, y_size );
        set_viewport_update_flag( &popup->graphics.graphics, 0, NORMAL_PLANES );
    }
}

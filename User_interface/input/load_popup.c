#include  <def_user_interface.h>

public  void  initialize_load_popup(
    load_meter_popup_struct   *popup,
    int                       x_position,
    int                       y_position,
    char                      filename[] )
{
    Bitplane_types          bitplane;
    int                     x, y;
    object_struct           *object;

    create_popup_window( &popup->graphics_window, filename,
                         x_position, y_position,
                         Load_popup_x_size, Load_popup_y_size );

    /* create the background of the meter */

    object = create_rectangle( (Colour) Load_meter_background_colour );

    popup->meter_background = (polygons_struct *) get_object_pointer( object );

    x = (Load_popup_x_size - Load_meter_x_size) / 2;
    y = (Load_popup_y_size - Load_meter_y_size) / 2;

    position_rectangle( popup->meter_background, x, y,
                        Load_meter_x_size, Load_meter_y_size );

    
    add_object_to_viewport( &popup->graphics_window.graphics, 0, NORMAL_PLANES,
                            object );

    /* create the meter */

    object = create_rectangle( (Colour) Load_meter_colour );

    popup->meter = (polygons_struct *) get_object_pointer( object );

    position_rectangle( popup->meter, x, y, 0, 0 );

    add_object_to_viewport( &popup->graphics_window.graphics, 0, NORMAL_PLANES,
                            object );

    for_enum( bitplane, N_BITPLANE_TYPES, Bitplane_types )
    {
        set_bitplanes_clear_flag( &popup->graphics_window.graphics,
                                  bitplane );
        set_viewport_update_flag( &popup->graphics_window.graphics, 0,
                                  bitplane );
    }
}

public  void  delete_load_popup(
    load_meter_popup_struct   *popup )
{
    delete_popup_window( &popup->graphics_window );
}

public  void  set_load_popup_meter(
    load_meter_popup_struct   *popup,
    Real                      fraction_done )
{
    int   x, y, x_size, y_size;

    x = (int) Point_x(popup->meter_background->points[0]);
    y = (int) Point_y(popup->meter_background->points[0]);
    x_size = (int) Point_x(popup->meter_background->points[1]) - x + 1;
    y_size = (int) Point_y(popup->meter_background->points[2]) - y + 1;

    position_rectangle( popup->meter, x, y,
                        ROUND( fraction_done * x_size ), y_size );
    set_viewport_update_flag( &popup->graphics_window.graphics, 0,
                              NORMAL_PLANES );
}

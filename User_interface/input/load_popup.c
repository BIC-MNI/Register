#include  <def_user_interface.h>

private  DEFINE_WIDGET_CALLBACK( cancel_button_callback )  /* ARGSUSED */
{
    cancel_loading( (load_struct *) callback_data );
}

public  void  initialize_load_popup(
    load_struct   *load_data,
    int           x_position,
    int           y_position,
    char          filename[] )
{
    int                     x, y;
    object_struct           *object;

    create_popup_window( &load_data->graphics_window, filename,
                         x_position, y_position,
                         Load_popup_x_size, Load_popup_y_size );

    /* create the background of the meter */

    object = create_rectangle( (Colour) Load_meter_background_colour );

    load_data->meter_background = (polygons_struct *)
                                  get_object_pointer( object );

    x = (Load_popup_x_size - Load_meter_x_size) / 2;
    y = Interface_y_spacing + Button_height +
          (Load_popup_y_size - Interface_y_spacing - Button_height -
           Load_meter_y_size) / 2;

    position_rectangle( load_data->meter_background, x, y,
                        Load_meter_x_size, Load_meter_y_size );
    
    add_object_to_viewport( &load_data->graphics_window.graphics, 0,
                            NORMAL_PLANES, object );

    /* create the meter */

    object = create_rectangle( (Colour) Load_meter_colour );

    load_data->meter = (polygons_struct *) get_object_pointer( object );

    position_rectangle( load_data->meter, x, y, 0, 0 );

    add_object_to_viewport( &load_data->graphics_window.graphics, 0,
                            NORMAL_PLANES, object );

    load_data->cancel_widget = create_button( &load_data->graphics_window,
                   0, Interface_x_spacing, Interface_y_spacing,
                   Button_width, Button_height,
                   "Cancel",
                   ON, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                   Button_text_font, Button_text_font_size,
                   cancel_button_callback, (void *) load_data );
}

public  void  delete_load_popup(
    load_struct   *load_data )
{
    delete_widget( load_data->cancel_widget );
    delete_popup_window( &load_data->graphics_window );
}

public  void  set_load_popup_meter(
    load_struct   *load_data,
    Real          fraction_done )
{
    int   x, y, x_size, y_size;

    x = (int) Point_x(load_data->meter_background->points[0]);
    y = (int) Point_y(load_data->meter_background->points[0]);
    x_size = (int) Point_x(load_data->meter_background->points[1]) - x + 1;
    y_size = (int) Point_y(load_data->meter_background->points[2]) - y + 1;

    position_rectangle( load_data->meter, x, y,
                        ROUND( fraction_done * x_size ), y_size );
    set_viewport_update_flag( &load_data->graphics_window.graphics, 0,
                              NORMAL_PLANES );
}

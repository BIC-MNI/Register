#include  <def_user_interface.h>

typedef  enum
{
    QUIT_BUTTON,
    RESAMPLE_BUTTON,
    SYNC_VOLUMES_BUTTON,
    INTERPOLATION_BUTTON,
    COLOUR_MODE_BUTTON,
    DOUBLE_BUFFER_BUTTON,
    N_MAIN_WIDGETS
} Main_widgets;

private  DEFINE_WIDGET_CALLBACK( quit_button_callback )
{
    set_quit_program_flag();
}

private  DEFINE_WIDGET_CALLBACK( resample_and_load_button_callback )
{
}

private  DEFINE_WIDGET_CALLBACK( sync_volumes_button_callback )
{
}

private  DEFINE_WIDGET_CALLBACK( interpolation_button_callback )
{
}

private  DEFINE_WIDGET_CALLBACK( colour_mode_button_callback )
{
}

private  DEFINE_WIDGET_CALLBACK( double_buffer_button_callback )
{
}

public  void  add_main_widgets(
    UI_struct         *ui_info )
{
    create_button( ui_info, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Quit", Button_colour, Button_text_colour,
                   Button_text_font, Button_text_font_size,
                   quit_button_callback );

    create_button( ui_info, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Resample", Button_colour, Button_text_colour,
                   Button_text_font, Button_text_font_size,
                   resample_and_load_button_callback );

    create_button( ui_info, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Sync Volumes", Button_colour, Button_text_colour,
                   Button_text_font, Button_text_font_size,
                   sync_volumes_button_callback );

    create_button( ui_info, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Smooth/Flat", Button_colour, Button_text_colour,
                   Button_text_font, Button_text_font_size,
                   interpolation_button_callback );

    create_button( ui_info, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "RGB / Colour", Button_colour, Button_text_colour,
                   Button_text_font, Button_text_font_size,
                   colour_mode_button_callback );

    create_button( ui_info, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Double/Single", Button_colour, Button_text_colour,
                   Button_text_font, Button_text_font_size,
                   double_buffer_button_callback );

    position_main_widgets( ui_info );
}

public  void  position_main_widgets(
    UI_struct         *ui_info )
{
    int   i, x, y, x_min, x_max, y_min, y_max;

    get_graphics_viewport( &ui_info->graphics_window.graphics,
                           Main_menu_viewport,
                           &x_min, &x_max, &y_min, &y_max );

    x = Main_menu_x_offset;
    y = y_max - y_min - 1 - Main_menu_y_offset;

    for_less( i, 0, N_MAIN_WIDGETS )
    {
        position_widget( ui_info->widget_list.widgets[i],
                         &ui_info->graphics_window.event_viewports,
                         Main_menu_viewport, x, y );

        y -= Button_height + Interface_y_spacing;
    }
}

#include  <def_user_interface.h>

typedef  enum
{
    QUIT_BUTTON,
    N_MAIN_WIDGETS
} Main_widgets;

private  DEFINE_EVENT_FUNCTION( quit_button_callback )
{
    set_quit_program_flag();
}

public  void  add_main_widgets(
    UI_struct         *ui_info )
{
    create_button( ui_info, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Quit", Button_colour, Button_text_colour,
                   Button_text_font, Button_text_font_size,
                   quit_button_callback );

    position_main_widgets( ui_info );
}

public  void  position_main_widgets(
    UI_struct         *ui_info )
{
    int   x, y, x_min, x_max, y_min, y_max;

    get_graphics_viewport( &ui_info->graphics_window.graphics,
                           Main_menu_viewport,
                           &x_min, &x_max, &y_min, &y_max );

    x = Main_menu_x_offset;
    y = y_max - y_min - 1 - Main_menu_y_offset;

    position_widget( ui_info->widget_list.widgets[QUIT_BUTTON],
                     &ui_info->graphics_window.event_viewports,
                     Main_menu_viewport, x, y );

    y -= Button_height + Interface_y_spacing;
}

#include  <def_user_interface.h>

typedef  enum
{
    OPACITY_2_SLIDER,
    OPACITY_1_SLIDER,
    RESET_VIEW_BUTTON,
    N_MERGED_WIDGETS
}
Merged_widgets;

static  int  widget_indices[N_MERGED_WIDGETS];

private  void  opacity_callback(
    widget_struct  *widget,
    int            which_volume )
{
    Real  value, ignored;

    get_slider_values( widget, &value, &ignored );

    IF_set_merged_volume_opacity( which_volume, value );
}

private  DEFINE_WIDGET_CALLBACK( opacity_2_callback ) /* ARGSUSED */
{
    opacity_callback( widget, 1 );
}

private  DEFINE_WIDGET_CALLBACK( opacity_1_callback ) /* ARGSUSED */
{
    opacity_callback( widget, 0 );
}

public  void  add_merged_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index )
{
    int      x, y, height;

    x = Volume_menu_x_offset;
    y = Volume_menu_y_offset;

    widget_indices[OPACITY_2_SLIDER] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_slider( &ui_info->graphics_window,
                   viewport_index, x, y, Opacity_slider_width,
                   Opacity_slider_height,
                   0.5, 0.0, 1.0, Opacity_text_format,
                   OFF,
                   SLIDER_ACTIVE_COLOUR, SLIDER_INACTIVE_COLOUR,
                   SLIDER_PEG_COLOUR,
                   opacity_2_callback, (void *) NULL ) );

    y += Opacity_slider_height + Volume_button_height +
         Interface_y_spacing;

    widget_indices[OPACITY_1_SLIDER] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_slider( &ui_info->graphics_window,
                   viewport_index, x, y, Opacity_slider_width,
                   Opacity_slider_height,
                   0.5, 0.0, 1.0, Opacity_text_format,
                   OFF,
                   SLIDER_ACTIVE_COLOUR, SLIDER_INACTIVE_COLOUR,
                   SLIDER_PEG_COLOUR,
                   opacity_1_callback, (void *) NULL ) );

    y += Opacity_slider_height + Volume_button_height +
         Interface_y_spacing;

    widget_indices[RESET_VIEW_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window,
               viewport_index, x, y,
               Volume_button_width, Volume_button_height,
               "Reset View",
               OFF, TRUE, BUTTON_ACTIVE_COLOUR,
               BUTTON_SELECTED_COLOUR,
               BUTTON_INACTIVE_COLOUR,
               BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
               Button_text_font, Button_text_font_size,
               reset_view_callback, (void *) NULL ) );

    y += Volume_button_height + Interface_y_spacing;

    ui_info->position_text_start_index[2] =
                                add_cursor_position_widgets(
                                ui_info, viewport_index, &height );
}

public  void  set_merged_widgets_activity(
    UI_struct         *ui_info,
    Boolean           activity )
{
    Merged_widgets       widgets;

    for_enum( widgets, N_MERGED_WIDGETS, Merged_widgets )
    {
        set_widget_activity( ui_info->widget_list[Merged_menu_viewport].widgets
                                          [widget_indices[widgets]],
                             activity );
    }

    set_voxel_position_widgets_activity( ui_info, Merged_menu_viewport,
                               ui_info->position_text_start_index[2],
                               activity );
}

#include  <def_user_interface.h>

typedef  enum
{
    OPACITY_2_SLIDER,
    OPACITY_1_SLIDER,
    RESET_VIEW_BUTTON,
    VALUE1_READOUT_TEXT,
    VALUE2_READOUT_TEXT,
    ONE_ON_TWO_BUTTON,
    TWO_ON_ONE_BUTTON,
    BLEND_BUTTON,
    WEIGHT_BUTTON,
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

private  void  set_merge_sliders_activity(
    UI_struct      *ui,
    Merge_methods  method )
{
    Boolean  slider1_on, slider2_on;

    switch( method )
    {
    default:
    case ONE_ON_TWO:
    case TWO_ON_ONE:
        slider1_on = FALSE;
        slider2_on = FALSE;
        break;

    case BLEND_VOLUMES:
        slider1_on = TRUE;
        slider2_on = FALSE;
        break;

    case WEIGHTED_VOLUMES:
        slider1_on = TRUE;
        slider2_on = TRUE;
        break;
    }

    set_widget_activity( ui->widget_list[Merged_menu_viewport].
                         widgets[widget_indices[OPACITY_1_SLIDER]],
                         slider1_on );
    set_widget_activity( ui->widget_list[Merged_menu_viewport].
                         widgets[widget_indices[OPACITY_2_SLIDER]],
                         slider2_on );
}

private  void  set_merging_method(
    Merge_methods  method )
{
    set_merge_sliders_activity( get_ui_struct(), method );
    IF_set_merge_method( method );
}

private  DEFINE_WIDGET_CALLBACK( one_on_two_callback ) /* ARGSUSED */
{
    set_merging_method( ONE_ON_TWO );
}

private  DEFINE_WIDGET_CALLBACK( two_on_one_callback ) /* ARGSUSED */
{
    set_merging_method( TWO_ON_ONE );
}

private  DEFINE_WIDGET_CALLBACK( blend_callback ) /* ARGSUSED */
{
    set_merging_method( BLEND_VOLUMES );
}

private  DEFINE_WIDGET_CALLBACK( weight_callback ) /* ARGSUSED */
{
    set_merging_method( WEIGHTED_VOLUMES );
}

private  Merged_widgets  get_merge_method_widget(
    Merge_methods merge_method )
{
    switch( merge_method )
    {
    default:
    case ONE_ON_TWO:         return( ONE_ON_TWO_BUTTON );
    case TWO_ON_ONE:         return( TWO_ON_ONE_BUTTON );
    case BLEND_VOLUMES:      return( BLEND_BUTTON );
    case WEIGHTED_VOLUMES:   return( WEIGHT_BUTTON );
    }
}

public  void  add_merged_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index )
{
    int             x, y, height;
    widget_struct   *radio_widgets[4];

    x = Volume_menu_x_offset;
    y = Volume_menu_y_offset;

    widget_indices[OPACITY_2_SLIDER] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_slider( &ui_info->graphics_window,
                   viewport_index, x, y, Opacity_slider_width,
                   Opacity_slider_height,
                   IF_get_merged_volume_opacity(1),
                   0.0, 1.0, Opacity_text_format,
                   OFF,
                   SLIDER_ACTIVE_COLOUR, SLIDER_INACTIVE_COLOUR,
                   SLIDER_PEG_COLOUR,
                   opacity_2_callback, (void *) NULL ) );

    x += Opacity_slider_width + Interface_x_spacing;

    widget_indices[ONE_ON_TWO_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window,
                       viewport_index, x, y,
                       Colour_bar_button_width, Volume_button_height,
                       "1 on 2",
                       OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                       BUTTON_SELECTED_COLOUR,
                       BUTTON_INACTIVE_COLOUR,
                       BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                       Button_text_font, Button_text_font_size,
                       one_on_two_callback, (void *) NULL ) );

    x += Colour_bar_button_width + Interface_x_spacing;

    widget_indices[TWO_ON_ONE_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window,
                       viewport_index, x, y,
                       Colour_bar_button_width, Volume_button_height,
                       "2 on 1",
                       OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                       BUTTON_SELECTED_COLOUR,
                       BUTTON_INACTIVE_COLOUR,
                       BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                       Button_text_font, Button_text_font_size,
                       two_on_one_callback, (void *) NULL ) );

    x = Volume_menu_x_offset;

    y += Opacity_slider_height + Volume_button_height +
         Interface_y_spacing;

    widget_indices[OPACITY_1_SLIDER] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_slider( &ui_info->graphics_window,
                   viewport_index, x, y, Opacity_slider_width,
                   Opacity_slider_height,
                   IF_get_merged_volume_opacity(0),
                   0.0, 1.0, Opacity_text_format,
                   OFF,
                   SLIDER_ACTIVE_COLOUR, SLIDER_INACTIVE_COLOUR,
                   SLIDER_PEG_COLOUR,
                   opacity_1_callback, (void *) NULL ) );

    x += Opacity_slider_width + Interface_x_spacing;

    widget_indices[BLEND_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window,
                       viewport_index, x, y,
                       Colour_bar_button_width, Volume_button_height,
                       "Blend",
                       OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                       BUTTON_SELECTED_COLOUR,
                       BUTTON_INACTIVE_COLOUR,
                       BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                       Button_text_font, Button_text_font_size,
                       blend_callback, (void *) NULL ) );

    x += Colour_bar_button_width + Interface_x_spacing;

    widget_indices[WEIGHT_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window,
                       viewport_index, x, y,
                       Colour_bar_button_width, Volume_button_height,
                       "Weights",
                       OFF, TRUE, BUTTON_ACTIVE_COLOUR,
                       BUTTON_SELECTED_COLOUR,
                       BUTTON_INACTIVE_COLOUR,
                       BUTTON_PUSHED_COLOUR, BUTTON_TEXT_COLOUR,
                       Button_text_font, Button_text_font_size,
                       weight_callback, (void *) NULL ) );

    x = Volume_menu_x_offset;

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

    widget_indices[VALUE1_READOUT_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_label( &ui_info->graphics_window, viewport_index,
                   x + Volume_button_width + Interface_x_spacing, y,
                   Value_readout_width, Volume_button_height,
                   "", OFF, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   BACKGROUND_COLOUR,
                   BACKGROUND_COLOUR,
                   Label_text_font, Label_text_font_size ) );

    widget_indices[VALUE2_READOUT_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_label( &ui_info->graphics_window, viewport_index,
                   x + Volume_button_width + Value_readout_width +
                   2 * Interface_x_spacing, y,
                   Value_readout_width, Volume_button_height,
                   "", OFF, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   BACKGROUND_COLOUR,
                   BACKGROUND_COLOUR,
                   Label_text_font, Label_text_font_size ) );

    y += Volume_button_height + Interface_y_spacing;

    ui_info->position_text_start_index[2] =
                                add_cursor_position_widgets(
                                ui_info, viewport_index, &height );


    radio_widgets[0] = ui_info->widget_list[viewport_index].widgets
                               [widget_indices[ONE_ON_TWO_BUTTON]];
    radio_widgets[1] = ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[TWO_ON_ONE_BUTTON]];
    radio_widgets[2] = ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[BLEND_BUTTON]];
    radio_widgets[3] = ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[WEIGHT_BUTTON]];

    define_radio_buttons( 4, radio_widgets );

    set_widget_selected( ui_info->widget_list[viewport_index].widgets
        [widget_indices[get_merge_method_widget(IF_get_merge_method())]], ON );
}

public  void  set_merged_widgets_activity(
    UI_struct         *ui_info,
    Boolean           activity )
{
    Merged_widgets       widgets;
    Merge_methods        merge_method;

    for_enum( widgets, N_MERGED_WIDGETS, Merged_widgets )
    {
        set_widget_activity( ui_info->widget_list[Merged_menu_viewport].widgets
                                          [widget_indices[widgets]],
                             activity );
    }

    set_voxel_position_widgets_activity( ui_info, Merged_menu_viewport,
                                         ui_info->position_text_start_index[2],
                                         activity );

    if( activity )
    {
        merge_method = IF_get_merge_method();
        set_widget_selected(
             ui_info->widget_list[Merged_menu_viewport].widgets
             [widget_indices[get_merge_method_widget(merge_method)]], ON );
        set_merge_sliders_activity( ui_info, merge_method );
    }
}

public  widget_struct  *get_merged_readout_widget(
    UI_struct     *ui_info,
    int           volume )
{
    return( ui_info->widget_list[Merged_menu_viewport].widgets
                             [widget_indices[VALUE1_READOUT_TEXT + volume]] );
}

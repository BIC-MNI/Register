/**
 * \file merged_interface.c
 * \brief User interface for the third "merged" volume.
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

#include  <user_interface.h>

typedef  enum
{
    OPACITY_1_SLIDER,
    OPACITY_2_SLIDER,
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

static  void  opacity_callback(
    widget_struct  *widget,
    int            which_volume )
{
    VIO_Real  value, ignored;

    get_slider_values( widget, &value, &ignored );

    IF_set_merged_volume_opacity( which_volume, value );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( opacity_2_callback )
{
    opacity_callback( widget, 1 );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( opacity_1_callback )
{
    opacity_callback( widget, 0 );
}

static  void  set_merge_sliders_activity(
    UI_struct      *ui,
    Merge_methods  method )
{
    VIO_BOOL  slider1_on, slider2_on;

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

static  void  set_merging_method(
    Merge_methods  method )
{
    set_merge_sliders_activity( get_ui_struct(), method );
    IF_set_merge_method( method );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( one_on_two_callback )
{
    set_merging_method( ONE_ON_TWO );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( two_on_one_callback )
{
    set_merging_method( TWO_ON_ONE );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( blend_callback )
{
    set_merging_method( BLEND_VOLUMES );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( weight_callback )
{
    set_merging_method( WEIGHTED_VOLUMES );
}

static  Merged_widgets  get_merge_method_widget(
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

  void  add_merged_widgets(
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
                   FALSE,
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
                       FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                       BUTTON_SELECTED_COLOUR,
                       BUTTON_INACTIVE_COLOUR,
                       BUTTON_TEXT_COLOUR,
                       (Font_types) Button_text_font, Button_text_font_size,
                       one_on_two_callback, (void *) NULL ) );

    x += Colour_bar_button_width + Interface_x_spacing;

    widget_indices[TWO_ON_ONE_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window,
                       viewport_index, x, y,
                       Colour_bar_button_width, Volume_button_height,
                       "2 on 1",
                       FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                       BUTTON_SELECTED_COLOUR,
                       BUTTON_INACTIVE_COLOUR,
                       BUTTON_TEXT_COLOUR,
                       (Font_types) Button_text_font, Button_text_font_size,
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
                   FALSE,
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
                       FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                       BUTTON_SELECTED_COLOUR,
                       BUTTON_INACTIVE_COLOUR,
                       BUTTON_TEXT_COLOUR,
                       (Font_types) Button_text_font, Button_text_font_size,
                       blend_callback, (void *) NULL ) );

    x += Colour_bar_button_width + Interface_x_spacing;

    widget_indices[WEIGHT_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window,
                       viewport_index, x, y,
                       Colour_bar_button_width, Volume_button_height,
                       "Weights",
                       FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                       BUTTON_SELECTED_COLOUR,
                       BUTTON_INACTIVE_COLOUR,
                       BUTTON_TEXT_COLOUR,
                       (Font_types) Button_text_font, Button_text_font_size,
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
               FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
               BUTTON_SELECTED_COLOUR,
               BUTTON_INACTIVE_COLOUR,
               BUTTON_TEXT_COLOUR,
               (Font_types) Button_text_font, Button_text_font_size,
               reset_view_callback, (void *) NULL ) );

    widget_indices[VALUE1_READOUT_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_label( &ui_info->graphics_window, viewport_index,
                   x + Volume_button_width + Interface_x_spacing, y,
                   Value_readout_width, Volume_button_height,
                   "", FALSE, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   BACKGROUND_COLOUR,
                   BACKGROUND_COLOUR,
                   (Font_types) Label_text_font, Label_text_font_size ) );

    widget_indices[VALUE2_READOUT_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_label( &ui_info->graphics_window, viewport_index,
                   x + Volume_button_width + Value_readout_width +
                   2 * Interface_x_spacing, y,
                   Value_readout_width, Volume_button_height,
                   "", FALSE, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   BACKGROUND_COLOUR,
                   BACKGROUND_COLOUR,
                   (Font_types) Label_text_font, Label_text_font_size ) );

    y += Volume_button_height + Interface_y_spacing;

    ui_info->position_text_start_index[MERGED_VOLUME_INDEX] =
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
        [widget_indices[get_merge_method_widget(IF_get_merge_method())]], TRUE );
}

  void  set_merged_widgets_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity )
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
                                         ui_info->position_text_start_index[MERGED_VOLUME_INDEX],
                                         activity );

    if( activity )
    {
        merge_method = IF_get_merge_method();
        set_widget_selected(
             ui_info->widget_list[Merged_menu_viewport].widgets
             [widget_indices[get_merge_method_widget(merge_method)]], TRUE );
        set_merge_sliders_activity( ui_info, merge_method );
    }
}

  widget_struct  *get_merged_readout_widget(
    UI_struct     *ui_info,
    int           volume )
{
    if (volume >= N_VOLUMES)
        return NULL;
    return( ui_info->widget_list[Merged_menu_viewport].widgets
                             [widget_indices[VALUE1_READOUT_TEXT + volume]] );
}

widget_struct *get_merged_blend_widget(UI_struct *ui_info, int volume)
{
    if (volume >= N_VOLUMES)
        return NULL;
    return( ui_info->widget_list[Merged_menu_viewport].widgets
            [widget_indices[OPACITY_1_SLIDER + volume]] );
}

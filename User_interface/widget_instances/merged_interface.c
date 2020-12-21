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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif //HAVE_CONFIG_H

#include  <user_interface.h>

typedef  enum
{
    BLEND_SLIDER,
    RESET_VIEW_BUTTON,
    VALUE1_READOUT_TEXT,
    VALUE2_READOUT_TEXT,
    VALUE3_READOUT_TEXT,
    VALUE4_READOUT_TEXT,
    VALUE5_READOUT_TEXT,
    VALUE6_READOUT_TEXT,
    VALUE7_READOUT_TEXT,
    VALUE8_READOUT_TEXT,
    N_MERGED_WIDGETS
} Merged_widgets;

static  int  widget_indices[N_MERGED_WIDGETS];

/* ARGSUSED */

static DEFINE_WIDGET_CALLBACK(blend_callback)
{
  UI_struct *ui = get_ui_struct();
  VIO_Real value, ignored;

  get_slider_values( widget, &value, &ignored );

  set_volume_blend_value( ui, value );
}

void  add_merged_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index )
{
    int             x, y, height;
    int             i;

    x = Volume_menu_x_offset;
    y = Volume_menu_y_offset;

    widget_indices[BLEND_SLIDER] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_slider( &ui_info->graphics_window,
                   viewport_index, x, y, Opacity_slider_width,
                   Opacity_slider_height,
                   IF_get_merged_volume_opacity(0),
                   0.0, 1.0, Opacity_text_format,
                   FALSE,
                   SLIDER_ACTIVE_COLOUR, SLIDER_INACTIVE_COLOUR,
                   SLIDER_PEG_COLOUR,
                   blend_callback, (void *) 0 ) );

    x = Volume_menu_x_offset;

    y += Opacity_slider_height + Volume_button_height +
         Interface_y_spacing;

    for (i = 0; i < (N_VOLUMES/2); i++) 
    {
        widget_indices[VALUE5_READOUT_TEXT + i] =
                   add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_label( &ui_info->graphics_window, viewport_index,
                   x, y,
                   Value_readout_width, Volume_button_height,
                   "", FALSE, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   BACKGROUND_COLOUR,
                   BACKGROUND_COLOUR,
                   (Font_types) Label_text_font, Label_text_font_size ) );
        x += Volume_button_width + Volume_x_spacing;
    }

    x = Volume_menu_x_offset;
    y += Volume_button_height + Interface_y_spacing;

    for (i = 0; i < N_VOLUMES/2; i++)
    {
        widget_indices[VALUE1_READOUT_TEXT + i] =
                   add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_label( &ui_info->graphics_window, viewport_index,
                   x, y,
                   Value_readout_width, Volume_button_height,
                   "", FALSE, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   BACKGROUND_COLOUR,
                   BACKGROUND_COLOUR,
                   (Font_types) Label_text_font, Label_text_font_size ) );
        x += Volume_button_width + Volume_x_spacing;
    }
    
    x = Volume_menu_x_offset;
    y += Volume_button_height + Interface_y_spacing;

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


    ui_info->position_text_start_index[MERGED_VOLUME_INDEX] =
      add_cursor_position_widgets( ui_info, viewport_index, &height, FALSE );
}

void
set_merged_widgets_activity( UI_struct *ui_info,
                             VIO_BOOL  activity )
{
    Merged_widgets widgets;
    int            i;

    for_enum( widgets, N_MERGED_WIDGETS, Merged_widgets )
    {
        set_widget_activity( ui_info->widget_list[Merged_menu_viewport].widgets
                                          [widget_indices[widgets]],
                             activity );
    }

    for (i = 0; i < N_VOLUMES; i++)
    {
      set_widget_activity( get_merged_readout_widget( ui_info, i),
                           i < ui_info->n_volumes_loaded );
    }
    set_voxel_position_widgets_activity( ui_info, Merged_menu_viewport,
                                         ui_info->position_text_start_index[MERGED_VOLUME_INDEX],
                                         activity, FALSE );
}

widget_struct *
get_merged_readout_widget( UI_struct *ui_info, int volume )
{
    if (volume >= N_VOLUMES)
        return NULL;
    return( ui_info->widget_list[Merged_menu_viewport].widgets
                             [widget_indices[VALUE1_READOUT_TEXT + volume]] );
}

widget_struct *
get_merged_blend_widget( UI_struct *ui_info )
{
    return( ui_info->widget_list[Merged_menu_viewport].widgets
            [widget_indices[BLEND_SLIDER]] );
}


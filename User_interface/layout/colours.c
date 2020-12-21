/**
 * \file User_interface/layout/colours.c
 * \brief Handle special indexed UI colours.
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

static  VIO_Colour  colours[N_UI_COLOURS];

  void  initialize_ui_colours( void )
{
    int i;

    colours[BACKGROUND_COLOUR]          = Default_UI_background_colour;
    colours[DIVIDER_COLOUR]             = Default_divider_colour;
    colours[BUTTON_ACTIVE_COLOUR]       = Default_button_active_colour;
    colours[BUTTON_INACTIVE_COLOUR]     = Default_button_inactive_colour;
    colours[BUTTON_SELECTED_COLOUR]     = Default_button_selected_colour;
    colours[BUTTON_TEXT_COLOUR]         = Default_button_text_colour;
    colours[TEXT_ENTRY_ACTIVE_COLOUR]   = Default_text_entry_active_colour;
    colours[TEXT_ENTRY_SELECTED_COLOUR] = Default_text_entry_selected_colour;
    colours[TEXT_ENTRY_INACTIVE_COLOUR] = Default_text_entry_inactive_colour;
    colours[TEXT_ENTRY_EDIT_COLOUR]     = Default_text_entry_edit_colour;
    colours[TEXT_ENTRY_TEXT_COLOUR]     = Default_text_entry_text_colour;
    colours[TEXT_ENTRY_EDIT_TEXT_COLOUR]= Default_text_entry_edit_text_colour;
    colours[TEXT_ENTRY_CURSOR_COLOUR]   = Default_text_entry_cursor_colour;
    colours[LABEL_ACTIVE_COLOUR]        = Default_label_active_colour;
    colours[LABEL_SELECTED_COLOUR]      = Default_label_selected_colour;
    colours[LABEL_INACTIVE_COLOUR]      = Default_label_inactive_colour;
    colours[LABEL_TEXT_COLOUR]          = Default_label_text_colour;
    colours[SLIDER_ACTIVE_COLOUR]       = Default_slider_active_colour;
    colours[SLIDER_INACTIVE_COLOUR]     = Default_slider_inactive_colour;
    colours[SLIDER_PEG_COLOUR]          = Default_slider_peg_colour;

    for (i = 0; i < N_VOLUMES; i++)
    {
      colours[VOLUME1_UNDER_COLOUR + i] = Default_volume1_under_colour;
      colours[VOLUME1_OVER_COLOUR + i]  = Default_volume1_over_colour;
    }
}

  void  set_start_colour_table(
    UI_struct  *ui )
{
}

  int  get_ui_colour_index(
    UI_colours  colour_name )
{
    return 0;
}

  VIO_Colour  get_ui_rgb_colour(
    UI_colours  colour_name )
{
    return( colours[colour_name] );
}

  VIO_Colour  get_ui_colour(
    VIO_BOOL        colour_map_state,
    UI_colours     colour_name )
{
    return( get_ui_rgb_colour(colour_name) );
}

void  set_ui_colour(
    UI_struct    *ui,
    UI_colours   colour_name,
    VIO_Colour       colour )
{
    colours[colour_name] = colour;
}

void  set_window_colours( window_struct *window )
{
    G_set_background_colour( window, get_ui_rgb_colour(BACKGROUND_COLOUR) );
}

void  colour_map_state_has_changed( UI_struct  *ui )
{
    set_start_colour_table( ui );

    set_window_colours( ui->graphics_window.window );

    update_all_widget_colours( ui );

    set_clear_and_update_flags( ui );
}

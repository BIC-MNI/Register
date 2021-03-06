/* ----------------------------------------------------------------------------
@COPYRIGHT  :
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
---------------------------------------------------------------------------- */

#ifndef lint
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/layout/colours.c,v 1.12 1998-06-29 15:01:58 david Exp $";
#endif

#include  <user_interface.h>

static  Colour  colours[N_UI_COLOURS];

public  void  initialize_ui_colours( void )
{
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

    colours[VOLUME1_UNDER_COLOUR]       = Default_volume1_under_colour;
    colours[VOLUME1_OVER_COLOUR]        = Default_volume1_over_colour;
    colours[VOLUME2_UNDER_COLOUR]       = Default_volume2_under_colour;
    colours[VOLUME2_OVER_COLOUR]        = Default_volume2_over_colour;
}

private  int  start_colour_table = 0;

public  void  set_start_colour_table(
    UI_struct  *ui )
{
    if( G_get_n_colour_map_entries(ui->graphics_window.window) <= 256 )
        start_colour_table = Colour_table_8bit_start;
    else
        start_colour_table = Colour_table_start;
}

public  int  get_ui_colour_index(
    UI_colours  colour_name )
{
    return( colour_name + start_colour_table );
}

public  Colour  get_ui_rgb_colour(
    UI_colours  colour_name )
{
    return( colours[colour_name] );
}

public  Colour  get_ui_colour(
    BOOLEAN        colour_map_state,
    UI_colours     colour_name )
{
    if( colour_map_state )
        return( (Colour) get_ui_colour_index(colour_name) );
    else
        return( get_ui_rgb_colour(colour_name) );
}

public  void  set_ui_colour(
    UI_struct    *ui,
    UI_colours   colour_name,
    Colour       colour )
{
    colours[colour_name] = colour;

    if( G_get_colour_map_state(ui->graphics_window.window) )
    {
        G_set_colour_map_entry( ui->graphics_window.window,
                                get_ui_colour_index(colour_name),
                                colour );
    }
}

public  void  set_window_colours( window_struct *window )
{
    UI_colours  colour_name;

    if( G_get_colour_map_state(window) )
    {
        for_enum( colour_name, N_UI_COLOURS, UI_colours )
        {
            G_set_colour_map_entry( window,
                                    get_ui_colour_index(colour_name),
                                    get_ui_rgb_colour(colour_name) );
        }
    }

    if( G_get_colour_map_state(window) )
    {
        G_set_background_colour( window,
                                 (Colour)
                                 get_ui_colour_index(BACKGROUND_COLOUR) );
    }
    else
    {
        G_set_background_colour( window,
                                 get_ui_rgb_colour(BACKGROUND_COLOUR) );
    }
}

public  void  colour_map_state_has_changed( UI_struct  *ui )
{
    set_start_colour_table( ui );

    set_window_colours( ui->graphics_window.window );

    IF_colour_mode_has_toggled( start_colour_table + N_UI_COLOURS );

    update_all_widget_colours( ui );

    set_clear_and_update_flags( ui );
}

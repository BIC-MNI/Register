#include  <def_user_interface.h>

static  Colour  colours[N_UI_COLOURS];

public  void  initialize_ui_colours()
{
    colours[BACKGROUND_COLOUR]          = Default_UI_background_colour;
    colours[BUTTON_ACTIVE_COLOUR]       = Default_button_active_colour;
    colours[BUTTON_INACTIVE_COLOUR]     = Default_button_inactive_colour;
    colours[BUTTON_SELECTED_COLOUR]     = Default_button_selected_colour;
    colours[BUTTON_PUSHED_COLOUR]       = Default_button_pushed_colour;
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
}

public  int  get_ui_colour_index(
    UI_colours  colour_name )
{
    return( colour_name + Colour_table_start );
}

public  Colour  get_ui_rgb_colour(
    UI_colours  colour_name )
{
    return( colours[colour_name] );
}

public  Colour  get_ui_colour(
    Boolean        colour_map_state,
    UI_colours     colour_name )
{
    if( colour_map_state )
        return( (Colour) get_ui_colour_index(colour_name) );
    else
        return( get_ui_rgb_colour(colour_name) );
}

private  void  set_window_colours( window_struct *window )
{
    UI_colours  colour_name;

    for_enum( colour_name, N_UI_COLOURS, UI_colours )
    {
        if( G_get_colour_map_state(window) )
        {
            G_set_colour_map_entry( window,
                                    get_ui_colour_index(colour_name),
                                    get_ui_rgb_colour(colour_name) );
        }
    }

    if( G_get_colour_map_state(window) )
    {
        G_set_background_colour( window,
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
    set_window_colours( ui->graphics_window.window );

    IF_colour_mode_has_toggled( Colour_table_start + N_UI_COLOURS );

    update_all_widget_colours( ui );

    set_clear_and_update_flags( ui );
}

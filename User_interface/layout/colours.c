#include  <def_user_interface.h>

static  Colour  colours[N_UI_COLOURS];

public  void  initialize_ui_colours()
{
    colours[BACKGROUND_COLOUR]          = Default_UI_background_colour;
    colours[BUTTON_ACTIVE_COLOUR]       = Default_button_active_colour;
    colours[BUTTON_INACTIVE_COLOUR]     = Default_button_inactive_colour;
    colours[BUTTON_PUSHED_COLOUR]       = Default_button_pushed_colour;
    colours[BUTTON_TEXT_COLOUR]         = Default_button_text_colour;
    colours[TEXT_ENTRY_ACTIVE_COLOUR]   = Default_text_entry_active_colour;
    colours[TEXT_ENTRY_INACTIVE_COLOUR] = Default_text_entry_inactive_colour;
    colours[TEXT_ENTRY_EDIT_COLOUR]     = Default_text_entry_edit_colour;
    colours[TEXT_ENTRY_TEXT_COLOUR]     = Default_text_entry_text_colour;
    colours[TEXT_ENTRY_TEXT_EDIT_COLOUR]= Default_text_entry_text_edit_colour;
    colours[TEXT_ENTRY_CURSOR_COLOUR]   = Default_text_entry_cursor_colour;
    colours[LABEL_ACTIVE_COLOUR]        = Default_label_active_colour;
    colours[LABEL_INACTIVE_COLOUR]      = Default_label_inactive_colour;
    colours[LABEL_TEXT_COLOUR]          = Default_label_text_colour;
}

public  Colour  get_ui_colour( UI_colours colour_name )
{
    return( colours[colour_name] );
}

public  Colour  get_ui_colour_index( UI_colours colour_name )
{
    return( colour_name + Colour_table_start );
}

public  void  set_window_colours( window_struct *window )
{
    UI_colours  colour_name;

    for_enum( colour_name, N_UI_COLOURS, UI_colours )
    {
        if( G_get_colour_map_state(window) )
        {
            G_set_colour_map_entry( window,
                                    get_ui_colour_index(colour_name),
                                    get_ui_colour(colour_name) );
        }
        else
        {
            G_set_pretend_colour_map( window,
                                      get_ui_colour_index(colour_name),
                                      get_ui_colour(colour_name) );
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
                                 get_ui_colour(BACKGROUND_COLOUR) );
    }
}

public  void  colour_map_state_has_changed( UI_struct  *ui )
{
    set_window_colours( ui->graphics_window.window );

    IF_set_start_of_colour_maps( Colour_table_start + N_UI_COLOURS );
}

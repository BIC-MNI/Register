#include  <user_interface.h>

#define  GLOBALS_LOOKUP_NAME   UI_globals_list
#include  <globals.h>

#define   UI_GLOBALS_FILENAME   "register_UI.globals"

private  void     initialize_global_colours();

private  UI_struct  ui_struct;

public  UI_struct  *get_ui_struct()
{
    return( &ui_struct );
}

int  main(
    int   argc,
    char  *argv[] )
{
    int              n_volumes;
    char             *filename;
    STRING           home_filename;
    Status           status;

    initialize_global_colours();

    (void) sprintf( home_filename, "$HOME/%s", UI_GLOBALS_FILENAME );

    if( file_exists( home_filename ) )
        status = input_globals_file( SIZEOF_STATIC_ARRAY(UI_globals_list),
                          UI_globals_list, home_filename );

    if( file_exists( UI_GLOBALS_FILENAME ) )
        status = input_globals_file( SIZEOF_STATIC_ARRAY(UI_globals_list),
                          UI_globals_list, UI_GLOBALS_FILENAME );

    initialize_argument_processing( argc, argv );

    status = initialize_user_interface( &ui_struct );

    n_volumes = 0;

    while( get_string_argument( "", &filename ) )
    {
        if( string_ends_in( filename, TAG_FILE_SUFFIX ) )
        {
            load_tags_file( &ui_struct, filename );
        }
        else if( n_volumes < 2 )
        {
            set_load_filename( &ui_struct, n_volumes, filename );
            status = initialize_loading_volume( &ui_struct, n_volumes,
                                                filename, FALSE );
            ++n_volumes;
        }
    }

    event_loop();

    terminate_user_interface( &ui_struct );

    output_alloc_to_file( ".register.alloc_debug" );

    return( status != OK );
}

private  void     initialize_global_colours()
{
    Default_UI_background_colour = DARK_SLATE_BLUE;
    Default_divider_colour = WHITE;
    Default_button_active_colour = YELLOW;
    Default_button_selected_colour = RED;
    Default_button_inactive_colour = DIM_GRAY;
    Default_button_pushed_colour = BLACK;
    Default_button_text_colour = BLACK;
    Default_text_entry_active_colour = WHITE;
    Default_text_entry_selected_colour = RED;
    Default_text_entry_inactive_colour = DIM_GRAY;
    Default_text_entry_edit_colour = BLACK;
    Default_text_entry_edit_text_colour = YELLOW;
    Default_text_entry_text_colour = BLACK;
    Default_text_entry_cursor_colour = GREEN;
    Default_label_active_colour = LIGHT_BLUE;
    Default_label_selected_colour = RED;
    Default_label_inactive_colour = DIM_GRAY;
    Default_label_text_colour = BLACK;
    Default_slider_active_colour = HOT_PINK;
    Default_slider_inactive_colour = DIM_GRAY;
    Default_slider_peg_colour = GREEN;
    Default_volume1_under_colour = BLACK;
    Default_volume1_over_colour = WHITE;
    Default_volume2_under_colour = BLACK;
    Default_volume2_over_colour = WHITE;
    Load_meter_background_colour = WHITE;
    Load_meter_colour = BLUE;
    Resample_meter_background_colour = WHITE;
    Resample_meter_colour = BLUE;
    Popup_background_colour = DIM_GREY;
    Message_text_colour = WHITE;
}

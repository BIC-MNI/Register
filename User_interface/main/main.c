#include  <user_interface.h>

#define  GLOBALS_LOOKUP_NAME   UI_globals_list
#include  <globals.h>

#define   UI_GLOBALS_FILENAME   "register_UI.globals"


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

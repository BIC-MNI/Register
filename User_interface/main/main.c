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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/main/main.c,v 1.19 1998-06-29 15:02:00 david Exp $";
#endif

#include  <user_interface.h>

private  STRING  version = "1.2      July  1, 1998";

#define  HARD_CODED_REGISTER_DIRECTORY1    "/usr/local/mni/lib"
#define  HARD_CODED_REGISTER_DIRECTORY2    "/usr/local/lib"

#define  GLOBALS_LOOKUP_NAME   UI_globals_list
#include  <globals.h>

#define   UI_GLOBALS_FILENAME   "register_UI.globals"

private  void     initialize_global_colours( void );
private  void  read_global_files(
    STRING  executable_name );

private  UI_struct  ui_struct;

public  UI_struct  *get_ui_struct( void )
{
    return( &ui_struct );
}

private  void  print_usage(
    char   executable[] )
{
    static  STRING  usage =
"Usage: %s  [-help] [-rgb] [-cmap] [-single] [-double] [volume1.mnc] \n\
                                [volume2.mnc] [tags.tag]\n\
\n\
    -help:      Prints this message.\n\
    -rgb:       Starts program in RGB mode.\n\
    -cmap:      Starts program in colour map mode.\n\
    -single:    Starts program in single buffer mode.\n\
    -double:    Starts program in double buffer mode.\n\
    -global variable value:\n\
                Sets a global variable that changes configuration of program.\n\
    -version:   Displays the version number of the program.\n\n";

    print_error( usage, executable );
}

int  main(
    int   argc,
    char  *argv[] )
{
    int              volume, n_volumes;
    STRING           argument;
    STRING           volume_filenames[2], tag_filename;
    STRING           variable_name, variable_value;
    Status           status;

    initialize_global_colours();

    read_global_files( argv[0] );

    initialize_argument_processing( argc, argv );

    tag_filename = NULL;

    n_volumes = 0;

    while( get_string_argument( "", &argument ) )
    {
        if( equal_strings( argument, "-help" ) )
        {
            print_usage( argv[0] );
            return( 0 );
        }
        else if( equal_strings( argument, "-version" ) )
        {
            print( "%s:  Version: %s\n", argv[0], version );
            return( 0 );
        }
        else if( equal_strings( argument, "-rgb" ) )
        {
            Initial_rgb_state = TRUE;
        }
        else if( equal_strings( argument, "-cmap" ) )
        {
            Initial_rgb_state = FALSE;
        }
        else if( equal_strings( argument, "-single" ) )
        {
            Initial_double_buffer_state = FALSE;
        }
        else if( equal_strings( argument, "-double" ) )
        {
            Initial_double_buffer_state = TRUE;
        }
        else if( equal_strings( argument, "-global" ) )
        {
            if( !get_string_argument( NULL, &variable_name ) ||
                !get_string_argument( NULL, &variable_value ) )
            {
                print_error( "Error in arguments after -global.\n" );
                return( 1 );
            }

            if( set_global_variable( SIZEOF_STATIC_ARRAY(UI_globals_list),
                        UI_globals_list, variable_name, variable_value ) != OK )
            {
                if( UI_set_global_variable( variable_name, variable_value )!=OK)
                    print("Error setting global variable from command line.\n");
            }
        }
        else if( string_ends_in( argument, TAG_FILE_SUFFIX ) )
        {
            tag_filename = argument;
        }
        else if( n_volumes < 2 )
        {
            volume_filenames[n_volumes] = argument;
            ++n_volumes;
        }
        else
        {
            print_usage( argv[0] );
            return( 0 );
        }
    }

    status = initialize_user_interface( &ui_struct, argv[0] );

    for_less( volume, 0, n_volumes )
    {
        set_load_filename( &ui_struct, volume, volume_filenames[volume] );
        status = initialize_loading_volume( &ui_struct, volume,
                                            volume_filenames[volume], FALSE );
    }

    if( tag_filename != NULL )
        load_tags_file( &ui_struct, tag_filename );

    event_loop();

    terminate_user_interface( &ui_struct );

    delete_global_variables( SIZEOF_STATIC_ARRAY(UI_globals_list),
                             UI_globals_list );

    output_alloc_to_file( ".register.alloc_debug" );

    return( status != OK );
}

private  void     initialize_global_colours( void )
{
    Default_UI_background_colour = DARK_SLATE_BLUE;
    Default_divider_colour = WHITE;
    Default_button_active_colour = YELLOW;
    Default_button_selected_colour = RED;
    Default_button_inactive_colour = DIM_GRAY;
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

private  void  read_global_files(
    STRING  executable_name )
{
    int      dir, n_directories;
    STRING   runtime_directory, *directories, globals_filename;

    runtime_directory = extract_directory( executable_name );

    n_directories = 0;
    directories = NULL;

    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          HARD_CODED_REGISTER_DIRECTORY2, DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          HARD_CODED_REGISTER_DIRECTORY1, DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          runtime_directory, DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          getenv("HOME"), DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories, ".", DEFAULT_CHUNK_SIZE );

    for_less( dir, 0, n_directories )
    {
        globals_filename = get_absolute_filename( UI_GLOBALS_FILENAME,
                                                  directories[dir] );

        if( file_exists( globals_filename ) )
        {
            (void) input_globals_file( SIZEOF_STATIC_ARRAY(UI_globals_list),
                                       UI_globals_list, globals_filename );
        }

        delete_string( globals_filename );
    }

    delete_string( runtime_directory );

    FREE( directories );
}

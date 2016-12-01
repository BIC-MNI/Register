/**
 * \file main.c
 * \brief Entry point and command line processing for Register.
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

#include "config.h"
#include  <user_interface.h>

/* Search for "register_UI.globals in  the following
 * three directories, then in the directory containing argv[0],
 * $HOME, and ".".
 *
 * Hard coded directories 2 and 3 are deprecated and will be removed
 * in a future release.
 */
// #define  HARD_CODED_REGISTER_DIRECTORY1    LIBDIR
// #define  HARD_CODED_REGISTER_DIRECTORY2    "/usr/local/mni/lib"
// #define  HARD_CODED_REGISTER_DIRECTORY3    "/usr/local/lib"

#include <float.h>
#define  GLOBALS_LOOKUP_NAME   UI_globals_list
#include <bicpl/globals.h>

#define   UI_GLOBALS_FILENAME   "register_UI.globals"

static  void     initialize_global_colours( void );
static  void  read_global_files(
    VIO_STR  executable_name );

static  UI_struct  ui_struct;

  UI_struct  *get_ui_struct( void )
{
    return( &ui_struct );
}

static  void  print_usage(
    char   executable[] )
{
    static  VIO_STR  usage =
"Usage: %s  [OPTION]... [volume.mnc] [tags.tag] [colourmap.ccd]\n\
You can specify up to %d volumes.\n\
\nOptions:\n\
    -help:      Prints this message.\n\
    -global variable value:\n\
                Sets a global variable that changes configuration of program.\n\
    -sync:      Start program with volume positions synchronized.\n\
    -range volume min max:\n\
                Sets the minimum and maximum colour range for volume 0 or 1.\n\
    -gray       Selects the gray colourmap for subsequent volumes.\n\
    -red        Selects the red colourmap for subsequent volumes.\n\
    -blue       Selects the blue colourmap for subsequent volumes.\n\
    -green      Selects the green colourmap for subsequent volumes.\n\
    -spectral   Selects the spectral colourmap for subsequent volumes.\n\
    -hot        Selects the hot metal colourmap for subsequent volumes.\n\
    -version:   Displays the version number of the program.\n\n";

    print_error( usage, executable, N_VOLUMES );
}

/**
 * Default error function for Register. Register just prints any errors to
 * the tty that is connected to the Register process.
 *
 * \param msg The message to print.
 */
static void
register_error( char *msg )
{
    fputs( msg, stderr );
}

int  main(
    int   argc,
    char  *argv[] )
{
    int              volume, n_volumes;
    VIO_STR          argument;
    VIO_STR          volume_filenames[N_VOLUMES], tag_filename;
    VIO_STR          variable_name, variable_value;
    VIO_Status       status;
    VIO_STR          ccd_filename;
    int              colour_coding_type[N_VOLUMES];

    set_print_error_function( register_error );
    initialize_global_colours();

    read_global_files( argv[0] );

    initialize_argument_processing( argc, argv );

    tag_filename = NULL;
    ccd_filename = NULL;

    n_volumes = 0;


    for (volume = 0; volume < N_VOLUMES; volume++)
    {
      colour_coding_type[volume] = -1;
    }

    while( get_string_argument( "", &argument ) )
    {
        if( equal_strings( argument, "-help" ) )
        {
            print_usage( argv[0] );
            return( 0 );
        }
        else if( equal_strings( argument, "-version" ) )
        {
            print("%s %s (built %s) git:%s/%s\n",
                  PACKAGE_NAME, PACKAGE_VERSION, __DATE__,
                  GIT_BRANCH, GIT_COMMIT );
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
        else if ( equal_strings( argument, "-sync" ) )
        {
            Initial_volumes_synced = TRUE;
        }
        else if( equal_strings( argument, "-range" ) )
        {
            int      volume_index;
            VIO_Real min_value;
            VIO_Real max_value;

            if (!get_int_argument( 0, &volume_index ) ||
                volume_index >= N_VOLUMES ||
                !get_real_argument( -1.0, &min_value ) ||
                !get_real_argument( -1.0, &max_value ) )
            {
                print_error( "Error in arguments after -range.\n");
                return( 1 );
            }
            if (volume_index == 0)
            {
                Volume_1_colour_coding_min = min_value;
                Volume_1_colour_coding_max = max_value;
            }
            else
            {
                Volume_2_colour_coding_min = min_value;
                Volume_2_colour_coding_max = max_value;
            }
        }
        else if( equal_strings( argument, "-global" ) )
        {
            if( !get_string_argument( NULL, &variable_name ) ||
                !get_string_argument( NULL, &variable_value ) )
            {
                print_error( "Error in arguments after -global.\n" );
                return( 1 );
            }

            if( set_global_variable( VIO_SIZEOF_STATIC_ARRAY(UI_globals_list),
                        UI_globals_list, variable_name, variable_value ) != VIO_OK )
            {
                if( UI_set_global_variable( variable_name, variable_value )!=VIO_OK)
                    print("Error setting global variable from command line.\n");
            }
        }
        else if( equal_strings( argument, "-gray") ||
                 equal_strings( argument, "-grey"))
        {
            colour_coding_type[n_volumes] = GRAY_SCALE;
        }
        else if( equal_strings( argument, "-red"))
        {
            colour_coding_type[n_volumes] = RED_COLOUR_MAP;
        }
        else if( equal_strings( argument, "-green"))
        {
            colour_coding_type[n_volumes] = GREEN_COLOUR_MAP;
        }
        else if( equal_strings( argument, "-blue"))
        {
            colour_coding_type[n_volumes] = BLUE_COLOUR_MAP;
        }
        else if( equal_strings( argument, "-hot"))
        {
            colour_coding_type[n_volumes] = HOT_METAL;
        }
        else if( equal_strings( argument, "-spectral"))
        {
            colour_coding_type[n_volumes] = SPECTRAL;
        }
        else if( string_ends_in( argument, TAG_FILE_SUFFIX ) )
        {
            tag_filename = argument;
        }
        else if( string_ends_in( argument, ".ccd" ) )
        {
            ccd_filename = argument;
            colour_coding_type[n_volumes] = USER_DEFINED_COLOUR_MAP;
        }
        else if( n_volumes < N_VOLUMES )
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

    for (volume = 0; volume < n_volumes; volume++)
    {
      if (colour_coding_type[volume] < 0)
      {
        colour_coding_type[volume] = (volume % 2) == 0 ?
          Volume_1_default_colour_coding :
          Volume_2_default_colour_coding;
      }
    }

    status = initialize_user_interface( &ui_struct, argv[0], n_volumes );

    for_less( volume, 0, n_volumes )
    {
        set_load_filename( &ui_struct, volume, volume_filenames[volume] );
        status = initialize_loading_volume( &ui_struct, volume,
                                            volume_filenames[volume], FALSE );
        if( ccd_filename != NULL && colour_coding_type[volume] == USER_DEFINED_COLOUR_MAP )
        {
            IF_load_volume_colour_coding( volume, ccd_filename );
        }

        IF_set_volume_colour_coding_type( volume, colour_coding_type[volume] );
    }

    if( tag_filename != NULL )
        load_tags_file( &ui_struct, tag_filename );

    event_loop();

    terminate_user_interface( &ui_struct );

    delete_global_variables( VIO_SIZEOF_STATIC_ARRAY(UI_globals_list),
                             UI_globals_list );

    return( status != VIO_OK );
}

static  void     initialize_global_colours( void )
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
    Overlay_colour_1 = RED;
    Overlay_colour_2 = GREEN;
    Overlay_colour_3 = BLUE;
    Slice_background_colour = DARK_SLATE_GREY;
    Cursor_inside_colour = RED;
    Cursor_outside_colour = BLUE;
    Tag_inside_colour = CYAN;
    Tag_outside_colour = MAGENTA;
    Tag_inside_inactive_colour = WHITE;
    Tag_outside_inactive_colour = GRAY;
    Initial_under_colour = BLACK;
    Initial_over_colour = WHITE;
}

void try_global_file(VIO_STR filename, VIO_STR directory)
{
    VIO_STR globals_filename = get_absolute_filename( filename, directory );
    if( file_exists( globals_filename ) )
    {
        input_globals_file( VIO_SIZEOF_STATIC_ARRAY(UI_globals_list),
                            UI_globals_list, globals_filename );
    }
    delete_string( globals_filename );
}

static  void  read_global_files(
    VIO_STR  executable_name )
{
    int      dir, n_directories;
    VIO_STR   runtime_directory, *directories;

    runtime_directory = extract_directory( executable_name );

    n_directories = 0;
    directories = NULL;

    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          HARD_CODED_REGISTER_DIRECTORY1, DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          HARD_CODED_REGISTER_DIRECTORY2, DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          HARD_CODED_REGISTER_DIRECTORY3, DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          runtime_directory, DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories,
                          getenv("HOME"), DEFAULT_CHUNK_SIZE );
    ADD_ELEMENT_TO_ARRAY( directories, n_directories, ".", DEFAULT_CHUNK_SIZE );

    for_less( dir, 0, n_directories )
    {
        try_global_file( UI_GLOBALS_FILENAME, directories[dir] );
        try_global_file( "register.globals", directories[dir] );
    }

    delete_string( runtime_directory );

    FREE( directories );
}

  VIO_Status  set_functional_global_variable(
    VIO_STR  variable_name,
    VIO_STR  value_to_set )
{
    return( set_global_variable( VIO_SIZEOF_STATIC_ARRAY(UI_globals_list),
                                 UI_globals_list, variable_name,
                                 value_to_set ) );
}

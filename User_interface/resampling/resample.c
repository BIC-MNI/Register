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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/resampling/resample.c,v 1.7 1995-07-31 19:54:27 david Exp $";
#endif

#include  <user_interface.h>

public  void  do_resampling(
    UI_struct           *ui,
    General_transform   *resampling_transform,
    char                resampled_filename[] )
{
    STRING                  output_filename;

    (void) strcpy( ui->original_filename_volume_2,
                   IF_get_volume_filename(RESAMPLED_VOLUME_INDEX) );

    expand_filename( resampled_filename, output_filename );
    (void) strcpy( ui->resampled_filename, resampled_filename );

    delete_general_transform( &ui->resampling_transform );
    copy_general_transform( resampling_transform, &ui->resampling_transform );

    print( "Please wait 2 minutes for the resampling.\n" );
    
    force_update_all_windows();

    if( IF_do_resampling( resampled_filename ) == OK )
    {
        (void) initialize_loading_volume( get_ui_struct(),
                                          RESAMPLED_VOLUME_INDEX,
                                          get_ui_struct()->resampled_filename,
                                          TRUE );
    }
    else
        print( "Resampling failed.\n" );
}

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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/resampling/resample.c,v 1.10 1998-06-29 15:02:01 david Exp $";
#endif

#include  <user_interface.h>

public  void  do_resampling(
    UI_struct           *ui,
    General_transform   *resampling_transform,
    STRING              resampled_filename )
{
    replace_string( &ui->original_filename_volume_2,
               create_string(IF_get_volume_filename(RESAMPLED_VOLUME_INDEX)) );

    replace_string( &ui->resampled_filename,
                    expand_filename( resampled_filename ) );

    delete_general_transform( &ui->resampling_transform );
    copy_general_transform( resampling_transform, &ui->resampling_transform );

    print( "Please wait 2 minutes for the resampling.\n" );
    
    make_windows_up_to_date();

    if( IF_do_resampling( resampled_filename ) == OK )
    {
        (void) initialize_loading_volume( ui, RESAMPLED_VOLUME_INDEX,
                                          ui->resampled_filename, TRUE );
    }
    else
        print( "Resampling failed.\n" );
}

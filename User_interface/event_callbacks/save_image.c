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
static char rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/event_callbacks/save_image.c,v 1.3 1995-10-02 18:34:50 david Exp $";
#endif

#include  <user_interface.h>

/* ARGSUSED */

public  void  save_current_viewport(
    UI_struct        *ui,
    Viewport_types   viewport )
{
    int   volume, view;

    ui_get_volume_view_index( viewport, &volume, &view );

    IF_save_image( volume, view );
}

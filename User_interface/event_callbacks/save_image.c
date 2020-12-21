/**
 * \file User_interface/event_callbacks/save_image.c
 * \brief Handle saving slice images.
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif //HAVE_CONFIG_H

#include  <user_interface.h>

/* ARGSUSED */

  void  save_current_viewport(
    UI_struct        *ui,
    Viewport_types   viewport )
{
    int   volume, view;

    ui_get_volume_view_index( viewport, &volume, &view );

    IF_save_image( volume, view );
}

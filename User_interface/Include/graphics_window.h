
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

#ifndef  DEF_GRAPHICS_WINDOW
#define  DEF_GRAPHICS_WINDOW

#ifndef lint
static char graphics_window_rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/Include/graphics_window.h,v 1.5 1995-07-31 19:54:19 david Exp $";
#endif

#include  <graphics.h>
#include  <events.h>

typedef struct
{
    window_struct           *window;
    event_viewports_struct  event_viewports;
    graphics_struct         graphics;
    int                     current_buffer;
} graphics_window_struct;

#endif

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

#ifndef  DEF_EVENTS
#define  DEF_EVENTS

#ifndef lint
static char ui_events_rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/Include/events.h,v 1.6 1996-12-09 20:21:44 david Exp $";
#endif

#include  <graphics.h>
#include  <viewports.h>

typedef  void  (*event_function_type) ( Viewport_types, int, void * );

#define  DEFINE_EVENT_FUNCTION( name )                                        \
                                                /* ARGSUSED */                \
         void   name( Viewport_types  event_viewport_index,                   \
                      int             keyboard_character,                     \
                      void            *callback_data )

typedef  enum  { ANY_MODIFIER, NO_SHIFT_ONLY, SHIFT_ONLY } Event_modifiers;

typedef  struct
{
    BOOLEAN               enabled;
    Event_modifiers       modifier;
    int                   x_min, x_max, y_min, y_max;
    event_function_type   callback;
    void                  *callback_data;
} event_callback_struct;

typedef  struct
{
    int                     n_callbacks;
    event_callback_struct   *callbacks;
} event_callback_list_struct;

typedef  struct
{
    int                           x_min, x_max, y_min, y_max;
    event_callback_list_struct    table[N_EVENT_TYPES];
} event_viewport_struct;

typedef  struct
{
    int                    n_event_viewports;
    event_viewport_struct  *event_viewports;
} event_viewports_struct;

#endif

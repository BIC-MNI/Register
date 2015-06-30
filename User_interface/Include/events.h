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

#include  <graphics.h>
#include  <viewports.h>

typedef  enum  {
                   NO_EVENT,
                   TERMINATE_INTERACTION_EVENT,
                   KEY_DOWN_EVENT,
                   KEY_UP_EVENT,
                   MOUSE_MOVEMENT_EVENT,
                   LEFT_MOUSE_DOWN_EVENT,
                   LEFT_MOUSE_UP_EVENT,
                   MIDDLE_MOUSE_DOWN_EVENT,
                   MIDDLE_MOUSE_UP_EVENT,
                   RIGHT_MOUSE_DOWN_EVENT,
                   RIGHT_MOUSE_UP_EVENT,
                   REDRAW_OVERLAY_EVENT,
                   WINDOW_REDRAW_EVENT,
                   WINDOW_RESIZE_EVENT,
                   WINDOW_ICONIZED_EVENT,
                   WINDOW_DEICONIZED_EVENT,
                   WINDOW_QUIT_EVENT,
                   WINDOW_LEAVE_EVENT,
                   WINDOW_ENTER_EVENT,
                   SCROLL_UP_EVENT,
                   SCROLL_DOWN_EVENT,
                   N_EVENT_TYPES
               } Event_types;

typedef  void  (*event_function_type) ( Viewport_types, int, void * );

#define  DEFINE_EVENT_FUNCTION( name )                                        \
                                                /* ARGSUSED */                \
         void   name( Viewport_types  event_viewport_index,                   \
                      int             keyboard_character,                     \
                      void            *callback_data )

typedef  enum  { ANY_MODIFIER, NO_SHIFT_ONLY, SHIFT_ONLY } Event_modifiers;

typedef  struct
{
    VIO_BOOL               enabled;
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

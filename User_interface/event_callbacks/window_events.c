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
static char rcsid[] = "$Header: /static-cvsroot/visualization/Register/User_interface/event_callbacks/window_events.c,v 1.13 1998-06-29 15:01:55 david Exp $";
#endif

#include  <user_interface.h>

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( redraw_overlay_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;

    set_clear_and_update_flags( ui );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( redraw_window_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;

    set_clear_and_update_flags( ui );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( resize_window_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;

    resize_layout( ui );

    position_main_widgets( ui );

    set_recreate_all_slice_flags();

    set_clear_and_update_flags( ui );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( quit_window_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;
#ifdef DEBUG
    set_quit_program_flag();
#else
    popup_quit_confirm( ui );
#endif
}

/* ---- a hook to force update of colour map, when other program has
        screwed up the colour map ------------------------------------ */

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( deiconize_window_callback )
{
    UI_struct  *ui = (UI_struct *) callback_data;

    set_window_colours( ui->graphics_window.window );
}

  void  install_window_events(
    UI_struct  *ui )
{
    set_event_viewport( &ui->graphics_window.event_viewports,
                        Whole_window_event_viewport, -1, -1, -1, -1 );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 REDRAW_OVERLAY_EVENT, -1, -1, -1, -1,
                                 redraw_overlay_callback, ANY_MODIFIER,
                                 (void *) ui );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_REDRAW_EVENT, -1, -1, -1, -1,
                                 redraw_window_callback, ANY_MODIFIER,
                                 (void *) ui );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_RESIZE_EVENT, -1, -1, -1, -1,
                                 resize_window_callback, ANY_MODIFIER,
                                 (void *) ui );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_QUIT_EVENT, -1, -1, -1, -1,
                                 quit_window_callback, ANY_MODIFIER,
                                 (void *) ui );

    add_event_viewport_callback( &ui->graphics_window.event_viewports,
                                 Whole_window_event_viewport,
                                 WINDOW_DEICONIZED_EVENT, -1, -1, -1, -1,
                                 deiconize_window_callback, ANY_MODIFIER,
                                 (void *) ui );
}

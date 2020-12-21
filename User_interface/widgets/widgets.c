/**
 * \file User_interface/widgets/widgets.c
 * \brief Generic functions for all widget types (e.g. the 'base class')
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

  void  initialize_widget_list(
    widgets_struct   *widget_list )
{
    widget_list->n_widgets = 0;
}

  void  delete_widget_list(
    widgets_struct   *widget_list )
{
    int   i;

    if( widget_list->n_widgets > 0 )
    {
        for_less( i, 0, widget_list->n_widgets )
            delete_widget( widget_list->widgets[i] );

        FREE( widget_list->widgets );
    }
}

  int  add_widget_to_list(
    widgets_struct   *widget_list,
    widget_struct    *widget )
{
    int   widget_index;

    widget_index = widget_list->n_widgets;

    ADD_ELEMENT_TO_ARRAY( widget_list->widgets, widget_list->n_widgets,
                          widget, DEFAULT_CHUNK_SIZE );

    return( widget_index );
}

  void  delete_nth_widget_from_list(
    widgets_struct   *widget_list,
    int              index )
{
    DELETE_ELEMENT_FROM_ARRAY( widget_list->widgets, widget_list->n_widgets,
                               index, DEFAULT_CHUNK_SIZE );
}

  widget_struct  *create_widget(
    Widget_types            type,
    int                     x,
    int                     y,
    int                     x_size,
    int                     y_size,
    VIO_BOOL                 initial_activity,
    VIO_BOOL                 use_ui_colours,
    graphics_window_struct  *graphics,
    Viewport_types          viewport_index )
{
    widget_struct   *widget;

    ALLOC( widget, 1 );
    widget->widget_type = type;
    widget->x = x;
    widget->y = y;
    widget->x_size = x_size;
    widget->y_size = y_size;
    widget->active_flag = initial_activity;
    widget->selected_flag = FALSE;
    widget->use_ui_colours = use_ui_colours;
    widget->graphics = graphics;
    widget->viewport_index = viewport_index;

    return( widget );
}

  int  get_widget_height(
    widget_struct   *widget )
{
    return( widget->y_size );
}

/* -------------------- function lookup ----------------------------- */

/* -------------------- position function ----------------------------- */

static  void  position_button_widget(
    widget_struct            *widget,
    int                      x,
    int                      y )
{
    position_button( widget, x, y );
}

static  void  position_text_entry_widget(
    widget_struct            *widget,
    int                      x,
    int                      y )
{
    position_text_entry( widget, x, y );
}

static  void  position_slider_widget(
    widget_struct            *widget,
    int                      x,
    int                      y )
{
    position_slider( widget, x, y );
}

/* -------------------- update colours function ----------------------------- */

static  void  update_colours_button_widget(
    widget_struct            *widget )
{
    update_button_colours( widget );
}

static  void  update_colours_text_entry_widget(
    widget_struct            *widget )
{
    update_text_entry_colours( widget );
}

static  void  update_colours_slider_widget(
    widget_struct            *widget )
{
    update_slider_colours( widget );
}

/* -------------------- delete function ----------------------------- */

static  void  delete_button_widget(
    widget_struct            *widget )
{
    delete_button( widget );
}

static  void  delete_text_entry_widget(
    widget_struct            *widget )
{
    delete_text_entry( widget );
}

static  void  delete_slider_widget(
    widget_struct            *widget )
{
    delete_slider( widget );
}

/* -------------------------------------------------------------------------- */

typedef  struct
{
    void   (*position_function)( widget_struct *, int, int );
    void   (*update_activity)( widget_struct * );
    void   (*update_colours)( widget_struct * );
    void   (*delete_function)( widget_struct * );
}
widget_functions_list;

static  widget_functions_list   widget_functions[N_WIDGET_TYPES] =
{
    {
        position_button_widget,
        update_button_activity,
        update_colours_button_widget,
        delete_button_widget
    },                                                    /* BUTTON */

    {
        position_text_entry_widget,
        update_text_entry_activity,
        update_colours_text_entry_widget,
        delete_text_entry_widget
    },                                                    /* TEXT_ENTRY */

    {
        position_slider_widget,
        update_slider_activity,
        update_colours_slider_widget,
        delete_slider_widget
    }                                                     /* TEXT_ENTRY */
};

/* ------------------------------------------------------------------- */

  void  position_widget(
    widget_struct           *widget,
    int                     x,
    int                     y )
{
    widget->x = x;
    widget->y = y;
    widget_functions[widget->widget_type].position_function( widget, x, y );
}

  button_struct  *get_widget_button(
    widget_struct  *widget )
{
    return( &widget->specific.button );
}

  text_entry_struct  *get_widget_text_entry(
    widget_struct  *widget )
{
    return( &widget->specific.text_entry );
}

  slider_struct  *get_widget_slider(
    widget_struct  *widget )
{
    return( &widget->specific.slider );
}

  void  set_widget_activity(
    widget_struct  *widget,
    VIO_BOOL        activity )
{
    widget->active_flag = activity;

    widget_functions[widget->widget_type].update_activity( widget );

    set_viewport_update_flag( &widget->graphics->graphics,
                              (int) widget->viewport_index, NORMAL_PLANES );
}

  void  set_widget_selected(
    widget_struct  *widget,
    VIO_BOOL        selected )
{
    widget->selected_flag = selected;

    update_widget_colours( widget );
}

  void  update_widget_colours(
    widget_struct  *widget )
{
    widget_functions[widget->widget_type].update_colours( widget );
}

  void  update_widget_list_colours(
    widgets_struct      *widget_list )
{
    int    i;

    for_less( i, 0, widget_list->n_widgets )
        update_widget_colours( widget_list->widgets[i] );
}

  void  delete_widget(
    widget_struct  *widget )
{
    widget_functions[widget->widget_type].delete_function( widget );
    FREE( widget );
}

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

#ifndef  DEF_WIDGETS
#define  DEF_WIDGETS

#ifndef lint
static char widgets_rcsid[] = "$Header: /private-cvsroot/visualization/Register/User_interface/Include/widgets.h,v 1.6 1995-07-31 19:54:18 david Exp $";
#endif

#include  <events.h>
#include  <graphics_window.h>
#include  <ui_colours.h>

typedef  enum { BUTTON,
                TEXT_ENTRY,
                SLIDER,
                N_WIDGET_TYPES } Widget_types;


typedef  void  (*widget_callback_type) ();

#define  DEFINE_WIDGET_CALLBACK( name )                                        \
                                                           /* ARGSUSED */      \
         void   name( widget_struct           *widget,                         \
                      void                    *callback_data )

typedef  struct
{
    BOOLEAN                  toggle_flag;
    BOOLEAN                  state;
    STRING                   toggle_text[2];
    Real                     time_to_unpush;
    widget_callback_type     push_callback;
    void                     *callback_data;
    polygons_struct          *polygons;
    text_struct              *text;
    int                      update_counter;

    UI_colours               active_colour;
    UI_colours               selected_colour;
    UI_colours               inactive_colour;
    UI_colours               pushed_colour;
    UI_colours               text_colour;

    struct  widget_struct    *next_radio_button;
} button_struct;

typedef  struct
{
    BOOLEAN                  label_only_flag;
    BOOLEAN                  in_edit_mode;
    BOOLEAN                  clear_text_when_selected;
    STRING                   string;
    STRING                   saved_string;
    int                      left_index;
    int                      string_index;
    widget_callback_type     hit_return_callback;
    void                     *callback_data;
    polygons_struct          *polygons;
    object_struct            *cursor;
    text_struct              *text;

    UI_colours               active_colour;
    UI_colours               selected_colour;
    UI_colours               inactive_colour;
    UI_colours               text_colour;
    UI_colours               edit_colour;
    UI_colours               text_edit_colour;
    UI_colours               cursor_colour;
} text_entry_struct;

typedef  struct
{
    BOOLEAN                  colour_bar_flag;
    Real                     min_value, max_value;
    BOOLEAN                  values_allowed_outside_range;
    Real                     values[2];
    STRING                   format_string;

    int                      peg_being_moved;
    int                      middle_mouse_offset;

    UI_colours               active_colour;
    UI_colours               inactive_colour;
    UI_colours               peg_colour;
    Real                     peg_width;

    widget_callback_type     value_changed_callback[2];
    void                     *callback_data[2];

    polygons_struct          *polygons;
    polygons_struct          *pegs[2];
    struct widget_struct     *text_widgets[2];
}
slider_struct;

typedef  struct  widget_struct
{
    Widget_types             widget_type;
    graphics_window_struct   *graphics;
    int                      viewport_index;
    int                      x, y, x_size, y_size;
    BOOLEAN                  active_flag;
    BOOLEAN                  selected_flag;
    BOOLEAN                  use_ui_colours;
    union
    {
    button_struct            button;
    text_entry_struct        text_entry;
    slider_struct            slider;
    } specific;
}
widget_struct;

typedef  struct
{
    int            n_widgets;
    widget_struct  **widgets;
}
widgets_struct;

#endif

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

#include  <events.h>
#include  <graphics_window.h>
#include  <ui_colours.h>

typedef  enum { BUTTON,
                TEXT_ENTRY,
                SLIDER,
                N_WIDGET_TYPES } Widget_types;


struct  widget_struct;

typedef  void  (*widget_callback_type) ( struct  widget_struct *, void * );

#define  DEFINE_WIDGET_CALLBACK( name )                                        \
                                                           /* ARGSUSED */      \
         void   name( struct  widget_struct   *widget,                         \
                      void                    *callback_data )

typedef  struct
{
    int                      toggle_count;
    int                      state;
    VIO_STR                  *toggle_text;
    VIO_Real                 time_to_unpush;
    widget_callback_type     push_callback;
    void                     *callback_data;
    polygons_struct          *polygons;
    text_struct              *text;
    int                      update_counter;

    VIO_Colour                   active_colour;
    VIO_Colour                   selected_colour;
    VIO_Colour                   inactive_colour;
    VIO_Colour                   text_colour;

    struct  widget_struct    *next_radio_button;
} button_struct;

typedef  struct
{
    VIO_BOOL                 label_only_flag;
    VIO_BOOL                 in_edit_mode;
    VIO_BOOL                 clear_text_when_selected;
    VIO_STR                  string;
    VIO_STR                  saved_string;
    int                      left_index;
    int                      string_index;
    widget_callback_type     hit_return_callback;
    void                     *callback_data;
    polygons_struct          *polygons;
    object_struct            *cursor;
    text_struct              *text;

    VIO_Colour               active_colour;
    VIO_Colour               selected_colour;
    VIO_Colour               inactive_colour;
    VIO_Colour               text_colour;
    VIO_Colour               edit_colour;
    VIO_Colour               text_edit_colour;
    VIO_Colour               cursor_colour;
    VIO_Real                 blink_time;
} text_entry_struct;

typedef  struct
{
    VIO_BOOL                  colour_bar_flag;
    VIO_Real                     min_value, max_value;
    VIO_BOOL                  values_allowed_outside_range;
    VIO_Real                     values[2];
    VIO_STR                   format_string;

    int                      peg_being_moved;
    int                      middle_mouse_offset;

    VIO_Colour                   active_colour;
    VIO_Colour                   inactive_colour;
    VIO_Colour                   peg_colour;
    VIO_Real                     peg_width;

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
    Viewport_types           viewport_index;
    int                      x, y, x_size, y_size;
    VIO_BOOL                  active_flag;
    VIO_BOOL                  selected_flag;
    VIO_BOOL                  use_ui_colours;
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

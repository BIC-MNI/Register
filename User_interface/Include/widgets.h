#ifndef  DEF_WIDGETS
#define  DEF_WIDGETS

#include  <def_events.h>
#include  <def_graphics_window.h>
#include  <def_ui_colours.h>

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
    Boolean                  toggle_flag;
    Boolean                  state;
    String                   toggle_text[2];
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
    Boolean                  label_only_flag;
    Boolean                  in_edit_mode;
    Boolean                  clear_text_when_selected;
    String                   string;
    String                   saved_string;
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
    Boolean                  colour_bar_flag;
    Real                     min_value, max_value;
    Boolean                  values_allowed_outside_range;
    Real                     values[2];
    String                   format_string;

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
    Boolean                  active_flag;
    Boolean                  selected_flag;
    Boolean                  use_ui_colours;
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

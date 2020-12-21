/**
 * \file User_interface/widgets/sliders.c
 * \brief Implement draggable scroll bars for setting volume opacity, e.g.
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

static  VIO_Real  convert_position_to_slider_value( slider_struct *, int, int );

static  int  convert_slider_value_to_position( slider_struct *, int, VIO_Real );

static  void  update_value_from_text(
    widget_struct  *widget,
    int            ind )
{
    VIO_Real            value, values[2];
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    if( get_text_entry_real_value( slider->text_widgets[ind], &value ) )
    {
        values[0] = slider->values[0];
        values[1] = slider->values[1];

        values[ind] = value;

        if( slider->colour_bar_flag )
        {
            if( ind == 0 && value > values[1] )
                values[0] = values[1];
            else if( ind == 1 && value < values[0] )
                values[1] = values[0];
        }

        set_slider_values( widget, values[0], values[1] );
    }
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( upper_value_hit_return_callback )
{
    update_value_from_text( (widget_struct *) callback_data, 1 );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( lower_value_hit_return_callback )
{
    update_value_from_text( (widget_struct *) callback_data, 0 );
}

static  void  update_slider_position_from_mouse(
    widget_struct  *widget )
{
    int             ind, x_mouse, y_mouse;
    VIO_Real            values[2];
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    ind = slider->peg_being_moved;

    get_viewport_mouse_position( widget->graphics, widget->viewport_index,
                                 &x_mouse, &y_mouse);

    values[0] = slider->values[0];

    if( slider->colour_bar_flag )
        values[1] = slider->values[1];

    values[ind] = convert_position_to_slider_value( slider, x_mouse - widget->x,
                                                    widget->x_size );

    if( slider->colour_bar_flag && values[0] > values[1] )
        values[ind] = values[1-ind];

    set_slider_values( widget, values[0], values[1] );
}

static  void  update_both_slider_position_from_mouse(
    widget_struct  *widget )
{
    int             x_mouse, y_mouse;
    VIO_Real            value, diff;
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    get_viewport_mouse_position( widget->graphics, widget->viewport_index,
                                 &x_mouse, &y_mouse);

    value = convert_position_to_slider_value( slider,
                      x_mouse - widget->x + slider->middle_mouse_offset,
                      widget->x_size );

    diff = slider->values[1] - slider->values[0];

    if( !slider->values_allowed_outside_range )
    {
        if( value < slider->min_value )
            value = slider->min_value;
        else if( value > slider->max_value )
            value = slider->max_value;

        if( value + diff > slider->max_value )
            value = slider->max_value - diff;
    }

    set_slider_values( widget, value, value + diff );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( update_slider_position_event )
{
    update_slider_position_from_mouse( (widget_struct *) callback_data );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( done_moving_slider )
{
    update_slider_position_from_mouse( (widget_struct *) callback_data );

    remove_global_event_callback( LEFT_MOUSE_UP_EVENT, done_moving_slider,
                                  callback_data );

    remove_global_event_callback( NO_EVENT, update_slider_position_event,
                                  callback_data );

    set_interaction_in_progress( FALSE );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( update_both_position_event )
{
    update_both_slider_position_from_mouse( (widget_struct *) callback_data );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( done_moving_both_sliders )
{
    update_both_slider_position_from_mouse( (widget_struct *) callback_data );

    remove_global_event_callback( MIDDLE_MOUSE_UP_EVENT,
                                  done_moving_both_sliders,
                                  callback_data );

    remove_global_event_callback( NO_EVENT, update_both_position_event,
                                  callback_data );

    set_interaction_in_progress( FALSE );
}

static  void  select_slider(
    widget_struct   *widget,
    int             ind )
{
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    slider->peg_being_moved = ind;

    add_global_event_callback( LEFT_MOUSE_UP_EVENT, done_moving_slider,
                               ANY_MODIFIER, (void *) widget );

    add_global_event_callback( NO_EVENT, update_slider_position_event,
                               ANY_MODIFIER, (void *) widget );

    set_interaction_in_progress( TRUE );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( select_upper_slider )
{
    select_slider( (widget_struct *) callback_data, 1 );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( select_lower_slider )
{
    select_slider( (widget_struct *) callback_data, 0 );
}

static  void  select_colour_bar_sliders(
    widget_struct   *widget )
{
    int             x_mouse, y_mouse;
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    get_viewport_mouse_position( widget->graphics, widget->viewport_index,
                                 &x_mouse, &y_mouse);

    slider->middle_mouse_offset = convert_slider_value_to_position( slider,
           widget->x_size, slider->values[0] ) - (x_mouse - widget->x);

    add_global_event_callback( MIDDLE_MOUSE_UP_EVENT, done_moving_both_sliders,
                               ANY_MODIFIER, (void *) widget );

    add_global_event_callback( NO_EVENT, update_both_position_event,
                               ANY_MODIFIER, (void *) widget );

    set_interaction_in_progress( TRUE );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( select_both_sliders )
{
    select_colour_bar_sliders( (widget_struct *) callback_data );
}

static  void  update_one_slider_colours(
    slider_struct   *slider,
    int             ind,
    VIO_BOOL         use_ui_colours,
    VIO_BOOL         colour_map_state )
{
    if( use_ui_colours )
    {
        slider->pegs[ind]->colours[0] = get_ui_colour(colour_map_state,
                                      (UI_colours) slider->peg_colour);
    }
    else
    {
        slider->pegs[ind]->colours[0] = slider->peg_colour;
    }

    update_widget_colours( slider->text_widgets[ind] );
}

  void  update_slider_colours(
    widget_struct   *widget )
{
    VIO_Colour         rectangle_colour;
    slider_struct  *slider;

    slider = get_widget_slider( widget );

    if( widget->active_flag )
        rectangle_colour = slider->active_colour;
    else
        rectangle_colour = slider->inactive_colour;

    if( widget->use_ui_colours )
        slider->polygons->colours[0] = get_ui_colour(FALSE,
                                               (UI_colours) rectangle_colour);
    else
        slider->polygons->colours[0] = rectangle_colour;

    update_one_slider_colours( slider, 0, widget->use_ui_colours, FALSE );

    if( slider->colour_bar_flag )
        update_one_slider_colours( slider, 1, widget->use_ui_colours, FALSE );
}

static  VIO_Real  convert_position_to_slider_value(
    slider_struct *slider,
    int           x_position,
    int           x_size )
{
    VIO_Real  value;

    value = slider->min_value + (slider->max_value - slider->min_value) *
            ((VIO_Real) x_position) / (VIO_Real) (x_size-1);

    return( value );
}

static  int  convert_slider_value_to_position(
    slider_struct *slider,
    int           x_size,
    VIO_Real          value )
{
    VIO_Real  ratio;
    int   x_position;

    ratio = (value - slider->min_value) /
            (slider->max_value - slider->min_value);

    if( ratio <= 0.0 )
        x_position = 0;
    else if( ratio >= 1.0 )
        x_position = x_size - 1;
    else
        x_position = (int) ((VIO_Real) (x_size-1) * ratio);

    return( x_position );
}

static  void  update_one_slider_position(
    widget_struct           *widget,
    int                     ind )
{
    int                   x, peg_x1, peg_x2, text_entry_pos;
    slider_struct         *slider;
    event_function_type   callback;

    slider = get_widget_slider( widget );

    x = widget->x +
        convert_slider_value_to_position( slider, widget->x_size,
                                          slider->values[ind] );

    peg_x1 = x - VIO_ROUND( slider->peg_width / 2.0 );
    peg_x2 = peg_x1 + (int) slider->peg_width - 1;

    position_rectangle( slider->pegs[ind], peg_x1, widget->y,
                        (int) slider->peg_width, widget->y_size );

    if( ind == 0 )
    {
        text_entry_pos = x - Slider_text_entry_x_offset -
                         slider->text_widgets[ind]->x_size/2;
        callback = select_lower_slider;
    }
    else
    {
        text_entry_pos = x - Slider_text_entry_x_offset -
          slider->text_widgets[ind]->x_size/2;
        callback = select_upper_slider;
    }

    set_event_viewport_callback_viewport(
                   &widget->graphics->event_viewports,
                   widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                   callback,
                   (void *) widget,
                   peg_x1,    peg_x2,
                   widget->y, widget->y + widget->y_size - 1 );

    set_text_entry_real_value( slider->text_widgets[ind], slider->format_string,
                               slider->values[ind] );

    position_widget( slider->text_widgets[ind],
                     text_entry_pos, widget->y + Slider_text_entry_y_offset );
}

static  void  update_slider_position(
    widget_struct           *widget )
{
    update_one_slider_position( widget, 0 );
    if( get_widget_slider(widget)->colour_bar_flag )
        update_one_slider_position( widget, 1 );
}

  void  get_slider_values(
    widget_struct           *widget,
    VIO_Real                    *low_value,
    VIO_Real                    *high_value )
{
    slider_struct    *slider;

    slider = get_widget_slider( widget );

    *low_value = slider->values[0];

    if( slider->colour_bar_flag )
        *high_value = slider->values[1];
}

  void  set_slider_values(
    widget_struct           *widget,
    VIO_Real                    low_value,
    VIO_Real                    high_value )
{
    VIO_BOOL          changed[2];
    slider_struct    *slider;

    changed[0] = FALSE;
    changed[1] = FALSE;

    slider = get_widget_slider( widget );

    if( !slider->values_allowed_outside_range )
    {
        if( low_value < slider->min_value )
            low_value = slider->min_value;
        else if( low_value > slider->max_value )
            low_value = slider->max_value;
    }

    if( low_value != slider->values[0] )
    {
        slider->values[0] = low_value;
        changed[0] = TRUE;
    }

    if( slider->colour_bar_flag )
    {
        if( !slider->values_allowed_outside_range )
        {
            if( high_value < slider->min_value )
                high_value = slider->min_value;
            else if( high_value > slider->max_value )
                high_value = slider->max_value;
        }

        if( high_value < low_value )
            high_value = low_value;

        if( high_value != slider->values[1] )
        {
            slider->values[1] = high_value;
            changed[1] = TRUE;
        }
    }

    if( changed[0] || changed[1] )
    {
        update_slider_position( widget );

        set_viewport_update_flag( &widget->graphics->graphics,
                                  (int) widget->viewport_index, NORMAL_PLANES );
    }

    if( changed[0] )
        slider->value_changed_callback[0]( widget, slider->callback_data[0] );

    if( changed[1] )
        slider->value_changed_callback[1]( widget, slider->callback_data[1] );
}

  void  set_slider_limits(
    widget_struct  *widget,
    VIO_Real           min_value,
    VIO_Real           max_value )
{
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    slider->min_value = min_value;
    slider->max_value = max_value;
}

  void  update_slider_activity(
    widget_struct           *widget )
{
    slider_struct    *slider;

    slider = get_widget_slider( widget );

    update_slider_colours( widget );

    set_event_viewport_callback_enabled(
                       &widget->graphics->event_viewports,
                       widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                       select_lower_slider, (void *) widget,
                       widget->active_flag );

    set_widget_activity( slider->text_widgets[0], widget->active_flag );

    if( slider->colour_bar_flag )
    {
        set_event_viewport_callback_enabled(
                           &widget->graphics->event_viewports,
                           widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                           select_upper_slider, (void *) widget,
                           widget->active_flag );

        set_event_viewport_callback_enabled(
                           &widget->graphics->event_viewports,
                           widget->viewport_index, MIDDLE_MOUSE_DOWN_EVENT,
                           select_both_sliders, (void *) widget,
                           widget->active_flag );

        set_widget_activity( slider->text_widgets[1], widget->active_flag );
    }
}

/* ARGSUSED */

  void  position_slider(
    widget_struct                 *widget,
    int                           x,
    int                           y )
{
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    position_rectangle( slider->polygons, widget->x, widget->y,
                        widget->x_size, widget->y_size );

    update_slider_position( widget );
}

static  void  create_slider_graphics(
    widget_struct    *widget )
{
    object_struct     *object;
    slider_struct     *slider;

    slider = get_widget_slider( widget );

    object = create_rectangle( slider->active_colour );
    slider->polygons = get_polygons_ptr( object );

    add_object_to_viewport( &widget->graphics->graphics,
                            (int) widget->viewport_index, NORMAL_PLANES,
                            object );

    object = create_rectangle( slider->peg_colour );
    slider->pegs[0] = get_polygons_ptr( object );

    add_object_to_viewport( &widget->graphics->graphics,
                            (int) widget->viewport_index, NORMAL_PLANES,
                            object );

    if( slider->colour_bar_flag )
    {
        object = create_rectangle( slider->peg_colour );
        slider->pegs[1] = get_polygons_ptr( object );

        add_object_to_viewport( &widget->graphics->graphics,
                                (int) widget->viewport_index, NORMAL_PLANES,
                                object );
    }
}

/* ARGSUSED */

  void  delete_slider(
    widget_struct  *widget )
{
    slider_struct     *slider;

    slider = get_widget_slider( widget );

    delete_string( slider->format_string );

    delete_widget( slider->text_widgets[0] );

    if( slider->colour_bar_flag )
        delete_widget( slider->text_widgets[1] );
}

static  widget_struct  *create_a_slider(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_BOOL                    colour_bar_flag,
    VIO_Real                       initial_low_value,
    VIO_Real                       initial_high_value,
    VIO_Real                       min_value,
    VIO_Real                       max_value,
    VIO_BOOL                    values_allowed_outside_range,
    VIO_STR                     format_string,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     peg_colour,
    widget_callback_type       lower_value_callback,
    void                       *lower_value_callback_data,
    widget_callback_type       upper_value_callback,
    void                       *upper_value_callback_data )
{
    widget_struct      *widget;
    slider_struct      *slider;

    widget = create_widget( SLIDER, x, y, x_size, y_size, initial_activity,
                            TRUE, graphics, viewport_index );

    slider = get_widget_slider( widget );

    slider->peg_width = (VIO_Real) Slider_text_peg_width;

    slider->active_colour = active_colour;
    slider->inactive_colour = inactive_colour;
    slider->peg_colour = peg_colour;

    slider->min_value = min_value;
    slider->max_value = max_value;
    slider->values_allowed_outside_range = values_allowed_outside_range;
    slider->values[0] = initial_low_value;
    slider->format_string = create_string( format_string );

    slider->colour_bar_flag = colour_bar_flag;
    slider->value_changed_callback[0] = lower_value_callback;
    slider->callback_data[0] = lower_value_callback_data;

    add_event_viewport_callback( &graphics->event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 select_lower_slider, ANY_MODIFIER,
                                 (void *) widget );

    slider->text_widgets[0] = (void *) create_text_entry( graphics,
                       viewport_index,
                       0, 0, Slider_text_width, Slider_text_height,
                       TRUE, "", TRUE,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Slider_text_font, Slider_text_font_size,
                       lower_value_hit_return_callback,
                       (void *) widget );

    if( colour_bar_flag )
    {
        slider->values[1] = initial_high_value;
        slider->value_changed_callback[1] = upper_value_callback;
        slider->callback_data[1] = upper_value_callback_data;
        add_event_viewport_callback( &graphics->event_viewports,
                                     viewport_index,
                                     LEFT_MOUSE_DOWN_EVENT,
                                     x, x + x_size - 1, y, y + y_size - 1,
                                     select_upper_slider, ANY_MODIFIER,
                                     (void *) widget );

        slider->text_widgets[1] = (void *) create_text_entry( graphics,
                       viewport_index,
                       0, 0, Slider_text_width, Slider_text_height,
                       TRUE, "", TRUE,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Slider_text_font, Slider_text_font_size,
                       upper_value_hit_return_callback,
                       (void *) widget );

        add_event_viewport_callback( &graphics->event_viewports,
                                     viewport_index,
                                     MIDDLE_MOUSE_DOWN_EVENT,
                                     x, x + x_size - 1, y, y + y_size - 1,
                                     select_both_sliders, ANY_MODIFIER,
                                     (void *) widget );

    }

    create_slider_graphics( widget );

    update_slider_activity( widget );

    update_slider_colours( widget );

    position_slider( widget, x, y );

    return( widget );
}

  widget_struct  *create_slider(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_Real                       initial_value,
    VIO_Real                       min_value,
    VIO_Real                       max_value,
    VIO_STR                     format_string,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     peg_colour,
    widget_callback_type       value_changed_callback,
    void                       *value_changed_data )
{
    return( create_a_slider( graphics, viewport_index,
                     x, y, x_size, y_size,
                     FALSE, initial_value, 0.0, min_value, max_value, FALSE,
                     format_string, initial_activity,
                     active_colour, inactive_colour, peg_colour,
                     value_changed_callback, value_changed_data,
                     (widget_callback_type) 0, (void *) NULL ) );
}

  widget_struct  *create_colour_bar_slider(
    graphics_window_struct     *graphics,
    Viewport_types             viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    VIO_Real                       initial_low_value,
    VIO_Real                       initial_high_value,
    VIO_Real                       min_value,
    VIO_Real                       max_value,
    VIO_STR                     format_string,
    VIO_BOOL                    initial_activity,
    VIO_Colour                     active_colour,
    VIO_Colour                     inactive_colour,
    VIO_Colour                     peg_colour,
    widget_callback_type       lower_value_changed_callback,
    void                       *lower_value_callback_data,
    widget_callback_type       upper_value_changed_callback,
    void                       *upper_value_callback_data )
{
    return( create_a_slider( graphics, viewport_index,
                    x, y, x_size, y_size,
                    TRUE, initial_low_value, initial_high_value,
                    min_value, max_value, TRUE, format_string,
                    initial_activity,
                    active_colour, inactive_colour, peg_colour,
                    lower_value_changed_callback, lower_value_callback_data,
                    upper_value_changed_callback, upper_value_callback_data ) );
}

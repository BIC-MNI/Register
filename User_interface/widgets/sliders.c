#include  <def_user_interface.h>

private  Real  convert_position_to_slider_value( slider_struct *, int, int );

private  void  change_slider_value(
    widget_struct  *widget,
    int            ind,
    Real           value )
{
    Real            values[2];
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    values[0] = slider->values[0];
    values[1] = slider->values[1];

    if( slider->colour_bar_flag )
    {
        if( ind == 0 && value > slider->values[1] )
            value = slider->values[1];
        else if( ind == 1 && value < slider->values[0] )
            value = slider->values[0];
    }

    values[ind] = value;

    if( set_slider_values( widget, values[0], values[1] ) )
    {
        set_viewport_update_flag( &widget->graphics->graphics,
                                  widget->viewport_index, NORMAL_PLANES );

        slider->value_changed_callback[ind]( widget,
                                             slider->callback_data[ind] );
    }
}

private  void  update_value_from_text(
    widget_struct  *widget,
    int            ind )
{
    Real            value;
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    if( get_text_entry_real_value( slider->text_widgets[ind], &value ) )
    {
        change_slider_value( widget, ind, value );
    }
}

private  DEFINE_WIDGET_CALLBACK( upper_value_hit_return_callback )/* ARGSUSED */
{
    update_value_from_text( (widget_struct *) callback_data, 1 );
}

private  DEFINE_WIDGET_CALLBACK( lower_value_hit_return_callback )/* ARGSUSED */
{
    update_value_from_text( (widget_struct *) callback_data, 0 );
}

private  void  update_slider_position_from_mouse(
    widget_struct  *widget )
{
    int             ind, x_mouse, y_mouse;
    Real            value;
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    ind = slider->peg_being_moved;

    get_viewport_mouse_position( widget->graphics, widget->viewport_index,
                                 &x_mouse, &y_mouse);

    value = convert_position_to_slider_value( slider, x_mouse - widget->x,
                                              widget->x_size );

    change_slider_value( widget, ind, value );
}

private  DEFINE_EVENT_FUNCTION( update_slider_position_event )   /* ARGSUSED */
{
    update_slider_position_from_mouse( (widget_struct *) callback_data );
}

private  DEFINE_EVENT_FUNCTION( done_moving_slider )   /* ARGSUSED */
{
    update_slider_position_from_mouse( (widget_struct *) callback_data );

    remove_global_event_callback( LEFT_MOUSE_UP_EVENT, done_moving_slider,
                                  callback_data );

    remove_global_event_callback( NO_EVENT, update_slider_position_event,
                                  callback_data );

    set_interaction_in_progress( FALSE );
}

private  void  select_slider(
    widget_struct   *widget,
    int             ind )
{
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    slider->peg_being_moved = ind;

    add_global_event_callback( LEFT_MOUSE_UP_EVENT, done_moving_slider,
                               (void *) widget );

    add_global_event_callback( NO_EVENT, update_slider_position_event,
                               (void *) widget );

    set_interaction_in_progress( TRUE );
}

private  DEFINE_EVENT_FUNCTION( select_upper_slider )   /* ARGSUSED */
{
    select_slider( (widget_struct *) callback_data, 1 );
}

private  DEFINE_EVENT_FUNCTION( select_lower_slider )   /* ARGSUSED */
{
    select_slider( (widget_struct *) callback_data, 0 );
}

private  void  update_one_slider_colours(
    slider_struct   *slider,
    int             ind,
    Boolean         colour_map_state )
{
    slider->pegs[ind]->colours[0] = get_ui_colour(colour_map_state,
                                                  slider->peg_colour);

    update_widget_colours( slider->text_widgets[ind] );
}

public  void  update_slider_colours(
    widget_struct   *widget )
{
    UI_colours     rectangle_colour;
    Boolean        colour_map_state;
    slider_struct  *slider;

    colour_map_state = G_get_colour_map_state( widget->graphics->window );

    slider = get_widget_slider( widget );

    if( widget->active_flag )
        rectangle_colour = slider->active_colour;
    else
        rectangle_colour = slider->inactive_colour;

    slider->polygons->colours[0] = get_ui_colour(colour_map_state,
                                                 rectangle_colour);

    update_one_slider_colours( slider, 0, colour_map_state );

    if( slider->colour_bar_flag )
        update_one_slider_colours( slider, 1, colour_map_state );
}

private  Real  convert_position_to_slider_value(
    slider_struct *slider,
    int           x_position,
    int           x_size )
{
    Real  value;

    value = slider->min_value + (slider->max_value - slider->min_value) *
            ((Real) x_position) / (Real) (x_size-1);

    return( value );
}

private  int  convert_slider_value_to_position(
    slider_struct *slider,
    int           x_size,
    Real          value )
{
    int   x_position;

    x_position = (int) ((Real) (x_size-1) * (value - slider->min_value) /
                                 (slider->max_value - slider->min_value) );

    return( x_position );
}

private  void  update_one_slider_position(
    widget_struct           *widget,
    int                     ind )
{
    int                   x, peg_x1, peg_x2, text_entry_pos;
    slider_struct         *slider;
    widget_callback_type  callback;
    String                position_string;

    slider = get_widget_slider( widget );

    x = widget->x +
        convert_slider_value_to_position( slider, widget->x_size,
                                          slider->values[ind] );

    peg_x1 = x - slider->peg_width / 2;
    peg_x2 = peg_x1 + slider->peg_width - 1;

    position_rectangle( slider->pegs[ind], peg_x1, widget->y,
                        slider->peg_width, widget->y_size );

    if( ind == 0 )
    {
        text_entry_pos = x - Slider_text_entry_x_offset -
                         slider->text_widgets[ind]->x_size;
        callback = select_lower_slider;
    }
    else
    {
        text_entry_pos = x + Slider_text_entry_x_offset;
        callback = select_upper_slider;
    }

    set_event_viewport_callback_viewport(
                   &widget->graphics->event_viewports,
                   widget->viewport_index, LEFT_MOUSE_DOWN_EVENT,
                   callback,
                   (void *) widget,
                   peg_x1,    peg_x2,
                   widget->y, widget->y + widget->y_size - 1 );

    (void) sprintf( position_string, Slider_text_format, slider->values[ind] );

    set_text_entry_string( slider->text_widgets[ind], position_string );

    position_widget( slider->text_widgets[ind],
                     text_entry_pos, widget->y + Slider_text_entry_y_offset );
}

private  void  update_slider_position(
    widget_struct           *widget )
{
    update_one_slider_position( widget, 0 );
    if( get_widget_slider(widget)->colour_bar_flag )
        update_one_slider_position( widget, 1 );
}

public  void  get_slider_values(
    widget_struct           *widget,
    Real                    *low_value,
    Real                    *high_value )
{
    slider_struct    *slider;

    slider = get_widget_slider( widget );

    *low_value = slider->values[0];

    if( slider->colour_bar_flag )
        *high_value = slider->values[1];
}

public  Boolean  set_slider_values(
    widget_struct           *widget,
    Real                    low_value,
    Real                    high_value )
{
    Boolean          changed;
    slider_struct    *slider;

    changed = FALSE;

    slider = get_widget_slider( widget );

    if( low_value != slider->values[0] &&
        low_value >= slider->min_value && low_value <= slider->max_value )
    {
        slider->values[0] = low_value;
        changed = TRUE;
    }

    if( slider->colour_bar_flag &&
        high_value != slider->values[1] &&
        high_value >= slider->min_value && high_value <= slider->max_value )
    {
        if( high_value < low_value )  high_value = low_value;
        slider->values[1] = high_value;
        changed = TRUE;
    }

    update_slider_position( widget );

    return( changed );
}

public  void  update_slider_activity(
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

        set_widget_activity( slider->text_widgets[1], widget->active_flag );
    }
}

public  void  position_slider(
    widget_struct                 *widget,
    int                           x,
    int                           y )    /* ARGSUSED */
{
    slider_struct   *slider;

    slider = get_widget_slider( widget );

    position_rectangle( slider->polygons, widget->x, widget->y,
                        widget->x_size, widget->y_size );

    update_slider_position( widget );
}

private  void  create_slider_graphics(
    widget_struct    *widget )
{
    object_struct     *object;
    slider_struct     *slider;

    slider = get_widget_slider( widget );

    object = create_rectangle( slider->active_colour );
    slider->polygons = (polygons_struct *) get_object_pointer( object );

    add_object_to_viewport( &widget->graphics->graphics,
                            widget->viewport_index, NORMAL_PLANES, object );

    object = create_rectangle( slider->peg_colour );
    slider->pegs[0] = (polygons_struct *) get_object_pointer( object );

    add_object_to_viewport( &widget->graphics->graphics,
                            widget->viewport_index, NORMAL_PLANES, object );

    if( slider->colour_bar_flag )
    {
        object = create_rectangle( slider->peg_colour );
        slider->pegs[1] = (polygons_struct *) get_object_pointer( object );

        add_object_to_viewport( &widget->graphics->graphics,
                                widget->viewport_index, NORMAL_PLANES, object );
    }
}

public  void  delete_slider(
    widget_struct  *widget )      /* ARGSUSED */
{
    slider_struct     *slider;

    slider = get_widget_slider( widget );

    delete_widget( slider->text_widgets[0] );

    if( slider->colour_bar_flag )
        delete_widget( slider->text_widgets[1] );
}

private  widget_struct  *create_a_slider(
    graphics_window_struct     *graphics,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    Boolean                    colour_bar_flag,
    Real                       initial_low_value,
    Real                       initial_high_value,
    Real                       min_value,
    Real                       max_value,
    Boolean                    initial_activity,
    UI_colours                 active_colour,
    UI_colours                 inactive_colour,
    UI_colours                 peg_colour,
    widget_callback_type       lower_value_callback,
    void                       *lower_value_callback_data,
    widget_callback_type       upper_value_callback,
    void                       *upper_value_callback_data )
{
    widget_struct      *widget;
    slider_struct      *slider;

    widget = create_widget( SLIDER, x, y, x_size, y_size, initial_activity,
                            graphics, viewport_index );

    slider = get_widget_slider( widget );

    slider->peg_width = Slider_text_peg_width;

    slider->active_colour = active_colour;
    slider->inactive_colour = inactive_colour;
    slider->peg_colour = peg_colour;

    slider->min_value = min_value;
    slider->max_value = max_value;
    slider->values[0] = initial_low_value;

    slider->colour_bar_flag = colour_bar_flag;
    slider->value_changed_callback[0] = lower_value_callback;
    slider->callback_data[0] = lower_value_callback_data;

    add_event_viewport_callback( &graphics->event_viewports,
                                 viewport_index,
                                 LEFT_MOUSE_DOWN_EVENT,
                                 x, x + x_size - 1, y, y + y_size - 1,
                                 select_lower_slider,
                                 (void *) widget );

    slider->text_widgets[0] = (void *) create_text_entry( graphics,
                       viewport_index,
                       0, 0, Slider_text_width, Slider_text_height,
                       "", ON,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Slider_text_font, Slider_text_font_size,
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
                                     select_upper_slider,
                                     (void *) widget );

        slider->text_widgets[1] = (void *) create_text_entry( graphics,
                       viewport_index,
                       0, 0, Slider_text_width, Slider_text_height,
                       "", ON,
                       TEXT_ENTRY_ACTIVE_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       Slider_text_font, Slider_text_font_size,
                       upper_value_hit_return_callback,
                       (void *) widget );
    }

    create_slider_graphics( widget );

    update_slider_activity( widget );

    update_slider_colours( widget );

    position_slider( widget, x, y );

    return( widget );
}

public  widget_struct  *create_slider(
    graphics_window_struct     *graphics,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    Real                       initial_value,
    Real                       min_value,
    Real                       max_value,
    Boolean                    initial_activity,
    UI_colours                 active_colour,
    UI_colours                 inactive_colour,
    UI_colours                 peg_colour,
    widget_callback_type       value_changed_callback,
    void                       *value_changed_data )
{
    return( create_a_slider( graphics, viewport_index,
                     x, y, x_size, y_size,
                     FALSE, initial_value, 0.0, min_value, max_value,
                     initial_activity,
                     active_colour, inactive_colour, peg_colour,
                     value_changed_callback, value_changed_data,
                     (widget_callback_type) 0, (void *) NULL ) );
}

public  widget_struct  *create_colour_bar_slider(
    graphics_window_struct     *graphics,
    int                        viewport_index,
    int                        x,
    int                        y,
    int                        x_size,
    int                        y_size,
    Real                       initial_low_value,
    Real                       initial_high_value,
    Real                       min_value,
    Real                       max_value,
    Boolean                    initial_activity,
    UI_colours                 active_colour,
    UI_colours                 inactive_colour,
    UI_colours                 peg_colour,
    widget_callback_type       lower_value_changed_callback,
    void                       *lower_value_callback_data,
    widget_callback_type       upper_value_changed_callback,
    void                       *upper_value_callback_data )
{
    return( create_a_slider( graphics, viewport_index,
                    x, y, x_size, y_size,
                    TRUE, initial_low_value, initial_high_value,
                    min_value, max_value,
                    initial_activity,
                    active_colour, inactive_colour, peg_colour,
                    lower_value_changed_callback, lower_value_callback_data,
                    upper_value_changed_callback, upper_value_callback_data ) );
}

#include  <def_user_interface.h>

public  void  initialize_widget_list(
    widgets_struct   *widget_list )
{
    widget_list->n_widgets = 0;
}

public  void  delete_widget_list(
    widgets_struct   *widget_list )
{
    int   i;

    if( widget_list->n_widgets > 0 )
    {
        for_less( i, 0, widget_list->n_widgets )
            FREE( widget_list->widgets[i] );

        FREE( widget_list->widgets );
    }
}

public  int  add_widget_to_list(
    widgets_struct   *widget_list,
    widget_struct    *widget )
{
    int   widget_index;

    widget_index = widget_list->n_widgets;

    ADD_ELEMENT_TO_ARRAY( widget_list->n_widgets, widget_list->widgets,
                          widget, DEFAULT_CHUNK_SIZE );

    return( widget_index );
}

public  void  delete_nth_widget_from_list(
    widgets_struct   *widget_list,
    int              index )
{
    DELETE_ELEMENT_FROM_ARRAY( widget_list->n_widgets, widget_list->widgets,
                               index, DEFAULT_CHUNK_SIZE );
}

public  widget_struct  *create_widget(
    Widget_types    type,
    int             x,
    int             y,
    int             x_size,
    int             y_size,
    Boolean         initial_activity,
    int             viewport_index )
{
    widget_struct   *widget;

    ALLOC( widget, 1 );
    widget->widget_type = type;
    widget->x = x;
    widget->y = y;
    widget->x_size = x_size;
    widget->y_size = y_size;
    widget->active_flag = initial_activity;
    widget->viewport_index = viewport_index;

    return( widget );
}

public  int  get_widget_height(
    widget_struct   *widget )
{
    return( widget->y_size );
}

/* -------------------- function lookup ----------------------------- */

/* -------------------- position function ----------------------------- */

private  void  position_button_widget(
    widget_struct            *widget,
    event_viewports_struct   *event_viewports,
    int                      x,
    int                      y )
{
    position_button( widget, event_viewports, x, y );
}

private  void  position_text_entry_widget(
    widget_struct            *widget,
    event_viewports_struct   *event_viewports,
    int                      x,
    int                      y )
{
    position_text_entry( widget, event_viewports, x, y );
}

private  void  position_slider_widget(
    widget_struct            *widget,
    event_viewports_struct   *event_viewports,
    int                      x,
    int                      y )
{
    position_slider( widget, event_viewports, x, y );
}

/* -------------------- update colours function ----------------------------- */

private  void  update_colours_button_widget(
    widget_struct            *widget,
    Boolean                  colour_map_state )
{
    update_button_colours( widget, colour_map_state );
}

private  void  update_colours_text_entry_widget(
    widget_struct            *widget,
    Boolean                  colour_map_state )
{
    update_text_entry_colours( widget, colour_map_state );
}

private  void  update_colours_slider_widget(
    widget_struct            *widget,
    Boolean                  colour_map_state )
{
    update_slider_colours( widget, colour_map_state );
}

/* -------------------------------------------------------------------------- */

typedef  struct
{
    void   (*position_function)( widget_struct *, event_viewports_struct *,
                                 int, int );
    void   (*update_activity)( widget_struct *, event_viewports_struct *,
                               Boolean );
    void   (*update_colours)( widget_struct *, Boolean );
}
widget_functions_list;

static  widget_functions_list   widget_functions[N_WIDGET_TYPES] =
{
    {
        position_button_widget,
        update_button_activity,
        update_colours_button_widget
    },                                                    /* BUTTON */

    {
        position_text_entry_widget,
        update_text_entry_activity,
        update_colours_text_entry_widget
    },                                                    /* TEXT_ENTRY */

    {
        position_slider_widget,
        update_slider_activity,
        update_colours_slider_widget
    }                                                     /* TEXT_ENTRY */
};

/* ------------------------------------------------------------------- */

public  void  position_widget(
    widget_struct           *widget,
    event_viewports_struct  *event_viewports,
    int                     x,
    int                     y )
{
    widget->x = x;
    widget->y = y;
    widget_functions[widget->widget_type].position_function( widget,
                    event_viewports, x, y );
}

public  button_struct  *get_widget_button(
    widget_struct  *widget )
{
    return( &widget->specific.button );
}

public  text_entry_struct  *get_widget_text_entry(
    widget_struct  *widget )
{
    return( &widget->specific.text_entry );
}

public  slider_struct  *get_widget_slider(
    widget_struct  *widget )
{
    return( &widget->specific.slider );
}

public  void  set_widget_activity(
    widget_struct           *widget,
    event_viewports_struct  *event_viewports,
    Boolean                 colour_map_state,
    Boolean                 activity )
{
    widget->active_flag = activity;

    widget_functions[widget->widget_type].update_activity( widget,
                               event_viewports, colour_map_state );
}

public  void  set_widget_activity_and_update(
    UI_struct      *ui_info,
    widget_struct  *widget,
    Boolean        activity )
{
    set_widget_activity( widget,
                &ui_info->graphics_window.event_viewports,
                G_get_colour_map_state(ui_info->graphics_window.window),
                activity );

    set_viewport_update_flag( &ui_info->graphics_window.graphics,
                              widget->viewport_index, NORMAL_PLANES );
}

public  void  update_widget_colours(
    widget_struct  *widget,
    Boolean        colour_map_state )
{
    widget_functions[widget->widget_type].update_colours( widget,
                                                          colour_map_state );
}

public  void  update_widget_list_colours(
    widgets_struct      *widget_list,
    Boolean             colour_map_state )
{
    int    i;

    for_less( i, 0, widget_list->n_widgets )
        update_widget_colours( widget_list->widgets[i], colour_map_state );
}

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
            delete_widget( widget_list->widgets[i] );

        FREE( widget_list->widgets );
    }
}

public  int  add_widget_to_list(
    widgets_struct   *widget_list,
    widget_struct    *widget )
{
    int   widget_index;

    widget_index = widget_list->n_widgets;

    ADD_ELEMENT_TO_ARRAY( widget_list->widgets, widget_list->n_widgets,
                          widget, DEFAULT_CHUNK_SIZE );

    return( widget_index );
}

public  void  delete_nth_widget_from_list(
    widgets_struct   *widget_list,
    int              index )
{
    DELETE_ELEMENT_FROM_ARRAY( widget_list->widgets, widget_list->n_widgets,
                               index, DEFAULT_CHUNK_SIZE );
}

public  widget_struct  *create_widget(
    Widget_types            type,
    int                     x,
    int                     y,
    int                     x_size,
    int                     y_size,
    Boolean                 initial_activity,
    graphics_window_struct  *graphics,
    int                     viewport_index )
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
    widget->graphics = graphics;
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
    int                      x,
    int                      y )
{
    position_button( widget, x, y );
}

private  void  position_text_entry_widget(
    widget_struct            *widget,
    int                      x,
    int                      y )
{
    position_text_entry( widget, x, y );
}

private  void  position_slider_widget(
    widget_struct            *widget,
    int                      x,
    int                      y )
{
    position_slider( widget, x, y );
}

/* -------------------- set selected function ----------------------------- */

private  void  update_selected_button_widget(
    widget_struct            *widget,
    Boolean                  selected )
{
    update_button_selected( widget );
}

private  void  update_selected_text_entry_widget(
    widget_struct            *widget,
    Boolean                  selected )
{
    update_text_entry_selected( widget );
}

private  void  update_selected_slider_widget(
    widget_struct            *widget,                 /* ARGSUSED */
    Boolean                  selected )
{
}

/* -------------------- update colours function ----------------------------- */

private  void  update_colours_button_widget(
    widget_struct            *widget )
{
    update_button_colours( widget );
}

private  void  update_colours_text_entry_widget(
    widget_struct            *widget )
{
    update_text_entry_colours( widget );
}

private  void  update_colours_slider_widget(
    widget_struct            *widget )
{
    update_slider_colours( widget );
}

/* -------------------- delete function ----------------------------- */

private  void  delete_button_widget(
    widget_struct            *widget )
{
    delete_button( widget );
}

private  void  delete_text_entry_widget(
    widget_struct            *widget )
{
    delete_text_entry( widget );
}

private  void  delete_slider_widget(
    widget_struct            *widget )
{
    delete_slider( widget );
}

/* -------------------------------------------------------------------------- */

typedef  struct
{
    void   (*position_function)( widget_struct *, int, int );
    void   (*update_activity)( widget_struct * );
    void   (*update_selected)( widget_struct * );
    void   (*update_colours)( widget_struct * );
    void   (*delete_function)( widget_struct * );
}
widget_functions_list;

static  widget_functions_list   widget_functions[N_WIDGET_TYPES] =
{
    {
        position_button_widget,
        update_button_activity,
        update_selected_button_widget,
        update_colours_button_widget,
        delete_button_widget
    },                                                    /* BUTTON */

    {
        position_text_entry_widget,
        update_text_entry_activity,
        update_selected_text_entry_widget,
        update_colours_text_entry_widget,
        delete_text_entry_widget
    },                                                    /* TEXT_ENTRY */

    {
        position_slider_widget,
        update_slider_activity,
        update_selected_slider_widget,
        update_colours_slider_widget,
        delete_slider_widget
    }                                                     /* TEXT_ENTRY */
};

/* ------------------------------------------------------------------- */

public  void  position_widget(
    widget_struct           *widget,
    int                     x,
    int                     y )
{
    widget->x = x;
    widget->y = y;
    widget_functions[widget->widget_type].position_function( widget, x, y );
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
    widget_struct  *widget,
    Boolean        activity )
{
    widget->active_flag = activity;

    widget_functions[widget->widget_type].update_activity( widget );

    set_viewport_update_flag( &widget->graphics->graphics,
                              widget->viewport_index, NORMAL_PLANES );
}

public  void  set_widget_selected(
    widget_struct  *widget,
    Boolean        selected )
{
    widget->selected_flag = selected;
    widget_functions[widget->widget_type].update_selected( widget );
}

public  void  update_widget_colours(
    widget_struct  *widget )
{
    widget_functions[widget->widget_type].update_colours( widget );
}

public  void  update_widget_list_colours(
    widgets_struct      *widget_list )
{
    int    i;

    for_less( i, 0, widget_list->n_widgets )
        update_widget_colours( widget_list->widgets[i] );
}

public  void  delete_widget(
    widget_struct  *widget )
{
    widget_functions[widget->widget_type].delete_function( widget );
    FREE( widget );
}

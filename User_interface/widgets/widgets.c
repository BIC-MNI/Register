#include  <def_user_interface.h>

public  void  initialize_widget_list(
    widgets_struct   *widget_list )
{
    widget_list->n_widgets = 0;
}

public  void  delete_widget_list(
    widgets_struct   *widget_list )
{
    if( widget_list->n_widgets > 0 )
    {
        FREE( widget_list->widgets );
    }
}

public  void  add_widget_to_list(
    widgets_struct   *widget_list,
    widget_struct    *widget )
{
    ADD_ELEMENT_TO_ARRAY( widget_list->n_widgets, widget_list->widgets,
                          widget, DEFAULT_CHUNK_SIZE );
}

public  void  delete_nth_widget_from_list(
    widgets_struct   *widget_list,
    int              index )
{
    DELETE_ELEMENT_FROM_ARRAY( widget_list->n_widgets, widget_list->widgets,
                               index, DEFAULT_CHUNK_SIZE );
}

public  widget_struct  *create_widget(
    Widget_types    type )
{
    widget_struct   *widget;

    ALLOC( widget, 1 );
    widget->widget_type = type;

    return( widget );
}

public  void  *get_widget_pointer(
    widget_struct   *widget )
{
    void  *ptr;

    switch( widget->widget_type )
    {
    case  BUTTON:   ptr = (void *) &widget->specific.button;   break;
    default:        ptr = (void *) 0;                          break;
    }

    return( ptr );
}

/* -------------------- function lookup ----------------------------- */

private  void  position_button_widget(
    widget_struct            *widget,
    event_viewports_struct  *event_viewports,
    int                      viewport_index,
    int                      x,
    int                      y )
{
    position_button( event_viewports, viewport_index,
                     (button_struct *) get_widget_pointer(widget), x, y );
}

typedef  struct
{
    void   (*position_function)( widget_struct *, event_viewports_struct *,
                                 int, int, int );
}
widget_functions_list;

static  widget_functions_list   widget_functions[N_WIDGET_TYPES] =
{
    {
        position_button_widget
    }                              /* BUTTON */
};

public  void  position_widget(
    widget_struct           *widget,
    event_viewports_struct  *event_viewports,
    int                     viewport_index,
    int                     x,
    int                     y )
{
    widget_functions[widget->widget_type].position_function( widget,
                    event_viewports, viewport_index, x, y );
}


#include  <def_user_interface.h>

#define  EVENT_CHUNK_ALLOC_SIZE   10

private  void  delete_event_callbacks( event_callback_list_struct * );

public  void  initialize_event_table( event_callback_list_struct  table[] )
{
    Event_types   event_type;

    for_enum( event_type, N_EVENT_TYPES, Event_types )
        table[event_type].n_callbacks = 0;
}

public  void  delete_event_table( event_callback_list_struct  table[] )
{
    Event_types   event_type;

    for_enum( event_type, N_EVENT_TYPES, Event_types )
        delete_event_callbacks( &table[event_type] );
}

private  int  get_callback_function_index(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data )
{
    int     i;

    for_less( i, 0, callback_list->n_callbacks )
    {
        if( callback_list->callbacks[i].callback == callback_function &&
            callback_list->callbacks[i].callback_data == callback_data )
            break;
    }

    if( i >= callback_list->n_callbacks )
    {
        HANDLE_INTERNAL_ERROR( "get_callback_function_index" );
        i = -1;
    }

    return( i );
}

public  void  set_event_callback_enabled(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data,
    Boolean                      enabled )
{
    int    i;

    i =  get_callback_function_index( callback_list, callback_function,
                                      callback_data );

    if( i >= 0 )
        callback_list->callbacks[i].enabled = enabled;
}

public  void  add_event_callback_function(
    event_callback_list_struct   *callback_list,
    int                          x_min,
    int                          x_max,
    int                          y_min,
    int                          y_max,
    event_function_type          callback_function,
    Event_modifiers              modifier,
    void                         *callback_data )
{
    event_callback_struct  callback;

    callback.enabled = TRUE;
    callback.x_min = x_min;
    callback.x_max = x_max;
    callback.y_min = y_min;
    callback.y_max = y_max;
    callback.callback = callback_function;
    callback.modifier = modifier;
    callback.callback_data = callback_data;

    ADD_ELEMENT_TO_ARRAY( callback_list->callbacks, callback_list->n_callbacks,
                          callback, EVENT_CHUNK_ALLOC_SIZE );
}

public  void  set_event_callback_viewport(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data,
    int                          x_min,
    int                          x_max,
    int                          y_min,
    int                          y_max )
{
    int     i;

    i =  get_callback_function_index( callback_list, callback_function,
                                      callback_data );

    if( i >= 0 )
    {
        callback_list->callbacks[i].x_min = x_min;
        callback_list->callbacks[i].x_max = x_max;
        callback_list->callbacks[i].y_min = y_min;
        callback_list->callbacks[i].y_max = y_max;
    }
}

public  void  remove_event_callback_function(
    event_callback_list_struct   *callback_list,
    event_function_type          callback_function,
    void                         *callback_data )
{
    int     i;

    i =  get_callback_function_index( callback_list, callback_function,
                                      callback_data );

    if( i >= 0 )
    {
        DELETE_ELEMENT_FROM_ARRAY( callback_list->callbacks,
                                   callback_list->n_callbacks,
                                   i, EVENT_CHUNK_ALLOC_SIZE );
    }
}

private  Boolean  is_correct_shift_modifier(
    Boolean           shift_state,
    Event_modifiers   modifier )
{
    switch( modifier )
    {
    case ANY_MODIFIER:    return( TRUE );
    case NO_SHIFT_ONLY:   return( !shift_state );
    case SHIFT_ONLY:      return( shift_state );
    default:              return( TRUE );
    }
}

public  Boolean  execute_event_callback_functions(
    Boolean                      shift_state,
    event_callback_list_struct   *callback_list,
    int                          mouse_x,
    int                          mouse_y,
    int                          event_viewport_index,
    int                          key_pressed )
{
    Boolean                 callback_found;
    event_callback_struct   *callback;
    int                     i;

    callback_found = FALSE;

    for_less( i, 0, callback_list->n_callbacks )
    {
        callback = &callback_list->callbacks[i];
        if( callback->enabled &&
            (callback->x_min < 0 ||   /* signals entire region */
             mouse_x >= callback->x_min &&
             mouse_x <= callback->x_max &&
             mouse_y >= callback->y_min &&
             mouse_y <= callback->y_max) &&
             is_correct_shift_modifier( shift_state, callback->modifier) )
        {
            callback->callback( event_viewport_index, key_pressed,
                                callback->callback_data );
            callback_found = TRUE;

            if( callback->x_min >= 0 )
                break;
        }
    }

    return( callback_found );
}

private  void  delete_event_callbacks(
    event_callback_list_struct   *callback_list )
{
    if( callback_list->n_callbacks > 0 )
    {
        FREE( callback_list->callbacks );
    }
}

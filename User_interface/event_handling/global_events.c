#include  <def_user_interface.h>

private  event_callback_list_struct    global_event_table[N_EVENT_TYPES];

private  Boolean   interaction_in_progress = FALSE;

private  Event_types  interaction_starting_events[] = {
                            LEFT_MOUSE_DOWN_EVENT,
                            MIDDLE_MOUSE_DOWN_EVENT,
                            RIGHT_MOUSE_DOWN_EVENT
                         };

private  Boolean      event_can_start_interaction[N_EVENT_TYPES];

public  Boolean  event_is_allowable(
    Event_types     event_type )
{
    static  Boolean  first = TRUE;
    int              i;
    Event_types      type;

    if( first )
    {
        first = FALSE;
        for_enum( type, N_EVENT_TYPES, Event_types )
            event_can_start_interaction[type] = FALSE;
        for_less( i, 0, SIZEOF_STATIC_ARRAY(interaction_starting_events) )
            event_can_start_interaction[interaction_starting_events[i]] = TRUE;
    }

    return( !interaction_in_progress ||
            !event_can_start_interaction[event_type] );
}

public  void  set_interaction_in_progress(
    Boolean  state )
{
    interaction_in_progress = state;
}

/* ------------------------------------------------------------------------ */

private  Event_types  in_window_events[] = {
                            KEY_DOWN_EVENT,
                            LEFT_MOUSE_DOWN_EVENT,
                            MIDDLE_MOUSE_DOWN_EVENT,
                            RIGHT_MOUSE_DOWN_EVENT
                         };

private  Boolean      event_must_be_in_window[N_EVENT_TYPES];

public  Boolean  mouse_must_be_in_window(
    Event_types     event_type )
{
    static  Boolean  first = TRUE;
    int              i;
    Event_types      type;

    if( first )
    {
        first = FALSE;
        for_enum( type, N_EVENT_TYPES, Event_types )
            event_must_be_in_window[type] = FALSE;
        for_less( i, 0, SIZEOF_STATIC_ARRAY(in_window_events) )
            event_must_be_in_window[in_window_events[i]] = TRUE;
    }

    return( event_must_be_in_window[event_type] );
}

public  void  add_global_event_callback(
    Event_types               event_type,
    event_function_type       callback,
    Event_modifiers           modifier,
    void                      *callback_data )
{
    add_event_callback_function( &global_event_table[event_type],
                                 -1, -1, -1, -1, callback, modifier,
                                 callback_data );
}

public  void  remove_global_event_callback(
    Event_types               event_type,
    event_function_type       callback,
    void                      *callback_data )
{
    remove_event_callback_function( &global_event_table[event_type], callback,
                                    callback_data );
}

public  Boolean  execute_global_event_callbacks(
    Boolean                 shift_state,
    Event_types             event_type,
    int                     key_pressed )
{
    return( execute_event_callback_functions( shift_state,
                                              &global_event_table[event_type],
                                              0, 0, 0,
                                              key_pressed ) );
}

public  void  initialize_global_events( void )
{
    initialize_event_table( global_event_table );
}

public  void  delete_global_events( void )
{
    delete_event_table( global_event_table );
}

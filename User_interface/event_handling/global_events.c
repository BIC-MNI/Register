#include  <def_user_interface.h>

private  event_callback_list_struct    global_event_table[NUM_EVENT_TYPES];

public  void  add_global_event_callback(
    Event_types               event_type,
    event_function_type       callback,
    void                      *callback_data )
{
    add_event_callback_function( &global_event_table[event_type],
                                 -1, -1, -1, -1, callback, callback_data );
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
    Event_types             event_type,
    int                     key_pressed )
{
    return( execute_event_callback_functions( &global_event_table[event_type],
                                              0, 0, 0,
                                              key_pressed ) );
}

public  void  initialize_global_events( void )
{
    initialize_event_table( global_event_table );
}

#ifndef  DEF_EVENTS
#define  DEF_EVENTS

#include  <def_mni.h>

typedef  void  (*event_function_type) ( int, int, void * );

#define  DEFINE_EVENT_FUNCTION( name )                                        \
                                                /* ARGSUSED */                \
         void   name( int          event_viewport_index,                      \
                      int          keyboard_character,                        \
                      void         *callback_data )

typedef  struct
{
    Boolean               enabled;
    int                   x_min, x_max, y_min, y_max;
    event_function_type   callback;
    void                  *callback_data;
} event_callback_struct;

typedef  struct
{
    int                     n_callbacks;
    event_callback_struct   *callbacks;
} event_callback_list_struct;

typedef  struct
{
    int                           x_min, x_max, y_min, y_max;
    event_callback_list_struct    table[N_EVENT_TYPES];
} event_viewport_struct;

typedef  struct
{
    int                    n_event_viewports;
    event_viewport_struct  *event_viewports;
} event_viewports_struct;

#endif

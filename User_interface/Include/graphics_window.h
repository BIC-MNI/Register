#ifndef  DEF_GRAPHICS_WINDOW
#define  DEF_GRAPHICS_WINDOW

#include  <def_mni.h>
#include  <def_events.h>

typedef struct
{
    window_struct           *window;
    event_viewports_struct  event_viewports;
    graphics_struct         graphics;
    int                     current_buffer;
} graphics_window_struct;

#endif

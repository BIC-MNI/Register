#include  <user_interface.h>

/* ARGSUSED */

public  void  save_current_viewport(
    UI_struct        *ui,
    Viewport_types   viewport )
{
    int   volume, view;

    ui_get_volume_view_index( viewport, &volume, &view );

    IF_save_image( volume, view );
}

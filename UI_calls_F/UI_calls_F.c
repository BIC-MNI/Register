#include  <def_UI_calls_F.h>

public  void  IF_initialize_register( window_struct *window )
{
    (void) initialize_register( window );
}

public  void  IF_terminate_register()
{
    terminate_register();
}

public  Boolean  IF_volume_is_loaded(
    int            volume_index )
{
    return( is_volume_loaded( get_main_struct(), volume_index ) );
}

public  void  IF_set_volume(
    int            volume_index,
    volume_struct  *volume )
{
    set_register_volume( get_main_struct(), volume_index, volume );
}

public  void  IF_delete_volume(
    int            volume_index )
{
    delete_register_volume( get_main_struct(), volume_index );
}

public  void  IF_set_interpolation_flag( Boolean  flag )
{
}

public  void  IF_set_recreate_slice_flag(
    int    volume,
    int    view )
{
    set_recreate_slice_flag( get_main_struct(), volume, view );
}

public  void  IF_set_update_slice_viewport_flag(
    int             volume,
    int             view,
    Bitplane_types  bitplane )
{
    set_update_slice_viewport_flag( get_main_struct(), volume, view,
                                    bitplane );
}

public  Boolean  IF_redraw_slices(
    window_struct   *window,
    int             current_buffer )
{
    return( update_slice_display( get_main_struct(), window, current_buffer ) );
}

public  void  IF_set_volume_position(
    int       volume,
    Real      position[] )
{
    set_volume_position( get_main_struct(), volume, position );
}

public  void  IF_get_volume_position(
    int       volume,
    Real      position[] )
{
    get_volume_position( get_main_struct(), volume, position );
}

public  void  IF_translate_slice(
    int       volume,
    int       view,
    int       x_translation,
    int       y_translation )
{
    translate_slice( get_main_struct(), volume, view,
                     x_translation, y_translation );
}

public  void  IF_set_slice_viewport(
    int       volume,
    int       view,
    int       x_min,
    int       x_max,
    int       y_min,
    int       y_max )
{
    set_slice_viewport( get_main_struct(), volume, view,
                        x_min, x_max, y_min, y_max );
}

public  Boolean  IF_convert_pixel_to_voxel(
    int    volume,
    int    view,
    int    x_pixel,
    int    y_pixel,
    Real   voxel_position[] )
{
    return( convert_pixel_to_voxel( get_main_struct(), volume, view,
                                    x_pixel, y_pixel, voxel_position ) );
}

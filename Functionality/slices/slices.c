
#include  <def_register.h>

private   int     x_axes[] = { Y, X, X };
private   int     y_axes[] = { Z, Z, Y };

private  void  check_axes_assigned()
{
    private  Boolean  first = TRUE;

    if( first )
    {
        first = FALSE;
        x_axes[0] = Slice_1_x_axis_index;
        x_axes[1] = Slice_2_x_axis_index;
        x_axes[2] = Slice_3_x_axis_index;
        y_axes[0] = Slice_1_y_axis_index;
        y_axes[1] = Slice_2_y_axis_index;
        y_axes[2] = Slice_3_y_axis_index;
    }
}

public  void  get_slice_axes(
    int   view_index,
    int   *x_axis_index,
    int   *y_axis_index )
{
    check_axes_assigned();

    *x_axis_index = x_axes[view_index];
    *y_axis_index = y_axes[view_index];
}

public  int  get_slice_axis(
    int   view_index )
{
    int   axis;

    check_axes_assigned();

    for_less( axis, 0, N_DIMENSIONS )
    {
        if( x_axes[view_index] != axis && 
            y_axes[view_index] != axis )
        {
            return( axis );
        }
    }

    return( 0 );
}

public  int  get_slice_viewport_index( int volume, int view )
{
    return( volume * N_VIEWS + view );
}

public  void  get_volume_view_index(
    int   viewport_index,
    int   *volume,
    int   *view )
{
    *volume = viewport_index / N_VIEWS;
    *view = viewport_index % N_VIEWS;
}

public  void  get_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_min,
    int          *x_max,
    int          *y_min,
    int          *y_max )
{
    get_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           x_min, x_max, y_min, y_max );
}

public  void  get_slice_viewport_size(
    main_struct  *main,
    int          volume,
    int          view,
    int          *x_size,
    int          *y_size )
{
    int   x_min, x_max, y_min, y_max;
    get_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           &x_min, &x_max, &y_min, &y_max );

    *x_size = x_max - x_min + 1;
    *y_size = y_max - y_min + 1;
}

public  void  set_slice_viewport(
    main_struct  *main,
    int          volume,
    int          view,
    int          x_min,
    int          x_max,
    int          y_min,
    int          y_max )
{
    set_graphics_viewport( &main->graphics,
                           get_slice_viewport_index(volume,view),
                           x_min, x_max, y_min, y_max );

    if( is_volume_active( main, volume ) )
        fit_slice_to_view( main, volume, view );
}

public  void  set_volume_voxel_position(
    main_struct    *main,
    int            volume_index,
    Real           position[N_DIMENSIONS] )
{
    Boolean        changed;
    volume_struct  *volume;
    int            view, axis;
    Real           pos;
    Real           *cursor_ptr;

    volume = get_slice_volume( main, volume_index );
    cursor_ptr = get_volume_cursor( main, volume_index );

    changed = FALSE;

    for_less( axis, 0, N_DIMENSIONS )
    {
        if( position[axis] < -0.5 )
            pos = -0.5;
        else if( position[axis] >
                 (Real) volume->sizes[axis] - 0.5 )
            pos = (Real) volume->sizes[axis] - 0.5;
        else
            pos = position[axis];

        if( pos != cursor_ptr[axis] )
        {
            changed = TRUE;

            cursor_ptr[axis] = pos;

            for_less( view, 0, N_VIEWS )
            {
                if( get_slice_axis( view ) == axis )
                {
                    set_recreate_slice_flag( main, volume_index, view );
                    update_slice_tag_objects( main, volume_index, view );
                }
            }
        }
    }

    if( changed )
        update_volume_cursors( main, volume_index );
}

public  void  get_volume_voxel_position(
    main_struct   *main,
    int           volume,
    Real          position[N_DIMENSIONS] )
{
    Real           *cursor_ptr;

    cursor_ptr = get_volume_cursor( main, volume );

    position[X] = cursor_ptr[X];
    position[Y] = cursor_ptr[Y];
    position[Z] = cursor_ptr[Z];
}

public  void  set_volume_world_position(
    main_struct    *main,
    int            volume_index,
    Real           world_position[N_DIMENSIONS] )
{
    Real           voxel_position[N_DIMENSIONS];
    volume_struct  *volume;

    volume = get_slice_volume( main, volume_index );

    convert_world_to_voxel( volume,
                            world_position[X],
                            world_position[Y],
                            world_position[Z],
                            &voxel_position[X],
                            &voxel_position[Y],
                            &voxel_position[Z] );

    set_volume_voxel_position( main, volume_index, voxel_position );
}

public  void  get_volume_world_position(
    main_struct   *main,
    int           volume_index,
    Real          world_position[N_DIMENSIONS] )
{
    Real           voxel_position[N_DIMENSIONS];
    volume_struct  *volume;

    volume = get_slice_volume( main, volume_index );

    get_volume_voxel_position( main, volume_index, voxel_position );

    convert_voxel_to_world( volume,
                            voxel_position[X],
                            voxel_position[Y],
                            voxel_position[Z],
                            &world_position[X],
                            &world_position[Y],
                            &world_position[Z] );
}

public  volume_struct  *get_slice_volume(
    main_struct   *main,
    int           volume_index )
{
    if( volume_index < N_VOLUMES )
        return( &main->trislice[volume_index].volume );
    else
        return( &main->trislice[volume_index-MERGED_VOLUME_INDEX].volume );
}

public  Real  *get_volume_cursor(
    main_struct   *main,
    int           volume_index )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( main->merged.position );
    else
        return( main->trislice[volume_index].position );
}

public  slice_struct  *get_slice_struct(
    main_struct   *main,
    int           volume_index,
    int           view )
{
    if( volume_index == MERGED_VOLUME_INDEX )
        return( &main->merged.slices[view] );
    else
        return( &main->trislice[volume_index].slices[view] );
}

public  void  get_slice_transform(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          *x_trans,
    Real          *y_trans,
    Real          *x_scale,
    Real          *y_scale )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    *x_trans = slice->x_translation;
    *y_trans = slice->y_translation;
    *x_scale = slice->x_scale;
    *y_scale = slice->y_scale;
}

public  void  set_slice_translation(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          x_trans,
    Real          y_trans )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->x_translation = x_trans;
    slice->y_translation = y_trans;
}

public  void  set_slice_scale(
    main_struct   *main,
    int           volume_index,
    int           view,
    Real          x_scale,
    Real          y_scale )
{
    slice_struct   *slice;

    slice = get_slice_struct( main, volume_index, view );

    slice->x_scale = x_scale;
    slice->y_scale = y_scale;
}

#include  <def_register.h>

private  void  initialize_slice( main_struct *, int, int );

public  void  initialize_slices( main_struct  *main )
{
    int            view, volume;

    for_less( volume, 0, N_VOLUMES )
    {
        main->trislice[volume].input_flag = FALSE;
        initialize_colour_coding( &main->trislice[volume].colour_coding,
                                  GRAY_SCALE,
                                  Initial_under_colour,
                                  Initial_over_colour,
                                  -0.5, (Real) N_VOXEL_VALUES - 0.5 );
    }

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
    {
        for_less( view, 0, N_VIEWS )
            initialize_slice( main, volume, view );
    }

    main->merged.active_flag = FALSE;

    ALLOC2D( main->merged.cmode_colour_map, N_VOXEL_VALUES, N_VOXEL_VALUES );

    ALLOC2D( main->merged.rgb_colour_map, N_VOXEL_VALUES, N_VOXEL_VALUES );

    for_less( volume, 0, N_MERGED )
    {
        initialize_colour_coding( &main->merged.colour_coding[volume],
                                  GRAY_SCALE,
                                  Initial_under_colour, Initial_over_colour,
                                  -0.5, (Real) N_VOXEL_VALUES - 0.5 );
    }

    main->merged.opacity[0] = Initial_merged_1_weight;
    main->merged.opacity[1] = Initial_merged_2_weight;

    main->merged.merge_method = (Merge_methods) Initial_merge_method;
}

private  void  initialize_slice(
    main_struct   *main,
    int           volume,
    int           view )
{
    object_struct   *object;
    slice_struct    *slice;

    slice = get_slice_struct( main, volume, view );

    slice->pixels_are_up_to_date = FALSE;
    slice->n_pixels_alloced = 0;
    slice->x_translation = 0.0;
    slice->y_translation = 0.0;
    slice->x_scale = 1.0;
    slice->y_scale = 1.0;

    set_graphics_viewport_background( &main->graphics,
                                      get_slice_viewport_index(volume,view),
                                      Slice_background_colour, 0 );

    /* create pixels */

    object = create_object( PIXELS );

    add_object_to_viewport( &main->graphics,
                            get_slice_viewport_index(volume,view),
                            NORMAL_PLANES, object );

    slice->pixels = get_pixels_ptr( object );

    initialize_pixels( slice->pixels, 0, 0, 0, 0, 1.0, 1.0, RGB_PIXEL );

    /* create cursor */

    slice->cursor_lines = create_cursor( main, volume, view );

    set_viewport_objects_visibility( &main->graphics,
                                     get_slice_viewport_index(volume,view),
                                     OFF );
}

public  void  terminate_slices( main_struct  *main )    /* ARGSUSED */
{
    FREE2D( main->merged.cmode_colour_map );

    FREE2D( main->merged.rgb_colour_map );
}

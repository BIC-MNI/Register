#include  <def_register.h>

private  Colour  merge_colours(
    Real      alpha1,
    Colour    col1,
    Real      alpha2,
    Colour    col2 )
{
    Real   r, g, b;

    r = alpha1 * get_Colour_r_0_1(col1) + alpha2 * get_Colour_r_0_1(col2);
    g = alpha1 * get_Colour_g_0_1(col1) + alpha2 * get_Colour_g_0_1(col2);
    b = alpha1 * get_Colour_b_0_1(col1) + alpha2 * get_Colour_b_0_1(col2);

    return( make_Colour_0_1( r, g, b ) );
}

private  void  update_merged_rgb_colour_maps(
    main_struct  *main )
{
    int     i, volume;
    Colour  col;

    for_less( volume, 0, N_MERGED )
    {
        for_less( i, 0, N_VOXEL_VALUES )
        {
            col = get_colour_code( &main->merged.colour_coding[volume],(Real)i);

            col = SCALE_COLOUR( col, main->merged.opacity[volume] );
            main->merged.rgb_colour_map[volume][i] = col;
        }
    }
}

private  void  update_merged_cmode_maps(
    main_struct  *main )
{
    int     i, j;
    Colour  col1, col2;

    for_less( i, 0, N_VOXEL_VALUES )
    {
        col1 = get_colour_code( &main->merged.colour_coding[0], (Real) i );

        for_less( j, 0, N_VOXEL_VALUES )
        {
            col2 = get_colour_code( &main->merged.colour_coding[1], (Real) j );

            main->merged.cmode_colour_map[i][j] = 
                    merge_colours( main->merged.opacity[0], col1,
                                   main->merged.opacity[1], col2 );
        }
    }
}

private  void  update_rgb_colour_maps(
    main_struct  *main,
    int          volume )
{
    int   i;

    for_less( i, 0, N_VOXEL_VALUES )
    {
        main->trislice[volume].rgb_colour_map[i] =
                    get_colour_code( &main->trislice[volume].colour_coding,
                                     (Real) i );
    }
}

private  void  update_cmode_colour_maps(
    main_struct  *main,
    int          volume )
{
    int   i, voxel_value, min_ind, max_ind;

    min_ind = main->trislice[volume].start_colour_map;
    max_ind = min_ind + main->trislice[volume].n_colour_entries-1;

    for_less( voxel_value, 0, N_VOXEL_VALUES )
    {
        main->trislice[volume].cmode_colour_map[voxel_value] = 
                    CONVERT_INTEGER_RANGE( voxel_value, 0, N_VOXEL_VALUES-1,
                                           min_ind, max_ind );
    }

    G_set_bitplanes( main->window, NORMAL_PLANES );

    for_inclusive( i, min_ind, max_ind )
    {
        voxel_value = CONVERT_INTEGER_RANGE( i, min_ind, max_ind,
                                             0, N_VOXEL_VALUES-1 );
        G_set_colour_map_entry( main->window, i, 
                    get_colour_code( &main->trislice[volume].colour_coding,
                                     (Real) voxel_value ) );
    }
}

public  void   update_colour_maps(
    main_struct  *main,
    int          volume )
{
    if( volume == MERGED_VOLUME_INDEX )
    {
        if( main->merged.active_flag )
        {
            if( G_get_colour_map_state( main->window ) )
                update_merged_cmode_maps( main );
            else
                update_merged_rgb_colour_maps( main );
        }
    }
    else
    {
        if( G_get_colour_map_state( main->window ) )
            update_cmode_colour_maps( main, volume );
        else
            update_rgb_colour_maps( main, volume );
    }
}

public  void  colour_mode_has_toggled(
    main_struct  *main,
    int          start_index )
{
    int        volume;
    int        total_colours, n_volume, n_merged;

    if( G_get_colour_map_state(main->window) )
    {
        total_colours = G_get_n_colour_map_entries( main->window )-start_index;

        n_merged = Merged_colour_table_fraction * total_colours;

        n_volume = (total_colours - n_merged) / 2;

        if( n_volume > N_VOXEL_VALUES )
            n_volume = N_VOXEL_VALUES;

        n_merged = total_colours - 2 * n_volume;

        main->trislice[0].start_colour_map = start_index;
        main->trislice[0].n_colour_entries = n_volume;

        main->trislice[1].start_colour_map = start_index + n_volume;
        main->trislice[1].n_colour_entries = n_volume;

        main->merged.start_colour_map = start_index + 2 * n_volume;
        main->merged.n_colour_entries = n_merged;
    }

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
        update_colour_maps( main, volume );

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
        set_recreate_3_slices_flags( main, volume );

    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );

    set_merged_volume_visibility( main, main->merged.active_flag );
}

private  colour_coding_has_changed (
    main_struct          *main,
    int                  volume_index )
{
    update_colour_maps( main, volume_index );

    if( !G_get_colour_map_state( main->window ) )
    {
        if( volume_index < N_VOLUMES )
            set_recreate_3_slices_flags( main, volume_index );
        else
            set_recreate_one_merged_volume( main,
                                           volume_index - MERGED_VOLUME_INDEX );
    }
}

private  colour_coding_struct  *get_volume_colour_coding(
    main_struct          *main,
    int                  volume_index )
{
    if( volume_index < N_VOLUMES )
        return( &main->trislice[volume_index].colour_coding );
    else
        return( &main->merged.colour_coding[volume_index-MERGED_VOLUME_INDEX] );
}

public  void   set_volume_colour_coding_type( 
    main_struct          *main,
    int                  volume_index,
    Colour_coding_types  type )
{
    set_colour_coding_type( get_volume_colour_coding(main,volume_index),
                            type );
    colour_coding_has_changed( main, volume_index );
}

public  Colour_coding_types   get_volume_colour_coding_type( 
    main_struct          *main,
    int                  volume_index )
{
    return( get_volume_colour_coding(main,volume_index)->type );
}

public  void  set_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    Real                 min_value,
    Real                 max_value )
{
    set_colour_coding_min_max( get_volume_colour_coding(main,volume_index),
                               min_value, max_value );

    colour_coding_has_changed( main, volume_index );
}

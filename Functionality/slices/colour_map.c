#include  <def_register.h>

private  Colour  merge_colours(
    Real      alpha1,
    Colour    col1,
    Real      alpha2,
    Colour    col2 )
{
    int   r, g, b;

    r = alpha1 * (Real) get_Colour_r(col1) + alpha2 * (Real) get_Colour_r(col2);
    if( r > 255 ) r = 255;

    g = alpha1 * (Real) get_Colour_g(col1) + alpha2 * (Real) get_Colour_g(col2);
    if( g > 255 ) g = 255;

    b = alpha1 * (Real) get_Colour_b(col1) + alpha2 * (Real) get_Colour_b(col2);
    if( b > 255 ) b = 255;

    return( make_Colour( r, g, b ) );
}

private  void  update_merged_rgb_colour_maps(
    main_struct  *main )
{
    int     i, j;
    Colour  col1, col2[N_VOXEL_VALUES];

    for_less( j, 0, N_VOXEL_VALUES )
        col2[j] = get_colour_code( &main->merged.colour_coding[1], (Real) j );

    for_less( i, 0, N_VOXEL_VALUES )
    {
        col1 = get_colour_code( &main->merged.colour_coding[0], (Real) i );

        for_less( j, 0, N_VOXEL_VALUES )
        {
            main->merged.rgb_colour_map[i][j] = 
                    merge_colours( main->merged.opacity[0], col1,
                                   main->merged.opacity[1], col2[j] );
        }
    }
}

private  void  update_merged_cmode_indices(
    main_struct  *main )
{
    int     i, j, n1, n2, min_ind, i1, i2[N_VOXEL_VALUES];

    min_ind = main->merged.start_colour_map;
    n1 = main->merged.n_colour_entries1;
    n2 = main->merged.n_colour_entries2;

    for_less( j, 0, N_VOXEL_VALUES )
        i2[j] = CONVERT_INTEGER_RANGE( j, 0, N_VOXEL_VALUES-1, 0, n2-1 );

    for_less( i, 0, N_VOXEL_VALUES )
    {
        i1 = CONVERT_INTEGER_RANGE( i, 0, N_VOXEL_VALUES-1, 0, n1-1 );
        for_less( j, 0, N_VOXEL_VALUES )
            main->merged.cmode_colour_map[i][j] = min_ind + IJ(i1,i2[j],n2);
    }
}

private  void  update_merged_cmode_maps(
    main_struct  *main )
{
    int     i, j, n1, n2, min_ind, val;
    Colour  col1, col2[N_VOXEL_VALUES];

    min_ind = main->merged.start_colour_map;
    n1 = main->merged.n_colour_entries1;
    n2 = main->merged.n_colour_entries2;

    G_set_bitplanes( main->window, NORMAL_PLANES );

    for_less( j, 0, n2 )
    {
        val = CONVERT_INTEGER_RANGE( j, 0, n2-1, 0, N_VOXEL_VALUES-1 );
        col2[j] = get_colour_code( &main->merged.colour_coding[1], (Real) val);
    }

    for_less( i, 0, n1 )
    {
        val = CONVERT_INTEGER_RANGE( i, 0, n1-1, 0, N_VOXEL_VALUES-1 );
        col1 = get_colour_code( &main->merged.colour_coding[0], (Real) val );
        for_less( j, 0, n2 )
        {
            G_set_colour_map_entry( main->window, min_ind + IJ(i,j,n2), 
                    merge_colours( main->merged.opacity[0], col1,
                                   main->merged.opacity[1], col2[j] ) );
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

private  void  update_cmode_indices(
    main_struct  *main,
    int          volume )
{
    int   min_ind, max_ind, voxel_value;

    min_ind = main->trislice[volume].start_colour_map;
    max_ind = min_ind + main->trislice[volume].n_colour_entries-1;

    for_less( voxel_value, 0, N_VOXEL_VALUES )
    {
        main->trislice[volume].cmode_colour_map[voxel_value] = 
                    CONVERT_INTEGER_RANGE( voxel_value, 0, N_VOXEL_VALUES-1,
                                           min_ind, max_ind );
    }
}

private  void  update_cmode_colour_maps(
    main_struct  *main,
    int          volume )
{
    int   i, voxel_value, min_ind, max_ind;

    min_ind = main->trislice[volume].start_colour_map;
    max_ind = min_ind + main->trislice[volume].n_colour_entries-1;

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
    int        volume, view, bg_index;
    int        total_colours, n_volume, n_merged;

    if( G_get_colour_map_state(main->window) )
    {
        bg_index = start_index;

        G_set_colour_map_entry( main->window, bg_index,
                                Slice_background_colour );

        ++start_index;

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
        main->merged.n_colour_entries1 = (int) sqrt( (double) n_merged );
        if( main->merged.n_colour_entries1 > N_VOXEL_VALUES )
            main->merged.n_colour_entries1 = N_VOXEL_VALUES;

        main->merged.n_colour_entries2 = n_merged /
                                         main->merged.n_colour_entries1;
        if( main->merged.n_colour_entries2 > N_VOXEL_VALUES )
            main->merged.n_colour_entries2 = N_VOXEL_VALUES;

        for_less( volume, 0, N_VOLUMES )
            update_cmode_indices( main, volume );

        update_merged_cmode_indices( main );

        for_less( volume, 0, N_VOLUMES_DISPLAYED )
        {
            for_less( view, 0, N_VIEWS )
            {
                set_graphics_viewport_background( &main->graphics,
                                  get_slice_viewport_index(volume,view),
                                  Slice_background_colour, bg_index );
            }
        }
    }

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
        update_colour_maps( main, volume );

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
        set_recreate_3_slices_flags( main, volume );

    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
}

private  colour_coding_has_changed (
    main_struct          *main,
    int                  volume_index )
{
    if( volume_index > MERGED_VOLUME_INDEX )
        volume_index = MERGED_VOLUME_INDEX;

    update_colour_maps( main, volume_index );

    if( !G_get_colour_map_state( main->window ) )
        set_recreate_3_slices_flags( main, volume_index );
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

public  void  set_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume,
    Real                 opacity )
{
    main->merged.opacity[which_volume] = opacity;

    colour_coding_has_changed( main, MERGED_VOLUME_INDEX );
}

public  void   set_volume_under_colour( 
    main_struct          *main,
    int                  volume_index,
    Colour               colour )
{
    set_colour_coding_under_colour( get_volume_colour_coding(main,volume_index),
                                    colour );
    colour_coding_has_changed( main, volume_index );
}

public  void   set_volume_over_colour( 
    main_struct          *main,
    int                  volume_index,
    Colour               colour )
{
    set_colour_coding_over_colour( get_volume_colour_coding(main,volume_index),
                                   colour );
    colour_coding_has_changed( main, volume_index );
}

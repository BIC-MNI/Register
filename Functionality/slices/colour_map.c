#include  <def_register.h>

typedef  enum  { UNDER_RANGE, WITHIN_RANGE, OVER_RANGE } Range_flags;

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

private  Colour  get_merged_colour(
    Merge_methods  method,
    Boolean        use_under_over_colour_with_weights,
    Colour         under_colour,
    Colour         over_colour,
    Range_flags    range_flag1,
    Real           alpha1,
    Colour         col1,
    Range_flags    range_flag2,
    Real           alpha2,                        /* ARGSUSED */
    Colour         col2 )
{
    if( (!use_under_over_colour_with_weights ||
         method == ONE_ON_TWO || method == TWO_ON_ONE) &&
         (range_flag1 == UNDER_RANGE && range_flag2 == UNDER_RANGE) )
    {
        return( under_colour );
    }

    switch( method )
    {
    case ONE_ON_TWO:
        if( range_flag1 != UNDER_RANGE )
            return( col1 );
        else
            return( col2 );

    case TWO_ON_ONE:
        if( range_flag2 != UNDER_RANGE )
            return( col2 );
        else
            return( col1 );

    case BLEND_VOLUMES:
        alpha2 = 1.0 - alpha1;   /* drop through to WEIGHTED_VOLUMES */
    case WEIGHTED_VOLUMES:
        if( use_under_over_colour_with_weights ||
            range_flag1 != UNDER_RANGE && range_flag2 != UNDER_RANGE )
            return( merge_colours( alpha1, col1, alpha2, col2 ) );
        else if( range_flag1 == UNDER_RANGE )
            return( merge_colours( 0.0, col1, alpha2, col2 ) );
        else
            return( merge_colours( alpha1, col1, 0.0, col2 ) );
    }

    return( BLACK );
}

private  Range_flags  lookup_colour_code(
    volume_struct         *volume,
    colour_coding_struct  *colour_coding,
    int                   voxel,
    Colour                *col )
{
    Real          value;
    Range_flags   flag;

    value = CONVERT_VOXEL_TO_VALUE( *volume, voxel );

    *col = get_colour_code( colour_coding, value );
    if( value < colour_coding->min_value )
        flag = UNDER_RANGE;
    else if( value > colour_coding->max_value )
        flag = OVER_RANGE;
    else
        flag = WITHIN_RANGE;

    return( flag );
}

private  void  update_merged_rgb_colour_maps(
    main_struct  *main )
{
    int             i, j;
    int             min_value1, max_value1, min_value2, max_value2;
    Colour          col1, col2[N_VOXEL_VALUES];
    Range_flags     flag1, flags2[N_VOXEL_VALUES];
    volume_struct   *volume1, *volume2;

    volume1 = get_slice_volume( main, 0 );
    volume2 = get_slice_volume( main, 1 );

    get_volume_voxel_range( main, 0, &min_value1, &max_value1 );
    get_volume_voxel_range( main, 1, &min_value2, &max_value2 );

    for_inclusive( j, min_value2, max_value2 )
    {
        flags2[j] = lookup_colour_code( volume2, &main->merged.colour_coding[1],
                                        j, &col2[j] );
    }

    for_inclusive( i, min_value1, max_value1 )
    {
        flag1 = lookup_colour_code( volume1, &main->merged.colour_coding[0],
                                    i, &col1 );

        for_inclusive( j, min_value2, max_value2 )
        {
            main->merged.rgb_colour_map[i][j] = 
                    get_merged_colour( main->merged.merge_method,
                                   Use_over_under_colour_in_weights,
                                   main->merged.colour_coding[0].under_colour,
                                   main->merged.colour_coding[0].over_colour,
                                   flag1,     main->merged.opacity[0], col1,
                                   flags2[j], main->merged.opacity[1], col2[j]);
        }
    }
}

private  void  update_merged_cmode_indices(
    main_struct  *main )
{
    int     i, j, n1, n2, min_ind, i1, i2[N_VOXEL_VALUES];
    int     min_value1, max_value1, min_value2, max_value2;

    get_volume_voxel_range( main, 0, &min_value1, &max_value1 );
    get_volume_voxel_range( main, 1, &min_value2, &max_value2 );

    min_ind = main->merged.start_colour_map;
    n1 = main->merged.n_colour_entries1;
    n2 = main->merged.n_colour_entries2;

    for_inclusive( j, min_value2, max_value2 )
        i2[j] = CONVERT_INTEGER_RANGE( j, min_value2, max_value2, 0, n2-1 );

    for_inclusive( i, min_value1, max_value1 )
    {
        i1 = CONVERT_INTEGER_RANGE( i, min_value1, max_value1, 0, n1-1 );
        for_less( j, 0, N_VOXEL_VALUES )
            main->merged.cmode_colour_map[i][j] = min_ind + IJ(i1,i2[j],n2);
    }
}

private  void  update_merged_cmode_maps(
    main_struct  *main )
{
    int             i, j, n1, n2, min_ind, voxel;
    Colour          col1, col2[N_VOXEL_VALUES];
    Range_flags     flag1, flags2[N_VOXEL_VALUES];
    int             min_value1, max_value1, min_value2, max_value2;
    volume_struct   *volume1, *volume2;

    volume1 = get_slice_volume( main, 0 );
    volume2 = get_slice_volume( main, 1 );

    get_volume_voxel_range( main, 0, &min_value1, &max_value1 );
    get_volume_voxel_range( main, 1, &min_value2, &max_value2 );

    min_ind = main->merged.start_colour_map;
    n1 = main->merged.n_colour_entries1;
    n2 = main->merged.n_colour_entries2;

    for_less( j, 0, n2 )
    {
        voxel = CONVERT_INTEGER_RANGE( j, 0, n2-1, min_value2, max_value2 );
        flags2[j] = lookup_colour_code( volume2, &main->merged.colour_coding[1],
                                        voxel, &col2[j] );
    }

    for_less( i, 0, n1 )
    {
        voxel = CONVERT_INTEGER_RANGE( i, 0, n1-1, min_value1, max_value1 );
        flag1 = lookup_colour_code( volume1, &main->merged.colour_coding[0],
                                    voxel, &col1 );
        for_less( j, 0, n2 )
        {
            G_set_colour_map_entry( main->window, min_ind + IJ(i,j,n2), 
                get_merged_colour( main->merged.merge_method,
                               Use_over_under_colour_in_weights,
                               main->merged.colour_coding[0].under_colour,
                               main->merged.colour_coding[0].over_colour,
                               flag1, main->merged.opacity[0], col1,
                               flags2[j], main->merged.opacity[1], col2[j] ) );
        }
    }
}

private  void  update_rgb_colour_maps(
    main_struct  *main,
    int          volume_index )
{
    int            i;
    int            min_value, max_value;
    volume_struct  *volume;

    volume = get_slice_volume( main, volume_index );

    get_volume_voxel_range( main, volume_index, &min_value, &max_value );

    for_inclusive( i, min_value, max_value )
    {
        main->trislice[volume_index].rgb_colour_map[i] =
                    get_colour_code(
                        &main->trislice[volume_index].colour_coding,
                        CONVERT_VOXEL_TO_VALUE(*volume,i) );
    }
}

private  void  update_cmode_indices(
    main_struct  *main,
    int          volume )
{
    int   min_ind, max_ind, voxel_value;
    int   min_value, max_value;

    min_ind = main->trislice[volume].start_colour_map;
    max_ind = min_ind + main->trislice[volume].n_colour_entries-1;

    get_volume_voxel_range( main, volume, &min_value, &max_value );

    for_inclusive( voxel_value, min_value, max_value )
    {
        main->trislice[volume].cmode_colour_map[voxel_value] = 
                    CONVERT_INTEGER_RANGE( voxel_value, min_value, max_value,
                                           min_ind, max_ind );
    }
}

private  void  update_cmode_colour_maps(
    main_struct  *main,
    int          volume_index )
{
    int            i, voxel_value, min_ind, max_ind;
    int            min_value, max_value;
    volume_struct  *volume;

    min_ind = main->trislice[volume_index].start_colour_map;
    max_ind = min_ind + main->trislice[volume_index].n_colour_entries-1;

    get_volume_voxel_range( main, volume_index, &min_value, &max_value );

    volume = get_slice_volume( main, volume_index );

    for_inclusive( i, min_ind, max_ind )
    {
        voxel_value = CONVERT_INTEGER_RANGE( i, min_ind, max_ind,
                                             min_value, max_value );
        G_set_colour_map_entry( main->window, i, 
                    get_colour_code(
                         &main->trislice[volume_index].colour_coding,
                         CONVERT_VOXEL_TO_VALUE(*volume,voxel_value) ) );
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

public  void  repartition_colour_maps(
    main_struct  *main )
{
    int        volume, start_index;
    int        total_colours, n_merged;
    int        min_value1, max_value1, min_value2, max_value2;
    int        max_colours_1, max_colours_2, max_colours_merged, total_volume;
    int        n_volume_1, n_volume_2, n_merged_1, n_merged_2;
    Real       ratio;

    start_index = main->start_colour_index + N_MAIN_COLOURS;

    total_colours = G_get_n_colour_map_entries( main->window ) - start_index;

    get_volume_voxel_range( main, 0, &min_value1, &max_value1 );
    get_volume_voxel_range( main, 1, &min_value2, &max_value2 );

    max_colours_1 = max_value1 - min_value1 + 1;
    max_colours_2 = max_value2 - min_value2 + 1;
    max_colours_merged = max_colours_1 * max_colours_2;

    n_merged = Merged_colour_table_fraction * total_colours;

    if( n_merged > max_colours_merged )
        n_merged = max_colours_merged;

    total_volume = total_colours - n_merged;
    if( total_volume >= max_colours_1 + max_colours_2 )
    {
        n_volume_1 = max_colours_1;
        n_volume_2 = max_colours_2;
        n_merged = MIN( max_colours_merged,
                        total_colours - n_volume_1 - n_volume_2 );
    }
    else
    {
        ratio = (Real) max_colours_1 / (Real) max_colours_2;
        n_volume_1 = ROUND( (Real) total_volume * ratio );
        if( n_volume_1 < 1 )
            n_volume_1 = 1;
        n_volume_2 = total_volume - n_volume_1;
    }

    if( n_merged + n_volume_1 + n_volume_2 > total_colours )
        HANDLE_INTERNAL_ERROR( "repartition_colour_maps" );

    if( n_volume_1 * n_volume_2 <= n_merged )
    {
        n_merged_1 = n_volume_1;
        n_merged_2 = n_volume_2;
    }
    else
    {
        n_merged_1 = (int) sqrt( (double) n_merged );
        if( n_merged_1 < 1 )
            n_merged_1 = 1;

        if( max_colours_1 <= n_merged_1 )
        {
            n_merged_1 = max_colours_1;
            n_merged_2 = MAX( max_colours_2, n_merged / n_merged_1 );
        }
        else if( max_colours_2 <= n_merged_1 )
        {
            n_merged_2 = max_colours_2;
            n_merged_1 = MAX( max_colours_1, n_merged / n_merged_2 );
        }
        else
        {
            n_merged_2 = n_merged / n_merged_1;
        }
    }

#ifdef  DEBUG
(void) printf( "%d %d  (%d * %d) = %d   total = %d\n",
       n_volume_1, n_volume_2, n_merged_1, n_merged_2, n_merged_1 * n_merged_2,
       n_volume_1 + n_volume_2 + n_merged_1 * n_merged_2 );
#endif

    main->trislice[0].start_colour_map = start_index;
    main->trislice[0].n_colour_entries = n_volume_1;

    main->trislice[1].start_colour_map = start_index + n_volume_1;
    main->trislice[1].n_colour_entries = n_volume_2;

    main->merged.start_colour_map = start_index + n_volume_1 + n_volume_2;
    main->merged.n_colour_entries1 = n_merged_1;
    main->merged.n_colour_entries2 = n_merged_2;

    for_less( volume, 0, N_VOLUMES )
        update_cmode_indices( main, volume );

    update_merged_cmode_indices( main );

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
        set_recreate_3_slices_flags( main, volume );
}

public  void  colour_mode_has_toggled(
    main_struct  *main,
    int          start_index )
{
    int        volume, view, bg_index;

    if( G_get_colour_map_state(main->window) )
    {
        main->start_colour_index = start_index;

        bg_index = main->start_colour_index + BACKGROUND_COLOUR;

        G_set_colour_map_entry( main->window, bg_index,
                                Slice_background_colour );
        G_set_colour_map_entry( main->window,
                                main->start_colour_index + TAG_INSIDE_COLOUR,
                                Tag_inside_colour );
        G_set_colour_map_entry( main->window,
                                main->start_colour_index + TAG_OUTSIDE_COLOUR,
                                Tag_outside_colour );
        G_set_colour_map_entry( main->window,
                                main->start_colour_index +
                                      TAG_INSIDE_INACTIVE_COLOUR,
                                Tag_inside_inactive_colour );
        G_set_colour_map_entry( main->window,
                                main->start_colour_index +
                                      TAG_OUTSIDE_INACTIVE_COLOUR,
                                Tag_outside_inactive_colour );
        G_set_colour_map_entry( main->window,
                                main->start_colour_index + CURSOR_INSIDE_COLOUR,
                                Cursor_inside_colour );
        G_set_colour_map_entry( main->window,
                                main->start_colour_index +CURSOR_OUTSIDE_COLOUR,
                                Cursor_outside_colour );

        for_less( volume, 0, N_VOLUMES_DISPLAYED )
        {
            for_less( view, 0, N_VIEWS )
            {
                set_graphics_viewport_background( &main->graphics,
                                  get_slice_viewport_index(volume,view),
                                  Slice_background_colour, bg_index );
            }
        }

        repartition_colour_maps( main );
    }

    update_all_tag_colours( main );

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
        update_colour_maps( main, volume );

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
        set_recreate_3_slices_flags( main, volume );

    for_less( volume, 0, N_VOLUMES_DISPLAYED )
        for_less( view, 0, N_VIEWS )
            update_cursor_colours( main, volume, view );
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

public  void  get_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    Real                 *min_value,
    Real                 *max_value )
{
    get_colour_coding_min_max( get_volume_colour_coding(main,volume_index),
                               min_value, max_value );
}

public  void  set_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume,
    Real                 opacity )
{
    main->merged.opacity[which_volume] = opacity;

    colour_coding_has_changed( main, MERGED_VOLUME_INDEX );
}

public  Real  get_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume )
{
    return( main->merged.opacity[which_volume] );
}

public  void  set_merged_method(
    main_struct       *main,
    Merge_methods     method )
{
    main->merged.merge_method = method;

    colour_coding_has_changed( main, MERGED_VOLUME_INDEX );
}

public  Merge_methods  get_merged_method(
    main_struct       *main )
{
    return( main->merged.merge_method );
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

/**
 * \file Functionality/slices/colour_map.c
 * \brief Handle mapping of voxel data to colours.
 *
 * \copyright
              Copyright 1993,1994,1995 David MacDonald,
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- */

#include  <register.h>

typedef  enum  { UNDER_RANGE, WITHIN_RANGE, OVER_RANGE } Range_flags;

#if 0
static  VIO_Colour  merge_colours(
    VIO_Real      alpha1,
    VIO_Colour    col1,
    VIO_Real      alpha2,
    VIO_Colour    col2 )
{
    int   r, g, b;

    r = (int) (alpha1 * (VIO_Real) get_Colour_r(col1) +
               alpha2 * (VIO_Real) get_Colour_r(col2));
    if( r > 255 ) r = 255;

    g = (int) (alpha1 * (VIO_Real) get_Colour_g(col1) +
               alpha2 * (VIO_Real) get_Colour_g(col2));
    if( g > 255 ) g = 255;

    b = (int) (alpha1 * (VIO_Real) get_Colour_b(col1) +
               alpha2 * (VIO_Real) get_Colour_b(col2));
    if( b > 255 ) b = 255;

    return( make_Colour( r, g, b ) );
}

/* ARGSUSED */

static  VIO_Colour  get_merged_colour(
    Merge_methods  method,
    VIO_BOOL        use_under_over_colour_with_weights,
    VIO_Colour         under_colour,
    VIO_Colour         over_colour,
    Range_flags    range_flag1,
    VIO_Real           alpha1,
    VIO_Colour         col1,
    Range_flags    range_flag2,
    VIO_Real           alpha2,
    VIO_Colour         col2 )
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
    case WEIGHTED_VOLUMES:
        if( method == BLEND_VOLUMES )
            alpha2 = 1.0 - alpha1;

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

static  Range_flags  lookup_colour_code(
    VIO_Volume                volume,
    colour_coding_struct  *colour_coding,
    int                   voxel,
    VIO_Colour                *col )
{
    VIO_Real          value;
    Range_flags   flag;

    value = CONVERT_VOXEL_TO_VALUE( volume, (VIO_Real) voxel );

    *col = get_colour_code( colour_coding, value );
    if( value < colour_coding->min_value )
        flag = UNDER_RANGE;
    else if( value > colour_coding->max_value )
        flag = OVER_RANGE;
    else
        flag = WITHIN_RANGE;

    return( flag );
}
#endif /* 0 */

static  void  update_rgb_colour_maps(
    main_struct  *main,
    int          volume_index )
{
    int            i;
    int            min_value, max_value;
    VIO_Volume         volume;
    VIO_Colour         *tmp_ptr;

    volume = get_slice_volume( main, volume_index );

    /* If we had previously allocated a colour map, free it.
     */
    if (main->trislice[volume_index].rgb_colour_map != NULL) {
        free(main->trislice[volume_index].rgb_colour_map +
             main->trislice[volume_index].rgb_colour_offset);
    }

    get_volume_range_of_voxels( main, volume_index, &min_value, &max_value );
    if (((double) max_value - (double) min_value) > 100000000 ||
        ((double) max_value - (double) min_value) < Min_colour_map) {
        /* It is impractical to optimize colour map accesses in these
         * cases, so we don't bother.
         */
        main->trislice[volume_index].rgb_colour_map = NULL;
        return;
    }

    tmp_ptr = malloc((max_value - min_value + 1) * sizeof (*tmp_ptr));
    if (tmp_ptr == NULL) {
        fprintf(stderr, "ERROR: Unable to allocate space for colour map.\n");
        exit(EXIT_FAILURE);
    }

    /* The colour map is set to the allocated pointer MINUS the minimum
     * value.  For volumes with a signed range, this means that we can
     * index the colour map by a voxel value with no translation.
     */
    main->trislice[volume_index].rgb_colour_map = tmp_ptr - min_value;
    main->trislice[volume_index].rgb_colour_offset = min_value;

    /* Initialize all of the colour map entries.
     */
    for_inclusive( i, min_value, max_value )
    {
        main->trislice[volume_index].rgb_colour_map[i] =
                    get_colour_code(
                        &main->trislice[volume_index].colour_coding,
                        CONVERT_VOXEL_TO_VALUE(volume,(VIO_Real) i) );
    }
}

void   update_colour_maps(
    main_struct  *main,
    int          volume )
{
    if( volume != MERGED_VOLUME_INDEX && is_volume_active( main, volume ) )
    {
      update_rgb_colour_maps( main, volume );
    }
}

  void  colour_mode_has_toggled(
    main_struct  *main,
    int          start_index )
{
    int        volume, view;

    update_all_tag_colours( main );

    for_less( volume, 0, main->n_volumes_displayed )
        update_colour_maps( main, volume );

    for_less( volume, 0, main->n_volumes_displayed )
        set_recreate_3_slices_flags( main, volume );

    for_less( volume, 0, main->n_volumes_displayed )
        for_less( view, 0, N_VIEWS )
            update_cursor_colours( main, volume, view );
}

static  void   colour_coding_has_changed (
    main_struct          *main,
    int                  volume_index )
{
    if( volume_index > MERGED_VOLUME_INDEX )
        volume_index = MERGED_VOLUME_INDEX;

    update_colour_maps( main, volume_index );

    if( !G_get_colour_map_state( main->window ) )
        set_recreate_3_slices_flags( main, volume_index );
}

static  colour_coding_struct  *get_volume_colour_coding(
    main_struct          *main,
    int                  volume_index )
{
  if( volume_index < main->n_volumes_displayed - 1 )
        return( &main->trislice[volume_index].colour_coding );
    else
        return( &main->merged.colour_coding[volume_index-MERGED_VOLUME_INDEX] );
}

  void   set_volume_colour_coding_type(
    main_struct          *main,
    int                  volume_index,
    Colour_coding_types  type )
{
    set_colour_coding_type( get_volume_colour_coding(main,volume_index),
                            type );
    colour_coding_has_changed( main, volume_index );
}

  Colour_coding_types   get_volume_colour_coding_type(
    main_struct          *main,
    int                  volume_index )
{
    return( get_volume_colour_coding(main,volume_index)->type );
}

  void  set_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    VIO_Real                 min_value,
    VIO_Real                 max_value )
{
    set_colour_coding_min_max( get_volume_colour_coding(main,volume_index),
                               min_value, max_value );

    colour_coding_has_changed( main, volume_index );
}

  void  get_volume_colour_coding_limits(
    main_struct          *main,
    int                  volume_index,
    VIO_Real                 *min_value,
    VIO_Real                 *max_value )
{
    get_colour_coding_min_max( get_volume_colour_coding(main,volume_index),
                               min_value, max_value );
}

  void  set_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume,
    VIO_Real                 opacity )
{
    main->merged.opacity[which_volume] = opacity;

    colour_coding_has_changed( main, MERGED_VOLUME_INDEX );
}

  VIO_Real  get_merged_volume_opacity(
    main_struct          *main,
    int                  which_volume )
{
    return( main->merged.opacity[which_volume] );
}

Merge_methods get_volume_merge_method(
    main_struct          *main,
    int                  which_volume )
{
    return( main->merged.method[which_volume] );
}

void set_volume_merge_method(
    main_struct          *main,
    int                  which_volume,
    Merge_methods        merge_method )
{
    main->merged.method[which_volume] = merge_method;

    colour_coding_has_changed( main, MERGED_VOLUME_INDEX );
}

  void   set_volume_under_colour(
    main_struct          *main,
    int                  volume_index,
    VIO_Colour               colour )
{
    set_colour_coding_under_colour( get_volume_colour_coding(main,volume_index),
                                    colour );
    colour_coding_has_changed( main, volume_index );
}

  void   set_volume_over_colour(
    main_struct          *main,
    int                  volume_index,
    VIO_Colour               colour )
{
    set_colour_coding_over_colour( get_volume_colour_coding(main,volume_index),
                                   colour );
    colour_coding_has_changed( main, volume_index );
}

  void  composite_merged_pixels(
    main_struct          *main,
    pixels_struct        pixels[],
    pixels_struct        *result )
{
    int            p, n_pixels;
    VIO_Colour     col1, col2;
    VIO_Colour     *res_ptr;
    int            r1, g1, b1, r2, g2, b2, r, g, b;
    VIO_Real       alpha1, a1;
    VIO_Real       alpha2, a2;
    int            vol;
    int            x, y;
    int            dx, dy;
    int            ind;

    n_pixels = result->x_size * result->y_size;

    res_ptr = &PIXEL_RGB_COLOUR( *result, 0, 0 );
    for_less(p, 0, n_pixels)
    {
      res_ptr[p] = 0;
    }

    for (vol = 0; vol < main->n_volumes_displayed - 1; vol++)
    {
      if (main->merged.method[vol] == DISABLED)
          continue;

      dx = (result->x_size - pixels[vol].x_size) / 2;
      dy = (result->y_size - pixels[vol].y_size) / 2;
      if (dx < 0)
      {
        dx = 0;
      }
      if (dy < 0)
      {
        dy = 0;
      }

#ifdef DEBUG
      printf("%d %d, %d %d, %d %d\n", dx, dy, result->x_size, result->y_size,
             pixels[vol].x_size, pixels[vol].y_size);
#endif /* DEBUG */

      alpha1 = main->merged.opacity[vol];
      for (x = 0; x < pixels[vol].x_size; x++)
      {
        for (y = 0; y < pixels[vol].y_size; y++)
        {
          if (x < 0 || x >= result->x_size ||
              y < 0 || y >= result->y_size)
            continue;
          if (x < 0 || x >= pixels[vol].x_size ||
              y < 0 || y >= pixels[vol].y_size)
            continue;
          col1 = PIXEL_RGB_COLOUR( pixels[vol], x, y );
          col2 = PIXEL_RGB_COLOUR( *result, x, y );
          a1 = get_Colour_a_0_1( col1 );
          a2 = get_Colour_a_0_1( col2 );
          if (main->merged.method[vol] == OPAQUE)
          {
            a2 = (1.0 - a1) * a2;
          }
          else
          {
            a1 = alpha1 * a1;
          }

          r1 = get_Colour_r( col1 );
          g1 = get_Colour_g( col1 );
          b1 = get_Colour_b( col1 );

          r2 = get_Colour_r( col2 );
          g2 = get_Colour_g( col2 );
          b2 = get_Colour_b( col2 );

          r = (int) (a1 * (VIO_Real) r1) + (a2 * (VIO_Real) r2);
          g = (int) (a1 * (VIO_Real) g1) + (a2 * (VIO_Real) g2);
          b = (int) (a1 * (VIO_Real) b1) + (a2 * (VIO_Real) b2);

          if( r < 0 )
            r = 0;
          else if( r > 255 )
            r = 255;

          if( g < 0 )
            g = 0;
          else if( g > 255 )
            g = 255;

          if( b < 0 )
            b = 0;
          else if( b > 255 )
            b = 255;

          PIXEL_RGB_COLOUR(*result, x+dx, y+dy) = make_Colour( r, g, b );
        }
      }
    }
}

  VIO_BOOL  can_switch_colour_modes(
    main_struct          *main )
{
    return( FALSE );
}

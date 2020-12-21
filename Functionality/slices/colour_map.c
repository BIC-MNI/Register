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
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif //HAVE_CONFIG_H

#include  <register.h>

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

static  void   colour_coding_has_changed (
    main_struct          *main,
    int                  volume_index )
{
    if( volume_index < MERGED_VOLUME_INDEX )
    {
        update_colour_maps( main, volume_index );
        set_recreate_3_slices_flags( main, volume_index );
    }
    set_recreate_3_slices_flags( main, MERGED_VOLUME_INDEX );
}

static  colour_coding_struct  *get_volume_colour_coding(
    main_struct          *main,
    int                  volume_index )
{
  if( volume_index >= MERGED_VOLUME_INDEX )
    volume_index -= MERGED_VOLUME_INDEX;
  return( &main->trislice[volume_index].colour_coding );
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

static void
composite_one_image( pixels_struct *result,
                     const pixels_struct *source,
                     Merge_methods method,
                     VIO_Real alpha1 )
{
  int        dx = (result->x_size - source->x_size) / 2;
  int        dy = (result->y_size - source->y_size) / 2;
  int        x;
  int        y;
  int        r1, g1, b1, r2, g2, b2, r, g, b;
  VIO_Real   a1, a2;
  VIO_Colour col1, col2;

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
         source->x_size, source->y_size);
#endif /* DEBUG */

  for (x = 0; x < source->x_size; x++)
  {
    for (y = 0; y < source->y_size; y++)
    {
      if ((x + dx) >= result->x_size || (y + dy) >= result->y_size)
        continue;
      col1 = PIXEL_RGB_COLOUR( *source, x, y );
      col2 = PIXEL_RGB_COLOUR( *result, x + dx, y + dy);
      a1 = get_Colour_a_0_1( col1 );
      a2 = get_Colour_a_0_1( col2 );
      if (method == OPAQUE)
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

      PIXEL_RGB_COLOUR( *result, x + dx, y + dy ) = make_Colour( r, g, b );
    }
  }
}

  void  composite_merged_pixels(
    main_struct          *main,
    pixels_struct        pixels[],
    pixels_struct        *result )
{
    int            p, n_pixels;
    VIO_Colour     *res_ptr;
    int            vol;

    n_pixels = result->x_size * result->y_size;

    res_ptr = &PIXEL_RGB_COLOUR( *result, 0, 0 );
    for_less(p, 0, n_pixels)
    {
      res_ptr[p] = 0;
    }

    /* Draw blended images FIRST. */
    for (vol = 0; vol < main->n_volumes_displayed - 1; vol++)
    {
      if (main->merged.method[vol] == BLENDED)
      {
        composite_one_image( result, &pixels[vol], BLENDED,
                             main->merged.opacity[vol] );
      }
    }

    /* Then draw opaque images. */
    for (vol = 0; vol < main->n_volumes_displayed - 1; vol++)
    {
      if (main->merged.method[vol] == OPAQUE)
      {
        composite_one_image( result, &pixels[vol], OPAQUE,
                             main->merged.opacity[vol] );
      }
    }

}

  VIO_BOOL  can_switch_colour_modes(
    main_struct          *main )
{
    return( FALSE );
}

void load_volume_colour_coding( main_struct *main,
                                int volume_index, VIO_STR filename )
{
  input_user_defined_colour_coding( get_volume_colour_coding( main, volume_index ),
                                    filename );
}

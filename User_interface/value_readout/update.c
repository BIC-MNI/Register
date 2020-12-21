/**
 * \file User_interface/value_readout/update.c
 * \brief Single function to update the 'value readout' for each volume.
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

#include  <user_interface.h>

  void  update_volume_readout(
    UI_struct  *ui,
    int        volume_index )
{
    VIO_Real   value;
    VIO_Real   voxel[VIO_MAX_DIMENSIONS];

    if( volume_index < MERGED_VOLUME_INDEX )
    {
        IF_get_volume_voxel_position( volume_index, voxel );
        value = IF_get_voxel_value( volume_index, voxel[VIO_X], voxel[VIO_Y], voxel[VIO_Z] );
        set_text_entry_real_value( get_volume_readout_widget(ui, volume_index),
                                   Readout_values_format, value );
    }
    else
    {
        int i;
        for (i = 0; i < ui->n_volumes_loaded; i++)
        {
          IF_get_volume_voxel_position( MERGED_VOLUME_INDEX + i, voxel );
          value = IF_get_voxel_value( i, voxel[VIO_X], voxel[VIO_Y], voxel[VIO_Z] );
          set_text_entry_real_value( get_merged_readout_widget(ui, i),
                                     Readout_values_format, value );
        }
    }
}


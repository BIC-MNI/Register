/**
 * \file User_interface/widget_instances/position_widgets.c
 *
 * \brief Implement the position (world, voxel, and time coordinates) 
 * widgets for the per-volume interface.
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

static  void    set_voxel_position_callback( UI_struct *, widget_struct *,
                                              int );
static  void    set_world_position_callback( UI_struct *, widget_struct *,
                                              int );

typedef  enum
{
    VOXEL_LABEL,
    X_VOXEL_TEXT,
    Y_VOXEL_TEXT,
    Z_VOXEL_TEXT,
    WORLD_LABEL,
    X_WORLD_TEXT,
    Y_WORLD_TEXT,
    Z_WORLD_TEXT,
    TIME_LABEL,
    TIME_TEXT,
    N_POSITION_WIDGETS
} Position_widgets;

static  int  widget_indices[N_POSITION_WIDGETS];

static  DEFINE_WIDGET_CALLBACK( pos_voxel_callback )
{
    set_voxel_position_callback( get_ui_struct(), widget,
                                 (intptr_t) callback_data );
}

static  DEFINE_WIDGET_CALLBACK( pos_world_callback )
{
    set_world_position_callback( get_ui_struct(), widget,
                                 (intptr_t) callback_data );
}

static  DEFINE_WIDGET_CALLBACK( pos_time_callback )
{
    int    volume_index;
    int    start_widget_index;
    VIO_Real   value;
    VIO_Real   tpos;
    UI_struct *ui_info = get_ui_struct();

    volume_index = get_viewport_volume_index( widget->viewport_index );
    start_widget_index = ui_info->position_text_start_index[volume_index];

    IF_get_volume_time_position( volume_index, &tpos );

    if ( get_text_entry_real_value( ui_info->widget_list
                                    [widget->viewport_index].widgets
                                    [start_widget_index+
                                     widget_indices[TIME_TEXT]],
                                    &value ) )
    {
        tpos = value;
    }

    ui_set_volume_time_position( ui_info, volume_index, tpos );
}

  int  add_cursor_position_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               *height,
    VIO_BOOL          show_time )
{
    int           x, y, start_index, x_start, dx;
    int           x_min, x_max, y_min, y_max;

    get_graphics_viewport( &ui_info->graphics_window.graphics,
                           (int) viewport_index,
                           &x_min, &x_max, &y_min, &y_max );

    x = Volume_x_spacing;
    y = (y_max - y_min) + 1 - Volume_y_spacing - Text_entry_height;

    start_index = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_label( &ui_info->graphics_window, viewport_index, 
                  x, y, Position_label_width, Text_entry_height,
                  "V:", FALSE, LABEL_ACTIVE_COLOUR,
                  LABEL_SELECTED_COLOUR, LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  (Font_types) Label_text_font, Label_text_font_size ) );

    widget_indices[VOXEL_LABEL] = 0;

    x_start = x + Position_label_width + Position_values_separation;

    dx = Position_values_width + Position_values_separation;

    widget_indices[X_VOXEL_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_text_entry( &ui_info->graphics_window, viewport_index,
                       x_start, y,
                       Position_values_width, Text_entry_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       pos_voxel_callback, (void *) VIO_X ) ) - start_index;

    widget_indices[Y_VOXEL_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_text_entry( &ui_info->graphics_window, viewport_index,
                       x_start + dx, y,
                       Position_values_width, Text_entry_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       pos_voxel_callback, (void *) VIO_Y ) ) - start_index;

    widget_indices[Z_VOXEL_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_text_entry( &ui_info->graphics_window, viewport_index,
                       x_start + 2 * dx, y,
                       Position_values_width, Text_entry_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       pos_voxel_callback, (void *) VIO_Z ) ) - start_index;

    widget_indices[WORLD_LABEL] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_label( &ui_info->graphics_window, viewport_index, 
                  x_start + 3 * dx + Position_values_separation, y,
                  Position_label_width, Text_entry_height,
                  "W:", FALSE, LABEL_ACTIVE_COLOUR, LABEL_SELECTED_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  (Font_types) Label_text_font, Label_text_font_size ) ) -
                  start_index;

    x_start = x_start + 3 * dx + Position_values_separation +
              Position_label_width + Position_values_separation;

    widget_indices[X_WORLD_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_text_entry( &ui_info->graphics_window, viewport_index,
                       x_start, y,
                       Position_values_width, Text_entry_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       pos_world_callback, (void *) VIO_X ) ) - start_index;

    widget_indices[Y_WORLD_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_text_entry( &ui_info->graphics_window, viewport_index,
                       x_start + dx, y,
                       Position_values_width, Text_entry_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       pos_world_callback, (void *) VIO_Y ) ) - start_index;

    widget_indices[Z_WORLD_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_text_entry( &ui_info->graphics_window, viewport_index,
                       x_start + 2 * dx, y,
                       Position_values_width, Text_entry_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       pos_world_callback, (void *) VIO_Z ) ) - start_index;

    if (show_time)
    {
        y -= Text_entry_height + Volume_y_spacing;
        x = x_start + 2 * dx - (Position_label_width + Position_values_separation);

        widget_indices[TIME_LABEL] = add_widget_to_list(
                  &ui_info->widget_list[viewport_index],
                  create_label( &ui_info->graphics_window, viewport_index, 
                  x, y, Position_label_width, Text_entry_height,
                  "T:", FALSE, LABEL_ACTIVE_COLOUR,
                  LABEL_SELECTED_COLOUR, LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  (Font_types) Label_text_font, 
                  Label_text_font_size ) ) - start_index;

        x += Position_label_width + Position_values_separation;

        widget_indices[TIME_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                  create_text_entry( &ui_info->graphics_window, viewport_index,
                       x, y,
                       Position_values_width, Text_entry_height,
                       FALSE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       pos_time_callback, (void *) NULL ) ) - start_index;
    }

    *height = Text_entry_height;

    return( start_index );
}

  void  set_voxel_position_widgets_activity(
    UI_struct         *ui_info,
    Viewport_types    viewport_index,
    int               start_widget_index,
    VIO_BOOL          activity,
    VIO_BOOL          has_time_axis )
{
    Position_widgets       widget_index;

    for_enum( widget_index, N_POSITION_WIDGETS, Position_widgets )
    {
        int n = start_widget_index + widget_indices[widget_index];
        /* Make sure the time widgets are actually present here.
         */
        if ( n >= ui_info->widget_list[viewport_index].n_widgets )
        {
            continue;
        }

        if (widget_index == TIME_LABEL || widget_index == TIME_TEXT)
        {
            set_widget_activity( ui_info->widget_list[viewport_index].widgets[n],
                                 activity && has_time_axis );
        }
        else
        {
            set_widget_activity( ui_info->widget_list[viewport_index].widgets[n],
                                 activity );
        }
    }
}

  void  set_volume_voxel_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    VIO_Real              value )
{
    Viewport_types  viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_text_entry_real_value( ui_info->widget_list[viewport_index].widgets
                           [ui_info->position_text_start_index[volume_index]+
                            widget_indices[X_VOXEL_TEXT+view_index]],
                            Position_values_format,
                            value );
}

  void  set_volume_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               view_index,
    VIO_Real              value )
{
    Viewport_types  viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_text_entry_real_value( ui_info->widget_list[viewport_index].widgets
                           [ui_info->position_text_start_index[volume_index]+
                            widget_indices[X_WORLD_TEXT+view_index]],
                            Position_values_format,
                            value );
}

  void  set_volume_time_text(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_Real          value )
{
    Viewport_types viewport_index = get_volume_menu_viewport_index( volume_index );
    int n = ui_info->position_text_start_index[volume_index]+widget_indices[TIME_TEXT];

    /* Make sure the time widgets are actually present here.
     */
    if ( n >= ui_info->widget_list[viewport_index].n_widgets )
    {
        return;
    }

    set_text_entry_real_value( ui_info->widget_list[viewport_index].widgets[n],
                               Position_values_format, value );
}

static  void   set_voxel_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            axis )
{
    int    volume_index;
    int    start_widget_index;
    VIO_Real   value;
    VIO_Real   position[VIO_N_DIMENSIONS];

    volume_index = get_viewport_volume_index( widget->viewport_index );
    start_widget_index = ui_info->position_text_start_index[volume_index];

    IF_get_volume_voxel_position( volume_index, position );

    if( get_text_entry_real_value( ui_info->widget_list
                                     [widget->viewport_index].widgets
                                     [start_widget_index+
                                      widget_indices[X_VOXEL_TEXT+axis]],
                                   &value ) )
    {
        position[axis] = value;
    }

    ui_set_volume_voxel_position( ui_info, volume_index, position );
}

static  void   set_world_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            axis )
{
    int    volume_index;
    VIO_Real   value;
    int    start_widget_index;
    VIO_Real   position[VIO_N_DIMENSIONS];

    volume_index = get_viewport_volume_index( widget->viewport_index );
    start_widget_index = ui_info->position_text_start_index[volume_index];

    volume_index = get_viewport_volume_index( widget->viewport_index );

    IF_get_volume_original_world_position( volume_index, position );

    if( get_text_entry_real_value( ui_info->widget_list
                                     [widget->viewport_index].widgets
                                     [start_widget_index+
                                      widget_indices[X_WORLD_TEXT+axis]],
                                   &value ) )
    {
        position[axis] = value;
    }

    ui_set_volume_original_world_position( ui_info, volume_index, position );
}

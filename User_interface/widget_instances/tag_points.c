/**
 * \file User_interface/widget_instances/tag_points.c
 * \brief Create widgets for creating and editing tag points.
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

/* Because we pass a lot of integers as void * callback data, and
 * convert that callback data back to integers, these macros are
 * useful to avoid compiler warnings. These macros should work on 
 * most 32 and 64 bit systems, I think.
 */
#define INT_TO_PTR(i) (void *)(intptr_t)(i)
#define PTR_TO_INT(p) (int)(intptr_t)(p)

typedef enum
{
    RMS_ERROR_BUTTON,
    RMS_ERROR_NUMBER,
    N_RMS_WIDGETS
} Rms_widgets;

typedef enum
{
    WORLD_POSITION_BUTTON,
    WORLD_X_POSITION_TEXT,
    WORLD_Y_POSITION_TEXT,
    WORLD_Z_POSITION_TEXT,
    N_POSITION_WIDGETS
} Position_widgets;

typedef enum
{
    TAG_NUMBER_BUTTON,
    TAG_ACTIVITY_BUTTON,
    TAG_POINT_NAME,
    N_TAG_NAME_WIDGETS
} Tag_name_widgets;

static  int  **rms_widget_indices;
static  int  ***position_widgets_indices;
static  int  **tag_name_widget_indices;

static  int  get_tag_index(
    UI_struct      *ui_info,
    int            tag );

static  void   type_in_world_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            tag,
    int            volume_index,
    int            axis );

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( world_x_position_callback )
{
    int n = widget->viewport_index - Volume_1_tags_viewport;
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     PTR_TO_INT(callback_data), n, VIO_X );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( world_y_position_callback )
{
    int n = widget->viewport_index - Volume_1_tags_viewport;
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     PTR_TO_INT(callback_data), n, VIO_Y );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( world_z_position_callback )
{
    int n = widget->viewport_index - Volume_1_tags_viewport;
    type_in_world_position_callback( get_ui_struct(), widget, 
                                     PTR_TO_INT(callback_data), n, VIO_Z );
}

static  void  set_and_jump_to_tag(
     int  tag_index )
{
    VIO_Real  position[VIO_N_DIMENSIONS];
    int   volume_index, tag_volume;
    UI_struct *ui_struct = get_ui_struct();
    set_current_tag_index( ui_struct, tag_index );

    if( IF_get_tags_visibility() )
    {
        for_less( volume_index, 0, ui_struct->n_volumes_displayed )
        {
            if( volume_index == MERGED_VOLUME_INDEX )
                tag_volume = 0;
            else
                tag_volume = volume_index;

            if( IF_volume_is_loaded( volume_index ) &&
                IF_get_tag_point_position( tag_index, tag_volume, position ) )
            {
                ui_set_volume_original_world_position( get_ui_struct(),
                                                       volume_index, position );
            }
        }
    }
}

static  void  set_current_tag_from_button(
    void           *callback_data )
{
    set_and_jump_to_tag( get_tag_index(get_ui_struct(), PTR_TO_INT(callback_data)) );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( tag_name_callback )
{
    int     tag_index;
    VIO_STR  name;

    name = get_text_entry_string( widget );
    tag_index = get_tag_index( get_ui_struct(), PTR_TO_INT(callback_data) );

    IF_set_tag_point_name( tag_index, name );

    set_current_tag_from_button( callback_data );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( tag_number_button_callback )
{
    set_current_tag_from_button( callback_data );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( tag_activity_callback )
{
    int   tag_index;

    tag_index = get_tag_index( get_ui_struct(), PTR_TO_INT(callback_data) );

    IF_set_tag_point_activity( tag_index, get_toggle_button_state( widget ) );

    set_current_tag_from_button( callback_data );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( world_button_callback )
{
    set_current_tag_from_button( callback_data );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( rms_error_callback )
{
    set_current_tag_from_button( callback_data );
}

  void  previous_current_tag_point( UI_struct *ui )
{
    int  tag_index;

    tag_index =  get_current_tag_index( ui );

    if( tag_index > 0 )
    {
        --tag_index;
        set_and_jump_to_tag( tag_index );
    }
}

  void  advance_current_tag_point( UI_struct  *ui )
{
    int  tag_index;

    tag_index =  get_current_tag_index( ui );

    if( tag_index < IF_get_n_tag_points() )
    {
        ++tag_index;
        set_and_jump_to_tag( tag_index );
    }
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( prev_tag_button_callback )
{
    previous_current_tag_point( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( next_tag_button_callback )
{
    advance_current_tag_point( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( end_tags_button_callback )
{
    int  tag_index;

    tag_index =  get_current_tag_index( get_ui_struct() );

    if( tag_index < IF_get_n_tag_points() )
    {
        set_current_tag_index( get_ui_struct(), IF_get_n_tag_points() );
    }
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( start_tags_button_callback )
{
    int  tag_index;

    tag_index =  get_current_tag_index( get_ui_struct() );

    if( tag_index != 0 )
    {
        set_current_tag_index( get_ui_struct(), 0 );
    }
}

static  Viewport_types  get_tag_menu_viewport_index(
    int   volume_index )
{
    return (Volume_1_tags_viewport + volume_index);
}

  void  set_tag_world_text(
    UI_struct         *ui_info,
    int               volume_index,
    int               tag_index,
    int               axis_index,
    VIO_Real              value )
{
    Viewport_types  viewport_index;

    viewport_index = get_tag_menu_viewport_index( volume_index );

    set_text_entry_real_value( ui_info->widget_list
                           [viewport_index].widgets
                           [position_widgets_indices[volume_index][tag_index]
                            [WORLD_X_POSITION_TEXT+axis_index]],
                            Position_values_format, value );
}

  void  add_tag_point_widgets(
    UI_struct         *ui_info,
    int               n_tag_points,
    Viewport_types    rms_viewport_index,
    Viewport_types    volume1_viewport_index,
    Viewport_types    volume2_viewport_index,
    Viewport_types    names_viewport_index )
{
    int      tag, x, y, x_left, y_top, x_min, x_max, y_min, y_max;
    int      vol;

    VIO_ALLOC2D( rms_widget_indices, n_tag_points, N_RMS_WIDGETS );
    VIO_ALLOC3D( position_widgets_indices, N_VOLUMES, n_tag_points, N_POSITION_WIDGETS );
    VIO_ALLOC2D( tag_name_widget_indices, n_tag_points, N_TAG_NAME_WIDGETS );

    get_graphics_viewport( &ui_info->graphics_window.graphics,
                           (int) rms_viewport_index,
                           &x_min, &x_max, &y_min, &y_max );

    x_left = Tags_x_spacing;
    y_top = y_max - y_min - 1 - Tags_y_spacing;

    x = x_left;
    y = y_top - Tag_point_height;

    for_less( tag, 0, n_tag_points )
    {
        x = x_left;

        rms_widget_indices[tag][RMS_ERROR_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[rms_viewport_index],
                   create_button( &ui_info->graphics_window, rms_viewport_index,
                   x, y, Rms_button_width, Tag_point_height,
                   "Dst:",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   rms_error_callback, INT_TO_PTR(tag) ) );

        rms_widget_indices[tag][RMS_ERROR_NUMBER] =
                   add_widget_to_list(
                   &ui_info->widget_list[rms_viewport_index],
                   create_label( &ui_info->graphics_window,
                   rms_viewport_index,
                   x + Interface_x_spacing + Rms_button_width, y,
                   Rms_number_width, Tag_point_height,
                   "", FALSE, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   LABEL_INACTIVE_COLOUR,
                   LABEL_TEXT_COLOUR,
                   (Font_types) Label_text_font, Label_text_font_size ) );

        for_less(vol, 0, ui_info->n_volumes_loaded)
        {
          x = x_left;
          position_widgets_indices[vol][tag][WORLD_POSITION_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume1_viewport_index+vol],
                   create_button( &ui_info->graphics_window,
                   volume1_viewport_index+vol,
                   x, y, Tag_world_button_width, Tag_point_height,
                   "World:",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   world_button_callback, INT_TO_PTR(tag) ) );


          x += Tag_world_button_width + Position_values_separation;

          position_widgets_indices[vol][tag][WORLD_X_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume1_viewport_index+vol],
                   create_text_entry(&ui_info->graphics_window,
                       volume1_viewport_index+vol, x, y,
                       Tag_position_width, Tag_point_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       world_x_position_callback, INT_TO_PTR(tag) ) );

          x += Tag_position_width + Position_values_separation;

          position_widgets_indices[vol][tag][WORLD_Y_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume1_viewport_index+vol],
                   create_text_entry(&ui_info->graphics_window,
                       volume1_viewport_index+vol, x, y,
                       Tag_position_width, Tag_point_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       world_y_position_callback, INT_TO_PTR(tag) ) );

        x += Tag_position_width + Position_values_separation;

        position_widgets_indices[vol][tag][WORLD_Z_POSITION_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[volume1_viewport_index+vol],
                   create_text_entry(&ui_info->graphics_window,
                       volume1_viewport_index+vol, x, y,
                       Tag_position_width, Tag_point_height,
                       TRUE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       world_z_position_callback, INT_TO_PTR(tag) ) );
        }

        x = x_left;

        tag_name_widget_indices[tag][TAG_NUMBER_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Tag_number_button_width, Tag_point_height,
                   "",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   tag_number_button_callback, INT_TO_PTR(tag) ) );

        x += Tag_number_button_width + Interface_x_spacing;

        tag_name_widget_indices[tag][TAG_ACTIVITY_BUTTON] =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_toggle_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Tag_activity_width, Tag_point_height,
                   "Ignore", "On",
                   TRUE,
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   tag_activity_callback, INT_TO_PTR(tag) ) );

        x += Tag_activity_width + Interface_x_spacing;

        tag_name_widget_indices[tag][TAG_POINT_NAME] =
                   add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_text_entry( &ui_info->graphics_window,
                       names_viewport_index, x, y,
                       Tag_name_width, Tag_point_height,
                       FALSE, "", FALSE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       tag_name_callback, INT_TO_PTR(tag) ) );

        x += Tag_name_width + Interface_x_spacing;

        if( tag == 0 )
        {
            (void) add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Advance_tags_button_width, Tag_point_height,
                   "First",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   start_tags_button_callback, (void *) NULL ) );
        }
        else if( tag == 1 )
        {
            (void) add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Advance_tags_button_width, Tag_point_height,
                   "^",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   prev_tag_button_callback, (void *) NULL ) );
        }
        else if( tag == 2 )
        {
            (void) add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Advance_tags_button_width, Tag_point_height,
                   "v",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   next_tag_button_callback, (void *) NULL ) );
        }
        else if( tag == n_tag_points-1 )
        {
            (void) add_widget_to_list(
                   &ui_info->widget_list[names_viewport_index],
                   create_button( &ui_info->graphics_window,
                   names_viewport_index,
                   x, y, Advance_tags_button_width, Tag_point_height,
                   "Last",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   end_tags_button_callback, (void *) NULL ) );
        }

        y -= Tag_point_height + Tags_y_spacing;
    }

    update_all_tag_widgets( ui_info );
}

  void  delete_tag_point_widgets_indices( void )
{
    VIO_FREE2D( rms_widget_indices );
    VIO_FREE3D( position_widgets_indices );
    VIO_FREE2D( tag_name_widget_indices );
}

static  void  update_rms_error(
    UI_struct         *ui_info,
    int               tag )
{
    VIO_Real           rms;
    widget_struct  *widget;

    widget = ui_info->widget_list[RMS_error_viewport].widgets
                   [rms_widget_indices[tag][RMS_ERROR_NUMBER]];

    if( get_tag_index(ui_info,tag) < IF_get_n_tag_points() &&
        IF_get_tag_point_rms_error( get_tag_index(ui_info,tag), &rms ) )
    {
        set_text_entry_real_value( widget, Rms_error_format, rms );
    }
    else
    {
        set_text_entry_string( widget, "" );
    }
}

static  void  update_tag_position(
    UI_struct         *ui_info,
    int               volume_index,
    int               tag )
{
    VIO_BOOL        exists;
    int            i;
    VIO_Real           position[VIO_N_DIMENSIONS];
    widget_struct  *widget;

    exists = get_tag_index(ui_info,tag) < IF_get_n_tag_points() &&
             IF_get_tag_point_position( get_tag_index(ui_info,tag),
                                        volume_index, position );

    for_less( i, 0, VIO_N_DIMENSIONS )
    {
        widget = ui_info->widget_list[Volume_1_tags_viewport+volume_index].
                 widgets[position_widgets_indices[volume_index][tag]
                        [WORLD_X_POSITION_TEXT+i]];

        if( exists )
        {
            set_text_entry_real_value( widget, Position_values_format,
                                       position[i] );
        }
        else
        {
            set_text_entry_string( widget, "" );
        }
    }
}

static  void  update_tag_name(
    UI_struct         *ui_info,
    int               tag )
{
    VIO_STR         name;
    int            tag_index;
    VIO_BOOL        exists;
    widget_struct  *widget;

    tag_index = get_tag_index(ui_info,tag);

    exists = tag_index < IF_get_n_tag_points();

    widget = ui_info->widget_list[Tag_names_viewport].widgets
                    [tag_name_widget_indices[tag][TAG_NUMBER_BUTTON]];

    if( exists )
    {
        set_button_int_value( widget, Tag_number_format, tag_index+1 );
    }
    else
    {
        set_button_text( widget, "" );
    }

    widget = ui_info->widget_list[Tag_names_viewport].widgets
                          [tag_name_widget_indices[tag][TAG_ACTIVITY_BUTTON]];

    set_widget_activity( widget, exists );

    if( exists )
        set_toggle_button_state( widget, IF_get_tag_point_activity(tag_index) );

    widget = ui_info->widget_list[Tag_names_viewport].widgets
                 [tag_name_widget_indices[tag][TAG_POINT_NAME]];

    if( exists )
        name = IF_get_tag_point_name( tag_index );
    else
        name = "";

    set_text_entry_string( widget, name );
}

static  void  set_widget_activity_and_selected(
    widget_struct  *widget,
    VIO_BOOL        activity,
    VIO_BOOL        selected )
{
    set_widget_activity( widget, activity );
    set_widget_selected( widget, selected );
}

  void  update_tag_widgets(
    UI_struct         *ui_info,
    int               tag )
{
    int              tag_index;
    Rms_widgets      rms_widget_index;
    Position_widgets pos_widget_index;
    Tag_name_widgets tag_widget_index;
    VIO_BOOL         exists, selected;
    int              i;

    exists = (ui_info->tag_points.first_tag_displayed + tag <=
              IF_get_n_tag_points());

    tag_index = get_tag_index(ui_info,tag);
    selected = tag_index == ui_info->tag_points.current_tag_index;

    for_enum( rms_widget_index, N_RMS_WIDGETS, Rms_widgets )
    {
        set_widget_activity_and_selected(
                ui_info->widget_list[RMS_error_viewport].widgets
                             [rms_widget_indices[tag][rms_widget_index]],
                 exists, selected );
    }

    for_enum( pos_widget_index, N_POSITION_WIDGETS, Position_widgets )
    {
        for_less(i, 0, ui_info->n_volumes_loaded)
        {
            set_widget_activity_and_selected(
              ui_info->widget_list[Volume_1_tags_viewport + i].
              widgets[position_widgets_indices[i][tag][pos_widget_index]],
              exists, selected );
        }
    }

    for_enum( tag_widget_index, N_TAG_NAME_WIDGETS, Tag_name_widgets )
    {
        set_widget_activity_and_selected(
              ui_info->widget_list[Tag_names_viewport].widgets
                     [tag_name_widget_indices[tag][tag_widget_index]], exists,
              selected );
    }

    update_rms_error( ui_info, tag );
    for (i = 0; i < ui_info->n_volumes_loaded; i++) 
      update_tag_position( ui_info, i, tag );
    update_tag_name( ui_info, tag );

    set_transform_buttons_activity( ui_info, IF_does_transform_exist() );
}

  void  update_all_tag_widgets(
    UI_struct         *ui_info )
{
    int  tag;

    for_less( tag, 0, ui_info->tag_points.n_tags_displayed )
        update_tag_widgets( ui_info, tag );
    update_avg_rms_error( ui_info );
}

static  void   type_in_world_position_callback(
    UI_struct      *ui_info,
    widget_struct  *widget,
    int            tag,
    int            volume_index,
    int            axis )
{
    int    tag_index;
    VIO_Real   value;
    VIO_Real   position[VIO_N_DIMENSIONS];

    if( get_text_entry_real_value( widget, &value ) )
    {
        tag_index = get_tag_index( ui_info, tag );

        (void) IF_get_tag_point_position( tag_index, volume_index, position );
        position[axis] = value;
        IF_set_tag_point_position( tag_index, volume_index, position );
        set_and_jump_to_tag( tag_index );
    }
}

  void  set_current_tag_index(
    UI_struct      *ui_info,
    int            tag_index )
{
    ui_info->tag_points.current_tag_index = tag_index;

    while( tag_index < ui_info->tag_points.first_tag_displayed )
        --ui_info->tag_points.first_tag_displayed;

    while( tag_index >= ui_info->tag_points.first_tag_displayed +
                        ui_info->tag_points.n_tags_displayed )
        ++ui_info->tag_points.first_tag_displayed;

    update_all_tag_widgets( ui_info );
}

static  int  get_tag_index(
    UI_struct      *ui_info,
    int            tag )
{
    return( ui_info->tag_points.first_tag_displayed + tag );
}

  int  get_current_tag(
    UI_struct      *ui_info )
{
    return( ui_info->tag_points.current_tag_index -
            ui_info->tag_points.first_tag_displayed );
}

  int  get_current_tag_index(
    UI_struct      *ui_info )
{
    return( ui_info->tag_points.current_tag_index );
}

static  void  delete_tag_point(
    UI_struct  *ui,
    int        tag_index )
{
    if( tag_index < IF_get_n_tag_points() )
    {
        IF_delete_tag_point( tag_index );
        if( tag_index > IF_get_n_tag_points() )
            --tag_index;
        set_current_tag_index( ui, tag_index );
    }
}

  void  delete_current_tag_point(
    UI_struct  *ui )
{
    int      tag_index;

    tag_index = get_current_tag_index( ui );

    delete_tag_point( ui, tag_index );
}

  void  delete_all_tag_points(
    UI_struct  *ui )
{
    while( IF_get_n_tag_points() > 0 )
        delete_tag_point( ui, 0 );
}

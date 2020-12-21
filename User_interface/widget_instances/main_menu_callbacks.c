/**
 * \file User_interface/widget_instances/main_menu_callbacks.c
 * \brief Implement the commands on the left panel.
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

typedef  enum
{
    QUIT_BUTTON,
    RESAMPLE_BUTTON,
    RESAMPLE_FILENAME_ENTRY,
    SYNC_VOLUMES_BUTTON,
    LOAD_TAGS_BUTTON,
    TAGS_FILENAME_ENTRY,
    SAVE_TAGS_BUTTON,
    SAVE_TRANSFORM_BUTTON,
    TRANSFORM_FILENAME_ENTRY,
    RECORD_TAG_BUTTON,
    DELETE_TAG_BUTTON,
    DELETE_ALL_TAGS_BUTTON,
    TAG_VISIBILITY_BUTTON,
    TRANSFORM_TYPE_BUTTON,
    CURSOR_VISIBILITY_BUTTON,
    INTERPOLATION_BUTTON,
    AVG_RMS_LABEL,
    AVG_RMS_ERROR,
    N_MAIN_WIDGETS
} Main_widgets;

static  int  widget_indices[N_MAIN_WIDGETS];

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( quit_button_callback )
{
#ifdef DEBUG
    set_quit_program_flag();
#else
    popup_quit_confirm( get_ui_struct() );
#endif
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( transform_type_callback )
{
    popup_transform_dialog( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( resample_and_load_button_callback )
{
    VIO_General_transform  *transform;
    VIO_STR             filename;

    filename = get_text_entry_string(
                     get_ui_struct()->widget_list[Main_menu_viewport].widgets
                                 [widget_indices[RESAMPLE_FILENAME_ENTRY]] );

    if( blank_string( filename ) )
        print( "You must enter a filename before pressing save.\n" );
    else if( !IF_get_resampling_transform(&transform) )
    {
        print( "There are not enough tag points\n" );
        print( "to have a resampling transformation.\n" );
    }
    else
    {
        do_resampling( get_ui_struct(), transform, filename );
    }
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( resample_filename_callback )
{
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( sync_volumes_button_callback )
{
    get_ui_struct()->volumes_synced = !get_ui_struct()->volumes_synced;

    if( get_ui_struct()->volumes_synced )
    {
        update_other_volume_positions( get_ui_struct(), 0 );
    }
}

  void  load_tags_file(
    UI_struct  *ui,
    VIO_STR     filename )
{
    IF_load_tags_file( filename );

    ui->tag_points.first_tag_displayed = 0;
    set_current_tag_index( ui, 0 );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( load_tags_button_callback )
{
    VIO_STR   filename;

    filename = get_text_entry_string(
                     get_ui_struct()->widget_list[Main_menu_viewport].widgets
                                 [widget_indices[TAGS_FILENAME_ENTRY]] );

    if( blank_string( filename ) )
        print( "You must enter a filename before pressing load.\n" );
    else
    {
        load_tags_file( get_ui_struct(), filename );
    }
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( tags_filename_hit_return_callback )
{
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( save_tags_button_callback )
{
    VIO_STR   filename;

    filename = get_text_entry_string(
                     get_ui_struct()->widget_list[Main_menu_viewport].widgets
                                 [widget_indices[TAGS_FILENAME_ENTRY]] );

    if( blank_string( filename ) )
        print( "You must enter a filename before pressing save.\n" );
    else
        IF_save_tags_file( filename );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( transform_filename_hit_return_callback )
{
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( save_transform_button_callback )
{
    VIO_STR   filename;

    filename = get_text_entry_string(
                     get_ui_struct()->widget_list[Main_menu_viewport].widgets
                                 [widget_indices[TRANSFORM_FILENAME_ENTRY]] );

    if( blank_string( filename ) )
        print( "You must enter a filename before pressing save.\n" );
    else
        IF_save_transform( filename );
}

  void  record_tag(
    UI_struct  *ui,
    int        tag_index )
{
    int      volume;
    VIO_Real     position[VIO_N_DIMENSIONS];

    if( tag_index >= IF_get_n_tag_points() )
        IF_create_new_tag_point();

    for_less( volume, 0, ui->n_volumes_loaded )
    {
        if( IF_volume_is_loaded( volume ) )
        {
            IF_get_volume_original_world_position( volume, position );
            IF_set_tag_point_position( tag_index, volume, position );
        }
    }

    if( tag_index == IF_get_n_tag_points() - 1 )
        ++tag_index;

    set_current_tag_index( ui, tag_index );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( record_tag_button_callback )
{
    record_tag( get_ui_struct(), get_current_tag_index(get_ui_struct()) );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( delete_tag_button_callback )
{
    delete_current_tag_point( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( delete_all_tags_button_callback )
{
    if( IF_get_n_tag_points() > 0 )
    {
        popup_delete_tags( get_ui_struct() );
    }
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( tag_visibility_button_callback )
{
    VIO_BOOL   visibility;

    visibility = get_toggle_button_state( widget );

    IF_set_tags_visibility( visibility );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( cursor_visibility_button_callback )
{
    VIO_BOOL   visibility;

    visibility = get_toggle_button_state( widget );

    IF_set_cursor_visibility( visibility );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( interpolation_button_callback )
{
    VIO_BOOL   smooth_flag;

    smooth_flag = get_toggle_button_state( widget );

    IF_set_interpolation_flag( smooth_flag );
}

  void  add_main_widgets(
    UI_struct         *ui_info )
{
    widget_indices[QUIT_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window, Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Quit",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   quit_button_callback, (void *) 0 ) );

    widget_indices[RESAMPLE_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Resample", 
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   resample_and_load_button_callback, (void *) 0 ) );

    widget_indices[RESAMPLE_FILENAME_ENTRY] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_text_entry( &ui_info->graphics_window,
                       Main_menu_viewport, 
                       0, 0, Tags_filename_width, Text_entry_height,
                       FALSE, "", TRUE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       resample_filename_callback, (void *) 0 ) );

    widget_indices[SYNC_VOLUMES_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_toggle_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Not Synced",
                   "Synced",
                   ui_info->volumes_synced, TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   sync_volumes_button_callback, (void *) 0 ) );

    widget_indices[LOAD_TAGS_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Load Tags",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   load_tags_button_callback, (void *) 0 ) );

    widget_indices[TAGS_FILENAME_ENTRY] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_text_entry( &ui_info->graphics_window,
                       Main_menu_viewport, 
                       0, 0, Tags_filename_width, Text_entry_height,
                       FALSE, "", TRUE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       tags_filename_hit_return_callback, (void *) 0 ) );

    widget_indices[SAVE_TAGS_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Save Tags",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   save_tags_button_callback, (void *) 0 ) );

    widget_indices[SAVE_TRANSFORM_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Save Transform",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   save_transform_button_callback, (void *) 0 ) );

    widget_indices[TRANSFORM_FILENAME_ENTRY] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_text_entry( &ui_info->graphics_window,
                       Main_menu_viewport, 
                       0, 0, Tags_filename_width, Text_entry_height,
                       FALSE, "", TRUE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       transform_filename_hit_return_callback, (void *) 0 ) );

    widget_indices[RECORD_TAG_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Record Tag",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   record_tag_button_callback, (void *) 0 ) );

    widget_indices[DELETE_TAG_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Delete Tag",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   delete_tag_button_callback, (void *) 0 ) );

    widget_indices[DELETE_ALL_TAGS_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Delete All Tags",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   delete_all_tags_button_callback, (void *) 0 ) );

    widget_indices[TRANSFORM_TYPE_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Transform Type",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   transform_type_callback, (void *) 0 ) );

    widget_indices[TAG_VISIBILITY_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_toggle_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Tags: Invisible",
                   "Tags: Visible",
                   IF_get_tags_visibility(),
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   tag_visibility_button_callback, (void *) 0 ) );

    widget_indices[CURSOR_VISIBILITY_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_toggle_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Cursor: Invisible",
                   "Cursor: Visible",
                   IF_get_cursor_visibility(),
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   cursor_visibility_button_callback, (void *) 0 ) );

    widget_indices[INTERPOLATION_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[Main_menu_viewport],
                   create_toggle_button( &ui_info->graphics_window,
                   Main_menu_viewport, 
                   0, 0, Button_width, Button_height,
                   "Interp: Flat",
                   "Interp: Smooth",
                   IF_get_interpolation_flag(),
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   interpolation_button_callback, (void *) 0 ) );

    widget_indices[AVG_RMS_LABEL] = add_widget_to_list(
                  &ui_info->widget_list[Main_menu_viewport],
                  create_label( &ui_info->graphics_window, Main_menu_viewport,
                  0, 0, Avg_rms_label_width, Text_entry_height,
                  "RMS:", TRUE, LABEL_ACTIVE_COLOUR, LABEL_SELECTED_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  (Font_types) Label_text_font, Label_text_font_size ) );

    widget_indices[AVG_RMS_ERROR] = add_widget_to_list(
                  &ui_info->widget_list[Main_menu_viewport],
                  create_label( &ui_info->graphics_window, Main_menu_viewport,
                  0, 0,
                  Avg_rms_number_width, Text_entry_height,
                  "", TRUE, LABEL_ACTIVE_COLOUR, LABEL_SELECTED_COLOUR,
                  LABEL_INACTIVE_COLOUR,
                  LABEL_TEXT_COLOUR,
                  (Font_types) Label_text_font, Label_text_font_size ) );

    position_main_widgets( ui_info );
}

  void  position_main_widgets(
    UI_struct         *ui_info )
{
    int           x, y_pos, x_pos, x_min, x_max, y_min, y_max;
    Main_widgets  widget;

    get_graphics_viewport( &ui_info->graphics_window.graphics,
                           Main_menu_viewport,
                           &x_min, &x_max, &y_min, &y_max );

    x = Main_menu_x_offset;
    y_pos = y_max - y_min - 1 - Main_menu_y_offset;

    for_enum( widget, N_MAIN_WIDGETS, Main_widgets )
    {
        if( widget == TAGS_FILENAME_ENTRY ||
            widget == TRANSFORM_FILENAME_ENTRY ||
            widget == RESAMPLE_FILENAME_ENTRY )
            x_pos = Tags_filename_x_offset;
        else
            x_pos = x;

        position_widget( ui_info->
                         widget_list[Main_menu_viewport].widgets[widget],
                         x_pos, y_pos );

        y_pos -= get_widget_height( ui_info->widget_list[Main_menu_viewport].
                                                      widgets[widget] ) +
                 Interface_y_spacing;

        if( widget == QUIT_BUTTON ||
            widget == RESAMPLE_FILENAME_ENTRY ||
            widget == SAVE_TAGS_BUTTON ||
            widget == TRANSFORM_FILENAME_ENTRY )
        {
            y_pos -= Interface_y_spacing;
        }
    }

    y_pos = Main_menu_y_offset;

    position_widget( ui_info->
                     widget_list[Main_menu_viewport].
                               widgets[widget_indices[AVG_RMS_LABEL]],
                     Tags_x_spacing, y_pos );

    position_widget( ui_info->
                     widget_list[Main_menu_viewport].
                               widgets[widget_indices[AVG_RMS_ERROR]],
                     Tags_x_spacing + Interface_x_spacing + Avg_rms_label_width,
                     y_pos );
}

  void  set_transform_buttons_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity )
{
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[SAVE_TRANSFORM_BUTTON]], activity );
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[RESAMPLE_BUTTON]], activity );
}

  void  set_quit_button_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity )
{
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[QUIT_BUTTON]], activity );
}

  void  set_resample_button_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity )
{
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[RESAMPLE_BUTTON]], activity );
}

  void  set_delete_tags_button_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity )
{
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[DELETE_ALL_TAGS_BUTTON]], activity );
}

  void  set_transform_type_button_activity(
    UI_struct         *ui_info,
    VIO_BOOL           activity )
{
    set_widget_activity( ui_info->widget_list[Main_menu_viewport].widgets
                         [widget_indices[TRANSFORM_TYPE_BUTTON]], activity );
}

  void  update_avg_rms_error(
    UI_struct         *ui_info )
{
    VIO_Real           avg_rms;
    widget_struct  *widget;

    widget = ui_info->widget_list[Main_menu_viewport].
                       widgets[widget_indices[AVG_RMS_ERROR]];
    if( IF_get_tag_point_avg_rms_error( &avg_rms ) )
    {
        set_text_entry_real_value( widget, Rms_error_format, avg_rms );
    }
    else
    {
        set_text_entry_string( widget, "" );
    }
}


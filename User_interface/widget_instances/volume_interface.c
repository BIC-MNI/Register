/* ----------------------------------------------------------------------------
@COPYRIGHT  :
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

#include  <user_interface.h>

typedef  enum
{
    RESET_VIEW_BUTTON,
    POPUP_FILTER_BUTTON,
    VALUE_READOUT_TEXT,
    LOAD_BUTTON,
    LOAD_FILENAME_TEXT,
    RESAMPLED_LABEL,

    N_VOLUME_WIDGETS
}
Volume_widgets;

static  int  widget_indices[N_VOLUME_WIDGETS];
static  int  colour_bar_start_index;

/* ARGSUSED */

  DEFINE_WIDGET_CALLBACK( reset_view_callback )
{
    int   volume_index, view_index;

    volume_index = get_viewport_volume_index(widget->viewport_index);

    for_less( view_index, 0, N_VIEWS )
        IF_reset_slice_view( volume_index, view_index );
}

/* ARGSUSED */

  DEFINE_WIDGET_CALLBACK( popup_filter_callback )
{
    int   volume_index;

    volume_index = get_viewport_volume_index(widget->viewport_index);

    set_widget_activity( widget, FALSE );
    popup_filter_selection( get_ui_struct(), volume_index );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( load_volume_callback )
{
    VIO_STR                filename;
    Viewport_types        viewport_index;

    viewport_index = widget->viewport_index;
    filename = get_text_entry_string( 
                     get_ui_struct()->widget_list[viewport_index].widgets
                                 [widget_indices[LOAD_FILENAME_TEXT]] );

    if( blank_string( filename ) )
        print( "You must enter a filename before pressing load.\n" );
    else
    {
        (void) initialize_loading_volume( get_ui_struct(),
                                      get_viewport_volume_index(viewport_index),
                                      filename, FALSE );
    }
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( volume_filename_callback )
{
}

  void  add_volume_widgets(
    UI_struct         *ui_info,
    Viewport_types    viewport_index )
{
    int            x, y, height, volume_index;

    volume_index = get_viewport_volume_index( viewport_index );

    x = Volume_menu_x_offset;
    y = Volume_menu_y_offset;

    colour_bar_start_index = add_colour_bar_widgets( ui_info, viewport_index,
                                                     x, y, &height );

    y += height + Volume_y_spacing;

    widget_indices[RESAMPLED_LABEL] =
                   add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_label( &ui_info->graphics_window, viewport_index,
                   x, y, Load_filename_width, Volume_button_height,
                   "VIO_Volume Resampled", FALSE, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   BACKGROUND_COLOUR,
                   BACKGROUND_COLOUR,
                   (Font_types) Label_text_font, Label_text_font_size ) );

    y += Volume_button_height + Volume_y_spacing;

    widget_indices[LOAD_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x, y, Volume_button_width, Volume_button_height,
                   "Load",
                   TRUE, TRUE, BUTTON_ACTIVE_COLOUR, BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   load_volume_callback, (void *) NULL ) );

    widget_indices[LOAD_FILENAME_TEXT] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_text_entry( &ui_info->graphics_window,
                       viewport_index, 
                       x + Volume_button_width + Interface_x_spacing, y,
                       Load_filename_width, Text_entry_height,
                       FALSE, "", TRUE,
                       TEXT_ENTRY_ACTIVE_COLOUR, TEXT_ENTRY_SELECTED_COLOUR,
                       TEXT_ENTRY_INACTIVE_COLOUR,
                       TEXT_ENTRY_TEXT_COLOUR,
                       TEXT_ENTRY_EDIT_COLOUR,
                       TEXT_ENTRY_EDIT_TEXT_COLOUR,
                       TEXT_ENTRY_CURSOR_COLOUR,
                       (Font_types) Text_entry_font, Text_entry_font_size,
                       volume_filename_callback, (void *) NULL ) );

    y += Text_entry_height + Volume_y_spacing;

    widget_indices[RESET_VIEW_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x, y, Volume_button_width, Volume_button_height,
                   "Reset View",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   reset_view_callback, (void *) NULL ) );

    widget_indices[POPUP_FILTER_BUTTON] = add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_button( &ui_info->graphics_window, viewport_index, 
                   x + Volume_button_width + Interface_x_spacing, y,
                   Filter_button_width, Filter_button_height,
                   "Filter",
                   FALSE, TRUE, BUTTON_ACTIVE_COLOUR,
                   BUTTON_SELECTED_COLOUR,
                   BUTTON_INACTIVE_COLOUR,
                   BUTTON_TEXT_COLOUR,
                   (Font_types) Button_text_font, Button_text_font_size,
                   popup_filter_callback, (void *) NULL ) );

    widget_indices[VALUE_READOUT_TEXT] =
                   add_widget_to_list(
                   &ui_info->widget_list[viewport_index],
                   create_label( &ui_info->graphics_window, viewport_index,
                   x + Volume_button_width + Interface_x_spacing +
                   Filter_button_width + Interface_x_spacing, y,
                   Value_readout_width, Volume_button_height,
                   "", FALSE, LABEL_ACTIVE_COLOUR,
                   LABEL_SELECTED_COLOUR,
                   BACKGROUND_COLOUR,
                   BACKGROUND_COLOUR,
                   (Font_types) Label_text_font, Label_text_font_size ) );


    ui_info->position_text_start_index[volume_index] =
                             add_cursor_position_widgets( ui_info,
                                   viewport_index, &height );
}

  void  set_load_activity(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_BOOL           state )
{
    Viewport_types   viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_widget_activity( ui_info->widget_list
                                 [viewport_index].widgets
                                 [widget_indices[LOAD_BUTTON]],
                         state );
    set_widget_activity( ui_info->widget_list
                                 [viewport_index].widgets
                                 [widget_indices[LOAD_FILENAME_TEXT]],
                         state );
}

  void  set_load_filename(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_STR            filename )
{
    Viewport_types   viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    set_text_entry_string( ui_info->widget_list[viewport_index].widgets
                             [widget_indices[LOAD_FILENAME_TEXT]],
                           filename );
}

  void  set_volume_widgets_activity(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_BOOL           activity,
    VIO_BOOL           is_rgb_volume )
{
    Viewport_types       viewport_index;
    Volume_widgets       widget_index;

    viewport_index = get_volume_menu_viewport_index( volume_index );

    for_enum( widget_index, N_VOLUME_WIDGETS, Volume_widgets )
    {
        if( widget_index != RESAMPLED_LABEL )
        {
            set_widget_activity( ui_info->widget_list[viewport_index].widgets
                                                [widget_indices[widget_index]],
                                 activity );
        }
    }

    if( volume_index == RESAMPLED_VOLUME_INDEX )
    {
        set_resampled_label_activity( ui_info, activity &&
                                      IF_is_resampled_volume_loaded() );
    }
    else
    {
        set_widget_activity( ui_info->widget_list[viewport_index].widgets
                                         [widget_indices[RESAMPLED_LABEL]],
                             FALSE );
    }

    set_colour_bar_widgets_activity( ui_info, viewport_index,
                          colour_bar_start_index, activity && !is_rgb_volume );

    set_voxel_position_widgets_activity( ui_info, viewport_index,
                           ui_info->position_text_start_index[volume_index],
                           activity );
}

  int  get_colour_bar_start_index( void )
{
    return( colour_bar_start_index );
}

  void  set_resampled_label_activity(
    UI_struct         *ui_info,
    VIO_BOOL           state )
{
    Viewport_types       viewport_index;

    viewport_index = get_volume_menu_viewport_index( RESAMPLED_VOLUME_INDEX );

    set_widget_activity( ui_info->widget_list
                                 [viewport_index].widgets
                                 [widget_indices[RESAMPLED_LABEL]],
                         state );
}

  widget_struct  *get_volume_readout_widget(
    UI_struct     *ui_info,
    int           volume )
{
    Viewport_types       viewport_index;

    viewport_index = get_volume_menu_viewport_index( volume );

    return( ui_info->widget_list[viewport_index].widgets
                                 [widget_indices[VALUE_READOUT_TEXT]] );
}

  void  set_filter_popup_activity(
    UI_struct         *ui_info,
    int               volume_index,
    VIO_BOOL           activity )
{
    Viewport_types      viewport;

    viewport = get_volume_menu_viewport_index( volume_index );

    set_widget_activity( ui_info->widget_list[viewport].widgets
                           [widget_indices[POPUP_FILTER_BUTTON]], activity );
}


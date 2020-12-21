/** 
 * \file User_interface/transform_popup/xform_selection.c
 * \brief Implement the transform type selection dialog.
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

static  void  delete_transform_selection(
    UI_struct             *ui,
    popup_struct          *popup )
{
    delete_popup_window( popup );

    FREE( popup );

    set_transform_type_button_activity( ui, TRUE );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( transform_callback )
{
    IF_set_transform_type( *((Trans_type *) callback_data) );
    update_all_tag_widgets( get_ui_struct() );
}

/* ARGSUSED */

static  DEFINE_WIDGET_CALLBACK( close_callback )
{
    delete_transform_selection( get_ui_struct(),
                                (popup_struct *) callback_data );
}

/* ARGSUSED */

static  DEFINE_EVENT_FUNCTION( quit_window_callback )
{
    delete_transform_selection( get_ui_struct(),
                                (popup_struct *) callback_data );
}

/* ARGSUSED */

  void  popup_transform_dialog(
    UI_struct   *ui )
{
    int                          i, x, y;
    popup_struct                 *popup;
    VIO_STR                       window_name = "Transform Type Selection";
    VIO_BOOL                      activity;
    Trans_type                   transform_type;
    static  struct
            {
                Trans_type           transform_type;
                VIO_STR               name;
            }                            choices[] = {
         { TRANS_LSQ6,  "3 Rot'ns & 3 Transl'ns" },
         { TRANS_LSQ7,  "3 Rot'ns & 3 Transl'ns & 1 Scale" },
         { TRANS_LSQ9,  "3 Rot'ns & 3 Transl'ns & 3 Scales" },
         { TRANS_LSQ10, "3 Rot'ns & 3 Transl'ns & 3 Scales & X shear" },
         { TRANS_LSQ12, "Full Affine 12 Parameter" },
         { TRANS_TPS,   "Thin Plate Splines (Non-linear)" }          };

    widget_struct    *widget;
    widget_struct    *radio_widgets[VIO_SIZEOF_STATIC_ARRAY( choices )];

    set_transform_type_button_activity( ui, FALSE );

    ALLOC( popup, 1 );

    G_get_mouse_screen_position( &x, &y );

    create_popup_window( popup, window_name, x, y,
                         Transform_selection_x_size, Transform_selection_y_size,
                         quit_window_callback, (void *) popup );

    initialize_widget_list( &popup->widgets );

    x = Interface_x_spacing;
    y = Transform_selection_y_size - 1 - Interface_y_spacing - Button_height;

    for_less( i, 0, VIO_SIZEOF_STATIC_ARRAY( choices ) )
    {
        if( choices[i].transform_type == TRANS_LSQ9 ||
            choices[i].transform_type == TRANS_LSQ10 )
        {
            activity = Activity_9_10;
        }
        else
        {
            activity = TRUE;
        }

        radio_widgets[i] = create_button( &popup->graphics, Main_menu_viewport,
                                   x, y, Transform_button_width, Button_height,
                                   choices[i].name, activity, FALSE,
                                   get_ui_rgb_colour(BUTTON_ACTIVE_COLOUR),
                                   get_ui_rgb_colour(BUTTON_SELECTED_COLOUR),
                                   get_ui_rgb_colour(BUTTON_INACTIVE_COLOUR),
                                   get_ui_rgb_colour(BUTTON_TEXT_COLOUR),
                                   (Font_types) Button_text_font,
                                   Button_text_font_size,
                                   transform_callback,
                                   (void *) &choices[i].transform_type );

        (void) add_widget_to_list( &popup->widgets, radio_widgets[i] );

        x = Interface_x_spacing;
        y -= Button_height + Interface_y_spacing;
    }

    transform_type = IF_get_transform_type();
    define_radio_buttons( VIO_SIZEOF_STATIC_ARRAY( choices ), radio_widgets );

    for_less( i, 0, VIO_SIZEOF_STATIC_ARRAY( choices ) )
    {
        if( choices[i].transform_type == transform_type )
            set_widget_selected( radio_widgets[i], TRUE );
    }

    y -= Interface_y_spacing;

    widget = create_button( &popup->graphics, Main_menu_viewport,
                            x, y, Button_width, Button_height,
                            "Close", TRUE, FALSE,
                            get_ui_rgb_colour(BUTTON_ACTIVE_COLOUR),
                            get_ui_rgb_colour(BUTTON_SELECTED_COLOUR),
                            get_ui_rgb_colour(BUTTON_INACTIVE_COLOUR),
                            get_ui_rgb_colour(BUTTON_TEXT_COLOUR),
                            (Font_types) Button_text_font,
                            Button_text_font_size,
                            close_callback, (void *) popup );

    (void) add_widget_to_list( &popup->widgets, widget );
}

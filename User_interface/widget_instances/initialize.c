#include  <def_user_interface.h>

public  void  initialize_UI_widgets( UI_struct  *ui_info )
{
    Viewport_types   viewport_index;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        initialize_widget_list( &ui_info->widget_list[viewport_index] );

    add_main_widgets( ui_info );

    add_volume_widgets( ui_info, Volume_1_menu_viewport );
    add_volume_widgets( ui_info, Volume_2_menu_viewport );

    add_merged_widgets( ui_info, Merged_menu_viewport );

    ui_info->tag_points.n_tags_displayed = Default_n_tag_points;
    ui_info->tag_points.first_tag_displayed = 0;
    ui_info->tag_points.current_tag_index = 0;
    add_tag_point_widgets( ui_info, ui_info->tag_points.n_tags_displayed,
                           RMS_error_viewport, Volume_1_tags_viewport,
                           Volume_2_tags_viewport, Tag_names_viewport );
}

public  void  delete_UI_widgets( UI_struct  *ui_info )
{
    Viewport_types   viewport_index;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        delete_widget_list( &ui_info->widget_list[viewport_index] );

    delete_tag_point_widgets_indices();
}

public  void  update_all_widget_colours( UI_struct *ui_info )
{
    Viewport_types   viewport_index;

    for_enum( viewport_index, N_UI_viewports, Viewport_types )
        update_widget_list_colours( &ui_info->widget_list[viewport_index] );
}

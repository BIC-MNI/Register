START_GLOBALS
    DEF_GLOBAL( Disable_alloc_checking, BOOLEAN, TRUE )
    DEF_GLOBAL( Alloc_debugging, BOOLEAN, FALSE )

    DEF_GLOBAL( Scaling_allowed, BOOLEAN, FALSE )

    DEF_GLOBAL( Use_overlay_planes, BOOLEAN, FALSE )
    DEF_GLOBAL( Overlay_colour_1, Colour, RED )
    DEF_GLOBAL( Overlay_colour_2, Colour, GREEN )
    DEF_GLOBAL( Overlay_colour_3, Colour, BLUE )

    DEF_GLOBAL( Slice_1_x_axis_index, int, X )
    DEF_GLOBAL( Slice_1_y_axis_index, int, Y )
    DEF_GLOBAL( Slice_2_x_axis_index, int, Y )
    DEF_GLOBAL( Slice_2_y_axis_index, int, Z )
    DEF_GLOBAL( Slice_3_x_axis_index, int, X )
    DEF_GLOBAL( Slice_3_y_axis_index, int, Z )

    DEF_GLOBAL( Slice_1_x_axis_flip, BOOLEAN, FALSE )
    DEF_GLOBAL( Slice_1_y_axis_flip, BOOLEAN, FALSE )
    DEF_GLOBAL( Slice_2_x_axis_flip, BOOLEAN, FALSE )
    DEF_GLOBAL( Slice_2_y_axis_flip, BOOLEAN, FALSE )
    DEF_GLOBAL( Slice_3_x_axis_flip, BOOLEAN, FALSE )
    DEF_GLOBAL( Slice_3_y_axis_flip, BOOLEAN, FALSE )
    DEF_GLOBAL( Slice_fit_size, Real, 0.9 )
    DEF_GLOBAL( Slice_background_colour, Colour, DARK_SLATE_GREY )

    DEF_GLOBAL( Slice_cursor_offset, int, 4 )
    DEF_GLOBAL( Slice_cursor_size, int, 10 )

    DEF_GLOBAL( Cursor_inside_colour, Colour, RED )
    DEF_GLOBAL( Cursor_outside_colour, Colour, BLUE )

    DEF_GLOBAL( Tag_inside_colour, Colour, CYAN )
    DEF_GLOBAL( Tag_outside_colour, Colour, MAGENTA )
    DEF_GLOBAL( Tag_inside_inactive_colour, Colour, WHITE )
    DEF_GLOBAL( Tag_outside_inactive_colour, Colour, GRAY )
    DEF_GLOBAL( Tag_circle_resolution, int, 8 )
    DEF_GLOBAL( Tag_radius_pixels, int, 5 )

    DEF_GLOBAL( Merged_colour_table_fraction, Real, 0.75 )

    DEF_GLOBAL( Initial_under_colour, Colour, BLACK )
    DEF_GLOBAL( Initial_over_colour, Colour, WHITE )

    DEF_GLOBAL( Initial_tags_visible, BOOLEAN, TRUE )

    DEF_GLOBAL( Initial_merge_method, int, 2 )
    DEF_GLOBAL( Initial_merged_1_weight, Real, 0.5 )
    DEF_GLOBAL( Initial_merged_2_weight, Real, 0.5 )
    DEF_GLOBAL( Use_over_under_colour_in_weights, BOOLEAN, TRUE )

    DEF_GLOBAL_STRING( Resample_command_name, "register_resample" )

    DEF_GLOBAL( Initial_slice_filter_type, int, 0 )
    DEF_GLOBAL( Initial_slice_filter_width, Real, 1.0 )
END_GLOBALS

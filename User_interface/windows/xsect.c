/* X-section window "class" for Register.
 */
#include <float.h>
#include <user_interface.h>

#define XS_MAX_OBJECTS 100
#define X_PIXELS_PER_TICK 64

extern void *get_main_struct();
extern Volume get_slice_volume(void *, int);

struct xs_window {
    struct popup_struct popup;
    int do_scale_yrange;
    object_struct *objects[XS_MAX_OBJECTS];
    int n_objects;
    Real x_min_ui, x_max_ui;
    Real x_min_abs, x_max_abs;
    int file_widget_id;
    int viewport_index; 
};


/* There can be only one cross-section window at a time in this 
 * implementation . */
static struct xs_window *_xswin = NULL;

private void
xs_delete(struct xs_window *xswin)
{
    delete_popup_window(&xswin->popup);

    FREE(xswin);

    _xswin = NULL;
}

private DEFINE_EVENT_FUNCTION( xs_quit_callback )
{
    xs_delete((struct xs_window *) callback_data);
}

private DEFINE_WIDGET_CALLBACK(xs_close_callback)
{
    xs_delete((struct xs_window *) callback_data);
}

private DEFINE_WIDGET_CALLBACK(xs_yscale_callback)
{
    struct xs_window *xswin = (struct xs_window *) callback_data;

    button_struct *button;

    button = get_widget_button(widget);

    xswin->do_scale_yrange = button->state;

    xs_display(get_ui_struct(), xswin->viewport_index, 0);
}

private DEFINE_WIDGET_CALLBACK(xs_save_callback)
{
    struct xs_window *xswin = (struct xs_window *) callback_data;
    char *filename;
    widgets_struct *widgets_ptr;
    FILE *fp;
    int i;
    Real x_min, x_max;
    int x_start, x_end;
    void *main_ptr;
    Volume volume;
    Real voxelpos[MAX_DIMENSIONS];
    Real worldpos[MAX_DIMENSIONS];
    int sizes[MAX_DIMENSIONS];
    Real separations[MAX_DIMENSIONS];
    Real starts[MAX_DIMENSIONS];
    int volume_index;
    int view_index;
    Real vx, vy, vz;
    Real t;

    if (xswin == NULL) {
        return;
    }

    /* TODO: It is just unbelievably gross that the code was designed
     * such that I have to do these next few lines.  Either there is
     * a better way, and I just don't know about it yet, or I need to
     * create one.
     */
    widgets_ptr = &xswin->popup.widgets;
    if (widgets_ptr->n_widgets < xswin->file_widget_id) {
        return;
    }

    filename = get_text_entry_string(widgets_ptr->widgets[xswin->file_widget_id]);
    /* End of grotesque stuff */

    if (blank_string(filename)) {
        print("You must enter a filename before pressing 'save'.\n");
        return;
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
        return;
    }

    /* CUT AND PASTED FROM BELOW - CONSIDER MOVING TO A FUNCTION!!! */

    ui_get_volume_view_index( xswin->viewport_index,
                              &volume_index, &view_index );

    IF_get_volume_voxel_position(volume_index, voxelpos);
    IF_get_volume_world_position(volume_index, worldpos);

    main_ptr = get_main_struct();
    volume = get_slice_volume(main_ptr, volume_index);

    get_volume_sizes(volume, sizes);
    get_volume_starts(volume, starts);
    get_volume_separations(volume, separations);

    vx = ROUND(voxelpos[0]);
    vy = ROUND(voxelpos[1]);
    vz = ROUND(voxelpos[2]);

    x_start = 0;
    x_end = sizes[3];

    /* Default X range is the absolute minimum and maximum */

    x_min = xswin->x_min_abs;
    x_max = xswin->x_max_abs;

    /* However, if the user has overridden these values, adopt them
     * here.
     */
    if (xswin->x_min_ui >= x_min && xswin->x_min_ui <= x_max) {
        x_min = xswin->x_min_ui;
        x_start = (x_min / separations[3]) - starts[3];
    }

    if (xswin->x_max_ui >= x_min && xswin->x_max_ui <= x_max) {
        x_max = xswin->x_max_ui;
        x_end = (x_max / separations[3]) - starts[3];
    }
    /* END OF CUT AND PASTE */

    fprintf(fp, "# File %s\n", get_volume_filename(main_ptr, volume_index));
    fprintf(fp, "# Timecourse of voxel at X=%.2f,Y=%.2f,Z=%.2f\n", 
            worldpos[X], worldpos[Y], worldpos[Z]);

    t = x_min;
    for_less( i, x_start, x_end ) {
        Real value = get_volume_real_value(volume, vx, vy, vz, (Real) i, 0);

        fprintf(fp, "%f %f\n", t, value);

        t += separations[3];    /* Add the delta T */
    }
    fclose(fp);
}

private DEFINE_WIDGET_CALLBACK(xs_name_callback)
{
    struct xs_window *xswin = (struct xs_window *) callback_data;
}

private DEFINE_WIDGET_CALLBACK(xs_min_callback)
{
    struct xs_window *xswin = (struct xs_window *) callback_data;
    Real value;
    int fix;

    fix = 0;
    if (get_text_entry_real_value(widget, &value)) {
        if (value < xswin->x_min_abs) {
            value = xswin->x_min_abs;
            fix = 1;
        }
        if (value > xswin->x_max_abs) {
            value = xswin->x_max_abs;
            fix = 1;
        }
        if (xswin->x_max_ui != DBL_MAX && value > xswin->x_max_ui) {
            value = xswin->x_max_ui;
            fix = 1;
        }
        if (fix) {
            set_text_entry_real_value(widget, "%5.03f", value);
        }
        xswin->x_min_ui = value;
    }
    xs_display(get_ui_struct(), xswin->viewport_index, 0);
}

private DEFINE_WIDGET_CALLBACK(x_max_callback)
{
    struct xs_window *xswin = (struct xs_window *) callback_data;
    Real value;
    int fix;

    fix = 0;
    if (get_text_entry_real_value(widget, &value)) {
        if (value < xswin->x_min_abs) {
            value = xswin->x_min_abs;
            fix = 1;
        }
        if (value > xswin->x_max_abs) {
            value = xswin->x_max_abs;
            fix = 1;
        }
        if (xswin->x_min_ui != DBL_MAX && value < xswin->x_min_ui) {
            value = xswin->x_min_ui;
            fix = 1;
        }
        if (fix) {
            set_text_entry_real_value(widget, "%5.03f", value);
        }
        xswin->x_max_ui = value;
    }
    xs_display(get_ui_struct(), xswin->viewport_index, 0);
}

void
add_object_to_xs_window(struct xs_window *xswin, object_struct *obj_ptr)
{
    if (xswin->n_objects >= XS_MAX_OBJECTS) {
        fprintf(stderr, "Too many objects!\n");
        abort();
    }
    xswin->objects[xswin->n_objects++] = obj_ptr;
}

static object_struct * display_text(struct xs_window *xswin,
                                    int x, int y, char *str_ptr)
{
    object_struct *obj_ptr = create_object( TEXT );
    text_struct *txt_obj_ptr = get_text_ptr( obj_ptr );
    Point point;

    fill_Point( point, x, y, 0.0);
    initialize_text( txt_obj_ptr, &point, BLUE,
                     (Font_types) Message_font, Message_font_size );

    replace_string( &txt_obj_ptr->string, create_string(str_ptr) );

    add_object_to_viewport( &xswin->popup.graphics.graphics, 0, NORMAL_PLANES,
                            obj_ptr );

    add_object_to_xs_window(xswin, obj_ptr);

    return (obj_ptr);
}

void
xs_create_widgets(widgets_struct *widgets,
                  graphics_window_struct *gwin)

{
    widget_struct *widget_ptr;
    char string[128];
    int x = 2;

    initialize_widget_list( widgets );

    widget_ptr = create_toggle_button( gwin, (Viewport_types) 0,
                                       x, 2, Button_width, Button_height,
                                       "Full range", "Scaled", OFF, ON, TRUE, 
                                       BUTTON_ACTIVE_COLOUR,
                                       BUTTON_INACTIVE_COLOUR,
                                       BUTTON_TEXT_COLOUR,
                                       (Font_types) Button_text_font,
                                       Button_text_font_size,
                                       xs_yscale_callback,
                                       (void *) _xswin );
    add_widget_to_list( widgets, widget_ptr );

    x += Button_width + 2 + 10;

    widget_ptr = create_label(gwin, 0,
                              x, 2,
                              35, Text_entry_height,
                              "T(min):",
                              OFF, 
                              LABEL_ACTIVE_COLOUR,
                              LABEL_SELECTED_COLOUR,
                              LABEL_INACTIVE_COLOUR,
                              LABEL_TEXT_COLOUR,
                              (Font_types) Label_text_font,
                              Label_text_font_size );

    add_widget_to_list( widgets, widget_ptr );

    x += 35 + 2;

    sprintf(string, "%5.03f", _xswin->x_min_abs);

    widget_ptr = create_text_entry( gwin, 0,
                                    x, 2,
                                    Position_values_width, 
                                    Text_entry_height,
                                    TRUE, string, ON,
                                    TEXT_ENTRY_ACTIVE_COLOUR, 
                                    TEXT_ENTRY_SELECTED_COLOUR,
                                    TEXT_ENTRY_INACTIVE_COLOUR,
                                    TEXT_ENTRY_TEXT_COLOUR,
                                    TEXT_ENTRY_EDIT_COLOUR,
                                    TEXT_ENTRY_EDIT_TEXT_COLOUR,
                                    TEXT_ENTRY_CURSOR_COLOUR,
                                    (Font_types) Text_entry_font, 
                                    Text_entry_font_size,
                                    xs_min_callback, (void *) _xswin );

    add_widget_to_list( widgets, widget_ptr );

    x += Position_values_width + 2;

    widget_ptr = create_label(gwin, 0,
                              x, 2,
                              35, Text_entry_height,
                              "T(max):",
                              OFF, 
                              LABEL_ACTIVE_COLOUR,
                              LABEL_SELECTED_COLOUR,
                              LABEL_INACTIVE_COLOUR,
                              LABEL_TEXT_COLOUR,
                              (Font_types) Label_text_font,
                              Label_text_font_size );

    add_widget_to_list( widgets, widget_ptr );

    x += 35 + 2;

    sprintf(string, "%5.03f", _xswin->x_max_abs);

    widget_ptr = create_text_entry( gwin, 0,
                                    x, 2,
                                    Position_values_width, 
                                    Text_entry_height,
                                    TRUE, string, ON,
                                    TEXT_ENTRY_ACTIVE_COLOUR, 
                                    TEXT_ENTRY_SELECTED_COLOUR,
                                    TEXT_ENTRY_INACTIVE_COLOUR,
                                    TEXT_ENTRY_TEXT_COLOUR,
                                    TEXT_ENTRY_EDIT_COLOUR,
                                    TEXT_ENTRY_EDIT_TEXT_COLOUR,
                                    TEXT_ENTRY_CURSOR_COLOUR,
                                    (Font_types) Text_entry_font, 
                                    Text_entry_font_size,
                                    x_max_callback, (void *) _xswin );

    add_widget_to_list( widgets, widget_ptr );

    x += Position_values_width + 2 + 10;

    widget_ptr = create_button( gwin, 0, 
                                x, 2, 
                                30, Button_height,
                                "Save",
                                ON, TRUE, 
                                BUTTON_ACTIVE_COLOUR, 
                                BUTTON_SELECTED_COLOUR,
                                BUTTON_INACTIVE_COLOUR,
                                BUTTON_TEXT_COLOUR,
                                (Font_types) Button_text_font, 
                                Button_text_font_size,
                                xs_save_callback, (void *) _xswin ) ;
    add_widget_to_list( widgets, widget_ptr);

    x += 30 + 2;

    widget_ptr = create_text_entry( gwin, 0,
                                    x, 2,
                                    Load_filename_width, Text_entry_height,
                                    FALSE, "", ON,
                                    TEXT_ENTRY_ACTIVE_COLOUR, 
                                    TEXT_ENTRY_SELECTED_COLOUR,
                                    TEXT_ENTRY_INACTIVE_COLOUR,
                                    TEXT_ENTRY_TEXT_COLOUR,
                                    TEXT_ENTRY_EDIT_COLOUR,
                                    TEXT_ENTRY_EDIT_TEXT_COLOUR,
                                    TEXT_ENTRY_CURSOR_COLOUR,
                                    (Font_types) Text_entry_font, 
                                    Text_entry_font_size,
                                    xs_name_callback, (void *) _xswin );
    _xswin->file_widget_id = add_widget_to_list( widgets, widget_ptr);

    x += Load_filename_width + 2 + 10;

    widget_ptr = create_button( gwin, (Viewport_types) 0,
                                x, 2, 
                                Button_width, Button_height,
                                "Close", ON, TRUE,
                                BUTTON_ACTIVE_COLOUR,
                                BUTTON_SELECTED_COLOUR,
                                BUTTON_INACTIVE_COLOUR,
                                BUTTON_TEXT_COLOUR,
                                (Font_types) Button_text_font,
                                Button_text_font_size,
                                xs_close_callback, (void *) _xswin );

    add_widget_to_list( widgets, widget_ptr );
}

void xs_display(UI_struct *ui_info,
                        Viewport_types viewport_index, 
                        int force)
{
    int x_axis_pixels;
    int y_axis_pixels;
    int i, j;
    int volume_index;
    int view_index;
    Real voxelpos[MAX_DIMENSIONS];
    Real worldpos[MAX_DIMENSIONS];
    int sizes[MAX_DIMENSIONS];
    Real separations[MAX_DIMENSIONS];
    Real starts[MAX_DIMENSIONS];
    Volume volume;
    void *main_ptr;
    widget_struct *widget;
    int x, y;
    char string[128];
    object_struct *obj_ptr;
    text_struct *txt_obj_ptr;
    lines_struct *lines_obj_ptr;
    Point point;
    int ticks;
    Real x_min, x_max;
    Real y_min, y_max;
    Real vx, vy, vz;
    int x_start, x_end;
    Real x_delta;
    struct xs_window *xswin;

    if (!force && _xswin == NULL) {
        return;
    }

    if ((xswin = _xswin) != NULL) {
        viewport_index = _xswin->viewport_index;
    }

    ui_get_volume_view_index( viewport_index,
                              &volume_index, &view_index );

    if ( volume_index < 0 || !IF_volume_is_loaded( volume_index )) {

        volume_index = 0;
        view_index = 0;

        if (!IF_volume_is_loaded(volume_index)) {
            return;
        }
    }

    main_ptr = get_main_struct();
    volume = get_slice_volume(main_ptr, volume_index);
    if (volume == NULL) {
        fprintf(stderr, "volume is null?\n");
        abort();
    }

    /* No time dimension? */
    if (get_volume_n_dimensions(volume) < 4) {
        return;
    }

    IF_get_volume_voxel_position(volume_index, voxelpos);
    IF_get_volume_world_position(volume_index, worldpos);

    get_volume_sizes(volume, sizes);
    get_volume_starts(volume, starts);
    get_volume_separations(volume, separations);

    G_get_mouse_screen_position( &x, &y );

    x_axis_pixels = (sizes[3] < 640) ? 640 : sizes[3];
    y_axis_pixels = 320;

#define LMARGIN 60
#define RMARGIN 20
#define TMARGIN 30
#define BMARGIN 40

    vx = ROUND(voxelpos[0]);
    vy = ROUND(voxelpos[1]);
    vz = ROUND(voxelpos[2]);

    x_start = 0;
    x_end = sizes[3];

    if (xswin == NULL) {
        ALLOC(xswin, 1);
        _xswin = xswin;

        memset(xswin, 0, sizeof (struct xs_window));

        xswin->viewport_index = viewport_index;

        create_popup_window(&xswin->popup, "Timecourse", x, y, 
                            x_axis_pixels+(LMARGIN+RMARGIN), 
                            y_axis_pixels+(TMARGIN+BMARGIN), 
                            xs_quit_callback, (void *) xswin);

        xswin->x_min_abs = starts[3] + (x_start * separations[3]);
        xswin->x_max_abs = starts[3] + (x_end * separations[3]);

        xswin->x_min_ui = xswin->x_min_abs;
        xswin->x_max_ui = xswin->x_max_abs;

        xs_create_widgets(&xswin->popup.widgets, &xswin->popup.graphics);
    }
    else {
        for (i = 0; i < XS_MAX_OBJECTS; i++) {
            if ((obj_ptr = xswin->objects[i]) != NULL) {

                remove_object_from_viewport(&xswin->popup.graphics.graphics, 
                                            0,
                                            NORMAL_PLANES,
                                            obj_ptr);
                delete_object( obj_ptr );
            }
        }
    }

    xswin->n_objects = 0;
           
    /* Default X range is the absolute minimum and maximum */

    x_min = xswin->x_min_abs;
    x_max = xswin->x_max_abs;

    /* However, if the user has overridden these values, adopt them
     * here.
     */
    if (xswin->x_min_ui >= x_min && xswin->x_min_ui <= x_max) {
        x_min = xswin->x_min_ui;
        x_start = (x_min / separations[3]) - starts[3];
    }

    if (xswin->x_max_ui >= x_min && xswin->x_max_ui <= x_max) {
        x_max = xswin->x_max_ui;
        x_end = (x_max / separations[3]) - starts[3];
    }

    /* Calculate the Y range here.  It can be either "scaled" or 
     * "full-range".  Full-range means the scale displays the full 
     * possible range of data, scaled means it displays only the 
     * range of valid data within the given cross-section.
     */
    if (xswin->do_scale_yrange) {
        /* Normalize to actual data range at this location.
         */
        y_max = -DBL_MAX;
        y_min = DBL_MAX;

        for_less( i, 0, x_axis_pixels ) {
            Real value = get_volume_real_value(volume, vx, vy, vz, (Real) i, 0);
            if (value > y_max) {
                y_max = value;
            }
            if (value < y_min) {
                y_min = value;
            }
        }
    }
    else {
        /* Just display the full range */
        get_volume_real_range(volume, &y_min, &y_max);
    }


    /* Draw the Y axis scale 
     */
    obj_ptr = create_object( LINES );
    lines_obj_ptr = get_lines_ptr( obj_ptr );

    initialize_lines( lines_obj_ptr, BLUE );

    lines_obj_ptr->n_points = 8*3;

    lines_obj_ptr->n_items = 1;
        
    ALLOC( lines_obj_ptr->points, lines_obj_ptr->n_points );

    ALLOC( lines_obj_ptr->end_indices, lines_obj_ptr->n_items );
    ALLOC( lines_obj_ptr->indices, lines_obj_ptr->n_points );
    
    lines_obj_ptr->end_indices[0] = 8*3;

    x = LMARGIN;
    y = BMARGIN;
    sprintf(string, "%5.03f", y_min);
    display_text(xswin, 2, y-(Message_font_size/4), string);

    for ( i = 0; i < 8; i++ ) {
        int t = i*3;
        fill_Point( lines_obj_ptr->points[t+0], x, y, 0.0 );
        y += y_axis_pixels/8;
        fill_Point( lines_obj_ptr->points[t+1], x, y, 0.0 );
        fill_Point( lines_obj_ptr->points[t+2], x+10.0, y, 0.0 );
        
        lines_obj_ptr->indices[t+0] = t+0;
        lines_obj_ptr->indices[t+1] = t+1;
        lines_obj_ptr->indices[t+2] = t+2;

        sprintf(string, "%5.03f", 
                y_min + ((y_max - y_min) * (i + 1)) / 8); 
        display_text(xswin, 2, y-(Message_font_size/4), string);
    }
        

    add_object_to_viewport( &xswin->popup.graphics.graphics, 
                            0, NORMAL_PLANES, obj_ptr );

    add_object_to_xs_window(xswin, obj_ptr);

    /* Draw the X axis scale 
     */
    obj_ptr = create_object( LINES );
    lines_obj_ptr = get_lines_ptr( obj_ptr );

    initialize_lines( lines_obj_ptr, BLUE );

    ticks = x_axis_pixels / X_PIXELS_PER_TICK;
    lines_obj_ptr->n_points = ticks*3;

    lines_obj_ptr->n_items = 1;
        
    ALLOC( lines_obj_ptr->points, lines_obj_ptr->n_points );

    ALLOC( lines_obj_ptr->end_indices, lines_obj_ptr->n_items );
    ALLOC( lines_obj_ptr->indices, lines_obj_ptr->n_points );
    
    lines_obj_ptr->end_indices[0] = ticks*3;

    x = LMARGIN;
    y = BMARGIN;

    sprintf(string, "%5.03f", x_min);
    display_text(xswin,
                 x-Message_font_size*(strlen(string)/3), 
                 y-Message_font_size, string);

    for ( i = 0; i < ticks; i++ ) {
        int t = i*3;
        fill_Point( lines_obj_ptr->points[t+0], x, y, 0.0 );
        x += X_PIXELS_PER_TICK;
        fill_Point( lines_obj_ptr->points[t+1], x, y, 0.0 );
        fill_Point( lines_obj_ptr->points[t+2], x, y+10.0, 0.0 );

        lines_obj_ptr->indices[t+0] = t+0;
        lines_obj_ptr->indices[t+1] = t+1;
        lines_obj_ptr->indices[t+2] = t+2;

        sprintf(string, "%5.03f", 
                x_min + ((x_max - x_min) * (i + 1)) / ticks); 
        display_text(xswin,
                     x-Message_font_size*(strlen(string)/3), 
                     y-Message_font_size, string);
    }

    add_object_to_viewport( &xswin->popup.graphics.graphics, 
                            0, NORMAL_PLANES, obj_ptr );

    add_object_to_xs_window(xswin, obj_ptr);

    sprintf(string, "Timecourse of voxel at X=%.2f,Y=%.2f,Z=%.2f",
            worldpos[X], worldpos[Y], worldpos[Z]);

    display_text(xswin, LMARGIN+10, 
                 y_axis_pixels + BMARGIN + TMARGIN - Message_font_size,
                 string);

    /* The graph itself. */

    obj_ptr = create_object( LINES );
    lines_obj_ptr = get_lines_ptr( obj_ptr );

    x_delta = ((Real) x_axis_pixels / (x_end - x_start));

    initialize_lines_with_size( lines_obj_ptr, WHITE, x_end - x_start, 0 );

    for_less( i, x_start, x_end ) {
        Real value = get_volume_real_value(volume, vx, vy, vz, (Real) i, 0);
        if (value > y_max) {
            value = y_max;
        }
        if (value < y_min) {
            value = y_min;
        }
        value = ((value - y_min) * y_axis_pixels) / (y_max - y_min);
        value = ROUND(value);

        x = ROUND((i - x_start) * x_delta);
        fill_Point( lines_obj_ptr->points[i-x_start], (Real) x+LMARGIN+1, 
                    value+BMARGIN, 0.0 );
    }

    add_object_to_viewport( &xswin->popup.graphics.graphics, 0, NORMAL_PLANES,
                            obj_ptr );

    add_object_to_xs_window(xswin, obj_ptr);
}

#include  <def_user_interface.h>

public  void  initialize_UI_widgets( UI_struct  *ui_info )
{
    initialize_widget_list( &ui_info->widget_list );

    add_main_widgets( ui_info );
}

#include  <def_user_interface.h>
#include  <signal.h>
#include  <sys/wait.h>

private  void  done_resampling(
    int  exit_status )
{
    if( exit_status == 0 )
    {
        (void) initialize_loading_volume( get_ui_struct(),
                                          RESAMPLED_VOLUME_INDEX,
                                          get_ui_struct()->resampled_filename,
                                          TRUE );
    }
    else
    {
        print( "Resampling failed %d\n", exit_status );
    }
}

#ifdef USE_FORK
private  void  child_died_signal( int sig, int code, struct sigcontext *sc )
{
    int   statptr;
    int   exit_status;

    (void) wait( &statptr );

    if( WIFEXITED(statptr) )
    {
        exit_status = WEXITSTATUS(statptr);

        done_resampling( exit_status );
    }

    (void) signal( SIGCLD, child_died_signal );
}
#endif

public  void  start_resampling(
    UI_struct   *ui,
    char        volume_1_filename[],
    char        volume_2_filename[],
    char        resampled_filename[],
    Transform   *resampling_transform )
{
    int     exit_status;
    char    command_str[2000];
    String  output_filename;
    String  transform_filename;

    (void) tmpnam( transform_filename );

    expand_filename( resampled_filename, output_filename );

    if( RESAMPLED_VOLUME_INDEX == 1 )
        (void) strcpy( ui->original_filename_volume_2, volume_2_filename );
    else
        (void) strcpy( ui->original_filename_volume_2, volume_1_filename );

    (void) strcpy( ui->resampled_filename, output_filename );
    ui->resampling_transform = *resampling_transform;

    IF_save_transform( transform_filename );

#ifdef USE_FORK
    (void) signal( SIGCLD, child_died_signal );

    if( fork() == 0 )
    {
        (void) execlp( "register_resample",
                       volume_1_filename,
                       volume_2_filename,
                       transform_filename,
                       output_filename,
                       (char *) NULL );
    }

    set_resample_button_activity( ui, FALSE );
#else
    (void) printf( "Please wait a couple of minutes for the resampling.\n" );
    (void) sprintf( command_str, "register_resample %s %s %s %s",
                    volume_1_filename,
                    volume_2_filename,
                    transform_filename,
                    output_filename );

    exit_status = system( command_str );

    done_resampling( exit_status );
#endif
}

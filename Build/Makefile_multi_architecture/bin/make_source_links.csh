#!/bin/csh -f
#
#  Usage: make_source_links.csh  dir1/src1.c dir2/src2.c
#
#  Creates the directories implied by the argument files.
#  Creates a link in the directory from src1.c to ../src1.c, etc.
#
#  This is used by the make system to provide a subdirectory in
#  each source directory for each architecture, and to put links to the
#  .c files, so that make dependencies will work.
#

    if( "$ARCH" == "SunOS_4" ) then
        set ln = ( ln -s )
    else
        set ln = ( ln -sf )
    endif
    
    foreach src_link ( $* )

        set dir = $src_link:h
        set file = $src_link:t

#---  if the file is in the current directory, then no link required

        if( $dir == $file ) continue

#---  make the directory if needed

        if( ! -e $dir ) then
            mkdir $dir
        endif

#---  make the link in the directory to the parent directory

        $ln ../$file $src_link 
    end

#!/bin/csh -f
#
#  create_prototypes.csh  proto.h  file1.c file2.c ..
#

    if( ! -e /usr/local/bin/perl ) exit;

    set exec_dir = $1
    set proto = $2
    shift
    shift
    set c_files = ( $* )
    
    set def_line = ${proto:t}
    set def_line = ${def_line:r}

    echo "#ifndef  DEF_$def_line"                       >  $proto
    echo "#define  DEF_$def_line"                       >> $proto
    $exec_dir/extract_functions -public $c_files        >> $proto
    echo "#endif"                                       >> $proto

#!/bin/csh -f
#
#   copies the first argument to the second argument with a directory insert
#

    set src = $1
    set dest = $2

    set p = `pwd | sed -e 's/\//\\\//g'`

    sed -e s/__INSERT_DIR_HERE__/$p/ $src > $dest

    pwd

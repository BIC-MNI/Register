#! /bin/sh

cat <<EOF
Messages of the following type may be safely ignored:

    automake: configure.in: installing [...]
    warning: AC_TRY_RUN called without default to allow cross compiling

Any other diagnostics may be a sign of trouble.  
Let us know if something goes wrong.


EOF

aclocal
autoheader
automake --add-missing
autoconf


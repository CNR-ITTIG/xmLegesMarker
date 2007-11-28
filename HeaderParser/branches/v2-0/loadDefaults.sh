#!/bin/bash

## Su Windows/Cygwin: in caso di errore provare:
## dos2unix.exe loadDefault.sh 

rm -f Default.h

for x in ` ls -1 Models/ | grep -v CVS`
do
    echo "const char * ${x}_default=\"`cat Models/$x | tr '\n' '#' | sed s/#/\\\\\\\\n/g`\";" >> Default.h  
done

dos2unix Default.h



rm -f Default.h

for x in ` ls -1 Models/ | grep CVS -v`
do
    echo "const char * ${x}_default=\"`cat Models/$x | tr '\n' '#' | sed s/#/\\\\\\\\n/g`\";" >> Default.h  
done

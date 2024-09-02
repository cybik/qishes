#! /bin/sh

# Imported work from past life.

cd $1

if [ ! -f ".processed" ]; then
    for FILE in $(\ls -d *.so*); do
        patchelf --set-rpath '$ORIGIN' $FILE
        NEWNAME=$(objdump -p $FILE |grep -i soname | tr -s ' ' | cut -d ' ' -f3)
        [ ! -e ${NEWNAME} ] && mv ${FILE} ${NEWNAME}
        chmod 755 ${NEWNAME}

        shortlib=$NEWNAME
        basename=$shortlib
        while extn=$(echo $shortlib | sed -n '/\.[0-9][0-9]*$/s/.*\(\.[0-9][0-9]*\)$/\1/p')
              [ -n "$extn" ]
        do
            shortlib=$(basename $shortlib $extn)
            ln -sf $basename $shortlib
            basename=$shortlib
        done
    done
    touch .processed
fi

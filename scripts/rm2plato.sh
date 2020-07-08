#!/bin/sh
# creates links with the document name to all remarkable pdfs (for use with plato)
set -e
dest=${1:-~/documents}
folder=/home/root/.local/share/remarkable/xochitl/
#delete all links
find "$dest" -type l -exec rm {} \;
for f in $folder/*pdf
do
    filename=$(basename -- "$f") 
    meta="${filename%.*}.metadata"
    metafile="$folder/$meta"
    #get the document name from the metadata file
    name=$(grep visibleName $metafile | cut -d: -f 2 |  xargs)
    linkname="$(realpath $dest)/${name//\"}.pdf"
    echo $linkname
    ln -sf "$f" "$linkname"
done

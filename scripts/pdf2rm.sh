#!/bin/sh
# This is where ssh will try to copy the files associated with the document
set -e
#set -x
xochitldir=/home/root/.local/share/remarkable/xochitl/

# Check if we have something to do
if [ $# -lt 1 ]; then
    echo "Link PDFs"
    echo "usage: $(basename $0) path-to-pdf-file [path-to-pdf-file]..."
    exit 1
fi

# Create directory where we prepare the files as the reMarkable expects them
tmpdir=$xochitldir

# Loop over the command line arguments,
# which we expect are paths to the PDF files to be transfered

# reMarkable documents appear to be identified by universally unique IDs (UUID),
# so we generate one for the document at hand
uuid=$(</proc/sys/kernel/random/uuid)
fullname=$(realpath "$1")
pdfname=$(basename "$1")
extension="${pdfname##*.}"
documentname="${pdfname%.*}"


echo $fullname

if grep  "$documentname" ${tmpdir}*.metadata
then
    echo "Exists"
    exit 1
fi

# Link the file
ln "$fullname" ${tmpdir}/${uuid}.pdf

# Add metadata
# The lastModified item appears to contain the date in milliseconds since Epoch
cat <<EOF >>${tmpdir}/${uuid}.metadata
{   
    "deleted": false,
    "lastModified": "$(date +%s)000",
    "metadatamodified": false,
    "modified": false,
    "parent": "",
    "pinned": false,
    "synced": false,
    "type": "DocumentType",
    "version": 1,
    "visibleName": "$documentname"
}
EOF

# Add content information
cat <<EOF >${tmpdir}/${uuid}.content
{   
    "extraMetadata": {
    },
    "fileType": "pdf",
    "fontName": "",
    "lastOpenedPage": 0,
    "lineHeight": -1,
    "margins": 100,
    "pageCount": 1,
    "textScale": 1,
    "transform": {
        "m11": 1,
        "m12": 1,
        "m13": 1,
        "m21": 1,
        "m22": 1,
        "m23": 1,
        "m31": 1,
        "m32": 1,
        "m33": 1
    }
}
EOF

# Add cache directory
mkdir ${tmpdir}/${uuid}.cache

# Add highlights directory
mkdir ${tmpdir}/${uuid}.highlights

# Add thumbnails directory
mkdir ${tmpdir}/${uuid}.thumbnails

echo "Linked $fullname" 


#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Il manque le nom du fichier"
    echo "Usage : $0 <test a faire>"
    echo "1 : test trame HTTP"
    echo "2 : test trame option IP"
    echo "3 : test deux trame"
    echo "4 : test trame incomplet"
    echo "5 : test trame sans offset"
    echo "nom_ficher : pour test fichier quelconque dans le dossier test_file"
    exit 1
fi
path=test_file
val=1
if [ $1 -eq 1 ] ; then
  python3 entry.py $path\/test_trame_HTTP.txt> data.txt
elif [ $1 -eq 2 ] ; then
  python3 entry.py $path\/test_optionIP.txt> data.txt
elif [ $1 -eq 3 ] ; then
  python3 entry.py $path\/test_deux_trames.txt> data.txt
elif [ $1 -eq 4 ] ; then
  python3 entry.py $path\/test_trame_incomplet.txt> data.txt
elif [ $1 -eq 5 ] ; then
  python3 entry.py $path\/test_trame_sans_offset.txt> data.txt
else
  python3 entry.py $path\/$1> data.txt
fi
cat data.txt

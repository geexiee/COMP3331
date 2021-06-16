#!/bin/sh

# usage: ./autotest.sh YOURPORTNUMBER

# all client errors will be displayed to screen.
# check the "serverlog" file to see if server was printing correctly.
# check "servererr" file for server errors.

#################
# IMPORTANT!!!!!!
#################
# CHANGE THESE
#
CLIENTDIR="CLIENTDIR" # change to your own client dir
SERVERDIR="SERVERDIR" # change to your own server dir
#
# YOUR FOLDER STRUCTURE SHOULD LOOK LIKE THIS FIRST
# asst/
#   | test              // your test file to upload/download
#   | credentials.txt   // should be the EXACT same one as webcms
#   | CLIENTDIR         // your client program dir
#       | client.py
#       | ...
#   | SERVERDIR         // your server program dir
#       | server.py
#       | ...
#

# some variables u probs dont need to touch
PORT=$1
ADMINPASS="andy"

cat >yoda << EOF
Yoda
asdfasdf
Yoda
jedi*knight
LST
HELLO
CRT 3331
CRT 3331
CRT 9331
LST 3331
LST
MSG 3331 Networks is awesome
RDT
RDT 9331
RDT 3331
UPD 3331 test
RDT 3331
RMV 9331
XIT
EOF

cat >obiwan << EOF
Obi-wan
r2d2
CRT 9331
MSG 9331 Networks exam PWNED me
MSG 3331 Networks exam PWNED me
LST
RDT 331
RDT 3331
DWN 9331 test
DWN 3331 test
EDT 3331 1 I PWNED Networks exam
EDT 3331 2 I PWNED Networks exam
RDT 3331
RMV 3331
RMV 9331
LST
SHT
SHT monkey
EOF
echo "SHT ${ADMINPASS}" >> obiwan

cp credentials.txt $SERVERDIR
cp "test" $CLIENTDIR

# run server in background
cd $SERVERDIR
python3 server.py $PORT $ADMINPASS &
cd ..
sleep 0.5

# run client
cd $CLIENTDIR
cat ../yoda | python3 client.py 127.0.0.1 $PORT >/dev/null

# test if file uploaded correctly
if ! diff "test" ../$SERVERDIR/3331-test
then
    echo "file not uploaded correctly, content different"
    exit 1
fi

cat ../obiwan | python3 client.py localhost $PORT >/dev/null
cd ..
rm obiwan

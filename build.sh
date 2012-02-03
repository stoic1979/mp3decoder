#
# Script to build mp3 decoder application.
# It depends upon lame library.
#


#set the prefix to the prefix on your system
PREFIX=/home/navrooh/work/research/mp3decoder/lame/install/

INCLUDE="-I$PREFIX/include/lame"
LIBS="-L$PREFIX/lib -lmp3lame -DHAVE_MPGLIB"
SOURCE="app.c utils.c fft.c process_fft.c"
echo
echo "Building MP3 Decoder..."
echo "g++ -o app_lame $SOURCE $INCLUDE $LIBS"
gcc -o app_lame $SOURCE $INCLUDE $LIBS
echo
echo "Successfully Done !!!"
echo

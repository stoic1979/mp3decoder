#set the prefix to the prefix on your system
PREFIX=/home/navrooh/work/research/mp3decoder/lame/install/

INCLUDE="-I$PREFIX/include/lame"
LIBS="-L$PREFIX/lib -lmp3lame -DHAVE_MPGLIB"
SOURCE="app.c utils.c"
gcc -o app_lame $SOURCE $INCLUDE $LIBS

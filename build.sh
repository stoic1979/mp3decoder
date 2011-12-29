INCLUDE="-I/home/navrooh/work/detect-bas/lame/install/include/lame"
LIBS="-L/home/navrooh/work/detect-bas/lame/install/lib -lmp3lame -DHAVE_MPGLIB"
SOURCE="app.c"
gcc -o app_lame $SOURCE $INCLUDE $LIBS

#
# Script to build wav analyzer application.
#


SOURCE="WavAnalyzer.c fft.c"
echo
echo "Building Wav Analyzer..."
echo "g++ -o wav_analyzer $SOURCE"
gcc -o wav_analyzer $SOURCE
echo
echo "Successfully Done !!!"
echo

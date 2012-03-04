#ifndef _FFTH
#define _FFTH

#define PHI 0.618033988749895 // (-1+sqrt(5))/2
#define DeltaPhi 0.0001
#define DeltaRel 0.0001


typedef struct { int i,j,k; } TPHIRels;   // struct for PHI relations
typedef struct {int p;   double v; double ph; } Tpm; // formants pos.
// m=2^n, vector should be n+2 large!!
void fft(double *y, int m);
void fft_inv(double *y, int m);

double *ModPhaseFFT( int n, double *x, double *mod, double *phase );
double ClosestPhiRel( double *HzFrm, int n );
/*
void ProcessFFT(
        double *vfft, double *vx, int n, // i/o vfft i/o vx in Hz
        double *max, double *HzMax, // scaled values in power * Hz
        bool smooth, bool scalelog, // smooth?, log scale?
        double scale, int samples,  // scale==0 don't scale
        double *coherence,			 // calc coherence
        int *nform,                 // number of formants (must smooth!)
        double *HzFrm, double *PowFrm, // vector containing formants
        double *phFrm
        );	// scaled vfft values and samples for Hz conversion.
*/

void Formants1(double *vfft, double *HzFrm, double *PwrFrm, int n, int *nform);
void Formants2(double *vfft, double *HzFrm, double *PwrFrm, double *phFrm, int n, int *nform);
int GetPeaks(double *vfft, int nFFT, double *HzFrm, double *PwFrm, int *nform,
        double*fMax, double*fMin, double fc);

int CountRels(double *HzFrm, int nform, double Rel, double delta);    
int CountPhiRels(double *HzFrm, int nform);
int CountOctRels(double *HzFrm, int nform);
int CountFourthRels(double *HzFrm, int nform);
int CountFifthRels(double *HzFrm, int nform);

int GetPHIRels(double *HzFrm, int nform, TPHIRels *PHIRels);
int GetRels(double *HzFrm, int nform, double*rels, double from, double to, double factor);
double AbsScale1(double *vfft, int n);
double AbsScale2(double *vfft, int n, double scale);
int Freq2Index(double freq, double samples, int nFFT);
double Index2Freq(int i, double samples, int nFFT);
int FreqRange(double *v, int n, double sump, int *_fMin, int *_fMax);

double v_MinMax(double *fMin, double *fMax, double *vHz, double *v, int n);
void v_Zero(double *v, int n);
double MirrorVector(double *v, double *vo, int n);
void SortTwo(double*y, double*x, int n);
void SortTwoAsc(double*y, double*x, int n);
void Sort(double*v, int n);
double Jitter(double *vfft, int n);
double weakNote(double *HzFrm, int nf);
double excessNote(double *musGrid, int n);
int _cmp_int(const void *p1, const void *p2);
int _cmp_dbl(const void *p1, const void *p2);
int _cmp_flt(const void *p1, const void *p2);
void MinMax(double*v, int n, double*Min, double*Max);
double db1(double range, double val);
double db2(double val);
double db100(double val);
#endif

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "fft.h"


//---------------------------------------------------------------------------
// fft formant calc.
//---------------------------------------------------------------------------

// descendant order of power
static int _sort_pm(const void *p1, const void *p2) {
    if ( ((Tpm*)p1)->v > ((Tpm*)p2)->v ) return -1;
    else return +1;
}

//---------------------------------------------------------------------------
// calculates 'n' formants of a fft value
// needs a smooth fft vector.
//---------------------------------------------------------------------------
/*
void Formants2(double *vfft, double *HzFrm, double *PwrFrm, double *phFrm, int n, int *nform) {
#define forall(n) for (i=3; i<n; i++)
    Tpm *pm;
    int i;   int p, ip=0;

    if (!*nform) return;

    pm=new Tpm[*nform];

    // after smooth we've got: 0..0..local_max1..0..0..local_max2....
    forall(n-3) {
        while (vfft[i]==0 && i<n-3) i++; // skip ceros
        double max;
        for (max=-DBL_MAX, p=-1; vfft[i]!=0 && i<n-3; i++) // local max
            if (vfft[i]>max) { max=vfft[i]; p=i; }
        if (p!=-1) { // found something...?
            pm[ip].p=p; pm[ip].v=max;
            if (phFrm) pm[ip].ph=phFrm[p]; // save local max
            if (++ip >= *nform) ip--;
        }
    }
    // find 'nform' greater max (formants)
    qsort(pm, ip, sizeof(*pm), _sort_pm); // sort desc. and retrieve 3 first

    if (ip<*nform) *nform=ip; // at least 'nform' values.
    for (i=0; i<*nform; i++) {
        HzFrm[i]=pm[i].p; // pos of fft to convert to Hz.
        PwrFrm[i]=pm[i].v; // already in % max power
        if (phFrm) phFrm[i]=pm[i].ph;
    }
    delete[]pm;
}

void Formants1(double *vfft, double *HzFrm, double *PwrFrm, int n, int *nform)
{
    Formants(vfft, HzFrm, PwrFrm, NULL, n, nform);
}
*/
//---------------------------------------------------------------------------
// ProcessFFT
// input: FFT vector.
// 1.modulus, 2.max, 3. smooth, 4. log scale, 5. scale if != 0
// 6.Hz conversion
//---------------------------------------------------------------------------
/*
void ProcessFFT(
        double *vfft, double *vx, int n, // i/o vfft i/o vx in Hz (if !NULL)
        double &max, double &HzMax, // scaled values in power & Hz
        bool smooth, bool scalelog, // smooth?, log scale?
        double scale, int samples,  // scale==0 don't scale
        double &coherence,			 // calc coherence
        int &nform,                 // number of formants (must smooth!)
        double *HzFrm, double *PowFrm, // vector containing formants
        double *phFrm
        )	// scaled vfft values and samples for Hz conversion.
{
    int pm;
    double sfx=(double)samples/n/2., sfy;
    double s=0, s2=0, stddev; // for coherence

    vfft[0]=vfft[1]=vfft[2]=0; // first three values=0
    max=-DBL_MAX; pm=-1;
    // max, modulus, log, x vaues in Hz
    for (int i=3; i<n; i++) { // ignore 3 first values
        double &va=vfft[i];
        if (va<0) va=-va; // modulus fabs
        { s+=va; s2+=va*va; } // for stddev in coherence calc.
        if (scalelog) va = (va) ? log(va):0;
        if (va>max) { max=va; pm=i; } // max
        if (vx) vx[i]  *= sfx; // Hz in x if not NULL
    }

    {
        double N=n-3; // coherence calc.
        stddev=sqrt( (N*s2 - s*s)/(N*(N-1)) ); // stddev calc
        coherence = (stddev) ? 1-(s/N)/stddev:0; // coherence calc. mean/stddev.
    }

    HzMax=pm*sfx; // convert to Hz

    // scale
    if (scale && max) {
        sfy=scale/max;  max*=sfy;
        for (int i=3; i<n; i++) vfft[i]*= sfy;
    }

    // smooth fft.
    if (smooth) {
        double max10=0.05 * max;
        for (int i=3; i<n-1; i++)
            if (vfft[i] < max10) vfft[i]=0;
        for (int c=0; c<4; c++) {
            for (int i=4; i<n-1; i++) {
                if (vfft[i-1] > vfft[i] && vfft[i] < vfft[i+1])
                    vfft[i]=(vfft[i-1]+vfft[i+1])/2;
            }
        }

        if (nform) {
            // get formants
            Formants(vfft, HzFrm, PowFrm, phFrm, n, nform);
            // convert HzFrm to Hz
            for (int i=0; i<nform; i++)
                HzFrm[i] *= (double)samples/n/2.;
        }
    }
}
*/

//---------------------------------------------------------------------------
// ClosestPhiRel. find closest relation to PHI in a series of numbers.
// 0....... phi ... 1
//---------------------------------------------------------------------------
double ClosestPhiRel( double *Frm, int n ) {
    int i, j;
    double min=1, rel;
    for (i=0; i<n; i++) {
        for (j=i+1; j<n; j++) {
            if (Frm[i]>Frm[j])
                if (Frm[i]!=0) rel=fabs(Frm[j]/Frm[i] - PHI); else rel=min;
                else
                    if (Frm[j]!=0) rel=fabs(Frm[i]/Frm[j] - PHI);  else rel=min;
            if (rel < min) min=rel;
        }
    }
    // min distance to phi
    return min;
}

//------------------------------------------------------------------------------
//                          ^
// Find 'nform' freq peaks / \, returning converted (samples/nFFT) Hz
// fc = SAMPLES/nFFT/2.
//  HzFrm[0]=0
// Input comes from adding several fft sound samples
//------------------------------------------------------------------------------
int GetPeaks(double *vfft, int nFFT, double *HzFrm, double *PwFrm, int *nform, double*fMax, double*fMin, double fc) {
    int i,c, pmax, pmin;

    HzFrm[0]=PwFrm[0]=0; // initial value is cero for PHI process
    // find max value and a left cero value
    double max=-DBL_MAX; int pm=-1;
    for (i=0; i<nFFT; i++) if (vfft[i]>max) { max=vfft[i]; pm=i; }
    for (i=pm; vfft[i]!=0 && i!=0; i--); // find a left cero value
    i++; // here's the first significant value.
    *fMin=DBL_MAX; *fMax=-DBL_MAX;
    for (c=1; i<nFFT; i++) {
        double va=vfft[i];
        if (va>*fMax) {pmax=i; *fMax=va;}  if (va<*fMin) {pmin=i; *fMin=va; } // Max, Min.
        if (vfft[i-1]<vfft[i] && vfft[i]>vfft[i+1]) { // peak?
            HzFrm[c]=(double)i * fc; // save, cnv to Hz
            PwFrm[c]=vfft[i];
            if (++c>=*nform) c--;
        }
    }
    *fMax=(double)pmax*fc;
    *fMin=(double)pmin*fc;
    *nform=c;
    return c;
}

//------------------------------------------------------------------------------
// GetPHIRels. get all possible Phi rels in a Hz vector
//------------------------------------------------------------------------------
    int GetPHIRels(double *HzFrm, int nform, TPHIRels *PHIRels) {
    int i, j, k;
    int c=0; double r;
#define phiREL(x1,x2,x3,x4) ( (HzFrm[x1]-HzFrm[x2])/(((HzFrm[x3]-HzFrm[x4]))?(HzFrm[x3]-HzFrm[x4]):100) )
#define inPHI(x) ((x)>=(PHI-DeltaPhi) && (x)<=(PHI+DeltaPhi))
#define savePhi(i,j,k,r) {PHIRels[c].i=i; PHIRels[c].j=j; PHIRels[c].k=k; c++;}// i,j,k are in close to PHI relation r~=PHI.

        for (i=0; i<nform; i++)
            for (j=i+1; j<nform; j++)
                for (k=j+1; k<nform; k++) {
                    r=phiREL(j,i,k,i); if (inPHI(r)) savePhi(i,j,k,r);
                    r=phiREL(j,i,k,j); if (inPHI(r)) savePhi(i,j,k,r);
                    r=phiREL(k,i,j,i); if (inPHI(r)) savePhi(i,j,k,r);
                    r=phiREL(k,i,k,j); if (inPHI(r)) savePhi(i,j,k,r);
                    r=phiREL(k,j,j,i); if (inPHI(r)) savePhi(i,j,k,r);
                    r=phiREL(k,j,k,i); if (inPHI(r)) savePhi(i,j,k,r);
                }
        return c;
    }

    int GetRels(double *HzFrm, int nform, double*rels, double from, double to, double factor) {
    int i, j, k;
#define RelREL(x1,x2,x3,x4) ( (HzFrm[x1]-HzFrm[x2])/(((HzFrm[x3]-HzFrm[x4]))?(HzFrm[x3]-HzFrm[x4]):100) )
#define inRel(x) ((x)>=(from) && (x)<=(to))
#define saveRel(i) {rels[(int)(r*factor)]++;}

        int c=0; double r;
        for (i=0; i<nform; i++)
            for (j=i+1; j<nform; j++)
                for (k=j+1; k<nform; k++) {
                    r=RelREL(j,i,k,i); if (inRel(r)) saveRel(r);
                    r=RelREL(j,i,k,j); if (inRel(r)) saveRel(r);
                    r=RelREL(k,i,j,i); if (inRel(r)) saveRel(r);
                    r=RelREL(k,i,k,j); if (inRel(r)) saveRel(r);
                    r=RelREL(k,j,j,i); if (inRel(r)) saveRel(r);
                    r=RelREL(k,j,k,i); if (inRel(r)) saveRel(r);
                }
        return c;
    }
// is 'x' in r+-delta
/*
   static inline bool inRel(double x,double REL,double delta)
   { return (x>=(REL-delta)) && (x<=(REL+delta)); }
// calc difference relation.
static inline double calcRel(double *HzFrm, int x1,int x2,int x3,int x4)
{ return (HzFrm[x1]-HzFrm[x2])/(((HzFrm[x3]-HzFrm[x4]))?(HzFrm[x3]-HzFrm[x4]):100); }
 */
#define inReld(r,Rel,delta) (r>=(Rel-delta)) && (r<=(Rel+delta))
#define calcRel(HzFrm, x1,x2,x3,x4) (HzFrm[x1]-HzFrm[x2])/(((HzFrm[x3]-HzFrm[x4]))?(HzFrm[x3]-HzFrm[x4]):100)
//
int CountRels(double *HzFrm, int nform, double Rel, double delta) {
    int c=0; double r;
    int i, j, k;
    for (i=0; i<nform; i++)
        for (j=i+1; j<nform; j++)
            for (k=j+1; k<nform; k++) {
                r=calcRel(HzFrm,j,i,k,i); if (inReld(r,Rel,delta)) c++;
                r=calcRel(HzFrm,j,i,k,j); if (inReld(r,Rel,delta)) c++;
                r=calcRel(HzFrm,k,i,j,i); if (inReld(r,Rel,delta)) c++;
                r=calcRel(HzFrm,k,i,k,j); if (inReld(r,Rel,delta)) c++;
                r=calcRel(HzFrm,k,j,j,i); if (inReld(r,Rel,delta)) c++;
                r=calcRel(HzFrm,k,j,k,i); if (inReld(r,Rel,delta)) c++;
            }
    return c;
}
//------------------------------------------------------------------------------
// CountPhiRels. count all possible Phi rels in a Hz vector
//------------------------------------------------------------------------------
int CountPhiRels(double *HzFrm, int nform)    {	return CountRels(HzFrm, nform, PHI, DeltaRel); }
int CountOctRels(double *HzFrm, int nform)    {	return CountRels(HzFrm, nform, 0.5, DeltaRel); }
int CountFourthRels(double *HzFrm, int nform) {	return CountRels(HzFrm, nform, 0.75, DeltaRel); }
int CountFifthRels(double *HzFrm, int nform)  {	return CountRels(HzFrm, nform, 2/3., DeltaRel); }
double AbsScale1(double *vfft, int n)         {	return AbsScale2(vfft, n, 100.); } // absulute values & scale to

double AbsScale2(double *vfft, int n, double scale) {
    double max=-1;
    int i;
    for (i=0; i<n; i++) {
        if (vfft[i]<0) vfft[i] *= -1;
        if (vfft[i]>max) max=vfft[i];
    }
    if (max==0) return 1;
    for (i=0; i<n; i++) {
        vfft[i]=scale*vfft[i]/max;
    }
    return max; //??
}


//------------------------------------------------------------------------------
// Vector Min, Max, find in v and return values in vHz
//------------------------------------------------------------------------------
double v_MinMax(double *fMin, double *fMax, double *vHz, double *v, int n) {
    int i, pmax, pmin;
    for (*fMax=-DBL_MAX, *fMin=DBL_MAX, i=0; i<n; i++) {
        if (v[i] > *fMax) { *fMax=v[i]; pmax=i; }
        if (v[i] < *fMin) { *fMin=v[i]; pmin=i; }
    }
    *fMax=vHz[pmax];
    *fMin=vHz[pmin];
    return  fabs(*fMax-*fMin);
}

//------------------------------------------------------------------------------
// Zero vector
//------------------------------------------------------------------------------
void v_Zero(double *v, int n) { memset(v,0,n*sizeof(double));}
//------------------------------------------------------------------------------
// index <--> freq conversions
//------------------------------------------------------------------------------
double Index2Freq(int i, double samples, int nFFT)    { return (double)i*(samples/nFFT/2.); }
int Freq2Index(double freq, double samples, int nFFT) {	return (int)freq/(samples/nFFT/2.); }

//------------------------------------------------------------------------------
// find range of freq. with sum percentage (%) of freq.
//------------------------------------------------------------------------------
int FreqRange(double *v, int n, double sump, int *_fMin, int *_fMax) {
    int i,j;
    double sp, ps;
    // sum all freq.
    double s=0; 
    int d=0, mind=n;
    for (i=0; i<n; i++) s+=v[i];
    for ( i=0; i<n-1; i++) {
        // find sump% of s
        sp=0;
        ps=sump*s/100.;
        for (j=i; j<n-1; j++) {
            sp+=v[j];
            if (sp>=ps) {
                d=j-i;
                break;
            }
        }
        if (d<mind) {
            mind=d; *_fMin=i; *_fMax=j;
        }
    }
    return mind;
}

//---------------------------------------------------------------------------
// MirrorVector. v[i]=max-v[i]
//---------------------------------------------------------------------------
double MirrorVector(double *v, double *vo, int n) {
    int i;
    double max=-DBL_MAX;
    for (i=0; i<n; i++) if (v[i]>max) max=v[i];
    for (i=0; i<n; i++) vo[i]=max-v[i];
    return max;
}

//---------------------------------------------------------------------------
// SortTwo. based on y keeping order in x
//---------------------------------------------------------------------------
static int _cmp2dbl(const void *p1, const void *p2) { 
    typedef struct { double y,x; } T2double;
    return (int)( ((T2double*)p1)->y - ((T2double*)p2)->y);
}
static int _cmp2dblAsc(const void *p1, const void *p2) {	
    typedef struct { double y,x; } T2double;
    return (int)( ((T2double*)p2)->y - ((T2double*)p1)->y);
}
/*
void SortTwo(double*y, double*x, int n) {
    typedef struct { double y,x; } T2double;
    T2double *yx;
    yx=new T2double[n];
    for (int i=0; i<n; i++) { yx[i].y=y[i]; yx[i].x=x[i]; }
    qsort(yx, n, sizeof(T2double), _cmp2dbl);
    for (int i=0; i<n; i++) { y[i]=yx[i].y; x[i]=yx[i].x; }

    delete[]yx;
}

void SortTwoAsc(double*y, double*x, int n) {
    typedef struct { double y,x; } T2double;
    T2double *yx;
    yx=new T2double[n];
    for (int i=0; i<n; i++) { yx[i].y=y[i]; yx[i].x=x[i]; }
    qsort(yx, n, sizeof(T2double), _cmp2dblAsc);
    for (int i=0; i<n; i++) { y[i]=yx[i].y; x[i]=yx[i].x; }

    delete[]yx;
}
*/

//
// tmp code
//
int CountPhiRels_ant(double *HzFrm, int nform) {
#define savePhic(i,j,k,r) c++ // i,j,k are in close to PHI relation r~=PHI.

    int i, j, k;
    int c=0; double r;
    for (i=0; i<nform; i++)
        for (j=i+1; j<nform; j++)
            for (k=j+1; k<nform; k++) {
                r=phiREL(j,i,k,i); if (inPHI(r)) savePhic(i,j,k,r);
                r=phiREL(j,i,k,j); if (inPHI(r)) savePhic(i,j,k,r);
                r=phiREL(k,i,j,i); if (inPHI(r)) savePhic(i,j,k,r);
                r=phiREL(k,i,k,j); if (inPHI(r)) savePhic(i,j,k,r);
                r=phiREL(k,j,j,i); if (inPHI(r)) savePhic(i,j,k,r);
                r=phiREL(k,j,k,i); if (inPHI(r)) savePhic(i,j,k,r);
            }
    return c;
}

//
// calc jitter from filtered spectrum
//
double Jitter(double *vfft, int n) {
    int i;
    // find max and pos max (px)
    double mx=-999; int px=0; for (i=0; i<n; i++) if (vfft[i]>mx) {mx=vfft[i]; px=i; }
    // from 'px' find left and right margin to '0'
    int lm; for (lm=px; lm && vfft[lm]!=0;   lm--);
    int rm; for (rm=px; rm<n && vfft[rm]!=0; rm++);
    int dst=rm-lm;
    // jitter is the rel between px and dst
    if (px) mx=(double)dst/(double)px;
    else mx=0;
    return mx;
}

//
// Find min note (weak note) in HzFrm as min(frm 0..max..0)
//
double weakNote(double *musGrid, int n) { 
    int i, lm, rm;
    double mn=1e55;
    // find max and pos max (px)
    double mx=-1e55; int px=0; for (i=0; i<n; i++) if (musGrid[i]>mx) {mx=musGrid[i]; px=i; }
    // from 'px' find left and right margin to '0'
    for (lm=px; lm && musGrid[lm]!=0;   lm--); if (musGrid[lm]==0) lm++;
    for (rm=px; rm<n && musGrid[rm]!=0; rm++); if (musGrid[rm]==0) rm--;
    // min of lm..rm interval
    if (lm==rm) mn=0;
    for(i=lm; i<rm; i++) if (musGrid[i]<mn) mn=musGrid[i];
    return mn;
}

double excessNote(double *musGrid, int n) {
    int i;
    // find max and pos max (px)
    double mx=-1e55; for (i=0; i<n; i++) if (musGrid[i]>mx) {mx=musGrid[i];}
    return mx;
}

//---------------------------------------------------------------------------
// Comparision Sorting support
//---------------------------------------------------------------------------
int _cmp_int(const void *p1, const void *p2) { 	return *(int*)p1 - *(int*)p2; }

int _cmp_dbl(const void *p1, const void *p2) {
    if ( *(double*)p1 > *(double*)p2 )      return +1;
    else if ( *(double*)p1 < *(double*)p2 ) return -1;
    else 												 return  0;
}

int _cmp_flt(const void *p1, const void *p2) {
    if ( *(float*)p1 > *(float*)p2 )      return +1;
    else if ( *(float*)p1 < *(float*)p2 ) return -1;
    else 									   	  return  0;
}

void MinMax(double*v, int n, double *Min, double *Max) {
    int i;
    double mi=v[0],mx=v[0];
    printf("MinMax 111\n");
    if (n==0) { *Min=*Max=0; return; }
    printf("MinMax 222\n");
    for (i=0; i<n; i++) {
        if (v[i]>mx) mx=v[i];
        if (v[i]<mi) mi=v[i];
    }
    *Min = mi;
    *Max = mx;
}

void Sort(double*v, int n) { qsort(v, n, sizeof(double), _cmp_dbl); }

// decibel calc
double db1(double range, double val) {
    if (val==0) return -80;
    return 20. * log10(val/range);
}

// for 16bit sound
double db2(double val) {
    if (val==0) return -80;
    return 20. * log10(val/32767.);
}

double db100(double val) {
    if (val==0) return -80;
    return 20. * log10(val/100.);
}

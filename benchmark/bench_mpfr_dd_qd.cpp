#include <iostream>
#include <string>
#include <random>
#include <unistd.h>
#include <gmp.h>
#include <mpfr.h>
#include <sys/times.h>
#include <qd/qd_real.h>
#include <string.h>

// uncomment to check values
//#define PRINT_VALUES


// config
// matSize[0 .. (matSizeN-1)] and polySize[0 .. (polySizeN-1)] will be tested
#define matSizeN 1
#define polySizeN 1
int matSize[] = {100, 200, 500, 1000, 2000};
int polySize[] = {1000, 2000, 5000, 10000, 20000};

// ------------------------------------------------------------ //

using namespace std;

#define TICK_PER_SEC        (sysconf(_SC_CLK_TCK))
#define idx(x, y, n)        (x*n+y)

string mpfrString(mpfr_t a, int D) {
  char buf[1000], fmt[1000];
  string fmtS1("%."), fmtS2(to_string(D-1)), fmtS3("Re");
  strcpy(fmt, (fmtS1+fmtS2+fmtS3).c_str());
  mpfr_sprintf(buf, fmt, a);
  string str(buf);
  return str;
}

void ddMatrixTest() {
  int n = 2;      // number of doubles
  int v = 128;      // number of bits of MPFR
  int D = 32;     // number of digits to display
  int N = matSize[matSizeN-1];

  double *mat1 = new double[N*N], *mat2 = new double[N*N];
  mpfr_t *mpfrMat1 = new mpfr_t[N*N], *mpfrMat2 = new mpfr_t[N*N], *mpfrMat3 = new mpfr_t[N*N];
  dd_real *ddMat1 = new dd_real[N*N], *ddMat2 = new dd_real[N*N], *ddMat3 = new dd_real[N*N];
  tms tms0, tms1;
  long mpfrElapsed[matSizeN] = {0L}, ddElapsed[matSizeN] = {0L};
#ifdef PRINT_VALUES
  string *ddRes = new string[N*N], *mpfrRes = new string[N*N];
#endif
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> dist(-1e1, 1e1);

  // init
  dd_real ddTmp;
  mpfr_t mpfrTmp;
  mpfr_init2(mpfrTmp, v);

  // calc
  for(int i=0;i<matSizeN;i++) {
    // (m*m) * (m*m)
    int m = matSize[i];

    // generate matrix
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        mat1[idx(j,k,m)] = dist(mt);
        mat2[idx(j,k,m)] = dist(mt);
      }
    }

    // --- mpfr --- //
    // init
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        mpfr_init2(mpfrMat1[idx(j,k,m)], v);
        mpfr_set_d(mpfrMat1[idx(j,k,m)], mat1[idx(j,k,m)], MPFR_RNDN);
        mpfr_init2(mpfrMat2[idx(j,k,m)], v);
        mpfr_set_d(mpfrMat2[idx(j,k,m)], mat2[idx(j,k,m)], MPFR_RNDN);
        mpfr_init2(mpfrMat3[idx(j,k,m)], v);
        mpfr_set_d(mpfrMat3[idx(j,k,m)], 0, MPFR_RNDN);
      }
    }

    // compute
    times(&tms0);
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        for(int l=0;l<m;l++) {
          mpfr_mul(mpfrTmp, mpfrMat1[idx(j,l,m)], mpfrMat2[idx(l,k,m)], MPFR_RNDN);
          mpfr_add(mpfrMat3[idx(j,k,m)], mpfrMat3[idx(j,k,m)], mpfrTmp, MPFR_RNDN);
        }
      }
    }
    times(&tms1);
    mpfrElapsed[i] = (tms1.tms_utime - tms0.tms_utime) * 1000.0 / TICK_PER_SEC; // millisecond

    // save result
#ifdef PRINT_VALUES
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        mpfrRes[idx(j,k,m)] = mpfrString(mpfrMat3[idx(j,k,m)], D);
      }
    }
#endif

    // clean
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        mpfr_clear(mpfrMat1[idx(j,k,m)]);
        mpfr_clear(mpfrMat2[idx(j,k,m)]);
        mpfr_clear(mpfrMat3[idx(j,k,m)]);
      }
    }
    mpfr_free_cache();


    // --- dd --- //
    // init
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        ddMat1[idx(j,k,m)] = mat1[idx(j,k,m)];
        ddMat2[idx(j,k,m)] = mat2[idx(j,k,m)];
        ddMat3[idx(j,k,m)] = dd_real(0);
      }
    }

    // compute
    times(&tms0);
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        for(int l=0;l<m;l++) {
          ddTmp = ddMat1[idx(j,l,m)] * ddMat2[idx(l,k,m)];
          ddMat3[idx(j,k,m)] = ddMat3[idx(j,k,m)] + ddTmp;
        }
      }
    }
    times(&tms1);
    ddElapsed[i] = (tms1.tms_utime - tms0.tms_utime) * 1000 / TICK_PER_SEC; // millisecond

    // save result
#ifdef PRINT_VALUES
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        ddRes[idx(j,k,m)] = ddMat3[idx(j,k,m)].to_string();
      }
    }
#endif

    // --- print results --- //
#ifdef PRINT_VALUES
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        cout << "mpfr: " << mpfrRes[idx(j,k,m)] << "\n";
        cout << "dd  : " << ddRes[idx(j,k,m)] << "\n";
      }
    }
    cout << "\n\n";
#endif
  }

  // print elapsed time
  printf("Elapsed_time in ms\n");
  printf("(mat.size),mpfr,dd\n");
  for(int i=0;i<matSizeN;i++) {
    printf("%d,%ld,%ld\n", matSize[i], mpfrElapsed[i], ddElapsed[i]);
  }
}

void qdMatrixTest() {
  int n = 4;      // number of doubles
  int v = 224;      // number of bits of MPFR
  int D = 63;     // number of digits to display
  int N = matSize[matSizeN-1];

  double *mat1 = new double[N*N], *mat2 = new double[N*N];
  mpfr_t *mpfrMat1 = new mpfr_t[N*N], *mpfrMat2 = new mpfr_t[N*N], *mpfrMat3 = new mpfr_t[N*N];
  qd_real *qdMat1 = new qd_real[N*N], *qdMat2 = new qd_real[N*N], *qdMat3 = new qd_real[N*N];
  tms tms0, tms1;
  long mpfrElapsed[matSizeN] = {0L}, qdElapsed[matSizeN] = {0L};
#ifdef PRINT_VALUES
  string *qdRes = new string[N*N], *mpfrRes = new string[N*N];
#endif
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> dist(-1e1, 1e1);

  // init
  qd_real qdTmp;
  mpfr_t mpfrTmp;
  mpfr_init2(mpfrTmp, v);

  // calc
  for(int i=0;i<matSizeN;i++) {
    // (m*m) * (m*m)
    int m = matSize[i];

    // generate matrix
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        mat1[idx(j,k,m)] = dist(mt);
        mat2[idx(j,k,m)] = dist(mt);
      }
    }

    // --- mpfr --- //
    // init
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        mpfr_init2(mpfrMat1[idx(j,k,m)], v);
        mpfr_set_d(mpfrMat1[idx(j,k,m)], mat1[idx(j,k,m)], MPFR_RNDN);
        mpfr_init2(mpfrMat2[idx(j,k,m)], v);
        mpfr_set_d(mpfrMat2[idx(j,k,m)], mat2[idx(j,k,m)], MPFR_RNDN);
        mpfr_init2(mpfrMat3[idx(j,k,m)], v);
        mpfr_set_d(mpfrMat3[idx(j,k,m)], 0, MPFR_RNDN);
      }
    }

    // compute
    times(&tms0);
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        for(int l=0;l<m;l++) {
          mpfr_mul(mpfrTmp, mpfrMat1[idx(j,l,m)], mpfrMat2[idx(l,k,m)], MPFR_RNDN);
          mpfr_add(mpfrMat3[idx(j,k,m)], mpfrMat3[idx(j,k,m)], mpfrTmp, MPFR_RNDN);
        }
      }
    }
    times(&tms1);
    mpfrElapsed[i] = (tms1.tms_utime - tms0.tms_utime) * 1000.0 / TICK_PER_SEC; // millisecond

    // save result
#ifdef PRINT_VALUES
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        mpfrRes[idx(j,k,m)] = mpfrString(mpfrMat3[idx(j,k,m)], D);
      }
    }
#endif

    // clean
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        mpfr_clear(mpfrMat1[idx(j,k,m)]);
        mpfr_clear(mpfrMat2[idx(j,k,m)]);
        mpfr_clear(mpfrMat3[idx(j,k,m)]);
      }
    }
    mpfr_free_cache();


    // --- qd --- //
    // init
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        qdMat1[idx(j,k,m)] = mat1[idx(j,k,m)];
        qdMat2[idx(j,k,m)] = mat2[idx(j,k,m)];
        qdMat3[idx(j,k,m)] = qd_real(0);
      }
    }

    // compute
    times(&tms0);
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        for(int l=0;l<m;l++) {
          qdTmp = qdMat1[idx(j,l,m)] * qdMat2[idx(l,k,m)];
          qdMat3[idx(j,k,m)] = qdMat3[idx(j,k,m)] + qdTmp;
        }
      }
    }
    times(&tms1);
    qdElapsed[i] = (tms1.tms_utime - tms0.tms_utime) * 1000 / TICK_PER_SEC; // millisecond

    // save result
#ifdef PRINT_VALUES
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        qdRes[idx(j,k,m)] = qdMat3[idx(j,k,m)].to_string();
      }
    }
#endif

    // --- print results --- //
#ifdef PRINT_VALUES
    for(int j=0;j<m;j++) {
      for(int k=0;k<m;k++) {
        cout << "mpfr: " << mpfrRes[idx(j,k,m)] << "\n";
        cout << "qd  : " << qdRes[idx(j,k,m)] << "\n";
      }
    }
    cout << "\n\n";
#endif
  }

  // print elapsed time
  printf("Elapsed_time in ms\n");
  printf("(mat.size),mpfr,qd\n");
  for(int i=0;i<matSizeN;i++) {
    printf("%d,%ld,%ld\n", matSize[i], mpfrElapsed[i], qdElapsed[i]);
  }
}

void ddPolynomialTest() {
  int n = 2;      // number of doubles
  int v = 128;      // number of bits of MPFR
  int D = 32;     // number of digits to display
  int N = polySize[polySizeN-1];

  double *poly1 = new double[N+1], *poly2 = new double[N+1];
  mpfr_t *mpfrPoly1 = new mpfr_t[N+1], *mpfrPoly2 = new mpfr_t[N+1], *mpfrPoly3 = new mpfr_t[2*N+1];
  dd_real *ddPoly1 = new dd_real[N+1], *ddPoly2 = new dd_real[N+1], *ddPoly3 = new dd_real[2*N+1];
  tms tms0, tms1;
  long mpfrElapsed[polySizeN] = {0L}, ddElapsed[polySizeN] = {0L};
#ifdef PRINT_VALUES
  string *ddRes = new string[2*N+1], *mpfrRes = new string[2*N+1];
#endif
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> dist(-1e1, 1e1);

  // init
  dd_real ddTmp;
  mpfr_t mpfrTmp;
  mpfr_init2(mpfrTmp, v);

  // calc
  for(int i=0;i<polySizeN;i++) {
    // (order m) * (order m)
    int m = polySize[i];

    // generate matrix
    for(int j=0;j<=m;j++) {
      poly1[j] = dist(mt);
      poly2[j] = dist(mt);
    }

    // --- mpfr --- //
    // init
    for(int j=0;j<=m;j++) {
      mpfr_init2(mpfrPoly1[j], v);
      mpfr_set_d(mpfrPoly1[j], poly1[j], MPFR_RNDN);
      mpfr_init2(mpfrPoly2[j], v);
      mpfr_set_d(mpfrPoly2[j], poly2[j], MPFR_RNDN);
    }
    for(int j=0;j<=2*m;j++) {
      mpfr_init2(mpfrPoly3[j], v);
      mpfr_set_d(mpfrPoly3[j], 0, MPFR_RNDN);
    }

    // compute
    times(&tms0);
    for(int j=0;j<=m;j++) {
      for(int k=0;k<=m;k++) {
        mpfr_mul(mpfrTmp, mpfrPoly1[j], mpfrPoly2[k], MPFR_RNDN);
        mpfr_add(mpfrPoly3[j+k], mpfrPoly3[j+k], mpfrTmp, MPFR_RNDN);
      }
    }
    times(&tms1);
    mpfrElapsed[i] = (tms1.tms_utime - tms0.tms_utime) * 1000.0 / TICK_PER_SEC; // millisecond

    // save result
#ifdef PRINT_VALUES
    for(int j=0;j<=2*m;j++) mpfrRes[j] = mpfrString(mpfrPoly3[j], D);
#endif

    // clean
    for(int j=0;j<=m;j++) {
      mpfr_clear(mpfrPoly1[j]);
      mpfr_clear(mpfrPoly2[j]);
    }
    for(int j=0;j<=2*m;j++) mpfr_clear(mpfrPoly3[j]);
    mpfr_free_cache();


    // --- dd --- //
    // init
    for(int j=0;j<=m;j++) {
      ddPoly1[j] = poly1[j];
      ddPoly2[j] = poly2[j];
    }
    for(int j=0;j<=2*m;j++) ddPoly3[j] = dd_real(0);

    // compute
    times(&tms0);
    for(int j=0;j<=m;j++) {
      for(int k=0;k<=m;k++) {
        ddTmp = ddPoly1[j] * ddPoly2[k];
        ddPoly3[j+k] = ddPoly3[j+k] + ddTmp;
      }
    }
    times(&tms1);
    ddElapsed[i] = (tms1.tms_utime - tms0.tms_utime) * 1000 / TICK_PER_SEC; // millisecond

    // save result
#ifdef PRINT_VALUES
    for(int j=0;j<=2*m;j++) ddRes[j] = ddPoly3[j].to_string();
#endif

    // --- print results --- //
#ifdef PRINT_VALUES
    for(int j=0;j<=2*m;j++) {
      cout << "mpfr: " << mpfrRes[j] << "\n";
      cout << "dd  : " << ddRes[j] << "\n";
    }
    cout << "\n\n";
#endif
  }

  // print elapsed time
  printf("Elapsed_time in ms\n");
  printf("(poly.order),mpfr,dd\n");
  for(int i=0;i<polySizeN;i++) {
    printf("%d,%ld,%ld\n", polySize[i], mpfrElapsed[i], ddElapsed[i]);
  }
}

void qdPolynomialTest() {
  int n = 4;      // number of doubles
  int v = 224;      // number of bits of MPFR
  int D = 63;     // number of digits to display
  int N = polySize[polySizeN-1];

  double *poly1 = new double[N+1], *poly2 = new double[N+1];
  mpfr_t *mpfrPoly1 = new mpfr_t[N+1], *mpfrPoly2 = new mpfr_t[N+1], *mpfrPoly3 = new mpfr_t[2*N+1];
  qd_real *qdPoly1 = new qd_real[N+1], *qdPoly2 = new qd_real[N+1], *qdPoly3 = new qd_real[2*N+1];
  tms tms0, tms1;
  long mpfrElapsed[polySizeN] = {0L}, qdElapsed[polySizeN] = {0L};
#ifdef PRINT_VALUES
  string *qdRes = new string[2*N+1], *mpfrRes = new string[2*N+1];
#endif
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> dist(-1e1, 1e1);

  // init
  qd_real qdTmp;
  mpfr_t mpfrTmp;
  mpfr_init2(mpfrTmp, v);

  // calc
  for(int i=0;i<polySizeN;i++) {
    // (order m) * (order m)
    int m = polySize[i];

    // generate matrix
    for(int j=0;j<=m;j++) {
      poly1[j] = dist(mt);
      poly2[j] = dist(mt);
    }

    // --- mpfr --- //
    // init
    for(int j=0;j<=m;j++) {
      mpfr_init2(mpfrPoly1[j], v);
      mpfr_set_d(mpfrPoly1[j], poly1[j], MPFR_RNDN);
      mpfr_init2(mpfrPoly2[j], v);
      mpfr_set_d(mpfrPoly2[j], poly2[j], MPFR_RNDN);
    }
    for(int j=0;j<=2*m;j++) {
      mpfr_init2(mpfrPoly3[j], v);
      mpfr_set_d(mpfrPoly3[j], 0, MPFR_RNDN);
    }

    // compute
    times(&tms0);
    for(int j=0;j<=m;j++) {
      for(int k=0;k<=m;k++) {
        mpfr_mul(mpfrTmp, mpfrPoly1[j], mpfrPoly2[k], MPFR_RNDN);
        mpfr_add(mpfrPoly3[j+k], mpfrPoly3[j+k], mpfrTmp, MPFR_RNDN);
      }
    }
    times(&tms1);
    mpfrElapsed[i] = (tms1.tms_utime - tms0.tms_utime) * 1000.0 / TICK_PER_SEC; // millisecond

    // save result
#ifdef PRINT_VALUES
    for(int j=0;j<=2*m;j++) mpfrRes[j] = mpfrString(mpfrPoly3[j], D);
#endif

    // clean
    for(int j=0;j<=m;j++) {
      mpfr_clear(mpfrPoly1[j]);
      mpfr_clear(mpfrPoly2[j]);
    }
    for(int j=0;j<=2*m;j++) mpfr_clear(mpfrPoly3[j]);
    mpfr_free_cache();


    // --- qd --- //
    // init
    for(int j=0;j<=m;j++) {
      qdPoly1[j] = poly1[j];
      qdPoly2[j] = poly2[j];
    }
    for(int j=0;j<=2*m;j++) qdPoly3[j] = qd_real(0);

    // compute
    times(&tms0);
    for(int j=0;j<=m;j++) {
      for(int k=0;k<=m;k++) {
        qdTmp = qdPoly1[j] * qdPoly2[k];
        qdPoly3[j+k] = qdPoly3[j+k] + qdTmp;
      }
    }
    times(&tms1);
    qdElapsed[i] = (tms1.tms_utime - tms0.tms_utime) * 1000 / TICK_PER_SEC; // millisecond

    // save result
#ifdef PRINT_VALUES
    for(int j=0;j<=2*m;j++) qdRes[j] = qdPoly3[j].to_string();
#endif

    // --- print results --- //
#ifdef PRINT_VALUES
    for(int j=0;j<=2*m;j++) {
      cout << "mpfr: " << mpfrRes[j] << "\n";
      cout << "qd  : " << qdRes[j] << "\n";
    }
    cout << "\n\n";
#endif
  }

  // print elapsed time
  printf("Elapsed_time in ms\n");
  printf("(poly.order),mpfr,qd\n");
  for(int i=0;i<polySizeN;i++) {
    printf("%d,%ld,%ld\n", polySize[i], mpfrElapsed[i], qdElapsed[i]);
  }
}

int main ()
{
  ddMatrixTest(); cout << "\n";
  qdMatrixTest(); cout << "\n";
  ddPolynomialTest(); cout << "\n";
  qdPolynomialTest(); cout << "\n";

  return 0;
}

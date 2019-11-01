#include "iRRAM.h"
#include <vector>
#include <string>
#include <qd/qd_real.h>
#include <chrono>


using namespace iRRAM;

using std::string;
using std::vector;
using std::to_string;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;


void compute(){
  fpu_fix_start(NULL);

  // # of iterations
  // double, DD, QD: correct by ? (depends on init value)
  long nPlus = 1000;

  // repetitions of performance tests
  long m = 1<<10;

  // init values
  REAL rInit = 314159;
  double dInit = 314159;
  dd_real ddInit = 314159;
  qd_real qdInit = 314159;

  // precision controller(epsilon)
  int eps = -20;    // 2^(?)

  // initial values
  REAL r;  double d;  dd_real dd;  qd_real qd;

  // correctness check
  r = rInit; d = dInit; dd = ddInit; qd = qdInit;
  REAL errD, errDD, errQD;
  bool isDbroken=false, isDDbroken=false, isQDbroken=false;
  for(long i=0;i<nPlus;i++) {
    // check error
    if(!isDbroken) { if(!bound(r - REAL(d), eps)) {
      cout << "correctness failure: double at " + to_string(i) + "\n";
      isDbroken = true;
    }}
    if(!isDDbroken) { if(!bound(r - REAL(dd.to_string()), eps)) {
      cout << "correctness failure: DD at " + to_string(i) + "\n";
      isDDbroken = true;
    }}
    if(!isQDbroken) { if(!bound(r - REAL(qd.to_string()), eps)) {
      cout << "correctness failure: QD at " + to_string(i) + "\n";
      isQDbroken = true;
    }}

    // do work!
    r = sin(r);
    d = sin(d);
    dd = sin(dd);
    qd = sin(qd);
  }
  cout << "correctness test done.\n";

  //- performance test -//
  auto beginTime = high_resolution_clock::now();
  auto endTime = high_resolution_clock::now();
  long elapsed;

  // iRRAM
  elapsed = 0;
  for(int i=0;i<m;i++) {
    beginTime = high_resolution_clock::now();
    r = rInit;
    for(long i=0;i<nPlus;i++) r = sin(r);
    endTime = high_resolution_clock::now();
    elapsed += duration_cast<nanoseconds>(endTime-beginTime).count();
  }
  cout << "iRRAM: " << elapsed/m << " ns \n";

  // double
  if(!isDbroken) {
    elapsed = 0;
    for(int i=0;i<m;i++) {
      beginTime = high_resolution_clock::now();
      d = dInit;
      for(long i=0;i<nPlus;i++) d = sin(d);
      endTime = high_resolution_clock::now();
      elapsed += duration_cast<nanoseconds>(endTime-beginTime).count();
    }
    cout << "double: " << elapsed/m << " ns \n";
  }

  // double double
  if(!isDDbroken) {
    elapsed = 0;
    for(int i=0;i<m;i++) {
      beginTime = high_resolution_clock::now();
      dd = ddInit;
      for(long i=0;i<nPlus;i++) dd = sin(dd);
      endTime = high_resolution_clock::now();
      elapsed += duration_cast<nanoseconds>(endTime-beginTime).count();
    }
    cout << "DD: " << elapsed/m << " ns \n";
  }

  // quad double
  if(!isQDbroken) {
    elapsed = 0;
    for(int i=0;i<m;i++) {
      beginTime = high_resolution_clock::now();
      qd = qdInit;
      for(long i=0;i<nPlus;i++) qd = sin(qd);
      endTime = high_resolution_clock::now();
      elapsed += duration_cast<nanoseconds>(endTime-beginTime).count();
    }
    cout << "QD: " << elapsed/m << " ns \n";
  }
}

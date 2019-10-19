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
  long nPlus = 40;

  // init values
  REAL rInit = 314159;
  double dInit = 314159;
  dd_real ddInit = 314159;
  qd_real qdInit = 314159;

  // precision controller(epsilon)
  REAL eps = 0.000001;

  // initial values
  REAL r;  double d;  dd_real dd;  qd_real qd;

  // correctness check
  r = rInit; d = dInit; dd = ddInit; qd = qdInit;
  REAL errD, errDD, errQD;
  bool isDbroken=false, isDDbroken=false, isQDbroken=false;
  for(long i=0;i<nPlus;i++) {
    // check error
    if(!isDbroken) { if(abs(r - REAL(d)) > eps) {
      cout << "correctness failure: double at " + to_string(i) + "\n";
      isDbroken = true;
    }}
    if(!isDDbroken) { if(abs(r - REAL(dd.to_string())) > eps) {
      cout << "correctness failure: DD at " + to_string(i) + "\n";
      isDDbroken = true;
    }}
    if(!isQDbroken) { if(abs(r - REAL(qd.to_string())) > eps) {
      cout << "correctness failure: QD at " + to_string(i) + "\n";
      isQDbroken = true;
    }}

    // do work!
    r = sqrt(r);
    d = sqrt(d);
    dd = sqrt(dd);
    qd = sqrt(qd);
  }
  cout << "correctness test done.\n";

  //- performance test -//
  auto beginTime = high_resolution_clock::now();
  auto endTime = high_resolution_clock::now();
  long elapsed;

  // iRRAM
  r = rInit;
  beginTime = high_resolution_clock::now();
  for(long i=0;i<nPlus;i++) r = sqrt(r);
  endTime = high_resolution_clock::now();
  elapsed = duration_cast<nanoseconds>(endTime-beginTime).count();
  cout << "iRRAM: " << elapsed << " ns \n";

  // double
  if(!isDbroken) {
    d = dInit;
    beginTime = high_resolution_clock::now();
    for(long i=0;i<nPlus;i++) d = sqrt(d);
    endTime = high_resolution_clock::now();
    elapsed = duration_cast<nanoseconds>(endTime-beginTime).count();
    cout << "double: " << elapsed << " ns \n";
  }

  // double double
  if(!isDDbroken) {
    dd = ddInit;
    beginTime = high_resolution_clock::now();
    for(long i=0;i<nPlus;i++) dd = sqrt(dd);
    endTime = high_resolution_clock::now();
    elapsed = duration_cast<nanoseconds>(endTime-beginTime).count();
    cout << "DD: " << elapsed << " ns \n";
  }

  // quad double
  if(!isQDbroken) {
    qd = qdInit;
    beginTime = high_resolution_clock::now();
    for(long i=0;i<nPlus;i++) qd = sqrt(qd);
    endTime = high_resolution_clock::now();
    elapsed = duration_cast<nanoseconds>(endTime-beginTime).count();
    cout << "QD: " << elapsed << " ns \n";
  }
}

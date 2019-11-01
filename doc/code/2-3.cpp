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
  long nPlus = 90;

  // repetitions of performance tests
  long m = 1<<10;

//  stiff code(20);
  show_statistics();

  // init values
  REAL rInit = REAL("314159");
  double dInit = 314159;
  dd_real ddInit = dd_real("314159");
  qd_real qdInit = qd_real("314159");

  // precision controller(epsilon)
  int eps = -70;    // 2^(?)

  // initial values
  REAL r;  double d;  dd_real dd;  qd_real qd;

  // correctness check
  r = rInit; d = dInit; dd = ddInit; qd = qdInit;
  REAL errD, errDD, errQD;
  bool isDbroken=false, isDDbroken=false, isQDbroken=false;
  for(long i=0;i<nPlus;i++) {
    // do work!
    r = sqrt(r);
    d = sqrt(d);
    dd = sqrt(dd);
    qd = sqrt(qd);

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
  }
  cout << "correctness test done.\n";

  //- performance test -//
  auto beginTime = high_resolution_clock::now();
  auto endTime = high_resolution_clock::now();
  int rElapsed=0,dElapsed=0,ddElapsed=0,qdElapsed=0;

  // iRRAM
  for(int i=0;i<m;i++) {
    beginTime = high_resolution_clock::now();
    r = rInit;
    for(long i=0;i<nPlus;i++) r = sqrt(r);
    endTime = high_resolution_clock::now();
    rElapsed += duration_cast<nanoseconds>(endTime-beginTime).count();
  }
  rElapsed /= m;

  // double
  if(!isDbroken) {
    for(int i=0;i<m;i++) {
      beginTime = high_resolution_clock::now();
      d = dInit;
      for(long i=0;i<nPlus;i++) d = sqrt(d);
      endTime = high_resolution_clock::now();
      dElapsed += duration_cast<nanoseconds>(endTime-beginTime).count();
    }
    dElapsed /= m;
  }

  // double double
  if(!isDDbroken) {
    for(int i=0;i<m;i++) {
      beginTime = high_resolution_clock::now();
      dd = ddInit;
      for(long i=0;i<nPlus;i++) dd = sqrt(dd);
      endTime = high_resolution_clock::now();
      ddElapsed += duration_cast<nanoseconds>(endTime-beginTime).count();
    }
    ddElapsed /= m;
  }

  // quad double
  if(!isQDbroken) {
    for(int i=0;i<m;i++) {
      beginTime = high_resolution_clock::now();
      qd = qdInit;
      for(long i=0;i<nPlus;i++) qd = sqrt(qd);
      endTime = high_resolution_clock::now();
      qdElapsed += duration_cast<nanoseconds>(endTime-beginTime).count();
    }
    qdElapsed /= m;
  }
  cout << "iRRAM: " << rElapsed << " ns \n";
  if(!isDbroken)  cout << "double: " << dElapsed << " ns \n";
  if(!isDDbroken) cout << "DD: " << ddElapsed << " ns \n";
  if(!isQDbroken) cout << "QD: " << qdElapsed << " ns \n";
}

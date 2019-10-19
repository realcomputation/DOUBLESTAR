#include "iRRAM.h"
#include <chrono>
#include <vector>
#include <qd/qd_real.h>

using namespace iRRAM;

using std::string;
using std::vector;
using std::to_string;
using std::chrono::microseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

int getReqIter(int p) {
  REAL y=1;
  int i=2;
  while ( !bound(y,p-1) ) {
    y=y/i;
    i+=1;
  }
  return i;
}
void compute() {
  int M = 15;               // number of steps
  int N = 1<<14;                // number of repetition
  int mode = 1;             // 1: double, 2: DD, 3: QD, 4: QD(over precision limit)


  vector<int> rAvg(M), targetAvg(M);

  // set the finest precision and number of digits
  int P, nDigits;
  switch(mode) {
  case 1: P = -15;  nDigits = 14; break;
  case 2: P = -97;  nDigits = 40; break;
  case 3: P = -196; nDigits = 71; break;
  case 4: P = -500; nDigits = 71; break;
  }

  auto beginTime = high_resolution_clock::now();
  auto endTime = high_resolution_clock::now();

  for(int m=1;m<=M;m++) {
    int p = P*m/M;      // precision

    // find out required # of iterations
    int reqIter = getReqIter(p);
    if(reqIter <= 0) {
      cout << "reqIter overflowed: " << p << "\n";
      return;
    }

    long rElapsed = 0, targetElapsed = 0;

    for(int n=0;n<N;n++) {
      REAL r=2,ry=1;
      double d=2,dy=1;
      dd_real dd=2,ddy=1;
      qd_real qd=2,qdy=1;

      // iRRAM
      beginTime = high_resolution_clock::now();
      for(int i=2;i<reqIter;i++) {      ry = ry / i;  r = r + ry;    }
      cout << setRwidth(nDigits) << r << "\n";
      endTime = high_resolution_clock::now();
      rElapsed += duration_cast<microseconds>(endTime-beginTime).count();

      switch(mode) {
      case 1:
        // double
        beginTime = high_resolution_clock::now();
        for(int i=2;i<reqIter;i++) {      dy = dy / i;  d = d + dy;    }
        cout << d << "\n";
        endTime = high_resolution_clock::now();
        targetElapsed += duration_cast<microseconds>(endTime-beginTime).count();
        break;
      case 2:
        // DD
        beginTime = high_resolution_clock::now();
        for(int i=2;i<reqIter;i++) {      ddy = ddy / i;  dd = dd + ddy;    }
        cout << dd.to_string() << "\n";
        endTime = high_resolution_clock::now();
        targetElapsed += duration_cast<microseconds>(endTime-beginTime).count();
        break;
      case 3:
      case 4:
        // QD
        beginTime = high_resolution_clock::now();
        for(int i=2;i<reqIter;i++) {      qdy = qdy / i;  qd = qd + qdy;    }
        cout << qd.to_string() << "\n";
        endTime = high_resolution_clock::now();
        targetElapsed += duration_cast<microseconds>(endTime-beginTime).count();
        break;
      }
    }
    rAvg[m-1] = rElapsed / N;
    targetAvg[m-1] = targetElapsed / N;
  }
  cout << "<averages> in us\n";
  for(int m=1;m<=M;m++) {
    int p = P*m/M;      // precision
    cout << -p << " " << rAvg[m] << " " << targetAvg[m] << "\n";
  }
}

#include "iRRAM.h"
#include <vector>
#include <string>
#include <qd/qd_real.h>

using namespace iRRAM;

using std::string;
using std::vector;
using std::to_string;

/* Compute iterated system x=3.75*x*(1-x) (Kulisch) */

void compute(){
  fpu_fix_start(NULL);

  // input
  int count;
  cout << "\nIterated functions system: x = 3.75*x*(1-x)\n";
  cout << "How many values: ";
  cin  >> count;
  cout << "\n";

  // initial values
  REAL   xr= 0.5, cr=3.75;
  double xd= 0.5, cd=3.75;
  dd_real xdd= 0.5, cdd=3.75;
  qd_real xqd= 0.5, cqd=3.75;

  // epsilon setup
  REAL eps=0.000001;
  bool isDbroken=false, isDDbroken=false, isQDbroken=false;
  REAL breakD, breakDD, breakQD;    // the value that produces more error than eps
  int breakDidx, breakDDidx, breakQDidx;    // the index at which more error occurs than eps

  // calc
  REAL errD, errDD, errQD;
  for(long i=1;i<=count;i++ ) {
    // calc errors
    errD = xr - REAL(xd);
    errDD = xr - REAL(xdd.to_string());
    errQD = xr - REAL(xqd.to_string());

    // check if double, DD, and QD has showed error more than epsilon, respectively
    if(!isDbroken) {  if(abs(errD) > eps) { isDbroken = true; breakD = REAL(xd); breakDidx = i; }}
    if(!isDDbroken) {  if(abs(errDD) > eps) { isDDbroken = true; breakDD = REAL(xdd.to_string()); breakDDidx = i; }}
    if(!isQDbroken) {  if(abs(errQD) > eps) { isQDbroken = true; breakQD = REAL(xqd.to_string()); breakQDidx = i; }}

    // print
    cout << i << " ";
    cout << setRwidth(40) << setRflags(iRRAM_float_absolute) << xr << " ";
    cout << xd << " " << xdd.to_string() << " " << xqd.to_string() << " ";
    cout << setRflags(iRRAM_float_absolute) << errD << " ";
    cout << setRflags(iRRAM_float_absolute) << errDD << " ";
    cout << setRflags(iRRAM_float_absolute) << errQD << "; \n";
    xr=cr*xr*(1-xr);
    xd=cd*xd*(1-xd);
    xdd=cdd*xdd*(1-xdd);
    xqd=cqd*xqd*(1-xqd);
  }

  if(isDbroken) { cout << "double is broken at: " << std::to_string(breakDidx) << " with " << breakD << "\n"; }
  if(isDDbroken) { cout << "DD is broken at: " << std::to_string(breakDDidx) << " with " << breakDD << "\n"; }
  if(isQDbroken) { cout << "QD is broken at: " << std::to_string(breakQDidx) << " with " << breakQD << "\n"; }
}

iRRAM = [0 1982 3082 4311 5056 6077 7333 8007 9137];
double = [0 61 63 91 115 139 184 179 254];
DD = [0 186 272 390 512 643 781 887 1048];
QD = [0 4333 8447 12624 16489 20411 24697 28649 31991];
idx = 0:5:40;

% values
plot(idx, iRRAM)
hold on
plot(idx(1:size(double,2)), double)
plot(idx(1:size(DD,2)), DD)
plot(idx(1:size(QD,2)), QD)
hold off
title('Performance test')
xlabel('Iteration')
ylabel('Elapsed time(ns)')
legend('iRRAM','double','dd','qd')

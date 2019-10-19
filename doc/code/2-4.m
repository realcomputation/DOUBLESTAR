iRRAM = [0 1818310 3871717 5459542 6750785 8635247 9947132 11758728 13812230 18689463 17253055];
double = [0 38 51 39 42 40 38 38 47 48 50];
DD = [0 30133 61892 130214 115878 141628 166278 219942 303960 290232 285337];
QD = [0 276612 594097 884736 1129187 1400049 1719012 2043182 2423032 2677895 2798857];
idx = 0:100:1000;

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

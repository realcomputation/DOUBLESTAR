iRRAM = [0 1529 2936 3744 4231 5273 6218 7108 7812 9127 10022 11084 11802];
double = [0 39 41 39];
DD = [0 100 147 201 327 326 375 445 545 543];
QD = [0 539 777 986 1277 1545 1858 2104 2308 2664 2935 3231];
idx = 0:5:60;

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

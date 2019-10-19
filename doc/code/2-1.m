iRRAM = [0 2218024 4602518 6904207 8786108 11305168 13481659 15487987 17854700 19961321 22194439];
double = [0 37 43 37 37 41 38 37 36 38];
DD = [0 89737 165619 270899 323277 445272 484875 630103 716757 750007 968803];
QD = [0 329317 801740 1061150 1330845 1754624 2088568 2358365 2752079 3075830 3428676];
idx = 0:10000:100000;

% values
plot(idx(1:size(iRRAM,2)), iRRAM)
hold on
plot(idx(1:size(double,2)), double)
plot(idx(1:size(DD,2)), DD)
plot(idx(1:size(QD,2)), QD)
hold off
title('Performance test')
xlabel('Iteration')
ylabel('Elapsed time(ns)')
legend('iRRAM','double','dd','qd')

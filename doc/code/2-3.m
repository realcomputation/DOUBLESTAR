% values
idx = 10:10:100;
iRRAM=[ 6576 7204 10624 14309 18002 21792 25075 28702 32293 35448   ];
% double=[ 0             ];
DD=[ 280 518 752 986 1245 1447 1635 1912 2125 2300   ];
QD=[ 8824 16734 25149 33089 41635 50126 56960 67796 73445 83925   ];



% linear regression
p_iRRAM = polyfit(idx,iRRAM,1)
% p_double = polyfit(idx(1:size(double,2)),double,1)
p_DD = polyfit(idx(1:size(DD,2)),DD,1)
p_QD = polyfit(idx(1:size(QD,2)),QD,1)


% graph
fplot(poly2sym(p_iRRAM))
hold on
% fplot(poly2sym(p_double))
fplot(poly2sym(p_DD))
fplot(poly2sym(p_QD))
scatter(idx,iRRAM,'*')
% scatter(idx(1:size(double,2)), double, '+')
scatter(idx(1:size(DD,2)), DD, 'o')
scatter(idx(1:size(QD,2)), QD, 'x')
hold off
title('Performance test')
xlabel('Iteration')
ylabel('Elapsed time(ns)')
xlim([0, inf])
ylim([0, inf])
% lgd = legend('iRRAM','double','dd','qd');
lgd = legend('iRRAM','dd','qd');
lgd.Location = 'northwest';

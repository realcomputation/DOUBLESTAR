% values
idx = 10000:10000:100000;
iRRAM=[ 2277466 4464846 6706339 8975040 11187951 13375699 15690342 17920073 20019130 22350155 ];
double=[ 40 61 59 40 51 47 39 39   ];
DD=[ 85559 171552 255780 341247 426594 513372 595202 682645 758295 849898 ];
QD=[ 352622 706488 1070855 1426889 1774732 2134957 2500221 2807334 3203934 3541002 ];


% linear regression
p_iRRAM = polyfit(idx,iRRAM,1)
p_double = polyfit(idx(1:size(double,2)),double,1)
p_DD = polyfit(idx(1:size(DD,2)),DD,1)
p_QD = polyfit(idx(1:size(QD,2)),QD,1)


% graph
fplot(poly2sym(p_iRRAM))
hold on
fplot(poly2sym(p_double))
fplot(poly2sym(p_DD))
fplot(poly2sym(p_QD))
scatter(idx,iRRAM,'*')
scatter(idx(1:size(double,2)), double, '+')
scatter(idx(1:size(DD,2)), DD, 'o')
scatter(idx(1:size(QD,2)), QD, 'x')
hold off
title('Performance test')
xlabel('Iteration')
ylabel('Elapsed time(ns)')
xlim([0, inf])
ylim([0, inf])
lgd = legend('iRRAM','double','dd','qd');
lgd.Location = 'northwest';

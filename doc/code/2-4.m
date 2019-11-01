% values
idx = 0:100:1000;
iRRAM=[ 0 1681688 3346861 5080634 6676655 8311261 10034068 11581949 13409914 15135516 17218219 ];
double=[ 0 53 53 56 40 40 40 54 53 39 40 ];
DD=[ 0 31403 63817 92358 118339 144700 171408 194743 222295 247799 282751 ];
QD=[ 0 291835 584602 867127 1138493 1411165 1748068 1984476 2316362 2602703 3000588 ];



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
% xlim([idx(1), inf])
% ylim([0, inf])
lgd = legend('iRRAM','double','dd','qd');
lgd.Location = 'northwest';

% values
idx = 5:5:60;
iRRAM=[ 1207 2164 3314 4575 4954 6190 7033 7952 8960 10191 10952 11834 ];
double=[ 43 40 40          ];
DD=[ 90 162 201 248 316 441 467 500 571    ];
QD=[ 253 466 728 898 1058 1388 1411 1606 1803 2016 2296  ];



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

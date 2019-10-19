data = [
4 23 8
9 26 11
14 29 9
19 34 12
24 34 10
29 34 6
33 29 12
38 32 10
43 31 8
48 32 11
53 34 8
58 36 7
63 50 4
67 43 4
72 38 8
77 32 8
82 36 12
87 37 9
92 38 9
97 31 15
];

% values
plot(data(:,1), data(:,2))
hold on
plot(data(:,1), data(:,3))
hold off
title('Performance test')
xlabel('-p')
ylabel('Elapsed time(\mus)')
legend('iRRAM','DD')

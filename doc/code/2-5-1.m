data = [
1 11 5
2 12 5
3 12 5
4 9 4
5 9 4
6 10 3
7 9 4
8 10 5
9 12 4
10 10 6
11 12 6
12 11 6
13 11 7
14 11 6
15 11 6
];

% values
plot(data(:,1), data(:,2))
hold on
plot(data(:,1), data(:,3))
hold off
title('Performance test')
xlabel('-p')
ylabel('Elapsed time(\mus)')
legend('iRRAM','double')

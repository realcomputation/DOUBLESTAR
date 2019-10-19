data = [
9 31 36
19 42 34
29 40 32
39 35 38
49 36 35
58 39 35
68 40 39
78 36 34
88 41 34
98 36 34
107 36 32
117 47 43
127 51 44
137 50 47
147 53 44
156 39 35
166 41 34
176 48 38
186 46 37
196 53 46
];

% values
plot(data(:,1), data(:,2))
hold on
plot(data(:,1), data(:,3))
hold off
title('Performance test')
xlabel('-p')
ylabel('Elapsed time(\mus)')
legend('iRRAM','QD')

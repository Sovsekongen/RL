clc
clear all
close



figure
hold on
bx1 = subplot(1, 4, 1);
boxplot(-8:15);
title('Bandit 1');

bx2 = subplot(1, 4, 2);
boxplot(-10:20);
title('Bandit 2');

bx3 = subplot(1, 4, 3);
boxplot(-5:21);
title('Bandit 3');

bx4 = subplot(1, 4, 4);
boxplot(-1:15);
title('Bandit 4');

linkaxes([bx1 bx2 bx3 bx4], 'y')



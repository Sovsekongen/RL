clear
close all
clc

fileUcb = fopen('C:/Users/vikpo/desktop/ucb.txt', 'r');
fileEp = fopen('C:/Users/vikpo/desktop/epsilon.txt', 'r');
fileGrad = fopen('C:/Users/vikpo/desktop/gradient.txt', 'r');
% fileEpisl
formatSpec = ('%f %f');

sizeUcb = [2 inf];

ucb = fscanf(fileUcb, formatSpec, sizeUcb);
epsilon = fscanf(fileEp, formatSpec, sizeUcb);
gradient = fscanf(fileGrad, formatSpec, sizeUcb);
fclose(fileUcb);
fclose(fileEp);
fclose(fileGrad);

ucb = ucb';
epsilon = epsilon';
gradient = gradient';

runAvgUcb = ucb(:,1);
perUcb = ucb(:,2);

runAvgEp = epsilon(:,1);
perEp = epsilon(:,2);

avgGradient = gradient(:,1);
perGradient = gradient(:,2);

[n, p] = size(perUcb);
t = 1:n;

figure
title('UCB');
subplot(2, 1, 1);
plot(t, perUcb);
title('Percentage');
xlabel('Iterations');
ylabel('Percentage (%)');

subplot(2, 1, 2);
plot(t, runAvgUcb);
title('Means');
xlabel('Iterations');
ylabel('Avg');

figure
title('Epsilon');
subplot(2, 1, 1);
plot(t, perEp);
title('Percentage');
xlabel('Iterations');
ylabel('Percentage (%)');

subplot(2, 1, 2);
plot(t, runAvgEp);
title('Means');
xlabel('Iterations');
ylabel('Avg');

figure
title('Gradient');
subplot(2, 1, 1);
plot(t, perGradient);
title('Percentage');
xlabel('Iterations');
ylabel('Percentage (%)');

subplot(2, 1, 2);
plot(t, avgGradient);
title('Means');
xlabel('Iterations');
ylabel('Avg');


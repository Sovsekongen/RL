clear
close all
clc

formatSpec = ('%f %f');
sizeUcb = [2 inf];

filePath  = 'C:/Users/vikpo/desktop/';

fileUCB8 = fopen('C:/Users/vikpo/desktop/gradient1.txt', 'r');
fileUCB4 = fopen('C:/Users/vikpo/desktop/gradient0.4.txt', 'r');
fileUCB2 = fopen('C:/Users/vikpo/desktop/gradienta0.1.txt', 'r');
fileUCB1 = fopen('C:/Users/vikpo/desktop/gradient0.01.txt', 'r');

ucb8 = fscanf(fileUCB8, formatSpec, sizeUcb);
ucb4 = fscanf(fileUCB4, formatSpec, sizeUcb);
ucb2 = fscanf(fileUCB2, formatSpec, sizeUcb);
ucb1 = fscanf(fileUCB1, formatSpec, sizeUcb);

fclose(fileUCB8);
fclose(fileUCB4);
fclose(fileUCB2);
fclose(fileUCB1);

ucb8 = ucb8';
ucb4 = ucb4';
ucb2 = ucb2';
ucb1 = ucb1';

perUCB8 = ucb8(:,2);
perUCB4 = ucb4(:,2);
perUCB2 = ucb2(:,2);
perUCB1 = ucb1(:,2);

avgUCB8 = ucb8(:,1);
avgUCB4 = ucb4(:,1);
avgUCB2 = ucb2(:,1);
avgUCB1 = ucb1(:,1);

avgArr = [avgUCB8 avgUCB4 avgUCB2 avgUCB1];
perArr = [perUCB8 perUCB4 perUCB2 perUCB1];

[n, p] = size(perUCB8);
t = 1:n;

figure
title('UCB');
subplot(2, 1, 1);
plot(t, perArr);
title('Percentage');
xlabel('Iterations');
ylabel('Percentage (%)');

subplot(2, 1, 2);
plot(t, avgArr);
title('Means');
xlabel('Iterations');
ylabel('Avg');
legend({'alpha = 1','alpha = 0.4', 'alpha = 0.1', 'alpha = 0.01'}, 'location', 'southeast');


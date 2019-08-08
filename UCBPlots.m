clear
close all
clc

c2File = fopen('C:/Users/vikpo/desktop/c2.txt', 'r');
c4File = fopen('C:/Users/vikpo/desktop/c4.txt', 'r');
c8File = fopen('C:/Users/vikpo/desktop/c8.txt', 'r');
c16File = fopen('C:/Users/vikpo/desktop/c16.txt', 'r');

sizeUcb = [6 inf];
formatSpec = ('%f %f %f %f %f %f');

c2 = fscanf(c2File, formatSpec, sizeUcb);
c4 = fscanf(c4File, formatSpec, sizeUcb);
c8 = fscanf(c8File, formatSpec, sizeUcb);
c16 = fscanf(c16File, formatSpec, sizeUcb);
fclose(c2File);
fclose(c4File);
fclose(c8File);
fclose(c16File);

c2 = c2';
c4 = c4';
c8 = c8';
c16 = c16';

c2per = c2(:,5);
c4per = c4(:,5);
c8per = c8(:,5);
c16per = c16(:,5);
c2runAvg = c2(:,6);
c4runAvg = c4(:,6);
c8runAvg = c8(:,6);
c16runAvg = c16(:,6);

per = [c2per c4per c8per c16per];
avg = [c2runAvg c4runAvg c8runAvg c16runAvg];

[n, p] = size(per);
t = 1:n;

figure
plot(t, avg)
title('UCB Means with different c');
xlabel('Iterations');
ylabel('Avg');
legend({'c = 2', 'c = 4', 'c = 8', 'c = 16'});

figure
plot(t, per)
title('UCB Percentage with different c');
xlabel('Iterations');
ylabel('Optimal Action (%)');
legend({'c = 2', 'c = 4', 'c = 8', 'c = 16'});

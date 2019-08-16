function mcplot(h, x)

global mc
pts = 100;
xmax = 1.05 * mc. L;
ymax = 1.1 * mc.h;

l = linspace(0, xmax, pts);
y = mc.g / 2 * (1 + cos(2 * pi * l / mc.L));
set(0, 'CurrentFigure', h); clf;

fill([0 l l(end)], [0 y 0], 'g'); hold on;
goal = [mc.L, mc.h];

plot(mc.L, mc.h + 0.0, 'rd', 'markerfacecolor', 'r', 'markersize', 10);

axis([0 xmax 0 ymax]);

carP = [x(1); mc.h / 2 * (1 + cos(2 * pi * x(1) / mc.L))];

plot(carP(1), carP(2), 'ks', 'linewidth', 5, 'markersize', 10);

end
clc
clear
close all
rng(2)

%%Variables
global cart;
R = 0;
a = 0;
T = 10;
t = 0.01;
w = rand(36,1);
cart.h = figure(1);


alpha = 0.8;
variance = 0.02;
cart.lambda = 0;
nH = 36;
 
%% Define radial basis functions
x_values = linspace(-pi, pi, 6);
sigma_x = (x_values(2) - x_values(1)) / 2;
v_values = linspace(-cart.wMax, cart.wMax, 6);
sigma_v = (v_values(2) - v_values(1)) / 2;
[X_radial, V_radial] = meshgrid(x_values, v_values); 
% Define vector function for all phi's
Phi_i = @(x, v) reshape(exp(- ( (x - X_radial).^2 / sigma_x + (v - V_radial).^2 / sigma_v )).',[],1);

error = 124120;
k = 1;
actTime = zeros(100, 1);
endCounter = 0;
%rng('shuffle')
while endCounter < 10
    jDelta = zeros(nH,1);
    Delta = zeros(nH, 36);
    
    for i = 1 : nH
        delta = randn(36, 1) * variance;
        state = [0, 20 / 180 * pi * 2 * (rand - 0.5), 0, 0];
        
        jPlus = rollout(w, delta, Phi_i, state);
        jMinus = rollout(w, -delta, Phi_i, state);
        
        actTime(i) = (jPlus + jMinus) / 2;

        jDelta(i) = jPlus - jMinus;
        Delta(i,:) = delta';
    end
    
    wNew = w + (0.5 * alpha * inv(transpose(Delta) * Delta + cart.lambda * eye(36)) * transpose(Delta) * jDelta);
    error = norm(inv(transpose(Delta) * Delta + cart.lambda * eye(36)) * transpose(Delta) * jDelta);
    
    if error < 0.0000001
        endCounter = endCounter + 1;
    else
        endCounter = 0;
    end
    w = wNew;
    fprintf('Iteration %i, mean time %0.2f, norm %0.10f \n', k, mean(actTime), error);
    k = k + 1;
end

while 1
    statePos = state(1:2)';
    stateVel = state(3:4)';
    k = 1;

    while k < T
        H = [cart.m + cart.M, -cart.m * cart.l * cos(statePos(2)); ...
        -cart.m * cos(statePos(2)), cart.m * cart.l];

        % C(q, dq):
        C = [0, cart.m * cart.l * sin(statePos(2)) * stateVel(2); ...
        0, 0];
        % G(q):
        G = [0;...
            -cart.m * cart.g * sin(statePos(2))];
        % and B:
        B = [1;...
            0];

        action = transpose(w + delta) * Phi_i(state(2), state(4));
        actionVec = [action; 0];

        statePos = statePos + t * stateVel;
        stateVel = stateVel + t * inv(H) * (actionVec - C * stateVel - G);

        state = [statePos, stateVel]; 
    
        cartPlot(cart.h, statePos); pause(0.001);
        
        k = k + t;
    end
    state = [0, 20 / 180 * pi * 2 * (rand - 0.5), 0, 0];
end

%%Functions
function R = rollout(w, delta, func, startState)
    global cart;
    state = startState;
    statePos = state(1:2)';
    stateVel = state(3:4)';
    T = 10;
    R = 0;
    t = 0.01;

    if rand() < 0.0001
        plotSim = 1;
    else
        plotSim = 0;
    end
    while R < T
        if state(2) >= pi/7 || state(2) <= -pi/7
           break; 
        end
        
        H = [cart.m + cart.M, -cart.m * cart.l * cos(statePos(2)); ...
        -cart.m * cos(statePos(2)), cart.m * cart.l];

        % C(q, dq):
        C = [0, cart.m * cart.l * sin(statePos(2)) * stateVel(2); ...
        0, 0];
        % G(q):
        G = [0;...
            -cart.m * cart.g * sin(statePos(2))];
        % and B:
        B = [1;...
            0];
    
        action = transpose(w + delta) * func(state(2), state(4));
        actionVec = [action; 0];

        statePos = statePos + t * stateVel;
        stateVel = stateVel + t * inv(H) * (actionVec - C * stateVel - G);
        
        state = [statePos, stateVel];        
 
        if plotSim
            %cartPlot(cart.h, statePos); pause(0.001);
            fprintf('Action: %0.2f, Current Time: %0.2f\n', action, R);
        end
        
        R = R + t;
    end
 end
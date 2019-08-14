clc 
close all 
clear 
%rng(time);
%% variables 
global mc
mc.g = 9.82; 
mc.L = 4;
mc.h = 1;
mc.m = 1;
mc.T = 0.05;
mc.aM = 4;
state=[mc.L/2;0];
mc.t = 10;
mc.u = -1;
mc.v_max = sqrt(2*mc.g*mc.h);
h = figure(1);
alpha = 0.4;
gamma = 1;
success = 0;

%% calculation 

A = @(x) mc.m * (1 + ((mc.h^2) / (mc.L^2)) * (pi^2) * ((sin(2 * pi * (x / mc.L)))^2));
B = @(x) mc.m * (mc.h^2 / mc.L^3) * pi^3 * sin(4 * pi * (x / mc.L));
C = @(x) -((mc.m * mc.g * mc.h) / mc.L) * pi * sin(2 * pi * (x / mc.L));
D = @(x) sqrt(1 + pi^2 * (mc.h^2 / mc.L^2) * (sin(2 * pi * (x / mc.L)))^2);
stateFunc = @(st,act) [st(1) + mc.T * st(2); 
    st(2) + mc.T * (1 / A(st(1))) * (-B (st(1)) * st(2)^2 - C(st(1)) + ((mc.aM * act) / D(st(1))))];

%% Define radial basis functions
x_values = linspace(0,mc.L,5);
sigma_x = (x_values(2)-x_values(1))/2;
v_values = linspace(-mc.v_max,mc.v_max,5);
sigma_v = (v_values(2)-v_values(1))/2;
[X_radial,V_radial] = meshgrid(x_values, v_values); 
% Define vector function for all phi's
Phi_i = @(x,v) reshape(exp(- ( (x-X_radial).^2/sigma_x + (v-V_radial).^2/sigma_v ) ).',[],1);

%% Instantiate Weights
W_plus = rand(25,1);
W_minus = rand(25,1);
W = {W_plus, W_minus};

%% Choose random action for start
actions = [-1,1];
rand_act = randi([1,2],1);
action = actions(rand_act);

%% Training the network
epsilon_default = 0.9;

for i=1:30
    time_counter = 0;
    epsilon = epsilon_default / i;
    state = [mc.L / 2; 0];
    new_state = state;
    
    while time_counter < mc.t 
        randomEpsilonVal = rand(1);
        if randomEpsilonVal < epsilon
            actions = [-1,1];
            rand_act = randi([1,2],1);
            action = actions(rand_act); 
            new_state = stateFunc(state, action);
            
            if new_state(1) >= mc.L || new_state(1) <=0  
                if action == -1
                    W{2} = W{2} - alpha * (new_state(1) + transpose(W{2})* ( - Phi_i(state(1),state(2)))) *Phi_i(state(1),state(2));                
                else
                    W{1} = W{1} - alpha * (new_state(1) + transpose(W{1})* ( - Phi_i(state(1),state(2)))) *Phi_i(state(1),state(2));                
                end
                
                if(new_state(1) >= mc.L)
                    success = success + 1;
                end
                break;
            end
            
            if action == -1
                W{2} = W{2} - alpha * (new_state(1) + transpose(W{2})* (gamma* Phi_i(new_state(1),new_state(2)) - Phi_i(state(1),state(2)))) *Phi_i(state(1),state(2));                
            else
                W{1} = W{1} - alpha * (new_state(1) + transpose(W{1})* (gamma* Phi_i(new_state(1),new_state(2)) - Phi_i(state(1),state(2)))) *Phi_i(state(1),state(2));                
            end
            
        else
            Q_plus = transpose(W{1})*Phi_i(state(1),state(2));
            Q_minus = transpose(W{2})*Phi_i(state(1),state(2));
            if Q_minus < Q_plus
               action = -1;
               new_state = stateFunc(state,action);
               if new_state(1) >= mc.L || new_state(1) <=0
                    W{2} = W{2} - alpha * (new_state(1) + transpose(W{2})* ( - Phi_i(state(1),state(2)))) *Phi_i(state(1),state(2));
                    
                    if(new_state(1) >= mc.L)
                        success = success + 1;
                    end
                    break;
               end
               %Q = transpose(W{2})*Phi_i(state(1),state(2));
               W{2} = W{2} - alpha * (new_state(1) + transpose(W{2})* (gamma* Phi_i(new_state(1),new_state(2)) - Phi_i(state(1),state(2)))) *Phi_i(state(1),state(2));
            else
                action = 1;
                new_state = stateFunc(state,action);
                if new_state(1) >= mc.L || new_state(1) <=0  
                    W{1} = W{1} - alpha * (new_state(1) + transpose(W{1})* ( - Phi_i(state(1),state(2)))) *Phi_i(state(1),state(2));    
                    if(new_state(1) >= mc.L)
                        success = success + 1;
                    end
                    break;
                end
                %Q = transpose(W{1})*Phi_i(state(1),state(2));
                W{1} = W{1} - alpha * (new_state(1) + transpose(W{1})* (gamma* Phi_i(new_state(1),new_state(2)) - Phi_i(state(1),state(2)))) *Phi_i(state(1),state(2));
            end
        end
        
        mcplot(h, state); pause(0.0001);
        
        state = new_state;
        time_counter = time_counter + mc.T;
    end
    fprintf('Iteration: %i. Succes percentage: %0.2f percent\n',i, (success/i)*100);
end

succes = 0;

for j = 1:20
     state = [mc.L / 2; 0];
     
     while time_counter < mc.t 
        Q_plus = transpose(W{1})*Phi_i(state(1),state(2));
        Q_minus = transpose(W{2})*Phi_i(state(1),state(2));
        if Q_minus < Q_plus
            action = -1;
        else
            action = 1;
        end
        new_state = stateFunc(state, action);
        if(new_state(1) >= mc.L || new_state(1) <= 0)
            if(new_state(1) >= mc.L)
                success = success + 1;
            end
            break;
        end
        state = new_state; 
        mcplot(h, state); pause(0.0001);
     end
     
     fprintf('Iteration: %i. Succes percentage: %0.2f percent\n',j, (success/j)*100);
     time_counter = time_counter + mc.T;
     
end

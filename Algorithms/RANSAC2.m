function [HRNS,CRNS,DRNS,TRNS,Inliers] = RANSAC2(Xe, eta, rho)
% RANSAC function
% INPUT
% Xe : segmented image
% eta : η is a control parameter that defines the probability that the best model
% parameters will not be found.
% rho :
% OUTPUT
% HRNS : coefficient of the curve approximating the electrode axis
% CRNS : cost of the model 
% DRNS : set of control points
% TRNS : value of parameters t for control points
% Inliers : inliers of the model

%% INITIALIZATION
CRNS = Inf;
HRNS = Inf(2);
DRNS = Inf(4);
TRNS = Inf(4);
Inliers = [];

% Compute the initial number of iterations J:
ksi = 0.5;
J = log(eta) / log(1 - ksi^2);
j = 0;

% Global variable 
thresh_ids = find(Xe(:) > 0);
N = length(thresh_ids);
[x_id,y_id] = ind2sub(size(Xe),thresh_ids);


%% ITERATIONS While j ≤ J repeat:

while j < J
    % Select randomly a subset Sj ⊂ Xe, |Sj| = 2 
    nb_point_picked = 0;
    Sj = [];
    while nb_point_picked < 2
        i = randi(length(x_id(:)),1);
        Sj = [Sj;[x_id(i) y_id(i)]];
        nb_point_picked = nb_point_picked + 1;
    end
    
	% Find Dj by ordering points in Sj
    D(:,1) = Sj(1,:)';
    D(:,2) = Sj(2,:)';
    
    % Compute coefficient matrix Hj from the pair Dj .
    t1 = 0;
    t2 = norm(D(:,1)-D(:,2));
    T = [[1 1];[t1 t2]];
    H = D/T;
    
    % Check consistency of a randomly selected point x from Xe with the model 
    % c(t; Hj ). If x passes, then continue; otherwise increment j and go to step 1.
    thresh_ids = find(Xe(:) > 0);
    [x_id,y_id] = ind2sub(size(Xe),thresh_ids);
    index_rand = randi(length(x_id(:)),1);
    rand_point = [x_id(index_rand);y_id(index_rand)];
    
    vL = [D(:,2)-D(:,1);0];
    w = [rand_point-D(:,1);0];
%     d = norm(cross(vL,w)) / norm(vL);
    
    vLx = vL(1,1);
    vLy = vL(2,1);
    wx = w(1,1);
    wy = w(2,1);
    d = abs(vLx*wy - vLy * wx) / norm(vL);
    
    if (d > rho)
        j = j + 1 ;
        break;
    end
    
    % Determine the model cost C(H).
    % C(H) being define as the negativ of the number of consistent points
    % C(H) = −| {M ∈ Xe : d (c(t; H), M) ≤ ρ} |.
    
    Inliers_tmp = [];
    C = 0;
    for i=1:N
        M = [x_id(i);y_id(i)];
        w = [M-D(:,1);0];

        vLx = vL(1,1);
        vLy = vL(2,1);
        wx = w(1,1);
        wy = w(2,1);
        d = abs(vLx*wy - vLy * wx) / norm(vL);
%           d = norm(cross(vL,w)) / norm(vL);
          
        if (d < rho)
            Inliers_tmp = [Inliers_tmp M];
            C = C - 1;
        end
    end
    
    % Compare the cost of current and the best-so-far model. 
    
    if C < CRNS
        % Update best result
        HRNS = H;
        DRNS = D;
        CRNS = C;
        TRNS = T;
        Inliers = Inliers_tmp;
        % Uptade J (approach optimal number of iterations) and reset j 
        j = 0;
        ksi = -C / (N+C);
        J = log(eta) / log(1 - ksi^2);
    else
        j = j+1;
    end
end
end


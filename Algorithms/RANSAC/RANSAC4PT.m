function [HRNS,CRNS,DRNS,TRNS,Inliers] = RANSAC4PT(Xe, eta, rho, kappa_max)
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
HRNS = Inf(4);
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

% Debug variable
display_debug = 1;


%% ITERATIONS While j ≤ J repeat:

while j < J
    DEBUG_PRINT(strcat('Starting loop, j=',int2str(j),' over ',int2str(J)),display_debug)
    % Select randomly a subset Sj ⊂ Xe, |Sj| = 2 
    nb_point_picked = 0;
    Sj = [];
    while nb_point_picked < 4
        i = randi(length(x_id(:)),1);
        Sj = [Sj;[x_id(i) y_id(i)]];
        nb_point_picked = nb_point_picked + 1;
    end
    
    % Assure that the curvature of the curve will not be too big
    Lj = mean(Sj)';
    [V,mag] = eigs(cov(Sj));;
    uj = V(:,1) / norm(V(:,1));

    kappa = -Inf;
    for i=1:4
        d = DistLinePoint(Lj,uj,Sj(i,:)');
        kappa = max(kappa,d);
    end
    
    if (kappa > kappa_max)
%         DEBUG_PRINT('kappa not accepted',display_debug)
        continue;
    end
    
%     DEBUG_PRINT('kappa accepted',display_debug)
    
    % Find Dj by ordering points in Sj
    sij = [];
    for i=1:4
        sij = [sij,dot((Sj(i,:)'-Lj),uj)];
    end
    
    
    [sorted,indices] = sort(sij);
    for i=1:4
        D(:,i) = Sj(indices(i),:);
    end
    
    % Compute coefficient matrix Hj from the pair Dj .
    t1 = 0;
    t2 = norm(D(:,1)-D(:,2));
    t3 = t2 + norm(D(:,2)-D(:,3));
    t4 = t3 + norm(D(:,3)-D(:,4));
    T = [[1 1 1 1];[t1 t2 t3 t4];[t1^2 t2^2 t3^2 t4^2];[t1^3 t2^3 t3^3 t4^3]];
    H = D/T;
    
    % Check consistency of a randomly selected point x from Xe with the model 
    % c(t; Hj ). If x passes, then continue; otherwise increment j and go to step 1.
    index_rand = randi(length(x_id(:)),1);
    rand_point = [x_id(index_rand);y_id(index_rand)];
    
    d = DistCurveToPoint(H,D,T,rand_point);
    
    if (d > rho)
        j = j + 1 ;
        continue;
    end
    
    % Determine the model cost C(H).
    % C(H) being define as the negativ of the number of consistent points
    % C(H) = −| {M ∈ Xe : d (c(t; H), M) ≤ ρ} |.
    
    DEBUG_PRINT(strcat('starting computing nb of inliers'),display_debug)
    Inliers_tmp = [];
    C = 0;
    for i=1:N
        M = [x_id(i);y_id(i)];
        d = DistCurveToPoint(H,D,T,M);
        if (d < rho)
%             Inliers_tmp = [Inliers_tmp M];
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
%         % Uptade J (approach optimal number of iterations) and reset j 
        j = 0;
        ksi = -C / (N+C);
        J = log(eta) / log(1 - ksi^2)
    else
        j = j+1;
    end
end
end


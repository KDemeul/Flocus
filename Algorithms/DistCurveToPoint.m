function d= DistCurveToPoint(H,D,T,P)
%DISTCURVETOPOINT approximate the distance between
% the curve c(H,t) and the point P 
% Parameters : H,D,T such as c(H,T) = D

    
    L = (mean(D'))';
    [V,mag] = eigs(cov(D'));
    u = V(:,1) / norm(V(:,1));
    
    sD1 = dot(D(:,1) - L,u);
    sD4 = dot(D(:,4) - L,u);
    sP = dot(P - L,u);
    
    t1 = T(2,1);
    t4 = T(2,4);
    
    tP = (t4-t1)/(sD4 - sD1) * (sP - sD1) + t1;
    
    d = norm(H*[1;tP;tP^2;tP^4]-P);
end


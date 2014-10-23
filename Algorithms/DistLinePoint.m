function d = DistLinePoint(L,vL,P)
%DISTLINEPOINT measure the euclidian distance between 
% the line defined by point L and vector vL and the point P
    w = P-L;
    vLx = vL(1,1);
    vLy = vL(2,1);
    wx = w(1,1);
    wy = w(2,1);
    d = abs(vLx*wy - vLy * wx) / norm(vL);
end


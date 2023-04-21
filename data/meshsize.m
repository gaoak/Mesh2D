function [s] = meshsize(pts, s0)
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
x0 = 0.12;
y0 = 0.01;
r = 0.05;
s = s0;
if((pts(1)-x0)^2+(pts(2)-y0)^2 < r*r)
    s = min(s0, 3.000e-04);
end
end


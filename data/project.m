function [p2] = project(p)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
p2 = p;
radius = sqrt(p(:,1).^2+p(:,2).^2);
theta = asin(p(:,1)./radius);
p2(:,1) = radius .* theta;
p2(:,2) = p(:,3);
p2(:,3) = 0;
end


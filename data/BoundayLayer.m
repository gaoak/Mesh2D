function [l] = BoundayLayer(h0, ratio, hmax,length)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
l = [0];
s = 0.;
while(s<length)
    s = s + h0;
    l = [l; s];
    h0 = h0 * ratio;
    if(h0>hmax)
        h0 = hmax;
    end
end
end


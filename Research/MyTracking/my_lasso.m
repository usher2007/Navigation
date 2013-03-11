function [ Alpha ] = my_lasso( x, dict, params )
%MY_LASSO Summary of this function goes here
%   Detailed explanation goes here
    x=x-repmat(mean(x),[size(x,1) 1]);
    x=x ./ repmat(sqrt(sum(x.^2)),[size(x,1) 1]);
    
    Alpha = mexLasso(x, dict, params);

end


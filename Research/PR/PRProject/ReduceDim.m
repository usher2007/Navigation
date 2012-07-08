function [ result ] = ReduceDim( data, dim )
%REDUCEDIM Summary of this function goes here
%   Detailed explanation goes here
    coeff = princomp(data);
    
    [row col] = size(data);
    data = data - repmat(mean(data), row, 1);
    
    pre_res = data * coeff(:, 1:dim);
    for i=1:dim
        pre_res(:,i) = pre_res(:,i)/norm(pre_res(:,i));
    end
    result = pre_res' * data;
    [row col] = size(result);
    for i=1:col
        result(:,i) = result(:,i) / norm(result(:,i));
    end
end


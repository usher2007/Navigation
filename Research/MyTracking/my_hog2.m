function [ H ] = my_hog2( img, bin_size, n_orients, index)
%MY_HOG Summary of this function goes here
%   Detailed explanation goes here
    img = single(img);
    h = hog(img, bin_size, n_orients);
    h = h(:,:,index:4:end);
    [m,n,q] = size(h);
    H = reshape(h, 1, m*n*q)';
end

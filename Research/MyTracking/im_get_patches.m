function [ patches ] = im_get_patches( img, n_row, n_col )
%IM_GET_PATCHES Summary of this function goes here
%   Detailed explanation goes here
    [img_row, img_col] = size(img);
    patch_row = ceil(img_row/n_row);
    patch_col = ceil(img_col/n_col);
    object_patches_vec = im2col(img, [patch_row patch_col], 'distinct');
    object_patches = zeros(n_row*n_col, patch_row, patch_col);
    for i=1:n_row*n_col
        object_patches(i, :, :) = reshape(object_patches_vec(:,i), patch_row, patch_col);
    end
    
    patches = single(object_patches);
end


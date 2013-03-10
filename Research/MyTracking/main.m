close all, clear all;
fprefix = 'data\car4\';
fannote = 'car4.txt';
fext = 'bmp';
s_annotation = strcat(fprefix, fannote);
start_frame = 1;
nframes = 659;

n_row = 3;
n_col = 3;

t_row = 60;
t_col = 75;

patch_row = ceil(t_row / n_row);
patch_col = ceil(t_col / n_col);

hog_bin_size = 5;
hog_n_orient = 9;
hog_index = 1;

s_frames = cell(nframes, 1);
for i=1:nframes
    image_no = start_frame + i - 1;
    fid = sprintf('%d', image_no);
    s_frames{i} = strcat(fprefix, fid, '.', fext);
end

%for the 1st frame, get the true object
truth = load(s_annotation);
first_loc = truth(1,:);
image_gray = imread(s_frames{1});
if(size(image_gray,3) ~= 1)
    image_gray = rgb2gray(image_gray);
end
[image_h image_w] = size(image_gray);
col_begin = first_loc(1);
row_begin = first_loc(2);
col_end = col_begin + first_loc(3);
row_end = row_begin + first_loc(4);
object = image_gray(row_begin:row_end, col_begin:col_end);
object = imresize(object, [t_row t_col]);
object_patches = im_get_patches(object, n_row, n_col);
hog_row = ceil(t_row/n_row)/hog_bin_size;
hog_col = ceil(t_col/n_col)/hog_bin_size;
hog_feature_len = hog_row*hog_col*hog_n_orient;
HT = zeros(n_row*n_col, 11+hog_feature_len, hog_feature_len);
for i=1:n_row*n_col
    HT(i, 1, :) = my_hog(object_patches(i, :, :), hog_bin_size, hog_n_orient, hog_index);
end
% trival templates
for i=1:n_row*n_col
    HT(i, 12:end, :) = eye(hog_feature_len);
end

for i=2:nframes
    image_gray = imread(s_frames{i});
    image_patches = im2col(image_gray, [n_row n_col], 'distinct');
end
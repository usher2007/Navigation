close all,clear all, clc;
im1 = imread('test3.jpg');
im2 = imread('test3.jpg');
marker = [100 150 50]/255;
[height1 width1 tun] = size(im1);
[height2 width2 tun] = size(im2);
im1 = im2double(im1);
im2 = im2double(im2);
markers_im1 = zeros(16,2);
markers_im2 = zeros(16,2);
count = 0;
for i=1:height1
    for j=1:width1
        tmp_pix(1:3) = im2(i,j,:);
        if max(abs(tmp_pix -marker)) < 0.08
            count = count + 1;
            markers_im2(count,:) = [i,j];
            im2(i-10:i+10,j-10:j+10,:) = zeros(21,21,3);
        end
    end
end
count = 0;
first_group_x = [1 88 189 313 415 441;
                 1 88 189 303 415 441;
                 1 137 193 292 356 441;
                 1 95 192 292 368 441;
                 1 121 181 307 407 441;
                 1 121 181 307 407 441];
first_group_y = [1 57 128 244 301 358;
                 1 57 128 244 301 358;
                 1 42 136 225 310 358;
                 1 102 147 219 260 358;
                 1 50 107 269 321 358;
                 1 50 107 269 321 358];
second_group_x = [1 79 196 328 366 441;
                  1 79 196 328 366 441;
                  1 144 206 303 374 441;
                  1 87 202 301 373 441;
                  1 105 182 328 389 441;
                  1 105 182 328 389 441];
second_group_y = [1 68 142 235 291 358;
                  1 68 142 235 291 358;
                  1 45 132 224 305 358;
                  1 108 143 219 244 358;
                  1 63 102 252 303 358;
                  1 63 102 252 303 358];
save markers2.mat first_group_x first_group_y second_group_x second_group_y

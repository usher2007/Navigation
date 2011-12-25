close all,clear all, clc;
im1 = imread('des2.jpg');
im2 = imread('des2.jpg');
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
            im2(i-5:i+5,j-5:j+5,:) = zeros(11,11,3);
        end
    end
end
count = 0;
first_group_x = [1 43 84 123 146 161;
                 1 43 84 123 146 161;
                 1 34 79 116 144 161;
                 1 34 82 117 145 161;
                 1 43 90 125 144 161;
                 1 43 90 125 144 161];
first_group_y = [1 19 36 64 83 93;
                 1 19 36 64 83 93;
                 1 17 35 60 85 93;
                 1 24 37 56 71 93;
                 1 13 40 53 80 93;
                 1 13 40 53 80 93];
second_group_x = [1 20 63 102 129 161;
                  1 20 63 102 129 161;
                  1 13 60 94 126 161;
                  1 14 62 98 126 161;
                  1 22 69 104 129 161;
                  1 22 69 104 129 161];
second_group_y = [1 15 32 62 80 93;
                  1 15 32 62 80 93;
                  1 12 31 57 82 93;
                  1 21 31 56 69 93;
                  1 13 33 52 81 93;
                  1 13 33 52 81 93];
save markers_yubo_v1.mat first_group_x first_group_y second_group_x second_group_y

close all, clear all, clc;
video = mmreader('movie1.avi');
height = video.Height;
width = video.Width;
range = [1 height 1 width];
A = [1 0 1 0 0.5 0;0 1 0 1 0 0.5;0 0 1 0 1 0;0 0 0 1 0 1;0 0 0 0 1 0;0 0 0 0 0 1];
Q = zeros(6);
R = zeros(2);
P = diag(ones(1,6));
H = [1 0 0 0 0 0;
     0 1 0 0 0 0];
N = 100;
circle = im2double(imread('frame.bmp'));
if video.NumberOfFrames < 100
    N = video.NumberOfFrames;
end
avg_frame = im2double(read(video,1))/N;
% for index = 2:N
%     avg_frame = avg_frame + im2double(read(video,index))/N;
% end
x_last = [100 100];
count_of_fail = 0;
for index = 1:video.NumberOfFrames
    index
    frame = im2double(read(video,index));
    if index ~= 1
        range = [x(1,1)-50 x(1,1)+50 x(1,2)-50 x(1,2)+50];
    end
    x = findTargetInRange(frame,avg_frame,range);
    %if didn't find the target in the region
    if max(x) < 1
        count_of_fail = count_of_fail + 1;
        x = x_last;
        if count_of_fail >= 10
            x = findTarget(frame,avg_frame);
            count_of_fail = 0;
        end
    else
        count_of_fail = 0;
    end
    frame = noteTarget(frame, x, circle);
    filename = ['DataSimpRange\' int2str(index) '.jpg'];
    imwrite(frame, filename, 'JPG');
    x_last = x;
%     if index == 1
%         x_p = [x 0.5 0.5 0.5 0.5]';
%     end
%     [xk_p,P] = kalmanFiltering(A,P,Q,H,R,x,x_p);
%     x_p = xk_p;
end
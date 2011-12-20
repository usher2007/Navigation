close all, clear all, clc;
video = mmreader('movie1.avi');
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
for index = 2:N
    avg_frame = avg_frame + im2double(read(video,index))/N;
end
for index = 1:video.NumberOfFrames
    frame = im2double(read(video,index));
    x = findTarget(frame,avg_frame);
    frame = noteTarget(frame, x, circle);
    filename = ['DataRaw\' int2str(index) '.jpg'];
    imwrite(frame, filename, 'JPG');
%     if index == 1
%         x_p = [x 0.5 0.5 0.5 0.5]';
%     end
%     [xk_p,P] = kalmanFiltering(A,P,Q,H,R,x,x_p);
%     x_p = xk_p;
end
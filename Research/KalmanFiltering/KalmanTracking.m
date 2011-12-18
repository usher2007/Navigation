close all, clear all, clc;
video = mmreader('movie1.avi');
A = [1 0 1 0 0.5 0;0 1 0 1 0 0.5;0 0 1 0 1 0;0 0 0 1 0 1;0 0 0 0 1 0;0 0 0 0 0 1];
Q = zeros(6);
R = zeros(2);
P = diag(ones(1,6));
H = [1 0 0 0 0 0;
     0 1 0 0 0 0];
for index = 1:video.NumberOfFrames
    frame = read(video,index);
    x = findTarget(frame);
    if index == 1
        x_p = [x 0.5 0.5 0.5 0.5]';
    end
    [xk_p,P] = kalmanFiltering(A,P,Q,H,R,x,x_p);
    x_p = xk_p;
end
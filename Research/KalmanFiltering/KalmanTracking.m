close all, clear all, clc;
video = mmreader('movie1.avi');
height = video.Height;
width = video.Width;
range = [1 height 1 width];
A = [1 0 1 0 0.5 0;0 1 0 1 0 0.5;0 0 1 0 1 0;0 0 0 1 0 1;0 0 0 0 1 0;0 0 0 0 0 1];
Q = diag(ones(1,6)*0.00001);
R = diag(ones(1,2)*0.001);
P = zeros(6);
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
    %use kalman filter to predict
    if index == 1
        x = findTargetInRange(frame,avg_frame,range);
    end
    %Kalman Tracking Predict
    if index == 1
        x_p = [x 0.5 0.5 0.5 0.5]';
    end
    [xk_p,P] = kalmanFiltering(A,P,Q,H,R,x,x_p);
    x_p = xk_p;
    x = round(x_p);
    range = [x(1,1)-25 x(1,1)+25 x(2,1)-25 x(2,1)+25];
    if range(1,1)<1
        range(1,1) = 1
    end
    if range(1,2) > height
        range(1,2) = height;
    end
    if range(1,3) <1
        range(1,3) = 1;
    end
    if range(1,4) > width
        range(1,4) = width;
    end
    x = findTargetInRange(frame,avg_frame,range);
    x
    %if didn't find the target in the region
    if max(x) < 1
        count_of_fail = count_of_fail + 1;
        x = x_last;
        if count_of_fail >= 1
            x = findTarget(frame,avg_frame);
        end
    else
        count_of_fail = 0;
    end
%     %Kalman Tracking
%     if index == 1
%         x_p = [x 0.5 0.5 0.5 0.5]';
%     end
%     [xk_p,P] = kalmanFiltering(A,P,Q,H,R,x,x_p);
%     x_p = xk_p;
    %Combain the res of tracking and kalman
    if count_of_fail > 0
        x = 0.95*x + 0.05*x_p(1:2,1)';
        count_of_fail = 0;
    else
        x = 0.5*x + 0.5*x_p(1:2,1)';
    end
    x = round(x);
    frame = noteTarget(frame, x, circle);
    filename = ['DataKalmanPredict\' int2str(index) '.jpg'];
    imwrite(frame, filename, 'JPG');
    x_last = x;
end
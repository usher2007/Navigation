close all, clear all, clc;
fgroundTruth = 'data\GroundTruth2\GT_singer1';
fres = 'res\res\singer1.txt';
load(fgroundTruth);
res = load(fres);
len = size(GT_corner);
total_diff = 0;
i = 6;
while i<321
    x_truth = mean(GT_corner(1,:,i));
    y_truth = mean(GT_corner(2,:,i));
    x_res = res(i-2,1)+0.5*res(i-2,3);
    y_res = res(i-2,2)+0.5*res(i-2,4);
    x_diff = abs(x_truth-x_res);
    y_diff = abs(y_truth-y_res);
    total_diff = total_diff + sqrt(x_diff*x_diff + y_diff*y_diff);
    i = i+5;
end
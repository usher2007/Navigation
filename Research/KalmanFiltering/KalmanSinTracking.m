close all, clear all, clc;
load('sin.mat');
len = size(x,2);
A = [1 0 1 0 0.5 0;0 1 0 1 0 0.5;0 0 1 0 1 0;0 0 0 1 0 1;0 0 0 0 1 0;0 0 0 0 0 1];
Q = diag(ones(1,6)*0.00001);
R = diag(ones(1,2)*0.01);
P = zeros(6);
H = [1 0 0 0 0 0;
     0 1 0 0 0 0];
res = zeros(len,2);
for i=1:len
    zpos=[x(1,i) y(1,i)];
    if i==1
        x_p = [zpos 0.5 0.5 0.5 0.5]';
    end
    [xk_p,P] = kalmanFiltering(A,P,Q,H,R,zpos,x_p);
    x_p = xk_p;
    res(i,:) = [x_p(1) x_p(2)];
end
plot(res(:,1),res(:,2),'*');
axis([-100 1100 -350 350]);
figure;
plot(x,y,'*');
axis([-100 1100 -350 350]);
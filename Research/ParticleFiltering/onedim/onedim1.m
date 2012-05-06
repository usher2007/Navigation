% 一维particle滤波小凤 加ekf
% Process function:
%           x(k) = x(k-1)/2 + 25*x(k-1)/(1 + x(k-1)^2) + 8*cos(1.2*(k-1)) + w(k);
%
% Measurement function:
%           y(k) = (x(k)^2)/20 + v(k)
%

clear;
clc;
close all;
N = 1000;                 % Number of particles
P0 = 2;                   % Initial state covariance
m0=0;                     % Initial state mean
Q = 10;                   % Process noise covariance
R = 1;                    % Measurement noise covariance
T=50;                     % Step of time

x0=0.1;                   % 系统初始值

x(1) = x0; % Initial state value
y(1) =(x(1)^2)/20 + sqrt(R)*randn(1);

for k = 2:T             % Simulate the system
    x(k) =x(k-1)/2 + 25*x(k-1)/(1 + x(k-1)^2) + 8*cos(1.2*(k-1))+ sqrt(Q)*randn(1);% 计算真实值x
    y(k) = (x(k)^2)/20+ sqrt(R)*randn(1);    % 计算真实值y
end
xTrue = x;   % 真实值

% EKF initial
xhat=m0;
xekf(1)=xhat;
Pest=P0;

% PF initial
x = sqrt(P0)*randn(1,N);          % Initialize the particles
xpf(1) = mean(x);
% tic;

% PF and EKF filtering

for k = 2:T
    %PF filtering
    x = x./2 + 25*x./(1 + x.^2) + 8*cos(1.2*(k-1))+sqrt(Q)*randn(1,N); %predict
    h=(x.^2)/20;
    e = repmat(y(k),1,N) - h;        % Calculate weights
    q0 =(1/sqrt(2*pi*R))*exp(-e.^2/(2*R));             % The likelihood function
    q = q0/sum(q0);               % Normalize the importance weights
    
    % 重采样                     
    P = cumsum(q);               % 计算q的累加值，维数和q一样


    ut(1)=rand(1)/N;
    kk = 1; 
    i = zeros(1,N);
    for j = 1:N
        ut(j)=ut(1)+(j-1)/N;
        while(P(kk)<ut(j));
        kk = kk + 1;
        end;
    i(j) = kk;
    q(j)=1/N;
    end;
                         
    x = x(:,i);                % The new particles
    xpf(k) = mean(x);           % Compute the estimate
    
    % EKF filtering
    % predict
    f=0.5+25*(1-xhat^2)/((1+xhat^2)^2);
    xhat=xhat/2 + 25*xhat/(1 + xhat^2) + 8*cos(1.2*(k-1));
    Pest=f*Pest*f' + Q;
    % observe
    % 计算观测方程一阶导数
    h=xhat/10;
    ypred=xhat^2/20;
    v = y(k)-ypred;
    K= Pest*h'*(h*Pest*h'+R)^(-1);
    xhat=xhat+K*v;
    Pest=(1-K*h)*Pest;
    xekf(k)=xhat;
end
% time = toc
figure(1)
clf;
plot(1:T,xTrue,'b*-'); % 真实值蓝色*表示，粒子滤波值红色三角表示 ，EKF滤波用绿色加号表示
% plot(1:T,xTrue,'b*-');
% hold on
% plot(1:T,xpf,'r^-');
legend('原始值');
xlabel('k');
axis([0 50 -40 40]);
figure(2)
clf;
plot(1:T,xTrue,'b*-',1:T,xekf,'g+--'); % 真实值蓝色*表示，粒子滤波值红色三角表示 ，EKF滤波用绿色加号表示
% plot(1:T,xTrue,'b*-');
% hold on
% plot(1:T,xpf,'r^-');
legend('原始值','EKF 滤波值');
xlabel('k');
axis([0 50 -40 40]);

figure(3)
clf;
plot(xpf,xTrue,'+');
hold on;
c=-25:1:25;
plot(c,c,'r');
axis([-25 25 -25 25]);


% op=std(xpf-xTrue)
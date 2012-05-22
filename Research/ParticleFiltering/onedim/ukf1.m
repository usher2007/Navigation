function [xest,Pest]=ukf1(xest,Pest,f,a,b,z,h,R,t,alpha,beta,kappa);

% TITLE    :  UNSCENTED KALMAN FILTER  
%
% PURPOSE  :  This function performs one complete step of the unscented Kalman filter.
%
% SYNTAX   :  [xEst,PEst,xPred,PPred,zPred,inovation]=ukf(xest,Pest,Q,f,z,R,h,alpha,beta,kappa)
%
% INPUTS   :  - xEst             : state mean estimate at time t-1  
%             - PEst             : state covariance at time t-1
%             - Q                : process noise covariance at time t-1  
%             - z                : observation at t  
%             - R                : measurement noise covariance at t  
%             - f                : process model function  
%             - h                : observation model function  
%               
%	          - alpha (optional) : sigma point scaling parameter. Defaults to 1.
%             - beta  (optional) : higher order error scaling parameter. Default to 0.  
%             - kappa (optional) : scalar tuning parameter 1. Defaults to 0.  
%
% OUTPUTS  :  - xest             : updated estimate of state mean at time t
%	          - Pest             : updated state covariance at time t
%            
%  
% AUTHORS  :  Simon J. Julier       (sjulier@erols.com)    1998-2000
%             Rudolph van der Merwe (rvdmerwe@ece.ogi.edu) 2000
%
% DATE     :  14 August 2000
%
% NOTES    :  The process model is of the form, x(k+1) = ffun[x(k),v(k)]
%             where v(k) is the process noise vector. The observation model is 
%             of the form, z(k) = hfun[x(k),w(k)], where w(k) is the 
%             observation noise vector.
%
%             This code was written to be readable. There is significant
%             scope for optimisation even in Matlab.
%
  

% Process defaults

if (nargin <10)
  alpha=1;
end;

if (nargin <11)
  beta=0;
end;

if (nargin < 12)
  kappa=0;
end;

Q=a*(b^2);
n_x=size(xest,1);   %状态向量的维数
n_w=size(Q,1);      %过程噪声的维数
n_v=size(R,1);      %观测噪声的维数



xest=[xest' zeros(size(Q,1),1) zeros(size(R,1),1)]';
Pest=blkdiag(Pest,Q,R);

n=n_x+n_w+n_v;   %扩维后的总的状态向量的维数

m=size(z,1);   %观测向量的维数

% Calculate the sigma points and there corresponding weights using the Scaled Unscented
% Transformation
[xSigmaPts, wmSigmaPts, wcSigmaPts, nsp] = scaledSymmetricSigmaPoints(xest,Pest,alpha,beta,kappa); 

% Duplicate wmSigmaPts,wcSigmaPts into matrix for code speedup
wmSigmaPts_xmat = repmat(wmSigmaPts,n_x,1);
wcSigmaPts_xmat = repmat(wcSigmaPts,n_x,1);

wmSigmaPts_zmat = repmat(wmSigmaPts,m,1);
wcSigmaPts_zmat = repmat(wcSigmaPts,m,1);


% Work out the projected sigma points and their means
% This routine is fairly generic. The only thing to watch out for are
% angular discontinuities. There is a standard trick for doing this -
% contact me (Julier) for details!

xPredSigmaPts = feval(f,xSigmaPts(1:n_x,:),t)+xSigmaPts(n_x+1:n_x+n_v,:);

% Calculate the mean. Based on discussions with C. Schaefer, form
% is chosen to maximise numerical robustness.
% - I vectorized this part of the code for a speed increase : RvdM 2000

xPred = sum(wmSigmaPts_xmat.* xPredSigmaPts(1:n_x,:), 2 );
exSigmaPts=xPredSigmaPts-repmat(xPred,1,nsp);
PxPred=(wcSigmaPts_xmat.*exSigmaPts)*exSigmaPts';


zPredSigmaPts = feval(h,xPredSigmaPts)+xSigmaPts(n-n_v+1:n,:);
zPred = sum(wmSigmaPts_zmat.* zPredSigmaPts,2);

% Work out the covariances and the cross correlations. Note that
% the weight on the 0th point is different from the mean
% calculation due to the scaled unscented algorithm.

ezSigmaPts=zPredSigmaPts-repmat(zPred,1,nsp);
PzPred= (wcSigmaPts_zmat .* ezSigmaPts) * ezSigmaPts';
PxzPred   =exSigmaPts * (wcSigmaPts_zmat .* ezSigmaPts)';


%%%%% MEASUREMENT UPDATE

% Calculate Kalman gain
K  = PxzPred / PzPred;
%K  = PxzPred *inv(PzPred);

% Calculate Innovation
inovation = z - zPred;

% Update mean
xest = xPred + K*inovation;

% Update covariance
Pest = PxPred - K*(PzPred)*K';
function [qt,qts] = CMRspline2(P,N,flag)
% CMRspline generate a catmull-rom spline according to the given points P
% and the number of points N for every piece
% P is the grid points you chose, align as a m*2 matrix
% N means the number of points you want, same of the length or width of the 
% picutre you, actually you just need to kown the flag and get the number
% from the P group, this function will appear in CMRspline3
%
% qt is the interpolate group
% qts is the resample group
% N is the number of points you want the final result
% flag=1 resample along the x axis
% flag=0 resample along the y axis
[m n] = size(P);
qt = zeros((m-1)*N+1,2);
t = 0:1/N:1;
A = [0 2 0 0; -1 0 1 0; 2 -5 4 -1; -1 3 -3 1];

for i = 1:m-1
    %generate your Ptemp the four points group used in the formula
    if i == 1
        Ptemp = [P(1,:);P(1,:);P(2,:);P(3,:)];
    else if i == m-1
            Ptemp = [P(m-2,:);P(m-1,:);P(m,:);P(m,:)];
        else
            Ptemp = [P(i-1,:);P(i,:);P(i+1,:);P(i+2,:)];
        end
    end
    for j = 1:N
        u = [1 t(j) t(j)^2 t(j)^3];
        qt(j+(i-1)*N,:) = 0.5*u*A*Ptemp;
    end
end
qt(3*N+1,:) = P(end,:);

%resample qt according to the flag
qts = zeros(N,2);
qts(1,:) = qt(1,:);
qts(N,:) = qt(end,:);
interpx = qt(1,1):1/(N-1):qt(end,1);
interpy = qt(1,2):1/(N-1):qt(end,2);
for i = 2:N-1
    if flag == 1
        qts(i,1) = interpx(i);
        qts(i,2) = myresample(qt,interpx(i),flag);
    else
        qts(i,2) = interpy(i);
        qts(i,1) = myresample(qt,interpy(i),flag);
    end        
end
end

% figure,plot(qt(:,1),qt(:,2),'bx')
% hold on
% plot(P2(:,1),P2(:,2),'ro')
% end
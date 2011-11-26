function [hough_space,hough_circle,para] = hough_circle(BW,step_r,step_angle,r_min,r_max,p)

% %%%%%%%%%%%%%%%%%%%%%%%%%%
% input
% BW:二值图像；
% step_r:检测的圆半径步长
% step_angle:角度步长，单位为弧度
% r_min:最小圆半径
% r_max:最大圆半径
% p:阈值，0，1之间的数
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% output
% hough_space:参数空间，h(a,b,r)表示圆心在(a,b)半径为r的圆上的点数
% hough_circl:二值图像，检测到的圆
% para:检测到的圆的圆心、半径

[m,n] = size(BW);%get the size of the binary picture
size_r = round((r_max-r_min)/step_r)+1;%size of hough_r
size_angle = round(2*pi/step_angle);%size of hough_angle

hough_space = zeros(m,n,size_r);

[rows,cols] = find(BW);
ecount = size(rows);

% Hough变换
% 将图像空间(x,y)对应到参数空间(a,b,r)
% a = x-r*cos(angle)
% b = y-r*sin(angle)
for i=1:ecount
    for r=1:size_r
        for k=1:size_angle
            a = round(rows(i)-(r_min+(r-1)*step_r)*cos(k*step_angle));
            b = round(cols(i)-(r_min+(r-1)*step_r)*sin(k*step_angle));
            if(a>0&a<=m&b>0&b<=n)
                hough_space(a,b,r) = hough_space(a,b,r)+1;
            end
        end
    end
end

% 搜索超过阈值的聚集点
max_para = max(max(max(hough_space)));
index = find(hough_space>=max_para*p);
length = size(index);
hough_circle = false(m,n);
for i=1:ecount
    for k=1:length
        par3 = floor(index(k)/(m*n))+1;
        par2 = floor((index(k)-(par3-1)*(m*n))/m)+1;
        par1 = index(k)-(par3-1)*(m*n)-(par2-1)*m;
        if((rows(i)-par1)^2+(cols(i)-par2)^2<(r_min+(par3-1)*step_r)^2+5&...
                (rows(i)-par1)^2+(cols(i)-par2)^2>(r_min+(par3-1)*step_r)^2-5)
            hough_circle(rows(i),cols(i)) = true;
        end
    end
end

% 打印检测结果
for k=1:length
    par3 = floor(index(k)/(m*n))+1;
    par2 = floor((index(k)-(par3-1)*(m*n))/m)+1;
    par1 = index(k)-(par3-1)*(m*n)-(par2-1)*m;
    par3 = r_min+(par3-1)*step_r;
    fprintf(1,'Center %d %d radius %d\n',par1,par2,par3);
    para(1,k) = par1;
    para(2,k) = par2;
    para(3,k) = par3;
%     para(:,k) = [par1,par2,par3];
end
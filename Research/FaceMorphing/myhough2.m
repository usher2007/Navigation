clear all,close all,clc
f=imread('test.jpg'); 
img_size = size(f);
img_height = img_size(1,1);
img_width = img_size(1,2);
i=rgb2gray(f); 
i_long =size(i,1); 
i_width=size(i,2); 
i_edge=edge(i,'sobel');
figure,imshow(i_edge)
r_min = 7;
r_max = 10;
r_step = 0.5;
[hough_space,hough_circle,para] = hough_circle(i_edge,r_step,0.02,r_min,r_max,1);
%para
%[m n] = size(para);
%for i = 1:n    
%    g = drawcircle(f,para(:,i),[255 0 0]);    
%end
g = drawcircle(f,para(:,1),[0 255 0]);  
left_eye = para;
%find the next eye by covering the first eye
new_hough_space = hough_space;
%set the (a,b,r) == 0
a = para(1);
b = para(2);
c = round(para(3))+1;
r = para(3);
[m n p] = size(new_hough_space);
for i = a-c:a+c
    for j = b-c:b+c
        if (i-a)^2 + (j-b)^2 <= r^2
            for k = 1:p
                new_hough_space(i,j,k) = 0;
            end
            %new_hough_space(i,j,:) = zeros(1,1,p);      
        end
    end
end
max_para = max(max(max(new_hough_space)));
index = find(new_hough_space>=max_para)
[m n k] = size(f);
par3 = floor(index/(m*n))+1;
par2 = floor((index-(par3-1)*(m*n))/m)+1;
par1 = index-(par3-1)*(m*n)-(par2-1)*m;
par3 = 7+(par3-1)*0.5;
par = [par1 par2 par3]
h = drawcircle(g,par,[0 255 0]);  
right_eye = par;
%find the next eye on the same r layer
%a = para(1);
%b = para(2);
%r = para(3);
%k = (r-r_min)/r_step+1;
%Same_rlayer = hough_space(:,:,k);
%Same_rlayer(a,b) = 0;
%nextmax = max(max(Same_rlayer));
%index = find(Same_rlayer == nextmax);
%par3 = floor(index/(m*n))+1;
%par2 = floor((index-(par3-1)*(m*n))/m)+1;
%par1 = index-(par3-1)*(m*n)-(par2-1)*m;
%par3 = r_min+(par3-1)*r_step;
%par = [par1 par2 par3]

red = f(:,:,1);
green = f(:,:,2);
blue = f(:,:,3);

red_coef = red - green;
d_red_coef = im2double(red_coef);
mouth = d_red_coef(2:img_height-1,2:img_width-1) + 0.3*d_red_coef(1:img_height-2,2:img_width-1) + 0.3*d_red_coef(3:img_height,2:img_width-1) + 0.3*d_red_coef(2:img_height-1,3:img_width) + 0.1*d_red_coef(2:img_height-1,1:img_width-2);
mouth_to_eye = 14*left_eye(3);
width = 4*left_eye(3);
left_mouth_corner_domain = mouth(left_eye(1)+0.5*mouth_to_eye:left_eye(1)+mouth_to_eye, left_eye(2)-0.5*width:left_eye(2)+0.5*width);
right_mouth_corner_domain = mouth(right_eye(1)+0.5*mouth_to_eye:right_eye(1)+mouth_to_eye, right_eye(2)-0.5*width:right_eye(2)+0.5*width);
[row_left_mouth,col_left_mouth] = find(left_mouth_corner_domain == max(max(left_mouth_corner_domain)));
[row_right_mouth,col_right_mouth] = find(right_mouth_corner_domain == max(max(right_mouth_corner_domain)));
left_mouth_corner = [row_left_mouth(1),col_left_mouth(1)] + [left_eye(1)+0.5*mouth_to_eye,left_eye(2)-0.5*width];
right_mouth_corner = [row_right_mouth(1),col_right_mouth(1)] + [right_eye(1)+0.5*mouth_to_eye, right_eye(2)-0.5*width];
h = drawcircle(h,[left_mouth_corner,3],[0 255 0]);  
h = drawcircle(h,[right_mouth_corner,3], [0,255,0]);






figure,imshow(h)
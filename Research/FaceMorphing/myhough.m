% Hough 变换 
clear all,close all,clc
i=imread('test.jpg'); 
%figure,imshow(i)
i=rgb2gray(i); 
%figure,imshow(i)
i_long =size(i,1); 
i_width=size(i,2); 
i_edge=edge(i,'robert');
figure,imshow(i_edge)
i_hough=zeros(300,300); 
%i_hough = zeros(i_long,i_width);
theta_step=3.14*2/299; 
theta=0:theta_step:2*3.14; 
 
x_max=1; 
x_min=1; 
y_max=1; 
y_min=1; 
 
for x=1:i_long 
   for y=1:i_width 
     if i_edge(x,y)==1 
     x_max=max(x_max,x); 
     x_min=min(x_min,x); 
     y_max=max(y_max,y); 
     y_min=min(y_min,y); 
     end 
   end 
end 
 
p_min=sqrt(x_min^2+y_min^2); 
p_max=sqrt(x_max^2+y_max^2); 
p_step=2*p_max/299; 
p=-p_max:p_step:p_max; 
 
for x=1:i_long 
    for y=1:i_width 
        if i_edge(x,y)==1     %对于边缘点进行累加 
           rou=x.*cos(theta)+y.*sin(theta); 
            w=fix(rou./p_step)+151; 
            l=fix(1+theta./theta_step); 
            n=300.*(l-1)+w; 
            i_hough(n)=i_hough(n)+1; 
        end 
    end 
end 
 
m=max(max(i_hough)); 
[x y] = find(i_hough == m)
i_hough=(i_hough./m); 
figure,imshow(i_hough)


function[left_eye,right_eye] = myhough2(image)
    f_gray=rgb2gray(image); 
    i_edge=edge(f_gray,'sobel');
    figure,imshow(i_edge)
    r_min = 7;
    r_max = 10;
    r_step = 0.5;
    [hough_space,hough_circle_param,para] = hough_circle(i_edge,r_step,0.02,r_min,r_max,1);
    g = drawcircle(image,para(:,1),[0 255 0]);  
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
    [m n k] = size(image);
    par3 = floor(index/(m*n))+1;
    par2 = floor((index-(par3-1)*(m*n))/m)+1;
    par1 = index-(par3-1)*(m*n)-(par2-1)*m;
    par3 = 7+(par3-1)*0.5;
    par = [par1 par2 par3]
    h = drawcircle(g,par,[0 255 0]);  
    right_eye = par;
    figure;
    imshow(h);
end
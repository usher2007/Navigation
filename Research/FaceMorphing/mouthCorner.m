function[left_mouth,right_mouth] = mouthCorner(left_eye,right_eye,image)
    red = image(:,:,1);
    green = image(:,:,2);
    blue = image(:,:,3);
    f_gray = rgb2gray(image);
    [im_height,im_width,tun] = size(image);
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %find the mouth corner just by color%
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % red_coef = red - green;
    % d_red_coef = im2double(red_coef);
    % mouth = d_red_coef(2:img_height-1,2:img_width-1) + 0.3*d_red_coef(1:img_height-2,2:img_width-1) + 0.3*d_red_coef(3:img_height,2:img_width-1) + 0.3*d_red_coef(2:img_height-1,3:img_width) + 0.1*d_red_coef(2:img_height-1,1:img_width-2);
    % mouth_to_eye = 14*left_eye(3);
    % width = 4*left_eye(3);
    % left_mouth_corner_domain = mouth(left_eye(1)+0.5*mouth_to_eye:left_eye(1)+mouth_to_eye, left_eye(2)-0.5*width:left_eye(2)+0.5*width);
    % right_mouth_corner_domain = mouth(right_eye(1)+0.5*mouth_to_eye:right_eye(1)+mouth_to_eye, right_eye(2)-0.5*width:right_eye(2)+0.5*width);
    % [row_left_mouth,col_left_mouth] = find(left_mouth_corner_domain == max(max(left_mouth_corner_domain)));
    % [row_right_mouth,col_right_mouth] = find(right_mouth_corner_domain == max(max(right_mouth_corner_domain)));
    % left_mouth_corner = [row_left_mouth(1),col_left_mouth(1)] + [left_eye(1)+0.5*mouth_to_eye,left_eye(2)-0.5*width];
    % right_mouth_corner = [row_right_mouth(1),col_right_mouth(1)] + [right_eye(1)+0.5*mouth_to_eye, right_eye(2)-0.5*width];
    % h = drawcircle(h,[left_mouth_corner,3],[0 255 0]);  
    % h = drawcircle(h,[right_mouth_corner,3], [0,255,0]);

    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %find the mouth corner by harris point and color  %
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    harris_metric = cornermetric(f_gray, 'Harris');
    harris_metric = imadjust(harris_metric);
    figure;
    imshow(harris_metric);
    harris_max = max(max(harris_metric));
    harris_thres = 0.3*harris_max;
    [harris_row,harris_col] = find(harris_metric>harris_thres);

    red_coef = red - 3*green;
    d_red_coef = im2double(red_coef);
    d_red_coef = imadjust(d_red_coef);
    figure;
    imshow(d_red_coef);
    red_thres = max(max(d_red_coef)) * 0.5;

    %eye params
    mouth_to_eye_min = 10*left_eye(3);
    mouth_to_eye_max = 20*left_eye(3);
    mouth_width_max = 10*left_eye(3);

    mouth_row = zeros(1,100);
    mouth_col = zeros(1,100);
    mouth_weight = zeros(1,100);
    mouth_index = 1;
    for i=1:length(harris_row)
        if (harris_row(i)<3) || (harris_row(i)>(im_height-3)) || (harris_col(i) < 3) || (harris_col(i) > (im_width-3))
            continue;
        end
        red_weight = max(max(d_red_coef(harris_row(i)-2:harris_row(i)+2, harris_col(i)-2:harris_col(i)+2)));
        harris_weight = harris_metric(harris_row(i), harris_col(i));
        distance_y = abs(left_eye(1)-harris_row(i));
        if red_weight > red_thres && distance_y > mouth_to_eye_min && distance_y < mouth_to_eye_max 
            mouth_row(mouth_index) = harris_row(i);
            mouth_col(mouth_index) = harris_col(i);
            mouth_weight(mouth_index) = red_weight*0.5 + harris_weight*0.5;
            mouth_index = mouth_index + 1;
        end
    end
    %first corner
    [v,index] = max(mouth_weight);
    row_first = mouth_row(index);
    col_first = mouth_col(index);
    h = drawcircle(image,[row_first,col_first,3],[0 255 0]);
    mouth_width_thres = 7*left_eye(3);
    %second corner
    for i=1:length(mouth_weight)
        mouth_weight(index) = 0;
        [v,index] = max(mouth_weight);
        row = mouth_row(index);
        col = mouth_col(index);
        if col-col_first>mouth_width_thres
            break;
        end
    end
    h = drawcircle(h,[row,col,3],[0 255 0]);

    if col<col_first
        left_mouth = [row,col];
        right_mouth = [row_first,col_first];
    else
        left_mouth = [row_first,col_first];
        right_mouth = [row,col];
    end
    figure,imshow(h)
end
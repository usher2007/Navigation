function [img] = GetMidianImg(img_src, src_x, src_y, des_x, des_y)
%Get one midian image using two group of control points
    [row_x,col_x] = size(src_x);
    [row_y,col_y] = size(src_y);
    aux_x = src_x;
    aux_y = des_y;
    [img_height, img_width, img_tun] = size(img_src);
    img_aux = zeros(img_height, img_width, img_tun);
    img = zeros(img_height, img_width, img_tun);
    sp_src = zeros(row_x, 3*img_height+1, 2);
    sp_mid_src = zeros(row_x, 3*img_height+1, 2);
    sp_mid_des = zeros(row_y, 3*img_width+1, 2);
    sp_des = zeros(row_y, 3*img_width+1, 2);
    sp_src(1,1:(3*img_height+1),1:2) = [[1:(img_height-1)/(3*img_height):img_height]' ones(3*img_height+1,1)];
    sp_mid_src(1,:,:) = sp_src(1,:,:);
    sp_des(1,1:(3*img_width+1),1:2) = [[1:(img_width-1)/(3*img_width):img_width]' ones(3*img_width+1,1)];
    sp_mid_des(1,:,:) = sp_des(1,:,:);
    for i=2:row_x-1
        [tmp,sp_src(i,:,:)] = CMRspline2([src_x(i,:)' src_y(:,i)], img_height,1);
        [tmp,sp_mid_src(i,:,:)] = CMRspline2([aux_x(i,:)' aux_y(:,i)], img_height,1);
    end
    for i=2:row_y-1
        [tmp,sp_des(i,:,:)] = CMRspline2([des_y(i,:)' des_x(:,i)], img_width,1);
        [tmp,sp_mid_des(i,:,:)] = CMRspline2([aux_y(i,:)' aux_x(:,i)], img_width,1);
    end
    sp_src(row_x,1:(3*img_height+1),1:2) = [[1:(img_height-1)/(3*img_height):img_height]' ones(3*img_height+1,1)*img_width];
    sp_mid_src(row_x,:,:) = sp_src(row_x,:,:);
    sp_des(row_y,1:(3*img_width+1),1:2) = [[1:(img_width-1)/(3*img_width):img_width]' ones(3*img_width+1,1)*img_height];
    sp_mid_des(row_y,:,:) = sp_des(row_y,:,:);
    %From src to aux, mapping each row
    for i=1:img_height
        intersection_points_src = [];
        intersection_points_aux = [];
        for j=1:row_x
            intersection_points_src = [intersection_points_src;j,sp_src(j,i*3,2)];
            intersection_points_aux = [intersection_points_aux;sp_mid_src(j,i*3,2),j];
        end
        [tmp,src_reverse_f] = CMRspline2(intersection_points_src, img_width, 1);
        [tmp,aux_f] = CMRspline2(intersection_points_aux, img_width, 1);
        for j=1:img_width
            if j==1 || j== img_width
                img_aux(i,j,:) = img_src(i,j,:);
                continue;
            end
            aux_location_ahead = ceil((aux_f(round((j-0.5)*3+1),2)-1)/(row_x-1) * img_width * 3);
            aux_location_after = floor((aux_f(round((j+0.5)*3+1),2)-1)/(row_x-1) * img_width * 3);
            location_from = round(src_reverse_f(aux_location_ahead,2));
            location_to = round(src_reverse_f(aux_location_after,2));
            img_aux(i,j,:) = mean(img_src(i, location_from:location_to,:), 2);
        end
    end
    %From aux to des, mapping each column
    for i=1:img_width
        intersection_points_aux = [];
        intersection_points_des = [];
        for j=1:row_y
            intersection_points_aux = [intersection_points_aux;j,sp_mid_des(j,i*3,2)];
            intersection_points_des = [intersection_points_des;sp_des(j,i*3,2),j];
        end
        [tmp,aux_reverse_f] = CMRspline2(intersection_points_aux, img_height, 1);
        [tmp,des_f] = CMRspline2(intersection_points_des, img_height, 1);
        for j=1:img_height
            if j==1 || j== img_height
                img(j,i,:) = img_aux(j,i,:);
                continue;
            end
            des_location_ahead = ceil((des_f(round((j-0.5)*3+1),2)-1)/(row_y-1) * img_height * 3);
            des_location_after = floor((des_f(round((j+0.5)*3+1),2)-1)/(row_y-1) * img_height * 3);
            location_from = round(aux_reverse_f(des_location_ahead,2));
            location_to = round(aux_reverse_f(des_location_after,2));
            img(j,i,:) = mean(img_aux(location_from:location_to,i,:), 1);
        end
    end
    
end
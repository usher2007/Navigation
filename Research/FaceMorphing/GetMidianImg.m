function [img] = GetMidianImg(img_src, src_x, src_y, des_x, des_y)
    aux_x = src_x;
    aux_y = des_y;
    [feature_col, feature_row] = size(aux_x);
    [img_height, img_width, img_tun] = size(img_src);
    img_dest = zeros(img_height, img_width, img_tun);
    sp_src = zeros(feature_col, 3*img_height+1, 2);
    sp_mid_src = zeros(feature_col, 3*img_height+1, 2);
    sp_mid_des = zeros(feature_col, 3*img_width+1, 2);
    sp_des = zeros(feature_col, 3*img_width+1, 2);
    sp_src(1,1:(3*img_height+1),1:2) = [[1:(img_height-1)/(3*img_height):img_height]' ones(3*img_height+1,1)];
    sp_mid_src(1,:,:) = sp_src(1,:,:);
    sp_des(1,1:(3*img_width+1),1:2) = [ones(3*img_width+1,1) [1:(img_width-1)/(3*img_width):img_width]'];
    sp_mid_des(1,:,:) = sp_des(1,:,:);
    for i=2:feature_col-1
        [tmp,sp_src(i,:,:)] = CMRspline2([src_x(i,:)' src_y(:,i)], img_height,1);
        [tmp,sp_mid_src(i,:,:)] = CMRspline2([aux_x(i,:)' aux_y(:,i)], img_height,1);
        [tmp,sp_des(i,:,:)] = CMRspline2([des_y(i,:)' des_x(:,i)], img_width,1);
        [tmp,sp_mid_des(i,:,:)] = CMRspline2([aux_y(i,:)' aux_x(:,i)], img_width,1);
    end
    sp_src(feature_col,1:(3*img_height+1),1:2) = [[1:(img_height-1)/(3*img_height):img_height]' ones(3*img_height+1,1)*img_width];
    sp_mid_src(feature_col,:,:) = sp_src(feature_col,:,:);
    sp_des(feature_col,1:(3*img_width+1),1:2) = [ones(3*img_width+1,1)*img_height [1:(img_width-1)/(3*img_width):img_width]'];
    sp_mid_des(feature_col,:,:) = sp_des(feature_col,:,:);
    %From src to aux, mapping each row
    for i=1:img_height
        intersection_points_src = [1,sp_src(1,i*3,2);
                                   2,sp_src(2,i*3,2);
                                   3,sp_src(3,i*3,2);
                                   4,sp_src(4,i*3,2)];
        intersection_points_aux = [sp_mid_src(1,i*3,2),1;
                                   sp_mid_src(2,i*3,2),2;
                                   sp_mid_src(3,i*3,2),3;
                                   sp_mid_src(4,i*3,2),4];
        [tmp,src_reverse_f] = CMRspline2(intersection_points_src, img_width, 1);
        [tmp,aux_f] = CMRspline2(intersection_points_aux, img_width, 1);
        for j=1:img_width
            if j==1 || j== img_width
                img_dest(i,j,:) = img_src(i,j,:);
                continue;
            end
            aux_location_ahead = ceil((aux_f(round((j-0.5)*3+1),2)-1)/3.0 * img_width * 3);
            aux_location_after = floor((aux_f(round((j+0.5)*3+1),2)-1)/3.0 * img_width * 3);
            location_from = round(src_reverse_f(aux_location_ahead,2));
            location_to = round(src_reverse_f(aux_location_after,2));
            img_dest(i,j,:) = mean(img_src(i, location_from:location_to,:), 2);
        end
    end
    img=img_dest;
    %From aux to des, mapping each column
end
function [img] = GetMidianImg(img_src, src_x, src_y, des_x, des_y)
    aux_x = src_x;
    aux_y = des_y;
    [feature_col, feature_row] = size(aux_x);
    [img_height, img_width, img_tun] = size(img_src);
    img_dest = zeros(img_height, img_width, img_tun);
    sp_src = zeros(feature_col, img_height, 2);
    sp_mid_src = zeros(feature_col, img_height, 2);
    sp_mid_des = zeros(feature_col, img_width, 2);
    sp_des = zeros(feature_col, img_width, 2);
    for i=1:feature_col
        sp_src(i,:,:) = crsp(src_x(i,:), src_y(i,:), img_height);
        sp_mid_src(i,:,:) = crsp(aux_x(i,:), aux,y(i,:), img_height);
        sp_des(i,:,:) = crsp(des_y(i,:), des_x(i,:), img_width);
        sp_mid_des(i,:,:) = csrp(aux_y(i,:), aux_x(i,:), img_width);
    end
    %every row's points mapping
    for i=1:img_height
        intersection_points_src = [0,sp_src(0,i,2);
                                   1,sp_src(1,i,2);
                                   2,sp_src(2,i,2);
                                   3,sp_src(3,i,2)];
        intersection_points_aux = [0,sp_mid_src(0,i,2);
                                   1,sp_mid_src(1,i,2);
                                   2,sp_mid_src(2,i,2);
                                   3,sp_mid_src(3,i,2)];
        src_reverse_f = CMRspline2(intersection_points_src, img_width, 1);
        aux_f = CMRspline2(intersection_points_aux, img_width, 0);
        for j=1:img_width
            if j==1 || j== img_width
                img_dest(i,j,:) = img_src(i,j,:);
            end
            aux_location_ahead = cell(aux_f(j-1,2)/3.0 * img_width);
            aux_location_after = floor(aux_f(j+1,2)/3.0 * img_width);
            location_from = round(src_reverse_f(aux_location_ahead,2));
            location_to = round(src_reverse_f(aux_location_after,2));
            img_dest(i,j,:) = mean(img_src(i, location_from:location_to,:), 2);
        end
    end
end
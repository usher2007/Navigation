function position = findTarget(frame,avg_frame)
    [height width] = size(frame);
    red = frame(:,:,1);
    green = frame(:,:,2);
    diff_gr = green - red;
    [x,y] = find(diff_gr > 150/255);
    diff_cur_avg = frame-avg_frame;
    imshow(diff_gr);
    figure;
    imshow(diff_cur_avg);
    max_count = 0;
    max_x = 0;
    max_y = 0;
    for i= 1:size(x,1)
        for j= 1:size(y,1)
            xi = x(i);
            yj = y(j);
            [x_range, y_range] = getRange(xi,yj,5,height,width);
            count = 0;
            for xinner=x_range(1,1):x_range(1,2)
                for yinner=y_range(1,1):y_range(1,2)
                    if diff_gr(xinner,yinner) > 150/255
                        count = count + 1;
                    end
                end
            end
            total_pxs = (x_range(1,2)-x_range(1,1)+1)*(y_range(1,2)-y_range(1,1)+1);
            big_rate = count*1.0/total_pxs;
            if big_rate > max_count
                max_count = big_rate;
                max_x = xi;
                max_y = yj;
            end
        end
    end
    position = [max_x max_y];
end

function [x_range, y_range] = getRange(xi,yi,num,height,width)
    x_from = xi-num;
    x_to = xi+num;
    y_from = yi-num;
    y_to = yi+num;
    if x_from < 1
        x_from = 1;
    end
    if y_from < 1
        y_from = 1;
    end
    if x_to >height
        x_to = height;
    end
    if y_to > width
        y_to = width;
    end
    x_range = [x_from x_to];
    y_range = [y_from y_to];
    
end
function position = findTargetInRange(frame,avg_frame,range)
    frame = frame(range(1,1):range(1,2),range(1,3):range(1,4),1:3);
    [height width tun] = size(frame);
    red = frame(:,:,1);
    green = frame(:,:,2);
    diff_gr = green - red;
    [v,x] = max(diff_gr);
    [v,y] = max(v);
    position = [x(1,y),y];
    if diff_gr(position(1,1),position(1,2)) < 180/255
        position = [0,0];
    end
    if max(position) > 0
        position = position + [range(1,1),range(1,3)];
    end
end
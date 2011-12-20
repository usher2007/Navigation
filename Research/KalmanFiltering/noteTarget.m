function frame=noteTarget(img, pos, mark)
    [height,width,tun] = size(mark);
    x = pos(1,1);
    y = pos(1,2);
    img(x-10:x+height-11,y-10:y+width-11,1:tun) = img(x-10:x+height-11,y-10:y+width-11,1:tun).*mark;
    frame = img;
end
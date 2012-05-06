close all, clear all, clc
x = [1 1 1;2 0 1;2 1 1;0 2 1;1 3 1; 1 -2 -1;0 0 -1;1 0 -1;1 1 -1;0 2 -1];
w = [1,1,1];
flag = zeros(1,10);
index = 1;
res = 0;
for i=1:1000
    res = x(index,:)*w';
    res
    if res <= 0
        w = w + x(index,:);
    else
        index = index + 1;
        if index > 10
            index = 1
        end
        for j=1:10
            res2 = x(j,:)*w'
            if res2 > 0
                flag(1,j) = 1
            else
                flag(1,j) = -1
            end
        end
        if min(flag) > 0
            break;
        end
    end    
end

flag
w
i

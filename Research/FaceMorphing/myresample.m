function resampleres = myresample(qt,i,flag)
% find the resample value of i from qt according to flag
% flag = 0 resample along y
% flag = 1 resample along x
if flag == 1
    j = 1;
    while(qt(j,1)<i)
        j = j+1;
    end
    if(qt(j-1,1)==i)
        resampleres = qt(j-1,2);
    else
        x1 = qt(j-1,1);
        y1 = qt(j-1,2);
        x2 = qt(j,1);
        y2 = qt(j,2);
        resampleres = y1+(y2-y1)*(i-x1)/(x2-x1);
    end
else
    j = 1;
    while(qt(j,2)<i)
        j = j+1;
    end
    if(qt(j-1,2)==i)
        resampleres = qt(j-1,1);
    else
        x1 = qt(j-1,2);
        y1 = qt(j-1,1);
        x2 = qt(j,2);
        y2 = qt(j,1);
        resampleres = y1+(y2-y1)*(i-x1)/(x2-x1);
    end
end
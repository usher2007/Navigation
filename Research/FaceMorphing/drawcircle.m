function[g] = drawcircle(f,pa,color)
%f:input image
%para:parameter of circle: a b r
[m n] = size(f);
g = f;
%if m<para(1) || n<para(2)
%    printf('error');
%end
a = pa(1);
b = pa(2);
r = pa(3);
for i=1:m
    for j=1:n
        if (i-a)^2+(j-b)^2 >= (r-1)^2 & (i-a)^2+(j-b)^2<=(r+1)^2
            g(i,j,:) = color;
        end
    end
end

        
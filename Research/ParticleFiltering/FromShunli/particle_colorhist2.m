function pu = particle_colorhist2(I,nbin)
% 直方图分布形式不同
I = double((I));
Tempt1 = I(:,:,1);
Tempt2 = I(:,:,2);
Tempt3 = I(:,:,3);
[Ty Tx] = size(Tempt1);
%  Tempt = rgb2hsv(Tempt);
n1 = nbin(1);
n2 = nbin(2);
n3 = nbin(3);

wa = sqrt(Tx^2/4+Ty^2/4);
inv1 = 256/n1;
inv2 = 256/n2;
inv3 = 256/n3;

p = zeros(1,n1*n2*n3);

% 核函数：1）k(r)=1-r^2  r<1; =0, otherwise


for i=1:Ty
    for j=1:Tx
        ind_p1 = floor(Tempt1(i,j)/inv1);
        ind_p2 = floor(Tempt2(i,j)/inv2);
        ind_p3 = floor(Tempt3(i,j)/inv3);
        
        ind = ind_p1 + ind_p2*n1 + ind_p3*n1*n2 + 1;
        
%         [ind_p1 ind_p2 ind_p3 ind]
        
        kr = 1 - ((i-Ty/2)^2+(j-Tx/2)^2)/wa^2; 
                  
        p(ind) = p(ind) + kr; 
    end
end


pu = p/sum(p);
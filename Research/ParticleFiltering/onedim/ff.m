function ss=ff(xc,h,sample)%x=-5:0.1:5;ss=ff(x,0.5,samples(:,11)); plot(x,ss);
[m,k]=size(xc);
sample=sample';
for i=1:k,
ss(i)=1/(500*h)*sum(inv((2*pi)^.5)*exp(-.5*((xc(1,i)-sample')*inv(h)).^2));
end

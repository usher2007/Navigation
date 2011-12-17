function [x,Pk] = kalmanFiltering(A,P,Q,H,R,z,x_p)
    xk_p = A*x_p;
    Pk_p = A*P*A' + Q;
    Kk = Pk_p*H'*(H*Pk_p*H' + R);
    x = xk_p +Kk*(z-H*xk_p);
    Pk = Pk_p-Kk*H*Pk_p; 
end
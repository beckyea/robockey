<<<<<<< Updated upstream
<<<<<<< Updated upstream
function [x, y, abs_theta] = sees4(X1, Y1, X2, Y2, X3, Y3, X4, Y4)
xdiff = 535;
ydiff = 370;
X1n = X1 - xdiff; Y1n = Y1 - ydiff;
X2n = X2 - xdiff; Y2n = Y2 - ydiff;
X3n = X3 - xdiff; Y3n = Y3 - ydiff;
X4n = X4 - xdiff; Y4n = Y4 - ydiff;

=======
function [x, y, theta] = sees4(X1, Y1, X2, Y2, X3, Y3, X4, Y4)
thetaScale = 0.0005;
X1n = tan((X1 - 512)*thetaScale)*487; Y1n = tan((Y1 - 384)*thetaScale)*487;
X2n = tan((X2 - 512)*thetaScale)*487; Y2n = tan((Y2 - 384)*thetaScale)*487;
X3n = tan((X3 - 512)*thetaScale)*487; Y3n = tan((Y3 - 384)*thetaScale)*487;
X4n = tan((X4 - 512)*thetaScale)*487; Y4n = tan((Y4 - 384)*thetaScale)*487;

 
>>>>>>> Stashed changes
=======
function [x, y, theta] = sees4(X1, Y1, X2, Y2, X3, Y3, X4, Y4)
thetaScale = 0.0005;
X1n = tan((X1 - 512)*thetaScale)*487; Y1n = tan((Y1 - 384)*thetaScale)*487;
X2n = tan((X2 - 512)*thetaScale)*487; Y2n = tan((Y2 - 384)*thetaScale)*487;
X3n = tan((X3 - 512)*thetaScale)*487; Y3n = tan((Y3 - 384)*thetaScale)*487;
X4n = tan((X4 - 512)*thetaScale)*487; Y4n = tan((Y4 - 384)*thetaScale)*487;

 
>>>>>>> Stashed changes
d12 = (X2n - X1n)^2 + (Y2n - Y1n)^2;
d13 = (X3n - X1n)^2 + (Y3n - Y1n)^2;
d14 = (X4n - X1n)^2 + (Y4n - Y1n)^2;
d23 = (X3n - X2n)^2 + (Y3n - Y2n)^2;
d24 = (X4n - X2n)^2 + (Y4n - Y2n)^2;
d34 = (X4n - X3n)^2 + (Y4n - Y3n)^2;

maxDist = max (max (max(d12, d13), max(d14, d23)), max(d24, d34));
minDist = min (min (min(d12, d13), min(d14, d23)), min(d24, d34));
 
Bx = 0; By = 0; Dx = 0; Dy = 0;
% check largest distance values to determine B and D, find center
if (maxDist == d12)
    if (minDist == d13 || minDist == d14)
        Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n;  disp('A1');
    else
        Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n;  disp('A2');
    end
elseif (maxDist == d13)
    if (minDist == d12 || minDist == d14)
        Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n;  disp('A3');
    else
        Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n;  disp('A4');
    end
elseif (maxDist == d14)
    if (minDist == d13 || minDist == d12)
        Bx = X1n; By = Y1n; Dx = X4n; Dy = Y4n;  disp('A5');
    else
        Dx = X1n; Dy = Y1n; Bx = X4n; By = Y4n;  disp('A6');
    end
elseif (maxDist == d23)
    if (minDist == d12 || minDist == d24)
        Bx = X2n; By = Y2n; Dx = X3n; Dy = Y3n;  disp('A7');
    else
        Dx = X2n; Dy = Y2n; Bx = X3n; By = Y3n;  disp('A8');
    end
elseif (maxDist == d24)
    if (minDist == d12 || minDist == d23)
        Bx = X2n; By = Y2n; Dx = X4n; Dy = Y4n;  disp('A9');
    else
        Dx = X2n; Dy = Y2n; Bx = X4n; By = Y4n;  disp('A10');
    end
elseif (maxDist == d34)
    if (minDist == d13 || minDist == d23)
        Bx = X3n; By = Y3n; Dx = X4n; Dy = Y4n;  disp('A11');
    else
        Dx = X3n; Dy = Y3n; Bx = X4n; By = Y4n;  disp('A12');
    end
end
<<<<<<< Updated upstream
<<<<<<< Updated upstream
if (Bx == 0 && By == 0 && Dx == 0 && Dy == 0)
    disp('Cannot sense stars.');
end
pi = 3.14159;
center_x = (Bx + Dx) / 2;
center_y = (By + Dy) / 2;
%disp([Bx By Dx Dy])
%disp([d12 d13 d14 23 d24 d34])
theta = atan2((Dy - By),(Dx - Bx));
theta = theta - pi/2;
if theta < -pi
    theta = theta + pi * 2;
end
theta2 = -atan2(center_x, center_y);
theta2 = theta2 - pi/2;
if theta2 < - pi
    theta2 = theta2 + pi * 2;
end
dist2cent = sqrt(center_x*center_x + center_y*center_y);
abs_theta =  theta - theta2 - pi;

x = dist2cent*cos(abs_theta);
y = dist2cent*sin(abs_theta);
disp([center_x center_y]);
%[x y]
end
=======
=======
>>>>>>> Stashed changes
%[Bx By Dx Dy]
center_x = (Bx + Dx) / 2;
center_y = (By + Dy) / 2;
theta = atan2((Bx - Dx), (By - Dy));
x = -(center_x * cos(theta) - center_y * sin(theta));
y = -(center_x * sin(theta) + center_y * cos(theta));
Bx_alt = x + (Bx * cos(theta) - By * sin(theta));
By_alt = y + (Bx * sin(theta) + By * cos(theta));
Dx_alt = x + (Dx * cos(theta) - Dy * sin(theta));
Dy_alt = y + (Dx * sin(theta) + Dy * cos(theta));
<<<<<<< Updated upstream

plot(Bx_alt + 230/2, By_alt + 120/2, 'o', 'MarkerFaceColor', 'k','MarkerSize', 5);
plot(Dx_alt + 230/2, Dy_alt + 120/2, 'o', 'MarkerFaceColor', 'k','MarkerSize', 5);

end
>>>>>>> Stashed changes
=======

plot(Bx_alt + 230/2, By_alt + 120/2, 'o', 'MarkerFaceColor', 'k','MarkerSize', 5);
plot(Dx_alt + 230/2, Dy_alt + 120/2, 'o', 'MarkerFaceColor', 'k','MarkerSize', 5);

end
>>>>>>> Stashed changes

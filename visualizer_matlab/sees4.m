function [x, y, theta] = sees4(X1, Y1, X2, Y2, X3, Y3, X4, Y4)
xdiff = 535;
ydiff = 370;
X1n = X1 - xdiff; Y1n = Y1 - ydiff;
X2n = X2 - xdiff; Y2n = Y2 - ydiff;
X3n = X3 - xdiff; Y3n = Y3 - ydiff;
X4n = X4 - xdiff; Y4n = Y4 - ydiff;

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
        Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n; disp('a');
    else
        Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n; disp('b');
    end
elseif (maxDist == d13)
    if (minDist == d12 || minDist == d14)
        Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n; disp('c');
    else
        Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n; disp('d');
    end
elseif (maxDist == d14)
    if (minDist == d13 || minDist == d12)
        Bx = X1n; By = Y1n; Dx = X4n; Dy = Y4n; disp('e');
    else
        Dx = X1n; Dy = Y1n; Bx = X4n; By = Y4n; disp('f');
    end
elseif (maxDist == d23)
    if (minDist == d12 || minDist == d24)
        Bx = X2n; By = Y2n; Dx = X3n; Dy = Y3n; disp('g');
    else
        Dx = X2n; Dy = Y2n; Bx = X3n; By = Y3n; disp('h');
    end
elseif (maxDist == d24)
    if (minDist == d12 || minDist == d23)
        Bx = X2n; By = Y2n; Dx = X4n; Dy = Y4n; disp('i');
    else
        Dx = X2n; Dy = Y2n; Bx = X4n; By = Y4n; disp('j');
    end
elseif (maxDist == d34)
    if (minDist == d13 || minDist == d23)
        Bx = X3n; By = Y3n; Dx = X4n; Dy = Y4n; disp('k');
    else
        Dx = X3n; Dy = Y3n; Bx = X4n; By = Y4n; disp('l');
    end
end
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
%disp([theta abs_theta]);
x = -dist2cent*cos(abs_theta);
y = -dist2cent*sin(abs_theta);
disp([dist2cent abs_theta])
end
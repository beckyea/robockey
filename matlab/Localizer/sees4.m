function [x, y, theta] = sees4(X1, Y1, X2, Y2, X3, Y3, X4, Y4)

X1n = X1 - 512; Y1n = Y1 - 384;
X2n = X2 - 512; Y2n = Y2 - 384;
X3n = X3 - 512; Y3n = Y3 - 384;
X4n = X4 - 512; Y4n = Y4 - 384;

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
        Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n;
    else
        Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n;
    end
elseif (maxDist == d13)
    if (minDist == d12 || minDist == d14)
        Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n;
    else
        Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n;
    end
elseif (maxDist == d14)
    if (minDist == d13 || minDist == d12)
        Bx = X1n; By = Y1n; Dx = X4n; Dy = Y4n;
    else
        Dx = X1n; Dy = Y1n; Bx = X4n; By = Y4n;
    end
elseif (maxDist == d23)
    if (minDist == d12 || minDist == d24)
        Bx = X2n; By = Y2n; Dx = X3n; Dy = Y3n;
    else
        Dx = X2n; Dy = Y2n; Bx = X3n; By = Y3n;
    end
elseif (maxDist == d24)
    if (minDist == d12 || minDist == d23)
        Bx = X2n; By = Y2n; Dx = X4n; Dy = Y4n;
    else
        Dx = X2n; Dy = Y2n; Bx = X4n; By = Y4n;
    end
elseif (maxDist == d34)
    if (minDist == d13 || minDist == d23)
        Bx = X3n; By = Y3n; Dx = X4n; Dy = Y4n;
    else
        Dx = X3n; Dy = Y3n; Bx = X4n; By = Y4n;
    end
end
if (Bx == 0 && By == 0 && Dx == 0 && Dy == 0)
    disp('Cannot sense stars.');
end
center_x = (Bx + Dx) / 2;
center_y = (By + Dy) / 2;
theta = atan2((Bx - Dx),(By - Dy));
x = -(center_x * cos(theta) - center_y * sin(theta));
y = (center_x * sin(theta) + center_y * cos(theta));

end
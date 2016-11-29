function [x, y, theta] = sees3(X1, Y1, X2, Y2, X3, Y3)

<<<<<<< Updated upstream
xdiff = 535;
ydiff = 370;
X1n = X1 - xdiff; Y1n = Y1 - ydiff;
X2n = X2 - xdiff; Y2n = Y2 - ydiff;
X3n = X3 - xdiff; Y3n = Y3 - ydiff;
=======
% X1n = X1 - 512; Y1n = Y1 - 384;
% X2n = X2 - 512; Y2n = Y2 - 384;
% X3n = X3 - 512; Y3n = Y3 - 384;
thetaScale = 0.0005;
X1n = tan((X1 - 512)*thetaScale)*487; Y1n = tan((Y1 - 384)*thetaScale)*487;
X2n = tan((X2 - 512)*thetaScale)*487; Y2n = tan((Y2 - 384)*thetaScale)*487;
X3n = tan((X3 - 512)*thetaScale)*487; Y3n = tan((Y3 - 384)*thetaScale)*487;
>>>>>>> Stashed changes

d12 = (X2n - X1n)^2 + (Y2n - Y1n)^2;
d13 = (X3n - X1n)^2 + (Y3n - Y1n)^2;
d23 = (X3n - X2n)^2 + (Y3n - Y2n)^2;

maxDist = max (max(d12, d13), d23);
minDist = min (min(d12, d13), d23);
<<<<<<< Updated upstream
ratio = maxDist / minDist;
disp(ratio)
=======
ratio = sqrt(maxDist) / sqrt(minDist);
>>>>>>> Stashed changes
if (ratio < 2.0)
    %Case in which B is missing: Max is CD, Min is AD
    if (maxDist == d12)
        if (minDist == d13)
            Dx = X1n; Dy = Y1n; Cx = X2n; Cy = Y2n; Ax = X3n; Ay = Yn3; disp('1A');
        else
            Dx = X2n; Dy = Y2n; Cx = X1n; Cy = Y1n; Ax = X3n; Ay = Yn3; disp('1B');
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Dx = X1n; Dy = Y1n; Cx = X3n; Cy = Y3n; Ax = X2n; Ay = Y2n; disp('2A');
        else
            Dx = X3n; Dy = Y3n; Cx = X1n; Cy = Y1n; Ax = X2n; Ay = Y2n; disp('2B');
        end
    elseif (maxDist == d23)
        if (minDist == 12)
            Dx = X2n; Dy = Y2n; Cx = X3n; Cy = Y3n; Ax = X1n; Ay = Y1n; disp('3A');
        else
            Dx = X3n; Dy = Y3n; Cx = X2n; Cy = Y2n; Ax = X1n; Ay = Y1n; disp('3B');
        end
    end
    Mx = Cx + (Cx - Ax) * 0.5245;
    My = Cy + (Cy - Ay) * 0.5245;
    DtoMx = Dx - Mx; DtoMy = Dy - My;
    center_x = Mx - DtoMx * 0.273;
    center_y = My - DtoMy * 0.273;
<<<<<<< Updated upstream
    theta = atan2((Dy - center_y),(Dx - center_x));
=======
    theta = atan2((center_x - Dx), (center_y - Dy));
    x = -(center_x * cos(theta) - center_y * sin(theta));
    y = -(center_x * sin(theta) + center_y * cos(theta));
>>>>>>> Stashed changes
elseif (ratio > 4.0)
    % Case in which A is missing: Max is BD, Min is BC
    if (maxDist == d12)
        if (minDist == d13)
<<<<<<< Updated upstream
            Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n; disp('1a');
        else
            Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n; disp('1b');
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n; disp('2a');
        else
            Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n; disp('2b');
        end
    elseif (maxDist == d23)
        if (minDist == d13)
            Bx = X3n; By = Y3n; Dx = X2n; Dy = Y2n; disp('3a');
        else
            Dx = X3n; Dy = Y3n; Bx = X2n; By = Y2n; disp('3b');
=======
            Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n;  disp('1a');
        else
            Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n;  disp('1b');
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n;  disp('2a');
        else
            Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n;  disp('2b');
        end
    elseif (maxDist == d23)
        if (minDist == d13)
            Bx = X3n; By = Y3n; Dx = X2n; Dy = Y2n;  disp('3a');
        else
            Dx = X3n; Dy = Y3n; Bx = X2n; By = Y2n;  disp('3b');
>>>>>>> Stashed changes
        end
    end
    center_x = (Bx + Dx) / 2;
    center_y = (By + Dy) / 2;
<<<<<<< Updated upstream
    theta = atan2((Dy - By),(Dx - Bx));
elseif (maxDist^2/minDist < 2200)
=======
    theta = atan2((Bx - Dx), (By - Dy));
    x = (center_x * cos(theta) - center_y * sin(theta));
    y = (center_x * sin(theta) + center_y * cos(theta));
elseif (ratio < 3.2)
>>>>>>> Stashed changes
    % Case in which D is missing: Max is AC, Min is BC
    if (maxDist == d12)
        if (minDist == d13)
            Cx = X1n; Cy = Y1n; Ax = X2n; Ay = Y2n; Bx = X3n; By = Y3n; disp('4A');
        else
            Cx = X2n; Cy = Y2n; Ax = X1n; Ay = Y1n; Bx = X3n; By = Y3n; disp('4B');
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Cx = X1n; Cy = Y1n; Ax = X3n; Ay = Y3n; Bx = X2n; By = Y2n; disp('5A');
        else
            Cx = X3n; Cy = Y3n; Ax = X1n; Ay = Y1n; Bx = X2n; By = Y2n; disp('5B');
        end
    elseif (maxDist == d23)
        if (minDist == d12)
            Cx = X2n; Cy = Y2n; Ax = X3n; Ay = Y3n; Bx = X1n; By = Y1n; disp('6A');
        else
            Cx = X3n; Cy = Y3n; Ax = X2n; Ay = Y2n; Bx = X1n; By = Y1n; disp('6B');
        end
    end
    Mx = Cx + (Cx - Ax) * 0.5246;
    My = Cy + (Cy - Ay) * 0.5246;
    BtoMx = Bx - Mx; BtoMy = By - My;
    center_x = Bx + BtoMx * 1.604;
    center_y = By + BtoMy * 1.604;
<<<<<<< Updated upstream
    theta = atan2((center_y - By),(center_x - Bx));
=======
    theta = atan2((Bx - center_x), (By - center_y));
    x = center_x * cos(theta) - center_y * sin(theta);
    y = (center_x * sin(theta) + center_y * cos(theta));
>>>>>>> Stashed changes
else
    %Case in which C is missing: Max is BD, Min is AB
    if (maxDist == d12)
        if (minDist == d13)
<<<<<<< Updated upstream
            Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n; disp('4a');
        else
            Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n; disp('4b');
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n; disp('5a');
        else
            Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n; disp('5b');
        end
    elseif (maxDist == d23)
        if (minDist == d13)
            Bx = X3n; By = Y3n; Dx = X2n; Dy = Y2n; disp('6a');
        else
            Dx = X3n; Dy = Y3n; Bx = X2n; By = Y2n; disp('6b');
=======
            Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n;  disp('4a');
        else
            Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n;  disp('4b');
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n;  disp('5a');
        else
            Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n;  disp('5b');
        end
    elseif (maxDist == d23)
        if (minDist == d13)
            Bx = X3n; By = Y3n; Dx = X2n; Dy = Y2n;  disp('6a');
        else
            Dx = X3n; Dy = Y3n; Bx = X2n; By = Y2n;  disp('6b');
>>>>>>> Stashed changes
        end
    end
    center_x = (Bx + Dx) / 2;
    center_y = (By + Dy) / 2;
<<<<<<< Updated upstream
    theta = atan2((Dy - By),(Dx - Bx));
end
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
%disp([X1n Y1n X2n Y2n X3n Y3n]);
=======
    theta = atan2((Bx - Dx), (By - Dy));
    x = (center_x * cos(theta) - center_y * sin(theta));
    y = (center_x * sin(theta) + center_y * cos(theta));
end
Bx_alt = x + (center_x * cos(theta) - center_y * sin(theta));
By_alt = y + (center_x * sin(theta) + center_y * cos(theta));
Dx_alt = x + (Dx * cos(theta) - Dy * sin(theta));
Dy_alt = y + (Dx * sin(theta) + Dy * cos(theta));

plot(Bx_alt + 230/2, By_alt + 120/2, 'o', 'MarkerFaceColor', 'k','MarkerSize', 5);
plot(Dx_alt + 230/2, Dy_alt + 120/2, 'o', 'MarkerFaceColor', 'k','MarkerSize', 5);
>>>>>>> Stashed changes
end
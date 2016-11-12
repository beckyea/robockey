function [x, y, theta] = sees3(X1, Y1, X2, Y2, X3, Y3)

X1n = X1 - 512; Y1n = Y1 - 384;
X2n = X2 - 512; Y2n = Y2 - 384;
X3n = X3 - 512; Y3n = Y3 - 384;

d12 = (X2n - X1n)^2 + (Y2n - Y1n)^2;
d13 = (X3n - X1n)^2 + (Y3n - Y1n)^2;
d23 = (X3n - X2n)^2 + (Y3n - Y2n)^2;

maxDist = max (max(d12, d13), d23);
minDist = min (min(d12, d13), d23);
ratio = maxDist / minDist;
if (ratio < 2.0)
    %Case in which B is missing: Max is CD, Min is AD
    if (maxDist == d12)
        if (minDist == d13)
            Dx = X1n; Dy = Y1n; Cx = X2n; Cy = Y2n; Ax = X3n; Ay = Yn3;
        else
            Dx = X2; Dy = Y2; Cx = X1; Cy = Y1; Ax = X3; Ay = Y3;
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Dx = X1; Dy = Y1; Cx = X3; Cy = Y3; Ax = X2; Ay = Y2;
        else
            Dx = X3; Dy = Y3; Cx = X1; Cy = Y1; Ax = X2; Ay = Y2;
        end
    elseif (maxDist == d23)
        if (minDist == 12)
            Dx = X2; Dy = Y2; Cx = X3; Cy = Y3; Ax = X1; Ay = Y1;
        else
            Dx = X3; Dy = Y3; Cx = X2; Cy = Y2; Ax = X1; Ay = Y1;
        end
    end
    Mx = Cx + (Cx - Ax) * 0.5246;
    My = Cy + (Cy - Ay) * 0.5246;
    DtoMx = Dx - Mx; DtoMy = Dy - My;
    center_x = Mx - DtoMx * 0.191;
    center_y = My - DtoMy * 0.191;
    theta = atan2((center_x - Dx),(center_y - Dy)); % - pi/2;
    x = (center_x * cos(theta) - center_y * sin(theta));
    y = (center_x * sin(theta) + center_y * cos(theta));
    x = 0;
    y = 0;
elseif (ratio > 4.0)
    % Case in which A is missing: Max is BD, Min is BC
    if (maxDist == d12)
        if (minDist == d13)
            Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n;
        else
            Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n;
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n;
        else
            Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n;
        end
    elseif (maxDist == d23)
        if (minDist == d13)
            Bx = X3n; By = Y3n; Dx = X2n; Dy = Y2n;
        else
            Dx = X3n; Dy = Y3n; Bx = X2n; By = Y2n;
        end
    end
    center_x = (Bx + Dx) / 2;
    center_y = (By + Dy) / 2;
    theta = atan2((Bx - Dx),(By - Dy)) - pi/2;
    x = -(center_x * cos(theta) - center_y * sin(theta));
    y = (center_x * sin(theta) + center_y * cos(theta));
elseif (ratio < 3.2)
    % Case in which D is missing: Max is AC, Min is BC
    if (maxDist == d12)
        if (minDist == d13)
            Cx = X1; Cy = Y1; Ax = X2; Ay = Y2; Bx = X3; By = Y3;
        else
            Cx = X2; Cy = Y2; Ax = X1; Ay = Y1; Bx = X3; By = Y3;
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Cx = X1; Cy = Y1; Ax = X3; Ay = Y3; Bx = X2; By = Y2;
        else
            Cx = X3; Cy = Y3; Ax = X1; Ay = Y1; Bx = X2; By = Y2;
        end
    elseif (maxDist == d23)
        if (minDist == 12)
            Cx = X2; Cy = Y2; Ax = X3; Ay = Y3; Bx = X1; By = Y1;
        else
            Cx = X3; Cy = Y3; Ax = X2; Ay = Y2; Bx = X1; By = Y1;
        end
    end
    Mx = Cx + (Cx - Ax) * 0.5246;
    My = Cy + (Cy - Ay) * 0.5246;
    BtoMx = Bx - Mx; BtoMy = By - My;
    center_x = Bx - BtoMx * 0.761;
    center_y = By - BtoMy * 0.761;
    theta = atan2((Bx - center_x),(By - center_y)) - pi/2;
    x = (center_x * cos(theta) - center_y * sin(theta));
    y = (center_x * sin(theta) + center_y * cos(theta));
    x = 0;
    y = 0;
else
    %Case in which C is missing: Max is BD, Min is AC
    if (maxDist == d12)
        if (minDist == d13)
            Bx = X1n; By = Y1n; Dx = X2n; Dy = Y2n;
        else
            Dx = X1n; Dy = Y1n; Bx = X2n; By = Y2n;
        end
    elseif (maxDist == d13)
        if (minDist == d12)
            Bx = X1n; By = Y1n; Dx = X3n; Dy = Y3n;
        else
            Dx = X1n; Dy = Y1n; Bx = X3n; By = Y3n;
        end
    elseif (maxDist == d23)
        if (minDist == d13)
            Bx = X3n; By = Y3n; Dx = X2n; Dy = Y2n;
        else
            Dx = X3n; Dy = Y3n; Bx = X2n; By = Y2n;
        end
    end
    center_x = (Bx + Dx) / 2;
    center_y = (By + Dy) / 2;
    theta = atan2((Bx - Dx),(By - Dy)); % - pi/2;
    x = -(center_x * cos(theta) - center_y * sin(theta));
    y = (center_x * sin(theta) + center_y * cos(theta));
end
end
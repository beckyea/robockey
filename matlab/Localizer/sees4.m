function [x, y, theta] = sees4(X1, Y1, X2, Y2, X3, Y3, X4, Y4)

    d12 = (X2 - X1)^2 + (Y2 - Y1)^2;
    d13 = (X3 - X1)^2 + (Y3 - Y1)^2;
    d14 = (X4 - X1)^2 + (Y4 - Y1)^2;
	d23 = (X3 - X2)^2 + (Y3 - Y2)^2;
	d24 = (X4 - X2)^2 + (Y4 - Y2)^2;
	d34 = (X4 - X3)^2 + (Y4 - Y3)^2;
	maxDist = max (max (max(d12, d13), max(d14, d23)), max(d24, d34));
	minDist = min (min (min(d12, d13), min(d14, d23)), min(d24, d34));

    Bx = 0; By = 0; Dx = 0; Dy = 0;
	% check largest distance values to determine B and D, find center
	if (maxDist == d12)
		if (minDist == d13 || minDist == d14)
			Bx = X1; By = Y1; Dx = X2; Dy = Y2;
        else
            Dx = X1; Dy = Y1; Bx = X2; By = Y2;
        end
    elseif (maxDist == d13)
		if (minDist == d12 || minDist == d14)
			Bx = X1; By = Y1; Dx = X3; Dy = Y3;
        else
            Dx = X1; Dy = Y1; Bx = X3; By = Y3;
        end
    elseif (maxDist == d14)
		if (minDist == d13 || minDist == d12)
			Bx = X1; By = Y1; Dx = X4; Dy = Y4;
        else
            Dx = X1; Dy = Y1; Bx = X4; By = Y4;
        end
    elseif (maxDist == d23)
		if (minDist == d12 || minDist == d24)
			Bx = X2; By = Y2; Dx = X3; Dy = Y3;
        else
            Dx = X2; Dy = Y2; Bx = X3; By = Y3;
        end
    elseif (maxDist == d24)
		if (minDist == d12 || minDist == d23)
			Bx = X2; By = Y2; Dx = X4; Dy = Y4;
        else
            Dx = X2; Dy = Y2; Bx = X4; By = Y4;
        end
    elseif (maxDist == d34)
		if (minDist == d13 || minDist == d23)
			Bx = X3; By = Y3; Dx = X4; Dy = Y4;
        else
            Dx = X3; Dy = Y3; Bx = X4; By = Y4;
        end  
    end
    if (Bx == 0 && By == 0 && Dx == 0 && Dy == 0) 
        disp('Cannot sense stars.');
    end
	center_x = (Bx + Dx) / 2;
	center_y = (By + Dy) / 2;
	theta = atan2((Bx - Dx),(By - Dy));
	x = (center_x * cos(theta) - center_y * sin(theta));
	y = (center_x * sin(theta) + center_y * cos(theta));
end
function [x, y, theta] = sees3(X1, Y1, X2, Y2, X3, Y3)
	d12 = (X2 - X1)^2 + (Y2 - Y1)^2;
	d13 = (X3 - X1)^2 + (Y3 - Y1)^2;
	d23 = (X3 - X2)^2 + (Y3 - Y2)^2;
	maxDist = max (max(d12, d13), d23);
	minDist = min (min(d12, d13), d23);
	ratio = maxDist / minDist;
	if (ratio < 2.0) 
		%Case in which B is missing: Max is CD, Min is AD
        x = 0;
        y = 0;
        theta = 0;
    elseif (ratio > 4.0)
		% Case in which A is missing: Max is BD, Min is BC
		if (maxDist == d12)
			if (minDist == d13)
				Bx = X1; By = Y1; Dx = X2; Dy = Y2; 
            else
                Dx = X1; Dy = Y1; Bx = X2; By = Y2;
            end
		elseif (maxDist == d13)
			if (minDist == d12)
				Bx = X1; By = Y1; Dx = X3; Dy = Y3;
            else
                Dx = X1; Dy = Y1; Bx = X3; By = Y3;
            end
        elseif (maxDist == d23)
			if (minDist == d13)
				Bx = X3; By = Y3; Dx = X2; Dy = Y2;
            else
                Dx = X3; Dy = Y3; Bx = X2; By = Y2;
            end
        end
		center_x = (Bx + Dx) / 2; 
		center_y = (By + Dy) / 2;
		theta = atan2((Bx - Dx),(By - Dy));
        x = (center_x * cos(theta) - center_y * sin(theta));
        y = (center_x * sin(theta) + center_y * cos(theta));
    elseif (ratio < 3.2)
		% Case in which D is missing: Max is AC, Min is BC
        x = 0;
        y = 0;
        theta = 0;
	else 
		%Case in which C is missing: Max is BD, Min is AC
		if (maxDist == d12)
			if (minDist == d13)
				Bx = X1; By = Y1; Dx = X2; Dy = Y2; 
            else
                Dx = X1; Dy = Y1; Bx = X2; By = Y2;
            end
        elseif (maxDist == d13)
			if (minDist == d12)
				Bx = X1; By = Y1; Dx = X3; Dy = Y3; 
            else
                Dx = X1; Dy = Y1; Bx = X3; By = Y3;
            end
        elseif (maxDist == d23)
			if (minDist == d13)
				Bx = X3; By = Y3; Dx = X2; Dy = Y2; 
            else
                Dx = X3; Dy = Y3; Bx = X2; By = Y2;
            end
        end
		center_x = (Bx + Dx) / 2; 
		center_y = (By + Dy) / 2;
		theta = atan2((Bx - Dx),(By - Dy));
        x = (center_x * cos(theta) - center_y * sin(theta));
        y = (center_x * sin(theta) + center_y * cos(theta));
    end
end
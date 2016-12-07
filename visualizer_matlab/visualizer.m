%% Variables for Shift
thetaShiftMult = 100;
xShift = -115;
yShift = -60;
scaleFactor = .31;

%% Configure Visualization
figure(1)
clf
hold on
axis equal
bot_r = 3.81; %bot radius in cm
% Draw Court
rectangle('Position',[0 0 230 120],'Curvature',[59/230, 59/120])
% Draw Goals
rectangle('Position',[-5 29.5 5 59.7])
rectangle('Position',[230 29.5 5 59.7])
drawnow

%% Define Bot initial positions, color
B1 = [0 0 0];
B2 = [0 0 0];
B3 = [0 0 0];

%% Initialize I/O
handle = serial('/dev/tty.usbmodem411','Baudrate', 9600, 'Parity','none');
%handle.InputBufferSize = 20;
fclose(instrfind);
buffer = zeros(4,1);
bufferindex = 0;
while true
    fopen(handle);
    num = str2num(fscanf(handle));
    for j = 1:length(num)
        if (num(j) == 5555)
            bufferindex = 1;
        elseif (bufferindex >= 1)
            buffer(bufferindex) = num(j);
            bufferindex = bufferindex + 1;
        end
        if (bufferindex == 4)
            x = buffer(1) + 230/2;
            y = buffer(2) + 120/2;
            t = buffer(3)/100 * 180/3.14159;
%             X1 = buffer(1); Y1 = buffer(2);
%             X2 = buffer(3); Y2 = buffer(4);
%             X3 = buffer(5); Y3 = buffer(6);
%             X4 = bufferD(7); Y4 = buffer(8);
%             xp = 0; %buffer(10); yp = buffer(11);
%             yp = 0;
%             tp = buffer(9);
%             if (X1 == 1023 && X2 ~= 1023 && X3 ~= 1023 && X4 ~= 1023)
%                 [x, y, t] = sees3(X2, Y2, X3, Y3, X4, Y4);
%             elseif (X1 ~= 1023 && X2 == 1023 && X3 ~= 1023 && X4 ~= 1023)
%                 [x, y, t] = sees3(X1, Y1, X3, Y3, X4, Y4);
%             elseif (X1 ~= 1023 && X2 ~= 1023 && X3 == 1023 && X4 ~= 1023)
%                 [x, y, t] = sees3(X1, Y1, X2, Y2, X4, Y4);
%             elseif (X1 ~= 1023 && X2 ~= 1023 && X3 ~= 1023 && X4 == 1023)
%                 [x, y, t] = sees3(X1, Y1, X2, Y2, X3, Y3);
%             elseif (X1 == 1023 || X2 == 1023 || X3 == 1023 || X4 == 1023)
%                 disp('2 or fewer stars');
%                 x = 0; y = 0;
%             else
%                 [x, y, t] = sees4(X1, Y1, X2, Y2, X3, Y3, X4, Y4);
%             end
%             x = x * scaleFactor + 230/2;
%             y = y * scaleFactor + 120/2;
            disp([x y t])
            plot(x, y, 'o', 'MarkerFaceColor', 'm' , 'MarkerSize', bot_r * 2);
            line([x, (-bot_r *sind(t) + x)], [y, (bot_r * cosd(t) + y)]);
            drawnow
        end
    end
    fclose(handle);
end
hold off
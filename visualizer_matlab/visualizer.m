%% Variables for Shift
thetaShiftMult = 100;
xShift = -115;
yShift = -60;

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
handle = serial('/dev/tty.usbmodem411','Baudrate', 9600, 'Parity','none','DataBits',8);
%handle.InputBufferSize = 20;
fclose(instrfind);
buffer = zeros(9,1);
bufferindex = 0;
while true
    fopen(handle);
    num = str2num(fscanf(handle));
    for j = 1:length(num)
        if (num(j) == 1000)
            bufferindex = 1;
        elseif (bufferindex >= 1)
            buffer(bufferindex) = num(j);
            bufferindex = bufferindex + 1;
        end
        if (bufferindex == 9)
            bufferindex = 0;
            %% Handle Input
            switch(buffer(1))
                case(1) % offensive bot 1
                    B1 = [ buffer(2), buffer(3), buffer(4) ];
                    x = B1(1) + xShift + 230/2;
                    y = B1(2) + yShift + 120/2;
                    t = B1(3) / thetaShiftMult;
                    plot(x, y, 'o', 'MarkerFaceColor', 'm' , 'MarkerSize', bot_r * 2);
                    %plot(x, y, 'o', 'MarkerFaceColor', 'm' , ...
                    %    'MarkerEdgeColor', 'm', 'MarkerSize', bot_r * 2);
                    line([x, (bot_r * cos(t) + x)], [y, (bot_r * sin(t) + y)]);
                case(2) % offensive bot 2
                    B2 = [ buffer(2), buffer(3), buffer(4) ];
                    x = B2(1) + xShift + 230/2;
                    y = B2(2) + yShift  + 120/2;
                    t = B2(3) / thetaShiftMult;
                    plot(x, y, 'o', 'MarkerFaceColor', 'c' , ...
                        'MarkerSize', bot_r * 2);
                    line([x, (bot_r * cos(t) + x)], [y, (bot_r * sin(t) + y)]);
                case(3) % goalie bot
                    B3 = [ buffer(2), buffer(3), buffer(4) ];
                    x = B3(1) + xShift + 230/2;
                    y = B3(2) + yShift + 120/2;
                    t = B3(3) / thetaShiftMult;
                    plot(x, y, 'o', 'MarkerFaceColor', 'g' , ...
                        'MarkerSize', bot_r * 2);
                    line([x, (bot_r * cos(t) + x)], [y, (bot_r * sin(t) + y)]);
            end
            drawnow
        end
    end
    fclose(handle);
end
hold off
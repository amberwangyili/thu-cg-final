clc;clear;
data = ["random" 1 0.126;
"random" 2 0.542;
"random" 5 1.102;
"random" 10 2.479;
"random" 20 4.097;
"random" 50 8.603;
"random" 100 15.769;
"random" 200 33.558;
"random" 400 61.997;
"random" 600 85.53;
"random" 800 112.107;
"random" 1000 143.769];
x = data(:,2);
y = data(:,3);
[xData, yData] = prepareCurveData( x, y );

% Set up fittype and options.
ft = fittype( 'poly1' );

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft );

data2 = ["circle" 1 14.944;
"circle" 2 16.657;
"circle" 5 18.268;
"circle" 10 22.456;
"circle" 20 27.708;
"circle" 50 37.328;
"circle" 100 41.65;
"circle" 200 53.897;
"circle" 400 73.713;
"circle" 600 97.302;
"circle" 800 114.426;
"circle" 1000 130.282];

x2 = data2(:,2);
y2 = data2(:,3);
[x2Data, y2Data] = prepareCurveData( x2, y2 );

% Set up fittype and options.
ft2 = fittype( 'poly1' );

% Fit model to data.
[fitresult2, gof] = fit( x2Data, y2Data, ft2 );

data3 = ["diagonal" 1 32.258;
"diagonal" 2 34.474;
"diagonal" 5 39.208;
"diagonal" 10 48.409;
"diagonal" 20 53.925;
"diagonal" 50 66.801;
"diagonal" 100 76.019;
"diagonal" 200 94.605;
"diagonal" 400 115.232;
"diagonal" 600 133.481;
"diagonal" 800 167.199;
"diagonal" 1000 172.157];

x3 = data3(:,2);
y3 = data3(:,3);
[x3Data, y3Data] = prepareCurveData( x3, y3 );

% Set up fittype and options.
ft3 = fittype( 'poly1' );

% Fit model to data.
[fitresult3, gof] = fit( x3Data, y3Data, ft3 );


figure(1);
f1 = plot( fitresult, xData, yData );
%legend( h, 'y vs. x', '$\mathcal{O}(n\log n)$',  'Location', 'NorthEast' );
hold on
f2 = plot(fitresult2, x2Data, y2Data);
hold on
set(f2(1),'color','r')
set(f2(2),'color','b')

f3 = plot(fitresult3, x3Data, y3Data);
set(f3(1),'color','k')
set(f3(2),'color','m')
%legend( h, 'y vs. x', '$\mathcal{O}(n\log n)$',  'Location', 'NorthEast' );
l = legend('random', 'random:$\mathcal{O}(k)$','circle', 'circle:$\mathcal{O}(k)$'...
    ,'diagonal', 'diagonal:$\mathcal{O}(k)$','Location','NorthWest');
set(l,'Interpreter','latex');
xlabel('k/points');
ylabel('time/us');
% Label axes

grid on



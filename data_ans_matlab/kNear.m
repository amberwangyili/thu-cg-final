clc;clear;
data = [1 0.126;
2 0.542;
5 1.102;
10 2.479;
20 4.097;
50 8.603;
100 15.769;
200 33.558;
400 61.997;
600 85.53;
800 112.107;
1000 143.769];
x = data(:,1);
y = data(:,2);
[xData, yData] = prepareCurveData( x, y );

% Set up fittype and options.
ft = fittype( 'poly1' );

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft );

data2 = [1 14.944;
2 16.657;
5 18.268;
10 22.456;
20 27.708;
50 37.328;
100 41.65;
200 53.897;
400 73.713;
600 97.302;
800 114.426;
1000 130.282];

x2 = data2(:,1);
y2 = data2(:,2);
[x2Data, y2Data] = prepareCurveData( x2, y2 );

% Set up fittype and options.
ft2 = fittype( 'poly1' );

% Fit model to data.
[fitresult2, gof] = fit( x2Data, y2Data, ft2 );

data3 = [1 32.258;
2 34.474;
5 39.208;
10 48.409;
20 53.925;
50 66.801;
100 76.019;
200 94.605;
400 115.232;
600 133.481;
800 167.199;
1000 172.157];

x3 = data3(:,1);
y3 = data3(:,2);
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



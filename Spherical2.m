clc;clear;
data = [25 0;
    50 0.108
100 0.418;
10000 6.977;
20000 12.748;
40000 28.227;
80000 60.829;
100000 88.098];

x = data(:,1);
y = data(:,2);
[xData, yData] = prepareCurveData( x, y );

% Set up fittype and options.
ft = fittype( 'poly1' );

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft);


data2 = [25 0;
50 0;
100 0;
10000 5.006;
20000 9.083;
40000 18.355;
80000 51.701;
100000 77.601];

x2 = data2(:,1);
y2 = data2(:,2);
[x2Data, y2Data] = prepareCurveData( x2, y2 );

% Set up fittype and options.
ft2 = fittype( 'poly1' );

% Fit model to data.
[fitresult2, gof] = fit( x2Data, y2Data, ft2 );


data3 = [25 0.014;
50 0.426;
100 1.237;
10000 72.563;
20000 71.327;
40000 158.002;
80000 348.711;
100000 431.176];

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
l = legend('random', 'random:$\mathcal{O}(n)$','circle', 'circle:$\mathcal{O}(n)$'...
    ,'diagonal', 'diagonal:$\mathcal{O}(n)$','Location','NorthWest');
set(l,'Interpreter','latex');
xlabel('n/points');
ylabel('time/us');
% Label axes

grid on
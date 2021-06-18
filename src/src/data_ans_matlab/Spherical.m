
clc;clear;
data = [0.05 0.595;
0.08 1.032;
0.1 1.189;
0.15 3.036;
0.2 5.019;
0.4 24.815;
0.5 35.989];
x = data(:,1);
y = data(:,2);
[xData, yData] = prepareCurveData( x, y );

% Set up fittype and options.
ft = fittype( 'poly2' );

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft );

data2 = [0.05 0;
0.08 0;
0.1 0.001;
0.15 0.001;
0.2 4.052;0.4 26.08;
0.5 39.729];

x2 = data2(:,1);
y2 = data2(:,2);
[x2Data, y2Data] = prepareCurveData( x2, y2 );

% Set up fittype and options.
ft2 = fittype( 'poly2' );

% Fit model to data.
[fitresult2, gof] = fit( x2Data, y2Data, ft2 );

data3 = [
0.05 6.382;
0.08 11.822;
0.1 11.112;
0.15 17.005;
0.2 26.915;
0.4 57.394;
0.5 66.45];

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
l = legend('random', 'random:$\mathcal{O}(r^2)$','circle', 'circle:$\mathcal{O}(r^2)$'...
    ,'diagonal', 'diagonal:$\mathcal{O}(r)$','Location','NorthWest');
set(l,'Interpreter','latex');
xlabel('r/(radius/min(width,height))');
ylabel('time/us');
% Label axes

grid on



clc;clear;
data = [25 2.799
50 3.103
100 2.547
10000 6.675
20000 6.632
40000 7.308
80000 8.185
100000 10.641];

x = data(:,1);
y = data(:,2);
[xData, yData] = prepareCurveData( x, y );

% Set up fittype and options.
ft = fittype( 'a*x^(0.5) + c', 'independent', 'x', 'dependent', 'y' );
opts = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts.Display = 'Off';
opts.Lower = [0 -Inf];

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft, opts );


data2 = [25 3.331;
50 2.323;
100 2.439;
10000 39.444;
20000 70.209;
40000 277.005;
80000 355.439;
100000 436.19];

x2 = data2(:,1);
y2 = data2(:,2);
[x2Data, y2Data] = prepareCurveData( x2, y2 );

% Set up fittype and options.
ft2 = fittype( 'a*x^(0.5) + c', 'independent', 'x', 'dependent', 'y' );
opts2 = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts2.Display = 'Off';
opts2.Lower = [0 -Inf];

% Fit model to data.
[fitresult2, gof] = fit( x2Data, y2Data, ft2, opts2 );


data3 = [25 1.01;
50 1.405;
100 1.913;
10000 50.804;
20000 105.85;
40000 209.342;
80000 583.803;
100000 923.039];

x3 = data3(:,1);
y3 = data3(:,2);
[x3Data, y3Data] = prepareCurveData( x3, y3 );

% Set up fittype and options.
ft3 = fittype( 'a*x^(0.5) + c', 'independent', 'x', 'dependent', 'y' );
opts3 = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts3.Display = 'Off';
opts3.Lower = [0 -Inf];

% Fit model to data.
[fitresult3, gof] = fit( x3Data, y3Data, ft3, opts3 );



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
l = legend('random', 'random:$\mathcal{O}(\sqrt{n})$','circle', 'circle:$\mathcal{O}(\sqrt{n})$'...
    ,'diagonal', 'diagonal:$\mathcal{O}(\sqrt{n})$','Location','NorthWest');
set(l,'Interpreter','latex');
xlabel('n/points');
ylabel('time/us');
% Label axes

grid on
clc,clear;
syms t
data = [0 0;
1 0;
5 1;
25 3;
50 8;
100 19;
10000 5494;
20000 11429;
40000 26944;
80000 62065;
100000 89771;
200000 238990;
400000 589308];

x = data(2:13,1);
y = data(2:13,2);


[xData, yData] = prepareCurveData( x, y );

% Set up fittype and options.
ft = fittype( 'a*x*log(b*x)+c', 'independent', 'x', 'dependent', 'y' );
opts = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts.Display = 'Off';
opts.Lower = [-Inf 0 -Inf];
opts.StartPoint = [0.585264091152724 0.54972360829114 0.91719366382981];

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft, opts );

% Plot fit with data.
data2 = [0 3;
1 0;
5 2;
25 5;
50 9;
100 20;
10000 2372;
20000 5395;
40000 13771;
80000 45230;
100000 49534;
200000 158761;
400000 411274];

x2 = data2(2:13,1);
y2 = data2(2:13,2);

[x2Data, y2Data] = prepareCurveData( x2, y2 );
ft2 = fittype( 'a*x*log(b*x)+c', 'independent', 'x', 'dependent', 'y' );
opts2 = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts2.Display = 'Off';
opts2.Lower = [-Inf 0 -Inf];
opts2.StartPoint = [0.331043853086389 0.357012428199026 0.340385579192823];

[fitresult2, gof] = fit( x2Data, y2Data, ft2, opts2);



data3 = [0 1;
1 0;
5 1;
25 3;
50 7;
100 13;
10000 2199;
20000 6306;
40000 10942;
80000 43981;
100000 53568;
200000 130291;
400000 325887];

x3 = data3(2:13,1);
y3 = data3(2:13,2);

[x3Data, y3Data] = prepareCurveData( x3, y3 );
ft3 = fittype( 'a*x*log(b*x)+c', 'independent', 'x', 'dependent', 'y' );
opts3 = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts3.Display = 'Off';
opts3.Lower = [-Inf 0 -Inf];
opts3.StartPoint = [0.196595250431208 0.251083857976031 0.616044676146639];

[fitresult3, gof] = fit( x3Data, y3Data, ft3, opts3);




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
l = legend('random', 'random:$\mathcal{O}(n\log n)$','circle', 'circle:$\mathcal{O}(n\log n)$'...
    ,'diagonal', 'diagonal:$\mathcal{O}(n\log n)$','Location','NorthWest');
set(l,'Interpreter','latex');
xlabel('n/points');
ylabel('time/us');
% Label axes

grid on




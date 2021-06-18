clc,clear;
syms t
data = [0 0;1 0;5 1;25 3;50 7;100 20;10000 5265;20000 12068;
    40000 29734;
    80000 73986;
    100000 93457;
    200000 242977;
    400000 998694;
    800000 4842686;
    1000000 14167902;
    1600000 24930752];
x = data(2:16,1);
y = data(2:16,2);

[xData, yData] = prepareCurveData( x, y );

% Set up fittype and options.
ft = fittype( 'a*x*log(b*x)+c', 'independent', 'x', 'dependent', 'y' );
opts = fitoptions( 'Method', 'NonlinearLeastSquares' );
opts.Display = 'Off';
opts.MaxIter = 4000;
opts.StartPoint = [0.251556538335812 0.253992327732941 0.519042247187466];

% Fit model to data.
[fitresult, gof] = fit( xData, yData, ft, opts );

% Plot fit with data.
figure( 'Name', 'untitled fit 1' );
h = plot( fitresult, xData, yData );
%legend( h, 'y vs. x', '$\mathcal{O}(n\log n)$',  'Location', 'NorthEast' );
l = legend('time vs $n$', '$\mathcal{O}(n\log n)$');
set(l,'Interpreter','latex');
xlabel('n/points');
ylabel('time/us');
% Label axes

grid on



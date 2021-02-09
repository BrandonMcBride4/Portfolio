% wind parameters
aerosonde_parameters
% steady state wind
WIND.wind_n = 3;
WIND.wind_e = 2;
WIND.wind_d = 0;

% dryden gust model parameters (chap4 slide 38)
% low altitude, light turbulence
WIND.L_u = 200;
WIND.L_v = WIND.L_u;
WIND.L_w = 50;

WIND.sigma_u = 1.06; 
WIND.sigma_v = WIND.sigma_u;
WIND.sigma_w = 0.7;

WIND.Va0 = norm([MAV.u0;MAV.v0;MAV.w0]);

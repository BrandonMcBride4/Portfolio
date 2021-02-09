addpath('../parameters')
aerosonde_parameters
wind_parameters
% compute trim conditions using 'mavsim_chap5_trim.slx'
TRIM.gamma = 0*pi/180;  % desired flight path angle (radians)
TRIM.R     = Inf;    % desired radius (m) - use (+) for right handed orbit, 
                             %                  (-) for left handed orbit
TRIM.Va = WIND.Va0;


% set initial conditions
TRIM.x0 = [
            0;...
            0;...
            0;...
            TRIM.Va;...
            0;...
            0;...
            0;...
            TRIM.gamma;...
            MAV.psi0;...
            0;...
            0;...
            0;...
          ];
% specify which states to hold equal to the initial conditions
TRIM.ix = [];

% specify initial inputs 
TRIM.u0 = [...
    0;... % 1 - delta_e
    0;... % 2 - delta_a
    0;... % 3 - delta_r
    0.5;... % 4 - delta_t
    ];
% specify which inputs to hold constant
TRIM.iu = [3];

% define constant outputs
TRIM.y0 = [...
    TRIM.Va;...       % 1 - Va
    TRIM.gamma;...    % 2 - alpha
    0;...             % 3 - beta
    ];
% specify which outputs to hold constant
TRIM.iy = [1,3];

% define constant derivatives
if TRIM.R~=Inf
    TRIM.psidot = TRIM.Va*cos(TRIM.gamma)/TRIM.R;
else
    TRIM.psidot = 0;
end  % 9 - TRIM.psidot
TRIM.dx0 = [...
        0;...
        0;...
        -TRIM.Va*sin(TRIM.gamma);...
        0;...
        0;...
        0;...
        0;...
        0;...
        TRIM.psidot;...
        0;...
        0;...
        0;...
     ];

% specify which derivaties to hold constant in trim algorithm
TRIM.idx = [3;4;5;6;7;8;9;10;11;12];
% compute trim conditions
[TRIM.x_trim, TRIM.u_trim, TRIM.y_trim, TRIM.dx_trim] = trim('mavsim_trim',...
                                                          TRIM.x0,...
                                                          TRIM.u0,...
                                                          TRIM.y0,...
                                                          TRIM.ix,...
                                                          TRIM.iu,...
                                                          TRIM.iy,...
                                                          TRIM.dx0,...
                                                          TRIM.idx);
% check to make sure that the linearization worked (should be small)
norm(TRIM.dx_trim(3:end)-TRIM.dx0(3:end));

% set initial conditions to trim conditions
% initial conditions
MAV.u0     = TRIM.x_trim(4);   % initial velocity along body x-axis
MAV.v0     = TRIM.x_trim(5);   % initial velocity along body y-axis
MAV.w0     = TRIM.x_trim(6);   % initial velocity along body z-axis
MAV.phi0   = TRIM.x_trim(7);   % initial roll angle
MAV.theta0 = TRIM.x_trim(8);   % initial pitch angle
MAV.psi0   = TRIM.x_trim(9);   % initial yaw angle
MAV.p0     = TRIM.x_trim(10);  % initial body frame roll rate
MAV.q0     = TRIM.x_trim(11);  % initial body frame pitch rate
MAV.r0     = TRIM.x_trim(12);  % initial body frame yaw rate
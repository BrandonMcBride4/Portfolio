function y = autopilot(uu, AP)
%
% autopilot for mavsim
% 
% Modification History:
%   2/11/2010 - RWB
%   5/14/2010 - RWB
%   11/14/2014 - RWB
%   2/13/2019 - RWB
%   

    % process inputs
    NN = 0;
%    pn       = uu(1+NN);  % inertial North position
%    pe       = uu(2+NN);  % inertial East position
    h        = uu(3+NN);  % altitude
    Va       = uu(4+NN);  % airspeed
    alpha    = uu(5+NN);  % angle of attack
    beta     = uu(6+NN);  % side slip angle
    phi      = uu(7+NN);  % roll angle
    theta    = uu(8+NN);  % pitch angle
    chi      = uu(9+NN);  % course angle
    p        = uu(10+NN); % body frame roll rate
    q        = uu(11+NN); % body frame pitch rate
    r        = uu(12+NN); % body frame yaw rate
%    Vg       = uu(13+NN); % ground speed
%    wn       = uu(14+NN); % wind North
%    we       = uu(15+NN); % wind East
%    psi      = uu(16+NN); % heading
%    bx       = uu(17+NN); % x-gyro bias
%    by       = uu(18+NN); % y-gyro bias
%    bz       = uu(19+NN); % z-gyro bias
    NN = NN+19;
    Va_c     = uu(1+NN);  % commanded airspeed (m/s)
    h_c      = uu(2+NN);  % commanded altitude (m)
    chi_c    = uu(3+NN);  % commanded course (rad)
    
%     % If phi_c_ff is specified in Simulink model, then do the following
%     phi_c_ff = uu(4+NN);  % feedforward roll command (rad)
%     NN = NN+4;
    
    % If no phi_c_ff is included in inputs in Simulink model, then do this
    NN = NN+3;
    phi_c_ff = 0;
    t        = uu(1+NN);   % time
    autopilot_version = 2;
        % autopilot_version == 1 <- used for tuning
        % autopilot_version == 2 <- standard autopilot defined in book
        % autopilot_version == 3 <- Total Energy Control for longitudinal AP
    switch autopilot_version
        case 1
           [delta, x_command] = autopilot_tuning(Va_c,h_c,chi_c,Va,h,chi,phi,theta,p,q,r,t,AP);
        case 2
           [delta, x_command] = autopilot_uavbook(Va_c,h_c,chi_c,Va,h,chi,phi,theta,p,q,r,t,AP);
%         case 3,
%            [delta, x_command] = autopilot_TECS(Va_c,h_c,chi_c,Va,h,chi,phi,theta,p,q,r,t,P);
    end
            
    y = [delta; x_command];
    
%     if length(y) ~= 16
%         disp('Error');
%     end
 
    end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Autopilot functions
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% autopilot_tuning
%   - used to tune each loop
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [delta, x_command] = autopilot_tuning(Va_c,h_c,chi_c,Va,h,chi,phi,theta,p,q,r,t,AP)
    chi_c = wrap(chi_c, chi);
    mode = 5;
    switch mode
        case 1 % tune the roll loop
            phi_c = chi_c; % interpret chi_c to autopilot as course command
            delta_a = roll_with_aileron(phi_c, phi, p, AP);
            delta_r = 0; % no rudder
            % use trim values for elevator and throttle while tuning the lateral autopilot
            delta_e = AP.u_trim(1);
            delta_t = AP.u_trim(4);
            theta_c = 0;
        case 2 % tune the course loop
            if t==0
                phi_c   = course_with_roll(chi_c, chi, 1, AP);
                delta_r = yaw_damper(r, 1, AP);
            else
                phi_c   = course_with_roll(chi_c, chi, 0, AP);
                delta_r = yaw_damper(r, 0, AP);
            end                
            delta_a = roll_with_aileron(phi_c, phi, p, AP);
            % use trim values for elevator and throttle while tuning the lateral autopilot
            delta_e = AP.u_trim(1);
            delta_t = AP.u_trim(4);
            theta_c = 0;
        case 3 % tune the throttle to airspeed loop and pitch loop simultaneously
            theta_c = h_c;
            chi_c = 0;
            if t==0
                phi_c   = course_with_roll(chi_c, chi, 1, AP);
                delta_t = airspeed_with_throttle(Va_c, Va, 1, AP);
           else
                phi_c   = course_with_roll(chi_c, chi,  0, AP);
                delta_t = airspeed_with_throttle(Va_c, Va, 0, AP);
            end
            delta_e = pitch_with_elevator(theta_c, theta, q, AP);
            delta_a = roll_with_aileron(phi_c, phi, p, AP);
            delta_r = 0; % no rudder
%             delta_a = AP.u_trim(2);
            % use trim values for elevator and throttle while tuning the lateral autopilot
        case 4 % tune the pitch to airspeed loop 
            chi_c = 0;
            delta_t = AP.u_trim(4);
            if t==0
                phi_c   = course_with_roll(chi_c, chi, 1, AP);
                theta_c = airspeed_with_pitch(Va_c, Va, 1, AP);
           else
                phi_c   = course_with_roll(chi_c, chi, 0, AP);
                theta_c = airspeed_with_pitch(Va_c, Va, 0, AP);
            end
            delta_a = roll_with_aileron(phi_c, phi, p, AP);
            delta_e = pitch_with_elevator(theta_c, theta, q, AP);
            delta_r = 0; % no rudder
            % use trim values for elevator and throttle while tuning the lateral autopilot
        case 5 % tune the pitch to altitude loop 
            chi_c = 0;
            if t==0
                phi_c   = course_with_roll(chi_c, chi, 1, AP);
                theta_c = altitude_with_pitch(h_c, h, 1, AP);
                delta_t = airspeed_with_throttle(Va_c, Va, 1, AP);
           else
                phi_c   = course_with_roll(chi_c, chi, 0, AP);
                theta_c = altitude_with_pitch(h_c, h, 0, AP);
                delta_t = airspeed_with_throttle(Va_c, Va, 0, AP);
            end
            delta_a = roll_with_aileron(phi_c, phi, p, AP);
            delta_e = pitch_with_elevator(theta_c, theta, q, AP);
            delta_r = 0; % no rudder
            % use trim values for elevator and throttle while tuning the lateral autopilot
    end
    %----------------------------------------------------------
    % create outputs
    
    % control outputs
    delta = [delta_e; delta_a; delta_r; delta_t];
    % commanded (desired) states
    x_command = [...
        0;...                    % pn
        0;...                    % pe
        h_c;...                  % h
        Va_c;...                 % Va
        0;...                    % alpha
        0;...                    % beta
        phi_c;...                % phi
        %theta_c*P.K_theta_DC;... % theta
        theta_c;
        chi_c;...                % chi
        0;...                    % p
        0;...                    % q
        0;...                    % r
        ];
            
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% autopilot_uavbook
%   - autopilot defined in the uavbook
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [delta, x_command] = autopilot_uavbook(Va_c,h_c,chi_c,Va,h,chi,phi,theta,p,q,r,t,AP)
    chi_c = wrap(chi_c, chi);
    %----------------------------------------------------------
    % lateral autopilot
    if t==0
        phi_c   = course_with_roll(chi_c, chi, 1, AP);
        delta_r = yaw_damper(r, 1, AP);
    else
        phi_c   = course_with_roll(chi_c, chi, 0, AP);
        delta_r = yaw_damper(r, 0, AP);
    end
    delta_a = roll_with_aileron(phi_c, phi, p, AP);     
  
    
    %----------------------------------------------------------
    % longitudinal autopilot
    
    % define persistent variable for state of altitude state machine
    persistent altitude_state;
    persistent init_flag_climb;
    persistent init_flag_descend;
    persistent init_flag_hold;
    % initialize persistent variable
    if t==0
        init_flag_climb = 1;
        init_flag_descend = 1;
        init_flag_hold = 1;
    end
    
    if h<=AP.altitude_take_off_zone     
        altitude_state = 1;
    elseif h<=h_c-AP.altitude_hold_zone 
        altitude_state = 2;
    elseif h>=h_c+AP.altitude_hold_zone 
        altitude_state = 3;
    else
        altitude_state = 4;
    end
    
    % implement state machine
    switch altitude_state
        case 1  % in take-off zone
            theta_c = AP.take_off_pitch;
            delta_t = AP.airspeed_throttle_delta_max;
        case 2  % climb zone
            theta_c = airspeed_with_pitch(Va_c, Va, init_flag_climb, AP);
            delta_t = AP.airspeed_throttle_delta_max;
            if init_flag_climb == 1
                init_flag_climb = 0;
            end
        case 3 % descend zone
            theta_c = airspeed_with_pitch(Va_c, Va, init_flag_descend, AP);
            delta_t = 0;
            if init_flag_descend == 1
                init_flag_descend = 0;
            end
        case 4 % altitude hold 
            theta_c = altitude_with_pitch(h_c, h, init_flag_hold, AP);
            delta_t = airspeed_with_throttle(Va_c, Va, init_flag_hold, AP);
            if init_flag_hold == 1
                init_flag_hold = 0;
            end
    end
    
    delta_e = pitch_with_elevator(theta_c, theta, q, AP);
    
    %----------------------------------------------------------
    % create outputs
    
    % control outputs
    delta = [delta_e; delta_a; delta_r; delta_t];
    % commanded (desired) states
    x_command = [...
        0;...                    % pn
        0;...                    % pe
        h_c;...                  % h
        Va_c;...                 % Va
        0;...                    % alpha
        0;...                    % beta
        phi_c;...                % phi
        %theta_c*P.K_theta_DC;... % theta
        theta_c;
        chi_c;...                % chi
        0;...                    % p
        0;...                    % q
        0;...                    % r
        ];
 
    end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% course_with_roll
%   - regulate heading using the roll command
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function phi_c_sat = course_with_roll(chi_c, chi, flag, AP)
    persistent integrator;
    persistent err_d1;
    if flag == 1
        integrator = 0;
        err_d1 = 0;
    end
    err = chi_c - chi;
    integrator = integrator + (AP.Ts/2)*(err+err_d1);
    err_d1 = err;
    phi_c_unsat = AP.course_kp*err + AP.course_ki*integrator;
    phi_c_sat = sat(phi_c_unsat, AP.course_phi_max, -AP.course_phi_max);
    
    % Anti-windup
    if AP.course_ki ~= 0
        integrator = integrator + AP.Ts/AP.course_ki * (phi_c_sat-phi_c_unsat);
    end
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% roll_with_aileron
%   - regulate roll using aileron
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function delta_a = roll_with_aileron(phi_c, phi, p, AP)
    err = phi_c - phi;
    delta_a = sat(AP.roll_kp*err - AP.roll_kd*p,...
                  AP.roll_delta_max,...
                 -AP.roll_delta_max);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% altitude_with_pitch
%   - regulate altitude using pitch angle
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function theta_c_sat = altitude_with_pitch(h_c, h, flag, AP)
    persistent integrator;
    persistent err_d1;
    if flag == 1
        integrator = 0;
        err_d1 = 0;
    end
    err = h_c - h;
    integrator = integrator + (AP.Ts/2)*(err+err_d1);
    err_d1 = err;
    theta_c_unsat = AP.altitude_kp*err + AP.altitude_ki*integrator;
    theta_c_sat = sat(theta_c_unsat, AP.theta_max, -AP.theta_max);
    
    % Anti-windup
    if AP.altitude_ki ~= 0
        integrator = integrator + AP.Ts/AP.altitude_ki * (theta_c_sat-theta_c_unsat);
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% pitch_with_elevator
%   - regulate pitch using elevator
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function delta_e = pitch_with_elevator(theta_c, theta, q, AP)
    err = theta_c - theta;
    delta_e = sat(AP.pitch_kp*err - AP.pitch_kd*q,...
                  AP.pitch_delta_max,...
                  -AP.pitch_delta_max);
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% airspeed_with_throttle
%   - regulate airspeed using throttle
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function theta_c_sat = airspeed_with_pitch(Va_c, Va, flag, AP)
    persistent integrator;
    persistent err_d1;
    if flag == 1
        integrator = 0;
        err_d1 = 0;
    end
    err = Va_c - Va;
    integrator = integrator + (AP.Ts/2)*(err+err_d1);
    err_d1 = err;
    theta_c_unsat = AP.airspeed_pitch_kp*err + AP.airspeed_pitch_ki*integrator;
    theta_c_sat = sat(theta_c_unsat, AP.theta_max, -AP.theta_max);
    
    % Anti-windup
    if AP.airspeed_pitch_ki ~= 0
        integrator = integrator + AP.Ts/AP.airspeed_pitch_ki * (theta_c_sat-theta_c_unsat);
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% airspeed_with_throttle
%   - regulate airspeed using throttle
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function delta_t_sat = airspeed_with_throttle(Va_c, Va, flag, AP)
    persistent integrator;
    persistent err_d1;
    if flag == 1
        integrator = 0;
        err_d1 = 0;
    end
    err = Va_c - Va;
    integrator = integrator + (AP.Ts/2)*(err+err_d1);
    err_d1 = err;
    delta_t_unsat = AP.airspeed_throttle_kp*err + AP.airspeed_throttle_ki*integrator;
    delta_t_sat = sat(delta_t_unsat, AP.airspeed_throttle_delta_max, 0);
    
    % Anti-windup
    if AP.airspeed_throttle_ki ~= 0
        integrator = integrator + AP.Ts/AP.airspeed_throttle_ki * (delta_t_sat-delta_t_unsat);
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% sideslip_with_rudder
%   - sideslip with rudder
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function delta_r = sideslip_with_rudder(v, flag, AP)
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% yaw_damper
%   - yaw rate with rudder
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function delta_r = yaw_damper(r, flag, AP)
    persistent delta_r1
    persistent r1
    
    if flag == 1
        delta_r1 = 0;
        r1 = 0;
    end
    a1 = (2*AP.sigma - AP.Ts) / (2*AP.sigma + AP.Ts);
    a2 = 2 / (2*AP.sigma + AP.Ts);
    
    % dirty derivative multiplied by kr and sigma
    delta_r = (AP.yaw_damper_kr*AP.sigma)*(a1*delta_r1 + a2*(r-r1));
    delta_r1 = delta_r;
    r1 = r;
end
  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% sat
%   - saturation function
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function out = sat(in, up_limit, low_limit)
  if in > up_limit
      out = up_limit;
  elseif in < low_limit
      out = low_limit;
  else
      out = in;
  end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% wrap
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function chi_c = wrap(chi_c, chi)
    diff = chi_c - chi;
    dist_p360 = abs(diff + 2*pi);
    dist_m360 = abs(diff - 2*pi);
    dist = abs(diff);
    if  dist_p360 < dist && dist_p360 < dist_m360
        chi_c = chi_c + 2*pi;
    elseif dist_m360 < dist && dist_m360 < dist_p360
        chi_c = chi_c - 2*pi;
    end
end
  
 
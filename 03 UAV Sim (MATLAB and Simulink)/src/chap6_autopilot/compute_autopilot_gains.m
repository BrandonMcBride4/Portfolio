addpath('../chap5_linear')
compute_tf_model
addpath('../parameters')
simulation_parameters

% AP stands for autopilot
AP.gravity = MAV.gravity;
AP.sigma = 0.05; % dirty derivative time constant
AP.Va0 = TRIM.Va_trim;
AP.Ts = SIM.ts_control;
AP.u_trim = TRIM.u_trim;

%----------autopilot -------------
AP.altitude_take_off_zone = 50;
AP.altitude_hold_zone = 25;
AP.take_off_pitch = 25*pi/180;

%----------roll loop-------------
AP.roll_delta_max = 10*pi/180; % max physically is 45 to -45 degrees
AP.roll_t_r = 0.1358;
AP.roll_wn = 2.2/AP.roll_t_r;
AP.roll_zeta = 1;

AP.roll_kp = AP.roll_wn^2/TF.a_phi2;
AP.roll_kd = (2*AP.roll_zeta*AP.roll_wn-TF.a_phi1)/TF.a_phi2;
%----------course loop-------------
AP.course_phi_max = 30*pi/180;
AP.course_bw_sep = 57.8;
AP.course_wn = AP.roll_wn/AP.course_bw_sep;
AP.course_zeta = 1/sqrt(2);
AP.yaw_damper_kr = 0.5;

AP.course_kp = 2*AP.course_zeta*AP.course_wn*AP.Va0/AP.gravity;
AP.course_ki = AP.course_wn^2*AP.Va0/AP.gravity; 

%----------sideslip loop-------------
AP.sideslip_e_max = 10*pi/180;     % max physically is 90 degrees
AP.sideslip_delta_max = 30*pi/180; % max physically is 45 to -45 degrees
AP.sideslip_zeta = 0.6;

AP.sideslip_kp = sign(TF.a_beta2)*AP.sideslip_delta_max/AP.sideslip_e_max;
AP.sideslip_ki = (1/TF.a_beta2)*...
                 ((TF.a_beta1+TF.a_beta2*AP.sideslip_kp)/(2*AP.sideslip_zeta))^2;

%----------pitch loop-------------
AP.pitch_e_max = 5.2941*pi/180;      % max physically is 90 degrees
AP.pitch_delta_max = 45*pi/180; % max physically is 45 to -45 degrees
AP.pitch_wn = sqrt(TF.a_theta2+abs(TF.a_theta3)*AP.pitch_delta_max/AP.pitch_e_max);
AP.pitch_zeta = 0.5789;

AP.pitch_kp = sign(TF.a_theta3)*AP.pitch_delta_max/AP.pitch_e_max;
AP.pitch_kd = (2*AP.pitch_zeta*AP.pitch_wn-TF.a_theta1)/TF.a_theta3;
AP.K_theta_DC = (AP.pitch_kp*TF.a_theta3)/(TF.a_theta2+AP.pitch_kp*TF.a_theta3);

%----------altitude loop-------------
AP.theta_max = 30*pi/180;
AP.altitude_bw_sep = 20.7762;
AP.altitude_wn = AP.pitch_wn/AP.altitude_bw_sep;
AP.altitude_zeta = 0.9711;

AP.altitude_kp = (2*AP.altitude_zeta*AP.altitude_wn)/(AP.K_theta_DC*AP.Va0);
AP.altitude_ki = (AP.altitude_wn^2)/(AP.K_theta_DC*AP.Va0);

%---------airspeed hold using pitch---------------
AP.airspeed_pitch_bw_sep = 20;
AP.airspeed_pitch_wn = AP.pitch_wn/AP.airspeed_pitch_bw_sep;
AP.airspeed_pitch_zeta = 1;

AP.airspeed_pitch_kp = (TF.a_V1-2*AP.airspeed_pitch_zeta*AP.airspeed_pitch_wn)/...
                       (AP.K_theta_DC*AP.gravity);
AP.airspeed_pitch_ki = (-AP.airspeed_pitch_wn^2)/(AP.K_theta_DC*AP.gravity);

%---------airspeed hold using throttle---------------
AP.airspeed_throttle_delta_max = 1;
AP.airspeed_throttle_t_r = 1; % rise time
AP.airspeed_throttle_wn = 2.2/AP.airspeed_throttle_t_r;
AP.airspeed_throttle_zeta = 0.7;

AP.airspeed_throttle_kp = (2*AP.airspeed_throttle_zeta*AP.airspeed_throttle_wn-TF.a_V1)/...
                          (TF.a_V2);
AP.airspeed_throttle_ki = AP.airspeed_throttle_wn^2/TF.a_V2;

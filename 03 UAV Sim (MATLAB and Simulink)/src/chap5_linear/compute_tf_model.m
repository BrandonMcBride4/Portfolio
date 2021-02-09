compute_trim
% x_trim is the trimmed state,
% u_trim is the trimmed input
TRIM.delta_e_trim = TRIM.u_trim(1);
TRIM.delta_t_trim = TRIM.u_trim(4);
TRIM.Va_trim = TRIM.y_trim(1);
TRIM.alpha_trim = TRIM.y_trim(2);
TRIM.beta_trim = TRIM.y_trim(3);
TRIM.theta_trim = TRIM.x_trim(8);

TF.a_phi1 = -0.5*MAV.rho*TRIM.Va_trim^2*MAV.S_wing*MAV.b*MAV.C_p_p*MAV.b/(2*TRIM.Va_trim);
TF.a_phi2 = 0.5*MAV.rho*TRIM.Va_trim^2*MAV.S_wing*MAV.b*MAV.C_p_delta_a;
TF.a_theta1 = -(MAV.rho*TRIM.Va_trim^2*MAV.c*MAV.S_wing/(2*MAV.Jy))*MAV.C_m_q*(MAV.c/(2*TRIM.Va_trim));
TF.a_theta2 = -MAV.C_m_alpha*MAV.rho*TRIM.Va_trim^2*MAV.c*MAV.S_wing/(2*MAV.Jy);
TF.a_theta3 = MAV.C_m_delta_e*MAV.rho*TRIM.Va_trim^2*MAV.c*MAV.S_wing/(2*MAV.Jy);
TF.a_V1 =  (MAV.rho*TRIM.Va_trim*MAV.S_wing/MAV.mass)...
       *(MAV.C_D_0+MAV.C_D_alpha*TRIM.alpha_trim+MAV.C_D_delta_e*TRIM.delta_e_trim)...
       +(MAV.rho*MAV.S_prop*MAV.C_prop*TRIM.Va_trim/MAV.mass);
TF.a_V2 = MAV.rho*MAV.S_prop*MAV.C_prop*MAV.k_motor^2*TRIM.delta_t_trim/MAV.mass;
TF.a_V3 = MAV.gravity*cos(TRIM.theta_trim-TRIM.alpha_trim);
TF.a_beta1 = -MAV.C_Y_beta*MAV.rho*TRIM.Va_trim*MAV.S_wing/(2*MAV.mass*cos(TRIM.beta_trim));
TF.a_beta2 = MAV.C_Y_delta_r*MAV.rho*TRIM.Va_trim*MAV.S_wing/(2*MAV.mass*cos(TRIM.beta_trim));

% define transfer functions
TF.T_phi_delta_a   = tf([TF.a_phi2],[1,TF.a_phi1,0]);
TF.T_chi_phi       = tf([MAV.gravity/TRIM.Va_trim],[1,0]);
TF.T_theta_delta_e = tf(TF.a_theta3,[1,TF.a_theta1,TF.a_theta2]);
TF.T_h_theta       = tf([TRIM.Va_trim],[1,0]);
TF.T_h_Va          = tf([TRIM.theta_trim],[1,0]);
TF.T_Va_delta_t    = tf([TF.a_V2],[1,TF.a_V1]);
TF.T_Va_theta      = tf([-TF.a_V3],[1,TF.a_V1]);
TF.T_v_delta_r     = tf([TF.a_beta2],[1,TF.a_beta1]);

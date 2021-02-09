EKF.N_att = 4; % Number of loops in prediction step (attitude)
EKF.N_gps = 8; % Number of loops in prediction step (gps)
EKF.Ts_att = SIM.ts_control/EKF.N_att;
EKF.Ts_gps = SIM.ts_control/EKF.N_gps;
EKF.threshold_att = SENSOR.accel_sigma;
EKF.P_att = eye(2);
EKF.Q_att = 1e-9*eye(2);
EKF.Q_gyro = SENSOR.gyro_sigma^2*eye(3);
EKF.R_accel = SENSOR.accel_sigma^2*eye(3);
EKF.P_gps = eye(5);
EKF.Q_gps = diag([...
                    7e-2,... # pn
                    9e-2,... # pe
                    8e-2,... # Vg
                    1e-3,... # chi
                    2e-3...  # psi
                 ]);
EKF.R_gps = diag([...
                    SENSOR.gps_n_sigma^2,...      # y_gps_n
                    SENSOR.gps_e_sigma^2,...      # y_gps_e
                    SENSOR.gps_Vg_sigma^2,...     # y_gps_Vg
                    SENSOR.gps_course_sigma^2,... # y_gps_chi
                 ]);

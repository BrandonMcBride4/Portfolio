% Accelerometer
SENSOR.accel_sigma = 0.0025*9.8; % (m/s^2)

%Rate Gyro
SENSOR.gyro_x_bias = 0;
SENSOR.gyro_y_bias = 0;
SENSOR.gyro_z_bias = 0;
SENSOR.gyro_sigma  = 0.13*pi/180; % (rad/sec)

% Pressure Sensor Altitude
SENSOR.static_pressure_sigma = 0.01*1000; % (Pascals)

% Pressure Sensor Airspeed
SENSOR.diff_pressure_sigma = 0.002*1000; % (Pascals)

% Magnetometer
SENSOR.mag_beta = 1*pi/180;
SENSOR.mag_sigma = 0.03*pi/180;

% GPS
SENSOR.ts_gps = 1;
SENSOR.gps_beta = 1/1100; % (s^-1)
SENSOR.gps_n_sigma = 0.21;
SENSOR.gps_e_sigma = 0.21;
SENSOR.gps_h_sigma = 0.40;
SENSOR.gps_Vg_sigma = 0.05;
SENSOR.gps_course_sigma = SENSOR.gps_Vg_sigma/10;

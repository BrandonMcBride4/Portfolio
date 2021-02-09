function [R] = getVehicleToBodyRotationMatrix(euler)
    phi = euler(1);
    theta = euler(2);
    psi = euler(3);
    
    % get rotation matrix
    c_phi = cos(phi);
    s_phi = sin(phi);

    c_theta = cos(theta);
    s_theta = sin(theta);

    c_psi = cos(psi);
    s_psi = sin(psi);

    % define rotation matrix (right handed)
    R_roll = [...
            1, 0, 0;...
            0, c_phi, s_phi;...
            0, -s_phi, c_phi];
    R_pitch = [...
            c_theta, 0, -s_theta;...
            0, 1, 0;...
            s_theta, 0, c_theta];
    R_yaw = [...
            c_psi, s_psi, 0;...
            -s_psi, c_psi, 0;...
            0, 0, 1];
    R = R_roll*R_pitch*R_yaw;
end


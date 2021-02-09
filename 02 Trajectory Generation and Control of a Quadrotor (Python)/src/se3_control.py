import numpy as np
from scipy.spatial.transform import Rotation

class SE3Control(object):
    """

    """
    def __init__(self, quad_params):
        """
        This is the constructor for the SE3Control object. You may instead
        initialize any parameters, control gain values, or private state here.

        For grading purposes the controller is always initialized with one input
        argument: the quadrotor's physical parameters. If you add any additional
        input arguments for testing purposes, you must provide good default
        values!

        Parameters:
            quad_params, dict with keys specified by crazyflie_params.py

        """

        # Quadrotor physical parameters.
        self.mass            = quad_params['mass'] # kg
        self.Ixx             = quad_params['Ixx']  # kg*m^2
        self.Iyy             = quad_params['Iyy']  # kg*m^2
        self.Izz             = quad_params['Izz']  # kg*m^2
        self.arm_length      = quad_params['arm_length'] # meters
        self.rotor_speed_min = quad_params['rotor_speed_min'] # rad/s
        self.rotor_speed_max = quad_params['rotor_speed_max'] # rad/s
        self.k_thrust        = quad_params['k_thrust'] # N/(rad/s)**2
        self.k_drag          = quad_params['k_drag']   # Nm/(rad/s)**2

        # You may define any additional constants you like including control gains.
        self.inertia = np.diag(np.array([self.Ixx, self.Iyy, self.Izz])) # kg*m^2
        self.g = 9.81 # m/s^2

        # Motor param
        self.gamma = self.k_drag / self.k_thrust

        # STUDENT CODE HERE
        self.f_matrix = np.row_stack((np.ones(4, ),
                                      [0, self.arm_length, 0, -self.arm_length],
                                      [-self.arm_length, 0, self.arm_length, 0],
                                      [self.gamma, -self.gamma, self.gamma, -self.gamma]))
        self.f_matrix_inv = np.linalg.inv(self.f_matrix)

        wr = 200 # bandwidth of attitude control should be 5-10x faster
        zr = 1. # Critically damped
        kr = wr**2
        # kr = 256
        kw = 2*wr*zr
        # kw = 33

        wrz = 50
        krz = wrz**2
        kwz = 2*wr*zr

        wp = wr/10  # bandwidth of position control
        zp = 1.  # 1 is critically damped
        kp = wp ** 2
        # kp = 10
        kd = 2*wp*zp
        # kd = 7

        wpz = wrz/10
        wpz = 20
        kpz = wpz**2
        kdz = 2 * wpz * zp

        self.K_p = kp*np.diag(np.ones(3,))
        self.K_d = kd*np.diag(np.ones(3,))

        self.K_p[2,2] = kpz
        self.K_d[2,2] = kdz

        self.K_R = kr*np.diag(np.ones(3,))
        self.K_w = kw*np.diag(np.ones(3,))

        self.K_R[2,2] = krz
        self.K_w[2,2] = kwz



    def update(self, t, state, flat_output):
        """
        This function receives the current time, true state, and desired flat
        outputs. It returns the command inputs.

        Inputs:
            t, present time in seconds
            state, a dict describing the present state with keys
                x, position, m
                v, linear velocity, m/s
                q, quaternion [i,j,k,w]
                w, angular velocity, rad/s
            flat_output, a dict describing the present desired flat outputs with keys
                x,        position, m
                x_dot,    velocity, m/s
                x_ddot,   acceleration, m/s**2
                x_dddot,  jerk, m/s**3
                x_ddddot, snap, m/s**4
                yaw,      yaw angle, rad
                yaw_dot,  yaw rate, rad/s

        Outputs:
            control_input, a dict describing the present computed control inputs with keys
                cmd_motor_speeds, rad/s
                cmd_thrust, N (for debugging and laboratory; not used by simulator)
                cmd_moment, N*m (for debugging; not used by simulator)
                cmd_q, quaternion [i,j,k,w] (for laboratory; not used by simulator)
        """
        cmd_motor_speeds = np.zeros((4,))
        cmd_thrust = 0
        cmd_moment = np.zeros((3,))
        cmd_q = np.zeros((4,))

        # STUDENT CODE HERE
        r = state['x']
        r_dot = state['v']

        rt = flat_output['x']
        rt_dot = flat_output['x_dot']
        rt_ddot = flat_output['x_ddot']

        # Calculate u1
        rddot_des = rt_ddot - self.K_d @ (r_dot - rt_dot) - self.K_p @ (r - rt)
        F_des = self.mass*rddot_des + np.array([0, 0, self.mass*self.g])
        R = Rotation(state['q']).as_matrix()
        b3 = R[:, 2]
        u1 = b3.T @ F_des

        # Calculate u2
        b3_des = F_des/np.linalg.norm(F_des)
        a_yaw = np.array([np.cos(flat_output['yaw']), np.sin(flat_output['yaw']), 0])
        b2_des = np.cross(b3_des, a_yaw)
        b2_des /= np.linalg.norm(b2_des) # make b2 unit length
        R_des = np.column_stack((np.cross(b2_des, b3_des),b2_des,b3_des))
        e_R = (R_des.T @ R - R.T @ R_des)
        e_R = 0.5*np.array([e_R[2, 1], e_R[0, 2], e_R[1, 0]])
        u2 = self.inertia@(-self.K_R@e_R - self.K_w@(state['w']))

        # Calculate motor speeds
        u = np.concatenate([[u1], u2])
        F = np.clip(self.f_matrix_inv @ u, self.rotor_speed_min, None)
        omega = np.clip(np.sqrt(F)/np.sqrt(self.k_thrust), self.rotor_speed_min, self.rotor_speed_max)

        cmd_motor_speeds = omega
        cmd_thrust = u1
        cmd_moment = u2
        cmd_q = Rotation.from_matrix(R_des).as_quat()



        control_input = {'cmd_motor_speeds':cmd_motor_speeds,
                         'cmd_thrust':cmd_thrust,
                         'cmd_moment':cmd_moment,
                         'cmd_q':cmd_q}
        return control_input

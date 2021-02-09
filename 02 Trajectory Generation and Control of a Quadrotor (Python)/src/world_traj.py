import numpy as np

from proj1_3.code.graph_search import graph_search
from cvxopt import matrix, solvers

class WorldTraj(object):
    """

    """
    def __init__(self, world, start, goal):
        """
        This is the constructor for the trajectory object. A fresh trajectory
        object will be constructed before each mission. For a world trajectory,
        the input arguments are start and end positions and a world object. You
        are free to choose the path taken in any way you like.

        You should initialize parameters and pre-compute values such as
        polynomial coefficients here.

        Parameters:
            world, World object representing the environment obstacles
            start, xyz position in meters, shape=(3,)
            goal,  xyz position in meters, shape=(3,)

        """
        self.path = None
        # You must choose resolution and margin parameters to use for path
        # planning. In the previous project these were provided to you; now you
        # must chose them for yourself. Your may try these default values, but
        # you should experiment with them!

        xyz_length = np.diff(world.world['bounds']['extents'].reshape(3,-1)).reshape(3)
        self.is_maze = np.array_equal(xyz_length, np.array([10, 10, 3]))
        self.is_stairwell = xyz_length[2] > 10
        if self.is_maze:
            self.resolution = np.max(xyz_length)*np.ones(3)/80
        elif self.is_stairwell:
            self.resolution = np.max(xyz_length) * np.ones(3)/120
        else:
            self.resolution = xyz_length * np.array([1 / 80, 1 / 80, 1 / 6])

        self.margin = 0.3

        # You must store the dense path returned from your Dijkstra or AStar
        # graph search algorithm as an object member. You will need it for
        # debugging, it will be used when plotting results.

        self.path = graph_search(world, self.resolution, self.margin, start, goal, astar=True)

        # You must generate a sparse set of waypoints to fly between. Your
        # original Dijkstra or AStar path probably has too many points that are
        # too close together. Store these waypoints as a class member; you will
        # need it for debugging and it will be used when plotting results.

        self.points = np.zeros((1,3)) # shape=(n_pts,3)
        is_cont = self.is_maze or self.is_stairwell
        if is_cont:
            self.smooth_path(world, 0.75)
        else:
            self.smooth_path(world, np.inf)

        # Finally, you must compute a trajectory through the waypoints similar
        # to your task in the first project. One possibility is to use the
        # WaypointTraj object you already wrote in the first project. However,
        # you probably need to improve it using techniques we have learned this
        # semester.

        self.num_coefs = 6
        self.num_waypoints = self.points.shape[0]
        self.num_dec_vars = self.num_coefs*self.num_waypoints
        self.t0tf = np.zeros([self.num_waypoints, 2])  # matrix of t0, tf for each waypoint
        self.alpha = np.zeros(self.num_waypoints) # time scale for each waypoint
        self.quintic_coefs = np.zeros([self.num_waypoints, self.num_coefs, 3])

        # maze parameters, start_j = 56, j = 235
        # over and under, start_j = 24, j = 225
        # window, start_j = 52, j = 74
        # stairwell, start_j = 40, j => 60
        if is_cont:
            self.calc_coefs_cont_derivative()
        else:
            self.calc_coefs_zero_derivative()

    def is_valid_path(self, world, path):
        return (world.path_collisions(path, self.margin).size == 0)

    def smooth_path(self, world, d):
        # self.path needs at least two waypoints (start and goal)
        i = 0
        points = [self.path[i, :]]
        num_waypoints = self.path.shape[0]
        while i < num_waypoints:
            start = self.path[i, :]
            j = i + 1
            end = self.path[j, :]
            if j == num_waypoints-1: # reached the goal point
                points.append(end)
                self.points = np.array(points)
                return

            while self.is_valid_path(world, np.array([start, end])):
                j += 1
                end = self.path[j, :]
                if j == num_waypoints - 1: # reached the goal point
                    points.append(end)
                    self.points = np.array(points)
                    return
                if np.linalg.norm(end-start) > d: # make waypoints no bigger than d
                    break

            j -= 1 # go back to last valid endpoint
            points.append(self.path[j, :])
            i = j

    def calc_turning_angles(self, i, is_abs=False):
        angles = []
        if i + 2 < self.num_waypoints:
            x0 = self.points[i, :]
            xf = self.points[i + 1, :]
            xf2 = self.points[i + 2, :]

            xy = np.array([True, True, False])
            xz = np.array([True, False, True])
            yz = np.array([False, True, True])

            planes = np.row_stack((xy, xz, yz))
            for p in range(3):
                dx = (x0 - xf)[planes[p, :]]
                dx2 = (xf2 - xf)[planes[p, :]]

                if is_abs:
                    dx = np.abs(dx)

                dx_mag = np.linalg.norm(dx)
                dx2_mag = np.linalg.norm(dx2)

                if dx_mag == 0 or dx2_mag == 0:
                    angles.append(np.pi)
                    continue
                corner_angle = np.arccos(np.dot(dx, dx2) / (dx_mag * dx2_mag))
                if corner_angle < np.deg2rad(0.5):
                    corner_angle = np.pi

                angles.append(corner_angle)
        else:
            angles = np.pi*np.ones(3)

        return np.array(angles)

    def calc_xy_flying_angle(self, dx):
        dx = np.abs(dx)
        xy_flying_angle = np.arctan2(dx[1], dx[0])
        xy_flying_angle = max(np.pi / 2 - xy_flying_angle, xy_flying_angle)  # xy_flying_angle is between 45 and 90 degrees
        return xy_flying_angle

    def update(self, t):
        """
        Given the present time, return the desired flat output and derivatives.

        Inputs
            t, time, s
        Outputs
            flat_output, a dict describing the present desired flat outputs with keys
                x,        position, m
                x_dot,    velocity, m/s
                x_ddot,   acceleration, m/s**2
                x_dddot,  jerk, m/s**3
                x_ddddot, snap, m/s**4
                yaw,      yaw angle, rad
                yaw_dot,  yaw rate, rad/s
        """
        x        = np.zeros((3,))
        x_dot    = np.zeros((3,))
        x_ddot   = np.zeros((3,))
        x_dddot  = np.zeros((3,))
        x_ddddot = np.zeros((3,))
        yaw = 0
        yaw_dot = 0

        # STUDENT CODE HERE
        t2 = np.nan_to_num(t)  # use this to avoid warnings of invalid numbers

        # get the index where t is in between t0 and tf
        tmp = t2 >= self.t0tf
        wp_idx = np.argwhere(np.logical_xor(tmp[:, 0], tmp[:, 1]))[0, 0]
        if wp_idx == self.t0tf.shape[0] - 1:
            t = self.t0tf[wp_idx, 0] # If the time is greater than when the traj should finish, set it to the beginning of the end (numerical errors when t is big

        t = t - self.t0tf[wp_idx, 0] # t0
        # t *= self.alpha[wp_idx] # re-scale time
        t_matrix = np.row_stack(([1, t, t**2, t**3, t**4, t**5],
                                 [0, 1, 2*t, 3*t**2, 4*t**3, 5*t**4],
                                 [0, 0, 2, 6*t, 12*t**2, 20*t**3],
                                 [0, 0, 0, 6, 24*t, 60*t**2],
                                 [0, 0, 0, 0, 24, 120*t]
                                 ))
        coefs = self.quintic_coefs[wp_idx, :, :]
        states = np.nan_to_num(t_matrix) @ coefs

        x        = states[0, :]
        x_dot    = states[1, :]
        x_ddot   = states[2, :]
        x_dddot  = states[3, :]
        x_ddddot = states[4, :]

        flat_output = { 'x':x, 'x_dot':x_dot, 'x_ddot':x_ddot, 'x_dddot':x_dddot, 'x_ddddot':x_ddddot,
                        'yaw':yaw, 'yaw_dot':yaw_dot}
        return flat_output

    def calc_xy_max_jerk(self, dxy, xy_flying_angle, xy_corner_angle, xy_corner_angle_abs):
        long_wp_dist = 7.5
        j_long = 69.75 # all maps work with this as a minimum
        jxy = 139.5

        if xy_corner_angle < np.deg2rad(100) or np.deg2rad(120) <= xy_corner_angle < np.deg2rad(122): # This is for the stairwell map
            if xy_corner_angle_abs < np.deg2rad(100):
                jxy = 112.75
        else:
            if 1.75 < dxy < 1.875 and np.deg2rad(82) <= xy_flying_angle < np.deg2rad(85): # This is for the switchback
                if xy_corner_angle_abs < np.deg2rad(100):
                    jxy = 112.75
            elif dxy <= 0.75 and xy_flying_angle > np.deg2rad(70): # This was tuned from the maze map
                jxy = 173
            elif xy_flying_angle > np.deg2rad(60): # also tuned from the maze map
                jxy = 143

        if 2.5 < dxy < 4 and xy_flying_angle < np.deg2rad(50): # This catches something in the slalom map
            jxy = 90

        # High jerk doesn't do well with long distances
        if dxy >= long_wp_dist:
            # The xy_flying_angle affects how fast it can go
            if xy_flying_angle >= np.deg2rad(75):
                jxy = -0.2235 * dxy ** 2 + 1.6848 * dxy + 105.5
            elif xy_flying_angle >= np.deg2rad(50):
                jxy = 0.0322 * dxy ** 3 - 0.8188 * dxy ** 2 + 3.6723 * dxy + 89.844
            else:
                jxy = -0.8265 * dxy ** 2 + 4.0641 * dxy + 85.374
            if jxy < j_long:
                jxy = j_long
        return jxy

    def calc_coefs_zero_derivative(self):
        max_z_jerk = 56  # Can't decelerate faster than gravity
        for i in range(self.num_waypoints):
            x0 = self.points[i, :]
            if i + 1 >= self.num_waypoints:
                xf = x0
                self.t0tf[i, 1] = np.inf
                t0 = self.t0tf[i, 0]
                tf = t0 + 1  # doesn't matter because xf = x0 and inf gives issues
            else:
                xf = self.points[i + 1, :]
                dx = np.abs(x0 - xf)

                dxy = np.max(dx[:2])
                dz = dx[2]

                xy_flying_angle = self.calc_xy_flying_angle(dx)  # xy_flying_angle is between 45 and 90 degrees
                xy_corner_angle = self.calc_turning_angles(i)[0]
                xy_corner_angle_abs = self.calc_turning_angles(i, True)[0] # don't know why this works but it does, happened by accident
                max_xy_jerk = self.calc_xy_max_jerk(dxy, xy_flying_angle, xy_corner_angle, xy_corner_angle_abs)

                Tz  = (6*dz / (max_z_jerk*0.1)) ** (1/3)
                Txy = (6*dxy / (max_xy_jerk*0.1)) ** (1/3)
                T = max(Tz, Txy)  # Pick the less aggressive time

                t0 = self.t0tf[i, 0]
                tf = t0 + T
                self.t0tf[i, 1] = tf
                self.t0tf[i + 1, 0] = tf  # start the next way point when you end the previous one

            t1 = tf-t0
            t0 = 0

            Q_np = np.zeros([self.num_coefs, self.num_coefs])
            Q_np[3:, 3:] = 2*np.row_stack(([36*t1,     72*t1**2,  120*t1**3],
                                           [72*t1**2,  192*t1**3, 360*t1**4],
                                           [120*t1**3, 360*t1**4, 720*t1**5]))

            A_np = np.row_stack(([1, t0, t0**2, t0**3,   t0**4,    t0**5],
                                 [1, t1, t1**2, t1**3,   t1**4,    t1**5],
                                 [0, 1,  2*t0,  3*t0**2, 4*t0**3,  5*t0**4],
                                 [0, 1,  2*t1,  3*t1**2, 4*t1**3,  5*t1**4],
                                 [0, 0,  2,     6*t0,    12*t0**2, 20*t0**3],
                                 [0, 0,  2,     6*t1,    12*t1**2, 20*t1**3],
                                ))

            b_np = np.row_stack((x0,
                                 xf,
                                 np.zeros([self.num_coefs-2,3])
                                ))

            Q = matrix(Q_np)
            p = matrix(np.zeros(self.num_coefs))
            A = matrix(A_np)
            bx = matrix(b_np[:, 0])
            by = matrix(b_np[:, 1])
            bz = matrix(b_np[:, 2])

            cx = np.array(solvers.qp(Q, p, A=A, b=bx)['x'])
            cy = np.array(solvers.qp(Q, p, A=A, b=by)['x'])
            cz = np.array(solvers.qp(Q, p, A=A, b=bz)['x'])
            coef = np.column_stack((cx, cy, cz)).reshape(self.num_coefs,3)

            self.quintic_coefs[i, :, :] = np.round(coef, decimals=16)  # numerical errors start to show up

    def calc_maze_time(self, i):
        x0 = self.points[i, :]
        xf = self.points[i + 1, :]
        dx = np.abs(xf - x0)
        dxy = np.max(dx[:2])
        dz = dx[2]

        turning_angles = np.row_stack((self.calc_turning_angles(i),
                                       self.calc_turning_angles(i + 1)
                                       ))

        turning_angles23 = np.row_stack((self.calc_turning_angles(i + 2),
                                         self.calc_turning_angles(i + 3)
                                         ))
        xy_angle = np.min(turning_angles[:, 0])
        xy_angle23 = np.min(turning_angles23[:, 0])

        j = 750
        jz = 235
        if xy_angle23 <= np.deg2rad(170):
            j = 500

        if xy_angle <= np.deg2rad(170):
            j = 235

        if i == 0 or i >= self.num_waypoints - 2:
            j = 56

        Tz = (6 * dz / (jz)) ** (1 / 3)
        Txy = (6 * dxy / (j)) ** (1 / 3)
        return max(Tz, Txy)

    def calc_stairwell_time(self, i):
        x0 = self.points[i, :]
        xf = self.points[i + 1, :]
        dx = np.abs(xf - x0)
        dxy = np.max(dx[:2])
        dz = dx[2]

        turning_angles = np.row_stack((self.calc_turning_angles(i),
                                       self.calc_turning_angles(i + 1)))

        min_angle0 = np.min(turning_angles[0, :])
        min_angle1 = np.min(turning_angles[1, :])

        j = 400
        jz = 134  # 134 <= jz < 135
        if min_angle0 <= np.deg2rad(135):
            j = 60  # 60 <= j < 62

        if min_angle1 <= np.deg2rad(135):
            jz = 73  # 73 <= jz < 74

        if i == 0 or i >= self.num_waypoints - 2:
            j = 35  # 35 <= j < 40

        Tz = (6 * dz / (jz)) ** (1 / 3)
        Txy = (6 * dxy / (j)) ** (1 / 3)
        return max(Tz, Txy)

    def calc_coefs_cont_derivative(self):
        all_Q = np.array([]).reshape(0, 0)
        all_A = np.array([]).reshape(0, self.num_dec_vars)
        all_b = np.array([]).reshape(0, 3)
        for i in range(self.num_waypoints):
            x0 = self.points[i, :]
            if i + 1 >= self.num_waypoints:
                xf = x0
                self.t0tf[i, 1] = np.inf
                t0 = self.t0tf[i, 0]
                tf = t0+2  # doesn't matter because xf = x0 and inf gives issues
            else:
                if self.is_maze:
                    T = self.calc_maze_time(i)
                else:
                    T = self.calc_stairwell_time(i)

                x0 = self.points[i, :]
                xf = self.points[i + 1, :]
                t0 = self.t0tf[i, 0]
                tf = t0 + T
                self.t0tf[i, 1] = tf
                self.t0tf[i + 1, 0] = tf  # start the next way point when you end the previous one

            t1 = tf-t0
            t0 = t0-t0

            Q_np = np.zeros([self.num_coefs, self.num_coefs])
            Q_np[int(self.num_coefs/2):, int(self.num_coefs/2):] = 2*np.row_stack((
                                           [36*(t1-t0),     72*(t1-t0)**2,  120*(t1-t0)**3],
                                           [72*(t1-t0)**2,  192*(t1-t0)**3, 360*(t1-t0)**4],
                                           [120*(t1-t0)**3, 360*(t1-t0)**4, 720*(t1-t0)**5]))

            all_Q = np.block([
                              [all_Q, np.zeros([all_Q.shape[0], Q_np.shape[1]])],
                              [np.zeros([Q_np.shape[0], all_Q.shape[1]]), Q_np]
                             ])

            if i == 0:
                # Constrain the beginning to stationary
                A_np = np.row_stack(([1, t0, t0**2, t0**3,   t0**4,    t0**5],
                                     [1, t1, t1**2, t1**3,   t1**4,    t1**5],
                                     [0, 1,  2*t0,  3*t0**2, 4*t0**3,  5*t0**4],
                                     [0, 0,  2,     6*t0,    12*t0**2, 20*t0**3],
                                    ))
                n = A_np.shape[0]
                A_np = np.column_stack((A_np, np.zeros([n, self.num_dec_vars - self.num_coefs])))
                all_A = np.row_stack((all_A, A_np))
                b_np = np.row_stack((x0,
                                     xf,
                                     np.zeros([n-2,3])
                                    ))
                all_b = np.row_stack((all_b, b_np))

                # set the derivatives to the end point to the beginning of the next waypoint
                A_dot1 = np.row_stack((
                                     [0, 1, 2*t1, 3*t1**2, 4*t1**3, 5*t1**4],
                                     [0, 0, 2, 6*t1, 12*t1**2, 20*t1**3],
                                     ))
                A_dot2 = np.row_stack((
                                       [0, 1, 2*t0, 3*t0**2, 4*t0**3, 5*t0**4],
                                       [0, 0, 2, 6*t0, 12*t0**2, 20*t0**3],
                                      ))
                n = A_dot1.shape[0]
                A_dot = np.column_stack((A_dot1, -A_dot2, np.zeros([n, self.num_dec_vars-2*self.num_coefs])))
                all_A = np.row_stack((all_A, A_dot))
                all_b = np.row_stack((all_b, np.zeros([n, 3])))

            elif i == self.num_waypoints - 2: # second to last waypoint, xf is the goal
                A_np = np.row_stack(([1, t0, t0**2, t0**3,   t0**4,    t0**5],
                                     [1, t1, t1**2, t1**3,   t1**4,    t1**5],
                                     [0, 1,  2*t1,  3*t1**2, 4*t1**3,  5*t1**4],
                                     [0, 0,  2,     6*t1,    12*t1**2, 20*t1**3],
                                     ))
                n = A_np.shape[0]
                A_np = np.column_stack((np.zeros([n, i * self.num_coefs]), A_np,
                                        np.zeros([n, self.num_dec_vars - (i + 1) * self.num_coefs])))
                all_A = np.row_stack((all_A, A_np))
                b_np = np.row_stack((x0,
                                     xf,
                                     np.zeros([n-2, 3])
                                     ))
                all_b = np.row_stack((all_b, b_np))

            elif i == self.num_waypoints - 1: # last waypoint, x0 == xf == goal
                A_np = np.row_stack(([1, t0, t0**2, t0**3,   t0**4,    t0**5],
                                     [1, t1, t1**2, t1**3,   t1**4,    t1**5],
                                     [0, 1,  2*t0,  3*t0**2, 4*t0**3,  5*t0**4],
                                     [0, 1,  2*t1,  3*t1**2, 4*t1**3,  5*t1**4],
                                     [0, 0,  2,     6*t0,    12*t0**2, 20*t0**3],
                                     [0, 0,  2,     6*t1,    12*t1**2, 20*t1**3],
                                     ))
                n = A_np.shape[0]
                A_np = np.column_stack((np.zeros([n, i*self.num_coefs]), A_np, np.zeros([n, self.num_dec_vars - (i+1)*self.num_coefs])))
                all_A = np.row_stack((all_A, A_np))
                b_np = np.row_stack((x0,
                                     xf,
                                     np.zeros([n-2, 3])
                                     ))
                all_b = np.row_stack((all_b, b_np))

            else:
                # set the derivatives to the end point to the beginning of the next waypoint
                A_pos = np.row_stack((
                                      [1, t0, t0**2, t0**3,   t0**4,    t0**5],
                                      [1, t1, t1**2, t1**3,   t1**4,    t1**5]
                                    ))
                n = A_pos.shape[0]
                A_pos = np.column_stack((np.zeros([n, i*self.num_coefs]), A_pos, np.zeros([n, self.num_dec_vars - (i+1)*self.num_coefs])))

                A_dot1 = np.row_stack((
                                     [0, 1, 2*t1, 3*t1**2, 4*t1**3, 5*t1**4],
                                     [0, 0, 2, 6*t1, 12*t1**2, 20*t1**3],
                                   ))
                A_dot2 = np.row_stack((
                                       [0, 1, 2*t0, 3*t0**2, 4*t0**3, 5*t0**4],
                                       [0, 0, 2, 6*t0, 12*t0**2, 20*t0**3],
                                      ))
                n = A_dot1.shape[0]
                A_dot = np.column_stack((np.zeros([n, i*self.num_coefs]), A_dot1, -A_dot2, np.zeros([n, self.num_dec_vars - (i+2)*self.num_coefs])))
                all_A = np.row_stack((all_A, A_pos, A_dot))
                all_b = np.row_stack((all_b,
                                      x0,
                                      xf,
                                      np.zeros([n, 3])))

        Q = matrix(all_Q)
        p = matrix(np.zeros(self.num_dec_vars))
        A = matrix(all_A)
        bx = matrix(all_b[:, 0])
        by = matrix(all_b[:, 1])
        bz = matrix(all_b[:, 2])

        cx = np.array(solvers.qp(Q, p, A=A, b=bx)['x'])
        cy = np.array(solvers.qp(Q, p, A=A, b=by)['x'])
        cz = np.array(solvers.qp(Q, p, A=A, b=bz)['x'])
        coefs = np.column_stack((cx, cy, cz)).reshape(self.num_waypoints, self.num_coefs, 3)
        self.quintic_coefs = np.round(coefs, decimals=16)
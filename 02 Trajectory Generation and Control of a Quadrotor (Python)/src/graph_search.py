import numpy as np
import itertools
from heapq import heappush, heappop
import json

from flightsim.world import World
from proj1_3.code.occupancy_map import OccupancyMap # Recommended.

class PriorityQueue:
    def __init__(self):
        self.pq = []
        self.entry_finder = {}
        self.REMOVED = '<removed-node>'
        self.counter = itertools.count()

    def push(self, priority, idx):
        'Add a new task or update the priority of an existing task'
        if idx in self.entry_finder:
            self.remove(idx)

        node_id = next(self.counter)

        entry = [priority, node_id, idx]
        self.entry_finder[idx] = entry
        heappush(self.pq, entry)

    def remove(self, idx):
        'Mark an existing task as REMOVED.  Raise KeyError if not found.'
        entry = self.entry_finder.pop(idx)
        entry[-1] = self.REMOVED

    def pop(self):
        'Remove and return the lowest priority task. Raise KeyError if empty.'
        while self.pq:
            priority, node_id, idx = heappop(self.pq)
            if idx is not self.REMOVED:
                del self.entry_finder[idx]
                return idx
        raise KeyError('pop from an empty priority queue')


class NumpyEncoder(json.JSONEncoder):
    """ Special json encoder for numpy types """
    def default(self, obj):
        if isinstance(obj, (np.int_, np.intc, np.intp, np.int8,
            np.int16, np.int32, np.int64, np.uint8,
            np.uint16, np.uint32, np.uint64)):
            return int(obj)
        elif isinstance(obj, (np.float_, np.float16, np.float32,
            np.float64)):
            return float(obj)
        elif isinstance(obj,(np.ndarray,)): #### This is the fix
            return obj.tolist()
        return json.JSONEncoder.default(self, obj)


def is_valid_path(world, path, margin):
    return not world.path_collisions(path, margin).size

# def get_next_to_idxs():
#     next_to_idxs = np.zeros([26, 3])
#     m = 0
#     for i in range(-1, 2):
#         for j in range(-1, 2):
#             for k in range(-1, 2):
#                 if i is 0 and j is 0 and k is 0:
#                     continue
#                 next_to_idxs[m, :] = [i, j, k]
#                 m += 1
#     return next_to_idxs

# Constant to save time
def get_next_to_idxs():
    return np.array([[-1., -1., -1.],
       [-1., -1.,  0.],
       [-1., -1.,  1.],
       [-1.,  0., -1.],
       [-1.,  0.,  0.],
       [-1.,  0.,  1.],
       [-1.,  1., -1.],
       [-1.,  1.,  0.],
       [-1.,  1.,  1.],
       [ 0., -1., -1.],
       [ 0., -1.,  0.],
       [ 0., -1.,  1.],
       [ 0.,  0., -1.],
       [ 0.,  0.,  1.],
       [ 0.,  1., -1.],
       [ 0.,  1.,  0.],
       [ 0.,  1.,  1.],
       [ 1., -1., -1.],
       [ 1., -1.,  0.],
       [ 1., -1.,  1.],
       [ 1.,  0., -1.],
       [ 1.,  0.,  0.],
       [ 1.,  0.,  1.],
       [ 1.,  1., -1.],
       [ 1.,  1.,  0.],
       [ 1.,  1.,  1.]])

def dijkstra_search(occ_map, start, goal):
    map_boundary = occ_map.map.shape
    num_voxels = occ_map.map.size
    voxel2array = np.array([map_boundary[1] * map_boundary[2], map_boundary[2], 1])

    # Retrieve the index in the occupancy grid matrix corresponding to a position in space.
    voxel_start_index = occ_map.metric_to_index(start)
    array_start_index = voxel_start_index @ voxel2array

    voxel_goal_index = occ_map.metric_to_index(goal)
    array_goal_index = voxel_goal_index @ voxel2array

    # Commented out for speed
    # if occ_map.is_occupied_index(voxel_start_index) or occ_map.is_occupied_index(voxel_goal_index):
    #     return None

    # Begin algorithm here
    pq = PriorityQueue()
    open_voxel_idxs = np.arange(num_voxels)
    open_voxel_idxs = open_voxel_idxs[(~occ_map.map).reshape(-1)]

    cost_array = -1 * np.ones(num_voxels, )
    voxel_array = np.argwhere(~occ_map.map | occ_map.map).astype(int)
    metric_array = occ_map.index_to_metric_center(voxel_array)
    parent_array = -1 * np.ones(num_voxels, ).astype(int)
    in_queue_array = np.ones(num_voxels, ).astype(bool)

    cost_array[open_voxel_idxs] = np.inf
    cost_array[array_start_index] = 0
    for i in open_voxel_idxs:
        pq.push(cost_array[i], i)

    next_to_idxs = get_next_to_idxs()
    neighbor_distances = np.linalg.norm(next_to_idxs*occ_map.resolution, axis=1)
    u_idx = array_start_index
    zero3 = np.zeros(3, )
    while u_idx != array_goal_index and not np.isinf(pq.pq[0][0]):
        u_idx = pq.pop()
        in_queue_array[u_idx] = False

        neighbors = next_to_idxs + voxel_array[u_idx]
        is_neighbor = (np.less(neighbors, map_boundary) & np.greater_equal(neighbors, zero3)).all(axis=1)
        valid_neighbors = neighbors[is_neighbor].astype(int)
        neighbors_idx = np.dot(valid_neighbors, voxel2array)

        neighbors_cost = cost_array[neighbors_idx]
        open_neighbors = np.argwhere(neighbors_cost != -1).reshape(-1)
        open_neighbors_idx = neighbors_idx[open_neighbors]

        is_neighbor_in_queue = in_queue_array[open_neighbors_idx]
        open_neighbors_idx = open_neighbors_idx[is_neighbor_in_queue]

        neighbors_cost = cost_array[open_neighbors_idx]
        d = cost_array[u_idx] + (neighbor_distances[is_neighbor])[open_neighbors[is_neighbor_in_queue]]
        is_better_d = d < neighbors_cost

        better_d_idx = open_neighbors_idx[is_better_d]
        cost_array[better_d_idx] = d[is_better_d]
        parent_array[better_d_idx] = u_idx
        for i in better_d_idx:
            pq.push(cost_array[i], i)

    return parent_array, array_goal_index, metric_array

def astar_search(occ_map, start, goal):
    map_boundary = occ_map.map.shape
    num_voxels = occ_map.map.size
    voxel2array = np.array([map_boundary[1] * map_boundary[2], map_boundary[2], 1])

    # Retrieve the index in the occupancy grid matrix corresponding to a position in space.
    voxel_start_index = occ_map.metric_to_index(start)
    array_start_index = voxel_start_index @ voxel2array

    voxel_goal_index = occ_map.metric_to_index(goal)
    array_goal_index = voxel_goal_index @ voxel2array

    # Commented out for speed
    # if occ_map.is_occupied_index(voxel_start_index) or occ_map.is_occupied_index(voxel_goal_index):
    #     return None

    # Begin algorithm here
    pq = PriorityQueue()
    open_voxel_idxs = np.arange(num_voxels)
    open_voxel_idxs = open_voxel_idxs[(~occ_map.map).reshape(-1)]

    cost_array = -1 * np.ones(num_voxels,)
    voxel_array = np.argwhere(~occ_map.map|occ_map.map).astype(int)
    metric_array = occ_map.index_to_metric_center(voxel_array)
    heuristic_array = np.linalg.norm(goal - metric_array, axis=1)
    parent_array = -1 * np.ones(num_voxels,).astype(int)
    in_queue_array = np.ones(num_voxels,).astype(bool)

    cost_array[open_voxel_idxs] = np.inf
    cost_array[array_start_index] = 0
    for i in open_voxel_idxs:
        pq.push(cost_array[i], i)

    next_to_idxs = get_next_to_idxs()
    neighbor_distances = np.linalg.norm(next_to_idxs*occ_map.resolution, axis=1)
    u_idx = array_start_index
    zero3 = np.zeros(3,)
    while u_idx != array_goal_index and not np.isinf(pq.pq[0][0]):
        u_idx = pq.pop()
        in_queue_array[u_idx] = False

        neighbors = next_to_idxs + voxel_array[u_idx]
        is_neighbor = (np.less(neighbors, map_boundary) & np.greater_equal(neighbors, zero3)).all(axis=1)
        valid_neighbors = neighbors[is_neighbor].astype(int)
        neighbors_idx = np.dot(valid_neighbors, voxel2array)

        neighbors_cost = cost_array[neighbors_idx]
        open_neighbors = np.argwhere(neighbors_cost != -1).reshape(-1)
        open_neighbors_idx = neighbors_idx[open_neighbors]

        is_neighbor_in_queue = in_queue_array[open_neighbors_idx]
        open_neighbors_idx = open_neighbors_idx[is_neighbor_in_queue]

        neighbors_cost = cost_array[open_neighbors_idx]
        d = cost_array[u_idx] + (neighbor_distances[is_neighbor])[open_neighbors[is_neighbor_in_queue]]
        is_better_d = d < neighbors_cost

        better_d_idx = open_neighbors_idx[is_better_d]
        cost_array[better_d_idx] = d[is_better_d]
        parent_array[better_d_idx] = u_idx
        for i in better_d_idx:
            pq.push(cost_array[i] + heuristic_array[i], i)

    return parent_array, array_goal_index, metric_array

def graph_search(world, resolution, margin, start, goal, astar):
    """
    Parameters:
        world,      World object representing the environment obstacles
        resolution, xyz resolution in meters for an occupancy map, shape=(3,)
        margin,     minimum allowed distance in meters from path to obstacles.
        start,      xyz position in meters, shape=(3,)
        goal,       xyz position in meters, shape=(3,)
        astar,      if True use A*, else use Dijkstra
    Output:
        path,       xyz position coordinates along the path in meters with
                    shape=(N,3). These are typically the centers of visited
                    voxels of an occupancy map. The first point must be the
                    start and the last point must be the goal. If no path
                    exists, return None.
    """
    # Commented out for speed
    # if np.array_equal(start, goal):
    #     return start

    # Commented out for speed
    # path = np.array([start, goal])
    # if is_valid_path(world, path, margin):
    #     return path

    # While not required, we have provided an occupancy map you may use or modify.
    occ_map = OccupancyMap(world, resolution, margin)
    if astar:
        parent_array, array_goal_index, metric_array = astar_search(occ_map, start, goal)
    else:
        parent_array, array_goal_index, metric_array = dijkstra_search(occ_map, start, goal)

    if parent_array[array_goal_index] == -1:
        return None

    path = [goal]
    p = parent_array[array_goal_index]
    while p != -1:
        v_center = metric_array[p]
        path.append(v_center)
        p = parent_array[p]

    path.append(start)
    path = np.flipud(path)
    return path
import heapq
from Environment import Environment
from Environment import Cell
import pdb

class Node:
    def __init__(self, row, column, parent, g, h, f, tie):
        self.row = row
        self.column = column
        self.parent = parent
        self.g = g
        self.h = h
        self.f = f
        self.tie = tie

    def __lt__(self, other):
        if self.f != other.f:
            return self.f < other.f
        else:
            if self.tie == 0:
                return self.g < other.g
            else:
                return self.g > other.g

    def __eq__(self, other):
        return self.row == other.row and self.column == other.column


class Coord:
    def __init__(self, row, column):
        self.row = row
        self.column = column
    def __eq__(self, other):
        return self.row == other.row and self.column == other.column


def forward_a(start, target, env):
    open_list = []
    closed_list = []
    check = False
    heapq.heapify(open_list)
    start.h = abs((start.row - target.row)) + abs((start.column - target.column))
    start.g = 0
    start.f = start.h + start.g
    start.parent = None
    start.tie = None
    heapq.heappush(open_list, start)
    while len(open_list) != 0:
        curr_node = heapq.heappop(open_list)
        if curr_node.row == target.row and curr_node.column == target.column:
            check = True
            break
        temp_cell = env.environment_map[curr_node.row][curr_node.column]
        neighbors = env.expand_cell(temp_cell)
        cell = Coord(curr_node.row, curr_node.column)
        closed_list.append(cell)
        for k in neighbors:
            temp = k
            temp_node = Node(temp.row, temp.column, None, None, None, None, None)
            if temp_node not in closed_list:
                if temp_node not in open_list:
                    temp_node.h = abs((temp_node.row - target.row)) + abs((temp_node.column - target.column))
                    temp_node.g = curr_node.g + 1
                    temp_node.f = temp_node.g + temp_node.h
                    heapq.heappush(open_list, temp_node)
                    temp_node.parent = curr_node
                else:
                    temp_g = curr_node.g + 1 #TODO: Make temp_node.g reference already created object
                    for m in open_list:
                        if temp_node == m:
                            temp_node = m
                    if temp_g < temp_node.g:
                        temp_node.g = temp_g
                        temp_node.f = temp_node.g + temp_node.h
                        temp_node.parent = curr_node
                        heapq.heapify(open_list)
    if check:
        result_list = []
        while curr_node is not None:
            result_list.append(curr_node)
            curr_node = curr_node.parent
        result_list.reverse()
        return result_list, closed_list
    else:
        return None, None
    
def diff_tie_break(start, target, env):
    open_list = []
    closed_list = []
    check = False
    heapq.heapify(open_list)
    start.h = abs((start.row - target.row)) + abs((start.column - target.column))
    start.g = 0
    start.f = start.h + start.g
    start.parent = None
    start.tie = 0
    heapq.heappush(open_list, start)
    while len(open_list) != 0:
        curr_node = heapq.heappop(open_list)
        if curr_node.row == target.row and curr_node.column == target.column:
            check = True
            break
        temp_cell = env.environment_map[curr_node.row][curr_node.column]
        neighbors = env.expand_cell(temp_cell)
        cell = Coord(curr_node.row, curr_node.column)
        closed_list.append(cell)
        for k in neighbors:
            temp = k
            temp_node = Node(temp.row, temp.column, None, None, None, None, 0)
            if temp_node not in closed_list:
                if temp_node not in open_list:
                    temp_node.h = abs((temp_node.row - target.row)) + abs((temp_node.column - target.column))
                    temp_node.g = curr_node.g + 1
                    temp_node.f = temp_node.g + temp_node.h
                    heapq.heappush(open_list, temp_node)
                    temp_node.parent = curr_node
                else:
                    temp_g = curr_node.g + 1 
                    for m in open_list:
                        if temp_node == m:
                            temp_node = m
                    if temp_g < temp_node.g:
                        temp_node.g = temp_g
                        temp_node.f = temp_node.g + temp_node.h
                        temp_node.parent = curr_node
                        heapq.heapify(open_list)
    if check:
        result_list = []
        while curr_node is not None:
            result_list.append(curr_node)
            curr_node = curr_node.parent
        result_list.reverse()
        return result_list, closed_list
    else:
        return None, None
"""For adaptive A*, must run normal A* once, and if path becomes
    blocked at some point, run adaptive A* with g_goal being equal to
    the distance in nodes from the agent's current state to the goal
    state according to the shortest path given by the previous A* run
    (whether that be normal or adaptive is irrelevant).  expanded_list
    is equal to the closed_list returned by the previous A* run since
    the heuristics is only being changed for expanded nodes."""
def adaptive_a(start, target, g_goal, expanded_list, env):
    open_list = []
    closed_list = []
    check = False
    heapq.heapify(open_list)
    start.h = g_goal
    start.g = 0
    start.f = start.h + start.g
    start.parent = None
    start.tie = None
    heapq.heappush(open_list, start)
    while len(open_list) != 0:
        curr_node = heapq.heappop(open_list)
        if curr_node.row == target.row and curr_node.column == target.column:
            check = True
            break
        temp_cell = env.environment_map[curr_node.row][curr_node.column]
        neighbors = env.expand_cell(temp_cell)
        cell = Coord(curr_node.row, curr_node.column)
        closed_list.append(cell)
        for k in neighbors:
            temp = k
            temp_node = Node(temp.row, temp.column, None, None, None, None, None)
            if temp_node not in closed_list:
                if temp_node not in open_list:
                    if temp_node in expanded_list:
                        temp_node.g = curr_node.g + 1
                        temp_node.h = g_goal - temp_node.g
                        temp_node.f = temp_node.g + temp_node.h
                        heapq.heappush(open_list, temp_node)
                        temp_node.parent = curr_node
                    else:
                        temp_node.g = curr_node.g + 1
                        temp_node.h = abs((temp_node.row - target.row)) + abs((temp_node.column - target.column))
                        temp_node.f = temp_node.g + temp_node.h
                        heapq.heappush(open_list, temp_node)
                        temp_node.parent = curr_node
                else:
                    temp_g = curr_node.g + 1
                    for m in open_list:
                        if temp_node == m:
                            temp_node = m
                    if temp_g < temp_node.g:
                        if temp_node in expanded_list:
                            temp_node.g = temp_g
                            temp_node.h = g_goal - temp_node.g
                            temp_node.f = temp_node.g + temp_node.h
                            temp_node.parent = curr_node
                        else:
                            temp_node.g = temp_g
                            temp_node.f = temp_node.g + temp_node.h
                            temp_node.parent = curr_node
                        heapq.heapify(open_list)
    if check:
        result_list = []
        while curr_node is not None:
            result_list.append(curr_node)
            curr_node = curr_node.parent
        result_list.reverse()
        return result_list, closed_list
    else:
        return None, None

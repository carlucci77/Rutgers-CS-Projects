import random
import Environment
import Search
import time
import shelve

def r_for_a(env, agent, target):
    env.reveal_cells(agent)
    count = 0
    agent_node = Search.Node(agent.x, agent.y, None, None, None, None, None)
    tar_node = Search.Node(target.x, target.y, None, None, None, None, None)
    start = time.time()
    prev_cell = None
    check = False
    while check is False:     
        path, expand_list = Search.forward_a(agent_node, tar_node, env)
        if path is None:
            break
        count += len(expand_list)
        for k in path:
            x = k.row
            y = k.column
            pos_cell = env.environment_map[x][y]
            if pos_cell.blocked is False:
                if pos_cell != agent and pos_cell != target:
                    if pos_cell.representation == ".":
                        while prev_cell != pos_cell:
                            prev_cell.representation = " "
                            parent = prev_cell.parent
                            prev_cell.parent = None
                            prev_cell = parent
                    else:
                        pos_cell.parent = prev_cell    
                        pos_cell.representation = "."
                agent_node.row = x
                agent_node.column = y
                if(agent_node.row == tar_node.row and agent_node.column == tar_node.column):
                    check = True
                    break
                else:
                    env.reveal_cells(pos_cell)
                prev_cell = pos_cell
            else:
                break
    end = time.time()
    return count, round(end - start, 3)

def diff_r_for_a(env, agent, target):
    env.reveal_cells(agent)
    count = 0
    agent_node = Search.Node(agent.x, agent.y, None, None, None, None, None)
    tar_node = Search.Node(target.x, target.y, None, None, None, None, None)
    start = time.time()
    prev_cell = None
    check = False
    while check is False:     
        path, expand_list = Search.diff_tie_break(agent_node, tar_node, env)
        if path is None:
            break
        count += len(expand_list)
        for k in path:
            x = k.row
            y = k.column
            pos_cell = env.environment_map[x][y]
            if pos_cell.blocked is False:
                if pos_cell != agent and pos_cell != target:
                    if pos_cell.representation == ".":
                        while prev_cell != pos_cell:
                            prev_cell.representation = " "
                            parent = prev_cell.parent
                            prev_cell.parent = None
                            prev_cell = parent
                    else:
                        pos_cell.parent = prev_cell    
                        pos_cell.representation = "."
                agent_node.row = x
                agent_node.column = y
                if(agent_node.row == tar_node.row and agent_node.column == tar_node.column):
                    check = True
                    break
                else:
                    env.reveal_cells(pos_cell)
                prev_cell = pos_cell
            else:
                break
    end = time.time()
    return count, round(end - start, 3)

def r_back_a(env, agent, target):
    env.reveal_cells(agent)
    count = 0
    agent_node = Search.Node(agent.x, agent.y, None, None, None, None, None)
    tar_node = Search.Node(target.x, target.y, None, None, None, None, None)
    start = time.time()
    prev_cell = None
    check = False
    while check is False:     
        path, expand_list = Search.forward_a(tar_node, agent_node, env)
        if path is None:
            break
        count += len(expand_list)
        path.reverse()
        for k in path:
            x = k.row
            y = k.column
            pos_cell = env.environment_map[x][y]
            if pos_cell.blocked is False:
                if pos_cell != agent and pos_cell != target:
                    if pos_cell.representation == ".":
                        while prev_cell != pos_cell:
                            prev_cell.representation = " "
                            parent = prev_cell.parent
                            prev_cell.parent = None
                            prev_cell = parent
                    else:
                        pos_cell.parent = prev_cell    
                        pos_cell.representation = "."
                agent_node.row = x
                agent_node.column = y
                if(agent_node.row == tar_node.row and agent_node.column == tar_node.column):
                    check = True
                    break
                else:
                    env.reveal_cells(pos_cell)
                prev_cell = pos_cell
            else:
                break
    end = time.time()
    return count, round(end - start, 3)

def r_adap_a(env, agent, target):
    env.reveal_cells(agent)
    count = 0
    agent_node = Search.Node(agent.x, agent.y, None, None, None, None, None)
    tar_node = Search.Node(target.x, target.y, None, None, None, None, None)
    start = time.time()
    prev_cell = None
    expand_list = None
    path = None
    search_func = 0
    check = False
    while check is False:
        g_goal = -1
        if search_func == 0 :    
            path, expand_list = Search.forward_a(agent_node, tar_node, env)
            expanded_list = expand_list
            search_func = 1
        else:
            path, expand_list = Search.adaptive_a(agent_node, tar_node, g_goal, expanded_list, env)
            set_1 = set(expanded_list)
            set_2 = set(expand_list)
            set_diff = set_1 - set_2
            expanded_list = expanded_list + list(set_diff)
        if path is None:
            break    
        g_goal += len(path)
        count += len(expand_list)
        for k in path:
            x = k.row
            y = k.column
            pos_cell = env.environment_map[x][y]
            if pos_cell.blocked is False:
                if pos_cell != agent and pos_cell != target:
                    if pos_cell.representation == ".":
                        while prev_cell != pos_cell:
                            prev_cell.representation = " "
                            parent = prev_cell.parent
                            prev_cell.parent = None
                            prev_cell = parent
                    else:
                        pos_cell.parent = prev_cell    
                        pos_cell.representation = "."
                agent_node.row = x
                agent_node.column = y
                if(agent_node.row == tar_node.row and agent_node.column == tar_node.column):
                    check = True
                    break
                else:
                    env.reveal_cells(pos_cell)
                    prev_cell = pos_cell
            else:
                break
    end = time.time()
    return count, round(end - start, 3)

def make_worlds():
    worlds = shelve.open("Environments")
    for count in range(50):
        env = Environment.Environment(random.randint(0, 100),random.randint(0,100))
        env.create_env(env.environment_map[env.row][env.column])
        worlds[str(count)] = env


worlds = shelve.open("Environments")
for_a_count = 0
diff_for_a_count = 0
back_a_count = 0
adap_a_count = 0
for_a_time = 0
diff_for_a_time = 0
back_a_time = 0
adap_a_time = 0
for k in range(50):
    print(str(k))
    env = worlds[str(k)]
    agent, target = env.agent_and_target()
    temp_count, temp_time = r_for_a(env, agent, target)
    for_a_count += temp_count
    for_a_time += temp_time
    env.clear_cells()
    temp_count, temp_time = diff_r_for_a(env, agent, target)
    diff_for_a_count += temp_count
    diff_for_a_time += temp_time
    env.clear_cells()
    temp_count, temp_time = r_back_a(env, agent, target)
    back_a_count += temp_count
    back_a_time += temp_time
    env.clear_cells()
    temp_count, temp_time = r_adap_a(env, agent, target)
    adap_a_count += temp_count
    adap_a_time += temp_time
    env.clear_cells()
avg_time = str(round(for_a_time/50, 3))
avg_count = str(for_a_count/50)
print("Forward A* took " + avg_time + " seconds on average and expanded " + avg_count + " cells on average.")
avg_time = str(round(diff_for_a_time/50, 3))
avg_count = str(diff_for_a_count/50)
print("Forward A* with different tie breaks took " + avg_time + " seconds on average and expanded " + avg_count + " cells on average.")
avg_time = str(round(back_a_time/50, 3))
avg_count = str(back_a_count/50)
print("Backward A* took " + avg_time + " seconds on average and expanded " + avg_count + " cells on average.")
avg_time = str(round(adap_a_time/50, 3))
avg_count = str(adap_a_count/50)
print("Adaptive A* took " + avg_time + " seconds on average and expanded " + avg_count + " cells on average.")

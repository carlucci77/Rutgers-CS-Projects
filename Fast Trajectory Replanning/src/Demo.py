import random
import Environment
import Search
import pdb
def run(env, agent, target):
    env.reveal_cells(agent)
    big_loop = True
    while big_loop:
        search_type = int (input("Which search would you like to do? (Forward A* [1], Backward A* [2], Adaptive A* [3])"))
        check = False
        loop = True
        count = 0
        if search_type == 1:
            agent_node = Search.Node(agent.x, agent.y, None, None, None, None, None)
            tar_node = Search.Node(target.x, target.y, None, None, None, None, None)
            while loop:
                tie_type = int (input("Which way would you like to break ties? (Greater G-Value [1], Smaller G-Value [2])"))
                if tie_type == 1:
                    prev_cell = None
                    while check is False:     
                        path, expand_list = Search.forward_a(agent_node, tar_node, env)
                        if path is None:
                            break
                        for l in expand_list:
                            count += 1
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
                    if check:
                        print("Congratulations! The agent made its way to the target in " + " seconds and expanded " + str(count) + " cells using Forward A*.")
                    else:
                        print("Unfortunately, the agent could not find its way to the target using Forward A*. The agent expanded " + str(count) + " cells and discovered the blocked path in " + " seconds.")
                    loop = False
                    big_loop = False
                    env.print_env()
                elif tie_type == 2:
                    prev_cell = None
                    while check is False:     
                        path, expand_list = Search.diff_tie_break(agent_node, tar_node, env)
                        if path is None:
                            break
                        for l in expand_list:
                            count += 1
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
                    if check:
                        print("Congratulations! The agent made its way to the target in " + " seconds and expanded " + str(count) + " cells using Forward A*.")
                    else:
                        print("Unfortunately, the agent could not find its way to the target using Forward A*. The agent expanded " + str(count) + " cells and discovered the blocked path in " + " seconds.")
                    loop = False
                    big_loop = False
                    env.print_env()
                else:
                    print("Error: Please enter valid input")
        elif search_type == 2:
            agent_node = Search.Node(agent.x, agent.y, None, None, None, None, None)
            tar_node = Search.Node(target.x, target.y, None, None, None, None, None)
            prev_cell = None
            while check is False:     
                path, expand_list = Search.forward_a(tar_node, agent_node, env)
                if path is None:
                    break
                for l in expand_list:
                    count += 1
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
            if check:
                print("Congratulations! The agent made its way to the target in " + " seconds and expanded " + str(count) + " cells using Backward A*.")
            else:
                print("Unfortunately, the agent could not find its way to the target using Backward A*. The agent expanded " + str(count) + " cells and discovered the blocked path in " + " seconds.")
            big_loop = False
            env.print_env()
        elif search_type == 3:
            agent_node = Search.Node(agent.x, agent.y, None, None, None, None, None)
            tar_node = Search.Node(target.x, target.y, None, None, None, None, None)
            prev_cell = None
            expand_list = None
            path = None
            search_func = 0
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
                for j in path:
                    g_goal += 1
                for l in expand_list:
                    count += 1
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
            if check:
                print("Congratulations! The agent made its way to the target in " + " seconds and expanded " + str(count) + " cells using Adaptive A*.")
            else:
                print("Unfortunately, the agent could not find its way to the target using Adaptive A*. The agent expanded " + str(count) + " cells and discovered the blocked path in " + " seconds.")
            big_loop = False
            env.print_env()
        else:
            print("Error: Please enter valid input")

test = Environment.Environment(random.randint(0, 100),random.randint(0,100))
test.create_env(test.environment_map[test.row][test.column])
agent, target = test.agent_and_target()
run(test, agent, target)

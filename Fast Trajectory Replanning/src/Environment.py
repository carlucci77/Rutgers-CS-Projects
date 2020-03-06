import random
import tkinter

class Coord:
    def __init__(self, row, column):
        self.row = row
        self.column = column
                  
class Cell:
    def __init__(self, x, y, representation, blocked, seen, visited, parent):
        self.x = x
        self.y = y
        self.representation = representation
        self.blocked = blocked
        self.seen = seen
        self.visited = visited
        self.parent = parent
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y
        
class Environment:
    def __init__(self, row, column):
        self.row = row
        self.column = column
        self.environment_map = [[Cell(x, y, " ", False, False, False, None) for y in range(101)] for x in range(101)]
                  
    def visit_cells(self, cell):
        neighbors = []
        if cell.y + 1 in range(101):
            north = (self.environment_map[cell.x][cell.y + 1])
            if north.visited is False:
                neighbors.append(north)
        if cell.y - 1 in range(101):
            south = (self.environment_map[cell.x][cell.y - 1])
            if south.visited is False:
                neighbors.append(south)
        if cell.x + 1 in range(101):
            east = (self.environment_map[cell.x + 1][cell.y])
            if east.visited is False:
                neighbors.append(east)
        if cell.x - 1 in range(101):
            west = (self.environment_map[cell.x - 1][cell.y])
            if west.visited is False:
                neighbors.append(west)
        return neighbors
    
    def reveal_cells(self, cell):
        if cell.y + 1 in range(101):
            north = (self.environment_map[cell.x][cell.y + 1])
            north.seen = True
        if cell.y - 1 in range(101):
            south = (self.environment_map[cell.x][cell.y - 1])
            south.seen = True
        if cell.x + 1 in range(101):
            east = (self.environment_map[cell.x + 1][cell.y])
            east.seen = True
        if cell.x - 1 in range(101):
            west = (self.environment_map[cell.x - 1][cell.y])
            west.seen = True
            
    def expand_cell(self, cell):
        neighbors = []
        if cell.y + 1 in range(101):
            north = (self.environment_map[cell.x][cell.y + 1])
            if north.seen is False:
                neighbors.append(Coord(north.x,north.y))
            else:
                if north.blocked is False:
                    neighbors.append(Coord(north.x,north.y))   
        if cell.y - 1 in range(101):
            south = (self.environment_map[cell.x][cell.y - 1])
            if south.seen is False:
                neighbors.append(Coord(south.x,south.y))
            else:
                if south.blocked is False:
                    neighbors.append(Coord(south.x,south.y)) 
        if cell.x + 1 in range(101):
            east = (self.environment_map[cell.x + 1][cell.y])
            if east.seen is False:
                neighbors.append(Coord(east.x,east.y))
            else:
                if east.blocked is False:
                    neighbors.append(Coord(east.x,east.y)) 
        if cell.x - 1 in range(101):
            west = (self.environment_map[cell.x - 1][cell.y])
            if west.seen is False:
                neighbors.append(Coord(west.x,west.y))
            else:
                if west.blocked is False:
                    neighbors.append(Coord(west.x,west.y)) 
        return neighbors
                   
    def create_env(self, start_cell):
        start_cell.visited = True
        start_cell.blocked = False
        stack = []
        curr_cell = start_cell
        check = True
        stack.append(curr_cell)
        while not stack:
            if check:
                check = False
                stack.pop()
            neighbors = self.visit_cells(curr_cell)
            if not neighbors:
                curr_cell = stack.pop()
                continue
            next_cell = random.choice(neighbors)
            next_cell.visited = True
            block_prob = random.randint(0, 100)
            if block_prob < 30:
                next_cell.blocked = True
                next_cell.representation = "H"
            else:
                stack.append(curr_cell)
                curr_cell = next_cell
        for k in range(101):
            for i in range(101):
                curr_cell = self.environment_map[k][i]
                if curr_cell.visited is False:
                    curr_cell.visited = True
                    block_prob = random.randint(0, 100)
                    if block_prob < 30:
                        curr_cell.blocked = True
                        curr_cell.representation = "H"
                    else:
                        stack.append(curr_cell)
                        while not stack:
                            neighbors = self.visit_cells(curr_cell)
                            if not neighbors:
                                curr_cell = stack.pop()
                                continue
                            next_cell = random.choice(neighbors)
                            next_cell.visited = True
                            block_prob = random.randint(0, 100)
                            if block_prob < 30:
                                next_cell.blocked = True
                                next_cell.representation = "H"
                            else:
                                stack.append(curr_cell)
                                curr_cell = next_cell
                                
    def print_env(self):
        window = tkinter.Tk()
        window.title("Fast Trajectory Replanning")
        canvas = tkinter.Canvas(window, width=1010, height=1010, background='white')
        canvas.grid(row=0, column=0)
        for y in range(101):
            for x in range(101):
                representation = self.environment_map[x][y].representation
                if representation == 'H':
                    canvas.create_rectangle(x * 10, y * 10, (x + 1) * 10, (y + 1) * 10, fill='#000')
                elif representation == 'A':
                    canvas.create_rectangle(x * 10, y * 10, (x + 1) * 10, (y + 1) * 10, fill='#00F')
                elif representation == 'T':
                    canvas.create_rectangle(x * 10, y * 10, (x + 1) * 10, (y + 1) * 10, fill='#F00')
                elif representation == '.':
                    canvas.create_rectangle(x * 10, y * 10, (x + 1) * 10, (y + 1) * 10, fill='#FF0')
                    
        window.mainloop()
        
    def agent_and_target(self):
        check = True
        agent = self.environment_map[0][0]
        target = self.environment_map[1][1]
        while(check):
            x = random.randint(0, 100)  
            y = random.randint(0,100)
            if self.environment_map[x][y].blocked is False:
                check = False
                self.environment_map[x][y].representation = 'A'
                agent = self.environment_map[x][y] 
        while(not check):
            x = random.randint(0, 100)  
            y = random.randint(0,100)
            if self.environment_map[x][y].blocked is False and self.environment_map[x][y].representation != 'A':
                check = True
                self.environment_map[x][y].representation = 'T'
                target = self.environment_map[x][y]
        return agent, target
    
    def diff_agent_and_target(self):
        agent = self.environment_map[0][100]
        target = self.environment_map[100][0]
        agent.representation = 'A' 
        agent.blocked = False
        target.representation = 'T'
        target.blocked = False
        return agent, target
    
    def clear_cells(self):
        for x in range(101):
            for y in range(101):
                cell = self.environment_map[x][y]
                cell.seen = False
                cell.parent = None
                if cell.representation == ".":
                    cell.representation = " "        

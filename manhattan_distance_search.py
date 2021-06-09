import re
import copy
import time

# Uniform Cost Search
# A* with misplaced tile
# A* with Manhattan Distance

# Solution to the puzzle.
GOAL_STATE = [['1', '2', '3'], ['4', '5', '6'], ['7', '8', '0']]

class Problem:
    #Initializations
    def __init__(self, puzzle):
        self.INITIAL_STATE = puzzle
        self.CURRENT_STATE = self.INITIAL_STATE
        self.history = []
        self.GN = 0
        self.history = []

#----------------------------------------------------------------Operators-------------------------------------------------------------------
    #Description: Operators will move the blank slate in the corresponding direction and return a NEW node
    #Steps:
    #1. Find zero
    #2. Swap zero
    def moveLeft(self):
        new_state =   copy.deepcopy(self.CURRENT_STATE)
        for words in self.CURRENT_STATE:
            for word in words:
                if word == 0:
                    word.position 
                #print(word)
        
        row = 0
        col = 0
        found = False
        for num, words in enumerate(self.CURRENT_STATE,start=0):
            for nums, word in enumerate(words,start=0):
                if word == '0':
                    row = num
                    col = nums
                    found = True
                #print(str(num) +"."+ str(nums) +": "+ word)
            if found:
                #If the blank slate is on the very left, return original
                if col == 0:
                    return -1

                #Swaps the variables
                new_state[row][col], new_state[row][col-1] = new_state[row][col-1], new_state[row][col]
                #print("New Moved Left State: " +str(new_state))
                return(new_state)
                break
        
    #Move Right
    def moveRight(self):
        new_state = copy.deepcopy(self.CURRENT_STATE)
        for words in self.CURRENT_STATE:
            for word in words:
                if word == 0:
                    word.position 
                #print(word)
        
        row = 0
        col = 0
        found = False
        for num, words in enumerate(self.CURRENT_STATE,start=0):
            for nums, word in enumerate(words,start=0):
                if word == '0':
                    row = num
                    col = nums
                    found = True
                #print(str(num) +"."+ str(nums) +": "+ word)
            if found:
                #If the blank slate is on the very right, return original
                if col == 2:
                    #print("Cannot move right. Returning original function")
                    return -1

                #Swaps the variables
                new_state[row][col], new_state[row][col+1] = new_state[row][col+1], new_state[row][col]
                #print("New Moved Right State: " +str(new_state))
                return(new_state)
                break
        
    #Move up
    def moveUp(self):
        new_state = copy.deepcopy(self.CURRENT_STATE)
        for words in self.CURRENT_STATE:
            for word in words:
                if word == 0:
                    word.position 
                #print(word)
        
        row = 0
        col = 0
        found = False
        for num, words in enumerate(self.CURRENT_STATE,start=0):
            for nums, word in enumerate(words,start=0):
                if word == '0':
                    row = num
                    col = nums
                    found = True
                #print(str(num) +"."+ str(nums) +": "+ word)
            if found:
                #If the blank slate is on the very top, return original
                if row == 0:
                    #print("Cannot move up. Returning original function")
                    return -1

                #Swaps the variables
                new_state[row][col], new_state[row-1][col] = new_state[row-1][col], new_state[row][col]
                #print("New Moved Up State: " +str(new_state))
                return(new_state)
                break

    #Move Down
    def moveDown(self):
        new_state = copy.deepcopy(self.CURRENT_STATE)
        for words in self.CURRENT_STATE:
            for word in words:
                if word == 0:
                    word.position 
                #print(word)
        
        row = 0
        col = 0
        found = False
        for num, words in enumerate(self.CURRENT_STATE,start=0):
            for nums, word in enumerate(words,start=0):
                if word == '0':
                    row = num
                    col = nums
                    found = True
                #print(str(num) +"."+ str(nums) +": "+ word)
            if found:
                #If the blank slate is on the very bottom, return original 
                if row == 2:
                    #print("Cannot move down. Returning original function")
                    return -1

                #Swaps the variables
                new_state[row][col], new_state[row+1][col] = new_state[row+1][col], new_state[row][col]
                #print("New Moved Down State: " +str(new_state))
                return(new_state)

#------------------------------------------------------------Helper Class Functions------------------------------------------------------------

    def print(self):
        #print("\n")
        for words in self.CURRENT_STATE:
            print(words)

    def goal_test(self):
        #Checks if the current puzzle is the goal state
        if self.CURRENT_STATE == GOAL_STATE:
            print("REACHED GOAL STATE")
            return True
        return False

    def getCurrentState(self):
        return self.CURRENT_STATE

    def getInitialState(self):
        return self.INITIAL_STATE

    
    #Keeps track and maintains the g(n) value for each node
    def addGN(self, hn):
        self.GN = self.GN + hn

    def getGN(self):
        return self.GN

    def setGN(self, GN):
        self.GN = GN


    #--------------------------------------------------------------------History-----------------------------------------------------------------
    #Description: Returns the history of the node
    def getHistory(self):
        return self.history


    #Description: Copies the prev node history and adds the current operator being used on it
    def setHistory(self, history, operator):
        history.append(operator)
        self.history = history.copy()
        #print(self.history)
        #self.printHistory()

    #Description: Prints the past operators that have been done on the current node 
    def printHistory(self):
        for operators in self.history:
            print(operators)
        #print(self.history)

    def getHistorySize(self):
        return len(self.history) - 1

    def printResults(self, initialState):
        print("Initial State: ")
        initialState.print()
        currState = copy.deepcopy(initialState)
        print("\nBelow is the trace of the solution")
        for operator in self.history:
            if (operator == "start"):
                continue
            print(operator)
            if (operator == "move 0 left"):
                currState = Problem(currState.moveLeft())
            if (operator == "move 0 right"):
                currState = Problem(currState.moveRight())
            if (operator == "move 0 up"):
                currState = Problem(currState.moveUp())
            if (operator == "move 0 down"):
                currState = Problem(currState.moveDown())
            currState.print()
            print("\n")


#Description: Keeps track of nodes via their heuristic v
class Queue:
    def __init__(self):
        #Initial states
        self.max_size = 0
        self.queue = []
        self.visitedList = []
        self.itemsInQueue = 0
        self.heuristicFunction = "uniform_cost_search" #By Default

    #Description: Checks if the node being added has been visited, then adds the node into the queue
    def add(self, node):
        if(self.isVisited(node)):
            #print("Node you're attempting to add to queue has been visited. . ")
            return 0

        #Adds to the end of the queue
        #self.queue.append((calculateHeristics(node, self.heuristicFunction), node))
        #hn = calculateHeristics(node, self.heuristicFunction)
        hn = 0
        self.queue.append((node.getGN(), node))

        #History
        #history = copy.deepcopy(node.getHistory())
        #node.setHistory(history, operator)

        #Max size counter
        self.itemsInQueue = self.itemsInQueue + 1
        if (self.itemsInQueue > self.max_size):
            self.max_size = self.itemsInQueue
        return hn

    #Description: Removes the first node in the queue, adds to visited then returns it
    def remove_front(self):
        try:
            firstItem = self.queue.pop(0)
        except:
            print("Error: Queue is empty. Cannot remove node")
            exit(0)
        self.visitedList.append(firstItem[1])
        self.itemsInQueue = self.itemsInQueue - 1
        return firstItem

    #Description: Prints the current list
    def printList(self):
        #print(self.queue)
        for node in self.queue:
            print("F(n): " + str(node[0]))
            node[1].print()

    #Description: Returns the max size of the queue
    def getMaxSize(self):
        return self.max_size
    
    #Description: Checks if the node parameter is in the visited list
    def isVisited(self, node):
        #print("Checking if node has been visited. . .")
        #self.printVisitedList()
        for visited_node in self.visitedList:
            if visited_node.getCurrentState() == node.getCurrentState():
                #print("Node has been visited")
                #visited_node.print()
                #print("Printing visited list. . . ")
                #self.printVisitedList()
                return True
        return False

    def setHeuristicFunction(self, heuristic_function):
        self.heuristicFunction = heuristic_function

    #------------------------------------------------Tester Functions---------------------------------------------------------------------------
    #Description: Prints visited list
    def printVisitedList(self):
        for visited_node in self.visitedList:
            print("\n")
            visited_node.print()

    def print(self):
        print("Printing queue. . .")
        for node in self.queue:
            print("Heuristic Value: " + str(node[0]))
            node[1].print()
            print("\n")

    def sort(self):
        #print("\nSorting queue. . .")
        self.queue.sort(key = lambda x: x[0])
        #self.printList()
        #for node in self.queue:
            #print("Heuristic Values: " +str(node[0]))
            #node[1].print()

    def isEmpty(self):
        if (len(self.queue)):
            return False
        return True
            
                
        

# Menu
# Description: Asks the user what puzzle they would like to solve. Zero represents an empty space.
def menu():
    name = "Jhaymar's"
    welcome = "Welcome to " +name+ " 8-puzzle solver.\nType '1' to use a default puzzle, or '2' to enter your own puzzle: "
    error = "\nPlease choose an appropriate choice\n"
    
    #Prompt puzzle option
    while True:
        choice = input(welcome)
        if choice == '1':
            puzzle = Problem(defaultPuzzle())
            break
        if choice == '2':
            puzzle = Problem(userPuzzle())
            break
        print(error)
        

    #Initialize queue
    queue = Queue()
    history = []
    puzzle.setHistory(history, "start")
    puzzle.setGN(int(0))
    #puzzle.printHistory()

    #Prompt algorithm choice
    result = []
    promptAlgorithm = ("\nAlgorithm Menu\n1. Uniform Cost Search\n2. A* with the Misplaced Tile Heuristic\n3. A* with the Manhattan distance heuristic.\nEnter your choice of algorithm: ")
    while True:
        choice = input(promptAlgorithm)
        if choice == '1':
            start = time.time()
            result = uniform_cost_search(puzzle, queue)
            end = time.time()
            break
        if choice == '2':
            start = time.time()
            result = misplaced_tile_heuristic(puzzle, queue)
            end = time.time()
            break
        if choice == '3':
            start = time.time()
            result = manhattan_distance(puzzle, queue)
            end = time.time()
            break
        print(error)
    #Prints algorithm execution time
    elapsed_time = end - start
    print("Execution time: " + str(elapsed_time) + " seconds")
    

    #Prompts the user if they would like to view the results
    while (True):
        if result[1] == True:
            choice = input("\nWould you like to print the results? (y/n): ")
            if choice == 'y' or choice == 'Y':
                result[0].printResults(puzzle)
                break
            if choice == 'n' or choice == 'N':
                break
        if result[1] == False:
            break
            






#---------------------------------------------------------------Puzzle Choices--------------------------------------------------------------------
def defaultPuzzle():
    INITIAL_STATE = [['1', '2', '3'], ['5', '0', '6'], ['4', '7', '8']]
    return INITIAL_STATE
    
def userPuzzle():
    #Prompt user to enter their puzzle
    puzzle = [[], [], []]
    prompt = "\nEnter puzzle use zero to represent a blank"
    print(prompt)
    puzzle1 = input("Enter the first row, use space or tabs between numbers: ")
    puzzle2 = input("Enter the second row, use space or tabs between numbers: ")
    puzzle3 = input("Enter the third row, use space or tabs between numbers: ")
    puzzle1 = re.split('\s|\t', puzzle1)
    puzzle2 = re.split('\s|\t', puzzle2)
    puzzle3 = re.split('\s|\t', puzzle3)
    puzzle = [puzzle1, puzzle2, puzzle3]

    #print(puzzle)
    return puzzle






# ----------------------------------------------------------------Algorithms-------------------------------------------------------------------
def uniform_cost_search(initialNode, queue):
    #Initializes heuristic function
    uniform_cost_search = "uniform_cost_search"
    queue.setHeuristicFunction(uniform_cost_search)

    #Initializes values, sets initial node to current node
    currNode = initialNode
    expansionCounter = 0
    printCtr = 0

    #Initialize queue by adding the initialNode to it
    currNode.setGN(calculateHeristics(currNode, uniform_cost_search))
    currHn = queue.add(currNode)

    #Loop until it finds a solution or not..
    while (True):
        #Test if queue is empty
        if (queue.isEmpty()):
            print("Failure! There is no solution to the problem.")
            return -1
        #Pops the first node, ready for expansion
        #Queue returns <f(n), first node in queue, h(n) of the node popped>
        resuts = queue.remove_front()
        currNode = resuts[1]
        currHn = calculateHeristics(currNode, uniform_cost_search)

        #Test for goal state
        if (currNode.goal_test()):
            print("Success! We have a found a solution")
            print("Amount of nodes expanded: " +str(expansionCounter))
            print("Max size of queue: " + str(queue.getMaxSize()))
            print("The depth of the solution node: "  + str(currNode.getHistorySize()))
            #currNode.printResults(problem)
            return (currNode, True)
        expansionCounter = expansionCounter + 1

        #Limits the amount of prints, save paper
        #Note: The language in this print value might confusing but getGN really returns the F(N) value of the node since the heuristic
            #value has already been added, which is why we subtract it to accurately show the G(N) value.
        if printCtr < 100:
            printCtr = printCtr + 1
            print("\n------------------------------------------------------\n")
            print("The best state to expand with G(n): " +str(currNode.getGN() - currHn) + " and H(n): " +str(currHn))
            currNode.print()
        
        #Prints the f(n) of the node being expanded
        #print("Result: " + str(resuts[0]))

        #Expands the node using for 4 different operators.
        #The new nodes created is then added to the queue
        expandNode(currNode, queue, uniform_cost_search)

def misplaced_tile_heuristic(initialNode, queue):
    #Initializes heuristic function
    misplaced_tile = "misplaced_tile_heuristic"
    queue.setHeuristicFunction(misplaced_tile)

    #Initializes values, sets initial node to current node
    currNode = initialNode
    expansionCounter = 0
    printCtr = 0

    #Initialize queue by adding the initialNode to it
    currNode.setGN(calculateHeristics(currNode, misplaced_tile))
    currHn = queue.add(currNode)

    #Loop until it finds a solution or not..
    while (True):
        #Test if queue is empty
        if (queue.isEmpty()):
            print("Failure! There is no solution to the problem.")
            return -1
        #Pops the first node, ready for expansion
        #Queue returns <f(n), first node in queue, h(n) of the node popped>
        resuts = queue.remove_front()
        currNode = resuts[1]
        currHn = calculateHeristics(currNode, misplaced_tile)

        #Test for goal state
        if (currNode.goal_test()):
            print("Success! We have a found a solution")
            print("Amount of nodes expanded: " +str(expansionCounter))
            print("Max size of queue: " + str(queue.getMaxSize()))
            print("The depth of the solution node: "  + str(currNode.getHistorySize()))
            #currNode.printResults(problem)
            return (currNode, True)
        expansionCounter = expansionCounter + 1

        #Limits the amount of prints, save paper
        #Note: The language in this print value might confusing but getGN really returns the F(N) value of the node since the heuristic
            #value has already been added, which is why we subtract it to accurately show the G(N) value.
        if printCtr < 100:
            printCtr = printCtr + 1
            print("\n------------------------------------------------------\n")
            print("Expanding node: With G(n): " +str(currNode.getGN() - currHn) + " and H(n) " +str(currHn))
            currNode.print()
        
        #Prints the f(n) of the node being expanded
        #print("Result: " + str(resuts[0]))

        #Expands the node using for 4 different operators.
        #The new nodes created is then added to the queue
        expandNode(currNode, queue, misplaced_tile)
        

#Note: Depth of the solution can be found based on how many times that solution node was expanded
def manhattan_distance(initialNode, queue):
    #Initializes heuristic function
    manhattan_distance = "manhattan_distance_heuristic"
    queue.setHeuristicFunction(manhattan_distance)

    #Initializes values, sets initial node to current node
    currNode = initialNode
    expansionCounter = 0
    printCtr = 0

    #Initialize queue by adding the initialNode to it
    currNode.setGN(calculateHeristics(currNode, manhattan_distance))
    currHn = queue.add(currNode)

    #Loop until it finds a solution or not..
    while (True):
        #Test if queue is empty
        if (queue.isEmpty()):
            print("Failure! There is no solution to the problem.")
            return -1
        #Pops the first node, ready for expansion
        #Queue returns <f(n), first node in queue, h(n) of the node popped>
        resuts = queue.remove_front()
        currNode = resuts[1]
        currHn = calculateHeristics(currNode, manhattan_distance)

        #Test for goal state
        if (currNode.goal_test()):
            print("Success! We have a found a solution")
            print("Amount of nodes expanded: " +str(expansionCounter))
            print("Max size of queue: " + str(queue.getMaxSize()))
            print("The depth of the solution node: "  + str(currNode.getHistorySize()))
            #currNode.printResults(problem)
            return (currNode, True)
        expansionCounter = expansionCounter + 1

        #Limits the amount of prints, save paper
        #Note: The language in this print value might confusing but getGN really returns the F(N) value of the node since the heuristic
            #value has already been added, which is why we subtract it to accurately show the G(N) value.
        if printCtr < 100000:
            printCtr = printCtr + 1
            print("\n------------------------------------------------------\n")
            print("The best state to expand wuth G(n): " +str(currNode.getGN() - currHn) + " and H(n): " +str(currHn))
            currNode.print()
        
        #Prints the f(n) of the node being expanded
        #print("Result: " + str(resuts[0]))

        #Expands the node using for 4 different operators.
        #The new nodes created is then added to the queue
        expandNode(currNode, queue, manhattan_distance)





#----------------------------------------------------Helper Functions--------------------------------------------------------------------------
#Description: Takes node as an input and performs the operators on it.
#Input: Current Puzzle, The Queue Object, Heuristic Algorithm
#Output: Operated Puzzles
def expandNode(node, queue, heuristicFunction):
    #Initializes the nodes, then wraps later in the problem class
    moveLeft = node.moveLeft()
    moveRight = node.moveRight()
    moveUp = node.moveUp()
    moveDown = node.moveDown()
    #print("Node: " +str(node.getGN()))


    #print("\n")
    if (moveLeft != -1):
        #print("Adding moveLeft..")
        pMoveLeft = Problem(moveLeft)
        hn = calculateHeristics(pMoveLeft, heuristicFunction)
        pMoveLeft.setHistory(node.getHistory().copy(), "move 0 left")
        pMoveLeft.setGN((node.getGN() + hn))
        queue.add(pMoveLeft)
        #print("Left: " + str(hn))
        #print("Left Total: " + str(pMoveLeft.getGN()))
    if (moveRight != -1):
        #print("Adding moveRight...")
        pMoveRight = Problem(moveRight)
        hn = calculateHeristics(pMoveRight, heuristicFunction)
        pMoveRight.setHistory(node.getHistory().copy(), "move 0 right")
        pMoveRight.setGN(node.getGN())
        pMoveRight.addGN(hn)
        queue.add(pMoveRight)
        #print("Rigth: " + str(hn))
        #print("Right Total: " + str(pMoveRight.getGN()))
    if (moveUp != -1):
        #print("Adding moveUp...")
        pMoveUp = Problem(moveUp)
        hn = calculateHeristics(pMoveUp, heuristicFunction)
        pMoveUp.setHistory(node.getHistory().copy(), "move 0 up")
        pMoveUp.setGN(node.getGN())
        pMoveUp.addGN(hn)
        queue.add(pMoveUp)
        #print("Up: " + str(hn))
        #print("Up Total: " + str(pMoveUp.getGN()))
    if (moveDown != -1):
        #print("Adding moveDown...")
        pMoveDown = Problem(moveDown)
        hn = calculateHeristics(pMoveDown, heuristicFunction)
        pMoveDown.setHistory(node.getHistory().copy(), "move 0 down")
        pMoveDown.setGN(node.getGN())
        pMoveDown.addGN(hn)
        queue.add(pMoveDown)
        #print("Down: " + str(hn))
        #print("Down Total: " + str(pMoveDown.getGN()))
    
    #Sort after expansion
    queue.sort()

#Input: Problem(node)
#Output: The heuristic value of the state in the object
def calculateHeristics(node, algorithm):
    gn = 0
    hn = 0
    currState = node.getCurrentState()
    i = 0
    j = 0
    if algorithm == "uniform_cost_search":
        return int(0)
    if algorithm == "misplaced_tile_heuristic":
        #Iterates the two states in parallel and increments h(n) if the tiles do not match
        #Run time: n
        for tile_currs, tile_goals in zip(currState, GOAL_STATE):
            for tile_curr, tile_goal in zip(tile_currs, tile_goals):
                #print(tile_curr)
                #print(tile_goal)
                if tile_curr != tile_goal:
                    i = i + 1
                    #print(str(i) +": " +tile_curr+ ", " +tile_goal+"\n")
                    hn = hn + 1
        #print("H(n): " +str(hn))
        return int(hn)

    if algorithm == "manhattan_distance_heuristic":
        row1 = 0
        col1 = 0
        row2 = 0
        col2 = 0
        #Run time: n^2
        for tile_currs , tile_goals in [(tile_currs, tile_goals) for tile_currs in currState for tile_goals in GOAL_STATE]:
            for tile_curr, tile_goal in [(tile_curr, tile_goal) for tile_curr in tile_currs for tile_goal in tile_goals]:
                #print("Tile Curr: ["+str(row1) +","+ str(col1) +"]: ("+ tile_curr +")")
                #print("Tile Goal: ["+str(row2) +","+ str(col2) +"] ("+ tile_goal +")")
                #print(str(i) +". " +tile_curr +","+tile_goal+"\n")
                if tile_curr == tile_goal:
                    if tile_curr != '0':
                        hn = hn + abs(row1 - row2) + abs(col1 - col2)
                    #print("Found: " +tile_curr+", " +tile_goal)
                    #print("Adding: "+str(hn) +" = "+ str(hn) + " + abs(" + str(row1) +" - "+ str(row2) +") + abs("+ str(col1) +" - "+str(col2)+ ")\n")
                i = i + 1
                col1 = int(i / 3)
                col2 = i % 3
            i = 0
            col1= i
            j = j + 1
            row1 = int(j / 3)
            row2 = j % 3
        #print("H(n): " +str(hn))
        return int(hn)
    print("\nError: Heuristic choice not found")




#----------------------------------------------------Tester Functions--------------------------------------------------------------------------
def testUserInput():
    puzzle = userPuzzle()
    
def testOperators():
    puzzle = Problem(userPuzzle())
    #puzzleMoveLeft = Puzzle(puzzle.moveLeft())
    #puzzleMoveRight = Puzzle(puzzle.moveRight())
    #puzzleMoveUp = Problem(puzzle.moveUp())
    puzzleMoveDown = Problem(puzzle.moveDown())
    #puzzle.print(puzzleMoveUp.CURRENT_STATE)
    puzzle.print()

def testProblem():
    puzzle = Problem(userPuzzle())
    puzzle.print()
    puzzle.goal_test()

def testQueue():
    #Initialize value
    queue = Queue()
    initial_state = Problem(userPuzzle())
    heuristicFunction = "manhattan_distance_heuristic"
    #queue.setHeuristicFunction(heuristicFunction)

    #Test add
    #-----------------#
    #Add initial state to queue
    queue.add(initial_state)
    expandNode(initial_state, queue, heuristicFunction )
    queue.print()

    #movedLeft = (Problem(initial_state.moveDown()))
    #queue.add(movedLeft)

    #Test print
    #queue.printList()

    #Test remove
    #print("\nRemoving first item. . ")
    #queue.remove_front()

    #print("Printing new list. . .")
    #queue.printList()

    #Test visited List
    #TO DO


def testHeuristicFunction():
    node = Problem(userPuzzle())

    #Test misplaced tile heuristic
   #calculateHeristics(node, "misplaced_tile_heuristic")
    
    #Test manhattan distance heuristic
    calculateHeristics(node, "manhattan_distance_heuristic")

#--------------------------------------------------------End Tester Functions-------------------------------------------------------------------------
def main():
    menu()
    #testUserInput
    #testOperators()
    #testProblem()
    #testQueue()
    #testHeuristicFunction()
main()
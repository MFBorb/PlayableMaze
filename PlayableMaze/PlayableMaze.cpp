#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <limits>

/////////////////////////////////////////////////////



// Struct definitions

struct coordinate {
    int x;
    int y;
};

struct mazeNode {
    bool northWall = true;
    bool southWall = true;
    bool eastWall = true;
    bool westWall = true;
    bool visited = false;
    coordinate coordinate = { 0, 0 };
};



////////////////////////////////////////////////////



// Global definitions

int mazeWidth = 10;
int mazeLength = 10;



////////////////////////////////////////////////////



// Prototype functions


void introScreen(void);
void clearScreen(void);
std::vector<std::vector<mazeNode>> mazeGenerator();
std::vector<coordinate> validNeighbors(coordinate coord, std::vector<std::vector<mazeNode>> maze);
void removeWall(mazeNode& currentNode, mazeNode& neighborNode);
void mazePrintOut(std::vector<std::vector<mazeNode>> mazeArray, coordinate playerPosition);
coordinate findStartingPosition(std::vector<std::vector<mazeNode>> mazeArray);
void movePlayer(coordinate* playerPosition, char userInput, std::vector<std::vector<mazeNode>> maze);



////////////////////////////////////////////////////



// Main function

int main(int argc, char* argv[]) {

    introScreen();

    // Determine the size that the user wants.
    std::cout << "What do you want the maze width to be?: ";
    std::cin >> mazeWidth;

    std::cout << "What do you want the maze length to be?: ";
    std::cin >> mazeLength;

    std::vector<std::vector<mazeNode>> mazeArray = mazeGenerator();

    coordinate playerPosition = findStartingPosition(mazeArray);

    char userInput;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    clearScreen();
    mazePrintOut(mazeArray, playerPosition);

    // Loop getting user input forever.
    while (std::cin.get(userInput)) {
        clearScreen();
        movePlayer(&playerPosition, userInput, mazeArray);
        mazePrintOut(mazeArray, playerPosition);
    }

}



////////////////////////////////////////////////////



// Support functions

void introScreen(void) {
    std::cout << "                                                  \n";
    std::cout << "                       Maze                       \n";
    std::cout << "                       Game                       \n";
    std::cout << "                                                  \n";
    std::cout << "                    Press Enter                   \n";
    std::cout << "                 to start the game!               \n";

    std::cin.get();
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
}

void clearScreen(void) {
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

std::vector<std::vector<mazeNode>> mazeGenerator() {

    std::vector<std::vector<mazeNode>> mazeArray;

    for (int i = 0; i < mazeLength; i++) {
        std::vector<mazeNode> row;
        for (int j = 0; j < mazeWidth; j++) {
            mazeNode node;
            node.coordinate.x = j;
            node.coordinate.y = i;
            row.push_back(node);
        }
        mazeArray.push_back(row);
    }

    // Choose the initial cell and mark it as visited. Push it to the stack
    std::vector<coordinate> stack;
    srand(time(NULL));
    int firstX = rand() % mazeWidth;
    int firstY = rand() % mazeLength;

    coordinate initCoord;
    initCoord.x = firstX;
    initCoord.y = firstY;
    stack.push_back(initCoord);

    mazeArray[firstY][firstX].visited = true;


    // While the stack is not empty
    while (!stack.empty()) {

        // Pop a cell off the stack and mark it as current cell.
        coordinate currentCoord = stack.back();
        stack.pop_back();

        // Find valid neighbors
        std::vector<coordinate> neighbors = validNeighbors(currentCoord, mazeArray);

        // if there are unvisited neighbors:
        if (!neighbors.empty()) {

            // Push the current cell onto the stack
            stack.push_back(currentCoord);

            // Choose an unvisited neighbor (choose first in neighbor list for now)
            coordinate chosenNeighbor = neighbors[rand() % neighbors.size()];
            mazeNode neighborNode = mazeArray[chosenNeighbor.y][chosenNeighbor.x];

            // Remove the wall between current cell and neighbor
            removeWall(mazeArray[currentCoord.y][currentCoord.x], neighborNode);

            // Mark the cell as visited and push it onto the stack
            neighborNode.visited = true;
            mazeArray[chosenNeighbor.y][chosenNeighbor.x] = neighborNode;
            stack.push_back(chosenNeighbor);
        }
    }

    // Generate a random opening and ending
    mazeArray[0][rand() % mazeWidth].northWall = false;
    mazeArray[mazeLength - 1][rand() % mazeWidth].southWall = false;

    return mazeArray;
}

std::vector<coordinate> validNeighbors(coordinate coord, std::vector<std::vector<mazeNode>> maze) {
    std::vector<coordinate> neighbors;
    mazeNode potNeighbor;

    // We check to see if there are neighbors above, below, left, and right of
    // the current node. A neighbor will be valid if it has not been visited yet.
    if (coord.y > 0) {
        potNeighbor = maze[coord.y - 1][coord.x];

        if (potNeighbor.visited == false) {
            coordinate neighborCoord;
            neighborCoord.x = coord.x;
            neighborCoord.y = coord.y - 1;
            neighbors.push_back(neighborCoord);
        }
    }

    if (coord.x > 0) {
        potNeighbor = maze[coord.y][coord.x - 1];

        if (potNeighbor.visited == false) {
            coordinate neighborCoord;
            neighborCoord.x = coord.x - 1;
            neighborCoord.y = coord.y;
            neighbors.push_back(neighborCoord);
        }
    }

    if (coord.y < mazeLength - 1) {
        potNeighbor = maze[coord.y + 1][coord.x];

        if (potNeighbor.visited == false) {
            coordinate neighborCoord;
            neighborCoord.x = coord.x;
            neighborCoord.y = coord.y + 1;
            neighbors.push_back(neighborCoord);
        }
    }

    if (coord.x < mazeWidth - 1) {
        potNeighbor = maze[coord.y][coord.x + 1];

        if (potNeighbor.visited == false) {
            coordinate neighborCoord;
            neighborCoord.x = coord.x + 1;
            neighborCoord.y = coord.y;
            neighbors.push_back(neighborCoord);
        }
    }

    return neighbors;
}

void removeWall(mazeNode& curNode, mazeNode& neighbor) {
    coordinate curCoord = curNode.coordinate;
    coordinate neighborCoord = neighbor.coordinate;

    // Check to see where the neighbor is in relation
    // to the current node and remove the appropriate wall.
    if (neighborCoord.y == curCoord.y - 1) {
        neighbor.southWall = false;
        curNode.northWall = false;
    }
    else if (neighborCoord.x == curCoord.x - 1) {
        neighbor.eastWall = false;
        curNode.westWall = false;
    }
    else if (neighborCoord.y == curCoord.y + 1) {
        neighbor.northWall = false;
        curNode.southWall = false;
    }
    else {
        neighbor.westWall = false;
        curNode.eastWall = false;
    }
}

void mazePrintOut(std::vector<std::vector<mazeNode>> mazeArray, coordinate player) {
    // Loop to go through each row
    for (int i = 0; i < mazeLength; i++) {

        // Loop to go through each element in a row. We only need to check the north wall and
        // west wall as a missing west wall is the same the previous node having a missing east
        // wall. This'll save computational time and make the maze come out properly.
        for (int j = 0; j < mazeWidth; j++) {
            mazeNode curNode = mazeArray[i][j];

            if (curNode.northWall == true) {
                std::cout << " ---";
            }
            else {
                std::cout << "    ";
            }
        }

        std::cout << std::endl;

        for (int j = 0; j < mazeWidth; j++) {
            mazeNode curNode = mazeArray[i][j];

            if (curNode.westWall == true) {
                std::cout << "| ";
            }
            else {
                std::cout << "  ";
            }

            if (player.x == j && player.y == i) {
                std::cout << "p ";
            }
            else {
                std::cout << "  ";
            }
        }

        std::cout << "|" << std::endl;
    }

    int bottomRowIndex = mazeLength - 1;
    for (int i = 0; i < mazeWidth; i++) {
        if (mazeArray[bottomRowIndex][i].southWall == true) {
            std::cout << " ---";
        }
        else {
            std::cout << "    ";
        }
    }

    std::cout << std::endl;

}

coordinate findStartingPosition(std::vector<std::vector<mazeNode>> mazeArray) {
    coordinate startingCoord;

    std::vector<mazeNode> bottomRow = mazeArray.back();

    for (int i = 0; i < bottomRow.size(); i++) {
        if (bottomRow[i].southWall == false) {
            startingCoord = bottomRow[i].coordinate;
            return startingCoord;
        }
    }

    return { 0, 0 };
}

void movePlayer(coordinate* player, char userInput, std::vector<std::vector<mazeNode>> maze) {
    int playerY = player->y;
    int playerX = player->x;

    int mazeHeight = maze.size();
    int mazeWidth = maze[0].size();

    switch (userInput) {
        // Up
        case 'w':

            if (playerY > 0 && maze[playerY][playerX].northWall == false) {
                player->y--;
            }

            break;

        // Left
        case 'a':

            if (playerX > 0 && maze[playerY][playerX].westWall == false) {
                player->x--;
            }

            break;

        // Down
        case 's':

            if (playerY < mazeHeight - 1 && maze[playerY][playerX].southWall == false) {
                player->y++;
            }

            break;

        // Right
        case 'd':

            if (playerX < mazeWidth - 1 && maze[playerY][playerX].eastWall == false) {
                player->x++;
            }

            break;
    }
}
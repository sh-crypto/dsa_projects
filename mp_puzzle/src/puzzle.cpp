/**
 * @file puzzle.cpp
 * Implementation of puzzle class.
 */
#include "puzzle.h"
#include "algorithm"
PuzzleState::PuzzleState() : board{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0}, empty_pos(15) {}

PuzzleState::PuzzleState(const std::array<uint8_t, 16> state) {
    std::array<uint8_t, 16> temp = state;
    std::sort(temp.begin(), temp.end());
    for(int i = 0; i < 16; ++i) {
        if(temp[i] != i) {
            board.fill(0);
            empty_pos = -1;
            break;
        }
    }
    board = state;
    empty_pos = std::distance(board.begin(), std::find(board.begin(), board.end(), 0));
}

std::array<uint8_t, 16> PuzzleState::asArray() const {
    return board;
}

bool PuzzleState::operator==(const PuzzleState &rhs) const {
    return board == rhs.board;
}

bool PuzzleState::operator!=(const PuzzleState &rhs) const {
    return !(*this == rhs);
}

bool PuzzleState::operator<(const PuzzleState &rhs) const {
    return board < rhs.board;
}

PuzzleState PuzzleState::getNeighbor(Direction direction) const {
    PuzzleState neighbour;
    int index = -1;
    for (int i = 0; i < 16; i++) {
        if (board[i] == 0) index = i;
    }
    if (direction == Direction::UP) {
       if (index > 0 && index < 12) {
            for (int i = 0; i < 16; i++) {
                neighbour.board[i] = board[i]; 
            }
            std::swap(neighbour.board[index],neighbour.board[index + 4]);
       }
    }
    if (direction == Direction::DOWN) {
       if (index > 3 && index < 16) {
            for (int i = 0; i < 16; i++) {
                neighbour.board[i] = board[i]; 
            }
            std::swap(neighbour.board[index],neighbour.board[index - 4]);
       }
    }
    if (direction == Direction::LEFT) {
       if (index > 0 && index < 16 && index % 4 != 3) {
            for (int i = 0; i < 16; i++) {
                neighbour.board[i] = board[i]; 
            }
            std::swap(neighbour.board[index],neighbour.board[index + 1]);       
       }
    }
    if (direction == Direction::RIGHT) {
       if (index > 0 && index < 16 && index % 4 != 0) {
            for (int i = 0; i < 16; i++) {
                neighbour.board[i] = board[i]; 
            }
            std::swap(neighbour.board[index],neighbour.board[index - 1]);
       }
    }
    return neighbour;
}

std::vector<PuzzleState> PuzzleState::getNeighbors() const {
    std::vector<PuzzleState> neighbors;

    int row = empty_pos / 4; 
    int col = empty_pos % 4;  

    auto addNeighborIfValid = [&](int new_row, int new_col) {
        if (new_row >= 0 && new_row < 4 && new_col >= 0 && new_col < 4) {
            std::array<uint8_t, 16> new_board = board;
            std::swap(new_board[empty_pos], new_board[new_row * 4 + new_col]);
            neighbors.push_back(PuzzleState(new_board));
        }
    };
    addNeighborIfValid(row - 1, col);  // UP
    addNeighborIfValid(row + 1, col);  // DOWN
    addNeighborIfValid(row, col - 1);  // LEFT
    addNeighborIfValid(row, col + 1);  // RIGHT

    return neighbors;
}

int PuzzleState::manhattanDistance(const PuzzleState desiredState) const {
    int distance = 0;
    for (int i = 0; i < 16; ++i) {
        if (board[i] != 0) {
            int currentPos = i;
            int targetPos = -1;
            int targetValue = board[i];

            // Find the position of the target value in the desired state
            for (int j = 0; j < 16; ++j) {
                if (desiredState.board[j] == targetValue) {
                    targetPos = j;
                    break;
                }
            }
            // Calculate the Manhattan distance for this tile
            int currentRow = currentPos / 4;
            int currentCol = currentPos % 4;
            int targetRow = targetPos / 4;
            int targetCol = targetPos % 4;
            int tileDistance = std::abs(currentRow - targetRow) + std::abs(currentCol - targetCol);

            distance += tileDistance;
        }
    }

    return distance;
}

std::vector<PuzzleState> solveAstar(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
    std::priority_queue<StateWithPriority> openSet;
    std::map<PuzzleState, PuzzleState> cameFrom;
    std::map<PuzzleState, int> gScore;
    std::set<PuzzleState> closedSet;

    openSet.push({startState, 0});
    gScore[startState] = 0;
    cameFrom[startState] = startState;

    if (iterations) *iterations = 0;

    while (!openSet.empty()) {
        PuzzleState current = openSet.top().state;
        openSet.pop();

        if (iterations) (*iterations)++;

        if (current == desiredState)
            return tracePath(cameFrom, startState, desiredState);

        closedSet.insert(current);

        for (const PuzzleState& neighbor : current.getNeighbors()) {
            if (closedSet.find(neighbor) != closedSet.end())
                continue;

            int tentativeGScore = gScore[current] + 1;  // Assuming each move costs 1
            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                gScore[neighbor] = tentativeGScore;
                int priority = tentativeGScore + neighbor.manhattanDistance(desiredState);
                openSet.push({neighbor, priority});
                cameFrom[neighbor] = current;
            }
        }
    }

    return {};  // Return empty path if no path found
}

std::vector<PuzzleState> solveBFS(const PuzzleState &initialState, const PuzzleState &goalState, size_t *stepCount) {
    std::queue<PuzzleState> frontier;
    std::set<PuzzleState> seen;
    std::map<PuzzleState, PuzzleState> parent;
    
    frontier.push(initialState);
    seen.insert(initialState);
    if (stepCount) *stepCount = 0;

    while (!frontier.empty()) {
        PuzzleState current = frontier.front();
        frontier.pop();
        if (stepCount) (*stepCount)++;

        if (current == goalState) return tracePath(parent, initialState, goalState);

        for (PuzzleState neighbor : current.getNeighbors()) {
            if (seen.count(neighbor) == 0) {
                seen.insert(neighbor);
                parent[neighbor] = current;
                frontier.push(neighbor);
            }
        }
    }
    return {};
}

std::vector<PuzzleState> tracePath(const std::map<PuzzleState, PuzzleState>& parents, const PuzzleState& start, const PuzzleState& end) {
    std::vector<PuzzleState> path;
    for (PuzzleState state = end; state != start; state = parents.at(state)) {
        path.push_back(state);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
    return path;
}



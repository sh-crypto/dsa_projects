/* Your code here! */
#include "maze.h"
#include "vector"
#include "queue"
#include "algorithm"
using namespace cs225;

SquareMaze::SquareMaze() {
    int height_ = 0;
    int width_ = 0;
}

void SquareMaze::makeMaze(int width, int height) {
    width_ = width;
    height_ = height;
    dset.addelements(width_*height_);
    wall.resize(width_*height_);

    for(auto& w : wall) {
        w = std::make_pair(true, true);
    }
    while(dset.size(0) != (width_*height_)) {
        int w = rand() % width_;
        int h = rand() % height_;
        bool rwall = rand() % 2;
        int val = h * width_ + w;
        int neighbourindex;
        if(rwall) {
            if(w < width_ - 1 && wall[val].first) {
                neighbourindex = val + 1;
                if(dset.find(val) != dset.find(neighbourindex)) {
                    setWall(w,h,0,false);
                    dset.setunion(val, neighbourindex);
                }
            }
        } else {
            if(h < height_ - 1 && wall[val].second) {
                neighbourindex = val + width_;
                if(dset.find(val) != dset.find(neighbourindex)) {
                    setWall(w,h,1,false);
                    dset.setunion(val, neighbourindex);
                }
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        return false;
    }
    int val = y*width_ + x;
    switch(dir) {
        case 0:
            return x < width_-1 && !wall[val].first;
        case 1:
            return y < height_-1 && !wall[val].second;
        case 2:
            return x > 0 && !wall[val-1].first;
        case 3:
            return y > 0 && !wall[val-width_].second;
        default:
            return false;
    }
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if(x < 0 || x >= width_ || y < 0 || y >= height_) {
        return;
    }
    int val = y*width_ + x;
    switch(dir) {
        case 0: 
            wall[val].first = exists;
            break;
        case 1:
            wall[val].second = exists;
            break;
        default:
            break;
    }
}

std::vector<int> SquareMaze::solveMaze() {
    std::vector<int> solution;
    std::queue<std::pair<int, int>> q;
    int size_ = width_*height_;
    std::vector<bool> visited(size_, false);
    std::vector<int> prev(size_, -1);
    std::vector<int> length(size_, -1);

    q.push({0, 0}); // Start from the origin (0, 0)
    visited[0] = true;
    length[0] = 1;

    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        for (int dir = 0; dir < 4; ++dir) {
            int newX = x, newY = y;
            if (dir == 0)
                newX++;
            else if (dir == 1)
                newY++;
            else if (dir == 2)
                newX--;
            else
                newY--;

            int index = newY * width_ + newX;
            if (canTravel(x, y, dir) && !visited[index]) {
                visited[index] = true;
                length[index] = length[y * width_ + x] + 1;
                prev[index] = y * width_ + x;
                q.push({newX, newY});
            }
        }
    }

    // Find the cell in the bottom row with the maximum distance
    int maxDist = 0;
    int destIndex = -1;
    for (int i = size_ - width_; i < size_; ++i) {
        if (length[i] > maxDist) {
            maxDist = length[i];
            destIndex = i;
        } else if (length[i] == maxDist && i < destIndex) {
            destIndex = i;
        }
    }

    // Reconstruct the path from the destination to the origin
    while (destIndex != 0) {
        int prevIndex = prev[destIndex];
        int prevX = prevIndex % width_;
        int prevY = prevIndex / width_;
        int x = destIndex % width_;
        int y = destIndex / width_;

        if (x > prevX)
            solution.push_back(0); // Right
        else if (y > prevY)
            solution.push_back(1); // Down
        else if (x < prevX)
            solution.push_back(2); // Left
        else
            solution.push_back(3); // Up

        destIndex = prevIndex;
    }

    std::reverse(solution.begin(), solution.end());
    return solution;
}

cs225::PNG* SquareMaze::drawMaze() const {
    int pngWidth = width_ * 10 + 1;
    int pngHeight = height_ * 10 + 1;
    PNG* png = new PNG(pngWidth, pngHeight);

    // Blacken the entire topmost row, except the entrance cell (1, 0) through (9, 0)
    for (int i = 10; i < pngWidth; ++i) {
        png->getPixel(i,0).l = 0;
    }

    // Blacken the leftmost column
    for (int j = 0; j < pngHeight; ++j) {
        png->getPixel(0, j).l = 0;
    }

    for (int i = 0; i < width_; ++i) {
        for (int j = 0; j < height_; ++j) {
            int index = j * width_ + i;
            if (wall[index].first) {
                // Draw the right wall
                for (int k = 0; k <= 10; ++k) {
                    png->getPixel((i + 1) * 10, j * 10 + k).l = 0;
                }
            }
            if (wall[index].second) {
                // Draw the bottom wall
                for (int k = 0; k <= 10; ++k) {
                    png->getPixel(i * 10 + k, (j + 1) * 10).l = 0;
                }
            }
        }
    }

    return png;
}

PNG* SquareMaze::drawMazeWithSolution() {
    PNG* png = drawMaze();

    std::vector<int> solution = solveMaze();
    int x = 5, y = 5; // Start at (5, 5)

    for (int dir : solution) {
        int prevX = x, prevY = y;

        if (dir == 0) { // Right
            for (int k = 0; k < 11; ++k) {
                HSLAPixel& pixel = png->getPixel(x + k, y);
                pixel.h = 0;
                pixel.s = 1;
                pixel.l = 0.5;
                pixel.a = 1;
            }
            x += 10;
        } else if (dir == 1) { // Down
            for (int k = 0; k < 11; ++k) {
                HSLAPixel& pixel = png->getPixel(x, y + k);
                pixel.h = 0;
                pixel.s = 1;
                pixel.l = 0.5;
                pixel.a = 1;
            }
            y += 10;
        } else if (dir == 2) { // Left
            for (int k = 0; k < 11; ++k) {
                HSLAPixel& pixel = png->getPixel(x - k, y);
                pixel.h = 0;
                pixel.s = 1;
                pixel.l = 0.5;
                pixel.a = 1;
            }
            x -= 10;
        } else { // Up
            for (int k = 0; k < 11; ++k) {
                HSLAPixel& pixel = png->getPixel(x, y - k);
                pixel.h = 0;
                pixel.s = 1;
                pixel.l = 0.5;
                pixel.a = 1;
            }
            y -= 10;
        }
    }

    // Whiten the bottom wall of the destination cell
    int destX, destY;
    destY = height_ - 1;
    destX = x / 10;

    for (int k = 1; k <= 9; ++k) {
        HSLAPixel& pixel = png->getPixel(destX * 10 + k, (destY + 1) * 10);
        pixel.l = 1;
    }

    return png;
}
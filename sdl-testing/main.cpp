#include <iostream>
#include <SDL2/SDL.h>
#include "src/piece.cpp"
#include <vector>
#include <array>
using namespace std;

const int WIDTH = 1280, HEIGHT = 720;
const int FPS = 60;
const int frameDelay = 1000/FPS;

int pieceColor[3] = {2, 48, 32};
int backgroundColor[3] = {16, 16, 16};
int outlineColor[3] = {255, 255, 255};
int interlineColor[3] = {0, 0, 0};

// Inherited class that handles the visual aspects of the piece class
class PieceR : public Piece {
    private:
    vector<SDL_Rect> pieceRects;
    int x, y;
    bool beingMoved;
    vector<vector<int>> outlines;
    vector<vector<int>> interlines;

    public:
    PieceR() : Piece() {}

    PieceR(vector<vector<int>> shape, int x = 0, int y = 0) : Piece(shape) {
        this->x = x;
        this->y = y;
        int tempX = x, tempY = y;
        for (int i = 0; i < shape.size(); i++) {
            tempX = x;
            for (int j = 0; j < shape[0].size(); j++) {
                if (shape[i][j]) {
                    pieceRects.push_back({tempX, tempY, 20, 20});
                }
                tempX += 20;
            }
            tempY += 20;
        }
    }
    
    PieceR(Piece piece, int x = 0, int y = 0) {
        this->shape = piece.getShape();
        this->size[0] = piece.getSize()[0];
        this->size[1] = piece.getSize()[1];
        this->x = x;
        this->y = y;
        updateRects();
    }

    vector<SDL_Rect> getPieceRects() {
        return pieceRects;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    bool isBeingMoved() {
        return beingMoved;
    }

    void updateLoc(int x = 0, int y = 0) {
        for (int i = 0; i < pieceRects.size(); i++) {
            pieceRects[i].x += x-this->x;
            pieceRects[i].y += y-this->y;
        }
        for (int i = 0; i < interlines.size(); i++) {
            interlines[i][0] += x-this->x;
            interlines[i][1] += y-this->y;
            interlines[i][2] += x-this->x;
            interlines[i][3] += y-this->y;
        }
        for (int i = 0; i < outlines.size(); i++) {
            outlines[i][0] += x-this->x;
            outlines[i][1] += y-this->y;
            outlines[i][2] += x-this->x;
            outlines[i][3] += y-this->y;
        }
        this->x = x;
        this->y = y;
    }

    // Thought this was kind of smart, in order to make outlines of the piece I added 0's on every space around the shape so it can detect when 0's and 1's meet properly
    void updateRects() {
        pieceRects.clear();
        outlines.clear();
        interlines.clear();
        int tempX = x-20, tempY = y-20;
        vector<vector<int>> tempShape;
        vector<int> tempVec;
        for (int i = 0; i < shape[0].size() + 2; i++) {
            tempVec.push_back(0);
        }
        tempShape.push_back(tempVec);
        for (int i = 0; i < shape.size(); i++) {
            tempVec.clear();
            tempVec.push_back(0);
            for (int j = 0; j < shape[0].size(); j++) {
                tempVec.push_back(shape[i][j]);
            }
            tempVec.push_back(0);
            tempShape.push_back(tempVec);
        }
        tempVec.clear();
        for (int i = 0; i < shape[0].size() + 2; i++) {
            tempVec.push_back(0);
        }
        tempShape.push_back(tempVec);
        // for (int i = 0; i < tempShape.size(); i++) {
        //     for (int j = 0; j < tempShape[0].size(); j++) {
        //         cout << tempShape[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        vector<int> line;
        bool right;
        bool down;
        for (int i = 0; i < tempShape.size()-1; i++) {
            tempX = x-20;
            for (int j = 0; j < tempShape[0].size()-1; j++) {
                right = tempShape[i][j + 1];
                down = tempShape[i + 1][j];
                if (tempShape[i][j]) {
                    pieceRects.push_back({tempX, tempY, 20, 20});
                    line.insert(line.end(), {tempX+20, tempY, tempX+20, tempY+20});
                    if (right) {
                        interlines.push_back(line);
                    } else {
                        outlines.push_back(line);
                    } line.clear();
                    line.insert(line.end(), {tempX, tempY+20, tempX+20, tempY+20});
                    if (down) {
                        interlines.push_back(line);
                    } else {
                        outlines.push_back(line);
                    } line.clear();
                }
                else {
                    if (right) {
                        line.insert(line.end(), {tempX+20, tempY, tempX+20, tempY+20});
                        outlines.push_back(line);
                        line.clear();
                    }
                    if (down) {
                        line.insert(line.end(), {tempX, tempY+20, tempX+20, tempY+20});
                        outlines.push_back(line);
                        line.clear();
                    }
                }
                tempX += 20;
            }
            tempY += 20;
        }
    }

    void setBeingMoved(bool moved) {
        beingMoved = moved;
    }

    void drawPiece(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, pieceColor[0], pieceColor[1], pieceColor[2], 255);
        SDL_Rect arr[pieceRects.size()];
        copy(pieceRects.begin(),pieceRects.end(),arr);
        SDL_RenderFillRects(renderer, arr, pieceRects.size());
        for (int i = 0; i < interlines.size(); i++) {
            SDL_SetRenderDrawColor(renderer, interlineColor[0], interlineColor[1], interlineColor[2], 0);
            SDL_RenderDrawLine(renderer, interlines[i][0], interlines[i][1], interlines[i][2], interlines[i][3]);
        }
        for (int i = 0; i < outlines.size(); i++) {
            SDL_SetRenderDrawColor(renderer, outlineColor[0], outlineColor[1], outlineColor[2], 255);
            SDL_RenderDrawLine(renderer, outlines[i][0], outlines[i][1], outlines[i][2], outlines[i][3]);
        }
    }

    bool pointInPiece(int x, int y) {
        SDL_Point point = {x, y};
        for (SDL_Rect rect : pieceRects) {
            if (SDL_PointInRect(&point, &rect)) {
                return true;
            }
        }
        return false;
    }

    void rotate() {
        vector<vector<int>> newShape;
        for (int c = 0; c < shape[0].size(); c++) {
            vector<int> vector;
            for (int r = 0; r < shape.size(); r++) {
                vector.insert(vector.begin(), shape[r][c]);
            }
            newShape.push_back(vector);
        }
        shape = newShape;

        updateRects();
    }
};

int main (int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetVSync(renderer, 1);

    if (window == NULL) {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Event event;

    vector<PieceR> pieces;

    Piece piece1 = Piece({{1, 0}, {1, 1}, {0, 1}});
    Piece piece2 = Piece({{1, 1, 1}, {0, 1, 0}});

    pieces.emplace_back(piece1, 200, 200);
    pieces.emplace_back(piece2, 800, 200);

    int mouseX, mouseY, gapX, gapY, mouseState;

    PieceR* pieceBeingMoved = nullptr;

    // Main Loop
    bool running = true;
    while (running) {

        // -----Input handling-----
        mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if (SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouseState = SDL_GetMouseState(NULL, NULL);
                    if (mouseState == 1) {
                        // cout << mouseX << " " << mouseY << endl;
                        for (int i = 0; i < pieces.size(); i++) {
                            if (pieces[i].pointInPiece(mouseX, mouseY)) {
                                pieceBeingMoved = &pieces[i];
                                pieces[i].setBeingMoved(true);
                                pieces.push_back(*pieceBeingMoved);
                                pieces.erase(pieces.begin() + i);
                                gapX = mouseX - pieces[i].getX();
                                gapY = mouseY - pieces[i].getY();
                            }
                        }
                    }
                    else if (mouseState == 5 && pieceBeingMoved != nullptr) {
                        pieceBeingMoved->rotate();
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    mouseState = SDL_GetMouseState(NULL, NULL);
                    if (mouseState == 0) {
                        if (pieceBeingMoved != nullptr) {
                            pieceBeingMoved->setBeingMoved(false);
                            pieceBeingMoved = nullptr;
                        }
                    }
                    break;
            }
        }

        // -----In-Game Logic-----
        if (pieceBeingMoved != nullptr) {
            pieceBeingMoved->updateLoc(mouseX - gapX, mouseY - gapY);
        }

        // -----Rendering-----

        SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], 255);
        SDL_RenderClear(renderer);
        
        for (PieceR piece : pieces) {
            piece.drawPiece(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
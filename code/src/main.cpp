#include <Functions.hpp>
#include <Rectangle.hpp>
#include <raylib-cpp.hpp>
#include <raylib.h>
#include <raymath.h>
#include <utility>

#define WIDTH 600
#define HEIGHT 1000
#define S 25

Color get_color(int);
void get_piece(int (&)[3][3]);
class Piece;

bool check_lines(int (&grid)[WIDTH/S][HEIGHT/S]){

    int sum =true;
    for(int i = HEIGHT/S-1; i>1;i--){
        for(int j = WIDTH/S-2; j>1;j--){
            if(grid[j][i]==0){
                sum = false;
            }
        }
        if(sum){
            for(int i = HEIGHT/S-1; i>1;i--){
                for(int j = WIDTH/S-1; j>0;j--){
                    grid[j][i] = grid[j][i-1];
                }
            }
            sum = true;
        }
    }
    return true;
}
bool check_collision(int piece[3][3],int (&grid)[WIDTH/S][HEIGHT/S],  std::pair<int,int> &pos ,std::pair<int,int>post){
    bool state = false;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if( piece[i][j] != 0){
                if(i+post.first<1 || j+post.second>HEIGHT/S-1||i+post.first > WIDTH/S-2){
                    state = true;
                }
                if( grid[post.first+i][post.second+j]!=0){
                    state = true; 
                } 
            }
        } 
    }
    return state;
}

void step( int (&grid)[WIDTH/S][HEIGHT/S], int (&piece)[3][3],int &state, std::pair<int,int> &pos, std::pair<int,int> &post){

    if( state==0 ){
        pos = {6,0};
        post = {6,3};
        get_piece(piece);
        state=1;
        return;
    }

    check_lines(grid);
    if( state==1 ){
        pos = post;
        post.second+=1;

    }

    if( check_collision(piece,grid,pos,post) ){

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if( piece[i][j] != 0){
                    grid[pos.first+i][pos.second+j] = piece[i][j];
                }
            }
        }
        state=0 ;
        return;
    }; 

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if(piece[i][j]!=0){
                grid[pos.first+i][pos.second+j] = piece[i][j];
            }
        }
    }

    for(int line=0 ;line<WIDTH/S; line++){
        for(int column=0 ;column<HEIGHT/S; column++){
            DrawRectangle(line*S,column*S, S, S, get_color(grid[line][column]));
            DrawText("Press q to pause the game",0,0,10,BLACK);
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if(piece[i][j]!=0){
                grid[pos.first+i][pos.second+j] = 0;
            }
        }
    }

}

int main() {
    int state = 0;
    
    // Initialization
    int screenWidth = WIDTH;
    int screenHeight = HEIGHT;

    raylib::Window w(screenWidth, screenHeight, "Tetris");
    int grid[WIDTH/S][HEIGHT/S];
    int piece[3][3];

    for (int i = 0; i < WIDTH/S; ++i) {
        for (int j = 0; j < HEIGHT/S; ++j) {
            grid[i][j] = 0;
        }
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            piece[i][j] = 0;
        }
    }


    SetTargetFPS(60);
    
    std::pair<int,int> pos = {6,0};
    std::pair<int,int> post = {6,1};
    int counter = 0;
    bool start = false;
    // Main game loop
    while (!w.ShouldClose()) // Detect window close button or ESC key
    { // Update
        // Draw
        BeginDrawing();

        if(!start){
            DrawText("To Start Game press Enter",0,0,10,WHITE);
            if(IsKeyPressed(KEY_ENTER)){
                start=true;
                continue;
            }
            EndDrawing();
            continue;
        }
            
        if(counter++%5==0){

            auto aux_pos = post;
            if (IsKeyDown(KEY_RIGHT)){
                post.first += 1;
                if(check_collision(piece,grid,pos,post)){
                    post = aux_pos;
                    
                }
            }


            if (IsKeyDown(KEY_LEFT)){
                post.first -= 1;
                if(check_collision(piece,grid,pos,post)){
                    post = aux_pos;
                    
                }
            }
            //if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f; // rotate
            if (IsKeyDown(KEY_DOWN)){
                post.second += 1;
                if(check_collision(piece,grid,pos,post)){
                    post = aux_pos;
                    
                }
            }
            if (IsKeyDown(KEY_Q)){
                start=false;
            }

            step( grid, piece, state, pos, post);

        }
        EndDrawing();
    }

    return 0;
}

Color get_color(int i){
    if( i==1 ){
        return BLACK;
    }else if(i==2){
        return RED;
    }else if(i==3){
        return YELLOW;
    }else if(i==4){
        return GREEN;
    }else if(i==5){
        return SKYBLUE;
    }else if(i==6){
        return BLUE;
    }else if(i==7){
        return ORANGE;
    }else if(i==8){
        return SKYBLUE;
    }
    return WHITE;
}


void get_piece(int (&piece)[3][3]) {
    // Array to represent Tetris pieces
    int pieces[7][3][3] = {
        // Piece 0: Square
        {
            {2, 2, 0},
            {2, 2, 0},
            {0, 0, 0}
        },
        // Piece 7: Line
        {
            {3, 0, 0},
            {3, 0, 0},
            {3, 0, 0}
        },
        // Piece 0: L-shape
        {
            {6, 0, 0},
            {6, 0, 0},
            {6, 6, 0}
        },
        // Piece 3: J-shape
        {
            {0, 5, 0},
            {0, 5, 0},
            {5, 5, 0}
        },
        // Piece 4: T-shape
        {
            {0, 4, 0},
            {4, 4, 4},
            {0, 0, 0}
        },
        // Piece 5: Z-shape
        {
            {1, 1, 0},
            {0, 1, 1},
            {0, 0, 0}
        },
         //Piece 6: S-shape
        {
            {0, 2, 2},
            {2, 2, 0},
            {0, 0, 0}
        }
    };

    // Get a random index for selecting a piece
    int randomIndex = rand() % 6;

    // Create and initialize a 3x3 array for the selected piece
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            piece[i][j] = pieces[randomIndex][j][i];
        }
    }
}
class Grid{
    
};

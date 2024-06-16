#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

enum FaceDirection {
    LEFT, BOTTOM, RIGHT, TOP
};

class IBlock {
private:
    FaceDirection dir = LEFT;
    int8_t i;

    void turnHorizontal() {
        for (i = 0; i < 4; i++) {
            grid[i][1] = 0;
        }

        for (i = 0; i < 4; i++) {
            grid[2][i] = 1;
        }
    }
    void turnVertical() {
        for (i = 0; i < 4; i++) {
            grid[2][i] = 0;
        }
        for (i = 0; i < 4; i++) {
            grid[i][1] = 1;
        }
    }
public:
    int8_t x = 4;
    int8_t y = 0;

    bool grid[4][4] = {
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 0, 0}
    };

    void rotateRight() {
        if (dir == LEFT) {
            turnHorizontal();
            dir = BOTTOM;
        }
        else {
            turnVertical();
            dir = LEFT;
        }
    }

void rotateLeft() {
    rotateRight();
}
} currentPiece;

class Board {
public:
    bool grid[20][10];
} board;

bool gameOver = 0;
char key;

void setup();
void draw();
void input();
void logic();
void hiddenCursor();

void hiddenCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void setup() {
    hiddenCursor();

    cout << "  Tetris" << endl;
    cout << endl << "  Instructions:" << endl;
    cout << "   - Press Q to quit" << endl;
    cout << "   - Press J to rotate right" << endl;
    cout << "   - Press L to rotate right" << endl;
    cout << "   - Press A to move left or D to move right" << endl << endl;

    draw();
}

void draw() {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 8 });

    int8_t row, col;
    /*for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            bool currentBlockState = currentIBlock.grid[row][col];

            if (currentBlockState) cout << "[]";
            else cout << "  ";
        }
        cout << endl;
    }*/
    // rendering the board
    for (row = 0; row < 20; row++) {
        cout << "    <!";
        for (col = 0; col < 10; col++) {
            bool currentBoardCell = board.grid[row][col];
            if (currentBoardCell) {
                cout << "[]";
            }
            else {
                cout << " .";
            }
        }
        cout << "!>" << endl;
    }

    cout << "    <!";
    for (col = 0; col < 10; col++) {
        cout << "==";
    }
    cout << "!>";
}

void logic() {
    // merge matrices
    int8_t col, row;
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            board.grid[currentPiece.y + row][currentPiece.x + col] = currentPiece.grid[row][col];
        }
    }
}

int8_t getCurrentRealPieceX() {
    int8_t realPieceX;
    for (int8_t row = 0; row < 4; row++) {
        for (int8_t col = 0; col < 4; col++) {

            bool currentPieceCellOnBoard = board.grid[currentPiece.y + row][currentPiece.x + col];
            if (currentPieceCellOnBoard) {
                realPieceX = currentPiece.x + col;
            }
        }
    }
    return realPieceX;
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'q':
            gameOver = 1;
            break;
        case 'j':
            currentPiece.rotateRight();
            break;
        case 'l':
            currentPiece.rotateLeft();
            break;
        case 'a':
            if (getCurrentRealPieceX() > 0) currentPiece.x--;

            break;
        case 'd':
            if (getCurrentRealPieceX() < 9) currentPiece.x++;
            break;
        }
    }
}

int main()
{
    
    setup();
    while (!gameOver) {
        input();
        logic();
        draw();
    }

    system("pause");
    return 0;
}





/* grid:
 * bool grid[20][10]
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . . . . . . . .!>
 * <! . . . .[][] . . . .!>
 * <! . . . . .[][] . . .!>
 * <! . . . . . . . . . .!>
 * <! . .[] . . . . . . .!>
 * <![][][][] . . .[] . .!>
 * <![][] .[][][][][][] .!>
 * <!====================!>
 *   \/\/\/\/\/\/\/\/\/\/
 * 
 * pieces:
 * 1. [4][4] | 2. [3][3] | 3. [3][3] | 4. [2][2]
 *           |           |           |
 * []        | []        |   []      | [][]
 * []        | []        |   []      | [][]
 * []        | [][]      | [][]      |
 * []        |           |           |
 * 
 * 5. [3][3] | 6. [3][3] | 7. [3][3]
 *           |           |
 *   [][]    | [][][]    | [][]
 * [][]      |   []      |   [][]
 * 
 * a matriz nao precisa ter o dobro de dados apesar de cada quadrado serem dois colchetes
 * a matriz pode ter a quantidade ideal de espaços e os para cada uma célula é desenhado um par
 * de colchetes
 */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 10;

void initializeGrid(vector<vector<char>>& grid);
void displayGrid(const vector<vector<char>>& grid, bool showShips);
bool placeShip(vector<vector<char>>& grid, int x, int y, int size, char shipSymbol, bool isUser);
bool isValidLocation(int x, int y);
void computerPlaceShips(vector<vector<char>>& grid);
bool userPlaceShips(vector<vector<char>>& grid);
void computerTurn(vector<vector<char>>& userGrid, vector<vector<char>>& userGuessGrid, int& userShips);
void userTurn(vector<vector<char>>& computerGrid, vector<vector<char>>& computerGuessGrid, int& computerShips);
bool isGameOver(int userShips, int computerShips);
void printResult(int userShips, int computerShips);

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    vector<vector<char>> userGrid(SIZE, vector<char>(SIZE, '~'));
    vector<vector<char>> computerGrid(SIZE, vector<char>(SIZE, '~'));
    vector<vector<char>> userGuessGrid(SIZE, vector<char>(SIZE, '~'));
    vector<vector<char>> computerGuessGrid(SIZE, vector<char>(SIZE, '~'));

    int userShips = 5, computerShips = 5;

    initializeGrid(userGrid);
    initializeGrid(computerGrid);

    computerPlaceShips(computerGrid);
    userPlaceShips(userGrid);

    while (!isGameOver(userShips, computerShips)) {
        userTurn(computerGrid, computerGuessGrid, computerShips);
        if (isGameOver(userShips, computerShips)) break;

        computerTurn(userGrid, userGuessGrid, userShips);
    }

    printResult(userShips, computerShips);

    return 0;
}



void initializeGrid(vector<vector<char>>& grid) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            grid[i][j] = '~';
        }
    }
}

void displayGrid(const vector<vector<char>>& grid, bool showShips) {
    cout << "  ";
    for (int i = 0; i < SIZE; ++i) {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < SIZE; ++i) {
        cout << i << " ";
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j] == '#' && !showShips) {
                cout << "~ ";
            }
            else {
                cout << grid[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool isValidLocation(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

bool placeShip(vector<vector<char>>& grid, int x, int y, int size, char shipSymbol, bool isUser) {
    if (isUser) {
        cout << "Enter the starting coordinates (x y) for your " << size << "-length ship: ";
        while (!(cin >> x >> y)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter valid coordinates (x y): ";
        }
    }
    else {
        x = rand() % SIZE;
        y = rand() % SIZE;
    }

    cout << "Enter the direction (H for horizontal, V for vertical): ";
    char direction;
    while (!(cin >> direction) || (direction != 'H' && direction != 'V')) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter 'H' or 'V' for direction: ";
    }

    if ((direction == 'H' && x + size > SIZE) || (direction == 'V' && y + size > SIZE)) {
        cout << "Invalid placement. Try again.\n";
        return false;
    }

    for (int i = 0; i < size; ++i) {
        int newX = x + (direction == 'H' ? i : 0);
        int newY = y + (direction == 'V' ? i : 0);

        if (!isValidLocation(newX, newY) || grid[newY][newX] != '~') {
            cout << "Invalid placement. Try again.\n";
            return false;
        }
    }

    for (int i = 0; i < size; ++i) {
        int newX = x + (direction == 'H' ? i : 0);
        int newY = y + (direction == 'V' ? i : 0);
        grid[newY][newX] = shipSymbol;
    }

    return true;
}





void computerPlaceShips(vector<vector<char>>& grid) {
    placeShip(grid, 0, 0, 5, '#', false);  // Carrier
    placeShip(grid, 1, 0, 4, '#', false);  // Battleship
    placeShip(grid, 2, 0, 3, '#', false);  // Cruiser
    placeShip(grid, 3, 0, 3, '#', false);  // Submarine
    placeShip(grid, 4, 0, 2, '#', false);  // Destroyer
}

bool userPlaceShips(vector<vector<char>>& grid) {
    return placeShip(grid, 0, 0, 5, '#', true) &&
        placeShip(grid, 0, 0, 4, '#', true) &&
        placeShip(grid, 0, 0, 3, '#', true) &&
        placeShip(grid, 0, 0, 3, '#', true) &&
        placeShip(grid, 0, 0, 2, '#', true);
}

void computerTurn(vector<vector<char>>& userGrid, vector<vector<char>>& userGuessGrid, int& userShips) {
    int x, y;
    do {
        x = rand() % SIZE;
        y = rand() % SIZE;
    } while (userGuessGrid[y][x] == 'H' || userGuessGrid[y][x] == 'M');

    if (userGrid[y][x] == '#') {
        cout << "Computer's turn: HIT at (" << x << ", " << y << ")!\n";
        userGuessGrid[y][x] = 'H';
        userShips--;
    }
    else {
        cout << "Computer's turn: MISS at (" << x << ", " << y << ")!\n";
        userGuessGrid[y][x] = 'M';
    }

    displayGrid(userGuessGrid, false);
}

void userTurn(vector<vector<char>>& computerGrid, vector<vector<char>>& computerGuessGrid, int& computerShips) {
    cout << "\nYour turn!\n";
    displayGrid(computerGuessGrid, true);

    int x, y;
    do {
        cout << "Enter coordinates to fire (x y): ";
        while (!(cin >> x >> y)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter valid coordinates (x y): ";
        }
    } while (!isValidLocation(x, y) || computerGuessGrid[y][x] == 'H' || computerGuessGrid[y][x] == 'M');

    if (computerGrid[y][x] == '#') {
        cout << "HIT at (" << x << ", " << y << ")!\n";
        computerGuessGrid[y][x] = 'H';
        computerShips--;
    }
    else {
        cout << "MISS at (" << x << ", " << y << ")!\n";
        computerGuessGrid[y][x] = 'M';
    }

    displayGrid(computerGuessGrid, true);
}


bool isGameOver(int userShips, int computerShips) {
    return userShips == 0 || computerShips == 0;
}

void printResult(int userShips, int computerShips) {
    if (userShips == 0) {
        cout << "You lose! Computer wins!\n";
    }
    else {
        cout << "Congratulations! You win!\n";
    }
}

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

enum Direction {
    up, left_dir, down, right_dir
};

class Observer {
public:
    virtual void update(vector<vector<int>>& board) = 0;
    virtual ~Observer() {}
};

class ScoreManager : public Observer {
    int score;

public:
    ScoreManager() : score(0) {}

    void update(vector<vector<int>>& board) override {
        int tot = 0;
        for (auto& row : board)
            for (auto& item : row)
                tot += item;
        score = tot;
        cout << "Score: " << score << endl;
    }

    int getScore() const {
        return score;
    }
};

class PlayerData {
    string playerName;
    int score;

public:
    PlayerData(string name) : playerName(name), score(0) {}

    int getScore() const {
        return score;
    }

    void updateScore(int points) {
        score += points;
    }
};

class Game2048 {
    static Game2048* instance;
    vector<vector<int>> board;
    vector<Observer*> observers;

    Game2048() {
        board = vector<vector<int>>(4, vector<int>(4, 0));
        addRandomTile();
        addRandomTile();
    }

    void addRandomTile() {
        int value = (rand() % 2 + 1) * 2;
        int row, col;
        do {
            row = rand() % 4;
            col = rand() % 4;
        } while (board[row][col] != 0);

        board[row][col] = value;
    }

public:
    static Game2048* getInstance() {
        if (instance == nullptr)
            instance = new Game2048();
        return instance;
    }

    void move(Direction dir) {
        // Implementation of move function
        // ...

        addRandomTile();
        notifyObservers();
    }

    void addObserver(Observer* ob) {
        observers.push_back(ob);
    }

    void notifyObservers() {
        for (auto& ob : observers)
            ob->update(board);
    }

    void showBoard() const {
        for (const auto& row : board) {
            for (const auto& j : row) {
                if (j)
                    cout << j << "\t";
                else
                    cout << "-\t";
            }
            cout << endl;
        }
    }
};

Game2048* Game2048::instance = nullptr;

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};

class MoveLeft : public Command {
public:
    void execute() override {
        Game2048::getInstance()->move(left_dir);
    }
};

class MoveDown : public Command {
public:
    void execute() override {
        Game2048::getInstance()->move(down);
    }
};

class MoveUp : public Command {
public:
    void execute() override {
        Game2048::getInstance()->move(up);
    }
};

class MoveRight : public Command {
public:
    void execute() override {
        Game2048::getInstance()->move(right_dir);
    }
};

class CommandFactory {
public:
    Command* createCommand(Direction dir) {
        switch (dir) {
        case left_dir:
            return new MoveLeft();
        case down:
            return new MoveDown();
        case up:
            return new MoveUp();
        case right_dir:
            return new MoveRight();
        default:
            throw invalid_argument("Invalid direction");
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    Game2048* game = Game2048::getInstance();
    ScoreManager* manager = new ScoreManager();
    game->addObserver(manager);

    PlayerData* player = new PlayerData("Player 1");

    CommandFactory* commands = new CommandFactory();

    Command* moveLeft = commands->createCommand(left_dir);
    Command* moveDown = commands->createCommand(down);
    Command* moveRight = commands->createCommand(right_dir);
    Command* moveUp = commands->createCommand(up);

    game->showBoard();
    moveLeft->execute();
    game->showBoard();
    moveRight->execute();
    game->showBoard();
    moveUp->execute();
    game->showBoard();
    moveDown->execute();
    game->showBoard();

    delete game;
    delete manager;
    delete player;
    delete commands;
    delete moveLeft;
    delete moveDown;
    delete moveRight;
    delete moveUp;

    return 0;
}

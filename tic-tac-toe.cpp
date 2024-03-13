// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>

using namespace std;

enum GameState{
    PLAYER1,PLAYER2,DRAW,PENDING
};

class PlayerData{
    string name;
    
    public:
    PlayerData(string name):name(name){}
    
    string getName(){
        return name;
    }
    void greet(){
        cout<<name<<"'s turn"<<endl;
        cout<<"Enter coordinate of cell:"<<endl;
    }
};


class Observer{
    public:
    virtual void update(vector<string> board)=0;
    virtual int getTurn()=0;
    virtual void setTurn(int t)=0;
    virtual GameState getGamestate()=0;
    virtual ~Observer(){}
};

class Manager:public Observer{
    GameState gamestate=PENDING;
    int turn=0;
    public:
    GameState getGamestate(){
        return gamestate;
    }
    int getTurn(){
        return turn;
    }
    void setTurn(int t){
        turn =t;
    }
    void update(vector<string> board){
        for(auto &i:board)
            if(i[0] ==i[1] && i[1]==i[2]){
                if(i[0]=='X') gamestate=PLAYER1;
                else if(i[0]=='0') gamestate=PLAYER2;
                return;
            }
        for(int i=0;i<3;i++)
            if(board[0][i]==board[1][i] && board[1][i]==board[2][i]){
                if(board[0][i]=='X') gamestate=PLAYER1;
                else if(board[0][i]=='0')gamestate=PLAYER2;
                return;
            }
        if((board[0][0]==board[1][1] && board[2][2]==board[1][1]) || (board[2][0]==board[1][1] && board[0][2]==board[1][1])){
        if(board[1][1]=='X') gamestate=PLAYER1;
                else if(board[1][1]=='0') gamestate=PLAYER2;
                return;}
                int count=0;
                
                for(auto &i:board)
                    for(auto &j:i)
                    if(j=='-') count++;
                if(count==0)
                gamestate=DRAW;
    }
};




class Game{
    static Game* instance;
    vector<string> board={"---","---","---"};
    vector<Observer*> observers;
    Game(){
        
    }
     public:
     static Game* getInstance(){
         if(instance==nullptr)
         instance=new Game();
         return instance;
     }
     void printBoard(){
         for(auto &i:board){
             for(auto &j:i)
             cout<<j<<"\t";
             cout<<endl;
         }
     }
     
     void nextMove(int x,int y,char val){
         if((x<3 && x>=0 && y<3 && y>=0 ) || board[x][y]=='-'){
         board[x][y]=val;
         for(auto &ob:observers)
         if(val=='X')
         ob->setTurn(1);
         else ob->setTurn(0);
         }
         else{
             cout<<"Invalid move"<<endl;
         }
         printBoard();
         notifyObservers();
     }
     
     void addObserver(Observer* ob){
         observers.push_back(ob);
     }
     
     void notifyObservers(){
         for(auto &ob:observers)
         ob->update(board);
     }
};

Game* Game::instance = nullptr;


int main() {
    // Write C++ code here
    Game* game=Game::getInstance();
    Manager* manager=new Manager();
    game->addObserver(manager);
    game->printBoard();
    int turns=9;
    cout<<"Enter name of first player:"<<endl;
    string name1,name2;
    cin>>name1;
    cout<<"Enter name of second player:"<<endl;
    cin>>name2;
    
    PlayerData player1(name1);
    PlayerData player2(name2);
    
    while(manager->getGamestate()==PENDING){
        int turn=manager->getTurn();
        if(!turn) player1.greet();
        else player2.greet();
        int x,y;
        cin>>x>>y;
        if(!turn) game->nextMove(x,y,'X');
        else game->nextMove(x,y,'0');
    }
    
    if(manager->getGamestate()==PLAYER1) cout<<player1.getName()<<"wins!"<<endl;
    else if(manager->getGamestate()==PLAYER2) cout<<player2.getName()<<"wins!"<<endl;
    else cout<<"Game is draw."<<endl;
    
    delete game;
    delete manager;

    return 0;
}
#include <iostream>
#include <conio.h>
#include<windows.h>
#include<cstdlib>
#include<time.h>
#include<fstream>
#include<vector>
#include<algorithm>
#include<map>
#include<sstream>

using namespace std;

enum DIRECTION
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DEFAULT
};
DIRECTION moveDirection = DEFAULT;
int X = 20;
int Y = 20;
int snakeX = X/2;
int snakeY = Y/2;
int fruitX = rand() % X;
int fruitY = rand() % Y;
bool GAMEOVER = false;
int SCORE = 0;
int LENGTH = 1;
int PREVX[100];
int PREVY[100];


void draw()
{
    Sleep(20);
    printf("SCORE: %i \n",SCORE);
    //draws snake, fruit and boarder
    for (int i = 0; i < Y+1; i++)
    {    
        for (int j = 0; j < X+1; j++)
        {
            if(i == snakeX && j == snakeY){
                cout << 'O';                
                j++;
            }
            if(i == fruitX && j == fruitY){
                cout << 'F';
                j++;
            }

            if (i == 0 || i == Y)
            {
                cout << '*';
            }
            else if (j == 0 || j == X)
            {
                cout << '*';
            }
            else
            {
                bool print = false;
                for(int k = 0; k < LENGTH; k++){
                    if(i == PREVX[k] && j == PREVY[k])
                    {
                        cout << 'o';
                        print = true;
                    }
                        
                }
                if(!print)
                cout << ' ';
            }
        }
        std::cout << '\n';
    }

    switch(moveDirection){
    case UP:
        snakeX--;
        break;
    case DOWN:
        snakeX++;
        break;
    case LEFT:
        snakeY--;
        break;
    case RIGHT:
        snakeY++;
        break;
    }

}
void logic()
{
    if(snakeX > X) snakeX = 0;
    if(snakeX < 0) snakeX = X;
    if(snakeY > Y) snakeY = 0;
    if(snakeY < 0) snakeY = Y;


    if(snakeX == fruitX && snakeY == fruitY){
        fruitX = rand() % X;
        fruitY = rand() % Y;
        SCORE++;
        LENGTH++;
    }

    int prevX = PREVX[0];
    int prevY = PREVY[0];
    int prevX2, prevY2;

    PREVX[0] = snakeX;
    PREVY[0] = snakeY;

    for(int i = 1; i < LENGTH; i++){
        prevX2 = PREVX[i];
        prevY2 = PREVY[i];
        PREVX[i] = prevX;
        PREVY[i] = prevY;
        prevX = prevX2;
        prevY = prevY2;
    }

    for(int i = 1; i < LENGTH; i++){
        if(snakeX == PREVX[i] && snakeY == PREVY[i]){
            GAMEOVER = true;
        }
    }

    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':
            if(moveDirection == DOWN) break;
            moveDirection = UP;
            break;
        case 's':
            if(moveDirection == UP) break;
            moveDirection = DOWN;
            break;
        case 'a':
            if(moveDirection == RIGHT) break;
            moveDirection = LEFT;
            break;
        case 'd':
            if(moveDirection == LEFT) break;
            moveDirection = RIGHT;
            break;
        default:
            return;
            break;
        }
    }
}

bool IsEmpty(ifstream& f){
    return f.peek() == std::ifstream::traits_type::eof();
}

void highScores(){
    std::ifstream Scores("Scores.txt");

    std::string data;
    if(IsEmpty(Scores)) return;

    std::multimap<int, std::string> playerScores;
    std::vector<int> sortedScores;
    std::cout << "-----HIGH SCORES-----" << '\n';

    int i = -1;
    std::string name;
    while(Scores >> data){
        std::cout << data;
        
        if(i % 2 == 0){
            int score = std::stoi(data);
            playerScores.insert({score,name});
            sortedScores.push_back(score);
            cout << '\n';
        }else{
            name = data;
        }
        i++;
    }
    if(SCORE > sortedScores[sortedScores.size()-1] && sortedScores.size() < 10 ){
        cout << "\nCongradulations, you made a highscore!\nPlease input your name: ";
        std::string name;
        getline(std::cin,name);
        if(name.length() > 12){
            name.resize(12);
        }
        name.push_back(':');

        std::cout << '\n';
        playerScores.insert({SCORE,name});      
        vector<pair<string,int>> fs;  
        for(auto x : playerScores){
            fs.push_back({x.second ,x.first});
        }
        std::reverse(fs.begin(), fs.end());
        
        std::fstream append("Scores.txt");
        for(auto y : fs){
            cout << y.first << " " << y.second << '\n'; 
            string s = y.first + ' ' + to_string(y.second) + '\n';
            append << s;
        }
        Scores.close();

    }

}

int main()
{
    srand(time(0));
    cout << "RUNNING\n";
    while (GAMEOVER)
    {
        system("cls");
        draw();
        logic();
    }
    printf("GAME OVER, SCORE: %i\n", SCORE);
    highScores();
}
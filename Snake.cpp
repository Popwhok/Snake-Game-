#include <iostream>
#include <deque>
#include <conio.h>
#include <ctime>
#include <windows.h>

using namespace std;
class Game
{
private:
     int height, width;
     deque<pair<int, int>> snake = { {5, 3} };
     pair<int, int> food ;
     int direction = 'd';

     
public:
    Game(int h, int w) : height(h), width(w)
    {
        srand(time(0));
        food = { rand() % height + 1, rand() % width + 1 };
    }
    void SnakeMap();
    void MoveSnake();
    bool gameOver = false;
    int score = 0;
    
};

void Game::SnakeMap()
{
    system("cls");
    cout << score << endl;
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 1; i <= height; i++)
    {
        cout << "#";
        for (int j = 1; j <= width; j++)
        {
            bool isSnake = false;
            for (auto& el : snake)
            {
                if (el.first == i && el.second == j)
                {
                    cout << "O";
                    isSnake = true;
                    
                }
                
            }

            if (!isSnake)
            {
                if (food.first == i && food.second == j)
                    cout << "@";
                else
                    cout << " ";
            }
        }
        cout << "#" << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;
}
void Game::MoveSnake()
{
    if (_kbhit()) {
        switch (_getch()) {
        case 'w': if (direction != 's') direction = 'w'; break;
        case 's': if (direction != 'w') direction = 's'; break;
        case 'a': if (direction != 'd') direction = 'a'; break;
        case 'd': if (direction != 'a') direction = 'd'; break;
        case 'q': gameOver = true;break;
        }
    }

    auto head = snake.front();

    switch (direction) {
    case 'w': head.first--; break;
    case 's': head.first++; break;
    case 'a': head.second--; break;
    case 'd': head.second++; break;

    }
    
    if (head.first <= 0 || head.first >height || head.second <=0 || head.second >width)
    {
        gameOver = true;
    }
    for (auto snake : snake)
    {
        if (snake.first == head.first && snake.second == head.second)
        {
            gameOver = true;
        }
      
    }

    snake.push_front(head);
    
    if (head==food)
    {
        food = { rand() % height + 1, rand() % width + 1 };
        score +=2;
    }
    else
    {
        snake.pop_back();
    }
    
    
    

}


int main()
{
    setlocale(LC_ALL, "ru");
    Game game(7, 20);
    while (!game.gameOver) {
        game.SnakeMap();
        game.MoveSnake();
        if (game.score ==20)
        {
            Sleep(400);
        }
        else
        {
            Sleep(300);
        }
        
    }
}
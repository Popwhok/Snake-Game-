#include <iostream>
#include <deque>
#include <conio.h>
#include <ctime>
#include <windows.h>
enum Difficulty
{
    Easy = 300,
    Hard = 400
};
using namespace std;
class Game
{
private:
     int height, width;
     deque<pair<int, int>> snake = { {5, 3} };
     pair<int, int> food ;
     int direction = 'd';
     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


     
public:
    Game(int h, int w) 
    {
        height = h;
        width = w;
        srand(time(0));
        food = { rand() % height + 1, rand() % width + 1 };
    }
    void Menu();
    void SnakeMap();
    void MoveSnake();
    bool gameOver;
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
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                    std::cout << "O";
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                              
                          isSnake = true;
                    
                }
                
            }

            if (!isSnake)
            {
                if (food.first == i && food.second == j)
                {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    cout << "@";
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
                else
                {
                    cout << " ";
                }
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
            system("cls");
            cout << "GAME OVER" << endl;
            
    }
    for (auto snake : snake)
    {
        if (snake.first == head.first && snake.second == head.second)
        {
            gameOver = true;
            system("cls");
            cout << "GAME OVER" << endl;
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
void Game::Menu()
{
    
      cout  << "1.Запустить игру" << endl
            << "2.Выход" << endl;
    int a;
    cin >> a;
    
    switch (a)
    {
    case 1: 
    {
        cout << "Выберите сложность:" << endl
            << "1.Агуша гейминг" << endl
            << "2.Фанат боли" << endl;
        int diff, ttime;
        cin >> diff;
        if (diff == 1)
        {
            ttime = Difficulty::Easy;
        }
        else if (diff == 2)
        {
            ttime = Difficulty::Hard;
        }
        else
        {
            cout << "ВЫБЕРИТЕ СЛОЖНОСТЬ!!!";
        }
        gameOver = false;
        snake = { {5, 3} };
        direction = 'd';
        score = 0;
            while (!gameOver) {
                SnakeMap();
                MoveSnake();
                if (score == 20)
                {
                    Sleep(ttime+100);
                }
                else
                {
                    Sleep(ttime);
                }
                           
            }
            Menu();
            
           
       
       
        break;
    }
    case 2: return; break;
    }

}

int main()
{
    setlocale(LC_ALL, "ru");
    
    Game game(7, 20);
    cout << "Вас приветсвует змейка!" << endl;
    
        
        game.Menu();
   
}
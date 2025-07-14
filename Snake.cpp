#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>


#include <iostream>
#include <deque>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <fstream>


#pragma warning(disable: 4996)

using namespace std;
char Count = 'f';
SOCKET Connection;
char Mydirect = 'd';
char Enemydirect = 's';
char buffer[sizeof(int)];


enum Difficulty
{
    Easy = 300,
    Hard = 400
};
class Game
{
private:
    int height, width;
    deque<pair<int, int>> snake = { {10, 4} };
    deque<pair<int, int>> snake2 = { {3, 8} };
    pair<int, int> food;
    int direction = 'd';
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int record = 0;
    bool gameOver;
    int score = 0;
    ofstream res;


public:
    Game()
    {
        height = 0;
        width = 0;
        srand(time(0));


    }

    void Menu();
    void SnakeMap();
    void MoveSnake();
    void Single_Game();
    void SnakeMapNetwork();
    void MoveSnakeNetwork();
    int NetworkGame();

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

    if (head.first <= 0 || head.first > height || head.second <= 0 || head.second > width)
    {
        if (head.first <= 0 || head.first > height)
        {
            if (head.first <= 0)
            {
                head.first = height;
            }
            else
            {
                head.first = 0;
            }
        }
        if (head.second <= 0 || head.second > width)
        {
            if (head.second <= 0)
            {
                head.second = width;
            }
            else
            {
                head.second = 0;
            }
        }

    }
    for (auto snake : snake)
    {
        if (snake.first == head.first && snake.second == head.second)
        {
            gameOver = true;
            system("cls");
            cout << "GAME OVER" << endl;

            if (score > record)
            {
                record = score;
                cout << "Вы побили свой рекорд!!!" << endl;
                res.open("record.txt");
                res << "Best score: " << record;
                res.close();
            }
            else
            {
                cout << "Рекорд на данный момент = " << score << endl;
            }


        }

    }

    snake.push_front(head);

    if (head == food)
    {
        food = { rand() % height + 1, rand() % width + 1 };
        score += 2;
    }
    else
    {
        snake.pop_back();
    }




}
void Game::Menu()
{

    cout << endl << "1.Запустить игру" << endl
        << "2.Выход" << endl;
    int a;
    cin >> a;

    switch (a)
    {
    case 1:
    {
        cout << endl << "1. Одиночная игра" << endl
            << "2. Игра по сети" << endl;
        int b;
        cin >> b;
        switch (b)
        {
        case 1:
        {
            Single_Game();
            break;
        }
        case 2:
        {
            NetworkGame();
        }
        }

        break;
    }
    case 2:
    {
        return; break;
    }
    }
}
void Game::Single_Game()
{

    cout << "Выберите сложность:" << endl
        << "1.Агуша гейминг" << endl
        << "2.Фанат боли" << endl;
    int diff, ttime;
    cin >> diff;
    if (diff == 1)
    {
        height = 10;
        width = 20;

        ttime = Difficulty::Easy;
    }
    else if (diff == 2)
    {
        height = 7;
        width = 17;

        ttime = Difficulty::Hard;
    }
    else
    {
        cout << "ВЫБЕРИТЕ СЛОЖНОСТЬ!!!";
    }
    food = { rand() % height + 1, rand() % width + 1 };
    gameOver = false;
    snake = { {5, 3} };
    snake2 = { {3, 4} };
    direction = 'd';
    score = 0;
    while (!gameOver) {
        SnakeMap();
        MoveSnake();
        if (score == 20)
        {
            Sleep(ttime + 100);
        }
        else
        {
            Sleep(ttime);
        }

    }
    Single_Game();
}

int Game::NetworkGame()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0)
    {
        cout << "Error" << endl;
        exit(1);
    }
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
    {
        cout << "Error: failed connect to server" << endl;
        return 1;
    }
    cout << "connect" << endl;

    recv(Connection, &Count, sizeof(Count), 0);

    height = 20;
    width = 40;
    food = { rand() % height + 1, rand() % width + 1 };
    gameOver = false;
    snake = { {10, 4} };
    snake2 = { {3, 8} };
    direction = 'd';
    score = 0;
    while (!gameOver)
    {
        SnakeMapNetwork();
        MoveSnakeNetwork();
        Sleep(300);
    }

}
void Game::SnakeMapNetwork()
{
    system("cls");

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
            for (auto& el : snake2)
            {
                if (el.first == i && el.second == j)
                {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
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
void Game::MoveSnakeNetwork()
{
    if (Count == 'f')
    {
        if (_kbhit()) {
            switch (_getch()) {
            case 'w': if (direction != 's') direction = 'w', Mydirect = 'w'; break;
            case 's': if (direction != 'w') direction = 's';Mydirect = 's'; break;
            case 'a': if (direction != 'd') direction = 'a';Mydirect = 'a'; break;
            case 'd': if (direction != 'a') direction = 'd';Mydirect = 'd'; break;
            case 'q': gameOver = true;break;
            }
        }


        send(Connection, &Mydirect, sizeof(Mydirect), 0);
        recv(Connection, &Enemydirect, sizeof(Enemydirect), 0);
        if (snake.empty()) {

            gameOver = true;
            return;
        }
        auto head = snake.front();

        switch (direction) {
        case 'w': head.first--; break;
        case 's': head.first++; break;
        case 'a': head.second--; break;
        case 'd': head.second++; break;

        }

        if (head.first <= 0 || head.first > height || head.second <= 0 || head.second > width)
        {
            if (head.first <= 0 || head.first > height)
            {
                if (head.first <= 0)
                {
                    head.first = height;
                }
                else
                {
                    head.first = 0;
                }
            }
            if (head.second <= 0 || head.second > width)
            {
                if (head.second <= 0)
                {
                    head.second = width;
                }
                else
                {
                    head.second = 0;
                }
            }

        }
        for (auto snake : snake)
        {
            if (snake.first == head.first && snake.second == head.second)
            {
                gameOver = true;
                system("cls");
                cout << "GAME OVER" << endl;

                if (score > record)
                {
                    record = score;
                    cout << "Вы побили свой рекорд!!!" << endl;
                    res.open("record.txt");
                    res << "Best score: " << record;
                    res.close();
                }
                else
                {
                    cout << "Рекорд на данный момент = " << score << endl;
                }


            }

        }

        snake.push_front(head);

        if (head == food)
        {
            food = { rand() % height + 1, rand() % width + 1 };
            score += 2;
        }
        else
        {
            snake.pop_back();
        }


        if (snake2.empty()) {

            gameOver = true;
            return;
        }
        auto head2 = snake2.front();

        switch (Enemydirect) {
        case 'w': head2.first--; break;
        case 's': head2.first++; break;
        case 'a': head2.second--; break;
        case 'd': head2.second++; break;

        }

        if (head2.first <= 0 || head2.first > height || head2.second <= 0 || head2.second > width)
        {
            if (head2.first <= 0 || head.first > height)
            {
                if (head2.first <= 0)
                {
                    head2.first = height;
                }
                else
                {
                    head2.first = 0;
                }
            }
            if (head2.second <= 0 || head2.second > width)
            {
                if (head2.second <= 0)
                {
                    head2.second = width;
                }
                else
                {
                    head2.second = 0;
                }
            }
        }

        for (auto snake : snake2)
        {
            if (snake.first == head2.first && snake.second == head2.second)
            {
                gameOver = true;
                system("cls");
                cout << "GAME OVER" << endl;

                if (score > record)
                {
                    record = score;
                    cout << "Вы побили свой рекорд!!!" << endl;
                    res.open("record.txt");
                    res << "Best score: " << record;
                    res.close();
                }
                else
                {
                    cout << "Рекорд на данный момент = " << score << endl;
                }
            }
        }
        snake2.push_front(head2);

        if (head == food)
        {
            food = { rand() % height + 1, rand() % width + 1 };
            score += 2;
        }
        else
        {
            snake2.pop_back();
        }
    }

    else if (Count == 's')
    {
        if (_kbhit()) {
            switch (_getch()) {
            case 'w': if (direction != 's') direction = 'w', Mydirect = 'w'; break;
            case 's': if (direction != 'w') direction = 's';Mydirect = 's'; break;
            case 'a': if (direction != 'd') direction = 'a';Mydirect = 'a'; break;
            case 'd': if (direction != 'a') direction = 'd';Mydirect = 'd'; break;
            case 'q': gameOver = true;break;

            }
        }
        send(Connection, &Mydirect, sizeof(Mydirect), 0);
        recv(Connection, &Enemydirect, sizeof(Enemydirect), 0);

        auto head = snake2.front();

        switch (direction) {
        case 'w': head.first--; break;
        case 's': head.first++; break;
        case 'a': head.second--; break;
        case 'd': head.second++; break;

        }

        if (head.first <= 0 || head.first > height || head.second <= 0 || head.second > width)
        {
            if (head.first <= 0 || head.first > height)
            {
                if (head.first <= 0)
                {
                    head.first = height;
                }
                else
                {
                    head.first = 0;
                }
            }
            if (head.second <= 0 || head.second > width)
            {
                if (head.second <= 0)
                {
                    head.second = width;
                }
                else
                {
                    head.second = 0;
                }
            }

        }
        for (auto snake : snake2)
        {
            if (snake.first == head.first && snake.second == head.second)
            {
                gameOver = true;
                system("cls");
                cout << "GAME OVER" << endl;

                if (score > record)
                {
                    record = score;
                    cout << "Вы побили свой рекорд!!!" << endl;
                    res.open("record.txt");
                    res << "Best score: " << record;
                    res.close();
                }
                else
                {
                    cout << "Рекорд на данный момент = " << score << endl;
                }


            }

        }

        snake2.push_front(head);

        if (head == food)
        {
            food = { rand() % height + 1, rand() % width + 1 };
            score += 2;
        }
        else
        {
            snake2.pop_back();
        }


        auto head2 = snake.front();

        switch (Enemydirect) {
        case 'w': head2.first--; break;
        case 's': head2.first++; break;
        case 'a': head2.second--; break;
        case 'd': head2.second++; break;

        }

        if (head2.first <= 0 || head2.first > height || head2.second <= 0 || head2.second > width)
        {
            if (head2.first <= 0 || head.first > height)
            {
                if (head2.first <= 0)
                {
                    head2.first = height;
                }
                else
                {
                    head2.first = 0;
                }
            }
            if (head2.second <= 0 || head2.second > width)
            {
                if (head2.second <= 0)
                {
                    head2.second = width;
                }
                else
                {
                    head2.second = 0;
                }
            }

        }
        for (auto snake : snake)
        {
            if (snake.first == head2.first && snake.second == head2.second)
            {
                gameOver = true;
                system("cls");
                cout << "GAME OVER" << endl;

                if (score > record)
                {
                    record = score;
                    cout << "Вы побили свой рекорд!!!" << endl;
                    res.open("record.txt");
                    res << "Best score: " << record;
                    res.close();
                }
                else
                {
                    cout << "Рекорд на данный момент = " << score << endl;
                }


            }
        }
        snake.push_front(head2);

        if (head == food)
        {
            food = { rand() % height + 1, rand() % width + 1 };
            score += 2;
        }
        else
        {
            snake.pop_back();
        }


    }





}

int main()
{
    setlocale(LC_ALL, "ru");

    Game game;
    cout << "Вас приветсвует змейка!" << endl;
    game.Menu();

}
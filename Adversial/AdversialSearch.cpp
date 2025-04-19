#include <bits/stdc++.h>
#include "Mancala.cpp"
using namespace std;
#define ll int
#define sng string

void AI_vs_AI(Mancala *Game)
{
    while (!Game->isGameFinished())
    {
        cout << endl;
        cout << "Player " << Game->get_player_turn() << "'s turn" << endl;
        ll best_move = Game->getBestmove(DEPTH);
        Game->PerformMove(best_move);
        Game->PrintMancalaBoard();
        cout << endl << endl;
    }
}


void HUMAN_vs_AI(Mancala *game, ll player_number)
{
    ll current_box = -1;

    while (!game->isGameFinished())
    {
        cout << endl;

        if (game->get_player_turn() == player_number)
        {
            cout << "Player " << player_number << "'s turn" << endl<<endl;
            cout << "Enter move (1-6):" << endl;
            cin >> current_box;
            if (player_number == 2)
            {
                current_box = 13 - current_box;
            }
            else
            {
                current_box -= 1;
            }

            if ((player_number == 1 && current_box >= 0 && current_box <= 5) ||
                    (player_number == 2 && current_box >= 7 && current_box <= 12))
            {
                if (game->getMancala_Board()[current_box] > 0)
                {
                    game->PerformMove(current_box);
                }
                else
                {
                    cout << "Invalid Move. The selected box is empty. Retry." << endl;
                }
            }
            else
            {
                cout << "Invalid Move. Retry." << endl;
            }
        }
        else
        {
            cout << "AI's turn" << endl;
            game->PerformMove(game->getBestmove(DEPTH));
        }
        game->PrintMancalaBoard();
        cout << endl<< endl;
    }
}

int main()
{
    ll choice;
    ll player_number = 1;
    ll player1score = 0;
    ll player2score = 0;
    cout << "CHOICE 1:Human VS AI" << endl;
    cout << "CHOICE 2:AI VS AI" << endl;
    cout << "Enter your choice:" << endl;
    cin >> choice;
    GameMode = (choice == 1 || choice == 2) ? choice : -1;
    if (GameMode == -1)
    {
        cout << "Invalid Mode" << endl;
        return 0;
    }
    if (GameMode == 1)
    {

        cout << "Choose Player number" << endl;
        cout << "1. Player 1" << endl;
        cout << "2. Player 2" << endl;
        cin >> choice;
        cout << "Select Heuristic Function for player 1:" << endl;
        cin>>func1;
        cout << "Select Heuristic Function for player 2:" << endl;
        cin>>func2;
        if (choice == 1 || choice == 2)
        {
            player_number = choice;
        }
        else
        {
            cout << "Invalid Choice" << endl;
            return 0;
        }
    }
    else if (GameMode == 2)
    {
        cout << "Select Heuristic Function for player 1:" << endl;
        cin>>func3;
        cout << "Select Heuristic Function for player 2:" << endl;
        cin>>func4;
        cout << "...... Start AI Game......   " << endl<<endl;

    }

    else
    {
        cout << "Invalid GameMode" << endl;
        return 0;
    }


    Mancala *Game = new Mancala();

    Game->PrintMancalaBoard();
    cout << endl
         << endl;
    if (GameMode == 1)
    {
        HUMAN_vs_AI(Game, player_number);
    }
    else
    {
        AI_vs_AI(Game);
    }

    cout << "Game Over" << endl;
    player1score = Game->getPlayerScore(1);
    player2score = Game->getPlayerScore(2);
    cout << "Player 1 Score: " << player1score << endl;
    cout << "Player 2 Score: " << player2score << endl;
    if (player1score > player2score)
    {
        cout << "Player 1 Wins" << endl;
    }
    else if (player2score > player1score)
    {
        cout << "Player 2 Wins" << endl;
    }
    else
    {
        cout << "Draw" << endl;
    }
}

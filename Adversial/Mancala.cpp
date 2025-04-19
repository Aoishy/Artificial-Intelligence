#include <bits/stdc++.h>
using namespace std;
#define ll int
#define sng string
#define DEPTH 7
#define W1 2
#define W2 3
#define W3 5
#define W4 7
#define W5 4

ll GameMode = 0;
ll func1=0,func2=0,func3=0,func4=0;
class Mancala
{
private:
    ll player_number;

    vector<ll> mancala_board;
    ll capture_stones_player1;
    ll capture_stones_player2;
    ll additional_move_player1;
    ll additional_move_player2;

public:
    Mancala(ll initialStones = 4)
    {
        for (int i = 0; i < 14; i++)
        {
            mancala_board.push_back((i == 6 || i == 13) ? 0 : initialStones);
        }
        this->player_number = 1;
        additional_move_player1 = additional_move_player2 = 0;
        capture_stones_player1 = capture_stones_player2 = 0;
    }
    Mancala(Mancala *mancala_game)
    {
        mancala_board = mancala_game->getMancala_Board();
        player_number = mancala_game->get_player_turn();
        capture_stones_player1 = mancala_game->get_CapturedStones_Player1();
        capture_stones_player2 = mancala_game->get_CapturedStones_Player2();
        additional_move_player1 = mancala_game->get_AdditionalMove_Player1();
        additional_move_player2 = mancala_game->get_AdditionalMove_Player2();
    }

    vector<ll> getMancala_Board()
    {
        return mancala_board;
    }

    ll get_player_turn()
    {
        return player_number;
    }
    ll get_AdditionalMove_Player1()
    {
        return additional_move_player1;
    }
    ll get_AdditionalMove_Player2()
    {
        return additional_move_player2;
    }

    ll get_CapturedStones_Player1()
    {
        return capture_stones_player1;
    }
    ll get_CapturedStones_Player2()
    {
        return capture_stones_player2;
    }

    void PrintMancalaBoard()
    {

        for (int i = 12; i > 6; i--)
            cout << "      " << mancala_board[i] << "  ";
        cout << endl;
        cout << " " << mancala_board[13] << "                                                           " << mancala_board[6] << endl
             << endl;
        for (int i = 0; i < 6; i++)
            cout << "      " << mancala_board[i] << "  ";
        cout << endl;
    }

    ll getPlayerScore(ll player)
    {
        return (player == 1) ? mancala_board[6] : mancala_board[13];
    }

    bool isGameFinished()
    {
        bool player1_Empty = true;
        int i = 0;
        while (i < 6)
        {
            if (mancala_board[i] != 0)
            {
                player1_Empty = false;
                break;
            }
            i++;
        }
        if (player1_Empty)
        {
            i = 7;
            while (i < 13)
            {
                mancala_board[13] += mancala_board[i];
                mancala_board[i] = 0;
                i++;
            }
            return true;
        }

        bool player2_Empty = true;
        i = 7;
        while (i < 13)
        {
            if (mancala_board[i] != 0)
            {
                player2_Empty = false;
                break;
            }
            i++;
        }

        if (player2_Empty)
        {
            i = 0;
            while (i < 6)
            {
                mancala_board[6] += mancala_board[i];
                mancala_board[i] = 0;
                i++;
            }
            return true;
        }

        return false;
    }

    vector<ll> fetchMoves()
    {
        vector<ll> playing_moves;
        int start = (player_number == 1) ? 0 : 7;
        int end = (player_number == 1) ? 6 : 13;
        for (int i = start; i < end; i++)
        {
            if (mancala_board[i] != 0)
            {
               playing_moves.push_back(i);
            }
        }

        return playing_moves;
    }
    void PerformMove(ll current_box)
    {
        ll stones = mancala_board[current_box];
        mancala_board[current_box] = 0;
        for (ll i = stones; i > 0; i--)
        {
            current_box = (current_box + 1) % 14;
            if ((player_number == 1 && current_box == 13) || (player_number == 2 && current_box == 6))
            {
                i++;
                continue;
            }
            if (i == 1 && mancala_board[current_box] == 0)
            {
                if (player_number == 1)
                {
                    if (current_box >= 0 && current_box <= 5)
                    {
                        capture_stones_player1 += mancala_board[12 - current_box];
                        mancala_board[6] += mancala_board[12 - current_box] + 1;
                        mancala_board[12 - current_box] = 0;
                    }
                    else if (current_box >= 6 && current_box <= 12)
                    {
                        mancala_board[current_box] += 1;
                    }
                }
                else if (player_number == 2)
                {
                    if (current_box >= 7 && current_box <= 12)
                    {
                        capture_stones_player2 += mancala_board[12 - current_box];
                        mancala_board[13] += mancala_board[12 - current_box] + 1;
                        mancala_board[12 - current_box] = 0;
                    }
                    else if (current_box == 13 || (current_box >= 0 && current_box <= 5))
                    {
                        mancala_board[current_box] += 1;
                    }
                }
            }

            else
            {
                mancala_board[current_box] += 1;
            }
        }
        if (player_number == 1 && current_box == 6)
        {
            additional_move_player1 += 1;
        }
        else if (player_number == 2 && current_box == 13)
        {
            additional_move_player2 += 1;
        }
        else
        {
            player_number = 3 - player_number;
        }
    }
    ll heuristic_func1(Mancala *Game)
    {
        ll result = 0;
        vector<ll> curr_mancala = Game->getMancala_Board();
        ll player_Score = curr_mancala[Game->get_player_turn() == 1 ? 6 : 13];
        ll opponent_Score = curr_mancala[Game->get_player_turn() == 1 ? 13 : 6];
        result = player_Score - opponent_Score;
        return result;
    }

    ll heuristic_func2(Mancala *Game)
    {
        ll stone_of_player1 = 0;
        ll stone_of_player2 = 0;
        ll result = 0;
        vector<ll> curr_mancala = Game->getMancala_Board();
        for (int i = 0; i < 6; i++)
        {
            stone_of_player1 += curr_mancala[i];
        }
        for (int i = 7; i < 13; i++)
        {
            stone_of_player2 += curr_mancala[i];
        }
        if (Game->get_player_turn() == 1)
        {
            result = W1 * (curr_mancala[6] - curr_mancala[13]) + W2 * (stone_of_player1 - stone_of_player2);
        }
        else
        {
            result = W1 * (curr_mancala[13] - curr_mancala[6]) + W2 * (stone_of_player2 - stone_of_player1);
        }
        return result;
    }

    ll heuristic_func3(Mancala *Game)
    {
        ll result = 0;
        ll additional_move = (Game->get_player_turn() == 1) ? Game->get_AdditionalMove_Player1() : Game->get_AdditionalMove_Player2();
        result = heuristic_func2(Game) + W3 * additional_move;
        return result;
    }

    ll heuristic_func4(Mancala *Game)
    {
        ll result = 0;
        ll capturestone = (Game->get_player_turn() == 1 ? Game->get_CapturedStones_Player1() : Game->get_CapturedStones_Player2());
        result = heuristic_func3(Game) + W4 * capturestone;
        return result;
    }

    ll get_heuristic_func()
    {
        ll heuristic = 0;
        ll result = 0;
        if (GameMode == 2)
        {
            if (player_number == 1)
            {
                heuristic = func3;
            }
            else
            {
                heuristic = func4;
            }
        }
        else if(GameMode==1)
        {
            if (player_number == 1)
            {
                heuristic = func1;
            }
            else
            {
                heuristic = func2;
            }

        }
        Mancala *mancala = new Mancala(this);
        switch (heuristic)
        {
        case 1:
            result = heuristic_func1(mancala);
            break;
        case 2:
            result = heuristic_func2(mancala);
            break;
        case 3:
            result = heuristic_func3(mancala);
            break;
        case 4:
            result = heuristic_func4(mancala);
            break;
        default:
            result = heuristic_func1(mancala);
            break;
        }
        return result;
    }

    ll MinMax(ll depth, ll alpha, ll beta, bool maxplayer)
    {
        if (depth == 0 || isGameFinished())
        {
            return get_heuristic_func();
        }
        if (maxplayer)
        {
            ll max_value = INT_MIN;
            vector<ll> playing_moves = fetchMoves();
            for (ll move : playing_moves)
            {
                Mancala *nextGame = new Mancala(this);
                nextGame->PerformMove(move);
                bool isSamePlayerTurn = nextGame->get_player_turn() == player_number;
                ll ans = 0;
                if (isSamePlayerTurn)
                {
                    ans = nextGame->MinMax(depth, alpha, beta, true);
                }
                else
                {
                    ans = nextGame->MinMax(depth - 1, alpha, beta, false);
                }
                max_value = max(max_value, ans);
                alpha = max(alpha, ans);
                if (beta <= alpha)
                {
                    break;
                }
            }
            return max_value;
        }
        else
        {
            ll min_value = INT_MAX;
            vector<ll> playing_moves = fetchMoves();
            for (ll move : playing_moves)
            {
                Mancala *nextGame = new Mancala(this);
                nextGame->PerformMove(move);
                ll ans = 0;
                bool isSamePlayerTurn = (nextGame->get_player_turn() == player_number);
                if (isSamePlayerTurn)
                {
                    ans = nextGame->MinMax(depth, alpha, beta, false);
                }
                else
                {
                    ans = nextGame->MinMax(depth - 1, alpha, beta, true);
                }
                min_value = min(min_value, ans);
                beta = min(beta, ans);
                if (beta <= alpha)
                {
                    break;
                }
            }
            return min_value;
        }
    }

    ll getBestmove(ll depth)
    {
        ll max_value = INT_MIN;
        ll bestmove = -1;
        vector<ll> playing_moves = fetchMoves();
        for (ll move : playing_moves)
        {
            Mancala *nextGame = new Mancala(this);
            nextGame->PerformMove(move);
            ll ans = nextGame->MinMax(depth, INT_MIN, INT_MAX, true);
            if (ans > max_value)
            {
                max_value = ans;
                bestmove = move;
            }
        }
        return bestmove;
    }
};

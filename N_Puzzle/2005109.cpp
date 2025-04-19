#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define sng string

map<vector<ll>,ll> grid_id;
ll cn=2;
vector<vector<ll>> current_board;

ll Hamming(vector<ll>curr_state,vector<ll>des)
{
    ll c=0;
    for(ll i=0;i<curr_state.size()-1;i++)
    {
       
        c+=(curr_state[i]!=des[i]);
       
    }
    return c;
}

ll Manhattan(vector<ll>curr_state,ll k,vector<ll>row,vector<ll>column)
{
    ll ans=0;
    for(ll i=0;i<curr_state.size();i++)
    {       
        if(curr_state[i]==0)
        {
            continue;
        }
        ll x=i/k;
        ll y=i%k;
        ans+=abs(x-row[curr_state[i]])+abs(y-column[curr_state[i]]);

    }
    return ans;
}


bool solveable(ll position_zero,ll k,vector<ll>begining)
{
    ll inversion_count=0;
    ll blank;
    ll row_position=position_zero/k;
    ll col_position=position_zero%k;
    for(ll i=0;i<k*k;i++)
    {
        if(begining[i]==0)
        {      
            continue;
        }
        for(ll j=0;j<i;j++){
            if(begining[j]==0)
            {
                continue;
            }
            if(begining[j]>begining[i])
            {
                inversion_count++;
            }
        }
    }
    if(k%2==1 && inversion_count%2==1)
    {
        return false;
    }
    if(k%2==1 && inversion_count%2==0)      
    {
        return true;
        
    }
   if(k%2==0 && ((row_position%2==0 && inversion_count%2==1)||(row_position%2==1 && inversion_count%2==0)))
    {
        return true;
    }
   return false; 
}


vector<ll>dis;
vector<ll>parent;

void clean(ll explore,ll expand,vector<ll>begining,vector<ll>destination)
{
    explore=0;
    expand=0;
    grid_id.clear();
    current_board.clear();
    dis.clear();
    parent.clear();
    cn=2;
    dis.resize(3, -1);   
    dis[0] = dis[1] = 0; 
    parent.resize(3, -1);
    grid_id[begining]=1;
    grid_id[destination]=2;
    current_board = {begining, begining, destination};


}

bool Invalid(ll row,ll col,ll k)
{
 if(row>=k||col>=k||row<0||col<0)
 {
    return true;

 }
 return false;
}


pair<ll, ll> directions[] = {
    {1, 0},   
    {-1, 0},  
    {0, 1},   
    {0, -1}  
};

ll findZeroPosition(vector<ll>& current_board)
 {
    for (ll i=0; i<current_board.size();i++) {
        if(current_board[i]==0){
            return i;
        }
    }
    return -1;
}


ll calculateHeuristic(vector<ll> temp,vector<ll>destination,ll choice,ll k,vector<ll>row,vector<ll>column)
 {
    return (choice==0)?Hamming(temp,destination):Manhattan(temp,k,row,column);
}

void Astar(ll k,int choice, ll explore,ll expand,vector<ll> begining,vector<ll> destination,vector<ll>row,vector<ll>column)
{
    clean(explore,expand,begining,destination);
    priority_queue<pair<ll,ll>,vector<pair<ll,ll> >,greater<pair<ll,ll> > > pq;
    pq.push({0,1});
    while (!pq.empty())
    {
        ll a=pq.top().second;
        explore+=1;
       if(a==2)
        {
            if(choice==0)
                cout<<"Hamming Distance : "<<endl;
            else
                cout<<"Manhattan Distance : "<<endl;
            cout<<"Explore: "<<explore<<endl;
            cout<<"Expand : "<<expand<<endl;
            cout<<endl;
            return;
        }  
       pq.pop();
       ll zero_pos=findZeroPosition(current_board[a]);
       ll row_pos=zero_pos/k;
       ll col_pos=zero_pos%k; 
           for (const auto& dir : directions) {
               ll new_row_pos = row_pos + dir.first;
               ll new_col_pos = col_pos + dir.second;
            if(Invalid(new_row_pos,new_col_pos,k))
            {
                continue;
            }
            ll new_pos=new_row_pos*k+new_col_pos;
            vector<ll>temp=current_board[a];
            swap(temp[new_pos],temp[zero_pos]);
            if(!grid_id.count(temp))
            {
                cn++;
                grid_id[temp]=cn;
                current_board.push_back(temp);
                dis.push_back(-1);
                parent.push_back(-1);

            }
            ll t=grid_id[temp];
            if(dis[t]==-1 || dis[t]>dis[a]+1)
            {               
                dis[t]=dis[a]+1;
                parent[t]=a;
            ll priority=dis[t]+calculateHeuristic(temp, destination, choice, k, row, column);
                pq.push({priority, t});
                expand++;
            }

       }


    }


}

ll totalStep=0;

void printBoard(ll node,ll k,ll step)
{
      ll n=k*k;
    cout<<"Step: "<<totalStep-step<<endl;
    for(int i=0;i<n;i++){
        if( current_board[node][i] == 0 )
            cout<<"* ";
        else
            cout<<current_board[node][i]<<" ";
        if( (i+1)%k == 0 ) cout<<endl;
}
}

void printSolve(ll node,ll step,ll k){
    totalStep++;
    if( node!=1 ) 
   {printSolve(parent[node],step+1,k);
     printBoard(node,k,step);
      cout<<endl;
   }
    
   
}

int main()
{
    ll n,k;
    cin>>k;
    n=k*k;
    int position_of_zero=0;
    vector<ll>begining;
    vector<ll>destination;
    for(ll i=0;i<n;i++)
    {
        sng s;
        cin>>s;
        ll digit=0;
        if(s=="*")
        {
            position_of_zero=i;
            begining.push_back(0);
        }
        else{
            digit=stoll(s);
            begining.push_back(digit);


        }

    }
    ll grid=1;
    vector<ll>row(n+1);
    vector<ll>column(n+1);
    for(ll i=0;i<k;i++)
    {
        for(ll j=0;j<k;j++)
        {
            destination.push_back(grid);
            row[grid]=i;
            column[grid]=j;
            grid++;
        }
    }
    ll last_gird=destination.size()-1;
    destination[last_gird]=0;
    row[0]=k-1;
    column[0]=k-1;

    

    if(!solveable(position_of_zero,k,begining))
    {
        cout<<"Not Solvable"<<endl;
        return 0;
    }
   
    ll s=1;
    ll explore=0;
    ll expand=0;
    Astar(k,0,explore,expand,begining,destination,row,column);
    Astar(k,1,explore,expand,begining,destination,row,column);
    printSolve(2,s,k);

}

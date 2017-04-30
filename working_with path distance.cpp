#include<iostream>
#include<vector>
#include<string>
#include<list>

using namespace std;

struct train
{
string trainname;
int trainno;
int speed;
};

struct halt
{
train t;
int arr;
int dep;
};

struct station
{
string name;
int platforms;
vector<halt> halts;
};

/*struct route               //  elements of matrix L
{
  vector<int> intermediate_stations;
};*/

void minplatforms(station &s)
{
vector<int> arrival;
vector<int> departure;
for(size_t i=0;i<s.halts.size();i++)
{
arrival.push_back(s.halts[0].arr);
int j=i-1;
while((s.halts[j].arr>s.halts[i].arr)&&(j>=0))
{
arrival[j+1]=arrival[j];
j--;
}
arrival[j+1]=s.halts[i].arr;
}
for(size_t i=0;i<s.halts.size();i++)
{
departure.push_back(s.halts[i].dep);
int j=i-1;
while((s.halts[j].dep>s.halts[i].dep)&&(j>=0))
{
departure[j+1]=departure[j];
j--;
}
departure[j+1]=s.halts[i].dep;
}
size_t i=1,j=0;              //i - index of arrival, j - index of departure
int present_platforms=1;
int max=1;
while(i<arrival.size()&&j<departure.size())
{
if(arrival[i]<departure[j])
{
present_platforms++;
if(present_platforms>max) max=present_platforms;
i++;
}
else
{
present_platforms--;
j++;
}
}
s.platforms=max;

}


struct next_station
{
 station destination;
  int distance;
};

struct graph_inputs
    {
      station station_node;
      list<next_station> edges;
    };

struct graph
{
  vector<graph_inputs> st;

  void createGraphNodes(int a, station s[])
  {
   st.resize(a);
    int arrayIndex = 0;
    for( vector<graph_inputs>::iterator vectorIterator = st.begin();vectorIterator != st.end();
        vectorIterator++, arrayIndex++)
    {
      (*vectorIterator).station_node=s[arrayIndex];
    }

  }
  void addEdge(station source, station destination, int distance)
  {
   //Iterate through the ‘vectorList’ for adding an edge
    next_station neighbour_info;
    neighbour_info.destination=destination;
    neighbour_info.distance=distance;
    for( vector<graph_inputs>::iterator vectorIterator = st.begin();
        vectorIterator != st.end();vectorIterator++)
    {
       string check=(*vectorIterator).station_node.name;
      if ( check == source.name )
       {
       //source node found
        (*vectorIterator).edges.push_back(neighbour_info);
        return; //path added
      }
    } //End of for
    cout<<"Station doesn't exist"<<endl;
    return;
  }

  void removeEdge(string source, string destination)
  {
   //Iterate through the ‘vectorList’ for removing an edge
   for( vector<graph_inputs>::iterator vectorIterator = st.begin();
        vectorIterator != st.end();vectorIterator++)
    {
      string check=(*vectorIterator).station_node.name;
      if (  check == source )
      {
      //source node found
      for(list<next_station>::iterator listIterator = (*vectorIterator).edges.begin();
           listIterator != (*vectorIterator).edges.end();listIterator++)
           {
          if(((*listIterator).destination).name==destination)
           {
           (*vectorIterator).edges.erase(listIterator);
           return;
           }
           }
         //path removed
      }
    }
     cout<< "Connection doesn't exist"<<endl;
     return;
  }
  void printoutgoing(){
    for( vector<graph_inputs>::iterator vectorIterator = st.begin();
        vectorIterator != st.end();vectorIterator++)
    {

      cout << (*vectorIterator).station_node.name << ": ";
      for(list<next_station>::iterator listIterator = (*vectorIterator).edges.begin() ;
      listIterator != (*vectorIterator).edges.end(); listIterator++){
        cout << (*listIterator).destination.name << " ";
      }
      cout << endl;
    }
  }
  int check_connection(string source,string destination)
  {
  int k;
  int min=12345;
    for( vector<graph_inputs>::iterator vectorIterator = st.begin();
        vectorIterator != st.end();vectorIterator++)
    {

      if((*vectorIterator).station_node.name==source)
      {
        for( list<next_station>::iterator listIterator = (*vectorIterator).edges.begin();
        listIterator != (*vectorIterator).edges.end(); listIterator++){
            string check=(*listIterator).destination.name;
          //cout<<"not equal"<<endl;
          if(check==destination)
             {
              //  cout<<"checked"<<endl;
               k=((*listIterator).distance);
               if(k<min) min=k;
             }
         // cout<<(*listIterator).distance;
        }
        //cout<<"Connection doesn't exist"<<endl;
        return min;

      }
    }
    // Source_station doesn't exist
    return 0;
  }
};
//code for W matrix of floyd marshall

  vector<vector<int> > f_w_shortest_path(graph g1,vector<vector<vector<int> > > &L)
  {
  size_t n=g1.st.size();
  vector<vector<int> > W;
  L.resize(n);
    W.resize(n);
    for(size_t i=0;i<n;i++)
    {
      W[i].resize(n);
      L[i].resize(n);
    }

    for(size_t i=0;i<n;i++)
    {
      for(size_t j=0;j<n;j++)
      {
        if(i==j)
        {
          //cout<<i<<j<<endl;
          W[i][j]=0;
          L[i][j].push_back(i);
        }
        if(i!=j)
        {
          //cout<<i<<j<<endl;
          W[i][j]=g1.check_connection(g1.st[i].station_node.name,g1.st[j].station_node.name);
          if(W[i][j]==12345) L[i][j].push_back(-1);
          else
          {
          L[i][j].push_back(i);
          }
        }
      }
    }
   // cout<<"entered shortest distance"<<endl;
    //cout<<n<<endl;
    vector<vector<int> > D;
   // vector<vector<route> > L;
    D=W;
   // cout<<D[0][1]<<endl;


     //cout<<"initialized"<<endl;
  for(size_t k=0;k<n;k++)
    {
        for(size_t i=0;i<n;i++)
      {
          for(size_t j=0;j<n;j++)
        {
           if(D[i][j]>D[i][k]+D[k][j])
            {
             if(D[i][j]==12345)
             {
             L[i][j].pop_back();
             L[i][j].push_back(i);
             }
              D[i][j]=D[i][k]+D[k][j];
              L[i][j]=L[i][k];
              for(size_t l=0;l<L[k][j].size();l++)
              {
              L[i][j].push_back(L[k][j][l]);
              }
              // stores updated path between stations corresponding to i and j
              //cout<<i<<"th time computed"<<endl;
            }
        }
      }
    }
    for(size_t i=0;i<n;i++)
    {
    for(size_t j=0;j<n;j++)
    {
    if(D[i][j]!=12345) L[i][j].push_back(j);
    }
    }


    return D;

  }

  void platform_manager()
  {
    station s;
    cout<<"Give the name of the station"<<endl;
    cin>>s.name;
    cout<<"Give number of halts at the station"<<endl;
    int noofhalts;
    cin>>noofhalts;
    for(int j=0;j<noofhalts;j++)
    {
      halt h;
      cout<<"give the train name,train number,times of arrival and departure of the "<<j<<"th train"<<endl;
      cin>>h.t.trainname>>h.t.trainno>>h.arr>>h.dep;
      s.halts.push_back(h);
    }
    minplatforms(s);
    cout<<s.name<<" "<<s.platforms<<endl;
  }


  void min_distance_path(graph g1,vector<vector<int> > shortest_path_matrix,vector<vector<vector<int> > > L)
  {
    string source_station,destination_station;

    cout<<"Give source station name"<<endl;
    cin>>source_station;
    cout<<"Give destination station name"<<endl;
    cin>>destination_station;

    int i=0,j=0;
    for( vector<graph_inputs>::iterator vectorIterator = g1.st.begin();
        vectorIterator != g1.st.end();vectorIterator++)
    {
       string check=(*vectorIterator).station_node.name;
      if ( check == source_station ) { //source node found
        break;
      }
      i++;
    } //End of for
    for( vector<graph_inputs>::iterator vectorIterator = g1.st.begin();
        vectorIterator != g1.st.end();vectorIterator++)
    {
       string check=(*vectorIterator).station_node.name;
      if ( check == destination_station ) { //source node found
        break;
      }
      j++;
    } //End of for
    if(shortest_path_matrix[i][j]==12345) cout<<"Sorry, no route available!"<<endl;
    else{
    cout<<"Shortest distance is "<<shortest_path_matrix[i][j]<<endl;
    cout<<"Shortest path from "<<source_station<<" to "<<destination_station<<" is:"<<endl;

        for(vector<int>::iterator vectorIterator = L[i][j].begin();
        vectorIterator!=L[i][j].end(); vectorIterator++)
          {

            int m=(*vectorIterator);

            cout<<g1.st[m].station_node.name<<" ";
           }

      cout<<endl;
    }

  }



int main() {
  // your code goes here
  //Write your code here
int no_of_stations;
  cout<<"give no of stations"<<endl;
cin>>no_of_stations;
station s[no_of_stations];
for(int i=0;i<no_of_stations;i++)
{
  cout<<"give "<<i<<"th station name"<<endl;
  cin>>s[i].name;
}
    graph g1;
    g1.createGraphNodes(no_of_stations,s);

  while(true)
  {
    station source_station,destination_station;
    int distance;
    cout<<"For the graph creation enter following details else press \"done\" "<<endl;
    cout<<"Give source name"<<endl;
    cin>>source_station.name;
    if(source_station.name=="done") break;
    cout<<"Give destination name"<<endl;
    cin>>destination_station.name;
    cout<<"Give distance between source and destination"<<endl;
    cin>>distance;
    g1.addEdge(source_station,destination_station,distance);
  }
    //cout<<"return from while"<<endl;
      g1.printoutgoing();

  vector<vector<vector<int> > > L;
  vector<vector<int> > shortest_path_matrix;
  //cout<<"matrix created"<<endl;
  shortest_path_matrix = f_w_shortest_path(g1,L);


  //cout<<"shortest path matrix algorithm worked"<<endl;

  while(true)
  {
    string user_input;
    cout<<"press '1' for min no of platforms;'2' for min distance between two stations;'3' for removing a connection;'4' for adding a connection"
    <<endl;
    cin>>user_input;
    if(user_input=="1")
    {
      platform_manager();
    }
    else if(user_input=="2")
    {
      min_distance_path(g1,shortest_path_matrix,L);
    }
    else if(user_input=="3")
    {
    cout<<"Enter the source"<<endl;
    string src,dest;
    cin>>src;
    cout<<"Enter the destination"<<endl;
    cin>>dest;
    g1.removeEdge(src,dest);
    g1.printoutgoing();
     L.resize(0);
    shortest_path_matrix = f_w_shortest_path(g1,L);
    }
    else if(user_input=="4")
    {
    station src,dest;
    int distance;
    cout<<"Give source name"<<endl;
    cin>>src.name;
    cout<<"Give destination name"<<endl;
    cin>>dest.name;
    cout<<"Give distance between source and destination"<<endl;
    cin>>distance;
    g1.addEdge(src,dest,distance);
    g1.printoutgoing();
    }
    else
      cout<<"sorry wrong input"<<endl;
  }
/*for(size_t i=0;i<s[i].trains.size();i++)
{
cout<<s[0].trains[i].trainname<<" "<<s[0].trains[i].arr<<" "<<s[0].trains[i].dep<<endl;
}*/

  return 0;
}

#include <iostream>  
#include <vector>                                                                                   

using namespace std;

namespace rws2017
{

	class Player
	{
		public:

		Player(string argin_name, string argin_team_name="blue")
		{
			cout << "player name " << argin_name << endl;
			this->name = argin_name;
			this->team_name = argin_team_name;
			set_team_name(argin_team_name);
		}

		string name;
		
		//accessor SET
		void set_team_name(string argin_team_name)
		{
			if (argin_team_name == "red" || argin_team_name == "green" || argin_team_name == "blue")
				this ->team_name=argin_team_name;

			else
				cout<< "Error: incorrect team name " << endl;
		}
		
		//Overloaded accessor
		void set_team_name(void)
		{
			set_team_name("red"); // default value
		}
		
		//accessor SET
		string get_team_name(void)
		{
			return this->team_name;
		}
		
		
		private:
		string team_name;
	};
	
	
	class MyPlayer: public Player //inherits the class Player
	{
		public:
		
		MyPlayer(string argin_name, string argin_team_name): Player (argin_name, argin_team_name)
		{
			cout<< "Initialized MyPlayer"<<endl;
		}
		
		vector<string> teammates;
	};
}

int main()
{
	//because we used <using namespace std>  we can replace the other line
	//std::cout << "Hello world" << std::endl;
	cout << "Hello world" << endl;

   
    //Creating an instance of class Player
    rws2017::MyPlayer myplayer("brocha","red");

    //cout << "Created an instance of class player with public name " << player.name << endl;
    cout << "name = " << myplayer.name << endl;
    cout << "team name = " << myplayer.get_team_name()<<endl;
    
    myplayer.teammates.push_back("rodolfo");
    myplayer.teammates.push_back("arnaldo");
    
    
    
    // size_t = unsigned long int (sortchut)
    for(size_t i=0; i<myplayer.teammates.size(); ++i)
    {
		cout<<myplayer.teammates[i]<<endl;
	}
    
    return 1;
}



//std::vector <boost::shared_pts<Player>> teammates;


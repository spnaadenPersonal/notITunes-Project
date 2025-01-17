/*
Group II Electric Boogaloo
This Time it's Personal

NotItunes

NotItunes is a music management program. It was designed for those who prefer a more minimalist approach to their music management.

This program allows users to create individual NotItunes accounts where they can download and store their music for playback either now or later.
Users will download their music from the NotItunes Store into their personal library.
Users are able to further organize their personal library into playlists so they can curate a playlist for any occation and have the appropriate songs ready at their fingertips.

Software Written by:

Spencer Naaden, spnaaden@dmacc.edu
Charles Broderick, charwillbro@gmail.com
Chris Adams, adamscj1990@gmail.com
Benjamin Grieme, bgrieme@gmail.com

Commented Code spacing and labeling created by Spencer.

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
/*Includes*/
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
#include "pch.h"
#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
//#include <stdafx.h> //needs to be commented out for dev c++]
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <random>
#include <algorithm>
#include <conio.h>
#include <stdio.h>
#include "USER_H.h"
#include <sstream>
#include <Windows.h>

using namespace std;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

//function prototypes

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

int userLibrary();																				   //written by Chris, goes to user library and playlist functions
void logIn();                                                                                      //written by Charles, based on a logIn function created by Ben Grieme for anothe program
void logOut();		                                                                               //written by Charles
void displayLibrary(vector<Song> &vect);														   //written by Chris, gives user library and playlists a unified look
void shuffleLibrary(vector<Song> &vect);                                                           //written by Chris, shuffles contents of a user's playlist
inline bool usernameExists(const std::string& name) { ifstream f(name.c_str()); return f.good(); } //function written by PherricOxide at https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c (returns true if given file name already exists)
void musicPlayer(char*);                                                                           //function created by Charles and Ben based research with SDL
void accessMainLibrary(vector <Song> &, vector <Song> &);
void addSongs(vector<Song> &, vector <Song> &);
int getValidInt(int, int, string);
void mainLibrary(vector <Song> &allSongs);
void musicMenu(vector<Song> &vect);
void musicFinished();                                                                              //written by Charles 
vector <Song> readInSongs();                                                                       //written by Charles to read in all the songs available to program and to store the song title, artist name, album name and relative filepath for use throughout the program
void displayLogo();

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*Global Objects/Vectors*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

vector<Song> songInfo{};
User curUser;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*logOut Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void logOut() {//logout function saves all the user info for later use
	curUser.saveToFile();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*logIn Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void logIn() {
	User user;
	int choice;
	string userName;
	string tempUsername;
	string tempPassword;
	string tempfName;
	string templName;

	cout << "Welcome to NotItunes." << endl;
	cout << "Please enter your Username." << endl;
	cin >> userName;
	user.setUsername(userName);

	string fileName = user.getUsername() + ".txt";

	while (!usernameExists(fileName)) //while the username does not exist
	{
		cout << user.getUsername() << " was not found. Please select an option below: " << endl << "1. Try a different username." << endl << "2. Create a new account." << endl;
		choice = getValidInt(1, 2, "Invalid entry. Please enter a number associated with the choices listed above: ");
		if (choice == 2) //create new account
		{
			system("CLS");

			cout << "What is your first name?" << endl;
			cin >> tempfName;
			user.setFirstname(tempfName);

			cout << "What is your last name?" << endl;
			cin >> templName;
			user.setLastname(templName);

			cout << "What would you like your password to be?" << endl;
			cin >> tempPassword;
			user.setPassword(tempPassword);

			cout << "Username: " << user.getUsername() << endl;
			cout << "Password: " << user.getPassword() << endl;
			cout << "First Name: " << user.getFirstname() << endl;
			cout << "Last Name: " << user.getLastname() << endl;

			user.saveToFile();

			cout << "User Creation was successful! You will need to enter your new password to continue! " << endl << endl;

		}
		else if (choice == 1) {
			logIn();
			return;
		}
		else {
			logIn();
			return;
		}
	}

	curUser.loadFromFile(user.getUsername());

	do
	{
		cout << "Please enter the password for " << curUser.getUsername() << ". ";
		cout << "|| Enter 999 to return to the login screen." << endl;
		cin >> tempPassword;

		if (tempPassword == "999") {
			logIn();
			return;
		}

	} while (tempPassword.compare(curUser.getPassword()));


	if (tempPassword == curUser.getPassword()) //correct password given
	{
		system("CLS"); //Added by Spencer

		cout << "Welcome, " << curUser.getUsername() << ". Log in successful." << endl;

	}
	return;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*main Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

int main(int argc, char * argv[]) //Written by Charles
{

	bool topMenuOn = true;//enter the Main menu
	int choice;
	vector <Song> allSongs = readInSongs(); //reads file into local vector 

	logIn();// login a user

	while (topMenuOn != false) {

		cout << endl;
		cout << "-----------------------------------------------\n";
		cout << " Welcome To NotItunes " << curUser.getUsername() << "!" << endl;
		cout << " We keep your danciest music at your fingetrips!" << endl;
		cout << " (>'-')> <('-'<) (>'-')> <('-'<) (>'-')> <('-'<)" << endl;
		cout << "-----------------------------------------------\n";
		cout << "  1 - View Your Personal Library.\n";
		cout << "  2 - Visit the NotItunes Store.\n";
		cout << "  3 - Exit.\n";
		cout << "  Enter your choice and press return: \n";

		cin >> choice;

		switch (choice)
		{
		case 1: //call user library function
			system("CLS"); //Clear previous text

			userLibrary();

			break;

		case 2:	//call NotItunesStore or Main library function
			system("CLS"); //Clear previous text

			mainLibrary(allSongs);
			break;

		case 3: //exit program
			system("CLS"); //Clear previous text
			logOut();
			cout << "Exiting NotItunes! Keep the Party Going!";
			return 0;
			break;
		case 4: //exit program
			system("CLS"); //Clear previous text

			displayLogo();
			system("pause");

			break;
		default:
			cout << "You must select an option from above." << endl;

			break;
		}
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*musicFinished Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void musicFinished() { //Written by Charles
	cout << "Enter 2 to play the next song, or enter 4 to stop." << endl;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*musicMenu Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void musicMenu(vector<Song> &vect) //Written by Charles, This function passes a song or vector to the music player function and displays the song name, album name, and artist name.
{
	bool musicMenuOn = true;//enter the Main menu
	int choice;
	int i = 0;
	int vectorSize = vect.size();
	string filePath;
	int isPlaying = 1;
	string exitMessage = "";

	while (i < vectorSize) {//while loop will step through a vector of songs
		musicMenuOn = true;
		filePath = vect[i].pathname;
		char *cfilePath = &filePath[0u];

		musicPlayer(cfilePath);//automatically starts playing first song in the vector

		while (musicMenuOn != false) {

			Mix_HookMusicFinished(musicFinished);//listens for the end of a song and calls a void function that takes no parameters. is usless as a trigger to start next song. attempted changing the value of the menu condition variable to force the next song. also had it change the value of a member of a struct that was then the menu condition. to no avail
												 //now it just calls a function that prints a message telling the user to hit a next song or exit.
			cout << endl;
			cout << "----------------------------------------------------\n";
			cout << setw(15) << right << "  Now Playing:" << endl;
			cout << setw(15) << right << "  Album:" << " " << vect[i].album << endl;
			cout << setw(15) << right << "  Artist:" << vect[i].artist << endl;
			cout << setw(15) << right << "  Song:" << vect[i].song << endl;
			cout << "----------------------------------------------------\n";
			if (Mix_PausedMusic() < 1) { cout << "  1 - pause song.\n"; }
			else if (Mix_PausedMusic() > 0) { cout << "  1 - resume song.\n"; } //acts as a play/pause toggle
			if (i < vectorSize - 1) { cout << "  2 - next song.\n"; }   //checks to confirm there is indeed a next song in the vector to jump to
			if (i > 0) { cout << "  3 - previous song.\n"; }
			cout << "  4 - Stop.\n";

			cout << "  Enter your choice and press return: \n";
			cout << exitMessage << endl;

			cin >> choice;

			switch (choice)
			{
			case 1:
				cout << endl << endl; //For display purposes only
				if (Mix_PausedMusic() < 1) { Mix_PauseMusic(); }//acts as a play/pause toggle
				else if (Mix_PausedMusic() > 0) { Mix_ResumeMusic(); }
				exitMessage = "";

				break;
			case 2:
				cout << endl << endl; //For display purposes only
				if (i < vectorSize - 1) {  //checks to confirm there is indeed a next song in the vector to jump to
					musicMenuOn = false;
					Mix_HaltMusic();
					exitMessage = "";
				}
				else {
					exitMessage = "This Is The Final Song In The Playlist.";
				}

				break;
			case 3:
				cout << endl << endl; //For display purposes only

				if (i > 0) { //checks to see if the index is larger than zero to see if it there is even a previous song to go back to
					Mix_HaltMusic();
					i = i - 2;
					musicMenuOn = false;
					exitMessage = "";
				}
				else {
					exitMessage = "There are no previous songs. You are at the beginning of the playlist!";
				}

				break;
			case 4:
				cout << endl << endl; //For display purposes only
				Mix_HaltMusic();
				i = 100;//some large number to fail the while loop that moves th the next track

				exitMessage = "You have stopped playback";
				musicMenuOn = false;
				break;
			default:
				cout << endl << endl; //For display purposes only

				break;
			}
		}
		i++;
	}
	cout << "You have reached the end of your playlist!" << endl;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*musicPlayer Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void musicPlayer(char *pathName)
{
	SDL_Init(SDL_INIT_AUDIO);
	//SDL_Window *window = SDL_CreateWindow("not iTunes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 100, 100, SDL_WINDOW_OPENGL);

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0)
		cout << "Error: " << Mix_GetError() << endl;

	Mix_Music *bgm = Mix_LoadMUS(pathName); //need to have double backslash to work
	Mix_PlayMusic(bgm, 1);

	std::cin.get();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*userLibrary Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

int userLibrary()
{
	// Function to load user song library
	vector<Song> userLibrary = curUser.getUserLibrary();

	// Get user name
	string userName = curUser.getFirstname();

	cout << "Welcome " << userName << "! Please select an option below" << endl << endl;
	int initialUserEntrySelect;
	bool initialUserEntry = true;
	while (initialUserEntry) {
		cout << "1. Display Your Library" << endl;
		cout << "2. Display Your Playlists" << endl;
		cout << "3. Exit to Main Menu" << endl;
		cin >> initialUserEntrySelect;

		while (cin.fail() || initialUserEntrySelect < 1 || initialUserEntrySelect > 3) {
			cin.clear();
			cin.ignore();
			cout << "Invalid Entry. Please try again." << endl;
			cin >> initialUserEntrySelect;
		}
		switch (initialUserEntrySelect) {
		case(1): {
			bool continueEntry = true;
			if (userLibrary.size() == 0) {
				system("CLS"); //Added by Spencer
				cout << "No songs in user library. Download songs from notItunes library to continue." << endl;
				continueEntry = false;
			}

			while (continueEntry) {
				// Display current song library
				displayLibrary(userLibrary);
				int initialMenuEntry;
				cout << "1. Play Song" << endl;
				cout << "2. Shuffle Library" << endl;
				cout << "3. Return to Main Menu" << endl;
				std::cin >> initialMenuEntry;

				while (std::cin.fail() || (initialMenuEntry < 1 || initialMenuEntry > 3)) {
					std::cin.clear();
					std::cin.ignore();
					cout << "Invalid Entry. Please try again." << endl;
					std::cin >> initialMenuEntry;
				}

				switch (initialMenuEntry) {
				case (1): { // Play song case
					int songChoice;
					cout << "Enter the number of the song you would like to play or type in 999 to play all songs " << endl;
					std::cin >> songChoice;
					while (std::cin.fail() || songChoice != 999 && (songChoice < 1 || songChoice > userLibrary.size())) {
						std::cin.clear();
						std::cin.ignore();
						cout << "Invalid Entry. Please try again." << endl;
						std::cin >> songChoice;
					}
					if (songChoice == 999) {
						musicMenu(userLibrary);
					}
					else {
						bool musicMenuOn = true;
						int choice;
						string filePath = userLibrary[songChoice - 1].pathname;
						char *cfilePath = &filePath[0u];
						musicPlayer(cfilePath);//automatically starts playing first song in the vector
						while (musicMenuOn != false) {
							cout << endl;
							cout << "----------------------------------------------------\n";
							cout << "  Now Playing: " << endl;
							cout << "  Album: " << userLibrary[songChoice - 1].album << endl;
							cout << "  Artist: " << userLibrary[songChoice - 1].artist << endl;
							cout << "  Song: " << userLibrary[songChoice - 1].song << endl;
							cout << "----------------------------------------------------\n";
							if (Mix_PausedMusic() < 1) { cout << "  1 - pause song.\n"; }
							else if (Mix_PausedMusic() > 0) { cout << "  1 - resume song.\n"; } //acts as a play/pause toggle
							cout << "  4 - Stop.\n";
							cout << "  Enter your choice and press return: \n";
							cin >> choice;

							switch (choice)
							{
							case 1:
								cout << endl << endl; //For display purposes only
								if (Mix_PausedMusic() < 1) { Mix_PauseMusic(); }//acts as a play/pause toggle
								else if (Mix_PausedMusic() > 0) { Mix_ResumeMusic(); }
								break;
							case 4:
								cout << endl << endl;//For display purposes only
								musicMenuOn = false;
								Mix_HaltMusic();
								cout << "Exiting NotItunes! Keep the Party Going!";
								break;
							default:
								cout << endl << endl;//For display purposes only
								cout << "You must select an option from above." << endl;
								break;
							}
						}
						break;
					}
				}
				case (2): { // Shuffle library selected, go to shuffleLibrary() function
					system("CLS"); //Added by Spencer
					shuffleLibrary(userLibrary);
					break;
				}
				case (3): { // Return to menu case
					continueEntry = false;
					break;
				}
				}
			}
			break;
		}
		case(2): { // Display User Playlists
			bool playlistMenuEntry = true;
			while (playlistMenuEntry) {
				int userPlaylistMenuEntry;
				system("CLS"); //Added by Spencer
				cout << "Please select an option below." << endl;
				cout << "1. View Playlists" << endl;
				cout << "2. Add Playlist" << endl;
				cout << "3. Edit Playlist" << endl;
				cout << "4. Delete Playlist" << endl;
				cout << "5. Exit to User Menu" << endl;
				cin >> userPlaylistMenuEntry;

				while (cin.fail() || userPlaylistMenuEntry < 1 || userPlaylistMenuEntry > 5) {
					cin.clear();
					cin.ignore();
					cout << "Invalid Entry. Please try again." << endl;
					cin >> userPlaylistMenuEntry;
				}
				switch (userPlaylistMenuEntry) {
				case(1): // View Playlists
				{
					vector<vector<Song>> userPlaylists = curUser.getUserPlaylists(); // Local version of class object
					if (userPlaylists.size() == 0) { // No playlists
						cout << "" << endl;
						cout << "No user playlists to display." << endl;
						cout << "" << endl;
						break;
					}
					vector<string> playlistNames = curUser.getUserPlaylistNames(); // Local version of class object
					for (int i = 0; i < playlistNames.size(); i++) { // Display all playlists
						cout << i + 1 << " " << playlistNames[i] << endl;
					}
					int userPlaylistEntry;
					cout << "Enter a playlist number to play music from or enter 999 to exit" << endl;
					cin >> userPlaylistEntry; // Get playlist number to view
					while (cin.fail() || userPlaylistEntry != 999 && (userPlaylistEntry < 1 || userPlaylistEntry > playlistNames.size())) {
						cin.clear();
						cin.ignore();
						cout << "Invalid Entry. Please try again." << endl;
						cin >> userPlaylistEntry;

					}
					if (userPlaylistEntry == 999) { // Exit playlist menu
						break;
					}
					vector<Song> thisPlaylist = curUser.getPlaylist(userPlaylistEntry - 1); // Local version of this playlist
					cout << "Contents of " << playlistNames[userPlaylistEntry - 1] << ": " << endl;
					bool playPlaylist = true;
					int playlistEntry;
					while (playPlaylist) {
						displayLibrary(thisPlaylist);
						cout << "1. Play Playlist" << endl;
						cout << "2. Shuffle Playlist" << endl;
						cout << "3. Exit to Main Menu" << endl;
						cin >> playlistEntry; // Playlist menu options
						while (cin.fail() || playlistEntry < 1 || playlistEntry > 3) {
							cin.clear();
							cin.ignore();
							cout << "Invalid entry. Please try again" << endl;
							cin >> playlistEntry;
						}
						switch (playlistEntry) {
						case(1): // Play current playlist
						{
							musicMenu(thisPlaylist);
							break;
						}
						case (2): { // Shuffle Playlist
							shuffleLibrary(thisPlaylist);
							break;
						}
						case(3): {
							playPlaylist = false; // Exit to playlist menu
							break;
						}
						}
					}
				}
				break;
				case(2): // Add Playlist
				{
					string playlistName;
					cout << "What would you like to name this playlist?" << endl;
					cin >> playlistName; // Add new playlist
					if (!curUser.addPlaylist(playlistName)) {
						cout << "Playlist entry error" << endl;
						return -1;
					}
					vector<Song> newPlaylist; // Local version of new playlist
					vector<Song> userLibrary = curUser.getUserLibrary(); // Local version of user library
					const int displaySongsSize = 1000;
					bool completePlaylistEntry = false;
					while (!completePlaylistEntry) {
						bool displaySongs[displaySongsSize] = { false };
						bool notDisplayed[displaySongsSize] = { false };
						if (newPlaylist.size() == 0) { // Playlist is empty, can add all songs
							for (int i = 0; i < userLibrary.size(); i++) {
								displaySongs[i] = true;
								cout << "Enter " << i + 1 << " to add " << userLibrary[i].song << " to your new playlist" << endl;
							}
						}
						else {
							for (int k = 0; k < newPlaylist.size(); k++) { // Playlist not empty, compare playlist to user library to see what's not in playlist
								for (int i = 0; i < userLibrary.size(); i++) {
									if (newPlaylist[k].pathname.compare(userLibrary[i].pathname) == 0) {
										displaySongs[i] = false;
										notDisplayed[i] = true;
										continue;
									}
									else if (newPlaylist[k].pathname.compare(userLibrary[i].pathname) != 0) {
										if (notDisplayed[i] != true) {
											displaySongs[i] = true;
										}
									}
								}
							}
							for (int i = 0; i < userLibrary.size(); i++) {
								if (displaySongs[i] == true) { // Which songs can be added
									cout << "Enter " << i + 1 << " to add " << userLibrary[i].song << " to your new playlist" << endl;
								}
							}
						}
						int addPlaylistEntry;
						cout << "Please enter a valid song to add to your playlist or enter 999 to complete playlist" << endl;
						cin >> addPlaylistEntry; // Enter a song to add to playlist
						while (cin.fail() || addPlaylistEntry != 999 && (addPlaylistEntry < 1 || addPlaylistEntry > userLibrary.size())) {
							cin.clear();
							cin.ignore();
							cout << "Invalid Entry. Please Try Again." << endl;
							cin >> addPlaylistEntry;
						}
						if (addPlaylistEntry == 999) { // Exit to mneu
							int playlistIndex = curUser.getPlaylistIndex(playlistName);
							for (unsigned int i = 0; i < newPlaylist.size(); i++) {
								curUser.addSongToPlaylist(playlistIndex, newPlaylist[i]); // Add songs to class playlist
							}
							completePlaylistEntry = true;
							continue;
						}
						while (displaySongs[addPlaylistEntry - 1] == false) { // Song already in playlist
							cin.clear();
							cin.ignore();
							cout << "Song already in playlist. Please try again." << endl;
							cin >> addPlaylistEntry;
						}
						newPlaylist.push_back(userLibrary[addPlaylistEntry - 1]); // Add song to local playlist
						cout << userLibrary[addPlaylistEntry - 1].song << " added to your playlist!" << endl;
					}

				}
				break;
				case(3): // Edit Playlist
				{
					vector<vector<Song>> userPlaylists = curUser.getUserPlaylists(); // Local version of class object
					if (userPlaylists.size() == 0) {
						cout << "" << endl;
						cout << "No user playlists to display." << endl;
						cout << "" << endl;
						break;
					}
					vector<string> playlistNames = curUser.getUserPlaylistNames(); // Local version of class object
					for (int i = 0; i < playlistNames.size(); i++) {
						cout << i + 1 << ". " << playlistNames[i] << endl;
					}
					int userPlaylistEntry;
					cout << "Enter a playlist to edit or enter 999 to exit to playlist menu." << endl;
					cin >> userPlaylistEntry; // Which playlist to edit
					while (cin.fail() || userPlaylistEntry != 999 && (userPlaylistEntry < 1 || userPlaylistEntry > playlistNames.size())) {
						cin.clear();
						cin.ignore();
						cout << "Invalid Entry. Please try again." << endl;
						cin >> userPlaylistEntry;
					}
					if (userPlaylistEntry == 999) {
						break;
					}
					vector<Song> thisPlaylist = curUser.getPlaylist(userPlaylistEntry - 1); // Local version of entered playlist
					cout << "Current contents of " << playlistNames[userPlaylistEntry - 1] << ":" << endl;
					cout << " " << endl; // Contents of this playlist
					for (int i = 0; i < thisPlaylist.size(); i++) {
						cout << "Artist : " << thisPlaylist[i].artist << endl;
						cout << "Song : " << thisPlaylist[i].song << endl;
						cout << "Album : " << thisPlaylist[i].album << endl;
						cout << " " << endl;
					}
					cout << " " << endl;
					int userEditEntry;
					cout << "1. Add song to current playlist" << endl;
					cout << "2. Delete song from current playlist" << endl;
					cout << "3. Exit to playlist menu" << endl;
					cout << "Please select an option listed above" << endl;
					cin >> userEditEntry; // Add or delete to/from playlist
					while (cin.fail() || userEditEntry < 1 || userEditEntry > 3) {
						cin.clear();
						cin.ignore();
						cout << "Invalid Entry. Please try again" << endl;
						cin >> userEditEntry;
					}
					switch (userEditEntry) {
					case(1): // Add song to current playlist
					{
						const int displaySongsSize = 1000;
						bool displaySongs[displaySongsSize] = { false };
						bool notDisplayed[displaySongsSize] = { false };
						bool continueAdd = true;
						int playlistIndex = curUser.getPlaylistIndex(playlistNames[userPlaylistEntry - 1]);
						while (continueAdd) {
							if (thisPlaylist.size() == userLibrary.size()) { // All songs from library in playlist
								cout << "Playlist contains all song in user library" << endl;
								continueAdd = false;
								break;
							}
							cout << " " << endl;
							cout << "Songs in user library that are not in current playlist: " << endl; // Songs not in playlist, but in user library
							for (int k = 0; k < thisPlaylist.size(); k++) {
								for (int i = 0; i < userLibrary.size(); i++) {
									if (thisPlaylist[k].pathname.compare(userLibrary[i].pathname) == 0) {
										displaySongs[i] = false;
										notDisplayed[i] = true;
										continue;
									}
									else if (thisPlaylist[k].pathname.compare(userLibrary[i].pathname) != 0) {
										if (notDisplayed[i] != true) {
											displaySongs[i] = true;
										}
									}
								}
							}
							for (int i = 0; i < userLibrary.size(); i++) {
								if (displaySongs[i] == true) { // Which songs can be added
									cout << "Enter " << i + 1 << " to add " << userLibrary[i].song << " to your new playlist" << endl;
								}
							}
							int addSongEntry;
							cout << " " << endl;
							cout << "Enter a song to add to your playlist or enter 999 to exit to playlist menu" << endl;
							cin >> addSongEntry; // Requested song from user
							while (cin.fail() || addSongEntry != 999 && (addSongEntry < 1 || addSongEntry > userLibrary.size())) {
								cin.clear();
								cin.ignore();
								cout << "Invalid entry. Please try again" << endl;
								cin >> addSongEntry;
							}
							if (addSongEntry == 999) { // Exit to playlist menu
								continueAdd = false;
								break;
							}
							Song addedSong = userLibrary[addSongEntry - 1]; // Create Song object
							curUser.addSongToPlaylist(playlistIndex, addedSong); // Add song to class object
							thisPlaylist.push_back(addedSong); // Add song to local object
						}

					}
					break;
					case(2): // Delete song from current playlist
					{
						bool continueDelete = true;
						int userDeleteEntry;
						int playlistIndex = curUser.getPlaylistIndex(playlistNames[userPlaylistEntry - 1]); // Index of this playlist
						while (continueDelete) {
							for (int i = 0; i < thisPlaylist.size(); i++) { // Songs in this playlist
								cout << i + 1 << ": " << endl;
								cout << "Artist : " << thisPlaylist[i].artist << endl;
								cout << "Song : " << thisPlaylist[i].song << endl;
								cout << "Album : " << thisPlaylist[i].album << endl;
								cout << " " << endl;
							}
							cout << "Enter the number of a song you would like to remove from this playlist or enter 999 to exit" << endl;
							cin >> userDeleteEntry;  // Get user request for which song to delete
							while (cin.fail() || userDeleteEntry != 999 && (userDeleteEntry < 1 || userDeleteEntry > thisPlaylist.size())) {
								cin.clear();
								cin.ignore();
								cout << "Invalid entry. Please try again." << endl;
								cin >> userDeleteEntry;
							}
							if (userDeleteEntry == 999) { // Exit to menu
								curUser.setUserPlaylist(thisPlaylist, playlistIndex); // Overwrite class object with local object
								continueDelete = false;
								break;
							}
							thisPlaylist.erase(thisPlaylist.begin() + (userDeleteEntry - 1)); // Erase Song objects from local playlist
						}
					}
					break;
					case(3): // Exit to playlist menu
						break;
					}
				}
				break;
				case(4): // Delete Playlist
				{
					vector<vector<Song>> userPlaylists = curUser.getUserPlaylists(); // Get all user playlists
					if (userPlaylists.size() == 0) {
						cout << "" << endl;
						cout << "No user playlists to display." << endl;
						cout << "" << endl;
						break;
					}
					vector<string> playlistNames = curUser.getUserPlaylistNames(); // Get all user playlist names
					for (int i = 0; i < playlistNames.size(); i++) {
						cout << i + 1 << ". " << playlistNames[i] << endl; // Display playlists
					}
					int userPlaylistEntry;
					bool continueDelete = true;
					while (continueDelete) {
						cout << "Enter a playlist to delete or enter 999 to exit to playlist menu" << endl;
						cin >> userPlaylistEntry; // Which playlist to delete
						while (cin.fail() || userPlaylistEntry != 999 && (userPlaylistEntry < 1 || userPlaylistEntry > userPlaylists.size())) {
							cin.clear();
							cin.ignore();
							cout << "Invalid entry. Please try again." << endl;
							cin >> userPlaylistEntry;
						}
						if (userPlaylistEntry == 999) { // Exit to main menu
							continueDelete = false;
							break;
						}
						int playlistIndex = curUser.getPlaylistIndex(playlistNames[userPlaylistEntry - 1]); // Playlist index of requested playlist to delete
						userPlaylists.erase(userPlaylists.begin() + userPlaylistEntry - 1);
						playlistNames.erase(playlistNames.begin() + userPlaylistEntry - 1); // Delete playlist and playlist name from local object
						curUser.setPlaylistEmpty(userPlaylistEntry);
						curUser.setPlaylistNamesEmpty(userPlaylistEntry); // Delete playlist and playlist name from class object
					}
				}
				break;
				case(5): // Exit to user menu
					playlistMenuEntry = false;
					system("CLS"); //Added by Spencer
					break;
				}
			}
			break;
		}
		case(3): {
			initialUserEntry = false;
			system("CLS"); //Added by Spencer
			break;
		}
		}
	}
}//end of userLibrary function

 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

 /*displayLibrary Function*/

 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void displayLibrary(vector<Song> &vect) { //charlie edited the display to make the spacing more uniform. feel free to tell him to go blow a balloon. he didnt ask permission before altering your code. 
	int titleColWidth = 0;
	int artistColWidth = 0;
	int albumColWidth = 0;
	int totalLength = 0;

	vector <string> duplicateSongPrevention;
	duplicateSongPrevention.push_back("meow");
	string spacer = " ||---|--";// The output was formatted by Charles to display the songs in an easy to read manner that shows all held information about the songs

	for (unsigned int i = 0; i < vect.size(); i++) {
		if (vect[i].song.length() > titleColWidth) { //set the width of the artist column
			titleColWidth = vect[i].song.length();

		}
		if (vect[i].artist.length() > artistColWidth) { //set the width of the artist column
			artistColWidth = vect[i].artist.length();

		}
		if (vect[i].album.length() > albumColWidth) { //set the width of the artist column
			albumColWidth = vect[i].album.length();

		}

	}
	cout << " Current Song Library: " << endl;

	totalLength = titleColWidth + artistColWidth + albumColWidth; //dynamically generate a spacing line
	for (int i = 0; i < totalLength; i++) {
		if (i == titleColWidth - 2) {
			spacer += "|";
		}
		else if (i == artistColWidth + titleColWidth - 1) {
			spacer += "|";
		}
		else if (i == albumColWidth + artistColWidth + titleColWidth) {
			spacer += "|";
		}
		else {
			spacer += "-";
		}
	}
	spacer += "||";//dynamically generate a spacing line end

				   //displays the songs in a semi GUI display
	cout << spacer << endl;
	cout << " || " << setw(3) << right << "|" << setw(titleColWidth) << left << "Title" << "|" << setw(artistColWidth) << "Artist" << "|" << setw(albumColWidth) << "Album" << "||" << endl;
	cout << spacer << endl;

	for (unsigned int i = 0; i < vect.size(); i++) { //pulls songs from the playlists to populate the library

		cout << " || " << setw(2) << right << i + 1 << "|" << setw(titleColWidth) << left << vect[i].song << "|" << setw(artistColWidth) << vect[i].artist << "|" << setw(albumColWidth) << vect[i].album << "||" << endl;

	}

	cout << spacer << endl;
	cout << " || " << setw(3) << right << "|" << setw(titleColWidth) << left << "Title" << "|" << setw(artistColWidth) << "Artist" << "|" << setw(albumColWidth) << "Album" << "||" << endl;
	cout << spacer << endl;
	cout << " " << endl;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*shuffleLibrary Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void shuffleLibrary(vector<Song> &vect) {
	random_shuffle(vect.begin(), vect.end());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*mainLibrary Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void mainLibrary(vector <Song> &allSongs) { //Created by Spencer Naaden: The mainLibrary function used to be the main menu of Spencer's section of the program, but once the each group members' code was woven together, 
	//mainLibrary just became a function that calls the accessMainLibrary function.

	vector <Song> userLibrary = curUser.getUserLibrary();

	accessMainLibrary(userLibrary, allSongs);
	curUser.setUserLibrary(userLibrary); // Update class 

	system("CLS");

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*accessMainLibrary Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void accessMainLibrary(vector <Song> &userLibrary, vector <Song> &allSongs) { //Created by Spencer Naaden: The accessMainLibrary function asks the user whether they want to add songs to their personal library or not.
	//If the answer is yes, then the function calls the add songs function. If not, then the the user will return to the mainLibrary function.

	int userChoice;

	bool continueOperations = true, allSongsDownloaded = false;

	do {

		if (userLibrary.size() == allSongs.size()) {

			allSongsDownloaded = true;
		}

		system("CLS");

		if (allSongsDownloaded == false) {

			cout << "Welcome to the Main Song Library Menu:" << endl;

			cout << "-----------------------------------------------" << endl;

			cout << "Enter 1 to Download New Songs:" << endl;

			cout << "Enter 2 to Quit to the Main Menu:" << endl;

			userChoice = getValidInt(1, 2, "Error, please enter a valid listed number to make a command selection.");

			switch (userChoice) {

			case 1:
				addSongs(userLibrary, allSongs);
				break;
			case 2:
				continueOperations = false;
				break;
			}

		}
		else {

			cout << "You have already downloaded every possible song to your account!" << endl;

			cout << "Press any key to return to main menu" << endl;

			system("pause");

			continueOperations = false;
		}

	} while (continueOperations == true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*addSongs Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void addSongs(vector <Song> &userLibrary, vector <Song> &allSongs) { //Created by Spencer Naaden, Chris Adams, and Charles Broderick: The addSongs function displays all the possible songs that a user can download from
	//the notiTunes library. The user can then enter a number to download a specific song, or exit to the accessMainLibrary function. Spencer worked on reading in the songs that a user can download from a file, 
	//Chris worked on class operations and other needed operations for 'downloading' songs to a user's personal library, and Charles worked on how songs display to the user.

	int songChoice = 0;
	int titleColWidth = 0;
	int artistColWidth = 0;
	int albumColWidth = 0;
	int totalLength = 0;
	string spacer = " ||---|--"; // The output was formatted by Charles to display the songs in an easy to read manner that shows all held information about the songs

	for (unsigned int i = 0; i < allSongs.size(); i++) {
		if (allSongs[i].song.length() > titleColWidth) { //set the width of the artist column
			titleColWidth = allSongs[i].song.length();
		}
		if (allSongs[i].artist.length() > artistColWidth) { //set the width of the artist column
			artistColWidth = allSongs[i].artist.length();
		}
		if (allSongs[i].album.length() > albumColWidth) { //set the width of the artist column
			albumColWidth = allSongs[i].album.length();
		}
	}

	totalLength = titleColWidth + artistColWidth + albumColWidth; //dynamically generate a spacing line
	for (int i = 0; i < totalLength; i++) {
		if (i == titleColWidth - 2) {
			spacer += "|";
		}
		else if (i == artistColWidth + titleColWidth - 1) {
			spacer += "|";
		}
		else if (i == albumColWidth + artistColWidth + titleColWidth) {
			spacer += "|";
		}
		else {
			spacer += "-";
		}
	}
	spacer += "||";//dynamically generate a spacing line end

	const int displaySongsSize = 1000;

	bool displaySongs[displaySongsSize] = { false };
	bool notDisplayed[displaySongsSize] = { false };

	cout << spacer << endl;
	cout << " || " << setw(3) << right << "|" << setw(titleColWidth) << left << "Title" << "|" << setw(artistColWidth) << "Artist" << "|" << setw(albumColWidth) << "Album" << "||" << endl;
	cout << spacer << endl;

	while (songChoice != 999) { //Charlie added the loop to make downloading songs easier

		system("CLS");

		cout << "    Please choose a song below that you would like to add to your personal library:" << endl;
		cout << spacer << endl;
		cout << " || " << setw(3) << right << "|" << setw(titleColWidth) << left << "Title" << "|" << setw(artistColWidth) << "Artist" << "|" << setw(albumColWidth) << "Album" << "||" << endl;
		cout << spacer << endl;// The output was formatted by Charles to display the songs in an easy to read manner that shows all held information about the songs

		if (userLibrary.size() == 0) { // User library is empty, can download any song

			for (int i = 0; i < allSongs.size(); i++) {

				cout << " || " << setw(2) << right << i + 1 << "|" << setw(titleColWidth) << left << allSongs[i].song << "|" << setw(artistColWidth) << allSongs[i].artist << "|" << setw(albumColWidth) << allSongs[i].album << "||" << endl;

				displaySongs[i] = true;

			}
		}
		else {
			for (int k = 0; k < userLibrary.size(); k++) { // Compare user library to notItunes library, mark songs not in library available for download
				for (int i = 0; i < allSongs.size(); i++) {
					if (userLibrary[k].pathname.compare(allSongs[i].pathname) == 0) {
						displaySongs[i] = false;
						notDisplayed[i] = true;
						continue;
					}
					else if (userLibrary[k].pathname.compare(allSongs[i].pathname) != 0) {
						if (notDisplayed[i] != true) {
							displaySongs[i] = true;
						}
					}
				}
			}

			for (int i = 0; i < allSongs.size(); i++) {
				if (displaySongs[i] == true) { // Display songs available for download
					cout << " || " << setw(2) << right << i + 1 << "|" << setw(titleColWidth) << left << allSongs[i].song << "|" << setw(artistColWidth) << allSongs[i].artist << "|" << setw(albumColWidth) << allSongs[i].album << "||" << endl;
				}
			}
		}

		cout << spacer << endl;
		cout << " || " << setw(3) << right << "|" << setw(titleColWidth) << left << "Title" << "|" << setw(artistColWidth) << "Artist" << "|" << setw(albumColWidth) << "Album" << "||" << endl;
		cout << spacer << endl;
		cout << " " << endl;

		cout << "     Enter 999 to exit to the Main Song Library Menu:" << endl;

		std::cin >> songChoice;

		if (songChoice == 999) {

			cout << "Quitting to Main Song Library Menu..." << endl;

			return;
		}

		while (std::cin.fail() || songChoice < 1 || songChoice > allSongs.size() || displaySongs[songChoice - 1] == false) {

			std::cin.clear(); //clear input buffer
			std::cin.ignore(INT_MAX, '\n'); //move stream to the end
			cout << "Error, please enter a valid number to choose a song to download:" << endl; //print the error message
			std::cin >> songChoice; //try again

			if (songChoice == 999) {

				cout << "Quitting to Main Song Library Menu..." << endl;

				return;
			}
		}
		userLibrary.push_back(allSongs[songChoice - 1]);

		cout << "The song named " << allSongs[songChoice - 1].song << " has been downloaded to your personal library!" << endl;

		string playlistName = "AllSongs";

		if (!curUser.addPlaylist(playlistName)) { //if default playlist already exists, remove it and create a new one

			int playlistIndex = curUser.getPlaylistIndex(playlistName);

			curUser.setPlaylistEmptyDefault(playlistIndex); //remove existing default allSongs playlist
			curUser.setPlaylistNamesEmptyDefault(playlistIndex); //remove existing default allSongs playlist

			curUser.addPlaylist(playlistName); //create default allSongs playlist

			playlistIndex = curUser.getPlaylistIndex(playlistName);

			for (unsigned int i = 0; i < userLibrary.size(); i++) {
				curUser.addSongToPlaylist(playlistIndex, userLibrary[i]); // Add songs to class playlist
			}

		}
		else { // if theres no existing default, one was created and is ready to be filled

			int playlistIndex = curUser.getPlaylistIndex(playlistName);
			for (unsigned int i = 0; i < userLibrary.size(); i++) {
				curUser.addSongToPlaylist(playlistIndex, userLibrary[i]); // Add songs to class playlist
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*getValidInt Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

int getValidInt(int beginRange, int endRange, string message) {
	//Benjamin Grieme, bgrieme@gmail.com
	int validInt;
	std::cin >> validInt;
	while (std::cin.fail() || !(validInt >= beginRange && validInt <= endRange)) //while the input failed OR validInt doesn't meet the range criteria
	{
		std::cin.clear(); //clear input buffer
		std::cin.ignore(INT_MAX, '\n'); //move stream to the end
		cout << message << endl; //print the error message
		std::cin >> validInt; //try again
	}
	return validInt;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*readInSongs Function*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

vector <Song> readInSongs() { // Written by Charles to read in all the songs available to program and to store the song title, artist name, album name and relative filepath for use throughout the program

	Song intoStruct;
	vector<Song> songInfo;
	vector<string> songNames;
	vector<string> filepath;
	string addFilePath = "";
	string rawSongInfo = "";
	string album = "";
	string artist = "";
	string songName = "";
	string folderName = "notItunesWavs\\";

	string fileName = "notItunesWavs\\songFilePath.txt";
	fstream songs(fileName);

	if (!songs) {
		cout << "Error opening songFilePath input file " << endl;

	}
	int i = 0;
	while (getline(songs, rawSongInfo)) { //READING FILE LINES INTO VECTOR
		if (rawSongInfo.size() > 0) {

			songNames.push_back(rawSongInfo);
			//cout << rawSongInfo << endl << endl;  //uncomment to verify data processing

			i++;
		}
	}

	i = 0; //reset counter
	while (i < songNames.size()) { //READING FILE LINES INTO VECTOR

		addFilePath = folderName + songNames[i];

		filepath.push_back(addFilePath);
		//cout << addFilePath << endl << endl; //uncomment to verify data processing
		i++;
	}

	i = 0; //reset counter
	while (i < songNames.size()) { //cut up the long filenames read in from the textfile
		stringstream ss(songNames[i]);
		getline(ss, album, '@');
		getline(ss, artist, '@');
		getline(ss, songName, '@');
		songName.pop_back();// 4 popbacks to remove . w a v 
		songName.pop_back();
		songName.pop_back();
		songName.pop_back();

		intoStruct.album = album;
		intoStruct.artist = artist;
		intoStruct.song = songName;
		intoStruct.pathname = filepath[i];

		songInfo.push_back(intoStruct);
		//cout << "Album: " << album << endl; //uncomment to verify data processing
		//cout << "Artist: " << artist << endl;
		//cout << "Song: " << songName << endl;
		i++;
	}
	return songInfo;
	//i = 0; //reset counter
	//while (i < songInfo.size()) { //print out vector of song structs //uncomment to verify data processing

	//	cout << "Album: " << songInfo[i].album << endl;
	//	cout << "Artist: " << songInfo[i].artist << endl;
	//	cout << "Song: " << songInfo[i].title << endl;
	//	cout << "FilePath: " << songInfo[i].filepath << endl << endl;
	//	i++;
	//}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

/*displayLogo Functions*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

void displayLogo() {// an easter egg written by charles
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left, r.top, 1100, 600, TRUE); // 800 width, 100 height


	//had to create literal character strings because '\' is an escape character an this was less work than escaping each '\' with its own escape character
	string str1 = R"(  ____                                     ______  ______                                                                     )";
	string str2 = R"( /\  _ `\                                 /\__  _\/\__  _\                                                                    )";
	string str3 = R"( \ \ \ \_\  _ __   ___   __  __  _____    \/_/\ \/\/_/\ \/                                                                    )";
	string str4 = R"(  \ \ \ __ /\`'__\/ __`\/\ \/\ \/\ '__`\     \ \ \   \ \ \                                                                    )";
	string str5 = R"(   \ \ \/, \ \ \//\ \L\ \ \ \_\ \ \ \L\ \     \_\ \__ \_\ \__                                                                 )";
	string str6 = R"(    \ \____/\ \_\\ \____/\ \____/\ \ ,__/     /\_____\/\_____\                                                                )";
	string str7 = R"(  ___\/___/__\/_/ \/___/  \/_____/\ \ \/      \/_____/\/___ _/__                                      ___                     )";
	string str8 = R"( /\  __\ /\_ \                /\ \_\ \_\     __            /\  _`\                                   /\_ \                    )";
	string str9 = R"( \ \ \___\//\ \      __    ___\ \ ,_\/_/ __ /\_\    ___    \ \ \L\ \    ___     ___      __      __  \//\ \     ___     ___   )";
	string str10 = R"(  \ \  __\ \ \ \   /'__`\ /'___\ \ \/ /\`'__\/\ \  /'___\   \ \  _ <'  / __`\  / __`\  /'_ `\  /'__`\  \ \ \   / __`\  / __`\ )";
	string str11 = R"(   \ \ \__  \_\ \_/\  __//\ \__/\ \ \_\ \ \/ \ \ \/\ \__/    \ \ \L\ \/\ \L\ \/\ \L\ \/\ \L\ \/\ \L\.\_ \_\ \_/\ \L\ \/\ \L\ \)";
	string str12 = R"(    \ \____\/\____\ \____\ \____\\ \__\\ \_\  \ \_\ \____\    \ \____/\ \____/\ \____/\ \____ \ \__/.\_\/\____\ \____/\ \____/)";
	string str13 = R"(     \/____/\/____/\/____/\/____/ \/__/ \/_/   \/_/\/____/     \/___/  \/___/  \/___/  \/___L\ \/__/\/_/\/____/\/___/  \/___/ )";
	string str14 = R"(                                                                                         /\____/                              )";
	string str15 = R"(                                                                                         \_/__/                               )";

	cout << str1 << endl;
	cout << str2 << endl;
	cout << str3 << endl;
	cout << str4 << endl;
	cout << str5 << endl;
	cout << str6 << endl;
	cout << str7 << endl;
	cout << str8 << endl;
	cout << str9 << endl;
	cout << str10 << endl;
	cout << str11 << endl;
	cout << str12 << endl;
	cout << str13 << endl;
	cout << str14 << endl;
	cout << str15 << endl;
}

// End Of Program
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

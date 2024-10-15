/* Author: Stevie Cervantes
 * Created: July 25, 2024
 * File Name: Map.hpp
 * Description: Contains the code to create the map as well as
 * 				the different functions to for printing maps.
*/

#include <cmath>
#include <iomanip>
#include <string>
#include "EasyBMP.h"
#include "termcolor.hpp"

using namespace std;

// Map Class Definition
class Map{
	int** raw_map;
	char** polished_map;
	int width;
	int height;
	int waterLine;
	int radius;
	int powerRating;
	int numDirtballs;
	int testSeed;
	string fileName;
	
	public:
		Map(string fileName);
		~Map();
		int GetNumDirtballs();
		void DropDirtball();
		void NormalizeIsland();
		void PolishedIsland();
		void PrintRawMap();
		void PrintPolishedMap();		
		void PrintPolishedBitMap();	
	private:
		void SetTermColor(char currChar);
		
};

// Map Constructor
Map::Map(string fileName){
	this->fileName = fileName;
	
	// Console print out solors
	cout << termcolor::bold << termcolor::on_green << termcolor::white;
	
	// Welcome Message
	cout << "Welcome to Stevie's for fun Terraformer!!\n" << endl;
	
	// Console print out colors
	cout << termcolor::white;
	
	// Take in Seed
	cout << "Enter the seed: ";
	cin >> testSeed;
	
	// Take in raw_map width
	cout << "Enter the grid width: ";
	cin >> width;
	
	// Take in raw_map height
	cout << "Enter the grid height: ";
	cin >> height;
	
	// Create the raw_map with given inputs
	this->raw_map = new int*[height];
	for(int i = 0; i < height; i++){
		raw_map[i] = new int[width];
	}
	
	// Initialize the array elements to 0
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			raw_map[i][j] = 0;
		}
	}
	
	// Get Waterline
	cout << "Enter value for waterline (40 - 200): ";
	cin >> waterLine;
	
	// Get Radius
	cout << "Enter dirtball radius (minimum 2): ";
	cin >> radius;
	
	// Get Power Rating
	cout << "Enter dirtball power rating (minimum = radius): ";
	cin >> powerRating;
	
	// Get num of dirtballs
	cout << "Enter number of dirtballs to drop: ";
	cin >> numDirtballs;
}

// Map deconstructor
Map::~Map(){
	delete raw_map;
	delete polished_map;
}

/* Drop the "dirtballs". Randomly drops dirtballs with a set radius.
 * This is what really builds the shape of our map
*/
void Map::DropDirtball(){	
	srand(testSeed);
	int centerX;
	int centerY;
	int distance;	
	int impactValue;

	for(int i = 0; i < numDirtballs; i++){		
		// Get random center coordinates
		centerX = rand() % width;
		centerY = rand() % height;
		
		this->raw_map[centerY][centerX] = 1;
		
		// Calculate distance and impact value
		for(int y = 0; y < height; y++){
			for(int x = 0; x < width; x++){
				distance = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
				
				if(distance > this->radius){
					continue;
				}else{
					impactValue = this->powerRating - floor(distance);
					this->raw_map[y][x] += impactValue;
				}
			}
		}
	}
}

/* Normalizing the island means we divide each element in the map
 * by the highest value and multiple the value by 255 to get values
 * between 0 and 255 for each point.
*/
void Map::NormalizeIsland(){
	float highestValue = this->raw_map[0][0];
	
	// Find the largest point
	for(int y = 0; y < height; y++){
		for(int x = 1; x < width; x++){
			if(highestValue < this->raw_map[y][x]){
				highestValue = this->raw_map[y][x];
			}
		}
	}
	
	// Divide every point and multiply to get normalized values
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			this->raw_map[y][x] = (this->raw_map[y][x] / highestValue) * 255;
		}
	}
}

/*  PolishedIsland function determines the waterline, different land
 *  height calculations, and takes those values to create the polished
 *  map. The polished map translates each value on the raw map to symbols
 *  that represent different biomes like desert(.), grass fields(-), forrests
 * 	(*), and mountains(^). For values less that the waterline, there are symbols
 *  for deep water(#) and shallow water(~).
*/
void Map::PolishedIsland(){	
	// Water Calculations
	int halfWaterline = waterLine / 2;
	
	// Land Calculations
	int fifteenPercLand = (255 - waterLine) * 0.15;
	int fortyPercLand = (255 - waterLine) * 0.4;
	int eightyPercLand = (255 - waterLine) * 0.8;
	
	// Create Polished Map
	this->polished_map = new char*[height];
	for(int i = 0; i < height; i++){
		polished_map[i] = new char[width];
	}
	
	// Initialize the array elements to 0
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			// <= waterline
			if(this->raw_map[i][j] < halfWaterline){
				this->polished_map[i][j] = '#';
			}else if(this->raw_map[i][j] >= halfWaterline && this->raw_map[i][j] <= waterLine){
				this->polished_map[i][j] = '~';
			}else{ // >waterline
				if(this->raw_map[i][j] < (waterLine + fifteenPercLand)){
					this->polished_map[i][j] = '.';
				}else if(this->raw_map[i][j] >= (waterLine + fifteenPercLand) && this->raw_map[i][j] < (waterLine + fortyPercLand)){
					this->polished_map[i][j] = '-';
				}else if(this->raw_map[i][j] >= (waterLine + fortyPercLand) && this->raw_map[i][j] < (waterLine + eightyPercLand)){
					this->polished_map[i][j] = '*';
				}else{
					this->polished_map[i][j] = '^';
				}
			}
		}
	}	
}

// Print the map that contains the integer values
void Map::PrintRawMap(){
	for(int i = 0; i < this->height; i++){
		for(int j = 0; j < this->width; j++){
			cout << setw(4) << this->raw_map[i][j];
		}
		cout << endl;
	}
}

// Print the map with the symbols
void Map::PrintPolishedMap(){
	BMP OutPolishedMap;
	OutPolishedMap.SetSize(width, height);
	OutPolishedMap.SetBitDepth(32);
	
	for(int i = 0; i < this->height; i++){
		for(int j = 0; j < this->width; j++){
			cout << setw(2) << this->polished_map[i][j];
		}
		cout << endl;
	}
	cout << termcolor::reset;
}

// Creates a bmp with color values for each symbol in the
// polished map
void Map::PrintPolishedBitMap(){
	// Creates the bitmap class from EasyBMP files
	BMP OutPolishedMap;
	OutPolishedMap.SetSize(width, height);
	OutPolishedMap.SetBitDepth(32);
	
	// Iterates through the symboles from the polished map
	// and prints a color at the polished map symbol in the bitmap
	for(int i = 0; i < this->height; i++){
		for(int j = 0; j < this->width; j++){
			// Setting color
			if(this->polished_map[i][j] == '#'){
				OutPolishedMap(i, j)->Red = 89;
				OutPolishedMap(i, j)->Green = 111;
				OutPolishedMap(i, j)->Blue = 161;
			}else if(this->polished_map[i][j] == '~'){
				OutPolishedMap(i, j)->Red = 131;
				OutPolishedMap(i, j)->Green = 182;
				OutPolishedMap(i, j)->Blue = 196;
			}else if(this->polished_map[i][j] == '.'){
				OutPolishedMap(i, j)->Red = 228;
				OutPolishedMap(i, j)->Green = 191;
				OutPolishedMap(i, j)->Blue = 139;
			}else if(this->polished_map[i][j] == '-'){
				OutPolishedMap(i, j)->Red = 159;
				OutPolishedMap(i, j)->Green = 178;
				OutPolishedMap(i, j)->Blue = 101;
			}else if(this->polished_map[i][j] == '*'){
				OutPolishedMap(i, j)->Red = 100;
				OutPolishedMap(i, j)->Green = 137;
				OutPolishedMap(i, j)->Blue = 95;
			}else if(this->polished_map[i][j] == '^'){				
				OutPolishedMap(i, j)->Red = 181;
				OutPolishedMap(i, j)->Green = 186;
				OutPolishedMap(i, j)->Blue = 182;
			}
			OutPolishedMap(i, j)->Alpha = 0;
		}
	}
	OutPolishedMap.WriteToFile("PolishedMap.bmp");
}

// Prints colored map to console
// Uses symbols to determine color
void Map::SetTermColor(char currChar){
	// Setting color
	if(currChar == '#'){
		cout << termcolor::on_blue;
		cout << termcolor::blue;
	}else if(currChar == '~'){
		cout << termcolor::on_bright_blue;
		cout << termcolor::bright_blue;	
	}else if(currChar == '.'){
		cout << termcolor::on_yellow;
		cout << termcolor::yellow;
	}else if(currChar == '-'){
		cout << termcolor::on_bright_green;
		cout << termcolor::bright_green;
	}else if(currChar == '*'){
		cout << termcolor::on_green;
		cout << termcolor::green;
	}else if(currChar == '^'){
		cout << termcolor::on_bright_grey;
		cout << termcolor::bright_grey;
	}
}
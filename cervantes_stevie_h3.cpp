/* Author: Stevie Cervantes
 * Created: July 25, 2024
 * File Name: cervantes_stevie_h3.hpp
 * Description: Contains the driver code for the project. Creates
 * 				a map, drops dirtballs, normalizes values, and 
 * 				polishes island with symbols, then creates a bitmap
 * 				with the colored map. Changing the printing function
 * 				will result in different output.
*/

#include "Map.hpp"
#include <fstream>
using namespace std;

int main(int argc, char** argv){
	string file = "polishedMap.txt";
	Map* asciiMap = new Map(file);	
	
	// Raw Map
	asciiMap->DropDirtball();
	// asciiMap->PrintRawMap();
	
	cout << endl;
	
	// Normalized Map
	asciiMap->NormalizeIsland();
	// asciiMap->PrintRawMap();
	
	cout << endl;
	
	// Polished Map
	asciiMap->PolishedIsland();
	asciiMap->PrintPolishedBitMap();
	return 0;
}

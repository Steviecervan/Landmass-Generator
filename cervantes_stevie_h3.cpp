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

#include<iostream>
#include<AssetManager.h>

using namespace schwi;
using namespace std;

int main() {
	AssetManager<SchwiImage> imgManager;
	string filename = "earthmap.png";
	imgManager.Add(filename);
}
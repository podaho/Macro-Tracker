#include <ostream>
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>

using std::string;
using std::cout;
using std::vector;

class Ingredient {
private:
	FILE *file;
	string name;
	int nmLen;
	int unLen;
	float calories = 0.0;
	float fat = 0.0;
	float carb = 0.0;
	float protein = 0.0;
	float fiber = 0.0;
	string unit = "";
	
	void writeInfo() {
		//cout << "Writing " << name.c_str() << ", " << calories << ", " << fat << ", " << carb << ", " << protein << ", " << fiber << ", " << unit.c_str() << "\n";
		nmLen = name.length();
		unLen = unit.length();
		fseek(file, 0, SEEK_SET);
		fwrite(&nmLen, sizeof(int), 1, file);
		for (int i = 0; i < nmLen; i++) {
			int x = int(name.at(i));
			fwrite(&x, sizeof(int), 1, file);
		}
		fwrite(&calories, sizeof(float), 1, file);
		fwrite(&fat, sizeof(float), 1, file);
		fwrite(&carb, sizeof(float), 1, file);
		fwrite(&protein, sizeof(float), 1, file);
		fwrite(&fiber, sizeof(float), 1, file);
		fwrite(&unLen, sizeof(int), 1, file);
		for (int i = 0; i < unLen; i++) {
			int x = int(unit.at(i));
			fwrite(&x, sizeof(int), 1, file);
		}
	}
	
	void readInfo() {
		fseek(file, 0, SEEK_SET);
		fread(&nmLen, sizeof(int), 1, file);
		name = "";
		unit = "";
		for (int i = 0; i < nmLen; i++) {
			int x;
			fread(&x, sizeof(int), 1, file);
			name += char(x);
		}
		fread(&calories, sizeof(float), 1, file);
		fread(&fat, sizeof(float), 1, file);
		fread(&carb, sizeof(float), 1, file);
		fread(&protein, sizeof(float), 1, file);
		fread(&fiber, sizeof(float), 1, file);
		fread(&unLen, sizeof(int), 1, file);
		for (int i = 0; i < unLen; i++) {
			int x;
			fread(&x, sizeof(int), 1, file);
			unit += char(x);
		}
		//cout << "Reading " << name.c_str() << ", " << calories << ", " << fat << ", " << carb << ", " << protein << ", " << fiber << ", " << unit.c_str() << "\n";
	}

public:
	Ingredient(const string &nm, const float &cl, const float &ft, const float &cb, const float &pt, const float &fb, const string &un) {
		file = std::fopen((nm+".bin").c_str(), "rb+");
		name = nm;
		nmLen = nm.length();
		if (file == nullptr) {
			int numFoods;
			file = fopen((nm + ".bin").c_str(), "wb+");
		}
		else {
			readInfo();
			cout << "Ingredient already exists; overwriting data: \n";
			cout << "Name:     " << name.c_str() << "\n";
			cout << "Calories: " << calories << " -> " << cl << "\n";
			cout << "Fat:      " << fat << " -> " << ft << " g\n";
			cout << "Carb:     " << carb << " -> " << cb << " g\n";
			cout << "Protein:  " << protein << " -> " << pt << " g\n";
			cout << "Fiber:    " << fiber << " -> " << fb << " g\n";
			cout << "Unit:     " << unit.c_str() << " -> " << un.c_str() << "\n";
		}
		name = nm;
		calories = cl;
		fat = ft;
		carb = cb;
		protein = pt;
		fiber = fb;
		unit = un;
		writeInfo();
		readInfo();
	}

	Ingredient(const string &nm) {
		file = std::fopen((nm + ".bin").c_str(), "rb+");
		name = nm;
		nmLen = nm.length();
		if (file == nullptr) {
			cout << nm.c_str() << " does not exist!\n";
		}
		else {
			readInfo();
		}
	}

	~Ingredient() {
		fclose(file);
	}

	void prettyPrint() {
		cout << "  =====================================\n";
		cout << "  = Name:    " << name << "\n";
		cout << "  = Serving: " << unit << "\n";
		cout << "  =====================================\n";
		cout << "  = Calories:     " << calories << "\n";
		cout << "  =-----------------------------------=\n";
		cout << "  = Fat:          " << fat << " g\n";
		cout << "  = Carbohydrate: " << carb << " g\n";
		cout << "  = Protein:      " << protein << " g\n";
		cout << "  = Fiber:        " << fiber << " g\n";
		cout << "  =====================================\n";
		cout << "\n";
	}

	void editName(string &nm) {
		readInfo();
		fclose(file);
		file = fopen((nm + ".bin").c_str(), "rb+");
		if (file != nullptr) {
			cout << "Food Name Already Exists... Name edit failed!\n";
			file = fopen((name + ".bin").c_str(), "rb+");
		}
		else {
			file = fopen((nm + ".bin").c_str(), "wb+");
			if (remove((name+".bin").c_str()) != 0) perror("Error Deleting File");
			name = nm;
			writeInfo();
			readInfo();
		}
	}

	void editUnit(string &txt) { 
		unit = txt;
		writeInfo();
		readInfo();
	}

	void editCal(float &cal) { 
		calories = cal;
		writeInfo();
		readInfo();
	}

	void editFat(float &ft) { 
		fat = ft;
		writeInfo();
		readInfo();
	}
	
	void editCarb(float &cb) { 
		carb = cb;
		writeInfo();
		readInfo();
	}
	
	void editProt(float &pt) { 
		protein = pt;
		writeInfo();
		readInfo();
	}
	
	void editFib(float &fb) { 
		fiber = fb;
		writeInfo();
		readInfo();
	}

	string getName() {
		return name;
	}

	string getUnit() {
		return unit;
	}

	float getCal() {
		return calories;
	}

	float getFat() {
		return fat;
	}

	float getCarb() {
		return carb;
	}

	float getProt() {
		return protein;
	}

	float getFib() {
		return fiber;
	}
};

class FoodMenu {
private: 
	FILE *file;
	string name;
	vector<Ingredient> menuLst;
	int nmLen;
	float totCal, totFat, totCarb, totProt, totFib;
public:
	FoodMenu(string &nm) {
		file = std::fopen((nm + "Menu.bin").c_str(), "rb+");
		name = nm;
		nmLen = nm.length();
		if (file == nullptr) {
			int numFoods;
			file = fopen((nm + "Menu.bin").c_str(), "wb+");
			name = nm;
			cout << "New Menu Added!\n";
		}
		else {
			vector<string> strLst = fetchFoods(1);
			totCal = 0.0;
			totFat = 0.0;
			totCarb = 0.0;
			totProt = 0.0;
			totFib = 0.0;
			for (auto x = strLst.begin(); x != strLst.end(); x++) {
				Ingredient ing(*x);
				menuLst.push_back(ing);
				totCal += ing.getCal();
				totFat += ing.getFat();
				totCarb += ing.getCarb();
				totProt += ing.getProt();
				totFib += ing.getFib();
			}
		}
	}

	vector<string> fetchFoods(int mode) {
		vector<string> ret;
		int numFoods;
		fseek(file, 0, SEEK_SET);
		fread(&numFoods, sizeof(int), 1, file);
		cout << numFoods << " foods found for " << name << "!\n";
		for (int i = 0; i < numFoods; i++) {
			string name;
			int strLen;
			fread(&strLen, sizeof(int), 1, file);
			for (int j = 0; j < strLen; j++) {
				int x;
				fread(&x, sizeof(int), 1, file);
				name += char(x);
			}
			ret.push_back(name);
			if (mode == 1) {
				Ingredient ing(name);
				cout << i + 1 << ". \n";
				ing.prettyPrint();
			}
		}
		return ret;
	}

	void addFood(string &name, const float &cl, const float &ft, const float &cb, const float &pt, const float &fb, const string &un) {
		//cout << "addfoods() begin\n";
		int numFoods, nmLen;
		fseek(file, 0, SEEK_SET);
		fread(&numFoods, sizeof(int), 1, file);
		//cout << numFoods << " foods prior to add\n";
		numFoods += 1;
		fseek(file, 0, SEEK_SET);
		fwrite(&numFoods, sizeof(int), 1, file);
		for (int i = 0; i < numFoods - 1; i++) {
			int strLen;
			fread(&strLen, sizeof(int), 1, file);
			for (int j = 0; j < strLen; j++) {
				int x;
				fread(&x, sizeof(int), 1, file);
			}
		}
		nmLen = name.length();
		fwrite(&nmLen, sizeof(int), 1, file);
		for (int i = 0; i < nmLen; i++) {
			int x = int(name.at(i));
			fwrite(&x, sizeof(int), 1, file);
		}
		//cout << "addfoods() end\n";
		Ingredient ing(name, cl, ft, cb, pt, fb, un);
		menuLst.push_back(ing);
	}

	void updateMenuList() {
		int numFoods = menuLst.size();
		int nmLen;
		fseek(file, 0, SEEK_SET);
		fwrite(&numFoods, sizeof(int), 1, file);
		for (auto x = menuLst.begin(); x != menuLst.end(); x++) {
			nmLen = ((*x).getName()).length();
			fwrite(&nmLen, sizeof(int), 1, file);
			for (int i = 0; i < nmLen; i++) {
				int y = int(((*x).getName()).at(i));
				fwrite(&y, sizeof(int), 1, file);
			}
		}
	}

	void editFoodName(string &name, int idx) {
		//cout << "editFoodName() begin\n";
		int nmLen;
		int initNum = 0;
		vector<string> names = fetchFoods(0);
		fseek(file, 0, SEEK_SET);
		fwrite(&initNum, sizeof(int), 1, file);
		menuLst[idx].editName(name);
		updateMenuList();
	}
};

vector<string> fetchFoods(int mode) {
	//cout << "fetchfoods() begin\n";
	FILE *file;
	vector<string> ret;
	int numFoods;
	file = fopen("FoodList.bin", "rb+");
	fseek(file, 0, SEEK_SET);
	fread(&numFoods, sizeof(int), 1, file);
	//cout << numFoods << " foods found!\n";
	for (int i = 0; i < numFoods; i++) {
		string name, unit;
		int strLen, unLen;
		fread(&strLen, sizeof(int), 1, file);
		for (int j = 0; j < strLen; j++) {
			int x;
			fread(&x, sizeof(int), 1, file);
			name += char(x);
		}
		ret.push_back(name);
		if (mode == 1) {
			Ingredient ing(name);
			cout << i + 1 << ". \n";
			ing.prettyPrint();
		}
	}
	fclose(file);
	//cout << "fetchfoods() end\n";
	return ret;
}

void addFood(string &name) {
	//cout << "addfoods() begin\n";
	FILE *file;
	int numFoods, nmLen;
	file = fopen("FoodList.bin", "rb+");
	fseek(file, 0, SEEK_SET);
	fread(&numFoods, sizeof(int), 1, file);
	//cout << numFoods << " foods prior to add\n";
	numFoods += 1;
	fseek(file, 0, SEEK_SET);
	fwrite(&numFoods, sizeof(int), 1, file);
	for (int i = 0; i < numFoods-1; i++) {
		int strLen;
		fread(&strLen, sizeof(int), 1, file);
		for (int j = 0; j < strLen; j++) {
			int x;
			fread(&x, sizeof(int), 1, file);
		}
	}
	nmLen = name.length();
	fwrite(&nmLen, sizeof(int), 1, file);
	for (int i = 0; i < nmLen; i++) {
		int x = int(name.at(i));
		fwrite(&x, sizeof(int), 1, file);
	}
	fclose(file);
	//cout << "addfoods() end\n";
}

void editFoodName(string &name, int &idx) {
	//cout << "editFoodName() begin\n";
	FILE *file;
	int nmLen;
	int initNum = 0;
	vector<string> names = fetchFoods(0);
	file = fopen("FoodList.bin", "rb+");
	fseek(file, 0, SEEK_SET);
	fwrite(&initNum, sizeof(int), 1, file);
	fclose(file);
	for (int i = 0; i < names.size(); i++) {
		if (i == idx) {
			addFood(name);
		}
		else {
			addFood(names[i]);
		}
	}
	//cout << "editFoodName() end\n";
}

int main() {
	FILE *file;
	int numFoods;
	file = std::fopen("FoodList.bin", "rb+");
	if (file == nullptr) {
		file = fopen("FoodList.bin", "wb+");
		fseek(file, 0, SEEK_SET);
		numFoods = 0;
		fwrite(&numFoods, sizeof(int), 1, file);
		fseek(file, 0, SEEK_SET);
		fread(&numFoods, sizeof(int), 1, file);
		cout << numFoods << " foods found; First time use!\n";
	}
	else {
		fseek(file, 0, SEEK_SET);
		fread(&numFoods, sizeof(int), 1, file);
		cout << numFoods << " foods found!\n";
	}
	fclose(file);

	string optionMain;
	string optionFoods;
	bool foods = false;
	cout << "Main Menu:\n";
	cout << "1. List/Select Users\n";
	cout << "2. Add User\n";
	std::getline(std::cin, optionMain);
	if (optionMain == "1") {
		int numUsers = 0;
		file = fopen("Users.bin", "rb+");
		if (file == nullptr) {
			file = fopen("Users.bin", "wb+");
			fseek(file, 0, SEEK_SET);
			fwrite(&numUsers, sizeof(int), 1, file);
			fseek(file, 0, SEEK_SET);
			fread(&numUsers, sizeof(int), 1, file);
			cout << numUsers << " users found; First time use!\n";
		}
		else {
			fseek(file, 0, SEEK_SET);
			fread(&numUsers, sizeof(int), 1, file);
			for (int i = 0; i < numUsers; i++) {
				int nmLen;
				string name;
				fread(&nmLen, sizeof(int), 1, file);
				for (int j = 0; j < nmLen; j++) {
					int x;
					fread(&x, sizeof(int), 1, file);
					name += char(x);
				}
				cout << i + 1 << ". " << name << "\n";
				//IN PROGRESS~IN PROGRESS~IN PROGRESS~IN PROGRESS~IN PROGRESS~IN PROGRESS~IN PROGRESS~IN PROGRESS~IN PROGRESS~IN PROGRESS~
				//NEED TO KEEP A MASTER LIST OF INGREDIENTS FOR REFERENCE/ADDING
			}
		}
	}



	if (optionMain == "1") foods = true;
	while (foods) {
		cout << "Food Menu:\n";
		cout << "1. List Foods\n";
		cout << "2. Add Foods\n";
		cout << "3. Edit Foods\n";
		std::getline(std::cin, optionFoods);
		system("cls");
		if (optionFoods == "1") {
			fetchFoods(1);
		}
		else if (optionFoods == "2") {
			string nm, un, tmp;
			float cal, ft, cb, pt, fb;
			cout << "Enter Food Name:\n";
			std::getline(std::cin, nm);
			cout << "Enter Food Calories:\n";
			std::getline(std::cin, tmp);
			cal = std::stof(tmp);
			cout << "Enter Food Fat:\n";
			std::getline(std::cin, tmp);
			ft = std::stof(tmp);
			cout << "Enter Food Carbs:\n";
			std::getline(std::cin, tmp);
			cb = std::stof(tmp);
			cout << "Enter Food Protein:\n";
			std::getline(std::cin, tmp);
			pt = std::stof(tmp);
			cout << "Enter Food Fiber:\n";
			std::getline(std::cin, tmp);
			fb = std::stof(tmp);
			cout << "Enter Basic Unit:\n";
			std::getline(std::cin, un);
			Ingredient ingr(nm, cal, ft, cb, pt, fb, un);
			addFood(nm);
		}
		else if (optionFoods == "3") {
			string editIndex;
			bool editing = true;
			vector<string> foodLst = fetchFoods(1);
			cout << "Select Associated Food Number From Selection Above:\n";
			std::getline(std::cin, editIndex);
			string itemName = foodLst[std::stoi(editIndex)-1];
			Ingredient ing(itemName);
			while (editing) {
				string editChoice;
				cout << "Which would you like to change about " << itemName.c_str() << "?\n";
				cout << "1. Name\n";
				cout << "2. Serving Size\n";
				cout << "3. Calories\n";
				cout << "4. Fat\n";
				cout << "5. Carbohydrates\n";
				cout << "6. Protein\n";
				cout << "7. Fiber\n";
				cout << "8. Return to Food Menu\n";
				std::getline(std::cin, editChoice);
				if (editChoice == "1") {
					string newName;
					cout << "Enter New Name:\n";
					std::getline(std::cin, newName);
					cout << "got line\n";
					ing.editName(newName);
					cout << "edit name class\n";
					int idx = std::stoi(editIndex) - 1;
					editFoodName(newName, idx);
					cout << "edited food name at index " << idx << "\n";
				}
				else if (editChoice == "2") {
					string newUnit;
					cout << "Enter New Serving Size:\n";
					std::getline(std::cin, newUnit);
					ing.editUnit(newUnit);
				}
				else if (editChoice == "3") {
					string newCalories;
					cout << "Enter New Calories:\n";
					std::getline(std::cin, newCalories);
					float input = std::stof(newCalories);
					ing.editCal(input);
				}
				else if (editChoice == "4") {
					string newFat;
					cout << "Enter New Fat:\n";
					std::getline(std::cin, newFat);
					float input = std::stof(newFat);
					ing.editFat(input);
				}
				else if (editChoice == "5") {
					string newCarbs;
					cout << "Enter New Carbohydrate:\n";
					std::getline(std::cin, newCarbs);
					float input = std::stof(newCarbs);
					ing.editCarb(input);
				}
				else if (editChoice == "6") {
					string newProt;
					cout << "Enter New Protein:\n";
					std::getline(std::cin, newProt);
					float input = std::stof(newProt);
					ing.editProt(input);
				}
				else if (editChoice == "7") {
					string newFib;
					cout << "Enter New Fiber:\n";
					std::getline(std::cin, newFib);
					float input = std::stof(newFib);
					ing.editProt(input);
				}
				else if (editChoice == "8") {
					editing = false;
				}
			}
		}
	}
}

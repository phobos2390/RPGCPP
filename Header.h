#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "LinkedList.h"
//#include "LinkedStack.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sstream>
#include <Windows.h>

using namespace std;

class Item;
	class Weapon;
	class Potion;
	class Bag;
class InventorySpace;
class InteractableObject;
	class Character;
		class Enemy;
		class Player;
	class Location;
class Game;

static bool newGame;

int numberOfEnemiesForLevelUp, intitialAggregate;
string defaultDirectory;

ofstream error;

char* concatenateStrings(char* oneString, char* anotherString)
{
	char* toReturn = new char[strlen(oneString)+strlen(anotherString)];
	strcpy(toReturn, oneString);
	strcat(toReturn, anotherString);
	return toReturn;
}

char* convertInttoCharA(int toConvert)
{
	char* toReturn = new char[10];
	_itoa(toConvert, toReturn, 10);
	return toReturn;
}

int Menu(const LinkedList<string>& items)
{
	for(LinkedList<string>::Iterator i = items.begin(); i; i++)
	{
		cout<<*i<<endl;
	}
	cout<<"Enter in your choice: ";
	string choice;
	cin >> choice;
	stringstream ss;
	ss << choice;
	int intChoice;
	ss >> intChoice;
	if(1<=intChoice&&intChoice<=items.size())
	{
		return --intChoice;
	}
	else if(intChoice<1||intChoice>items.size())
	{
		cout<<"ERROR Must Enter in a number on the list"<<endl;
		return Menu(items);
	}
	else
	{
		cout<<"ERROR not numerical"<<choice<<endl;
		return Menu(items);
	}
	return 0;
}

int Menu(int number, LinkedList<string>& items)
{
	LinkedList<string> menuItems;
	for(int i = 0; i < items.size(); i++)
	{
		stringstream ss;
		ss<<(i+1)<<": "<<items[i];
		menuItems+=ss.str();
	}
	return Menu(menuItems);
}

bool YesNoMenu()
{
	LinkedList<string> items;
	items += "1: Yes";
	items += "2: No";
	return (Menu(items)==0);
}

bool equal(char* firstString, char* secondString)
{
	int length = strlen(firstString);
	if(length == strlen(secondString))
	{
		for(int i = 0; i < length; i++)
		{
			if(firstString[i] != secondString[i])
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

enum MoveType
{
	MELEEATTACK,
	MELEEDEFENSE,
	MAGICATTACK,
	MAGICDEFENSE,
	MAGICHEALTH,
	RANGED,
	POTION,
	SEE
};

class Move
{
private:
	string identifier;
	string description;
	int bonus, speedReduction;
	MoveType type;
public:
	Move(string identifier, string description, MoveType type, int bonus = 0, int speedReduction = 0):identifier(identifier),description(description),type(type),bonus(bonus),speedReduction(speedReduction){} 
	Move(ifstream& file)
	{
		string buffer;
		bool continuing = true;
		getline(file,identifier);
		//moveIdentifier = buffer;
		description = "";
		while(continuing)
		{
			getline(file, buffer);
			if(buffer=="End Description")
			{
				continuing = false;
			}
			else if(buffer!="")
			{
				if(description=="")
				{
					description = buffer;
				}
				else
				{
					description += "\n" + buffer;
				}
			}
		}
		int intType;
		file>>intType>>bonus>>speedReduction;
		type = (MoveType)intType;
		getline(file,buffer);
	}
	~Move(){}
	int getSpeedReduction(){return speedReduction;}
	string getIdentifier(){return identifier;}
	string getDescription(){return description;}
	string getBattleVerb();
	MoveType getType(){return type;}
	ostream& saveOps(ostream& os);
	void edit();
	void execute(Character* initiator, Character* target);
};

string Move::getBattleVerb()
{
	if(identifier=="Basic Attack")
	{
		return "Attacked";
	}
	else if(identifier=="Basic Defense")
	{
		return "Defended";
	}
	else
	{
		stringstream returnStream;
		returnStream<<"used "<<identifier;
		return returnStream.str();
	}
}

ostream& Move::saveOps(ostream& os)
{
	os<<"New Move"<<endl;
	os<<identifier<<endl;
	os<<description<<endl<<"End Description"<<endl;
	os<<type<<" "<<bonus<<" "<<speedReduction;
	return os;
}

void Move::edit()
{
	bool continuing = true;
	LinkedList<string> options;
	options += "Edit Identifier";
	options += "Edit Description";
	options += "Edit Bonus";
	options += "Edit Speed Reduction";
	options += "Edit Move Type";
	options += "Back";
	while(continuing)
	{
		cout<<identifier<<endl;
		cout<<description<<endl;
		cout<<"Bonus:           "<<bonus<<endl;
		cout<<"Speed Reduction: "<<speedReduction<<endl;
		string choice = options[Menu(0,options)];
		if(choice == "Edit Identifier")
		{
			cout<<identifier<<endl;
			cout<<"New Identifier?"<<endl;
			if(YesNoMenu())
			{
				cout<<"Enter New Identifier"<<endl;
				cin>>identifier;
			}
		}
		else if(choice == "Edit Description")
		{
			cout<<description<<endl;
			cout<<"New Description?"<<endl;
			if(YesNoMenu())
			{
				cout<<"Enter New Description. To Finish, Enter End"<<endl;
				string buffer = "";
				description = "";
				while(continuing)
				{
					getline(cin,buffer);
					if(buffer != "End"&&buffer != "end")
					{
						if(description == "")
						{
							description = buffer;
						}
						else
						{
							description += "\n" + buffer;
						}
					}
					else
					{
						continuing = false;
					}
				}
				continuing = true;
			}
		}
		else if(choice == "Edit Move Type")
		{
			LinkedList<string> moveTypeOptions;
			moveTypeOptions += "Melee Attack";
			moveTypeOptions += "Melee Defense";
			moveTypeOptions += "Back";
			while(continuing)
			{
				choice = moveTypeOptions[Menu(0,moveTypeOptions)];
				if(choice == "Melee Attack")
				{
					type = MELEEATTACK;
				}
				else if(choice == "Melee Defense")
				{
					type = MELEEDEFENSE;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
			continuing = true;
		}
		else if(choice == "Edit Speed Reduction")
		{
			cout<<"Enter in new Speed Reduction: ";
			cin>>speedReduction;
		}
		else if(choice == "Edit Bonus")
		{
			cout<<"Enter in new Bonus: ";
			cin>>bonus;
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

class Item
{
protected:
	string identifier;
	string description;
	int weight;
	int height;
	int width;
	bool specialUse;
	virtual ostream& streamOperator(ostream& os);
	Item(int height, int width, int weight):height(height), width(width), weight(weight), specialUse(false){}
	Item(int height, int width):height(height), width(width), weight(height*width), specialUse(false){}
	Item(int height, int width, int weight, bool specialUse):height(height), width(width), weight(weight), specialUse(specialUse){}
	Item(int height, int width, bool specialUse):height(height), width(width), weight(height*width), specialUse(specialUse){}
	Item(ifstream& file)
	{
		file>>height>>width>>weight>>specialUse;
	}
public:
	virtual ~Item(){}
	virtual void edit();
	static Item* getItemFromFile(ifstream& file);
	string getIdentifier(){return identifier;}
	string getDescription(){return description;}
	virtual int getWeight(){return weight;}
	bool isSpecialUse(){return specialUse;}
	int getHeight(){return height;}
	int getWidth(){return width;}
	virtual ostream& saveOps(ostream& os);
	void save(char* fileName, LinkedList<Item*> itemsToSave);
	LinkedList<Item*> load(string fileName);
	virtual bool isKeyItem(){return false;}
	virtual int getValue();
	virtual Item* rotate();
	virtual string toString();
	virtual bool isOfType(char type);
	virtual char displayInInventory(){return 'I';}
	virtual void interactWithCharacter(Character* itemUser){}
	virtual bool operator==(Item* otherItem);
	friend ostream& operator<<(ostream& os, Item* toString);
};

void Item::save(char* fileName, LinkedList<Item*> itemsToSave)
{
	ofstream file;
	file.open(fileName);
	for(int i = 0; i < itemsToSave.size(); i++)
	{
		itemsToSave[i]->saveOps(file);
		file<<endl;
	}
	file.close();
}

int Item::getValue()
{
	return 10*getWeight();
}

string Item::toString()
{
	return concatenateStrings("Item. Weight of ", concatenateStrings(convertInttoCharA(getWeight()), "."));
}

ostream& Item::streamOperator(ostream& os)
{
	os<<"Item. Weight of "<<weight<<".";
	return os;
}

void Item::edit()
{
	bool continuing = true;
	LinkedList<string> options;
	options += "Edit Height";
	options += "Edit Width";
	options += "Edit Weight";
	options += "Back";
	while(continuing)
	{
		string choice = options[Menu(0,options)];
		if(choice == "Edit Height")
		{
			cout<<"Enter in new Height: ";
			cin>>height;
		}
		else if(choice == "Edit Width")
		{
			cout<<"Enter in new Width: ";
			cin>>width;
		}
		else if(choice == "Edit Weight")
		{
			cout<<"Enter in new Weight: ";
			cin>>weight;
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

ostream& Item::saveOps(ostream& os)
{
	os<<displayInInventory()<<" "<<height<<" "<<width<<" "<<weight<<" "<<specialUse;
	return os;
}

ostream& operator<<(ostream& os, Item* toString)
{
	return toString->streamOperator(os);
}

Item* Item::rotate()
{
	return new Item(width, height);
}

bool Item::isOfType(char type)
{
	return type == 'I'||type == 'i';
}

bool Item::operator==(Item*otherItem)
{
	return (height==otherItem->height)&&(width==otherItem->width)&&(weight==otherItem->weight)&&(specialUse==otherItem->specialUse);
}

class Weapon : public Item
{
protected:
	int damage;
	LinkedList<Move*> weaponMoves;
	ostream& streamOperator(ostream& os);
public:
	Weapon(string identifier, string description, int height, int width, int damage):Item(height, width), damage(damage)
	{
		 this->identifier=identifier; 
		 this->description=description;
	}
	Weapon(string identifier, string description, int height, int width, int damage, int weight):Item(height, width, weight), damage(damage)
	{
		this->identifier=identifier; 
		this->description=description;
	}
	Weapon(string identifier, string description, int height, int width, int damage, int weight, Move* weaponMove):Item(height, width, weight), damage(damage)
	{
		//weaponMoves+=weaponMove;
		addMove(weaponMove);
		this->identifier=identifier; 
		this->description=description;
	}
	Weapon(string identifier, string description, int height, int width, int damage, int weight, const LinkedList<Move*>& weaponMoves):Item(height, width, weight),damage(damage),weaponMoves(weaponMoves)
	{
		this->identifier=identifier; 
		this->description=description;
	}
	Weapon(ifstream& file):Item(file)
	{
		file>>damage;
		string buffer;
		getline(file,buffer);
		getline(file,identifier);
		description = "";
		bool continuing = true;
		while(continuing)
		{
			getline(file, buffer);
			if(buffer=="End Description")
			{
				continuing = false;
			}
			else if(description==""&&buffer!="")
			{
				description = buffer;
			}
			else
			{
				description += "\n" + buffer;
			}
		}
		continuing = true;
		getline(file,buffer);
		if(buffer=="Start Moves")
		{
			while(continuing)
			{
				getline(file,buffer);
				if(buffer=="End Moves")
				{
					continuing = false;
				}
				else
				{
					weaponMoves += new Move(file);
					continuing = true;
				}
			}
		}
	}
	~Weapon()
	{
		while(weaponMoves.size()>0)
		{
			delete weaponMoves.remove(0);
		}
	}
	virtual LinkedList<Move*> getWeaponMoves()
	{
		return weaponMoves;
	}
	void addMove(Move* weaponMove){weaponMoves+=weaponMove;}
	virtual int getDamage(){return damage;}
	int getValue();
	Item* rotate();
	string toString();
	bool isOfType(char type);
	char displayInInventory(){return 'W';}
	ostream& saveOps(ostream& os);
	void edit();
};

ostream& Weapon::saveOps(ostream& os)
{
	Item::saveOps(os);
	os<<" "<<getDamage()<<endl;
	os<<getIdentifier()<<endl;
	os<<getDescription()<<endl<<"End Description"<<endl;
	os<<"Start Moves"<<endl;
	for(int i = 0; i < weaponMoves.size(); i++)
	{
		weaponMoves[i]->saveOps(os);
		os<<endl;
	}
	os<<"End Moves";
	return os;
}

int Weapon::getValue()
{
	return damage * weight;
}

Item* Weapon::rotate()
{
	return new Weapon(identifier,description,width, height, damage, weight,weaponMoves);
}

string Weapon::toString()
{
	string returnString;
	returnString = getIdentifier() + "\n-" + getDescription();
	if(0<getWeaponMoves().size())
	{
		for(int i = 0; i < weaponMoves.size(); i++)
		{
			returnString = returnString + "\n-" + weaponMoves[i]->getIdentifier() + "\n *" + getDescription();
		}
	}
	else
	{
		returnString = returnString + "-None\n";
	}
	return returnString;
}

bool Weapon::isOfType(char type)
{
	return Item::isOfType(type)||type == 'W'||type == 'w';
}

ostream& Weapon::streamOperator(ostream& os)
{
	cout<<getIdentifier()<<"\n-"<<getDescription()<<endl;
	if(0<getWeaponMoves().size())
	{
		cout<<endl;
		for(int i = 0; i < weaponMoves.size(); i++)
		{
			cout<<"-"<<weaponMoves[i]->getIdentifier()<<"\n *"<<weaponMoves[i]->getDescription();
		}
	}
	else
	{
		cout<<"-None";
	}
	return os;
}

void Weapon::edit()
{
	LinkedList<string> options;
	options += "Edit Identifier";
	options += "Edit Description";
	options += "Edit Moves";
	options += "Edit Damage";
	options += "Edit Dimensions";
	options += "Save Ops Preview";
	options += "Back";
	bool continuing = true;
	while(continuing)
	{
		string choice = options[Menu(0,options)];
		if(choice == "Save Ops Preview")
		{
			saveOps(cout);
			cout<<endl;
		}
		if(choice == "Edit Identifier")
		{
			cout<<identifier<<endl;
			cout<<"New Identifier?"<<endl;
			if(YesNoMenu())
			{
				cout<<"Enter New Identifier"<<endl;
				cin>>identifier;
			}
		}
		else if(choice == "Edit Description")
		{
			cout<<description<<endl;
			cout<<"New Description?"<<endl;
			if(YesNoMenu())
			{
				cout<<"Enter New Description. To Finish, Enter End"<<endl;
				string buffer = "";
				description = "";
				while(continuing)
				{
					getline(cin,buffer);
					if(buffer != "End"&&buffer != "end")
					{
						if(description == "")
						{
							description = buffer;
						}
						else
						{
							description += "\n" + buffer;
						}
					}
					else
					{
						continuing = false;
					}
				}
				continuing = true;
			}
		}
		else if(choice == "Edit Moves")
		{
			while(continuing)
			{
				LinkedList<string> movesOptions;
				movesOptions += "New Move";
				for(LinkedList<Move*>::Iterator i = weaponMoves.begin(); i; i++)
				{
					movesOptions += (*i)->getIdentifier();
				}
				movesOptions += "Back";
				int iChoice = Menu(0,movesOptions);
				choice = movesOptions[iChoice];
				if(choice == "New Move")
				{
					weaponMoves += new Move("Default Move","Default Description",MoveType::MELEEATTACK,0,0);
					weaponMoves.setToLast();
					weaponMoves.currentData()->edit();
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
				else
				{
					weaponMoves[iChoice]->edit();
				}
			}
			continuing = true;
		}
		else if(choice == "Edit Damage")
		{
			cout<<"Enter in new damage: ";
			cin>>damage;
		}
		else if(choice == "Edit Dimensions")
		{
			Item::edit();
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

enum PotionType
{
	HEALTH,
	STRENGTH,
	DEFENSE,
	SPEED
};

class Potion : public Item
{
protected:
	int strength;
	PotionType type;
	ostream& streamOperator(ostream& os);
public:
	Potion(int strength, PotionType type):Item(1,1,true), strength(strength),type(type)
	{
		if(type == DEFENSE)
		{
			identifier = "Defense";
		}
		else if(type == HEALTH)
		{
			identifier = "Health";
		}
		else if(type == STRENGTH)
		{
			identifier = "Strength";
		}
		else
		{
			identifier = "Speed";
		}
		this->identifier += " Potion";
		description = this->toString();
	}
	Potion(int strength, PotionType type, bool specialUse):Item(1,1,true, specialUse), strength(strength), type(type)
	{
		if(type == DEFENSE)
		{
			identifier = "Defense";
		}
		else if(type == HEALTH)
		{
			identifier = "Health";
		}
		else if(type == STRENGTH)
		{
			identifier = "Strength";
		}
		else
		{
			identifier = "Speed";
		}
		identifier += " Potion";
		description = this->toString();
	}
	Potion(ifstream& file):Item(file)
	{
		int intType = 0;
		file>>intType>>strength;
		type = (PotionType)intType;
		if(type == DEFENSE)
		{
			identifier = "Defense";
		}
		else if(type == HEALTH)
		{
			identifier = "Health";
		}
		else if(type == STRENGTH)
		{
			identifier = "Strength";
		}
		else
		{
			identifier = "Speed";
		}
		identifier += " Potion";
		description = this->toString();
	}
	~Potion(){}
	int getStrength(){return strength;}
	int getValue();
	string toString();
	string getPotionTypeToString();
	PotionType getPotionType(){return type;}
	bool isOfType(char type);
	char displayInInventory(){return 'P';}
	void edit();
	ostream& saveOps(ostream& os);
};

ostream& Potion::saveOps(ostream& os)
{
	Item::saveOps(os);
	os<<" "<<type<<" "<<strength;
	/*if(type == HEALTH)
	{
		os<<0;
	}
	else if(type == STRENGTH)
	{
		os<<1;
	}
	else if(type == DEFENSE)
	{
		os<<2;
	}
	else if(type == SPEED)
	{
		os<<3;
	}
	else
	{
		os<<4;
	}*/
	return os;
}

int Potion::getValue()
{
	return strength;
}

string Potion::toString()
{
	stringstream ss;
	ss<<getPotionTypeToString()<<" potion. Strength of "<<strength<<". Weight of "<<weight;
	return ss.str();
}

string Potion::getPotionTypeToString()
{
	string theType;
	if(type == HEALTH)
	{
		theType = "Health";
	}
	else if(type == STRENGTH)
	{
		theType = "Strength";
	}
	else if(type == DEFENSE)
	{
		theType = "Defense";
	}
	else
	{
		theType = "Speed";
	}
	return theType;
}

bool Potion::isOfType(char type)
{
	return Item::isOfType(type)||type == 'P'||type == 'p';
}

ostream& Potion::streamOperator(ostream& os)
{
	os<<getDescription();
	return os;
}

void Potion::edit()
{
	bool continuing = true;
	LinkedList<string> options;
	options += "Edit Strength";
	options += "Edit Potion Type";
	options += "Save Ops Preview";
	options += "Back";
	while(continuing)
	{
		cout<<"Strength: "<<strength<<endl;
		cout<<"Type:     "<<getPotionTypeToString()<<endl;
		string choice = options[Menu(0,options)];
		if(choice == "Save Ops Preview")
		{
			saveOps(cout);
			cout<<endl;
		}
		else if(choice == "Edit Strength")
		{
			cout<<"Enter new strength";
			cin>>strength;
		}
		else if(choice == "Edit Potion Type")
		{
			LinkedList<string> potionOptions;
			potionOptions += "Strength";
			potionOptions += "Speed";
			potionOptions += "Defense";
			potionOptions += "Health";
			potionOptions += "Back";
			while(continuing)
			{
				cout<<"Type: "<<getPotionTypeToString()<<endl;
				choice = potionOptions[Menu(0,potionOptions)];
				if(choice == "Strength")
				{
					type = STRENGTH;
				}
				else if(choice == "Speed")
				{
					type = SPEED;
				}
				else if(choice == "Defense")
				{
					type = DEFENSE;
				}
				else if(choice == "Health")
				{
					type = HEALTH;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

enum BagType
{
	GOLD,
	FOOD
};

class Bag : public Item
{
private:
	int amountInBag;
	BagType type;
	ostream& streamOperator(ostream& os);
public:
	Bag(BagType type, int height, int width):Item(height, width), type(type), amountInBag(0)
	{
		if(type == GOLD)
		{
			this->identifier = "Bag of Gold";
		}
		else
		{
			this->identifier = "Bag of Food";
		}
		this->description = toString();
	}
	Bag(int amountInBag, BagType type, int height, int width):Item(height, width), type(type), amountInBag(amountInBag)
	{
		if(type == GOLD)
		{
			this->identifier = "Bag of Gold";
		}
		else
		{
			this->identifier = "Bag of Food";
		}
		this->description = toString();
	}
	Bag(ifstream& file):Item(file)
	{
		int intType = 0;
		file>>intType>>amountInBag;
		type = (BagType)intType;
		if(type == GOLD)
		{
			this->identifier = "Bag of Gold";
		}
		else
		{
			this->identifier = "Bag of Food";
		}
		this->description = toString();
	}
	~Bag(){}
	ostream& saveOps(ostream& os);
	int getValue();
	int getCapacity();
	int add(int amount);
	int take(int amount);
	int getAmount(){return amountInBag;}
	int getWeight();
	Item* rotate();
	string toString();
	string getBagTypeToString();
	bool isOfType(char type){return Item::isOfType(type)||type == 'B'||type == 'b';}
	char displayInInventory(){return 'B';}
	BagType getType(){return type;}
	void edit();
};

ostream& Bag::saveOps(ostream& os)
{
	Item::saveOps(os);
	os<<" "<<type<<" "<<amountInBag;
	return os;
}

int Bag::getValue()
{
	if(type == FOOD)
	{
		return amountInBag*10;
	}
	else
	{
		return amountInBag;
	}
}

int Bag::getCapacity()
{
	if(type == FOOD)
	{
		return height*width*10;
	}
	else
	{
		return height*width*100;
	}
}

int Bag::add(int amount)
{
	amountInBag += amount;
	int capacity = getCapacity();
	if(amountInBag > capacity)
	{
		int returnInt = amountInBag - capacity;
		amountInBag = capacity;
		return returnInt;
	}
	return 0;
}

int Bag::take(int amount)
{
	if(amount > amountInBag)
	{
		int returnAmount = amountInBag;
		amountInBag = 0;
		return returnAmount;
	}
	else
	{
		amountInBag -= amount;
		return amount;
	}
}

int Bag::getWeight()
{
	if(type == GOLD)
	{
		return amountInBag / 10;
	}
	else
	{
		return amountInBag;
	}
}

Item* Bag::rotate()
{
	return new Bag(type, width, height);
}

string Bag::toString()
{
	stringstream streamReturnString;
	streamOperator(streamReturnString);
	return streamReturnString.str();
}

string Bag::getBagTypeToString()
{
	string returnString;
	if(type == GOLD)
	{
		returnString = "Gold";
	}
	else
	{
		returnString = "Food";
	}
	return returnString;
}

ostream& Bag::streamOperator(ostream& os)
{
	if(type == GOLD)
	{
		os<<"Bag of gold. Amount: "<<amountInBag<<"/"<<getCapacity();
	}
	else
	{
		os<<"Bag of food. Amount: "<<amountInBag<<"/"<<getCapacity();
	}
	return os;
}

void Bag::edit()
{
	bool continuing = true;
	LinkedList<string> options;
	options += "Edit Amount in Bag";
	options += "Edit Bag Type";
	options += "Edit Dimensions";
	options += "Save Ops Preview";
	options += "Back";
	while(continuing)
	{
		cout<<"Amount in Bag: "<<amountInBag<<endl;
		cout<<"Type:     "<<getBagTypeToString()<<endl;
		string choice = options[Menu(0,options)];
		if(choice == "Save Ops Preview")
		{
			saveOps(cout);
			cout<<endl;
		}
		else if(choice == "Edit Amount in Bag")
		{
			cout<<"Enter new Amount in Bag";
			cin>>amountInBag;
		}
		else if(choice == "Edit Bag Type")
		{
			LinkedList<string> potionOptions;
			potionOptions += "Food";
			potionOptions += "Gold";
			potionOptions += "Back";
			while(continuing)
			{
				cout<<"Type: "<<getBagTypeToString()<<endl;
				choice = potionOptions[Menu(0,potionOptions)];
				if(choice == "Food")
				{
					type = FOOD;
				}
				else if(choice == "Gold")
				{
					type = GOLD;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
		}
		else if(choice == "Edit Dimensions")
		{
			Item::edit();
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

LinkedList<Item*> Item::load(string fileName)
{
	LinkedList<Item*> returnList;
	ifstream myfile;
	myfile.open(fileName);
	char input;
	while(myfile>>input)
	{
		if(input == 'I')
		{
			returnList+=new Item(myfile);
			/*int height,width,weight,specialUse;
			myfile>>height>>width>>weight>>specialUse;
			returnList+=new Item(height,width,weight,specialUse==1);*/
		}
		else if(input == 'P')
		{
			returnList+=new Potion(myfile);
			/*int height,width,weight,specialUse,type,strength;
			myfile>>height>>width>>weight>>specialUse>>type>>strength;
			returnList+=new Potion(strength,(PotionType)type, specialUse==1);*/
		}
		else if(input == 'W')
		{
			/*char* identifier, *description, *buffer;
			identifier = new char[50];
			myfile.getline(identifier, 50);
			description = "";
			buffer = new char[50];
			bool continuing = true;
			while(continuing)
			{
				myfile.getline(buffer, 50);
				if(equal(buffer,"End Description"))
				{
					continuing = false;
				}
				else
				{
					description = concatenateStrings(description, buffer);
				}
			}
			continuing = true;
			int height,width,weight,specialUse,damage;
			myfile>>height>>width>>weight>>specialUse>>damage;
			LinkedList<Move*> weaponMoves;
			myfile.getline(buffer, 50);
			if(equal(buffer,"Start Moves"))
			{
				while(continuing)
				{
					myfile.getline(buffer, 50);
					if(equal(buffer,"End Moves"))
					{
						continuing = false;
					}
					else
					{
						char *moveIdentifier, *moveDescription;
						int type, bonus, speedReduction;
						moveIdentifier = new char[50];
						moveDescription = "";
						while(continuing)
						{
							myfile.getline(buffer, 50);
							if(equal(buffer,"End Description"))
							{
								continuing = false;
							}
							else
							{
								moveDescription = concatenateStrings(moveDescription,buffer);
							}
						}
						myfile>>type>>bonus>>speedReduction;
						weaponMoves += new Move(moveIdentifier,moveDescription,(MoveType)type,bonus,speedReduction);
						continuing = true;
					}
				}
			}*/
			returnList+=new Weapon(myfile);//new Weapon(identifier,description,height,width,damage,weight,weaponMoves);
		}
		else if(input == 'B')
		{
			returnList+=new Bag(myfile);
			/*int height,width,weight,specialUse,type,amountInBag;
			myfile>>height>>width>>weight>>specialUse>>type>>amountInBag;
			returnList+=new Bag(amountInBag,(BagType)type, height, width);*/
		}
	}
	myfile.close();
	return returnList;
}

class InventorySpace
{
private:
	Item *** Inventory;
	int height;
	int width;
	bool checkItemSpace(Item* toAdd, int x, int y);
	void addItemToSpace(Item* toAdd, int x, int y);
	Item* retrieveItemFromSpace(int x, int y);
	void organize();
	void organize(LinkedList<Item*> allItems);
public:
	InventorySpace(int height, int width):height(height), width(width)
	{
		Inventory = new Item**[height];
		for(int i = 0; i < height; i++)
		{
			Inventory[i] = new Item*[width];
			for(int j = 0; j < width; j++)
			{
				Inventory[i][j] = NULL;
			}
		}
	}
	InventorySpace(ifstream& file)
	{
		file>>height>>width;
		Inventory = new Item**[height];
		for(int i = 0; i < height; i++)
		{
			Inventory[i] = new Item*[width];
			for(int j = 0; j < width; j++)
			{
				Inventory[i][j] = NULL;
			}
		}
		char input;
		while(file>>input)
		{
			if(input == 'P')
			{
				addItem(new Potion(file));
			}
			else if(input == 'W')
			{
				addItem(new Weapon(file));
			}
			else if(input == 'B')
			{
				addItem(new Bag(file));
			}
		}
	}
	~InventorySpace()
	{
		for(int i = 0; i < height; i++)
		{
			delete [] Inventory[i];
		}
		delete [] Inventory;
	}
	ostream& saveOps(ostream& os);
	int getHeight(){return height;}
	int getWidth(){return width;}
	Item* getItem();
	Item* getItem(int x, int y);
	bool addItem(Item* toAdd, int x, int y);
	bool addItem(Item* toAdd);
	void save(char* fileName);
	void load(char* fileName);
	LinkedList<Item*> retrieveAllItems(char type);
	bool containsItems(char type);
	bool resize(int newHeight, int newWidth);
	friend ostream& operator<<(ostream& os, InventorySpace* toString);
};

bool InventorySpace::checkItemSpace(Item* toAdd, int x, int y)
{
	for(int i = 0; i < toAdd->getHeight(); i++)
	{
		for(int j = 0; j < toAdd->getWidth(); j++)
		{
			if(x+i >= height||y+j >= width||Inventory[x+i][y+j] != NULL)
			{
				return false;
			}
		}
	}
	return true;
}

void InventorySpace::addItemToSpace(Item* toAdd, int x, int y)
{
	for(int i = 0; i < toAdd->getHeight(); i++)
	{
		for(int j = 0; j < toAdd->getWidth(); j++)
		{
			Inventory[x+i][y+j] = toAdd;
		}
	}
}

Item* InventorySpace::retrieveItemFromSpace(int x, int y)
{
	Item* itemToBeReturned = Inventory[x][y];
	int xMin,xMax,yMin,yMax;
	if(x-itemToBeReturned->getHeight())
	{
		xMin = 0;
	}
	else
	{
		xMin = x-itemToBeReturned->getHeight();
	}
	if(x+itemToBeReturned->getHeight())
	{
		xMax = height;
	}
	else
	{
		xMax = x+itemToBeReturned->getHeight();
	}
	if(y-itemToBeReturned->getWidth())
	{
		yMin = 0;
	}
	else
	{
		yMin = y-itemToBeReturned->getWidth();
	}
	if(y+itemToBeReturned->getWidth())
	{
		yMax = width;
	}
	else
	{
		yMax = y+itemToBeReturned->getWidth();
	}
	for(int i = xMin; i < xMax; i++)
	{
		for(int j = yMin; j < yMax; j++)
		{
			if(itemToBeReturned == Inventory[i][j])
			{
				Inventory[i][j] = NULL;
			}
		}
	}
	return itemToBeReturned;
}

void InventorySpace::organize(LinkedList<Item*> allItems)
{
	for(int i = 0; i < allItems.size()-1; i++)
	{
		for(int j = i+1; j < allItems.size(); j++)
		{
			if((allItems[i]->getHeight()*allItems[i]->getWidth())>(allItems[j]->getHeight()*allItems[j]->getWidth()))
			{
				allItems.swap(i,j);
			}
		}
	}
	for(;0 < allItems.size();)
	{
		addItem(allItems.remove(0));
	}
}

void InventorySpace::organize()
{
	organize(retrieveAllItems('I'));
}

ostream& InventorySpace::saveOps(ostream& os)
{
	os<<height<<" "<<width;
	LinkedList<Item*> allItems = retrieveAllItems('I');
	while(allItems.size()>0)
	{
		os<<endl;
		allItems[0]->saveOps(os);
		addItem(allItems.removeFirst());
	}
	return os;
}

Item* InventorySpace::getItem()
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(Inventory[i][j] != NULL)
			{
				return retrieveItemFromSpace(i, j);
			}
		}
	}
	return NULL;
}

Item* InventorySpace::getItem(int x, int y)
{
	if(x >= height)
	{
		x = height - 1;
	}
	else if(x < 0)
	{
		x = 0;
	}
	if(y >= width)
	{
		y = width - 1;
	}
	else if(y < 0)
	{
		y = 0;
	}
	if(Inventory[x][y] != NULL)
	{
		return retrieveItemFromSpace(x, y);
	}
	else
	{
		return getItem();
	}
}

bool InventorySpace::addItem(Item* toAdd, int x, int y)
{
	if(x+toAdd->getHeight() >= height)
	{
		x = height - toAdd->getHeight() - 1;
	}
	if(y+toAdd->getWidth() >= width)
	{
		y = width - toAdd->getWidth() - 1;
	}
	if(checkItemSpace(toAdd, x, y))
	{
		addItemToSpace(toAdd, x, y);
		return true;
	}
	else
	{
		return addItem(toAdd);
	}
}

bool InventorySpace::addItem(Item* toAdd)
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(Inventory[i][j] == NULL)
			{
				if(checkItemSpace(toAdd, i, j))
				{
					addItemToSpace(toAdd, i, j);
					return true;
				}
				else if(checkItemSpace(toAdd->rotate(), i, j))
				{
					addItemToSpace(toAdd->rotate(), i, j);
					return true;
				}
			}
		}
	}
	return false;
}

void InventorySpace::save(char* fileName)
{
	LinkedList<Item*> allItems = retrieveAllItems('I');
	if(allItems.size()>0)
	{
		allItems[0]->save(fileName, allItems);
		for(;0<allItems.size();)
		{
			addItem(allItems.remove(0));
		}
	}
	else
	{
		ofstream file;
		file.open(fileName);
		file.close();
	}
}

void InventorySpace::load(char* fileName)
{
	Potion potions(1,(PotionType)1);
	LinkedList<Item*> allItems = potions.load(fileName);
	for(;0 < allItems.size();)
	{
		addItem(allItems.remove(0));
	}
}

LinkedList<Item*> InventorySpace::retrieveAllItems(char type)
{
	LinkedList<Item*> allItems;
	LinkedList<Item*> itemsToReturn;
	Item * returnedItem = getItem();
	if('a' <= type&& type <= 'z')
	{
		type += 'A' - 'a';
	}
	while(returnedItem != NULL)
	{
		if(returnedItem->isOfType(type))
		{
			allItems += returnedItem;
		}
		else
		{
			itemsToReturn += returnedItem;
		}
		returnedItem = getItem();
	}
	int size = itemsToReturn.size();
	for(int i = 0; i < size; i++)
	{
		addItem(itemsToReturn.remove(0));
	}
	return allItems;
}

bool InventorySpace::containsItems(char type)
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(Inventory[i][j] != NULL)
			{
				if(Inventory[i][j]->isOfType(type))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool InventorySpace::resize(int newHeight, int newWidth)
{
	LinkedList<Item*> allItems = retrieveAllItems('I');
	if(newHeight >= height && newWidth >= width)
	{
		for(int i = 0; i < height; i++)
		{
			delete [] Inventory[i];
		}
		delete [] Inventory;
		Inventory = new Item**[newHeight];
		for(int i = 0; i < newHeight; i++)
		{
			Inventory[i] = new Item*[newWidth];
			for(int j = 0; j < newWidth; j++)
			{
				Inventory[i][j] = NULL;
			}
		}
		height = newHeight;
		width = newWidth;
		while(allItems.size() > 0)
		{
			addItem(allItems.removeFirst());
		}
		return true;
	}
	else
	{
		bool successful = true;
		for(int i = 0; i < height; i++)
		{
			delete [] Inventory[i];
		}
		delete [] Inventory;
		Inventory = new Item**[newHeight];
		for(int i = 0; i < newHeight; i++)
		{
			Inventory[i] = new Item*[newWidth];
			for(int j = 0; j < newWidth; j++)
			{
				Inventory[i][j] = NULL;
			}
		}
		for(LinkedList<Item*>::Iterator i = allItems.begin(); i; i++)
		{
			if(!addItem(*i))
			{
				successful = false;
			}
		}
		if(!successful)
		{
			for(int i = 0; i < height; i++)
			{
				delete [] Inventory[i];
			}
			delete [] Inventory;
			Inventory = new Item**[height];
			for(int i = 0; i < height; i++)
			{
				Inventory[i] = new Item*[width];
				for(int j = 0; j < width; j++)
				{
					Inventory[i][j] = NULL;
				}
			}
			while(allItems.size() > 0)
			{
				addItem(allItems.removeFirst());
			}
		}
		return successful;
	}
}

ostream& operator<<(ostream& os, InventorySpace* toString)
{
	for(int i = 0; i < toString->height; i++)
	{
		os<<"| ";
		for(int j = 0; j < toString->width; j++)
		{
			if(toString->Inventory[i][j] != NULL)
			{
				os<<toString->Inventory[i][j]->displayInInventory()<<" ";
			}
			else
			{
				os<<"  ";
			}
		}
		os<<"|\n";
	}
	LinkedList<Item*> allItems = toString->retrieveAllItems('I');
	int size = allItems.size();
	for(int i = 0; i < size; i++)
	{
		os<<(i+1)<<": "<<allItems[0]->getIdentifier()<<endl;
		toString->addItem(allItems.remove(0));
	}
	return os;
}

enum InteractableObjectType
{
	INTERACTABLE,
	UNINTERACTABLE
};

class InteractableObject
{
protected:
	string name;
	InteractableObjectType type;
	bool refresh;
	int index;
	Game* gameGraph;
	virtual ostream& streamOperator(ostream& os);
	InteractableObject(string name, InteractableObjectType type, bool refresh, int index, Game* gameGraph)
		:name(name),type(type),refresh(refresh),index(index),gameGraph(gameGraph){}
	InteractableObject(ifstream& file,Game* gameGraph):gameGraph(gameGraph)
	{
		getline(file,name);
		int intType;
		file>>index>>intType>>refresh;
		type = (InteractableObjectType)intType;
	}
public:
	virtual ~InteractableObject(){}
	static InteractableObject* getInteractableObjectFromFile(ifstream& file,Game*gameGraph);
	virtual ostream& saveOps(ostream& os);
	virtual InteractableObjectType getObjectType(){return type;}
	void setObjectType(InteractableObjectType type){this->type = type;}
	bool isRefresh(){return refresh;}
	int getIndex(){return index;}
	void setIndex(const int& index){this->index = index;}
	virtual void refreshObject(){type = INTERACTABLE;}
	virtual string getName(){return name;}
	virtual string getType(){return "InteractableObject";}
	virtual bool isOfType(string type){return type=="InteractableObject"||type=="InteractableObject";}
	virtual int interactWithPlayer(Player* thePlayer);
	static InteractableObject*selectNewInteractableObject(Game* gameGraph);
	virtual void edit();
	friend ostream& operator<<(ostream& os, InteractableObject* toString);
	friend class Location;
};

ostream& InteractableObject::streamOperator(ostream& os)
{
	os<<name;
	return os;
}

ostream& InteractableObject::saveOps(ostream& os)
{
	os<<getType()<<endl;
	os<<name<<endl;
	os<<index<<" "<<type<<" "<<refresh;
	return os;
}

void InteractableObject::edit()
{
	LinkedList<string> options;
	options += "Edit Name";
	if(!refresh)
	{
		options += "Make Refresh";
	}
	else
	{
		options += "Make Not Refresh";
	}
	if(type == INTERACTABLE)
	{
		options += "Make Uninteractable";
	}
	else if(type == UNINTERACTABLE)
	{
		options += "Make Interactable";
	}
	options += "Save Ops Preview";
	options += "Back";
	bool continuing = true;
	while(continuing)
	{
		int iChoice = Menu(0,options);
		string choice = options[iChoice];
		if(choice == "Save Ops Preview")
		{
			saveOps(cout);
			cout<<endl;
		}
		else if(choice == "Edit Name")
		{
			cout<<"Current Name: "<<name<<endl;
			cout<<"Enter new name: ";
			getline(cin, name);
			getline(cin, name);
		}
		else if(choice == "Make Refresh")
		{
			options.remove(iChoice);
			options.insert(iChoice, "Make Not Refresh");
			refresh = true;
		}
		else if(choice == "Make Not Refresh")
		{
			options.remove(iChoice);
			options.insert(iChoice, "Make Refresh");
			refresh = false;
		}
		else if(choice == "Make Interactable")
		{
			options.remove(iChoice);
			options.insert(iChoice, "Make Uninteractable");
			type = INTERACTABLE;
		}
		else if(choice == "Make Uninteractable")
		{
			options.remove(iChoice);
			options.insert(iChoice, "Make Interactable");
			type = UNINTERACTABLE;
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

ostream& operator<<(ostream& os, InteractableObject* toString)
{
	return toString->streamOperator(os);
}

class Character:public InteractableObject
{
protected:
	int strength, defense, speed, xp, level, hp, hpmax, stun;
	Bag *goldBag, *foodBag;
	Move *basicAttack, *basicDefense;
	bool defending;
	virtual bool attack(Character* enemy);
	virtual bool defend(Character* enemy);
	virtual Move* round(Character* enemy){return basicAttack;}
	virtual bool dealWithDamage(int power);
	virtual void addXP(int amount);
	virtual ostream& streamOperator(ostream& os);
	Character(string name, InteractableObjectType type, int index, bool refresh, Game* gameGraph, int strength, int defense, int speed, int xp, int hp, int gold):
		InteractableObject(name, type, refresh, index, gameGraph),strength(strength),defense(defense),speed(speed),xp(xp),hp(hp),hpmax(hp),stun(0),defending(false), basicAttack(new Move("Basic Attack","The person does a simple attack",MELEEATTACK)), basicDefense(new Move("Basic Defense","The person does a simple block",MELEEDEFENSE))
	{
		level = 1;
		while(intitialAggregate*numberOfEnemiesForLevelUp*(level*(level+1)*(2*level+1))/6 <= xp)
		{
			level++;
		}
		goldBag = new Bag(GOLD, 1, 1);
		goldBag->add(gold);
		foodBag = new Bag(FOOD, 1, 1);
		foodBag->add(10);
	}
	Character(string name, InteractableObjectType type, int index, bool refresh, Game* gameGraph, int strength, int defense, int speed, int xp, int hp, int hpmax, Bag* goldBag, Bag* foodBag):
		InteractableObject(name, type, refresh, index, gameGraph),strength(strength),defense(defense),speed(speed),xp(xp),hp(hp),hpmax(hp),stun(0),defending(false), goldBag(goldBag), foodBag(foodBag), basicAttack(new Move("Basic Attack","The person does a simple attack",MELEEATTACK)), basicDefense(new Move("Basic Defense","The person does a simple block",MELEEDEFENSE))
	{
		level = 1;
		while(intitialAggregate*numberOfEnemiesForLevelUp*(level*(level+1)*(2*level+1))/6 <= xp)
		{
			level++;
		}
	}
	Character(string name, InteractableObjectType type, int index, bool refresh, Game* gameGraph):InteractableObject(name, type, refresh, index, gameGraph),strength(20),defense(20),speed(20),xp(0),hp(100),hpmax(hp),stun(0),defending(false),basicAttack(new Move("Basic Attack","The person does a simple attack",MELEEATTACK)), basicDefense(new Move("Basic Defense","The person does a simple block",MELEEDEFENSE))
	{
		level = 1;
		goldBag = new Bag(GOLD, 1, 1);
		goldBag->add(10);
		foodBag = new Bag(FOOD, 1, 1);
		foodBag->add(10);
	}
	Character(ifstream& file,Game* gameGraph)
		:InteractableObject(file,gameGraph),defending(false),stun(0),basicAttack(new Move("Basic Attack","The person does a simple attack",MELEEATTACK)), basicDefense(new Move("Basic Defense","The person does a simple block",MELEEDEFENSE))
	{
		file>>strength>>defense>>speed>>xp>>hp>>hpmax;
		level = 1;
		while(intitialAggregate*numberOfEnemiesForLevelUp*(level*(level+1)*(2*level+1))/6 <= xp)
		{
			level++;
		}
		int aggregatePoints = intitialAggregate * level * level;
		while(aggregatePoints != getAggregatePoints())
		{
			int currentAggregatePoints = getAggregatePoints();
			strength = (strength * aggregatePoints)/currentAggregatePoints;
			defense = (defense * aggregatePoints)/currentAggregatePoints;
			speed = (speed * aggregatePoints)/currentAggregatePoints;
			hpmax = (hpmax * aggregatePoints)/currentAggregatePoints+(hpmax * aggregatePoints)%currentAggregatePoints;
			if(strength <= 0)
			{
				strength = 1;
			}
			if(defense <= 0)
			{
				defense = 1;
			}
			if(speed <= 0)
			{
				speed = 1;
			}
			if(hpmax <= 0)
			{
				hpmax = 1;
			}
			hp = hpmax;
		}
		int amount, height, width;
		file>>amount>>height>>width;
		goldBag = new Bag(amount,GOLD,height,width);
		file>>amount>>height>>width;
		foodBag = new Bag(amount,FOOD,height,width);
	}
public:
	~Character()
	{
		delete goldBag;
		delete foodBag;
		delete basicAttack;
		delete basicDefense;
	}
	virtual int getAggregatePoints(){return defense+speed+strength+hpmax;}
	virtual int getGold(){return goldBag->getAmount();}
	virtual int getFood(){return foodBag->getAmount();}
	virtual int getStrength(){return strength;}
	virtual int getDefense(){return defense;}
	virtual int getSpeed(){return speed;}
	virtual int getHP(){return hp;}
	virtual int getMaxHP(){return hpmax;}
	virtual void addStun(int amount);
	virtual void consumePotion(Item* thePotion);
	virtual bool isStunned(){return stun > 0;}
	virtual void setDefending(bool defending){this->defending = defending;}
	virtual bool isDefending(){return defending;}
	int getLevel(){return level;}
	virtual string getType(){return "Character";}
	virtual bool isOfType(string type){return InteractableObject::isOfType(type)
		||type=="Character"||type=="character";}
	virtual LinkedList<Item*> onDeath()
	{
		LinkedList<Item*> returnList;
		returnList += goldBag;
		returnList += foodBag;
		return returnList;
	}
	virtual void onWin(Character* enemy){}
	void restoreHpToMax(){hp = hpmax;}
	int interactWithPlayer(Player* thePlayer);
	virtual void battle(Character* enemy);
	virtual void takeDamage(int power);
	virtual ostream& saveOps(ostream& os);
	virtual LinkedList<Move*> getAllMoves() const;
};

void Move::execute(Character* initiator, Character* target)
{
	if(type == MELEEATTACK)
	{
		initiator->setDefending(false);
		if(target->isDefending())
		{
			initiator->addStun(target->getDefense()*2);
		}
		else
		{
			target->takeDamage((initiator->getStrength()/target->getDefense())+1+bonus);
		}
	}
	else if(type == MELEEDEFENSE)
	{
		initiator->setDefending(true);
	}
}

bool Character::attack(Character* enemy)
{
	bool returnBool = enemy->dealWithDamage(getStrength());
	if(returnBool)
	{
		return true;
	}
	else
	{
		stun+=2;
		return false;
	}
}

bool Character::defend(Character* enemy)
{
	if(defending)
	{
		return false;
	}
	else
	{
		return (defending = true);
	}
}

void Character::takeDamage(int damage)
{
	cout<<name<<" took "<<damage<<" damage!"<<endl;
	hp -= damage;
	if(hp < 0)
	{
		hp = 0;
	}
}

bool Character::dealWithDamage(int power)
{
	if(defending)
	{
		return false;
	}
	else
	{
		int damage = power/getDefense() + 1;
		hp -= damage;
		if(hp < 0)
		{
			hp = 0;
		}
		return true;
	}
}

void Character::addXP(int amount)
{
	xp += amount;
	while(xp >= (level * level * 10))
	{
		if(level < 100)
		{
			level++;
		}
	}
}

void Character::addStun(int amount)
{
	cout<<name<<" was stunned!"<<endl;
	stun += amount;
}

void Character::consumePotion(Item* thePotion)
{
	Potion* addPotion = (Potion*)thePotion;
	PotionType type = addPotion->getPotionType();
	if(type == STRENGTH)
	{
		strength += addPotion->getStrength();
	}
	else if(type == SPEED)
	{
		speed += addPotion->getStrength();
	}
	else if(type == DEFENSE)
	{
		defense += addPotion->getStrength();
	}
	else
	{
		hp += addPotion->getStrength();
	}
}

void Character::battle(Character* enemy)
{
	while(this->hp > 0 && enemy->hp > 0)
	{
		Move* currentEnemyMove,*currentMove;
		currentMove = round(enemy);
		currentEnemyMove = enemy->round(this);
		if(currentMove==NULL)
		{
			if(isStunned())
			{
				stun-=getSpeed();
				if(stun < 0)
				{
					stun = 0;
				}
				cout<<name<<" is too stunned to move!"<<endl;
				enemy->setDefending(false);
			}
			if(currentEnemyMove==NULL)
			{
				if(enemy->isStunned())
				{
					enemy->stun-=enemy->getSpeed();
					if(enemy->stun < 0)
					{
						enemy->stun = 0;
					}
					cout<<enemy->getName()<<"is too stunned to move!"<<endl;
					setDefending(false);
				}
				else
				{
					enemy->setDefending(false);
					setDefending(false);
				}
			}
			else
			{
				cout<<enemy->getName()<<" "<<currentEnemyMove->getBattleVerb()<<"!"<<endl;
				currentEnemyMove->execute(enemy,this);
			}
		}
		else if(currentEnemyMove==NULL)
		{
			cout<<getName()<<" "<<currentMove->getBattleVerb()<<"!"<<endl;
			currentMove->execute(this,enemy);
			enemy->stun-=enemy->getSpeed();
		}
		else if(getSpeed()-currentMove->getSpeedReduction()<enemy->getSpeed()-currentEnemyMove->getSpeedReduction())
		{
			cout<<enemy->getName()<<" "<<currentEnemyMove->getBattleVerb()<<"!"<<endl;
			currentEnemyMove->execute(enemy,this);
			if(hp>0)
			{
				cout<<getName()<<" "<<currentMove->getBattleVerb()<<"!"<<endl;
				currentMove->execute(this,enemy);
			}
		}
		else
		{
			cout<<getName()<<" "<<currentMove->getBattleVerb()<<"!"<<endl;
			currentMove->execute(this,enemy);
			if(enemy->getHP()>0)
			{
				cout<<enemy->getName()<<" "<<currentEnemyMove->getBattleVerb()<<"!"<<endl;
				currentEnemyMove->execute(enemy,this);
			}
		}
	}
	if(this->hp > 0)
	{
		cout<<name<<" won!"<<endl;
		if(enemy->isOfType("Enemy"))
		{
			enemy->restoreHpToMax();
			enemy->setObjectType(UNINTERACTABLE);
		}
		onWin(enemy);
	}
	else
	{
		cout<<enemy->getName()<<" won!"<<endl;
		if(isOfType("Enemy"))
		{
			restoreHpToMax();
			type = UNINTERACTABLE;
		}
		enemy->onWin(this);
	}
}

ostream& Character::streamOperator(ostream& os)
{
	os<<"============================="<<endl;
	os<<"Name------------------"<<name<<endl;
	os<<"Hit Points------------"<<hp<<"/"<<hpmax<<endl;
	os<<"Strength--------------"<<strength<<endl;
	os<<"Defense---------------"<<defense<<endl;
	os<<"Speed-----------------"<<speed<<endl;
	os<<"Level-----------------"<<level<<endl;
	os<<"Gold------------------"<<getGold()<<"/"<<goldBag->getCapacity()<<endl;
	os<<"Food------------------"<<getFood()<<"/"<<foodBag->getCapacity()<<endl;
	os<<"=============================";
	return os;
}

ostream& Character::saveOps(ostream& os)
{
	InteractableObject::saveOps(os);
	os<<" "<<strength<<" "<<defense<<" "<<speed<<" "<<xp<<" "<<hp<<" "<<hpmax<<" ";
	os<<goldBag->getAmount()<<" "<<goldBag->getHeight()<<" "<<goldBag->getWidth()<<" ";
	os<<foodBag->getAmount()<<" "<<foodBag->getHeight()<<" "<<foodBag->getWidth();
	return os;
}

LinkedList<Move*> Character::getAllMoves() const
{
	LinkedList<Move*> returnList;
	returnList+=basicAttack;
	returnList+=basicDefense;
	return returnList;
}

class Enemy:public Character
{
private:
	Item* dropItem;
	Weapon* enemyWeapon;
	Move* round(Character* enemy);
	ostream& streamOperator(ostream& os);
public:
	Enemy(string name, InteractableObjectType type, int index, bool refresh, Game* gameGraph, Item* dropItem, Weapon* enemyWeapon, int strength, int defense, int speed, int hp):
		Character(name, type, index, refresh, gameGraph, strength, defense, speed, 0, hp, 10), dropItem(dropItem), enemyWeapon(enemyWeapon){srand(time(NULL));}
	Enemy(ifstream& file, Game* gameGraph):Character(file,gameGraph)
	{
		char itemType;
		file>>itemType;
		enemyWeapon = new Weapon(file);
		dropItem = Item::getItemFromFile(file);
	}
	~Enemy()
	{
		delete dropItem;
		delete enemyWeapon;
	}
	ostream& saveOps(ostream& os);
	int getStrength();
	string getType(){return "Enemy";}
	LinkedList<Item*> onDeath();
	void onWin(Character* enemy){}
	bool isOfType(string type){return Character::isOfType(type)
		||type=="Enemy"||type=="enemy";}
	LinkedList<Move*> getAllMoves() const;
	void edit();
};

Move* Enemy::round(Character* enemy)
{
	srand(rand());
	bool enemyIsPlayer = enemy->isOfType("Player");
	bool continuing = true;
	LinkedList<Move*> enemyMoves = getAllMoves();
	while(continuing)
	{
		int possibility = rand() % enemyMoves.size();
		if(!isStunned())
		{
			if(enemy->isStunned())
			{
				return enemyMoves[0];
			}
			else if(defending)
			{
				return NULL;
			}
			else
			{
				return enemyMoves[possibility];
			}
		}
		else
		{
			return NULL;
		}
	}
}

ostream& Enemy::saveOps(ostream& os)
{
	Character::saveOps(os);
	os<<endl;
	enemyWeapon->saveOps(os);
	os<<endl;
	dropItem->saveOps(os);
	return os;
}

int Enemy::getStrength()
{
	if(enemyWeapon == NULL)
	{
		return strength;
	}
	else
	{
		return strength + enemyWeapon->getDamage();
	}
}

LinkedList<Item*> Enemy::onDeath()
{
	LinkedList<Item*> returnList = Character::onDeath();
	returnList+= dropItem;
	returnList+= enemyWeapon;
	return returnList;
}

ostream& Enemy::streamOperator(ostream& os)
{
	os<<"============================="<<endl;
	os<<"Enemy-----------------"<<endl;
	os<<"Hit Points------------"<<hp<<"/"<<hpmax<<endl;
	os<<"============================="<<endl;
	return os;
}

LinkedList<Move*> Enemy::getAllMoves() const
{
	LinkedList<Move*>returnList = Character::getAllMoves();
	returnList += enemyWeapon->getWeaponMoves();
	return returnList;
}

class Player:public Character
{
private:
	int upgradePoints, addStrength, addDefense, addSpeed, currentNumberOfMoves;
	bool gaming;
	Move* round(Character* enemy);
	void onWin(Character* enemy);
	void addXP(int amount);
	InventorySpace* inventory;
	LinkedList<Item*> keyItems;
	Weapon* rightHand;
	Weapon* leftHand;
	InteractableObject* currentInteractable;
	Move*moveSet[4];
	ostream& streamOperator(ostream& os);
public:
	Player(string name, Game* gameGraph = NULL, InteractableObject*currentInteractable = NULL, int strength = 5, int addStrength = 0,	int defense = 5, int addDefense = 0, int speed = 5,	int addSpeed = 0, int xp = 0, int hp = 10, int hpmax = 10, int upgradePoints = 0, InventorySpace* inventory = new InventorySpace(3,5), Bag* goldBag = new Bag(10, GOLD, 1, 1), Bag* foodBag = new Bag(10, FOOD, 1, 1), Weapon* rightHand = NULL, Weapon* leftHand = NULL, int currentNumberOfMoves=2)
		:Character(name,INTERACTABLE,0,false,gameGraph,strength,defense,speed,xp,hp,hpmax,goldBag,foodBag),addStrength(addStrength),addDefense(addDefense),addSpeed(addSpeed),upgradePoints(upgradePoints),rightHand(rightHand), leftHand(leftHand), currentInteractable(currentInteractable), gaming(true), inventory(inventory){}
	Player(ifstream& file, Game* gameGraph,InteractableObject* currentInteractable):Character(file, gameGraph),gaming(true),currentInteractable(currentInteractable)
	{
		file>>addStrength>>addDefense>>addSpeed>>upgradePoints;
		string buffer;
		bool continuing = true;
		rightHand = NULL;
		leftHand = NULL;
		while(continuing)
		{
			getline(file,buffer);
			if(buffer=="Right Hand")
			{
				char type;
				file>>type;
				rightHand = new Weapon(file);
				if(rightHand->getWeight() > strength/5)
				{
					leftHand = rightHand;
				}
			}
			if(buffer=="Left Hand")
			{
				leftHand = new Weapon(file);
			}
			if(buffer=="Key Items")
			{
				int size;
				file>>size;
				for(int i = 0; i < size; i++)
				{
					keyItems += Item::getItemFromFile(file);
				}
			}
			else if(buffer=="Inventory")
			{
				inventory = new InventorySpace(file);
				continuing = false;
			}
			if(!file)
			{
				continuing = false;
			}
		}
	}
	~Player()
	{
		delete inventory;
		while(keyItems.size() > 0)
		{
			delete keyItems.remove(0);
		}
		if(rightHand == leftHand)
		{
			delete rightHand;
		}
		else
		{
			delete rightHand;
			delete leftHand;
		}
		/*for(int i = 0; i < 4; i++)
		{
			delete moveSet[i];
		}*/
	}
	ostream& saveOps(ostream& os);
	LinkedList<Item*> retrieveAllItems(char itemType){return inventory->retrieveAllItems(itemType);}
	void addItem(Item* toAdd);
	int getGold();
	int getGoldCapacity();
	int addGold(int amount);
	int payGold(int amount);
	int getFood();
	int getFoodCapacity();
	int getStrength();
	void addFood(int amount);
	void eatFood(int amount);
	void consumePotion(Item* thePotion);
	int getDefense(){return addDefense + defense;}
	int getSpeed(){return addSpeed + speed;}
	string getType(){return "Player";}
	bool hasKeyItem(Item* keyItem);
	Item* removeKeyItem(Item* keyItem);
	LinkedList<Item*> onDeath();
	void equipBag();
	void equipWeapon();
	void usePotion();
	void upgradeStats();
	bool isOfType(string type){return Character::isOfType(type)
		||type=="Player"||type=="player";}
	void save(char* fileName);
	void saveInventory(char* fileName);
	void saveKeyItems(char* fileName);
	void edit();
	bool isGaming(){return gaming&&(hp>0);}
	bool pause();
	int getAggregatePoints(){return upgradePoints + Character::getAggregatePoints();}
	void setCurrentInteractable(InteractableObject* newCurrentInteractable){this->currentInteractable=newCurrentInteractable;}
	InteractableObject* getCurrentLocation(){return currentInteractable;}
	LinkedList<Move*> getAllMoves() const;
};

int Character::interactWithPlayer(Player*thePlayer)
{
	if(type == INTERACTABLE)
	{
		cout<<name<<" started to battle!"<<endl;
		battle((Character*)thePlayer);
	}
	if(thePlayer->getHP() > 0)
	{
		return InteractableObject::interactWithPlayer(thePlayer);
	}
	else
	{
		return 0;
	}
}

Move* Player::round(Character* enemy)
{
	LinkedList<string> choices;
	LinkedList<Move*> allMoves = getAllMoves();
	for(int i = 0; i < allMoves.size(); i++)
	{
		choices += allMoves[i]->getIdentifier();
	}
	bool continuing = true;
	while(continuing)
	{
		if(stun <= 0)
		{
			if(defending && !enemy->isStunned())
			{
				return NULL;
			}
			else
			{
				cout<<name<<"'s Hp: "<<hp<<endl;
				cout<<"What will "<<name<<" do?"<<endl;
				int choice = Menu(0,choices);
				if(allMoves[choice]->getType() == POTION)
				{
					usePotion();
				}
				return allMoves[choice];
			}
		}
		else
		{
			return NULL;
		}
	}
}

void Player::onWin(Character* enemy)
{
	addXP(enemy->getAggregatePoints());
	LinkedList<Item*> spoils = enemy->onDeath();
	for(int i = 0; i < spoils.size(); i++)
	{
		cout<<name<<" got a "<<spoils[i]->getIdentifier()<<endl;
		ofstream oFile;
		oFile.open("Item Copy");
		spoils[i]->saveOps(oFile);
		oFile.close();
		ifstream iFile;
		iFile.open("Item Copy");
		addItem(Item::getItemFromFile(iFile));
		iFile.close();
	}
	addDefense -= 10;
	addSpeed -= 10;
	addStrength -= 10;
	if(addDefense < 0)
	{
		addDefense = 0;
	}
	if(addSpeed < 0)
	{
		addSpeed = 0;
	}
	if(addStrength < 0)
	{
		addStrength = 0;
	}
}

void Player::addXP(int amount)
{
	xp += amount;
	while(xp >= (intitialAggregate * numberOfEnemiesForLevelUp*level*(level+1)*(2*level+1)/6))
	{
		if(level < 100)
		{
			level++;
			upgradePoints = intitialAggregate * level * level - hpmax - strength - defense - speed;
			cout<<name<<" grew to level "<<level<<"!"<<endl;
		}
	}
}

ostream& Player::saveOps(ostream& os)
{
	Character::saveOps(os);
	os<<" "<<addStrength<<" "<<addDefense<<" "<<addSpeed<<" "<<upgradePoints<<endl;
	if(rightHand != NULL)
	{
		os<<"Right Hand"<<endl;
		rightHand->saveOps(os);
		os<<endl;
	}
	else if(leftHand != NULL&&rightHand!=leftHand)
	{
		os<<"Left Hand"<<endl;
		leftHand->saveOps(os);
		os<<endl;
	}
	os<<"Key Items"<<endl;
	os<<keyItems.size()<<endl;
	for(int i = 0; i < keyItems.size(); i++)
	{
		keyItems[i]->saveOps(os);
		os<<endl;
	}
	os<<"Inventory"<<endl;
	inventory->saveOps(os);
	return os;
}

void Player::addItem(Item* toAdd)
{
	if(toAdd->isKeyItem())
	{
		keyItems.add(toAdd);
	}
	else if(!inventory->addItem(toAdd))
	{
		LinkedList<Item*> items = inventory->retrieveAllItems('I');
		items += toAdd;
		LinkedList<string> options;
		for(int i = 0; i < items.size(); i++)
		{
			options += items[i]->getIdentifier();
		}
		cout<<"Choose the Item you want to delete."<<endl;
		int choice = Menu(0, options);
		cout<<"Are you sure you want to delete "<<items[choice]->toString()<<"?\n";
		if(YesNoMenu())
		{
			items.remove(choice);
		}
		for(;0<items.size();)
		{
			addItem(items.remove(0));
		}
	}
}

int Player::getGold()
{
	int returnInt = goldBag->getAmount();
	LinkedList<Item*> allBags = inventory->retrieveAllItems('B');
	for(int i = 0; i < allBags.size();)
	{
		if(((Bag*)allBags[0])->getType() == GOLD)
		{
			returnInt += ((Bag*)allBags[0])->getAmount();
		}
		inventory->addItem(allBags.remove(0));
	}
	return returnInt;
}

int Player::getGoldCapacity()
{
	int returnInt = goldBag->getCapacity();
	LinkedList<Item*> allBags = inventory->retrieveAllItems('B');
	for(int i = 0; i < allBags.size();)
	{
		if(((Bag*)allBags[0])->getType() == GOLD)
		{
			returnInt += ((Bag*)allBags[0])->getCapacity();
		}
		inventory->addItem(allBags.remove(0));
	}
	return returnInt;
}

int Player::addGold(int amount)
{
	LinkedList<Bag*> goldBags;
	goldBags += goldBag;
	LinkedList<Item*> allBagsAsItems = inventory->retrieveAllItems('B');
	for(; 0 < allBagsAsItems.size();)
	{
		if(((Bag*)allBagsAsItems[0])->getType() == GOLD)
		{
			goldBags += (Bag*)allBagsAsItems.remove(0);
		}
		else
		{
			inventory->addItem(allBagsAsItems.remove(0));
		}
	}
	int currentAmount = amount;
	for(int i = 0; i < goldBags.size(); i++)
	{
		currentAmount = goldBags[i]->add(currentAmount);
	}
	if(currentAmount > 0)
	{
		cout<<"You couldn't fit any more gold in your bag!"<<endl;
		cout<<"You dropped "<<currentAmount<<" on the floor!"<<endl;
	}
	goldBag = goldBags.remove(0);
	for(;0<goldBags.size();)
	{
		inventory->addItem(goldBags.remove(0));
	}
	cout<<"Current amount in bag: "<<getGold()<<"/"<<getGoldCapacity()<<endl;
	if(currentAmount > 0)
	{
		return currentAmount;
	}
	return 0;
}

int Player::payGold(int amount)
{
	LinkedList<Bag*> goldBags;
	goldBags += goldBag;
	LinkedList<Item*> allBagsAsItems = inventory->retrieveAllItems('B');
	for(; 0 < allBagsAsItems.size();)
	{
		if(((Bag*)allBagsAsItems[0])->getType() == GOLD)
		{
			goldBags += (Bag*)allBagsAsItems.remove(0);
		}
		else
		{
			inventory->addItem(allBagsAsItems.remove(0));
		}
	}
	int currentAmount = 0;
	for(int i = 0; i < goldBags.size(); i++)
	{
		currentAmount += goldBags[i]->take(amount);
	}
	goldBag = goldBags.remove(0);
	for(;0<goldBags.size();)
	{
		inventory->addItem(goldBags.remove(0));
	}
	if(currentAmount > amount)
	{
		addGold(currentAmount-amount);
		return amount;
	}
	else
	{
		cout<<"Current amount in bag: "<<getGold()<<"/"<<getGoldCapacity()<<endl;
		return currentAmount;
	}
}

int Player::getFood()
{
	int returnInt = foodBag->getAmount();
	LinkedList<Item*> allBags = inventory->retrieveAllItems('B');
	for(int i = 0; i < allBags.size();)
	{
		if(((Bag*)allBags[0])->getType() == FOOD)
		{
			returnInt += ((Bag*)allBags[0])->getAmount();
		}
		inventory->addItem(allBags.remove(0));
	}
	return returnInt;
}

int Player::getFoodCapacity()
{
	int returnInt = foodBag->getCapacity();
	LinkedList<Item*> allBags = inventory->retrieveAllItems('B');
	for(int i = 0; i < allBags.size();)
	{
		if(((Bag*)allBags[0])->getType() == FOOD)
		{
			returnInt += ((Bag*)allBags[0])->getCapacity();
		}
		inventory->addItem(allBags.remove(0));
	}
	return returnInt;
}

void Player::addFood(int amount)
{
	LinkedList<Bag*> foodBags;
	foodBags += foodBag;
	LinkedList<Item*> allBagsAsItems = inventory->retrieveAllItems('B');
	for(; 0 < allBagsAsItems.size();)
	{
		if(((Bag*)allBagsAsItems[0])->getType() == FOOD)
		{
			foodBags += (Bag*)remove(0);
		}
		else
		{
			inventory->addItem(allBagsAsItems.remove(0));
		}
	}
	int currentAmount = amount;
	for(int i = 0; i < foodBags.size(); i++)
	{
		currentAmount = foodBags[i]->add(currentAmount);
	}
	if(currentAmount > 0)
	{
		cout<<"You couldn't fit any more food in your bag!"<<endl;
		cout<<"You dropped "<<currentAmount<<"lbs on the floor!"<<endl;
	}
	foodBag = foodBags.remove(0);
	for(;0<foodBags.size();)
	{
		inventory->addItem(foodBags.remove(0));
	}
	cout<<"Current amount in bag: "<<getFood()<<"/"<<getFoodCapacity()<<endl;
}

void Player::eatFood(int amount)
{
	LinkedList<Bag*> foodBags;
	foodBags += foodBag;
	LinkedList<Item*> allBagsAsItems = inventory->retrieveAllItems('B');
	for(; 0 < allBagsAsItems.size();)
	{
		if(((Bag*)allBagsAsItems[0])->getType() == GOLD)
		{
			foodBags += (Bag*)allBagsAsItems.remove(0);
		}
		else
		{
			inventory->addItem(allBagsAsItems.remove(0));
		}
	}
	int currentAmount = 0;
	for(int i = 0; i < foodBags.size(); i++)
	{
		currentAmount += foodBags[i]->take(amount);
	}
	foodBag = foodBags.remove(0);
	for(;0<foodBags.size();)
	{
		inventory->addItem(foodBags.remove(0));
	}
	if(currentAmount > amount)
	{
		addFood(currentAmount-amount);
	}
}

void Player::consumePotion(Item* thePotion)
{
	Potion* addPotion = (Potion*)thePotion;
	PotionType type = addPotion->getPotionType();
	if(type == STRENGTH)
	{
		addStrength += addPotion->getStrength();
	}
	else if(type == SPEED)
	{
		addSpeed += addPotion->getStrength();
	}
	else if(type == DEFENSE)
	{
		addDefense += addPotion->getStrength();
	}
	else
	{
		hp += addPotion->getStrength();
	}
}

int Player::getStrength()
{
	if(rightHand == NULL)
	{
		return addStrength + strength;
	}
	else
	{
		return addStrength + strength + rightHand->getDamage();
	}
}

bool Player::hasKeyItem(Item * keyItem)
{
	for(int i = 0; i < keyItems.size(); i++)
	{
		if(keyItems[i]->toString() == keyItem->toString())
		{
			return true;
		}
	}
	return false;
}

Item* Player::removeKeyItem(Item* keyItem)
{
	for(int i = 0; i < keyItems.size(); i++)
	{
		if(keyItems[i]->toString()==keyItem->toString())
		{
			return keyItems.remove(i);
		}
	}
	return NULL;
}

LinkedList<Item*> Player::onDeath()
{
	return this->inventory->retrieveAllItems('I');
}

void Player::equipBag()
{
	if(inventory->containsItems('B'))
	{
		LinkedList<Item*> bags = inventory->retrieveAllItems('B');
		LinkedList<string> options;
		options += "Keep current bags";
		for(int i = 0; i < bags.size(); i++)
		{
			options += bags[i]->toString();
		}
		bool choosing = true;
		while(choosing)
		{
			cout<<"Bag of Gold:----------"<<goldBag<<endl;
			cout<<"Bag of Food:----------"<<foodBag<<endl;
			int choice = Menu(0, options);
			if(options[choice]=="Keep current bags")
			{
				choosing = false;
			}
			else
			{
				choice--;
				Bag* bagToAdd = (Bag*)bags.remove(choice);
				if(bagToAdd->getType()==GOLD)
				{
					cout<<"Replace "<<goldBag<<endl;
					cout<<"With    "<<bagToAdd<<"?"<<endl;
					if(!YesNoMenu())
					{
						cout<<"Do you wish to stop equipping?";
						if(YesNoMenu())
						{
							choosing = false;
						}
					}
					else
					{
						Bag* temp = bagToAdd;
						bagToAdd = goldBag;
						goldBag = temp;
					}
				}
				else if(bagToAdd->getType()==FOOD)
				{
					cout<<"Replace "<<foodBag<<endl;
					cout<<"With    "<<bagToAdd<<"?"<<endl;
					if(!YesNoMenu())
					{
						cout<<"Do you wish to stop equipping?";
						if(YesNoMenu())
						{
							choosing = false;
						}
					}
					else
					{
						Bag* temp = bagToAdd;
						bagToAdd = foodBag;
						foodBag = temp;
					}
				}
				bags.insert(choice, bagToAdd);
			}
		}
		for(;0<bags.size();)
		{
			inventory->addItem(bags.remove(0));
		}
	}
	else
	{
		cout<<"You have no other bags to equip!"<<endl;
	}
}

void Player::equipWeapon()
{
	LinkedList<Item*> weapons = inventory->retrieveAllItems('W');
	LinkedList<string> options;
	if(rightHand != NULL)
	{
		options += "Right Hand: " + rightHand->getIdentifier();
	}
	if(leftHand != NULL&&leftHand != rightHand)
	{
		options += "Left Hand:  " + leftHand->getIdentifier();
	}
	for(int i = 0; i < weapons.size(); i++)
	{
		options += weapons[i]->getIdentifier();
	}
	options += "Quit";
	int offset=0;
	cout<<"Current Weapons"<<endl;
	cout<<"Right Hand--------";
	if(rightHand!=NULL)
	{
		cout<<rightHand->getIdentifier();
		offset = 1;
	}
	else
	{
		cout<<"None";
	}
	cout<<endl<<"Left Hand---------";
	if(leftHand!=NULL)
	{
		if(leftHand==rightHand)
		{
			cout<<"N/A";
		}
		else
		{
			cout<<leftHand->getIdentifier();
			offset++;
		}
	}
	else
	{
		cout<<"None";
	}
	cout<<endl;
	int choice = Menu(0,options);
	if(options[choice] != "Quit")
	{
		LinkedList<string> options2;
		int weaponsIndex = choice-offset;
		if(weaponsIndex < 0)
		{
			string optionsChoice = options.remove(choice);
			char rightOrLeftHand;
			if(rightHand!=NULL&&(offset == 1||choice == 0))
			{
				rightOrLeftHand = 'R';
				cout<<rightHand<<endl;
			}
			else
			{
				rightOrLeftHand = 'L';
				cout<<leftHand<<endl;
			}
			options2 += "Unequip";
			options2 += "Back";
			choice = Menu(0,options2);
			if(options2[choice]=="Unequip")
			{
				if(rightOrLeftHand == 'R')
				{
					if(rightHand==leftHand)
					{
						leftHand = NULL;
					}
					weapons += rightHand;
					rightHand = NULL;
				}
				else
				{
					weapons += leftHand;
					leftHand = NULL;
				}
			}
		}
		else
		{
			Weapon * choiceWeapon = (Weapon*)weapons.remove(weaponsIndex);
			cout<<choiceWeapon->getDescription()<<endl;
			options2 += "Equip";
			options2 += "Back";
			choice = Menu(0,options2);
			if(options2[choice]=="Equip")
			{
				if(choiceWeapon->getWeight() > strength/5)
				{
					if(rightHand==NULL)
					{
						rightHand = choiceWeapon;
						if(leftHand!=NULL)
						{
							weapons.insert(weaponsIndex, leftHand);
						}
						leftHand = rightHand;
					}
					else if(leftHand==NULL)
					{
						leftHand = choiceWeapon;
						weapons.insert(weaponsIndex, rightHand);
						rightHand = leftHand;
					}
					else if(rightHand==leftHand)
					{
						weapons.insert(weaponsIndex,rightHand);
						rightHand = choiceWeapon;
						leftHand = rightHand;
					}
					else
					{
						weapons.insert(weaponsIndex,leftHand);
						weapons.insert(weaponsIndex,rightHand);
						rightHand = choiceWeapon;
						leftHand = rightHand;
					}
				}
				else if(rightHand==NULL)
				{
					rightHand = choiceWeapon;
				}
				else if(leftHand == NULL)
				{
					leftHand = choiceWeapon;
				}
				else if(rightHand==leftHand)
				{
					weapons.insert(weaponsIndex,rightHand);
					rightHand = choiceWeapon;
					leftHand = NULL;
				}
				else
				{
					LinkedList<string> replaceWeapon;
					replaceWeapon += rightHand->getIdentifier();
					replaceWeapon += leftHand->getIdentifier();
					replaceWeapon += "Cancel";
					int choice3 = Menu(0,replaceWeapon);
					if(replaceWeapon[choice3]!="Cancel")
					{
						if(choice3 == 0)
						{
							weapons.insert(weaponsIndex, rightHand);
							rightHand = choiceWeapon;
						}
						else
						{
							weapons.insert(weaponsIndex, leftHand);
							leftHand = choiceWeapon;
						}
					}
					else
					{
						weapons.insert(weaponsIndex,choiceWeapon);
					}
				}
			}
			else
			{
				weapons.insert(weaponsIndex,choiceWeapon);
				options.insert(weaponsIndex+offset, choiceWeapon->getIdentifier());
			}
		}
	}
	else
	{
		for(;0<weapons.size();)
		{
			addItem(weapons.remove(0));
		}
		return;
	}
	for(;0<weapons.size();)
	{
		addItem(weapons.remove(0));
	}
	equipWeapon();
	return;
}

void Player::usePotion()
{
	LinkedList<Item*> potions = inventory->retrieveAllItems('P');
	LinkedList<string> options;
	if(potions.size()>0)
	{
		for(int i = 0; i < potions.size(); i++)
		{
			options += potions[i]->toString();
		}
		options += "Quit";
		bool choosing = true;
		while(choosing)
		{
			int choice = Menu(0,options);
			if(options[choice]=="Quit")
			{
				choosing = false;
			}
			else
			{
				cout<<"Are you sure you want to use "<<potions[choice]->toString()<<"? "<<endl;
				if(!YesNoMenu())
				{
					cout<<"Do you wish to quit using Potions? "<<endl;
					if(YesNoMenu())
					{
						choosing = false;
					}
				}
				else
				{
					Potion* addPotion = (Potion*)potions.remove(choice);
					options.remove(choice);
					PotionType type = addPotion->getPotionType();
					if(type == STRENGTH)
					{
						addStrength += addPotion->getStrength();
					}
					else if(type == SPEED)
					{
						addSpeed += addPotion->getStrength();
					}
					else if(type == DEFENSE)
					{
						addDefense += addPotion->getStrength();
					}
					else
					{
						hp += addPotion->getStrength();
					}
				}
			}
			
		}
		for(;0<potions.size();)
		{
			inventory->addItem(potions.remove(0));
		}
	}
	else
	{
		cout<<"You have no potions to use!"<<endl;
	}
}

void Player::upgradeStats()
{
	if(upgradePoints > 0)
	{
		LinkedList<string> options;
		options += "Health";
		options += "Strength";
		options += "Defense";
		options += "Speed";
		options += "Quit";
		bool continuing = true;
		while (continuing)
		{
			cout<<"You have "<<upgradePoints<<" upgrade points."<<endl;
			int choice = Menu(0,options);
			if(choice != 4)
			{
				int amount;
				cout<<"How many points do you wish to use? ";
				cin>>amount;
				if(amount > upgradePoints)
				{
					amount = upgradePoints;
				}
				upgradePoints -= amount;
				if(choice == 0)
				{
					hpmax += amount;
					hp = hpmax;
				}
				else if(choice == 1)
				{
					strength += amount;
				}
				else if(choice == 2)
				{
					defense += amount;
				}
				else
				{
					speed += amount;
				}
				cout<<"You increased your "<<options[choice]<<" by "<<amount<<"!"<<endl;
				if(upgradePoints == 0)
				{
					continuing = false;
				}
			}
			else
			{
				continuing = false;
			}
		}
	}
	else
	{
		cout<<"There are no upgrade points to use! You need to level up!"<<endl;
	}
}

ostream& Player::streamOperator(ostream& os)
{
	os<<"============================="<<endl;
	os<<"Name------------------"<<name<<endl;
	os<<"Hit Points------------"<<hp<<"/"<<hpmax<<endl;
	os<<"Strength--------------";
	if(strength != getStrength())
	{
		os<<"("<<getStrength()<<")"<<strength<<endl;
	}
	else
	{
		os<<strength<<endl;
	}
	os<<"Defense---------------";
	if(defense != getDefense())
	{
		os<<"("<<getDefense()<<")"<<defense<<endl;
	}
	else
	{
		os<<defense<<endl;
	}
	os<<"Speed-----------------";
	if(speed != getSpeed())
	{
		os<<"("<<getSpeed()<<")"<<speed<<endl;
	}
	else
	{
		os<<speed<<endl;
	}
	os<<"Level-----------------"<<getLevel()<<"(-"<<(intitialAggregate * numberOfEnemiesForLevelUp*level*(level+1)*(2*level+1)/6)-xp<<")"<<endl;
	os<<"Gold------------------"<<getGold()<<"/"<<getGoldCapacity()<<endl;
	os<<"Food------------------"<<getFood()<<"/"<<getFoodCapacity()<<endl;
	os<<"============================="<<endl;
	os<<"Inventory"<<endl;
	os<<inventory;
	os<<"============================="<<endl;
	os<<"Currently Equipped"<<endl;
	os<<"Bag of Gold:----------"<<goldBag->getAmount()<<"/"<<goldBag->getCapacity()<<endl;
	os<<"Bag of Food:----------"<<foodBag->getAmount()<<"/"<<foodBag->getCapacity()<<endl;
	os<<"Weapons"<<endl;
	os<<"Right Hand------------";
	if(rightHand == NULL)
	{
		os<<"None";
	}
	else
	{
		os<<rightHand->getIdentifier();
	}
	os<<endl<<"Left Hand-------------";
	if(leftHand == NULL)
	{
		os<<"None";
	}
	else
	{
		os<<leftHand->getIdentifier();
	}
	os<<endl;
	LinkedList<Move*> moves = getAllMoves();
	os<<"Moves"<<endl;
	for(int i = 0;i < moves.size(); i++)
	{
		os<<(i+1)<<": "<<moves[i]->getIdentifier()<<endl;
	}
	os<<"=============================";
	if(keyItems.size() > 0)
	{
		os<<"\nKey Items"<<endl;
		for(int i = 0; i < keyItems.size(); i++)
		{
			os<<(i+1)<<": "<<keyItems[i]->toString()<<endl;
		}
		os<<"=============================";
	}
	return os;
}

void Player::save(char* fileName)
{
	ofstream file;
	file.open(fileName);
	file<<name<<endl;
	file<<hp<<" "<<hpmax<<" "<<strength<<" "<<addStrength<<" "<<defense<<" "<<addDefense<<" "<<speed<<" ";
	file<<addSpeed<<" "<<xp<<" "<<goldBag->getAmount()<<" "<<goldBag->getHeight()<<" "<<goldBag->getWidth()<<" ";
	file<<foodBag->getAmount()<<" "<<foodBag->getHeight()<<" "<<foodBag->getWidth()<<" "<<upgradePoints<<" "<<inventory->getHeight()<<" "<<inventory->getWidth()<<endl;
	if(rightHand != NULL)
	{
		rightHand->saveOps(file);//file<<"W "<<rightHand->getDamage()<<" "<<rightHand->getHeight()<<" "<<rightHand->getWidth()<<" "<<rightHand->getWeight()<<endl;
		file<<endl;
	}
	if(leftHand != NULL && leftHand != rightHand)
	{
		leftHand->saveOps(file);
		file<<endl;
	}
	file.close();
}

void Player::saveInventory(char* fileName)
{
	inventory->save(fileName);
}

void Player::saveKeyItems(char* fileName)
{
	ofstream file;
	file.open(fileName);
	for(int i = 0; i < keyItems.size(); i++)
	{
		keyItems[i]->save(fileName, keyItems);
	}
	file.close();
}

LinkedList<Move*> Player::getAllMoves() const
{
	LinkedList<Move*> returnList = Character::getAllMoves();
	//returnList += basicAttack;
	//returnList += basicDefense;
	LinkedList<Move*> movesToAdd;
	if(rightHand!=NULL)
	{
		returnList += rightHand->getWeaponMoves();
	}
	if(rightHand!=leftHand&&leftHand!=NULL)
	{
		returnList += leftHand->getWeaponMoves();
	}
	if(inventory->containsItems('P'))
	{
		returnList+=new Move("Use Potion","Use a potion",POTION);
	}
	return returnList;
}

class Location:public InteractableObject
{
private:
	InteractableObject *introObject, *outroObject;
public:
	Location(string name, int index, Game*gameGraph, InteractableObject *introObject = NULL, InteractableObject *outroObject = NULL):InteractableObject(name,INTERACTABLE,false,index,gameGraph), introObject(introObject), outroObject(outroObject){}
	Location(ifstream& file, Game* gameGraph):InteractableObject(file,gameGraph)
	{
		string getInteractableType;
		introObject = NULL;
		outroObject = NULL;
		bool continuing = true;
		while(continuing)
		{
			getline(file,getInteractableType);
			if(getInteractableType == "Intro Object")
			{
				introObject = getInteractableObjectFromFile(file,NULL);
			}
			else if(getInteractableType == "Outro Object")
			{
				outroObject = getInteractableObjectFromFile(file,NULL);
			}
			else if(!file)
			{
				continuing = false;
			}
		}
	}
	~Location()
	{
		delete introObject;
		delete outroObject;
	}
	ostream& saveOps(ostream& os);
	void refreshObject();
	virtual void enterLocation(Player* playingCharacter);
	virtual void leaveLocation(Player* playingCharacter);
	string getType(){return "Location";}
	string getName(){return name;}
	bool isLocation(){return true;}
	virtual int interactWithPlayer(Player* playingCharacter);
	void saveLocation(char* fileName);
	void edit();
	bool isOfType(string type){return InteractableObject::isOfType(type)
		||type=="Location"||type=="location";}
};

ostream& Location::saveOps(ostream& os)
{
	InteractableObject::saveOps(os);
	os<<endl;
	if(introObject != NULL)
	{
		os<<"Intro Object"<<endl;
		introObject->saveOps(os);
		os<<endl;
	}
	if(outroObject != NULL)
	{
		os<<"Outro Object"<<endl;
		outroObject->saveOps(os);
		os<<endl;
	}
	return os;
}

void Location::enterLocation(Player* playingCharacter)
{
	if(introObject != NULL)
	{
		introObject->interactWithPlayer(playingCharacter);
	}
}

void Location::leaveLocation(Player* playingCharacter)
{
	if(outroObject != NULL)
	{
		outroObject->interactWithPlayer(playingCharacter);
	}
}

int Location::interactWithPlayer(Player* playingCharacter)
{
	enterLocation(playingCharacter);
	cout<<"Current Location: "<<getName()<<endl;
	int returnInt = InteractableObject::interactWithPlayer(playingCharacter);
	leaveLocation(playingCharacter);
	return returnInt;
}

void Location::saveLocation(char* fileName)
{
	ofstream locationFile;
	locationFile.open(fileName);
	if(introObject != NULL)
	{
		introObject->saveOps(locationFile);
	}
	if(introObject != NULL)
	{
		introObject->saveOps(locationFile);
	}
	locationFile.close();
}

class Game
{
protected:
	LinkedList<InteractableObject*> allInteractableObjects;
	bool locked;
	int**worldGraph;
	int currentIndex;
	Player* currentPlayer;
	InteractableObject* currentInteractable;
	virtual void loadLocations(){}
	virtual void loadPlayer(){currentPlayer = new Player("Anonymous");}
	virtual void savePlayer(){}
	virtual void saveLocations(){}
	virtual void createGraph();
	virtual void connect(int index1, int index2, int weight, bool bothWays = false);
	virtual void addInteractable(InteractableObject* const newObject);
	virtual bool navigate();
	void deleteWorldGraph();
	Game():locked(false),worldGraph(NULL),currentPlayer(NULL),currentInteractable(NULL),currentIndex(0){}
	Game(ifstream& os){}
	virtual ~Game()
	{
		delete currentPlayer;
		deleteWorldGraph();
		while(allInteractableObjects.size() > 0)
		{
			delete allInteractableObjects.remove(0);
		}
	}
public:
	virtual int size(){return allInteractableObjects.size();}
	virtual void load(){}
	virtual void save(){}
	virtual void startGame(){}
	virtual int* getArray(int index){return worldGraph[index];}
	virtual int getWeight(int index1, int index2){return worldGraph[index1][index2];}
	virtual InteractableObject* getInteractableObject(int index){return allInteractableObjects[index];}
};

int InteractableObject::interactWithPlayer(Player* thePlayer)
{
	if(gameGraph != NULL)
	{
		int* adjacentObjects = gameGraph->getArray(index);
		LinkedList<string> options;
		LinkedList<int> optionsInt;
		int returnInt = index;
		int adjacencies = 0;
		for(int i = 0; i < gameGraph->size(); i++)
		{
			if(gameGraph->getWeight(index, i) >= 0 && index != i && gameGraph->getInteractableObject(i)->getObjectType()==INTERACTABLE)
			{
				if(adjacencies <= 0)
				{
					returnInt = i;
				}
				else
				{
					if(adjacencies == 1)
					{
						options += gameGraph->getInteractableObject(returnInt)->getName();
						optionsInt += returnInt;
					}
					optionsInt += i;
					options += gameGraph->getInteractableObject(i)->getName();
				}
				adjacencies++;
			}
		}
		if(adjacencies <= 1)
		{
			if(gameGraph->getInteractableObject(returnInt)->isOfType("Location")&&isOfType("Location"))
			{
				refreshObject();
			}
			return returnInt;
		}
		else
		{
			options += "Pause";
			int answer = Menu(0,options);
			returnInt = optionsInt[answer];
			if(options[answer] == "Pause")
			{
				return -1;
			}
			else
			{
				if(gameGraph->getInteractableObject(returnInt)->isOfType("Location")&&isOfType("Location"))
				{
					refreshObject();
				}
				return returnInt;
			}
		}
	}
	else
	{
		return -1;
	}
}

bool Player::pause()
{
	LinkedList<string> options;
	options += "Back";
	options += "View Stats";
	options += "Save Game";
	options += "Quit";
	int choice;
	while (true)
	{
		if((inventory->containsItems('W')||rightHand!=NULL||leftHand!=NULL)&&!options.contains("Equip Weapon"))
		{
			options.insert(options.size()-2, "Equip Weapon");
		}
		else if(!inventory->containsItems('W')&&rightHand==NULL&&leftHand==NULL&&options.contains("Equip Weapon"))
		{
			options.removeWhere("Equip Weapon");
		}
		if(inventory->containsItems('B')&&!options.contains("Equip Bag"))
		{
			options.insert(options.size()-2, "Equip Bag");
		}
		else if(!inventory->containsItems('B')&&options.contains("Equip Bag"))
		{
			options.removeWhere("Equip Bag");
		}
		if(inventory->containsItems('P')&&!options.contains("Use Potion"))
		{
			options.insert(options.size()-2, "Use Potion");
		}
		else if(!inventory->containsItems('P')&&options.contains("Use Potion"))
		{
			options.removeWhere("Use Potion");
		}
		if(upgradePoints > 0&&!options.contains("Upgrade Stats"))
		{
			options.insert(options.size()-1, "Upgrade Stats");
		}
		else if(upgradePoints <= 0&&options.contains("Upgrade Stats"))
		{
			options.removeWhere("Upgrade Stats");
		}
		cout<<"Current Location: "<<currentInteractable->getName()<<endl;
		choice = Menu(0,options);
		string cChoice = options[choice];
		if(cChoice == "Back")
		{
			return true;
		}
		else if(cChoice == "Equip Weapon")
		{
			equipWeapon();
		}
		else if(cChoice == "Use Potion")
		{
			usePotion();
		}
		else if(cChoice == "View Stats")
		{
			cout<<this<<endl;
		}
		else if(cChoice == "Quit")
		{
			gaming = false;
			return false;
		}
		else if(cChoice == "Upgrade Stats")
		{
			upgradeStats();
		}
		else if(cChoice == "Equip Bag")
		{
			equipBag();
		}
		else if(cChoice == "Save Game")
		{
			gameGraph->save();
		}
	}
}

void Location::refreshObject()
{
	int* adjacentLocations = gameGraph->getArray(index);
	for(int i = 0; i < gameGraph->size(); i++)
	{
		if(adjacentLocations[i] >= 0 && gameGraph->getInteractableObject(i)->getObjectType() == UNINTERACTABLE && gameGraph->getInteractableObject(i)->isRefresh())
		{
			gameGraph->getInteractableObject(i)->setObjectType(INTERACTABLE);
		}
	}
}

void Game::createGraph()
{
	worldGraph = new int*[allInteractableObjects.size()];
	for(int i = 0; i < allInteractableObjects.size(); i++)
	{
		worldGraph[i] = new int[allInteractableObjects.size()];
		for(int j = 0; j < allInteractableObjects.size(); j++)
		{
			worldGraph[i][j] = -1;
		}
	}
	locked = true;
}

void Game::connect(int index1, int index2, int weight, bool bothWays)
{
	worldGraph[index1][index2] = weight;
	if(bothWays)
	{
		worldGraph[index2][index1] = weight;
	}
}

void Game::addInteractable(InteractableObject* const newObject)
{
	allInteractableObjects += newObject;
}

bool Game::navigate()
{
	while(currentPlayer->isGaming())
	{
		int next = currentInteractable->interactWithPlayer(currentPlayer);
		if(next < 0)
		{
			currentPlayer->pause();
		}
		else
		{
			currentIndex = next;
			currentPlayer->eatFood(worldGraph[currentInteractable->getIndex()][currentIndex]);
			currentInteractable = allInteractableObjects[currentIndex];
			currentPlayer->setCurrentInteractable(currentInteractable);
		}
	}
	return (currentPlayer->getHP() > 0);
}

void Game::deleteWorldGraph()
{
	for(int i = 0; i < allInteractableObjects.size(); i++)
	{
		delete [] worldGraph[i];
	}
	delete [] worldGraph;
}
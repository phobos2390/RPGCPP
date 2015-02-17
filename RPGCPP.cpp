#include "Header.h"
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

class KeyItem:public Item
{
public:
	ostream& saveOps(ostream& os);
	KeyItem(string identifier, string description):Item(1,1)
	{
		this->identifier = identifier;
		this->description = description;
	}
	KeyItem(ifstream& file):Item(1,1)
	{
		file>>identifier;
		string buffer;
		description = "";
		bool continuing = true;
		while(continuing)
		{
			getline(file,buffer);
			if(buffer == "End Description")
			{
				continuing = false;
			}
			else if(description == ""&&buffer != "")
			{
				description = buffer;
			}
			else
			{
				description += "\n" + buffer;
			}
		}
	}
	string toString(){return identifier;}
	bool isKeyItem(){return true;}
	bool operator==(Item*otherItem);
	bool isOfType(char type){return Item::isOfType(type)||type == 'k'||type == 'K';}
	char displayInInventory(){return 'K';}
	void edit();
};

ostream& KeyItem::saveOps(ostream& os)
{
	os<<"K "<<identifier<<endl;
	os<<description<<endl;
	os<<"End Description"<<endl;
	return os;
}

bool KeyItem::operator==(Item*otherItem)
{
	if(!otherItem->isOfType('K'))
	{
		return false;
	}
	else
	{
		return identifier == ((KeyItem*)otherItem)->identifier;
	}
}

void KeyItem::edit()
{
	LinkedList<string> options;
	options += "Edit Identifier";
	options += "Edit Description";
	options += "Back";
	bool continuing = true;
	while(continuing)
	{
		cout<<identifier<<endl;
		cout<<description<<endl;
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
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

Item* Item::getItemFromFile(ifstream& file)
{
	char  itemType;
	file>>itemType;
	if(itemType == 'W')
	{
		return new Weapon(file);
	}
	else if(itemType == 'P')
	{
		return new Potion(file);
	}
	else if(itemType == 'B')
	{
		return new Bag(file);
	}
	else if(itemType == 'K')
	{
		return new KeyItem(file);
	}
}

void Enemy::edit()
{
	bool continuing = true;
	LinkedList<string> playerOptions;
	playerOptions += "Edit Enemy Stats";
	playerOptions += "Edit Enemy Items";
	playerOptions += "Edit Interactable";
	playerOptions += "Back";
	while(continuing)
	{
		string choice = playerOptions[Menu(0,playerOptions)];
		if(choice == "Edit Interactable")
		{
			InteractableObject::edit();
		}
		if(choice == "Edit Enemy Stats")
		{
			LinkedList<string> statOptions;
			statOptions += "Strength";
			statOptions += "Defense";
			statOptions += "Speed";
			statOptions += "Xp";
			statOptions += "HpMax";
			statOptions += "Back";
			while(continuing)
			{
				Character::streamOperator(cout);
				cout<<endl<<"Xp--------------------"<<xp;
				cout<<endl<<"========================"<<endl;
				choice = statOptions[Menu(0,statOptions)];
				if(choice == "Strength")
				{
					cout<<"Current Strength: "<<strength<<endl;
					cout<<"Enter Strength: ";
					cin>>strength;
				}
				else if(choice == "Defense")
				{
					cout<<"Current Defense: "<<defense<<endl;
					cout<<"Enter Defense: ";
					cin>>defense;
				}
				else if(choice == "Speed")
				{
					cout<<"Current Speed: "<<speed<<endl;
					cout<<"Enter Speed: ";
					cin>>speed;
				}
				else if(choice == "Xp")
				{
					cout<<"Current xp: "<<xp<<endl;
					cout<<"Enter xp: ";
					cin>>xp;
				}
				else if(choice == "HpMax")
				{
					cout<<"Current hpmax: "<<hpmax<<endl;
					cout<<"Enter hpmax: ";
					cin>>hpmax;
					hp = hpmax;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
			continuing = true;
		}
		else if(choice == "Edit Enemy Items")
		{
			LinkedList<string> itemsOptions;
			itemsOptions += "Edit Weapon";
			itemsOptions += "Edit Drop Item";
			itemsOptions += "Back";
			while(continuing)
			{
				choice = itemsOptions[Menu(0,itemsOptions)];
				if(choice == "Edit Weapon")
				{
					enemyWeapon->edit();
				}
				else if(choice == "Edit Drop Item")
				{
					LinkedList<string> dropItemOptions;
					dropItemOptions += "Edit Item";
					dropItemOptions += "Change Item Type";
					dropItemOptions += "Back";
					while(continuing)
					{
						choice = dropItemOptions[Menu(0,dropItemOptions)];
						if(choice == "Edit Item")
						{
							dropItem->edit();
						}
						else if(choice == "Change Item Type")
						{
							LinkedList<string> typeOptions;
							typeOptions += "Potion";
							typeOptions += "Weapon";
							typeOptions += "Bag";
							typeOptions += "Key Item";
							choice = typeOptions[Menu(0,typeOptions)];
							if(dropItem->isOfType(choice[0]))
							{
								dropItem->edit();
							}
							else
							{
								delete dropItem;
								dropItem = NULL;
								if(choice == "Potion")
								{
									dropItem = new Potion(1,HEALTH);
								}
								else if(choice == "Weapon")
								{
									dropItem = new Weapon("New Identifier","New description",1,1,1);
								}
								else if(choice == "Bag")
								{
									dropItem = new Bag(0,GOLD,1,1);
								}
								else if(choice == "Key Item")
								{
									dropItem = new KeyItem("New Identifier","New Description");
								}
								dropItem->edit();
							}
						}
						else if(choice == "Back")
						{
							continuing = false;
						}
					}
					continuing = true;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
			continuing = true;
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

void Player::edit()
{
	bool continuing = true;
	LinkedList<string> playerOptions;
	playerOptions += "Edit Player Stats";
	playerOptions += "Edit Player Inventory";
	playerOptions += "Edit Key Items";
	playerOptions += "Save Ops Preview";
	playerOptions += "Back";
	while(continuing)
	{
		string choice = playerOptions[Menu(0,playerOptions)];
		if(choice == "Save Ops Preview")
		{
			saveOps(cout);
			cout<<endl;
		}
		else if(choice == "Edit Player Stats")
		{
			LinkedList<string> statOptions;
			statOptions += "Strength";
			statOptions += "Defense";
			statOptions += "Speed";
			statOptions += "xp";
			statOptions += "hpmax";
			statOptions += "Upgrade Points";
			statOptions += "Back";
			while(continuing)
			{
				Character::streamOperator(cout);
				cout<<endl;
				choice = statOptions[Menu(0,statOptions)];
				if(choice == "Strength")
				{
					cout<<"Current Strength: "<<strength<<endl;
					cout<<"Enter Strength: ";
					cin>>strength;
				}
				else if(choice == "Defense")
				{
					cout<<"Current Defense: "<<defense<<endl;
					cout<<"Enter Defense: ";
					cin>>defense;
				}
				else if(choice == "Speed")
				{
					cout<<"Current Speed: "<<speed<<endl;
					cout<<"Enter Speed: ";
					cin>>speed;
				}
				else if(choice == "xp")
				{
					cout<<"Current xp: "<<xp<<endl;
					cout<<"Enter xp: ";
					cin>>xp;
				}
				else if(choice == "hpmax")
				{
					cout<<"Current hpmax: "<<hpmax<<endl;
					cout<<"Enter hpmax: ";
					cin>>hpmax;
				}
				else if(choice == "Upgrade Points")
				{
					cout<<"Current Upgrade Points: "<<upgradePoints<<endl;
					cout<<"Enter Upgrade Points: ";
					cin>>upgradePoints;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
			continuing = true;
		}
		else if(choice == "Edit Player Inventory")
		{
			LinkedList<string> itemsOptions;
			itemsOptions += "Edit Equipped Items";
			itemsOptions += "Edit Inventory Items";
			itemsOptions += "Edit Key Items";
			itemsOptions += "Back";
			while(continuing)
			{
				choice = itemsOptions[Menu(0,itemsOptions)];
				if(choice == "Edit Equipped Items")
				{
					LinkedList<string> equippedOptions;
					equippedOptions += "Edit Equipped Weapons";
					equippedOptions += "Edit Equipped Bags";
					equippedOptions += "Back";
					while(continuing)
					{
						choice = equippedOptions[Menu(0,equippedOptions)];
						if(choice == "Edit Equipped Weapons")
						{
							LinkedList<string> weaponOptions;
							bool rightHandHolding = false, leftHandHolding = false;
							weaponOptions += "Right Hand";
							weaponOptions += "Left Hand";
							weaponOptions += "Back";
							while(continuing)
							{
								cout<<"Right Hand: ";
								if(rightHand != NULL)
								{
									rightHandHolding = true;
									cout<<rightHand->getIdentifier()<<endl;
									cout<<"Left Hand: ";
									if(leftHand != NULL)
									{
										if(leftHand != rightHand)
										{
											leftHandHolding = true;
											cout<<leftHand->getIdentifier()<<endl;
										}
										else
										{
											cout<<"N/A"<<endl;
										}
									}
								}
								else if(leftHand != NULL)
								{
									leftHandHolding = true;
									cout<<"None"<<endl;
									cout<<"Left Hand: "<<leftHand->getIdentifier();
								}
								else
								{
									cout<<"None"<<endl;
									cout<<"Left Hand: None"<<endl;
								}
								choice = weaponOptions[Menu(0,weaponOptions)];
								if(choice == "Right Hand")
								{
									if(rightHandHolding)
									{
										LinkedList<string> rightHandOptions;
										rightHandOptions += "Edit Weapon";
										rightHandOptions += "Delete Weapon";
										rightHandOptions += "Back";
										while(continuing)
										{
											choice = rightHandOptions[Menu(0,rightHandOptions)];
											if(choice == "Delete Weapon")
											{
												if(leftHand == rightHand)
												{
													leftHand = NULL;
												}
												rightHand = NULL;
												cout<<"Weapon Deleted"<<endl;
												continuing = false;
											}
											else if(choice == "Edit Weapon")
											{
												while(continuing)
												{
													rightHand->edit();
													if(rightHand->getWeight()>strength/5)
													{
														if(leftHandHolding)
														{
															if(inventory->addItem(leftHand))
															{
																leftHand = rightHand;
																continuing = false;
															}
															else
															{
																cout<<"Error...weapon too heavy"<<endl;
															}
														}
													}
													else
													{
														continuing = false;
													}
												}
												continuing = true;
											}
											else if(choice == "Back")
											{
												continuing = false;
											}
										}
										continuing = true;
									}
									else
									{
										rightHand = new Weapon("New Weapon","Default Description",1,1,1,1);
										rightHand->edit();
									}
								}
								else if(choice == "Left Hand")
								{
									if(leftHandHolding)
									{
										LinkedList<string> leftHandOptions;
										leftHandOptions += "Edit Weapon";
										leftHandOptions += "Delete Weapon";
										leftHandOptions += "Back";
										while(continuing)
										{
											if(choice == "Delete Weapon")
											{
												if(rightHand == leftHand)
												{
													rightHand = NULL;
												}
												leftHand = NULL;
												cout<<"Weapon Deleted"<<endl;
												continuing = false;
											}
											if(choice == "Edit Weapon")
											{
												while(continuing)
												{
													leftHand->edit();
													if(leftHand->getWeight()>strength/5)
													{
														if(rightHandHolding)
														{
															if(inventory->addItem(rightHand))
															{
																rightHand = leftHand;
																continuing = false;
															}
															else
															{
																cout<<"Error...weapon too heavy"<<endl;
															}
														}
													}
													else
													{
														continuing = false;
													}
												}
												continuing = true;
											}
											else if(choice == "Back")
											{
												continuing = false;
											}
										}
										continuing = true;
									}
									else
									{
										leftHand = new Weapon("New Weapon","Default Description",1,1,1,1);
										leftHand->edit();
									}
								}
								else if(choice == "Back")
								{
									continuing = false;
								}
							}
							continuing = true;
						}
						else if(choice == "Edit Equipped Bags")
						{
							LinkedList<string> weaponOptions;
							weaponOptions += "Edit Food Bag";
							weaponOptions += "Edit Gold Bag";
							weaponOptions += "Back";
							while(continuing)
							{
								choice = weaponOptions[Menu(0,weaponOptions)];
								if(choice == "Edit Food Bag")
								{
									foodBag->edit();
								}
								else if(choice == "Edit Gold Bag")
								{
									goldBag->edit();
								}
								else if(choice == "Back")
								{
									continuing = false;
								}
							}
							continuing = true;
						}
						else if(choice == "Back")
						{
							continuing = false;
						}
					}
					continuing = true;
				}
				else if(choice == "Edit Inventory Items")
				{
					LinkedList<string> inventoryOptions;
					inventoryOptions += "Add New Item";
					inventoryOptions += "Edit Dimensions";
					inventoryOptions += "Edit Items";
					inventoryOptions += "Back";
					while(continuing)
					{
						choice = inventoryOptions[Menu(0,inventoryOptions)];
						if(choice == "Add New Item")
						{
							LinkedList<string> itemOptions;
							itemOptions += "Potion";
							itemOptions += "Weapon";
							itemOptions += "Bag";
							itemOptions += "Key";
							while(continuing)
							{
								choice = itemOptions[Menu(0,itemOptions)];
								Item* addItem;
								if(choice == "Potion")
								{
									addItem = new Potion(0,HEALTH);
								}
								else if(choice == "Weapon")
								{
									addItem = new Weapon("New Identifier","New Description",1,1,1);
								}
								else if(choice == "Bag")
								{
									addItem = new Bag(0,GOLD,1,1);
								}
								else if(choice == "Key")
								{
									addItem = new KeyItem("New Identifier","New Description");
								}
								addItem->edit();
								if(inventory->addItem(addItem))
								{
									continuing = false;
								}
								else
								{
									cout<<"Error... not enough room in bag"<<endl;
								}
							}
							continuing = true;
						}
						else if(choice == "Edit Dimensions")
						{
							while(continuing)
							{
								cout<<"Current Dimensions: "<<inventory->getHeight()<<"x"<<inventory->getWidth()<<endl;
								cout<<"Enter new height: ";
								int newHeight, newWidth;
								cin>>newHeight;
								cout<<"Enter new width: ";
								cin>>newWidth;
								continuing = !inventory->resize(newHeight,newWidth);
								if(continuing)
								{
									cout<<"New dimensions did not work"<<endl;
								}
							}
							continuing = true;
						}
						else if(choice == "Edit Items")
						{
							LinkedList<string> editOptions;
							LinkedList<Item*> allItems = retrieveAllItems('I');
							for(LinkedList<Item*>::Iterator i = allItems.begin(); i; i++)
							{
								editOptions += (*i)->getIdentifier();
							}
							editOptions += "Back";
							while(continuing)
							{
								int iChoice = Menu(0,editOptions);
								choice = editOptions[iChoice];
								if(choice == "Back")
								{
									continuing = false;
								}
								else
								{
									LinkedList<string> editItemOptions;
									editItemOptions += "Edit Item";
									editItemOptions += "Delete Item";
									editItemOptions += "Back";
									while(continuing)
									{
										choice = editItemOptions[Menu(0,editItemOptions)];
										if(choice == "Edit Item")
										{
											allItems[iChoice]->edit();
										}
										else if(choice == "Delete Item")
										{
											allItems.remove(iChoice);
											editOptions.remove(iChoice);
											cout<<"Item Deleted"<<endl;
										}
										else if(choice == "Back")
										{
											continuing = false;
										}
										
									}
									continuing = false;
								}
							}

							continuing = true;
						}
						else if(choice == "Back")
						{
							continuing = false;
						}
					}
					continuing = true;
				}
				else if(choice == "Edit Key Items")
				{
					LinkedList<string> itemsOptions;
					itemsOptions += "New Key Item";
					itemsOptions += "Edit Key Items";
					itemsOptions += "Back";
					while(continuing)
					{
						choice = itemsOptions[Menu(0,itemsOptions)];
						if(choice == "New Key Item")
						{
							KeyItem* newKeyItem = new KeyItem("New Identifier","New Description");
							newKeyItem->edit();
							keyItems += newKeyItem;
						}
						else if(choice == "Edit Key Items")
						{
							LinkedList<string> keyOptions;
							for(LinkedList<Item*>::Iterator i = keyItems.begin(); i; i++)
							{
								keyOptions += (*i)->getIdentifier();
							}
							keyOptions += "Back";
							while(continuing)
							{
								int iChoice = Menu(0,keyOptions);
								choice = keyOptions[iChoice];
								if(choice == "Back")
								{
									continuing = false;
								}
								else
								{
									LinkedList<string> keyItemOptions;
									keyItemOptions += "Delete Key Item";
									keyItemOptions += "Edit Key Item";
									keyItemOptions += "Back";
									while(continuing)
									{
										choice = keyItemOptions[Menu(0,keyItemOptions)];
										if(choice == "Delete Key Item")
										{
											cout<<"Item Deleted"<<endl;
											keyItems.remove(iChoice);
											keyOptions.remove(iChoice);
										}
										else if(choice == "Edit Key Item")
										{
											keyItems[iChoice]->edit();
										}
										else if(choice == "Back")
										{
											continuing = false;
										}
									}
									continuing = true;
								}
							}
							continuing = true;
						}
						else if(choice == "Back")
						{
							continuing = false;
						}
					}
					continuing = true;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
			continuing = true;
		}
		else if(choice == "Edit Key Items")
		{
			LinkedList<string> itemsOptions;
			itemsOptions += "New Key Item";
			itemsOptions += "Edit Key Items";
			itemsOptions += "Back";
			while(continuing)
			{
				choice = itemsOptions[Menu(0,itemsOptions)];
				if(choice == "New Key Item")
				{
					KeyItem* newKeyItem = new KeyItem("New Identifier","New Description");
					newKeyItem->edit();
					keyItems += newKeyItem;
				}
				else if(choice == "Edit Key Items")
				{
					LinkedList<string> keyOptions;
					for(LinkedList<Item*>::Iterator i = keyItems.begin(); i; i++)
					{
						keyOptions += (*i)->getIdentifier();
					}
					keyOptions += "Back";
					while(continuing)
					{
						int iChoice = Menu(0,keyOptions);
						choice = keyOptions[iChoice];
						if(choice == "Back")
						{
							continuing = false;
						}
						else
						{
							LinkedList<string> keyItemOptions;
							keyItemOptions += "Delete Key Item";
							keyItemOptions += "Edit Key Item";
							keyItemOptions += "Back";
							while(continuing)
							{
								choice = keyItemOptions[Menu(0,keyItemOptions)];
								if(choice == "Delete Key Item")
								{
									cout<<"Item Deleted"<<endl;
									keyItems.remove(iChoice);
									keyOptions.remove(iChoice);
								}
								else if(choice == "Edit Key Item")
								{
									keyItems[iChoice]->edit();
								}
								else if(choice == "Back")
								{
									continuing = false;
								}
							}
							continuing = true;
						}
					}
					continuing = true;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
			continuing = true;
		}		
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

class Banker:public InteractableObject
{
private:
	int storedGold;
public:
	Banker(int index, Game* gameGraph):InteractableObject("Banker",INTERACTABLE,false,index,gameGraph),storedGold(0){}
	Banker(int index, Game* gameGraph, int storedGold):InteractableObject("Banker",INTERACTABLE,false,index,gameGraph),storedGold(storedGold){}
	Banker(ifstream& file,Game*gameGraph):InteractableObject(file,gameGraph)
	{
		file>>storedGold;
	}
	ostream& saveOps(ostream& os);
	int getStoredGold(){return storedGold;}
	int interactWithPlayer(Player* thePlayer);
	string getType(){return "Banker";}
	void refreshObject();
};

ostream& Banker::saveOps(ostream& os)
{
	InteractableObject::saveOps(os);
	os<<endl<<storedGold;
	return os;
}

int Banker::interactWithPlayer(Player* thePlayer)
{
	LinkedList<string> options;
	options += "Deposit";
	options += "Withdraw";
	options += "Exit Bank";
	cout<<"Welcome to my bank. How may I help you today?"<<endl;
	cout<<"You currently have "<<storedGold<<" gold stored."<<endl;
	cout<<"You currently have "<<thePlayer->getGold()<<"/"<<thePlayer->getGoldCapacity()<<" in your wallet."<<endl;
	bool continuing = true;
	while(continuing)
	{
		int choice = Menu(0,options);
		if(choice == 0)
		{
			cout<<"You currently have "<<thePlayer->getGold()<<"/"<<thePlayer->getGoldCapacity()<<" in your wallet."<<endl;
			cout<<"How much do you wish to deposit? ";
			int amount;
			cin>>amount;
			storedGold += thePlayer->payGold(amount);
		}
		else if(choice == 1)
		{
			cout<<"You currently have "<<storedGold<<" gold stored."<<endl;
			cout<<"How much do you wish to withdraw? ";
			int amount;
			cin>>amount;
			if(amount > storedGold)
			{
				amount = storedGold;
			}
			int excess = thePlayer->addGold(amount);
			if(excess > 0)
			{
				storedGold += excess;
			}
		}
		else
		{
			cout<<"Thank you for your time!"<<endl;
			continuing = false;
		}
	}
	return InteractableObject::interactWithPlayer(thePlayer);
}

void Banker::refreshObject()
{
	storedGold = (int)1.025*storedGold;
}

class Buyer:public InteractableObject
{
public:
	Buyer(int index,Game*gameGraph):InteractableObject("Buyer",INTERACTABLE,false,index,gameGraph){}
	Buyer(ifstream& file,Game*gameGraph):InteractableObject(file,gameGraph){}
	int interactWithPlayer(Player* thePlayer);
	string getType(){return "Buyer";}
};

int Buyer::interactWithPlayer(Player* thePlayer)
{
	cout<<"Hello, "<<thePlayer->getName()<<" what would you like to sell me today? "<<endl;
	LinkedList<string> options;
	LinkedList<Item*> allItems = thePlayer->retrieveAllItems('I');
	int ctr = 1;
	for(int i = 0;i<allItems.size();i++)
	{
		if(allItems[i]->isOfType('B')&&(((Bag*)allItems[i])->getType() == GOLD))
		{
			thePlayer->addItem(allItems.remove(i));
			i--;
		}
		else
		{
			options += allItems[i]->toString();
		}
	}
	options += "Finished";
	int choice;
	bool continuing = true;
	while(continuing)
	{
		choice = Menu(0,options);
		if(choice == allItems.size())
		{
			continuing = false;
		}
		else
		{
			cout<<"Are you sure you want to sell "<<allItems[choice]->toString()<<"? "<<endl;
			if(!YesNoMenu())
			{
				cout<<"Do you wish to stop selling items? "<<endl;
				if(YesNoMenu())
				{
					continuing = false;
				}
			}
			else
			{
				int value = allItems.remove(choice)->getValue();
				options.remove(choice);
				thePlayer->addGold(value/2);
			}
		}	
	}
	for(;0<allItems.size();)
	{
		thePlayer->addItem(allItems.remove(0));
	}
	return InteractableObject::interactWithPlayer(thePlayer);
}

class PotionSeller:public InteractableObject
{
public:
	PotionSeller(int index,Game*gameGraph):InteractableObject("Potion Seller",INTERACTABLE,false,index,gameGraph){}
	PotionSeller(ifstream& file,Game*gameGraph):InteractableObject(file,gameGraph){}
	int interactWithPlayer(Player* thePlayer);
	string getType(){return "PotionSeller";}
};

int PotionSeller::interactWithPlayer(Player* thePlayer)
{
	LinkedList<string> options;
	options += "Health";
	options += "Strength";
	options += "Defense";
	options += "Speed";
	options += "Quit";
	bool continuing = true;
	while(continuing)
	{
		cout<<"What type of Potion do you wish to purchase?"<<endl;
		int choice = Menu(0, options);
		if(choice != 4)
		{
			cout<<"How strong do you want it? (Max of 20) ";
			int strength;
			cin>>strength;
			if(strength > 20)
			{
				strength = 20;
			}
			PotionType type;
			if(choice == 0)
			{
				type = HEALTH;
			}
			else if(choice == 1)
			{
				type = STRENGTH;
			}
			else if(choice == 2)
			{
				type = DEFENSE;
			}
			else
			{
				type = SPEED;
			}
			Potion addPotion (strength, type);
			cout<<"Are you sure you wish to purchase "<<&addPotion<<" for "<<addPotion.getValue()+5<<"?"<<endl;
			if(!YesNoMenu())
			{
				cout<<"Do you wish to stop purchasing potions? "<<endl;
				if(YesNoMenu())
				{
					continuing = false;
				}
			}
			else
			{
				int cost = addPotion.getValue()+5;
				if(cost > thePlayer->getGold())
				{
					cout<<"You do not have enough gold!"<<endl;
				}
				else
				{
					thePlayer->payGold(cost);
					thePlayer->addItem(&addPotion);
					cout<<"Do you wish to continue purchasing potions? ";
					if(!YesNoMenu())
					{
						continuing = false;
					}
				}
			}
		}
		else
		{
			continuing = false;
		}
	}
	return InteractableObject::interactWithPlayer(thePlayer);
}

class TreasureChest:public InteractableObject
{
private:
	Item* containedItem;
public:
	TreasureChest(int index,Game*gameGraph,InteractableObjectType type,Item* containedItem):InteractableObject("Treasure Chest",type,false,index,gameGraph), containedItem(containedItem){}
	TreasureChest(ifstream& file,Game*gameGraph):InteractableObject(file,gameGraph)
	{
		containedItem = Item::getItemFromFile(file);
	}
	~TreasureChest()
	{
		delete containedItem;
	}
	int interactWithPlayer(Player* thePlayer);
	char* getSaveName(){return concatenateStrings("TreasureChest",convertInttoCharA(index));}
	string getType(){return "TreasureChest";}
	ostream& saveOps(ostream& os);
	void edit();
};

ostream& TreasureChest::saveOps(ostream& os)
{
	InteractableObject::saveOps(os);
	os<<endl;
	containedItem->saveOps(os);
	return os;
}

int TreasureChest::interactWithPlayer(Player* thePlayer)
{
	cout<<name<<" got a "<<containedItem->getIdentifier()<<"!"<<endl;
	ofstream oFile;
	oFile.open("Item Copy");
	containedItem->saveOps(oFile);
	oFile.close();
	ifstream iFile;
	iFile.open("Item Copy");
	thePlayer->addItem(Item::getItemFromFile(iFile));
	iFile.close();
	type = UNINTERACTABLE;
	return InteractableObject::interactWithPlayer(thePlayer);
}

void TreasureChest::edit()
{
	bool continuing = true;
	LinkedList<string> options;
	options += "Edit Contained Item";
	options += "Edit Interactable";
	options += "Back";
	while(continuing)
	{
		string choice = options[Menu(0,options)];
		if(choice == "Edit Contained Item")
		{
			LinkedList<string> dropItemOptions;
			dropItemOptions += "Edit Item";
			dropItemOptions += "Change Item Type";
			dropItemOptions += "Back";
			while(continuing)
			{
				choice = dropItemOptions[Menu(0,dropItemOptions)];
				if(choice == "Edit Item")
				{
					containedItem->edit();
				}
				else if(choice == "Change Item Type")
				{
					LinkedList<string> typeOptions;
					typeOptions += "Potion";
					typeOptions += "Weapon";
					typeOptions += "Bag";
					typeOptions += "Key Item";
					choice = typeOptions[Menu(0,typeOptions)];
					if(containedItem->isOfType(choice[0]))
					{
						containedItem->edit();
					}
					else
					{
						delete containedItem;
						containedItem = NULL;
						if(choice == "Potion")
						{
							containedItem = new Potion(1,HEALTH);
						}
						else if(choice == "Weapon")
						{
							containedItem = new Weapon("New Identifier","New description",1,1,1);
						}
						else if(choice == "Bag")
						{
							containedItem = new Bag(0,GOLD,1,1);
						}
						else if(choice == "Key Item")
						{
							containedItem = new KeyItem("New Identifier","New Description");
						}
						containedItem->edit();
					}
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
		}
		else if(choice == "Edit Interactable")
		{
			InteractableObject::edit();
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

class ItemStorage:public InteractableObject
{
private:
	LinkedList<Item*> storedItems;
	ostream& saveOps(ostream& os);
public:
	ItemStorage(int index,Game*gameGraph):InteractableObject("Item Storage",INTERACTABLE,false,index,gameGraph){}
	ItemStorage(int index,Game*gameGraph,LinkedList<Item*> storedItems):InteractableObject("Item Storage",INTERACTABLE,false,index,gameGraph),storedItems(storedItems){}
	ItemStorage(ifstream& file,Game*gameGraph):InteractableObject(file,gameGraph)
	{
		int size;
		file>>size;
		for(int i = 0; i < size; i++)
		{
			storedItems += Item::getItemFromFile(file);
		}
	}
	~ItemStorage()
	{
		for(LinkedList<Item*>::Iterator i = storedItems.begin(); i; i++)
		{
			delete (*i);
		}
	}
	int interactWithPlayer(Player* thePlayer);
	string getType(){return "ItemStorage";}
	void edit();
};

ostream& ItemStorage::saveOps(ostream& os)
{
	InteractableObject::saveOps(os);
	os<<endl<<storedItems.size();
	for(int i = 0; i < storedItems.size(); i++)
	{
		os<<endl;
		storedItems[i]->saveOps(os);
	}
	return os;
}

int ItemStorage::interactWithPlayer(Player* thePlayer)
{
	LinkedList<string> options;
	options += "Store Items";
	options += "Retrieve Items";
	options += "Return";
	int choice;
	bool stayingAtPlace=true;
	bool continuing=true;
	while(stayingAtPlace)
	{
		cout<<"Hello, "<<thePlayer->getName()<<" what would you like do today? "<<endl;
		choice = Menu(0, options);
		if(options[choice] == "Store Items")
		{
			LinkedList<string> playerItems;
			LinkedList<Item*> allItems = thePlayer->retrieveAllItems('I');
			if(allItems.size() <= 0)
			{
				cout<<"You don't have any Items to store!"<<endl;
				continuing = false;
			}
			for(int i = 0; i < allItems.size(); i++)
			{
				playerItems += allItems[i]->getIdentifier();
			}
			playerItems += "Return";
			continuing = true;
			while(continuing)
			{
				cout<<"Which Items do you wish to store?"<<endl;
				choice = Menu(0, playerItems);
				if(playerItems[choice]=="Return")
				{
					continuing = false;
				}
				else
				{
					cout<<allItems[choice]->getDescription()<<endl;
					cout<<"Confirm?"<<endl;
					if(YesNoMenu())
					{
						storedItems += allItems.remove(choice);
						playerItems.remove(choice);
					}
				}
			}
			for(;0<allItems.size();)
			{
				thePlayer->addItem(allItems.remove(0));
			}
		}
		else if(options[choice]=="Retrieve Items")
		{
			LinkedList<string> storedItems;
			if(this->storedItems.size() <= 0)
			{
				cout<<"You don't have any Items to retrieve!"<<endl;
				continuing = false;
			}
			else
			{
				for(int i = 0; i < this->storedItems.size(); i++)
				{
					storedItems += this->storedItems[i]->getIdentifier();
				}
				storedItems += "Return";
				continuing = true;
			}
			while(continuing)
			{
				cout<<"Which Items do you wish to retrieve?"<<endl;
				choice = Menu(0, storedItems);
				if(storedItems[choice]=="Return")
				{
					continuing = false;
				}
				else
				{
					cout<<this->storedItems[choice]->getDescription()<<endl;
					cout<<"Confirm?"<<endl;
					if(YesNoMenu())
					{
						thePlayer->addItem(this->storedItems.remove(choice));
						storedItems.remove(choice);
					}
				}
			}
		}
		else
		{
			stayingAtPlace = false;
		}
	}
	return InteractableObject::interactWithPlayer(thePlayer);
}

void ItemStorage::edit()
{
	bool continuing = true;
	LinkedList<string> inventoryOptions;
	inventoryOptions += "Add New Item";
	inventoryOptions += "Edit Items";
	inventoryOptions += "Back";
	while(continuing)
	{
		string choice = inventoryOptions[Menu(0,inventoryOptions)];
		if(choice == "Add New Item")
		{
			LinkedList<string> itemOptions;
			itemOptions += "Potion";
			itemOptions += "Weapon";
			itemOptions += "Bag";
			while(continuing)
			{
				choice = itemOptions[Menu(0,itemOptions)];
				Item* addItem;
				if(choice == "Potion")
				{
					addItem = new Potion(0,HEALTH);
				}
				else if(choice == "Weapon")
				{
					addItem = new Weapon("New Identifier","New Description",1,1,1);
				}
				else if(choice == "Bag")
				{
					addItem = new Bag(0,GOLD,1,1);
				}
				addItem->edit();
				storedItems+=addItem;
			}
			continuing = true;
		}
		else if(choice == "Edit Items")
		{
			LinkedList<string> editOptions;
			for(LinkedList<Item*>::Iterator i = storedItems.begin(); i; i++)
			{
				editOptions += (*i)->getIdentifier();
			}
			editOptions += "Back";
			while(continuing)
			{
				int iChoice = Menu(0,editOptions);
				choice = editOptions[iChoice];
				if(choice == "Back")
				{
					continuing = false;
				}
				else
				{
					LinkedList<string> editItemOptions;
					editItemOptions += "Edit Item";
					editItemOptions += "Delete Item";
					editItemOptions += "Back";
					while(continuing)
					{
						choice = editItemOptions[Menu(0,editItemOptions)];
						if(choice == "Edit Item")
						{
							storedItems[iChoice]->edit();
						}
						else if(choice == "Delete Item")
						{
							storedItems.remove(iChoice);
							editOptions.remove(iChoice);
							cout<<"Item Deleted"<<endl;
						}
						else if(choice == "Back")
						{
							continuing = false;
						}
					}
					continuing = false;
				}
			}
			continuing = true;
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

class LockedDoor:public InteractableObject
{
private:
	int locationIndex;
	bool staysLocked,locked;
	KeyItem* requiredItem;
	ostream& saveOps(ostream& os);
public:
	LockedDoor(int index, Game*gameGraph, InteractableObjectType type, int locationIndex, KeyItem* requiredItem)
		:InteractableObject("Locked Door",type,false,index,gameGraph), locationIndex(locationIndex), requiredItem(requiredItem), staysLocked(false),locked(true){}
	LockedDoor(string name,int index,Game*gameGraph,InteractableObjectType type,int locationIndex, KeyItem* requiredItem, bool staysLocked = false, bool locked = true)
		:InteractableObject(name,type,false,index,gameGraph), locationIndex(locationIndex), requiredItem(requiredItem), staysLocked(staysLocked),locked(locked){}
	LockedDoor(ifstream& file,Game*gameGraph):InteractableObject(file,gameGraph)
	{
		file>>locationIndex>>staysLocked>>locked;
		requiredItem = (KeyItem*)Item::getItemFromFile(file);
	}
	~LockedDoor()
	{
		delete requiredItem;
	}
	int interactWithPlayer(Player* thePlayer);
	string getType(){return "LockedDoor";}
	bool isOfType(char* type){return InteractableObject::isOfType(type)||equal(type,"LockedDoor")||equal(type,"lockeddoor");}
	void edit();
};

ostream& LockedDoor::saveOps(ostream& os)
{
	InteractableObject::saveOps(os);
	os<<endl<<locationIndex<<" "<<staysLocked<<" "<<locked<<endl;
	requiredItem->saveOps(os);
	return os;
}

int LockedDoor::interactWithPlayer(Player* thePlayer)
{
	cout<<"Do you wish to go through the door to "<<gameGraph->getInteractableObject(locationIndex)->getName()<<"?"<<endl;
	if(YesNoMenu())
	{
		if(locked&&thePlayer->hasKeyItem(requiredItem))
		{
			if(!staysLocked)
			{
				delete thePlayer->removeKeyItem(requiredItem);
				cout<<"You used the "<<requiredItem->toString()<<" to get through the door and it broke."<<endl;
				locked = false;
			}
			return locationIndex;
		}
		else if(!locked)
		{
			return locationIndex;
		}
		else
		{
			cout<<"You need a key to get past this door."<<endl;
			return InteractableObject::interactWithPlayer(thePlayer);
		}
		return locationIndex;
	}
	else
	{
		return index;
	}
}

void LockedDoor::edit()
{
	LinkedList<string> options;
	if(refresh)
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
	if(locked)
	{
		options += "Unlock";
	}
	else
	{
		options += "Lock";
	}
	if(staysLocked)
	{
		options += "Stays Locked";
	}
	else
	{
		options += "Does Not Stay Locked";
	}
	options += "Edit Required Item";
	options += "Adjacent Location";
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
		else if(choice == "Unlock")
		{
			options.remove(iChoice);
			options.insert(iChoice, "Lock");
			locked = false;
		}
		else if(choice == "Lock")
		{
			options.remove(iChoice);
			options.insert(iChoice, "Unlock");
			locked = true;
		}
		else if(choice == "Stays Locked")
		{
			options.remove(iChoice);
			options.insert(iChoice, "Does Not Stay Locked");
			locked = false;
		}
		else if(choice == "Does Not Stay Locked")
		{
			options.remove(iChoice);
			options.insert(iChoice, "Stays Locked");
			locked = true;
		}
		else if(choice == "Adjacent Location")
		{
			int size = gameGraph->size();
			LinkedList<string> adjacentOptions;
			for(int i = 0; i < size; i++)
			{
				adjacentOptions += gameGraph->getInteractableObject(i)->getName();
			}
			cout<<"Current Adjacent Location: "<<gameGraph->getInteractableObject(locationIndex);
			locationIndex = Menu(0,adjacentOptions);
		}
		else if(choice == "Edit Required Item")
		{
			requiredItem->edit();
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

class TalkPerson:public InteractableObject
{
private:
	string talk;
	bool disappearsAfter;
	ostream& saveOps(ostream& os);
public:
	TalkPerson(string name, string talk, InteractableObjectType type, Game * gameGraph, int index, bool disappearsAfter = false):InteractableObject(name, type, false, index, gameGraph), talk(talk), disappearsAfter(disappearsAfter){}
	TalkPerson(ifstream& file,Game*gameGraph):InteractableObject(file,gameGraph)
	{
		talk = "";
		bool continuing = true;
		bool disappears;
		do
		{
			string buffer;
			getline(file,buffer);
			if(buffer == "End Talk")
			{
				file>>disappearsAfter;
				continuing = false;
			}
			else if(buffer != "")
			{
				if(talk == "")
				{
					talk = buffer;
				}
				else
				{
					talk+= "\n" + buffer;
				}
			}
		}
		while(continuing);

	}
	~TalkPerson(){}
	int interactWithPlayer(Player* thePlayer);
	string getType(){return "TalkPerson";}
	void edit();
};

ostream& TalkPerson::saveOps(ostream& os)
{
	InteractableObject::saveOps(os);
	os<<endl<<talk<<endl;
	os<<"End Talk"<<endl;
	os<<disappearsAfter;
	return os;
}

int TalkPerson::interactWithPlayer(Player* thePlayer)
{
	cout<<talk<<endl;
	if(disappearsAfter)
	{
		type = UNINTERACTABLE;
	}
	return InteractableObject::interactWithPlayer(thePlayer);
}

void TalkPerson::edit()
{
	LinkedList<string> options;
	options += "Edit Name";
	if(refresh)
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
	options += "Edit Talk";
	options += "Save Ops Preview";
	options += "Back";
	bool continuing = true;
	while(continuing)
	{
		cout<<name<<endl;
		cout<<talk<<endl;
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
			cin>>name;
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
		else if(choice == "Edit Talk")
		{
			cout<<talk<<endl;
			cout<<"New Talk?"<<endl;
			if(YesNoMenu())
			{
				cout<<"Enter New Talk. To Finish, Enter End"<<endl;
				string buffer = "";
				talk = "";
				while(continuing)
				{
					getline(cin,buffer);
					if(buffer != "End"&&buffer != "end")
					{
						if(talk == "")
						{
							talk = buffer;
						}
						else
						{
							talk += "\n" + buffer;
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
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

InteractableObject* InteractableObject::selectNewInteractableObject(Game * gameGraph)
{
	LinkedList<string> options;
	options += "Location";
	options += "Enemy";
	options += "TalkPerson";
	options += "Banker";
	options += "Buyer";
	options += "LockedDoor";
	options += "ItemStorage";
	options += "TreasureChest";
	options += "PotionSeller";
	string interactableType = options[Menu(0,options)];
	if(interactableType == "Location")
	{
		return new Location("New Name",0,gameGraph,NULL,NULL);
	}
	else if(interactableType == "Enemy")
	{
		return new Enemy("New Name",INTERACTABLE,0,true,gameGraph,new Potion(1,HEALTH),new Weapon("New Weapon","Default Description",1,1,1),1,1,1,2);
	}
	else if(interactableType == "TalkPerson")
	{
		return new TalkPerson("New Name","New Talk",INTERACTABLE,gameGraph,0);
	}
	else if(interactableType == "Banker")
	{
		return new Banker(0,gameGraph);
	}
	else if(interactableType == "Buyer")
	{
		return new Buyer(0,gameGraph);
	}
	else if(interactableType == "LockedDoor")
	{
		return new LockedDoor(0,gameGraph,INTERACTABLE,0,NULL);
	}
	else if(interactableType == "ItemStorage")
	{
		return new ItemStorage(0,gameGraph);
	}
	else if(interactableType == "TreasureChest")
	{
		return new TreasureChest(0,gameGraph,INTERACTABLE,new Potion(1,HEALTH));
	}
	else if(interactableType == "PotionSeller")
	{
		return new PotionSeller(0,gameGraph);
	}
}

void Location::edit()
{
	bool continuing = true;
	LinkedList<string> options;
	options += "Edit Intro Object";
	options += "Edit Outro Object";
	options += "Edit Interactable";
	options += "Back";
	while(continuing)
	{
		string choice = options[Menu(0,options)];
		if(choice == "Edit Intro Object"||choice == "Edit Outro Object")
		{
			if(introObject == NULL && choice == "Edit Intro Object"||outroObject == NULL && choice == "Edit Outro Object")
			{
				InteractableObject* selectType;
				LinkedList<string> options;
				options += "Enemy";
				options += "TalkPerson";
				options += "TreasureChest";
				string interactableType = options[Menu(0,options)];
				if(interactableType == "Enemy")
				{
					selectType = new Enemy("New Name",INTERACTABLE,0,true,gameGraph,NULL,NULL,1,1,1,1);
				}
				else if(interactableType == "TalkPerson")
				{
					selectType = new TalkPerson("New Name","New Talk",INTERACTABLE,gameGraph,0);
				}
				else if(interactableType == "TreasureChest")
				{
					selectType = new TreasureChest(0,gameGraph,INTERACTABLE,NULL);
				}
				if(choice == "Edit Intro Object")
				{
					introObject = selectType;
				}
				else if(choice == "Edit Outro Object")
				{
					outroObject = selectType;
				}
			}
			if(choice == "Edit Intro Object")
			{
				introObject->edit();
			}
			else if(choice == "Edit Outro Object")
			{
				outroObject->edit();
			}
		}
		else if(choice == "Edit Interactable")
		{
			InteractableObject::edit();
		}
		else if(choice == "Back")
		{
			continuing = false;
		}
	}
}

InteractableObject* InteractableObject::getInteractableObjectFromFile(ifstream& file,Game*gameGraph)
{
	string interactableType;
	getline(file,interactableType);
	if(interactableType == "Location")
	{
		return new Location(file,gameGraph);
	}
	else if(interactableType == "Enemy")
	{
		return new Enemy(file,gameGraph);
	}
	else if(interactableType == "TalkPerson")
	{
		return new TalkPerson(file,gameGraph);
	}
	else if(interactableType == "Banker")
	{
		return new Banker(file,gameGraph);
	}
	else if(interactableType == "Buyer")
	{
		return new Buyer(file,gameGraph);
	}
	else if(interactableType == "LockedDoor")
	{
		return new LockedDoor(file,gameGraph);
	}
	else if(interactableType == "ItemStorage")
	{
		return new ItemStorage(file,gameGraph);
	}
	else if(interactableType == "TreasureChest")
	{
		return new TreasureChest(file,gameGraph);
	}
	else if(interactableType == "PotionSeller")
	{
		return new PotionSeller(file,gameGraph);
	}
}

class rpgMain:public Game
{
private:
	string fileDirectory;
	bool newGame;
	void loadGraph();
	void swapTwoInteractableIndex(int index1, int index2, LinkedList<LinkedList<int>>& worldGraph);
	void loadPlayer();
	void loadLocations();
	void saveGraph();
	void savePlayer();
	void saveLocations();
public:
	rpgMain(string fileDirectory,bool newGame=true):fileDirectory(fileDirectory),newGame(newGame){}
	~rpgMain(){}
	void playGame();
	void save();
	void load();
	void edit();
};

void rpgMain::loadGraph()
{
	ifstream file;
	if(newGame)
	{
		file.open(defaultDirectory+"/Graph");
	}
	else
	{
		file.open(fileDirectory+"/Graph");
	}
	int size;
	file>>size>>currentIndex;
	worldGraph = new int*[size];
	for(int i = 0; i < size; i++)
	{
		worldGraph[i] = new int[size];
		for(int j = 0; j < size; j++)
		{
			file>>worldGraph[i][j];
		}
	}
	file.close();
}

void rpgMain::swapTwoInteractableIndex(int index1, int index2, LinkedList<LinkedList<int>>& worldGraph)
{
	worldGraph.setToFirst();
	do
	{
		worldGraph.currentData().swap(index1,index2);
	}
	while(worldGraph++);
	LinkedList<int> firstLinkedList = worldGraph[index1];
	LinkedList<int> secondLinkedList = worldGraph[index2];
	while(worldGraph[index1].size() > 0)
	{
		worldGraph[index1].removeFirst();
	}
	while(worldGraph[index2].size() > 0)
	{
		worldGraph[index2].removeFirst();
	}
	for(LinkedList<int>::Iterator i = secondLinkedList.begin(); i; i++)
	{
		worldGraph[index1] += (*i);
	}
	for(LinkedList<int>::Iterator i = firstLinkedList.begin(); i; i++)
	{
		worldGraph[index2] += (*i);
	}
}

void rpgMain::loadLocations()
{
	/*if(newGame)
	{
		int currentIndex = 0;
		LinkedList<Move*> swordMoves;
		swordMoves+=new Move("Downward Thrust","A powerful downward thrust.\n+5 power\n-5 speed",MELEEATTACK,5,5);
		swordMoves+=new Move("Backhand Uppercut","A powerful upward thrust.\n+10 power\n-10 speed",MELEEATTACK,10,10);
		allInteractableObjects += new Location("Main Hub",currentIndex++,this,new TalkPerson("Prologue", concatenateStrings("Hello, sir. Welcome ","to RPG.\nMy name is John."),INTERACTABLE,0,true),NULL);
		allInteractableObjects += new Location("Open Field",currentIndex++,this);
		allInteractableObjects += new Location("MarketPlace",currentIndex++,this);
		allInteractableObjects += new Location("Cave",currentIndex++,this);
		allInteractableObjects += new Location("Castle",currentIndex++,this);
		allInteractableObjects += new Enemy("Orc",INTERACTABLE,currentIndex++,true,this,new Potion(15, STRENGTH), new Weapon("Spear","A simple thrusting weapon\nDamage: 15\nDimensions: 3x1",3,1,2), 3, 1, 1, 1);
		allInteractableObjects += new Enemy("Goblin",INTERACTABLE,currentIndex++,true,this,new Potion(15, HEALTH), new Weapon("Dagger","A concealable weapon. Light Weight and deadly.\nDamage: 5\nDimensions: 1x1",1,1,1), 3, 1, 2, 5);
		allInteractableObjects += new Enemy("Skeleton",INTERACTABLE,currentIndex++,true,this,new Potion(15, SPEED), new Weapon("Sword","A powerful, but tricky, weapon to use.\nDamage: 30\nDimensions: 5x1",5,1,3,10,swordMoves), 10, 5, 2, 3);
		allInteractableObjects += new Banker(currentIndex++,this);
		allInteractableObjects += new Buyer(currentIndex++,this);
		allInteractableObjects += new PotionSeller(currentIndex++,this);
		allInteractableObjects += new ItemStorage(currentIndex++,this);
		allInteractableObjects += new TreasureChest(currentIndex++,this,INTERACTABLE,new KeyItem("Secret Key","A metal key found in a cave"));
		allInteractableObjects += new TreasureChest(currentIndex++,this,INTERACTABLE,new Potion(15, SPEED));
		allInteractableObjects += new TreasureChest(currentIndex++,this,INTERACTABLE,new Potion(15, HEALTH));
		allInteractableObjects += new LockedDoor("Draw Bridge",currentIndex++,this,INTERACTABLE,4,new KeyItem("Secret Key","A metal key found in a cave"));
		allInteractableObjects += new TalkPerson("Map", "\nMarketPlace\n    |\nMain Hub\n    |\nOpen Field-Cave\n    |\n*Castle\n",INTERACTABLE,this,currentIndex++,false);
		createGraph();
		connect(0,1,0,true);
		connect(0,2,0,true);
		connect(1,3,0,true);
		connect(1,15,0);
		connect(1,5,0);
		connect(1,6,0);
		connect(1,7,0);
		connect(5,0,0);
		connect(6,0,0);
		connect(7,0,0);
		connect(15,0,0);
		connect(5,3,0);
		connect(6,3,0);
		connect(7,3,0);
		connect(15,3,0);
		connect(5,6,0,true);
		connect(5,7,0,true);
		connect(5,15,0,true);
		connect(6,7,0,true);
		connect(6,15,0,true);
		connect(7,15,0,true);
		connect(4,3,0);
		connect(3,2,0);
		connect(4,16,0,true);
		connect(14,3,0,true);
		connect(13,3,0,true);
		connect(12,3,0,true);
		connect(2,11,0,true);
		connect(2,10,0,true);
		connect(2,9,0,true);
		connect(2,8,0,true);
	}*/
	/*else
	{*/
	ifstream graphFile;
	if(newGame)
	{
		graphFile.open(defaultDirectory+"/Graph");
	}
	else
	{
		graphFile.open(fileDirectory+"/Graph");
	}
	int size;
	graphFile>>size>>currentIndex;
	for(int i = 0; i < size; i++)
	{
		ifstream locationFile;
		stringstream ss;
		if(newGame)
		{
			ss<<defaultDirectory<<"/Location Files/"<<i;
		}
		else
		{
			ss<<fileDirectory<<"/Location Files/"<<i;
		}
		locationFile.open(ss.str());
		allInteractableObjects += InteractableObject::getInteractableObjectFromFile(locationFile,this);
		allInteractableObjects[i]->setIndex(i);
		locationFile.close();
	}
	graphFile.close();
	//	//while(!file.eof())
	//	//{
	//	//	char* name = new char [50];
	//	//	file.getline(name,50);
	//	//	allLocations += new Location(name);
	//	//}
	//	//file.close();
	//	//file.open(concatenateStrings(fileDirectory,"/allConnections"));
	//	//int connection1, connection2;
	//	//while(file>>connection1>>connection2)
	//	//{
	//	//	allLocations[connection1]->addAdjacentLocation(allLocations[connection2]);
	//	//	allLocations[connection2]->addAdjacentLocation(allLocations[connection1]);
	//	//	connections += new int[2];
	//	//	connections[connections.size()-1][0] = connection1;
	//	//	connections[connections.size()-1][1] = connection2;
	//	//}
	//	//file.close();
	//	//for(int i = 0; i < allLocations.size(); i++)
	//	//{
	//	//	ifstream locationFile;
	//	//	bool refreshBuffer = false;
	//	//	locationFile.open(concatenateStrings(fileDirectory,concatenateStrings("/Location Files/",allLocations[i]->getName())));
	//	//	char personType [50];
	//	//	while(locationFile.getline(personType, 50))
	//	//	{
	//	//		bool refreshPerson;
	//	//		if(equal(personType, "Enemy"))
	//	//		{
	//	//			char* const enemyName = new char[50];
	//	//			locationFile.getline(enemyName, 50);
	//	//			int hp, strength, defense, speed, xp, goldInBag, goldHeight, goldWidth, foodInBag, foodHeight, foodWidth;
	//	//			locationFile>>hp>>strength>>defense>>speed>>xp>>goldInBag>>goldHeight>>goldWidth>>foodInBag>>foodHeight>>foodWidth;
	//	//			char itemType;
	//	//			locationFile>>itemType;
	//	//			Weapon* enemyWeapon = new Weapon(locationFile);
	//	//			Item* dropItem;
	//	//			locationFile>>itemType;
	//	//			if(itemType == 'K')
	//	//			{
	//	//				char * identifier = new char[50];
	//	//				locationFile.getline(identifier, 50);
	//	//				char * description, * buffer;
	//	//				buffer = "";
	//	//				while(!equal(buffer,"End Description"))
	//	//				{
	//	//					description = concatenateStrings(concatenateStrings(description,"\n"),buffer);
	//	//					locationFile.getline(buffer,50);
	//	//				}
	//	//				dropItem = new KeyItem(identifier,description);
	//	//			}
	//	//			else if(itemType == 'P')
	//	//			{
	//	//				int type, potionStrength, height, width, weight;
	//	//				bool specialUse;
	//	//				locationFile>>height>>width>>weight>>specialUse>>type>>potionStrength;
	//	//				dropItem = new Potion(potionStrength, (PotionType) type, specialUse);
	//	//			}
	//	//			else if(itemType == 'W')
	//	//			{
	//	//				//char* identifier, *description, *buffer;
	//	//				//identifier = new char[50];
	//	//				//playerFile.getline(identifier, 50);
	//	//				//description = "";
	//	//				//buffer = new char[50];
	//	//				//bool continuing = true;
	//	//				//while(continuing)
	//	//				//{
	//	//				//	playerFile.getline(buffer, 50);
	//	//				//	if(equal(buffer,"End Description"))
	//	//				//	{
	//	//				//		continuing = false;
	//	//				//	}
	//	//				//	else
	//	//				//	{
	//	//				//		description = concatenateStrings(description, buffer);
	//	//				//	}
	//	//				//}
	//	//				//continuing = true;
	//	//				//int height,width,weight,specialUse,damage;
	//	//				//playerFile>>height>>width>>weight>>specialUse>>damage;
	//	//				//LinkedList<Move*> weaponMoves;
	//	//				//playerFile.getline(buffer, 50);
	//	//				//if(equal(buffer,"Start Moves"))
	//	//				//{
	//	//				//	while(continuing)
	//	//				//	{
	//	//				//		playerFile.getline(buffer, 50);
	//	//				//		if(equal(buffer,"End Moves"))
	//	//				//		{
	//	//				//			continuing = false;
	//	//				//		}
	//	//				//		else
	//	//				//		{
	//	//				//			char *moveIdentifier, *moveDescription;
	//	//				//			int type, bonus, speedReduction;
	//	//				//			moveIdentifier = new char[50];
	//	//				//			moveDescription = "";
	//	//				//			while(continuing)
	//	//				//			{
	//	//				//				playerFile.getline(buffer, 50);
	//	//				//				if(equal(buffer,"End Description"))
	//	//				//				{
	//	//				//					continuing = false;
	//	//				//				}
	//	//				//				else
	//	//				//				{
	//	//				//					moveDescription = concatenateStrings(moveDescription,buffer);
	//	//				//				}
	//	//				//			}
	//	//				//			playerFile>>type>>bonus>>speedReduction;
	//	//				//			weaponMoves += new Move(moveIdentifier,moveDescription,(MoveType)type,bonus,speedReduction);
	//	//				//			continuing = true;
	//	//				//		}
	//	//				//	}
	//	//				dropItem = new Weapon(locationFile);//new Weapon(identifier,description,height, width, damage, weight,weaponMoves);
	//	//			}
	//	//			else if(itemType == 'B')
	//	//			{
	//	//				int type, amountInBag,height,width;
	//	//				bool specialUse;
	//	//				locationFile>>height>>width>>specialUse>>type>>amountInBag;
	//	//				dropItem = new Bag(amountInBag, (BagType) type, height, width);
	//	//			}
	//	//			locationFile>>refreshPerson;
	//	//			allLocations[i]->addInteractableObject(new Enemy(enemyName, dropItem, enemyWeapon, strength, defense, speed, hp), refreshPerson);
	//	//		}
	//	//		else if(equal(personType,"Treasure Chest"))
	//	//		{
	//	//			char itemType;
	//	//			int height, width, weight;
	//	//			bool specialUse;
	//	//			Item* containingItem;
	//	//			locationFile>>itemType;
	//	//			if(itemType == 'K')
	//	//			{
	//	//				char * identifier = new char[50];
	//	//				locationFile.getline(identifier, 50);
	//	//				char * description, * buffer;
	//	//				buffer = "";
	//	//				while(!equal(buffer,"End Description"))
	//	//				{
	//	//					description = concatenateStrings(concatenateStrings(description,"\n"),buffer);
	//	//					locationFile.getline(buffer,50);
	//	//				}
	//	//				containingItem = new KeyItem(identifier,description);
	//	//			}
	//	//			else if(itemType == 'P')
	//	//			{
	//	//				int type, potionStrength;
	//	//				locationFile>>height>>width>>weight>>specialUse>>type>>potionStrength;
	//	//				containingItem = new Potion(potionStrength, (PotionType) type, specialUse);
	//	//			}
	//	//			else if(itemType == 'W')
	//	//			{
	//	//				//int damage;
	//	//				//locationFile>>height>>width>>damage>>specialUse>>weight;
	//	//				containingItem = new Weapon(locationFile);//new Weapon(height, width, damage, weight);
	//	//			}
	//	//			else if(itemType == 'B')
	//	//			{
	//	//				int type, amountInBag;
	//	//				locationFile>>height>>width>>specialUse>>type>>amountInBag;
	//	//				containingItem = new Bag(amountInBag, (BagType) type, height, width);
	//	//			}
	//	//			locationFile>>refreshPerson;
	//	//			allLocations[i]->addInteractableObject(new TreasureChest(containingItem), refreshPerson);
	//	//		}
	//	//		else if(equal(personType,"Item Storage"))
	//	//		{
	//	//			char itemType;
	//	//			int height, width, weight;
	//	//			bool specialUse;
	//	//			LinkedList<Item*> storedItems;
	//	//			while(locationFile>>itemType)
	//	//			{
	//	//				if(itemType == 'K')
	//	//				{
	//	//					char * identifier = new char[50];
	//	//					locationFile.getline(identifier, 50);
	//	//					char * description, * buffer;
	//	//					buffer = "";
	//	//					while(!equal(buffer,"End Description"))
	//	//					{
	//	//						description = concatenateStrings(concatenateStrings(description,"\n"),buffer);
	//	//						locationFile.getline(buffer,50);
	//	//					}
	//	//					storedItems += new KeyItem(identifier,description);
	//	//				}
	//	//				else if(itemType == 'P')
	//	//				{
	//	//					int type, potionStrength;
	//	//					locationFile>>height>>width>>weight>>specialUse>>type>>potionStrength;
	//	//					storedItems += new Potion(potionStrength, (PotionType) type, specialUse);
	//	//				}
	//	//				else if(itemType == 'W')
	//	//				{
	//	//					//int damage;
	//	//					//locationFile>>height>>width>>damage>>specialUse>>weight;
	//	//					storedItems += new Weapon(locationFile);//new Weapon(height, width, damage, weight);
	//	//				}
	//	//				else if(itemType == 'B')
	//	//				{
	//	//					int type, amountInBag;
	//	//					locationFile>>height>>width>>specialUse>>type>>amountInBag;
	//	//					storedItems += new Bag(amountInBag, (BagType) type, height, width);
	//	//				}
	//	//			}
	//	//			locationFile>>refreshPerson;
	//	//			allLocations[i]->addInteractableObject(new ItemStorage(storedItems), refreshPerson);
	//	//		}
	//	//		else if(equal(personType, "Door"))
	//	//		{
	//	//			char* const doorName = new char[50];
	//	//			locationFile.getline(doorName, 50);
	//	//			int locationIndex;
	//	//			bool twoWay, refreshPerson;
	//	//			locationFile>>locationIndex>>twoWay>>refreshPerson;
	//	//			if(twoWay)
	//	//			{
	//	//				allLocations[locationIndex]->addAdjacentLocation(allLocations[i]);
	//	//			}
	//	//			allLocations[i]->addInteractableObject(new Door(doorName, allLocations[locationIndex], locationIndex, twoWay), refreshPerson);
	//	//		}
	//	//		else if(equal(personType, "LockedDoor"))
	//	//		{
	//	//			char* const doorName = new char[50];
	//	//			locationFile.getline(doorName, 50);
	//	//			char itemType;
	//	//			locationFile>>itemType;
	//	//			char* identifier = new char[50];
	//	//			locationFile.getline(identifier, 50);
	//	//			int locationIndex;
	//	//			bool staysLocked, locked, twoWay, refreshPerson;
	//	//			locationFile>>locationIndex>>staysLocked>>locked>>twoWay>>refreshPerson;
	//	//			if(twoWay)
	//	//			{
	//	//				allLocations[locationIndex]->addAdjacentLocation(allLocations[i]);
	//	//			}
	//	//			allLocations[i]->addInteractableObject(new LockedDoor(doorName, allLocations[locationIndex], locationIndex, new KeyItem(identifier), staysLocked, twoWay, locked), refreshPerson);
	//	//		}
	//	//		else if(equal(personType, "TalkPerson"))
	//	//		{
	//	//			char* const talkName = new char[50];
	//	//			locationFile.getline(talkName, 50);
	//	//			char* talk = "";
	//	//			bool continuing = true;
	//	//			bool disappears;
	//	//			do
	//	//			{
	//	//				char* buffer = new char[50];
	//	//				locationFile.getline(buffer, 50);
	//	//				if(equal(buffer,"1")||equal(buffer,"0"))
	//	//				{
	//	//					continuing = false;
	//	//					disappears = equal(buffer,"1");
	//	//				}
	//	//				else if(talk == "")
	//	//				{
	//	//					talk = buffer;
	//	//				}
	//	//				else
	//	//				{
	//	//					talk=concatenateStrings(talk,concatenateStrings("\n", buffer));
	//	//				}
	//	//			}
	//	//			while(continuing);
	//	//			allLocations[i]->addInteractableObject(new TalkPerson(talkName,talk,disappears));
	//	//		}
	//	//		else if(equal(personType,"Banker"))
	//	//		{
	//	//			int amountInBank;
	//	//			locationFile>>amountInBank>>refreshPerson;
	//	//			allLocations[i]->addInteractableObject(new Banker(amountInBank), refreshPerson);
	//	//		}
	//	//		else if(equal(personType,"Buyer"))
	//	//		{
	//	//			locationFile>>refreshPerson;
	//	//			allLocations[i]->addInteractableObject(new Buyer, refreshPerson);
	//	//		}
	//	//		else if(equal(personType,"Potion Seller"))
	//	//		{
	//	//			locationFile>>refreshPerson;
	//	//			allLocations[i]->addInteractableObject(new PotionSeller, refreshPerson);
	//	//		}
	//	//	}
	//	//	locationFile.close();
	//	//}
	//}
currentInteractable = allInteractableObjects[currentIndex];
}

void rpgMain::loadPlayer()
{
	ifstream file;
	if(!newGame)
	{
		file.open(fileDirectory + "/Player Data");
		//char*name = new char[50];
		//char* playerFileName = concatenateStrings(fileDirectory, "/Player Stats");
		//ifstream playerFile;
		//playerFile.open(playerFileName);
		//playerFile.getline(name, 50);
		//int hp,hpmax,strength,addStrength,defense,addDefense,speed,addSpeed,xp,goldBagAmount,goldBagHeight,goldBagWidth,foodBagAmount,foodBagHeight,foodBagWidth,upgradePoints,inventoryHeight,inventoryWidth;
		//playerFile>>hp>>hpmax>>strength>>addStrength>>defense>>addDefense>>speed>>addSpeed>>xp>>goldBagAmount>>goldBagHeight>>goldBagWidth>>foodBagAmount>>foodBagHeight>>foodBagWidth>>upgradePoints>>inventoryHeight>>inventoryWidth;
		//InventorySpace* inventory = new InventorySpace(inventoryHeight, inventoryWidth);
		//inventory->load(concatenateStrings(fileDirectory, "/Inventory"));
		//Weapon* rightHand, *leftHand;
		//char rightHandWeapon;
		//playerFile>>rightHandWeapon;
		//if(rightHandWeapon=='W')
		//{
		//	/*char* identifier, *description, *buffer;
		//	identifier = new char[50];
		//	playerFile.getline(identifier, 50);
		//	description = "";
		//	buffer = new char[50];
		//	bool continuing = true;
		//	while(continuing)
		//	{
		//		playerFile.getline(buffer, 50);
		//		if(equal(buffer,"End Description"))
		//		{
		//			continuing = false;
		//		}
		//		else
		//		{
		//			description = concatenateStrings(description, buffer);
		//		}
		//	}
		//	continuing = true;
		//	int height,width,weight,specialUse,damage;
		//	playerFile>>height>>width>>weight>>specialUse>>damage;
		//	LinkedList<Move*> weaponMoves;
		//	playerFile.getline(buffer, 50);
		//	if(equal(buffer,"Start Moves"))
		//	{
		//		while(continuing)
		//		{
		//			playerFile.getline(buffer, 50);
		//			if(equal(buffer,"End Moves"))
		//			{
		//				continuing = false;
		//			}
		//			else
		//			{
		//				char *moveIdentifier, *moveDescription;
		//				int type, bonus, speedReduction;
		//				moveIdentifier = new char[50];
		//				moveDescription = "";
		//				while(continuing)
		//				{
		//					playerFile.getline(buffer, 50);
		//					if(equal(buffer,"End Description"))
		//					{
		//						continuing = false;
		//					}
		//					else
		//					{
		//						moveDescription = concatenateStrings(moveDescription,buffer);
		//					}
		//				}
		//				playerFile>>type>>bonus>>speedReduction;
		//				weaponMoves += new Move(moveIdentifier,moveDescription,(MoveType)type,bonus,speedReduction);
		//				continuing = true;
		//			}
		//		}
		//	
		//	}*/
		//	rightHand = new Weapon(playerFile);
		//	if(rightHand->getWeight()<strength/5)
		//	{
		//		char leftHandWeapon;
		//		playerFile>>leftHandWeapon;
		//		if(leftHandWeapon=='W')
		//		{
		//			leftHand = new Weapon(playerFile);
		//		}
		//		else
		//		{
		//			leftHand = NULL;
		//		}
		//	}
		//	else
		//	{
		//		leftHand = rightHand;
		//	}
		//}
		//else
		//{
		//	rightHand = leftHand = NULL;
		//}
		//playerFile.close();
		//currentPlayer = new Player(name,this,currentInteractable,strength,addStrength,defense,addDefense,speed,addSpeed,xp,hp,hpmax,upgradePoints,inventory,new Bag(goldBagAmount,GOLD,goldBagHeight,goldBagWidth),new Bag(foodBagAmount,FOOD,foodBagHeight,foodBagWidth),rightHand,leftHand);
		//playerFile.open(concatenateStrings(fileDirectory,"/KeyItems"));
		//while(playerFile>>rightHandWeapon)
		//{
		//	char * identifier = new char[50];
		//	playerFile.getline(identifier, 50);
		//	char * description, * buffer;
		//	buffer = "";
		//	while(!equal(buffer,"End Description"))
		//	{
		//		description = concatenateStrings(concatenateStrings(description,"\n"),buffer);
		//		playerFile.getline(buffer,50);
		//	}
		//	currentPlayer->addItem(new KeyItem(identifier,description));
		//}
		//playerFile.close();
	}
	else
	{
		file.open(defaultDirectory + "/Player Data");
		//currentPlayer = new Player("James",this,currentInteractable);
	}
	string playerStuffz;
	getline(file,playerStuffz);
	currentPlayer = new Player(file,this,currentInteractable); 
}

void rpgMain::saveGraph()
{
	ofstream file (fileDirectory + "/Graph");
	file<<allInteractableObjects.size()<<" "<<currentIndex<<endl;
	for(int i = 0; i < allInteractableObjects.size(); i++)
	{
		for(int j = 0; j < allInteractableObjects.size(); j++)
		{
			file<<worldGraph[i][j]<<" ";
		}
		file<<endl;
	}
	file.close();
}

void rpgMain::savePlayer()
{
	ofstream file (fileDirectory + "/Player Data");
	currentPlayer->saveOps(file);
	/*currentPlayer->save(concatenateStrings(fileDirectory, "/Player Stats"));
	currentPlayer->saveInventory(concatenateStrings(fileDirectory,"/Inventory"));
	currentPlayer->saveKeyItems(concatenateStrings(fileDirectory,"/KeyItems"));*/
	file.close();
}

void rpgMain::saveLocations()
{
	for(LinkedList<InteractableObject*>::Iterator i = allInteractableObjects.begin(); i; i++)
	{
		stringstream ss;
		ss<<fileDirectory<<"/Location Files/"<<i.getIndex();
		ofstream file (ss.str());
		(*i)->saveOps(file);
		file.close();
	}
}

void rpgMain::playGame()
{
	cout<<"Welcome to my RPG program"<<endl;
	navigate();
	if(currentPlayer->getHP()>0)
	{
		cout<<"Bye!"<<endl;
	}
	else
	{
		cout<<"You died. Sorry."<<endl;
		//cout<<"Continue?"<<endl;
		//if(YesNoMenu())
		//{
		//	/*load();
		//	playGame();
		//	save();*/
		//}
	}
}

void rpgMain::save()
{
	saveGraph();
	saveLocations();
	savePlayer();
}

void rpgMain::load()
{
	loadLocations();
	loadGraph();
	loadPlayer();
}

void rpgMain::edit()
{
	LinkedList<string> options;
	LinkedList<LinkedList<int>> gameGraph;
	if(worldGraph != NULL)
	{
		for(int i = 0; i < allInteractableObjects.size(); i++)
		{
			LinkedList<int> addLinkedList;
			for(int j = 0; j < allInteractableObjects.size(); j++)
			{
				addLinkedList += worldGraph[i][j];
			}
			gameGraph += addLinkedList;
		}
	}
	else if(allInteractableObjects.size() > 0)
	{
		for(int i = 0; i < allInteractableObjects.size(); i++)
		{
			LinkedList<int> addLinkedList;
			for(int j = 0; j < allInteractableObjects.size(); j++)
			{
				addLinkedList += -1;
			}
			gameGraph += addLinkedList;
		}
	}
	options += "Edit Player";
	options += "Edit World";
	options += "Finish Editing";
	bool continuing = true;
	while(continuing)
	{
		string choice = options[Menu(0,options)];
		if(choice == "Edit Player")
		{
			if(currentPlayer == NULL)
			{
				currentPlayer = new Player("Enter Name",this,NULL);
			}
			currentPlayer->edit();
		}
		else if(choice == "Edit World")
		{
			while(continuing)
			{
				LinkedList<string> editOptions;
				for(LinkedList<InteractableObject*>::Iterator i = allInteractableObjects.begin(); i; i++)
				{
					editOptions += (*i)->getName();
				}
				editOptions += "Show Graph";
				editOptions += "New Interactable Object";
				editOptions += "Back";
				int iChoice = Menu(0,editOptions);
				choice = editOptions[iChoice];
				if(choice == "Back")
				{
					continuing = false;
				}
				else if(choice == "New Interactable Object")
				{
					LinkedList<int> newInteractableAdjacent;
					for(int i = 0; i < allInteractableObjects.size(); i++)
					{
						newInteractableAdjacent += -1;
					}
					allInteractableObjects += InteractableObject::selectNewInteractableObject(this);
					allInteractableObjects.setToLast();
					allInteractableObjects.currentData()->edit();
					gameGraph += newInteractableAdjacent;
					for(LinkedList<LinkedList<int>>::Iterator i = gameGraph.begin(); i; i++)
					{
						(*i) += -1;
					}
				}
				else if(choice == "Show Graph")
				{
					for(LinkedList<LinkedList<int>>::Iterator i = gameGraph.begin(); i; i++)
					{
						for(LinkedList<int>::Iterator j = (*i).begin(); j; j++)
						{
							cout<<(*j)<<" ";
						}
						cout<<endl;
					}
				}
				else
				{
					LinkedList<string> interactableOptions;
					interactableOptions += "Edit Object";
					interactableOptions += "Delete Object";
					interactableOptions += "Switch Index";
					interactableOptions += "Show Graph";
					interactableOptions += "Change Adjacencies";
					interactableOptions += "Back";
					while(continuing)
					{
						choice = interactableOptions[Menu(0,interactableOptions)];
						if(choice == "Edit Object")
						{
							allInteractableObjects[iChoice]->edit();
						}
						else if(choice == "Delete Object")
						{
							allInteractableObjects.remove(iChoice);
							for(LinkedList<LinkedList<int>>::Iterator i = gameGraph.begin(); i; i++)
							{
								(*i).remove(iChoice);
							}
							gameGraph.remove(iChoice);
							editOptions.remove(iChoice);
							continuing = false;
						}
						else if(choice == "Switch Index")
						{
							LinkedList<string> indexOptions;
							for(LinkedList<InteractableObject*>::Iterator i = allInteractableObjects.begin(); i; i++)
							{
								indexOptions += (*i)->getName();
							}
							indexOptions += "Back";
							while(continuing)
							{
								int jChoice = Menu(0,indexOptions);
								choice = indexOptions[jChoice];
								if(choice == "Back")
								{
									continuing = false;
								}
								else
								{
									swapTwoInteractableIndex(iChoice,jChoice,gameGraph);
									allInteractableObjects.swap(iChoice,jChoice);
									cout<<endl<<endl;
									continuing = false;
								}
							}
						}
						else if(choice == "Change Adjacencies")
						{
							while(continuing)
							{
								LinkedList<string> adjacentOptions;
								LinkedList<InteractableObject*>::Iterator k = allInteractableObjects.begin();
								LinkedList<int>::Iterator l = gameGraph[iChoice].begin();
								for(; k; l++, k++)
								{
									if((*l) < 0)
									{
										stringstream ss;
										ss<<(*k)->getName()<<" (Not Connected)";
										adjacentOptions += ss.str();
									}
									else
									{
										stringstream ss;
										ss<<(*k)->getName()<<" ("<<(*l)<<")";
										adjacentOptions += ss.str();
									}
								}
								adjacentOptions += "Back";
								int jChoice = Menu(0,adjacentOptions);
								choice = adjacentOptions[jChoice];
								if(choice == "Back")
								{
									continuing = false;
								}
								else
								{
									cout<<allInteractableObjects[iChoice]->getName()<<endl;
									cout<<choice<<endl;
									LinkedList<string>::Iterator m = adjacentOptions.begin();
									l = gameGraph[iChoice].begin();
									for(; m; l++, m++)
									{
										if(m.getIndex() == jChoice)
										{
											cout<<"Enter weight: ";
											cin>>(*l);
										}
									}
								}
							}
							continuing = true;
						}
						else if(choice == "Show Graph")
						{
							for(LinkedList<LinkedList<int>>::Iterator i = gameGraph.begin(); i; i++)
							{
								for(LinkedList<int>::Iterator j = (*i).begin(); j; j++)
								{
									cout<<(*j)<<" ";
								}
								cout<<endl;
							}
						}
						else if(choice == "Back")
						{
							continuing = false;
						}
					}
					continuing = true;
				}
			}
			continuing = true;
		}
		else if(choice == "Finish Editing")
		{
			continuing = false;
		}
	}
	worldGraph = new int*[gameGraph.size()];
	for(LinkedList<LinkedList<int>>::Iterator i = gameGraph.begin(); i; i++)
	{
		worldGraph[i.getIndex()] = new int[(*i).size()];
		for(LinkedList<int>::Iterator j = (*i).begin(); j; j++)
		{
			worldGraph[i.getIndex()][j.getIndex()] = (*j);
		}
	}
	gameGraph.setToFirst();
	while(gameGraph.size() > 0)
	{
		while(gameGraph.currentData().size() > 0)
		{
			gameGraph.currentData().removeFirst();
		}
		gameGraph.removeFirst();
	}
}

int main()
{
	numberOfEnemiesForLevelUp = 10;
	intitialAggregate = 10;
	defaultDirectory = "Demo";
	LinkedList<string> menuOptions;
	menuOptions += "New Game";
	menuOptions += "Load Game";
	menuOptions += "Level Editor";
	menuOptions += "Quit";
	bool continuing = true;
	error.open("Error File");
	while(continuing)
	{
		string choice = menuOptions[Menu(0,menuOptions)];
		if(choice == "New Game"||choice == "Load Game")
		{
			newGame = (choice == "New Game");
			LinkedList<string> gameOptions;
			gameOptions += "Game 1";
			gameOptions += "Game 2";
			gameOptions += "Game 3";
			gameOptions += "Back";
			while(continuing)
			{
				int iChoice = Menu(0,gameOptions);
				choice = gameOptions[iChoice];
				if(choice == "Game 1"||choice == "Game 2"||choice == "Game 3")
				{
					stringstream ss;
					ss<<"Save"<<(iChoice+1);
					rpgMain game (ss.str(),newGame);
					game.load();
					game.playGame();
					continuing = false;
				}
				else if(choice == "Back")
				{
					continuing = false;
				}
			}
			continuing = true;
		}
		else if(choice == "Level Editor")
		{
			rpgMain defaultGame (defaultDirectory);
			defaultGame.load();
			defaultGame.edit();
			defaultGame.save();
		}
		else if(choice == "Quit")
		{
			continuing = false;
		}
	}
	error.close();
	return 0;
}
#include <iostream>
#include "table.h"
using namespace std;

Table::Table(){
	attrs = nullptr;
	entries = nullptr;
	numAttrs = 0;
	numEntries = 0;
}

Table::Table(const Table& another){
	attrs = new string[another.numAttrs];
	for (int i = 0; i < another.numAttrs; ++i)
		attrs[i] = another.attrs[i];

	entries = new string* [another.numEntries];
	for (int i = 0; i < another.numEntries; ++i)
		entries[i] = new string[another.numAttrs];
	for (int i = 0; i < another.numEntries; ++i){
		for (int j = 0; j < another.numAttrs; ++j){
			entries[i][j] = another.entries[i][j];
		}
	}

	numAttrs = another.numAttrs;
	numEntries = another.numEntries;
}

Table::~Table(){
	if(numAttrs>0){
		delete [] attrs;
		attrs = nullptr;
	}
	if(numEntries>0){
		for (int i = 0; i < numEntries; ++ i){
			delete [] entries[i];
		}
		delete [] entries;
		entries=nullptr;
	}
}

/*bool Table::addAttribute(const string &attr, int index, const string &default_value){
	if (index < -1 || index > numAttrs)
		return false;
	else{
		if(numAttrs == 0){
			attrs = new string[1];
			attrs[0] = attr;

			entries = new string* [numEntries];
			for (int i = 0; i < numEntries; ++i)
				entries[i] = new string[numAttrs + 1];

			for (int i = 0; i < numEntries; ++i){
				for (int j = 0; j < numAttrs + 1; ++j){
					entries[i][j] = default_value;
				}
			}

			++numAttrs;

			return true;
		}

		Table temp(*this);
		delete [] attrs;
		for (int i = 0; i < numEntries; ++ i){
			delete [] entries[i];
		}
		delete [] entries;

		attrs = new string[numAttrs + 1];
		for (int i = 0, j = 0; i < numAttrs + 1; ++i, ++j){
			if (i == index){
				attrs[i] = attr;
				j--;
			}
			else if (i == numAttrs && index == -1){
				attrs[i] = attr;
			}
			else{
				attrs[i] = temp.attrs[j];
			}
		}

		entries = new string* [numEntries];
		for (int i = 0; i < numEntries; ++i)
			entries[i] = new string[numAttrs + 1];

		for (int i = 0; i < numEntries; ++i){
			for (int j = 0, k = 0; j < numAttrs + 1; ++j, ++k){
				if (index == j){
					entries[i][j] = default_value;
					k--;
				}
				else if (j == numAttrs && index == -1){
					entries[i][j] = default_value;
				}
				else{
					entries[i][j] = temp.entries[i][k];;
				}
			}
		}

		++numAttrs;
		temp.~Table();

		return true;
	}
}*/

bool Table::addAttribute(const string &attr, int index, const string &default_value){
	if (index < -1 || index > numAttrs)
		return false;
	else{
		if(numAttrs == 0){
			attrs = new string[1];
			attrs[0] = attr;

			entries = new string* [numEntries];
			for (int i = 0; i < numEntries; ++i)
				entries[i] = new string[numAttrs + 1];

			for (int i = 0; i < numEntries; ++i){
				for (int j = 0; j < numAttrs + 1; ++j){
					entries[i][j] = default_value;
				}
			}

			++numAttrs;

			return true;
		}

		//Table temp(*this);

		string* newAttrs = new string[numAttrs + 1];
		for (int i = 0, j = 0; i < numAttrs + 1; ++i, ++j){
			if (i == index){
				newAttrs[i] = attr;
				j--;
			}
			else if (i == numAttrs && index == -1){
				newAttrs[i] = attr;
			}
			else{
				newAttrs[i] = attrs[j];    //temp.attrs[j];
			}
		}

		delete [] attrs;
		attrs = newAttrs;
		newAttrs = nullptr;

		string** newEntries = new string* [numEntries];
		for (int i = 0; i < numEntries; ++i)
			newEntries[i] = new string[numAttrs + 1];

		for (int i = 0; i < numEntries; ++i){
			for (int j = 0, k = 0; j < numAttrs + 1; ++j, ++k){
				if (index == j){
					newEntries[i][j] = default_value;
					k--;
				}
				else if (j == numAttrs && index == -1){
					newEntries[i][j] = default_value;
				}
				else{
					newEntries[i][j] = entries[i][k];      //temp.entries[i][k];;
				}
			}
		}

		for (int i = 0; i < numEntries; ++ i){
			delete [] entries[i];
		}
		delete [] entries;

		entries = newEntries;
		newEntries=nullptr;

		++numAttrs;
		//temp.~Table();
		return true;
	}
}

/*bool Table::addEntry(const string *entry, int index){
	if (index < -1 || index > numEntries)
		return false;
	else{
		if(numEntries == 0){

			entries = new string* [numEntries + 1];
			entries[0] = new string[numAttrs];
			for (int i = 0; i < numEntries + 1; ++i){
				for (int j = 0; j < numAttrs; ++j){
					entries[i][j] = entry[j];
				}
			}

			++numEntries;

			return true;
		}
		Table temp(*this);
		for (int i = 0; i < numEntries; ++ i){
			delete [] entries[i];
		}
		delete [] entries;

		entries = new string* [numEntries + 1];
		for (int i = 0; i < numEntries + 1; ++i)
			entries[i] = new string[numAttrs];

		for (int i = 0, j = 0; i < numEntries + 1; ++i, ++j){
			for (int k = 0; k < numAttrs; ++k){
				if (index == i){
					entries[i][k] = entry[k];
					if(k == 0)
						j--;
				}
				else if (i == numEntries && index == -1){
					entries[i][k] = entry[k];
				}
				else{
					entries[i][k] = temp.entries[j][k];;
				}
			}
		}

		++numEntries;
		temp.~Table();
		return true;
	}
}*/

bool Table::addEntry(const string *entry, int index){
	if (index < -1 || index > numEntries)
		return false;
	else{
		if(numEntries == 0){

			entries = new string* [numEntries + 1];
			entries[0] = new string[numAttrs];
			for (int i = 0; i < numEntries + 1; ++i){
				for (int j = 0; j < numAttrs; ++j){
					entries[i][j] = entry[j];
				}
			}

			++numEntries;

			return true;
		}
		//Table temp(*this);

		string **newEntries = new string* [numEntries + 1];
		for (int i = 0; i < numEntries + 1; ++i)
			newEntries[i] = new string[numAttrs];

		for (int i = 0, j = 0; i < numEntries + 1; ++i, ++j){
			for (int k = 0; k < numAttrs; ++k){
				if (index == i){
					newEntries[i][k] = entry[k];
					if(k == 0)
						j--;
				}
				else if (i == numEntries && index == -1){
					newEntries[i][k] = entry[k];
				}
				else{
					newEntries[i][k] = entries[j][k];;
				}
			}
		}

		for (int i = 0; i < numEntries; ++ i){
			delete [] entries[i];
		}
		delete [] entries;

		entries = newEntries;
		newEntries = nullptr;

		++numEntries;
		//temp.~Table();
		return true;
	}
}

/*bool Table::deleteAttribute(int index){
	if (index < 0 || index >= numAttrs)
		return false;
	else{
		Table temp(*this);
		delete [] attrs;
		for (int i = 0; i < numEntries; ++ i){
			delete [] entries[i];
		}
		delete [] entries;

		if(numAttrs - 1 == 0){
			attrs = nullptr;
			entries = nullptr;
		}
		else{
			attrs = new string[numAttrs - 1];
			for (int i = 0, j = 0; i < numAttrs - 1; ++i, ++j){
				if (index == i){
					--i;
					index = -1;
				}
				else{
					attrs[i] = temp.attrs[j];
				}
			}

			entries = new string* [numEntries];
			for (int i = 0; i < numEntries; ++i)
				entries[i] = new string[numAttrs - 1];

			for (int i = 0; i < numEntries; ++i){
				for (int j = 0, k = 0; j < numAttrs - 1; ++j, ++k){
					if (index == j){
						--j;
						index = -1;
					}
				else{
						entries[i][j] = temp.entries[i][k];;
					}
				}
			}
		}
		--numAttrs;
		temp.~Table();

		return true;
	}
}*/

bool Table::deleteAttribute(int index){
	if (index < 0 || index >= numAttrs)
		return false;
	else{
		//Table temp(*this);

		if(numAttrs == 1){
			delete [] attrs;
			attrs = nullptr;

			if(numEntries > 0){
				for (int i = 0; i < numEntries; ++ i){
					delete [] entries[i];
				}
				delete [] entries;
				entries = nullptr;
			}
			numEntries = 0;
		}
		else{
			string* newAttrs = new string[numAttrs - 1];
			for (int i = 0, j = 0; i < numAttrs - 1; ++i, ++j){
				if (index == i){
					--i;
					index = -1;
				}
				else{
					newAttrs[i] = attrs[j];
				}
			}

			string **newEntries = new string* [numEntries];
			for (int i = 0; i < numEntries; ++i)
				newEntries[i] = new string[numAttrs - 1];

			for (int i = 0; i < numEntries; ++i){
				for (int j = 0, k = 0; j < numAttrs - 1; ++j, ++k){
					if (index == j){
						--j;
						index = -1;
					}
				else{
						newEntries[i][j] = entries[i][k];;
					}
				}
			}
			delete [] attrs;
			attrs = newAttrs;
			newAttrs = nullptr;

			for (int i = 0; i < numEntries; ++ i){
				delete [] entries[i];
			}
			delete [] entries;
			entries = newEntries;
			newEntries = nullptr;

		}
		--numAttrs;
		//temp.~Table();
		return true;
	}
}

/*bool Table::deleteEntry(int index){
	if (index < 0 || index >= numEntries)
		return false;
	else{
		Table temp(*this);
		for (int i = 0; i < numEntries; ++ i){
			delete [] entries[i];
		}
		delete [] entries;

		if(numEntries - 1 == 0)
			entries = nullptr;
		else{
			entries = new string* [numEntries - 1];
			for (int i = 0; i < numEntries - 1; ++i)
				entries[i] = new string[numAttrs];

			for (int i = 0, j = 0; i < numEntries - 1; ++i, ++j){
				for (int k = 0; k < numAttrs; ++k){
					if (i == index){
						index = -1;
						--i;
						break;
					}
					else{
						entries[i][k] = temp.entries[j][k];;
					}
				}
			}
		}


		--numEntries;
		temp.~Table();

		return true;
	}
}*/

bool Table::deleteEntry(int index){
	if (index < 0 || index >= numEntries)
		return false;
	else{
		//Table temp(*this);


		if(numEntries - 1 == 0){
			for (int i = 0; i < numEntries; ++ i){
				delete [] entries[i];
			}
			delete [] entries;
			entries = nullptr;
		}
		else{
			string **newEntries = new string* [numEntries - 1];
			for (int i = 0; i < numEntries - 1; ++i)
				newEntries[i] = new string[numAttrs];

			for (int i = 0, j = 0; i < numEntries - 1; ++i, ++j){
				for (int k = 0; k < numAttrs; ++k){
					if (i == index){
						index = -1;
						--i;
						break;
					}
					else{
						newEntries[i][k] = entries[j][k];;
					}
				}
			}
			for (int i = 0; i < numEntries; ++ i){
				delete [] entries[i];
			}
			delete [] entries;
			entries = newEntries;
			newEntries = nullptr;
		}


		--numEntries;
		//temp.~Table();

		return true;
	}
}


bool checkOrder(Table &ori, const Table &comp){
	if (ori.getNumAttrs() != comp.getNumAttrs())
		return false;

	for(int i = 0; i < ori.getNumAttrs(); i ++){
		if(ori.getAttribute(i) != comp.getAttribute(i))
			return false;
	}
	return true;
}

bool Table::append(const Table &another){
	if (checkOrder(*this, another) == false)
		return false;
	else{
		for(int i = 0; i < another.numEntries ;i ++)
			addEntry(another.entries[i], -1);

		return true;
	}
}

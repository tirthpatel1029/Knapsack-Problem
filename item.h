#ifndef ITEM_H
#define ITEM_H

using namespace std;

class Item{
	public: 
		Item(); // default constructor
		int SrNo;
		int Value;
		int Weight;
		
		void toString() const;
		bool operator<(const Item& other) const;
		Item operator=(const Item& other) const;
		
	private:
};

Item::Item(){
	SrNo = 0;
	Value = 0;
	Weight = 0;
}

void Item::toString() const{
	cout << "Sr:" << SrNo << " Value:" << Value << " Weight:" << Weight << endl;
}

bool Item::operator<(const Item& other) const{
	double i1 = (double)Value/Weight;
	double i2 = (double)other.Value/other.Weight;
	
	// An item is small if it's fraction profitability is less. Using it to sort items in main.
	if(i1<i2)
		return true;
	else if(i1 == i2){
		if(Value > other.Value)
			return true;
		else
			return false;
	}
	else 
		return false;
}

Item Item::operator=(const Item& other) const{
	return other;
}
    
#endif
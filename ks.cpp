#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <iomanip>
#include "item.h"

using namespace std;
int maxRunTime = 600;
time_t startTime;
int originalLimit;
int bestProfit;
vector<int> bestTaken;
vector<int> weightsSortedInd;	// List holding the sorted weight indices of items.

// Function to print the input data.
void printData(vector<Item>& items){
	cout << "SrNo: \t" << "Value \t" << "Weight \t"  << "FrcProf" << endl;
	for(int i=0; i<items.size(); i++){
		cout << items[i].SrNo << ": \t";
		cout << items[i].Value << "\t";
		cout << items[i].Weight << "\t";
		double frcProf = (double)items[i].Value/items[i].Weight;
		cout << setprecision(3) << frcProf << endl;
	}
}

// Function to swap two items with each other.
void swapItems(Item& a, Item& b){
	int temp;
	temp = a.SrNo;
	a.SrNo = b.SrNo;
	b.SrNo = temp;
	
	temp = a.Value;
	a.Value = b.Value;
	b.Value = temp;
	
	temp = a.Weight;
	a.Weight = b.Weight;
	b.Weight = temp;
}

// Function to sort the items list in decreasing fractional probability.
void sortListInDecreasingFractionalProfitability(vector<Item>& items){
	for(int i=0; i<items.size(); i++){
		int max = i;
		Item maxItem = items[i];
		for(int j=i+1; j<items.size(); j++){
			if(maxItem < items[j]){
				max = j;
				Item newMax = items[j];
				swapItems(maxItem, newMax);
			}
		}
		swapItems(items[i], items[max]);
	}
}

// Function to create a list with item index such that the items are sorted
// byt their weight values.
void listItemIndInSortedWeights(vector<Item>& items){
	weightsSortedInd.resize(items.size());
	int prevLeast = -1;
	int prevWeight = 0;
	int startInd = 0;
	for(int i=0; i<items.size(); i++){
		int curLeast = startInd;
		for(int j=1; j<items.size(); j++){
			if(j!=prevLeast && items[j].Weight>=prevWeight && items[j].Weight<items[curLeast].Weight){
				curLeast = j;
			}
		}
		weightsSortedInd[i] = curLeast;
		if(curLeast == startInd)
			startInd += 1;
		prevLeast = curLeast;
		prevWeight = items[prevLeast].Weight;
	}
}

// Function to calculate the total Value from given stating index to end of the vector.
int Total_Profit_In_Branch(vector<Item> items, int start){
	int total = 0;
	for(int i=start; i<items.size(); i++){
		total += items[i].Value;
	}
	return total;
}

// Function to find the greedy solution for the problem. Take all items
// with maximum fractional profitability as lond as it fits in the limit.
int Find_Greedy_Solution(vector<Item>& items, int limit){
	int profit = 0;
	
	for(int i=0; i<items.size(); i++){
		if(items[i].Weight < limit){
			bestTaken[items[i].SrNo] = 1;
			profit += items[i].Value;
			limit -= items[i].Weight;
		}
	}
	
	bestProfit = profit;
	return profit;
}

// Function to calculate the maximum Value for the knapsack problem. Uses recursion
// to find the the best optimal solution. For very big input's it will only run for a 
// certain period of time and returns the best solution found by then.
int Compute_Max_Profit(vector<Item>& items, vector<int>& taken, int limit, int branchTotal, int wInd = 0, int curProfit=0, int curInd = 0){
	// Base case 1: Terminate the program is run time exceds given time limit.
	if(difftime(time(NULL), startTime) >= maxRunTime){
		return curProfit;
	}
	// Base case 2: Solution found or we don't have any item that can fit our remaining capacity.
	if(limit<items[weightsSortedInd[wInd]].Weight || curInd>=items.size()){
		// If the solution is greater then the previous best, update the bestScore data.
		if(curProfit > bestProfit){
			bestProfit = curProfit;
			for(int i=0; i<items.size(); i++){
				bestTaken[i] = taken[i];
			}
			//cout << "New Best Profit : " << bestProfit << endl;
		}
		return curProfit;
	}
	// If the branchTotal is less than the bestProfit and the limit
	// is same as originalLimit, then there is no need to explore this branch.
	else if((branchTotal <= bestProfit) && (limit == originalLimit)){
		return curProfit;
	}
	// In all other case, keep doing the computation.
	else{
		// We are here means we are still computing the Value.
		
		// Check if the weight of cur item fits in the given limit. If it
		// doesn't then skip them.
		while((curInd < items.size()) && (items[curInd].Weight > limit)){
			branchTotal -= items[curInd].Value;	// Update the branch total
			curInd++;			// Increment the current index
		}
		// If we have reached at the end of the data while skiping, then do 
		// the function call to the base case for final computation.
		if(curInd >= items.size()){
			return Compute_Max_Profit(items, taken, limit, branchTotal, wInd, curProfit, curInd);
		}
		else{			
			// Else explore the branch posibility using backtracking.
			
			// Calculate the new parameter for the next function call
			int newBranchTotal = branchTotal - items[curInd].Value;
			int newProfit = curProfit + items[curInd].Value;
			int newLimit = limit - items[curInd].Weight;
			
			// If our current item is the least weighted not taken item, than we update the
			// pointing index value.
			if(items[weightsSortedInd[wInd]].Weight == items[curInd].Weight)
				wInd += 1;
			
			// Value if we take the current item, then limit and curProfit will be
			// replaced with newProfit and newLimit.
			taken[(items[curInd].SrNo)] = 1;
			int takenProfit = Compute_Max_Profit(items, taken, newLimit, newBranchTotal, wInd, newProfit, curInd+1);
			
			// Value if we don't take the current item, then limit and curProfit
			// remains same.
			if(items[weightsSortedInd[wInd]].Weight == items[curInd].Weight)
				wInd -= 1;
			taken[(items[curInd].SrNo)] = 0;
			int notTaken = Compute_Max_Profit(items, taken, limit, newBranchTotal, wInd, curProfit, curInd+1);
			
			// And return the greater Value.
			if(notTaken > takenProfit)
				return notTaken;
			else
				return takenProfit;
		}
	}
}

// Verifier to check whether our solution is valid or not.
bool verifier(vector<Item>& items, int limit, int solution, vector<int>& assign){
	// Calculate the total weight and profit of items that are taken.
	int verifyProfit = 0;
	int verifyWeight = 0;
	for(int i=0; i<assign.size(); i++){
		if(assign[i] == 1){
			int sortedInd = -1;
			for(int j=0; j<items.size(); j++){
				if(i == items[j].SrNo){
					sortedInd = j;
				}
			}
			verifyWeight += items[sortedInd].Weight;
			verifyProfit += items[sortedInd].Value;
		}
	}
	//cout << "verifyProfit = " << verifyProfit << " || actualProfit = " << solution << endl;
	//cout << "verifyWeight = " << verifyWeight << " || actualLimit = " << limit << endl;
	// Check if it is valid or not.
	if(verifyWeight>limit || verifyProfit != solution)
		return false;
	else
		return true;
}

int main(){
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Version: Asks user to enter the input file name and runs the program. 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Reading the file name and opening it
	string filename;
	cout << "Enter file name : ";
	cin >> filename;
	
	ifstream fin;
	fin.open(filename.data());
	while (!fin.is_open())
	{
		fin.clear();
		cout << "Invalid file, Enter a valid name : ";
		cin >> filename;
		fin.open(filename.data());
	}
	
	// Reading the content from file and saving it in the respective variables.	
	int numItems, weightLimit;		// Number of items and weight limit
	fin >> numItems;
	fin >> weightLimit;
	originalLimit = weightLimit;	// Global variable holding the original weight limit
	
	vector<Item> myItems;				// Vector holding all the items
	myItems.resize(numItems);
	vector<int> isTaken;			// Binary vector for taken, not taken.
	isTaken.resize(numItems);
	bestTaken.resize(numItems);		// Vector to hold the best solution
	
	for(int i=0; i<numItems; i++){
		myItems[i].SrNo = i;
		fin >> myItems[i].Value;
		fin >> myItems[i].Weight;
	}
	
	//printData(myItems);

	sortListInDecreasingFractionalProfitability(myItems);	// Sort all the items in decreasing frac. prof.
	listItemIndInSortedWeights(myItems);	// Create a sorted list of item indices based on their weights.
	
	// First we find the greedy solution.
	int greedy = Find_Greedy_Solution(myItems, weightLimit);
	cout << "greedy = " << greedy << endl;
	
	if(verifier(myItems, weightLimit, greedy, bestTaken))
		cout << "======Valid Solution======" << endl;
	else
		cout << "=====Invalid Solution=====" << endl;
	
	// We find our backtracking solution.
	int allTotal = Total_Profit_In_Branch(myItems, 0);	// Variable holding total Value of all items
	startTime = time(NULL);				// Start time of the program.
	int backtracking = Compute_Max_Profit(myItems, isTaken, weightLimit, allTotal);
	cout << "backtracking = " << backtracking << endl;
	
	if(verifier(myItems, weightLimit, backtracking, bestTaken))
		cout << "======Valid Solution======" << endl;
	else
		cout << "=====Invalid Solution=====" << endl;
	
	// Print out our result
	cout << bestProfit << endl;
	for(int i=0; i<numItems; i++)
		cout << bestTaken[i] << " ";
	cout << "" << endl;
	
	// Create the output file with the output
	string outputfile = filename + "_output";
	ofstream fout;
	fout.open(outputfile.data(), ios_base::out);
	
	fout << bestProfit << endl;
	for(int i=0; i<numItems; i++)
		fout << bestTaken[i] << " ";
	fout << "" << endl;
	
	return 0;
}
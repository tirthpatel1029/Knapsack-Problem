# Knapsack-Problem

Problem Description:
The problem is mathematically formulated in the following way. Given n items to choose from, each item i ∈ 0..n-1 has a value vi and a weight wi . The knapsack has a limited capacity K. Your job is to maximize the profit of all items taken while having the weight of the objects taken be ≤ K

Input File Format:
A knapsack input contains n + 1 lines. The first line contains two integers, the first is the number of items in the problem, n. The second number is the capacity of the knapsack, K. The remaining lines present the data for each of the items. Each line after the first contains two integers, the item's value vi followed by its weight wi.

Output File Format:
The output contains a knapsack solution and is made of two lines. The first line contains the maximum profit you computed. The next line is a list of n 0/1 values, corresponding to whether you’ve taken(1) or not taken(0) the item. Make sure the order of items in your output matches the order of items in your input! So if you sort or something, make sure you keep track of the original position of each item for when you do output.

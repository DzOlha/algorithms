
#include <iostream>
#include "IntervalTree.h"
int main()
{
    IntervalTree<double, double>* tree = new IntervalTree<double, double>;
    const int rows = 7;
    const int cols = 2;
    double arr[7][2] = {
        {19.5, 22},
        {10, 11.6},
        {4.7, 6.8},
        {1.5, 2.1},
        {2.2, 4.7},
        {13.8, 23.98},
        {11.65, 13}
    };
    Interval<double>* arrIntervals[rows];
   
    for (int i = 0; i < rows; i++) {
        arrIntervals[i] = new Interval<double>;
        arrIntervals[i]->setLow(arr[i][0]);
        arrIntervals[i]->setHigh(arr[i][1]);

        tree->insert(arrIntervals[i], arr[i][1] - arr[i][0]);
    }
 
    Node<double, double>* rootNode = tree->getRoot();
    tree->treeWalk(rootNode, 30);
}
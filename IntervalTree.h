#pragma once
#ifndef INTERVALTREE_H
#define INTERVALTREE_H
#include "Node.h"

template<typename TPoint, typename TValue>
class IntervalTree {
private:
	Node<TPoint, TValue>* root;
	Node<TPoint, TValue>* nilNode;
public:
	IntervalTree() {
		this->root = NULL;
		this->nilNode = NULL;
	}

	Node<TPoint, TValue>* getRoot() {
		return this->root;
	}
	bool isEmpty() {
		return (this->root == NULL || this->root == this->nilNode);
	}
	int findIntersections(Interval<TPoint>* interval) {
		Pair<TPoint, TValue>* pair(interval);
		Node<TPoint, TValue>* search_node = Node<TPoint, TValue>::withPair(pair);
		int count = 0;
		for(auto node : this->treeSearchInterval(search_node)) {
			node->getPair();
			count++;
		}
		return count;
	}
	bool hasIntersection(Interval<TPoint>* interval)
	{
		int nodes = this->findIntersections(interval);
		if (nodes > 0)
			return true;
		return false;
	}
	void insert(Interval<TPoint>* interval, TValue value)
	{
		Pair<TPoint, TValue>* new_pair = new Pair<TPoint, TValue>;
		new_pair->setInterval(interval);
		new_pair->setValue(value);
		Node<TPoint, TValue>* insertNode = new Node<TPoint, TValue>;
		insertNode->setPair(new_pair);
		insertNode->setLeft(this->nilNode);
		insertNode->setRight(this->nilNode);
		insertNode->setParent(NULL);
		insertNode->setColor("red");

		this->treeInsert(insertNode);
		insertNode->updateMax();
		this->recalculateMax(insertNode);
	}
	bool exist(Interval<TPoint>* interval, TValue value)
	{
		Node<TPoint, TValue>* searchNode = Node<TPoint, TValue>::withPair(new Pair<TPoint, TValue>*(interval, value));
		return (this->treeSearch(this->root, searchNode) != NULL);
	}
	bool remove(Interval<TPoint>* interval, TValue value)
	{
		Node<TPoint, TValue>* searchNode = new Node<TPoint, TValue>;
		Pair<TPoint, TValue>* new_pair = new Pair<TPoint, TValue>;
		new_pair->setInterval(interval);
		new_pair->setValue(value);
		searchNode->setPair(new_pair);

		Node<TPoint, TValue>* deleteNode = this->treeSearch(this->root, searchNode);
		if (deleteNode) {
			this->treeDelete(deleteNode);
			return true;
		}
		return false;
	}
	void recalculateMax(Node<TPoint, TValue>* node)
	{
		Node<TPoint, TValue>* nodeCurrent = node;
		while (nodeCurrent->getParent() != NULL) {
			nodeCurrent->getParent()->updateMax();
			nodeCurrent = nodeCurrent->getParent();
		}
	}
	void treeInsert(Node<TPoint, TValue>* insertNode)
	{
		Node<TPoint, TValue>* currentNode = this->root;
		Node<TPoint, TValue>* parentNode = NULL;

		if (this->root == NULL|| this->root == this->nilNode) {
			this->root = insertNode;
		}
		else {
			while (currentNode != this->nilNode) {
				parentNode = currentNode;
				if (insertNode->lessThan(currentNode)) {
					currentNode = currentNode->getLeft();
				}
				else {
					currentNode = currentNode->getRight();
				}
			}

			insertNode->setParent(parentNode);

			if (insertNode->lessThan(parentNode)) {
				parentNode->setLeft(insertNode);
			}
			else {
				parentNode->setRight(insertNode);
			}
		}
		this->insertFixup(insertNode);
	}
	void insertFixup(Node<TPoint, TValue>* insertNode)
	{
		Node<TPoint, TValue>* currentNode = insertNode;
		Node<TPoint, TValue>* grandfather;
		Node<TPoint, TValue>* uncleNode;
		while (currentNode->getParent() && currentNode->getParent()->isRed()) {
			grandfather = currentNode->getParent()->getParent();
			if (grandfather && currentNode->getParent() == grandfather->getLeft()) {
				uncleNode = grandfather->getRight();
				if (uncleNode->isRed()) {
					currentNode->getParent()->setColor("black");
					uncleNode->setColor("black");
					grandfather->setColor("red");
					currentNode = grandfather;
				}
				else {
					if (currentNode == currentNode->getParent()->getRight()) {
						currentNode = currentNode->getParent();
						this->rotateLeft(currentNode);
					}
					currentNode->getParent()->setColor("black");
					grandfather->setColor("red");
					this->rotateRight(grandfather);
				}
			}
			else {
				uncleNode = grandfather->getLeft();
				if (uncleNode->isRed()) {
					currentNode->getParent()->setColor("black");
					uncleNode->setColor("black");
					grandfather->setColor("red");
					currentNode = grandfather;
				}
				else {
					if (currentNode == currentNode->getParent()->getLeft()) {
						currentNode = currentNode->getParent();
						this->rotateRight(currentNode);
					}
					currentNode->getParent()->setColor("black");
					grandfather->setColor("red");
					this->rotateLeft(grandfather);
				}
			}
		}
		this->root->setColor("black");
	}
	void treeDelete(Node<TPoint, TValue>* deleteNode)
	{
		Node<TPoint, TValue>* cutNode;
		Node<TPoint, TValue>* fixNode;
		if (deleteNode->getLeft() == this->nilNode || deleteNode->getRight() == this->nilNode) {
			cutNode = deleteNode;
		}
		else {
			cutNode = this->treeSuccessor(deleteNode);
		}

		if (cutNode->getLeft() != this->nilNode) {
			fixNode = cutNode->getLeft();
		}
		else {
			fixNode = cutNode->getRight();
		}

		fixNode->setParent(cutNode->getParent());

		if (cutNode->getParent() == NULL) {
			this->root = fixNode;
		}
		else {
			if (cutNode == cutNode->getParent()->getLeft()) {
				cutNode->getParent()->setLeft(fixNode);
			}
			else {
				cutNode->getParent()->setRight(fixNode);
			}
			cutNode->getParent()->updateMax();
		}

		this->recalculateMax(fixNode);

		if (cutNode != deleteNode) {
			deleteNode->copyPairFrom(cutNode);
			deleteNode->updateMax();
			this->recalculateMax(deleteNode);
		}

		if (cutNode->isBlack()) {
			this->deleteFixup(fixNode);
		}
	}
	void deleteFixup(Node<TPoint, TValue>* fixNode)
	{
		Node<TPoint, TValue>* currentNode = fixNode;
		Node<TPoint, TValue>* brotherNode = NULL;
		while (
				currentNode != this->root && currentNode->getParent() != NULL
				&& currentNode->isBlack()
			) {
			if (currentNode == currentNode->getParent()->getLeft()) {
				brotherNode = currentNode->getParent()->getRight();
				if (brotherNode->isRed()) {
					brotherNode->setColor("black");
					currentNode->getParent()->setColor("red");
					this->rotateLeft(currentNode->getParent());
					brotherNode = currentNode->getParent()->getRight();
				}

				if (brotherNode->getLeft()->isBlack()) {
					brotherNode->setColor("red");
					currentNode = currentNode->getParent();
				}
				else {
					if (brotherNode->getRight()->isBlack()) {
						brotherNode->setColor("red");
						brotherNode->getLeft()->setColor("black");
						this->rotateRight(brotherNode);
					}
					brotherNode->setColor(currentNode->getParent()->getColor());
					currentNode->getParent()->setColor("black");
					brotherNode->getRight()->setColor("black");
					this->rotateLeft(currentNode->getParent());
					currentNode = this->root;
				}
			}
			else {
				brotherNode = currentNode->getParent()->getLeft();
				if (brotherNode->isRed()) {
					brotherNode->setColor("black");
					currentNode->getParent()->setColor("red");
					this->rotateRight(currentNode->getParent());
					brotherNode = currentNode->getParent()->getLeft();
				}
				if (brotherNode->getRight()->isBlack()) {
					brotherNode->setColor("red");
					currentNode = currentNode->getParent();
				}
				else {
					if (brotherNode->getLeft()->isBlack()) {
						brotherNode->setColor("red");
						brotherNode->getRight()->setColor("black");
						this->rotateLeft(brotherNode);
					}
					brotherNode->setColor(currentNode->getParent()->getColor());
					currentNode->getParent()->setColor("black");
					brotherNode->getLeft()->setColor("black");
					this->rotateRight(currentNode->getParent());
					currentNode = this->root;
				}
			}
		}
		currentNode->setColor("black");
	}
	Node<TPoint, TValue>* treeSearch(Node<TPoint, TValue>* startingNode, Node<TPoint, TValue>* node) {
		Node<TPoint, TValue>* searchedNode;
		if (startingNode == this->nilNode) {
			return NULL;
		}

		if (node->equalTo(startingNode)) {
			searchedNode = startingNode;
		} else
			if(node->lessThan(startingNode)) {
				searchedNode = this->treeSearch(startingNode->getLeft(), node);
			}
			else {
				searchedNode = this->treeSearch(startingNode->getRight(), node);
			}
		return searchedNode;
	}

	Node<TPoint, TValue>* localMinimum(Node<TPoint, TValue>* node)
	{
		Node<TPoint, TValue>* nodeMin = node;
		while (nodeMin->getLeft() != NULL && nodeMin->getLeft() != this->nilNode) {
			nodeMin = nodeMin->getLeft();
		}
		return nodeMin;
	}
	Node<TPoint, TValue>* treeSuccessor(Node<TPoint, TValue>* node)
    {
		Node<TPoint, TValue>* currentNode, * nodeSuccessor, * parentNode;
        if (node->getRight() != this->nilNode) {
            nodeSuccessor = this->localMinimum(node->getRight());
        } else {
            currentNode = node;
            parentNode = node->getParent();
            while (parentNode != NULL && (parentNode->getRight() == currentNode)) {
                currentNode = parentNode;
                parentNode = parentNode->getParent();
            }
            nodeSuccessor = parentNode;
        }
        return nodeSuccessor;
    }
	void rotateLeft(Node<TPoint, TValue>* x)
	{
		Node<TPoint, TValue>* y = x->getRight();
		x->setRight(y->getLeft());

		if (y->getLeft() != this->nilNode) {
			y->getLeft()->setParent(x);
		}
		y->setParent(x->getParent());

		if (x->getParent() == NULL) {
			this->root = y;
		} else
			if(x == x->getParent()->getLeft()) {
				x->getParent()->setLeft(y);
			}
			else {
				x->getParent()->setRight(y);
			}
		y->setLeft(x);
		x->setParent(y);

		if (x != this->nilNode) {
			x->updateMax();
		}

		y = x->getParent();
		if (y != NULL && y != this->nilNode) {
			y->updateMax();
		}
	}
	void rotateRight(Node<TPoint, TValue>* y)
	{
		Node<TPoint, TValue>* x = y->getLeft();
		y->setLeft(x->getRight());

		if (x->getRight() != this->nilNode) {
			x->getRight()->setParent(y);
		}
		x->setParent(y->getParent());

		if (y->getParent() == NULL) {
			this->root = x;
		} else
			if(y == y->getParent()->getLeft()) {
				y->getParent()->setLeft(x);
			}
			else {
				y->getParent()->setRight(x);
			}
		x->setRight(y);
		y->setParent(x);

		if (y != this->nilNode) {
			y->updateMax();
		}

		x = y->getParent();
		if (x != NULL && x != this->nilNode) {
			y->updateMax();
		}
	}
	void treeWalk(Node<TPoint, TValue>* root, int space) {
		if (!root) return;
		space += 10;
		treeWalk(root->getRight(), space);
		std::cout << "\n";
		for (int i = 10; i < space; i++) {
			std::cout << " ";
		}
		root->printNodeInterval();
		treeWalk(root->getLeft(), space);
	}
};
#endif
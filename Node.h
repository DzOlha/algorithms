#pragma once
#ifndef NODE_H
#define NODE_H
#include "Pair.h"
#include "Interval.h"
template<typename TPoint, typename TValue>
class Node {
private:
	Node<TPoint, TValue>* left;
	Node<TPoint, TValue>* right;
	Node<TPoint, TValue>* parent;
	int color;
	//0 - red; 1 - black
	Pair<TPoint, TValue>* pair;
	Interval<TPoint>* max;
public:
	Node<TPoint, TValue>(){
		right = left = parent = NULL;
		this->color = 0;
		Pair<TPoint, TValue>* new_pair = new Pair<TPoint, TValue>;
		this->pair = new_pair;
		Interval<TPoint>* new_interval = new Interval<TPoint>;
		this->max = new_interval;
	}
	Node<TPoint, TValue>(const Node<TPoint, TValue> &other) {
		this->right = other->right;
		this->left = other->left;
		this->parent = other->parent;
		this->pair = other->pair;
		this->color = other->color;
		this->max = other->max;
	}
	Node<TPoint, TValue>& operator=(const Node<TPoint, TValue>& other) {
		this->right = other->right;
		this->left = other->left;
		this->parent = other->parent;
		this->pair = other->pair;
		this->color = other->color;
		this->max = other->max;
		return *this;
	}
	void setPair(Pair<TPoint, TValue>* pair) {
		this->pair = pair;
	}
	
	Node<TPoint, TValue> nil() 
	{
		Node<TPoint, TValue> self = new Node<TPoint, TValue>;
		self.setColor("back");
		return self;
	}
	void setColor(const char* color) {
		if (color == "red")
			this->color = 0;
		if (color == "black")
			this->color = 1;
	}
	void setColor(int color) {
		this->color = color;
	}
	int getColor() {
		return this->color;
	}
	bool isRed() {
		if (this && this->color == 0)
			return true;
		return false;
	}
	bool isBlack() {
		if (this && this->color == 1)
			return true;
		return false;
	}

	Node<TPoint, TValue>* getLeft() {
		return this->left;
	}
	void setLeft(Node<TPoint, TValue>* node) {
		this->left = node;
	}

	Node<TPoint, TValue>* getRight() {
		return this->right;
	}
	void setRight(Node<TPoint, TValue>* node) {
		this->right = node;
	}

	Node<TPoint, TValue>* getParent() {
		if(this)
			return this->parent;
	}
	void setParent(Node<TPoint, TValue>* node) {
		if(this)
			this->parent = node;
	}

	Pair<TPoint, TValue>* getPair() {
		return this->pair;
	}
	Interval<TPoint>* getMax() {
		if (this != NULL) {
			return this->max;
		}
	}
	bool lessThan(Node<TPoint, TValue>* otherNode) {
		return this->getPair()->getInterval()->lessThan(otherNode->getPair()->getInterval());
	}
	bool equalTo(Node<TPoint, TValue>* otherNode) {
		bool valueEqual = true;
		if (this->getPair()->getValue() && otherNode->getPair()->getValue()) {
			valueEqual = (this->getPair()->getValue() == otherNode->getPair()->getValue());
		}
		return this->getPair()->getInterval()->equalTo(otherNode->getPair()->getInterval()) && valueEqual;
	}
	bool intersect(Node<TPoint, TValue>* otherNode) {
		return this->getPair()->getInterval()->intersect(otherNode->getPair()->getInterval());
	}
	void copyPairFrom(Node<TPoint, TValue>* otherNode) {
		this->pair = otherNode->getPair();
	}
	void updateMax() {
		this->max = this->getPair()->getInterval();
		if (this->getRight() != NULL) {
			if (this->getRight()->getMax() != NULL) {
				this->max = this->getMax()->merge(this->getRight()->getMax());
			}
		}
		if (this->getLeft() != NULL) {
			if (this->getLeft()->getMax() != NULL) {
				this->max = this->getMax()->merge(this->getLeft()->getMax());
			}
		}
	}
	TPoint operat(TPoint x, TPoint y) {
		if (x == NULL)
			return x;
		return y;
	}
	bool notIntersectLeftSubtree(Node<TPoint, TValue>* searchNode) {
		TPoint high = operat(this->getLeft()->max->getHigh(), this->getLeft()->getPair()->getInterval()->getHigh());
		return high < searchNode->getPair()->getInterval()->getLow();
	}
	bool notIntersectRightSubtree(Node<TPoint, TValue>* searchNode) {
		TPoint low = operat(this->getRight()->max->getLow(), this->getRight()->getPair()->getInterval()->getLow());
		return searchNode->getPair()->getInterval()->getHigh() < low;
	}
	void printNodeInterval() {
		std::cout << this->getColor() << " [" << this->pair->getInterval()->getLow() << ", " 
			<< this->pair->getInterval()->getHigh() << "] | " << this->max->getHigh();
	}
};
#endif
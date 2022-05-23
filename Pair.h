#pragma once
#ifndef PAIR_H
#define PAIR_H
#include "Interval.h"
template<typename TPoint, typename TValue>
class Pair {
private:
	Interval<TPoint>* interval;
	TValue value;
public:
	Pair() {
		this->interval = NULL;
		this->value = NULL;
	}
	Pair(Interval<TPoint>* interval, TValue value) {
		this->interval = interval;
		this->value = value;
	}
	void setInterval(Interval<TPoint>* interval) {
		this->interval = interval;
	}
	void setValue(TValue value) {
		this->value = value;
	}
	Interval<TPoint>* getInterval() {
		if(this)
			return this->interval;
	}
	TValue getValue() {
		return this->value;
	}
};
#endif
#pragma once
#ifndef INTERVAL_H
#define INTERVAL_H
#include "Exception.h"
template<typename TPoint>
class Interval {
private:
	TPoint low;
	TPoint high;
public:
	Interval<TPoint>() {
		this->low = 0;
		this->high = 0;
	}
	~Interval<TPoint>() {
		this->low = NULL;
		this->high = NULL;
	}
	Interval<TPoint>(TPoint low, TPoint high) {
		if (low > high) {
			throw Exception("Low interval cannot be greater than high");
		}
		this->low = low;
		this->high = high;
	}
	void setLow(TPoint low) {
		this->low = low;
	}
	void setHigh(TPoint high) {
		this->high = high;
	}
	static Interval<TPoint>* fromArray(TPoint* arr) {
		int count = 0;
		while (arr[count] != NULL) {
			count++;
		}
		if (count != 2) {
			throw Exception("Wrong interval array");
		}
		else {
			Interval<TPoint>* new_interval(arr[0], arr[1]);
			return new_interval;
		}
	}
	TPoint getLow() {
		return this->low;
	}
	TPoint getHigh() {
		return this->high;
	}
	bool equalTo(Interval<TPoint>* otherInterval)
	{
		bool first = (this->getLow() == otherInterval->getLow());
		bool second = (this->getHigh() == otherInterval->getHigh());
		return  (first && second) ;
	}
	bool lessThan(Interval<TPoint>* otherInterval)
	{
		bool lessLow = (this->getLow() < otherInterval->getLow());
		bool eqLow = (this->getLow() == otherInterval->getLow());
		bool lessHigh = (this->getHigh() < otherInterval->getHigh());
		return (lessLow || (eqLow && lessHigh));
	}
	bool intersect(Interval<TPoint>* otherInterval)
	{
		bool thisHigh_lessOtherLow = (this->getHigh() < otherInterval->getLow());
		bool  otherHigh_lessThisLow = (otherInterval->getHigh() < this->getLow());
		return !(thisHigh_lessOtherLow || otherHigh_lessThisLow);
	}
	Interval<TPoint>* merge(Interval<TPoint>* otherInterval)
	{
		TPoint min = this->getLow() <= otherInterval->getLow() ? this->getLow() : otherInterval->getLow();
		TPoint max = this->getHigh() >= otherInterval->getHigh() ? this->getHigh() : otherInterval->getHigh();
		Interval<TPoint>* new_interval = new Interval<TPoint>;
		if (min < max) {
			new_interval->setLow(min);
			new_interval->setHigh(max);
		}
		
		return new_interval;
	}
};
#endif
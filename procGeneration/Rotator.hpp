#pragma once



class Rotator {
	int index = 0;

public:
	void inc() {
		index++;
		if (index > 3) {
			index = 0;
		}
	}

	void dec() {
		index--;
		if (index < 0) {
			index = 3;
		}
	}

	void set(int index) {
		this->index = index;
	}

	int get() {
		return index;
	}


};
#include "checker.h"

bool isSemesterValid(const int semester) {
    return semester >= 1 && semester <= 8;
}

bool isWorkNumberValid(const int workNumber) {
    return workNumber >= 1;
}

bool isRussianString(const Wt::WString& str) {
    for (const auto& letter : str.toUTF16()) {
		if (!(0x0410 <= letter && letter <= 0x044F || letter == 0x0401 || letter == 0x0451)) {
			return false;
		}
	}

	return true;
}

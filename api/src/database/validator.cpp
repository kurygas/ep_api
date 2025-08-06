#include "validator.h"
#include "random_functions.h"

bool Validator::isSemesterNumberValid(const int semesterNumber) {
    return semesterNumber >= 1 && semesterNumber <= 8;
}

bool Validator::isRussianString(const Wt::WString& str) {
    for (const auto& letter : str.toUTF32()) {
		if (!(0x410 <= letter && letter <= 0x44F || letter == 0x401 || letter == 0x451)) {
			return false;
		}
	}

	return true;
}

bool Validator::isTimeSegmentValid(const Wt::WDateTime& start, const Wt::WDateTime& end) {
    return start < end;
}

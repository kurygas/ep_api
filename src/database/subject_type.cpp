#include "subject_type.h"

Subject::Type parse(const Wt::Json::Value& value) {
    return static_cast<Subject::Type>(static_cast<int>(value));
}

#include "http_exceptions.h"

int InvalidMethodException::code() const {
    return 405;
}

int AuthException::code() const {
    return 401;
}

int AdminException::code() const {
    return 403;
}

int NotFoundException::code() const {
    return 404;
}

int BadRequestException::code() const {
    return 400;
}

int UnprocessableEntityException::code() const {
    return 422;
}

int ServerException::code() const {
    return 500;
}

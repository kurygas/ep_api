#pragma once

#include <exception>
#include <stdexcept>

class HttpException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;

    virtual int code() const = 0;
};

class InvalidMethodException : public HttpException {
public:
    using HttpException::HttpException;

    int code() const override;
};

class AuthException : public HttpException {
public:
    using HttpException::HttpException;

    int code() const override;
};

class ForbiddenException : public HttpException {
public:
    using HttpException::HttpException;

    int code() const override;
};

class NotFoundException : public HttpException {
public:
    using HttpException::HttpException;

    int code() const override;
};

class BadRequestException : public HttpException {
public:
    using HttpException::HttpException;

    int code() const override;
};

class UnprocessableEntityException : public HttpException {
public:
    using HttpException::HttpException;

    int code() const override;
};

class ServerException : public HttpException {
public:
    using HttpException::HttpException;
    
    int code() const override;
};

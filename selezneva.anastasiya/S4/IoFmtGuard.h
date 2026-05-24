#ifndef IO_FMT_GUARD_H
#define IO_FMT_GUARD_H

#include <iostream>

class IoFmtGuard {
public:
    explicit IoFmtGuard(std::basic_ios<char>& s)
        : s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {
    }

    ~IoFmtGuard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    IoFmtGuard(const IoFmtGuard&) = delete;
    IoFmtGuard& operator=(const IoFmtGuard&) = delete;

private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

#endif

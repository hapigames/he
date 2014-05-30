#include <iostream>
#include <sstream>

using namespace std;

class hstring {
    ostringstream oss_;
    string delim_;
    int count_;

    void checkdelim() {
        if (count_ > 0) oss_<<delim_;
    }
    

public:

    hstring(const string &delim) : delim_(delim),count_(0) {
    }

    virtual ~hstring() {
        oss_.str("");
        oss_.clear();
        delim_.clear();
    }
    

    const char *str() const {
        return oss_.str().c_str();
    }


    hstring& operator <<(string &data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (int data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (unsigned int data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (long data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (unsigned long data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (long long data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (unsigned long long data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (bool data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (double data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (float data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator <<(const string &data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }
    hstring& operator << (const char *data) {
        checkdelim();
        oss_<<data;
        count_ ++;
        return *this;
    }

    hstring& operator << (const ostringstream &data) {
        checkdelim();
        oss_<<data.str();
        count_ ++;
        return *this;
    }
    hstring& operator << (const hstring &data) {
        checkdelim();
        oss_<<data.str();
        count_ ++;
        return *this;
    }
    
    //friend ostream& operator <<(ostream &os, const hstring &data);
};

ostream& operator <<(ostream &os, const hstring &data) {
    os<<data.str();
    return os;
}


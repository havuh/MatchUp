#ifndef DATE_H
#define DATE_H


class Date
{

private:
    unsigned short day, month, year;

public:
    Date() {}
    Date(unsigned short day, unsigned short month, unsigned short year) : day(day), month(month), year(year) {}
    ~Date() {}

    void setDay     (unsigned short day)    { this->day = day       ; }
    void setMonth   (unsigned short month)  { this->month = month   ; }
    void setYear    (unsigned short year)   { this->year = year     ; }

    unsigned short  getDay()                { return day    ; }
    unsigned short  getMonth()              { return month  ; }
    unsigned short  getYear()               { return year   ; }

};

#endif // DATE_H

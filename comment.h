#ifndef COMMENT_H
#define COMMENT_H


#include <iostream>
#include "date.h"

using std::string;

class Comment
{
private:
    string		autor;
    string		text;
    Date		date;

public:
    Comment() {}
    Comment(string autor, string text, Date date) : autor(autor), text(text), date(date) {}
    ~Comment(){}

    void setDate(Date date)     { this->date = date     ; }
    void setText(string text)   { this->text = text     ; }
    void setAutor(string autor) { this->autor = autor   ; }

    Date&	getDate()  { return date    ; }
    string	getText()  { return text    ; }
    string	getAutor() { return autor   ; }

};

#endif // COMMENT_H

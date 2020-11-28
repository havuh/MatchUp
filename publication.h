#ifndef PUBLICATION_H
#define PUBLICATION_H


#include <iostream>
#include <vector>
#include "date.h"
#include "comment.h"
#include "interaction.h"

using std::string;
using std::vector;

class Publication
{
private:
    int						ID;
    string					title;
    string					desc;
    Date					pubDate;
    unsigned int			numLikes;
    vector<Comment>			comments;
    vector<Interaction>		interactions;

public:
    Publication() { numLikes = 0; }
    Publication(int ID, string title, string desc, Date pubDate, unsigned int numLikes) : ID(ID), title(title),
        desc(desc), pubDate(pubDate), numLikes(numLikes) {}
    ~Publication() {}

    void setID(int ID)                                      { this->ID = ID             ; }
    void setTitle(string title)                             { this->title = title       ; }
    void setDesc(string desc)                               { this->desc = desc         ; }
    void setPubDate(Date pubDate)                           { this->pubDate = pubDate   ; }
    void setNumLikes(unsigned int numLikes)                 { this->numLikes = numLikes ; }
    void setComments(vector<Comment> comments)              { this->comments = comments ; }

    string					getTitle()                      { return title          ; }
    string					getDesc()                       { return desc           ; }
    Date&					getPubDate()                    { return pubDate        ; }
    unsigned int			getNumLikes()                   { return numLikes       ; }
    vector<Comment>&		getComments()                   { return comments       ; }
    vector<Interaction>&	getInteractions()               { return interactions   ; }

    void setInteractions(vector<Interaction> interactions)  { this->interactions = interactions ; }
    void addComment(Comment newComent)                      { comments.push_back(newComent)     ; }
    void addLike()                                          { numLikes++                        ; }
    int  getID()                                            { return ID                         ; }

};

#endif // PUBLICATION_H

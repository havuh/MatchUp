#ifndef PLABEL_H
#define PLABEL_H

#include <iostream>
using std::string;

class pLabel {
private:
    string		name;
    string		value;
    bool		open;
    bool		close;
public:
    pLabel() {}
    ~pLabel() {}

    string	getName()   { return name;   }
    string	getValue()  { return value;  }
    bool	is_open()   { return open;   }
    bool	is_close()  { return close;  }

    void	clear() { name = value = ""; open = close = true; }
    void	process(string line) {
        clearIndented(line);
        int i = 1;
        while (line.at(i - 1) != '>') {
            if (i == line.size() - 1) {
                if (line.at(1) == '/') {
                    open = false;
                    name = line.substr(2, i - 2);
                }
                else {
                    close = false;
                    name = line.substr(1, i - 1);
                }
            }

            else {
                name = line.substr(1, i - 1);
                value = line.substr(i + 1, line.size() - (name.size() + 2) * 2 - 1);
            }

            i++;
        }
    }

private:
    void clearIndented(string& line) {
        int ini = 0;
        for (ini; ini < line.size(); ini++) {
            if (line.at(ini) == '<') {
                break;
            }
        }
        line = line.substr(ini, line.size() - ini);
    }
};

#endif // PLABEL_H

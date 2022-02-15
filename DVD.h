/** Kellyn Mendez - CS 2337.003, Project 3
 *      NetID: kpm200000
 */

#ifndef DVD_H
#define DVD_H
#include <string>

class DVD
{
private:
    std::string title;
    int available;
    int rented;

public:
    // Constructors
    DVD();
    DVD(std::string t, int a, int r);

    // Accessors
    std::string getTitle() const {return title;}
    int getNumAvailable() const {return available;}
    int getNumRented() const {return rented;}

    // Mutators
    void setTitle(std::string t) {title = t;}
    void setNumAvilable(int a) {available = a;}
    void setNumRented(int r) {rented = r;}

    // Overloaded operators
    bool operator<(const DVD&);
    bool operator>(const DVD&);
    bool operator==(const DVD&);
    friend std::ostream& operator<<(std::ostream&, const DVD&);

};

#endif //DVD_H

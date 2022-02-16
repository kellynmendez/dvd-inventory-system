/** DVD class
 */

#include "DVD.h"
#include <string>

DVD::DVD()
{
    title = "";
    available = 0;
    rented = 0;
}

DVD::DVD(std::string t, int a, int r)
{
    title = t;
    available = a;
    rented = r;
}

bool DVD::operator<(const DVD& right)
{
    if (title < right.getTitle())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DVD::operator>(const DVD& right)
{
    if (title > right.getTitle())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DVD::operator==(const DVD& right)
{
    if (title == right.getTitle())
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::ostream& operator<<(std::ostream& o, const DVD& d)
{
    o << d.getTitle();
    return o;
}


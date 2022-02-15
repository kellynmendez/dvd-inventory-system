/** Kellyn Mendez - CS 2337.003, Project 3
 *      NetID: kpm200000
 */

#include "DVD.h"
#include "Node.h"
#include "BSTree.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
using namespace std;

// Function definitions
bool checkFormat(string, bool);
int displayReport(Node<DVD>*);

int main()
{
    // Declaring the inventory binary search tree
    BSTree<DVD>* inventory = nullptr;

    // Get inventory file name from the user
    string filename;
    cout << "Enter inventory file name: ";
    cin >> filename;

    // Open inventory file
    ifstream infile;
    infile.open(filename);
    if (infile.is_open())
    {

        // Create the binary search tree to store the inventory
        inventory = new BSTree<DVD>();
        string line;

        // While there are more lines to read from the file
        while (getline(infile, line))
        {
            // Variables to store information on the DVD
            string title, temp;
            int avail, rent;

            // Creating an input string stream to store information about the DVD
            istringstream stream(line);

            // Reading and storing the title and the quantities using a comma delimiter
            getline(stream, title, ',');
            getline(stream, temp, ',');
            title = title.substr(1, title.size() - 2); // deleting quotations
            avail = stoi(temp); // converts string to integer to store the quantity available
            getline(stream, temp, ',');
            rent = stoi(temp); // converts string to integer to store the quantity rented

            // Creating the DVD and a node to store it with the given input values
            DVD movie(title, avail, rent);

            // If the tree was previously empty, make this node the root
            if (inventory->getRoot() == nullptr)
            {
                Node<DVD>* add = new Node<DVD>(movie);
                inventory->setRoot(add);
            }
                // Otherwise, insert the new node into the binary search tree
            else
            {
                inventory->insert(movie, inventory->getRoot());
            }
        }
    }

    // Closing the file
    infile.close();

    // Getting transaction log file name
    cout << "Enter transaction log file name: ";
    cin >> filename;

    // Opening transaction log file
    infile.open(filename);
    if (infile.is_open() && inventory != nullptr)
    {
        // Creating an output file stream to write any possible errors to error log
        ofstream outfile("error.log");

        // While there is more information to read from the file
        string line;
        string storeLine;
        while(getline(infile, line))
        {
            // Create an input string stream to read in the line
            istringstream stream(line);

            // Read in the action to be done
            string action;
            stream >> action;

            // If the action is to add or remove DVDs
            if (action == "add" || action == "remove")
            {
                bool validFormat = checkFormat(line, true);
                if (!validFormat)
                {
                    // Writes error to error log
                    outfile << line << endl;
                }
                else
                {
                    // Clearing the stream to read only the rest of the line after the action
                    string line2;
                    line2 = line.substr(action.size() + 1);
                    stream.clear();
                    stream.str(line2);

                    // Read in the title and the quantity to add/remove using a comma as the delimiter
                    string title, temp;
                    getline(stream, title, ',');
                    getline(stream, temp, ',');
                    title = title.substr(1, title.size() - 2); // deleting quotations

                    DVD toFind(title, 0, 0);
                    Node<DVD>* found = inventory->search(toFind, inventory->getRoot());


                    // If the found node is null, then the movie does not exist in the tree
                    if (found == nullptr)
                    {
                        // If action is to add, then add the new movie to the tree
                        if (action == "add")
                        {
                            // Store number to add in an integer
                            int numToAdd = stoi(temp);

                            // Insert the new movie into the tree
                            DVD movie(title, numToAdd, 0);
                            inventory->insert(movie, inventory->getRoot());
                        }
                        else // If action is remove, then there is an error because the movie does not exist
                        {
                            // Writes error to error log
                            outfile << line << endl;
                        }
                    }
                    // If the found node is not null, then the movie exists
                    else if (action == "add")
                    {
                        // Add the number to add to the number already available and store in an integer
                        int numAvail = stoi(temp) + found->getPayload().getNumAvailable();
                        // Create DVD object with updated quantities
                        DVD movie(title, numAvail, found->getPayload().getNumRented());
                        // Reset found node's payload to the DVD object
                        found->setPayload(movie);
                    }
                    else // If action is remove
                    {
                        // Subtract the number to add to the number already available and store in an integer
                        int numAvail = found->getPayload().getNumAvailable() - stoi(temp);
                        // Create DVD object with updated quantities
                        DVD movie(title, numAvail, found->getPayload().getNumRented());
                        // Reset found node's payload to the DVD object
                        found->setPayload(movie);
                        // If the number available and the number rented are both equal to 0,
                        //      then remove the node from the tree
                        if (movie.getNumAvailable() == 0 && movie.getNumRented() == 0)
                        {
                            // Removing the node from the tree
                            Node<DVD>* toRemove = inventory->remove(movie, inventory);
                            // Deleting the allocated memory for that node
                            delete toRemove;
                        }
                    }
                }
            }
            // If the action was to rent or return
            else if (action == "rent" || action == "return")
            {
                // Checking whether or not the line has the correct format
                bool validFormat = checkFormat(line, false);
                // If the line did not have the correct format
                if (!validFormat)
                {
                    // Writes error to error log
                    outfile << line << endl;
                }
                else
                {
                    // Clearing the stream to read only the rest of the line after the action
                    string line2;
                    line2 = line.substr(action.size() + 1);
                    stream.clear();
                    stream.str(line2);

                    // Storing title in a string variable
                    string title;
                    getline(stream, title, '"'); // Scanning up to first quotation
                    getline(stream, title, '"'); // Reading in the title

                    // Creating a DVD object with the movie title, and searching the tree of the movie
                    DVD toFind(title, 0, 0);
                    Node<DVD>* found = inventory->search(toFind, inventory->getRoot());
                    // If the found node is null, then the movie does not exist in the tree
                    if (found == nullptr)
                    {
                        // Cannot rent or return a movie that does not exist
                        // Write the error to the error log
                        outfile << line << endl;
                    }
                    else
                    {
                        // If the action is to rent
                        if (action == "rent")
                        {
                            // Create a new DVD object with one less available and one more rented
                            DVD movie(title, found->getPayload().getNumAvailable() - 1,
                                      found->getPayload().getNumRented() + 1);
                            // Reset found node's value to the updated DVD
                            found->setPayload(movie);
                        }
                        else // If the action is to return
                        {
                            // Create a new DVD object with one more available and one less rented
                            DVD movie(title, found->getPayload().getNumAvailable() + 1,
                                      found->getPayload().getNumRented() - 1);
                            // Reset found node's value to the updated DVD
                            found->setPayload(movie);
                        }
                    }
                }
            }
            else // Action is not a valid action
            {
                // Write the error to the error log
                outfile << line << endl;
            }
        }

        // Closing the error log
        outfile.close();
    }
    // Closing the file
    infile.close();

    if (inventory != nullptr)
    {
        // Displaying inventory report
        displayReport(inventory->getRoot());
    }

    // Deleting the allocated memory for the inventory
    delete inventory;

    return 0;
}

/** Checks that there is an action at the beginning of the input line, that the title is in quotations,
 *      and that the title and number to add (if add/remove) is an integer
 * @param line, a non-empty string of input to check
 * @return whether or not the input is valid according to the format of a line in
 *      the transaction or the inventory file
 */
bool checkFormat(string line, bool addOrRemove)
{
    istringstream stream;

    // Reading in the action
    string action;
    // If the line is empty, then the format is not valid
    if (line.empty())
    {
        return false;
    }
    else
    {
        // Otherwise, have the stream read the line, and read in the first word as the action
        stream.str(line);
        stream >> action;
    }

    // Storing the rest of the string into a new line
    string line2;
    // If there is more to read after the string, make a substring with the rest of the line after the action
    if (action.size() + 1 < line.size())
    {
        line2 = line.substr(action.size() + 1);
    }
    else
    {
        // If there is not more to read after the string, the format is invalid
        return false;
    }

    // Clearing the stream to read the rest of the line
    stream.clear();
    stream.str(line2);
    string temp;

    // Read in the title of the movie
    if (addOrRemove && getline(stream, temp, ','))
    {
        // If the title does not begin with a quotation, then the input is invalid
        if (temp.at(0) != '"')
        {
            return false;
        }
        // If the title does not end with a quotation, then the input is invalid
        else if (temp.at(temp.size() - 1) != '"')
        {
            return false;
        }

        // Read in the number to add or remove
        if (getline(stream, temp, ','))
        {
            // For every index of temp
            for (int i = 0; i < static_cast<int>(temp.size()) - 1; i++)
            {
                // If the index is not a digit, then the input is invalid
                if (!isdigit(temp.at(i)))
                {
                    return false;
                }
            }
        }
        // If there was nothing left to read in, then the input is invalid
        else
        {
            return false;
        }
    }
    // If there was nothing left to read in, then the input is invalid
    else if (addOrRemove)
    {
        return false;
    }

    // If the line being checked is not for add or remove, then only check the title
    if (!addOrRemove)
    {
        // Storing the title in temp
        getline(stream, temp);

        // Get rid of whitespace at the end of temp
        while (isspace(temp.at(temp.size() - 1)))
        {
            temp = temp.substr(0, temp.size() - 1);
        }

        // If the title does not begin with a quotation, then the input is invalid
        if (temp.at(0) != '"')
        {
            return false;
        }
        // If the title does not end with a quotation, then the input is invalid
        else if (temp.at(temp.size() - 1) != '"')
        {
            return false;
        }
    }

    return true;
}

/** Prints the binary search tree containing the inventory information using an in-order
 *      recursive traversal of the tree
 * @param curr, the current node to print
 * @return 0 to indicate when the function's recursion is done
 */
int displayReport(Node<DVD>* curr)
{
    // If current node is null, then there is nothing else o=to print in this branch
    if (curr == nullptr)
    {
        return 0;
    }

    // Print the left subtree
    displayReport(curr->getLeft());
    // Print the current node with information formatted into neat columns
    DVD print = curr->getPayload();
    printf("%-45s %-3i  %-3i", print.getTitle().c_str(),
           print.getNumAvailable(), print.getNumRented());
    cout << endl;
    // Print the right subtree
    displayReport(curr->getRight());

    return 0;
}

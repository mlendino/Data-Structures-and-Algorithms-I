/*
 Michael Lendino Data Structures and Algorithms I Spring 2018 Programming Assignment #1

 This program asks the user for the name of an input text file and an output text file; the input
 file contains commands directing the program to create a stack or queue (either of which can store
 int, double, or string, and push to or pop from the stack or queue.The stacks and queues are
 implemented using a singly linked list.  Thank you Ali and Kevin for showing me new ways to debug and
 checking my C++ conventions.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

using namespace std;
/* standard class (user defined data type that represents the concept of a singly linked list)
  for the list, from this class, stacks and queues will be derived; what makes this class
  special is that it has a nested class for the node since lists have multiple nodes and
  we're going to have to work with the sequence of them */
template <class T>
class SimpleList{
private:
    /* private nested class called Node, which contains one field for data and another to
    point to the next node; we only define a pointer to the next node because we're
    implementing it as a singly linked list so we dont need a pointer to the previous node */
    class Node{
    public:
        Node *nextNode;
        T fieldForData;
    };
    string nameOfListt;
    int sizeOfList;
    //we define these as the dummy nodes since they arent inside of the Node class
    Node *firstNode, *lastNode;
public:
    SimpleList(string nameOfList);
    string get_nameOfList();
    int get_sizeOfList();
    virtual void push(T pushV) =  0;
    virtual T pop() = 0;
protected:
    void insertFirstNode(T fieldforData);
    void insertLastNode(T fieldforData);
    T removeFirstNode();
};

//member function for inserting the first node, special case included for an empty list
template <class T>
void SimpleList<T>::insertFirstNode(T fieldforData) {
    Node* temporary =  new Node();
    temporary->fieldForData = fieldforData;
    temporary->nextNode = NULL;
    if (sizeOfList == 0){
        firstNode = temporary;
        lastNode = temporary;
    }
    temporary->nextNode = firstNode;
    firstNode = temporary;
    sizeOfList++;
};
//similar to the above member function, except inserts last node, special case if empty list
template <class T>
void SimpleList<T>::insertLastNode(T data) {
    Node* temporary =  new Node();
    temporary->fieldForData = data;
    temporary->nextNode = NULL;
    if (sizeOfList == 0){
        firstNode = temporary;
        lastNode = temporary;
    }
    lastNode->nextNode = temporary;
    lastNode = temporary;
    sizeOfList++;
};
//another member function to remove the first node
template <class T>
T SimpleList<T>::removeFirstNode() {
    T valPop = firstNode->fieldForData;
    Node* temporary =  firstNode;
    firstNode = firstNode->nextNode;
    //deleting temporary because with large pops, we want to deallocate the memory as per convention
    delete temporary;
    sizeOfList--;
    return valPop;
};
//this constructor is implemented to allocate memory for the object SimpleList
template <class T>
SimpleList<T>::SimpleList(string nameOfList) {
    nameOfListt = nameOfList;
    sizeOfList = 0;
    firstNode = 0;
    lastNode = 0;
};

template <class T>
string SimpleList<T>::get_nameOfList() {
    return nameOfListt;
};

template <class T>
int SimpleList<T>::get_sizeOfList(){
    return sizeOfList;
};
//derived class from base SimpleList class, queue is FIFO as implemented
template <class T>
class Queue : public SimpleList<T>{
public:
    Queue<T>(string nameOfList):SimpleList<T>(nameOfList){};
    //overwriting the virtal push() function
    void push(T fieldforData){
        this->insertLastNode(fieldforData);
    }
    T pop(){
        return (this->removeFirstNode());
    }
};
//derived class from base SimpleList class, stack is LIFO as implemented
template <class T>
class Stack : public SimpleList<T>{
public:
    Stack<T>(string nameOfList):SimpleList<T>(nameOfList){};
    //overwriting the virtal push() function
    void push(T fieldforData){
        this->insertFirstNode(fieldforData);
    }
    T pop(){
        return (this->removeFirstNode());
    }
};

template <class T>
//takes in list of pointers to objects and name of the list as parameters
SimpleList<T>* simpleListSearch(list<SimpleList<T> *> *searchTheSimpleList, string nameOfList){
    for(typename list<SimpleList<T> *>::iterator i = searchTheSimpleList->begin(); i != searchTheSimpleList->end(); i++){
        if((*i)->get_nameOfList() == nameOfList){
            return *i;
        }
    }
    return NULL;
}

template <class T>
void process(list<SimpleList<T> *> *listofProcess, string command, string name, T Value, string value, string indicator, ofstream *of){
    if(command == "create"){
        if(simpleListSearch(listofProcess, name) != NULL){
            (*of) << "ERROR: This name already exists!" <<endl;
        }
        else{
            if(value == "stack"){
                SimpleList<T> *n = new Stack<T>(name);
                listofProcess->push_back(n);
            }
            else{
                SimpleList<T> *n = new Queue<T>(name);
                listofProcess->push_back(n);
            }
        }
    }
    else if(command == "push"){
        SimpleList<T> *n = simpleListSearch(listofProcess, name);
        if(n == NULL){
            (*of) << "ERROR: This name does not exist!"<<endl;
        }
        else{
            n->push(Value);
        }
    }
    else {
        SimpleList<T> *n = simpleListSearch(listofProcess, name);
        if (n == NULL) {
            (*of) << "ERROR: This name does not exist!" << endl;
        }
        else {
            if((n->get_sizeOfList()) == 0){
                (*of) << "ERROR: This list is empty!" << endl;
            }
            else {
                T value = n->pop();
                (*of) << "Value popped: " << value <<endl;
            }
        }
    }
}

int main(int argc, char **argv) {

    ofstream outputFile;
    string iFile, oFile, lineToBeParsed;
    //prompting the user for an input file, opening the file for reading; same for output file and writing.
    cout << "Enter name of input file: ";
    getline(cin, iFile);
    cout << "Enter name of output file: ";
    getline(cin, oFile);
    outputFile.open(oFile.c_str());
    ifstream inputFile(iFile.c_str());

    list < SimpleList<int> *> listSLi; //all integer stacks and queues (credit to Professor Sable)
    list < SimpleList<double> *> listSLd; //all double stacks and queues
    list < SimpleList<string> *> listSLs; //all string stacks and queues

    while (getline(inputFile, lineToBeParsed)) {
        string command1, name1, value1, indicator1;
        stringstream parse;
        parse << lineToBeParsed;
        //lineToBeParsed is passed into these three strings
        parse >> command1 >> name1 >> value1;
        indicator1 = name1[0];
        outputFile << "PROCESSING COMMAND: " << lineToBeParsed << endl;
        //this block of if and else's uses indicator1 to check to see what data type is stored in the stack or queue
        if (indicator1 == "d") {
            stringstream doubleStream(value1);
            double pushDouble;
            doubleStream >> pushDouble;
            process(&listSLd, command1, name1, pushDouble, value1, indicator1, &outputFile);
        } else if (indicator1 == "i") {
            stringstream intStream(value1);
            int pushInt;
            intStream >> pushInt;
            process(&listSLi, command1, name1, pushInt, value1, indicator1, &outputFile);
        } else {
            string valueOfString = value1.c_str();
            process(&listSLs, command1, name1, valueOfString, value1, indicator1, &outputFile);
        }
    }
    outputFile.close();
    inputFile.close();
    return 0;
}

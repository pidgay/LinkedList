#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

class Element {
public:
    int number{};
    char character{};
    class Element* nextElement = nullptr;
    class Element* prevElement = nullptr;

    void display(){
        cout
        << "Number: " << number << "    "
        << "Character: " << character << endl;
    }

    void update(int newNumber, char newCharacter){
        number = newNumber;
        character = newCharacter;
    }

    bool equals(Element* element){
        return (number == element->number) && (character == element->character);
    }

    bool compare(Element* element) {
        if(element == nullptr){
            return false;
        }
        return number < element->number ? true : false;
    }

    Element(){
        number = 1 + rand() % 10000;
        character = 'A' + (rand() % 26);
    }
};

template <class T>
class LinkedList {
public:
    T* First = nullptr;
    T* Last = nullptr;
    size_t size = 0;

    void addFirstExisting(T* newElement){
        First = newElement;
        Last = newElement;
        size++;
    }

    void addOnBack(T* newElement){
        if(checkIfEmpty()) {
            addFirstExisting(newElement);
        }
        else {
            Last->nextElement = newElement;
            newElement->prevElement = Last;
            Last = newElement;
            size++;
        }
    }

    void addOnFront(T* newElement){
        if(checkIfEmpty()) {
            addFirstExisting(newElement);
        }
        else {
            First->prevElement = newElement;
            newElement->nextElement = First;
            First = newElement;
            size++;
        }
    }

    void addBefore(T* newElement, T* currentElement){
        if (currentElement == First){
            addOnFront(newElement);
            return;
        }
            newElement->nextElement = currentElement;
            newElement->prevElement = currentElement->prevElement;
            currentElement->prevElement->nextElement = newElement;
            currentElement->prevElement = newElement;
            size++;
    }

    void addAfter(T* newElement, T* currentElement) {
        if (currentElement == Last){
            addOnBack(newElement);
            return;
        }
            newElement->nextElement = currentElement->nextElement;
            newElement->prevElement = currentElement;
            currentElement->nextElement = newElement;
            currentElement->nextElement->prevElement = newElement;
            size++;
    }

    void addWithOrder(T* newElement){
        if(checkIfEmpty()){
            addFirstExisting(newElement);
            return;
        }
        T* currentElement = First;
        while(currentElement->nextElement != nullptr && !(newElement->compare(currentElement))){
            currentElement = currentElement->nextElement;
        }
        if(newElement->compare(currentElement)) {
            addBefore(newElement, currentElement);
        }
        else{
            addAfter(newElement, currentElement);
        }
    }

    bool checkIfEmpty(){
       return First == nullptr && Last == nullptr ? true : false;
    }

    bool removeLast(){
        if(Last == First) {
            delete Last;
            First = nullptr;
            Last = nullptr;
            size--;
            return true;
        }
        T* newLast;
        Last->prevElement->nextElement = nullptr;
        newLast = Last->prevElement;
        delete Last;
        Last = newLast;
        size--;
        return true;
    }

    bool removeFirst(){
        if(First == Last) {
            delete First;
            First = nullptr;
            Last = nullptr;
            size--;
            return true;
        }
        T* newFirst;
        First->nextElement->prevElement = nullptr;
        newFirst = First->nextElement;
        delete First;
        First = newFirst;
        size--;
        return true;
    }

    bool checkIndex(int index){
        if (index < 0 || index >= size) {
            cout << "Incorrect index;" << endl;
            return false;
        }
        return true;
    }

    void updateElement(int index, int newNumber, char newCharacter){
        if(checkIndex(index)) {
            T *currentElement = findElement(index);
            currentElement->update(newNumber, newCharacter);
            return;
        }
        cout << "Could not update element" << endl;
    }

    T* findElement(T* elementToFind){
        size_t count = 0;
        T* currentElement = First;
        while(count != size) {
            if(currentElement->equals(elementToFind)) {
                return currentElement;
            }
            currentElement = currentElement->nextElement;
            count++;
        }
        return nullptr;
    }

    T* findElement(int index) {
        if (checkIndex(index)) {
            int currentIndex = (index > size / 2) ? size : 0;
            T *currentElement = (index > size / 2) ? Last : First;

            while (currentIndex != index) {
                if (index > size / 2) {
                    currentElement = currentElement->prevElement;
                    currentIndex--;
                } else {
                    currentElement = currentElement->nextElement;
                    currentIndex++;
                }
            }
            return currentElement;
        }
        return nullptr;
    }

    bool findAndRemoveElement(T* elementToFind){
        if (size == 0){
            return false;
        }

        T* currentElement = findElement(elementToFind);

        if (currentElement != nullptr) {
            if(currentElement == First && currentElement == Last){
                delete currentElement;
                size--;
                return true;
            }
            else if(currentElement == First){
                return removeFirst();
            }
            else if(currentElement == Last) {
                return removeLast();
            }
            else{
                currentElement->prevElement->nextElement = currentElement->nextElement;
                currentElement->nextElement->prevElement = currentElement->prevElement;
                delete currentElement;
                size--;
                return true;
            }
        }
        return false;
    }

    void printListInfo(){
        if(size == 0) {
            cout << "List is empty" << endl;
            return;
        }
        T* currentFirstElement = First;
        int quantityOfElementsToDisplay = 10;
        if (size < 10) quantityOfElementsToDisplay = size;

        cout << "    First elements: " << endl;
        for (size_t i = 0; i < quantityOfElementsToDisplay; ++i) {
            currentFirstElement->display();
            currentFirstElement = currentFirstElement->nextElement;
        }

        cout << "================================================================================" << endl;

        T* currentLastElement = Last;

        cout << "    Last elements: " << endl;
        for (size_t i = size - quantityOfElementsToDisplay; i < size; i++) {
            currentLastElement->display();
            currentLastElement = currentLastElement->prevElement;
        }
    }

    void clearList(){
        T* elementToRemove = First;
        T* nextElement;
        for (int i = 0; i < size; ++i) {
            nextElement = elementToRemove->nextElement;
            delete elementToRemove;
            elementToRemove = nextElement;
        }
        First = nullptr;
        Last = nullptr;
        size = 0;
    }
};

int main() {
    srand(time(nullptr));

    const int MAX_ORDER = 5; // Max order of number of elements

    class LinkedList<Element>* List = new LinkedList<Element>; // Creating a new list

    for (int order = 1; order <= MAX_ORDER; ++order) {

        clock_t timeBefore, timeAfter;

        double timeTaken, avgTime;
        const double numberOfElements = pow(10,order);

        // Adding random elements to the list
        timeBefore = clock();

        for (int i = 0; i < numberOfElements; ++i) {
            class Element *newElement = new Element();
            List->addWithOrder(newElement);
        }

        timeAfter = clock();
        timeTaken = (timeAfter - timeBefore) / (double)CLOCKS_PER_SEC;
        avgTime = timeTaken/numberOfElements*1000000000;

        //Printing list info and time taken
        cout << "================================================================================" << endl;
        cout << "    ADDING RANDOM ELEMENTS" << endl;
        cout << "================================================================================" << endl;
        cout << "Order: " << order << "    "
        << "Number of elements: " << List->size
        << "    " << "Total time:  " << timeTaken << "s" <<"    " << "Avg. time: " << avgTime << "ns" << endl;
        cout << "================================================================================" << endl;
        List->printListInfo();

        // Finding and removing random elements from the list
        const double numberOfTries = pow(10,6);

        timeBefore = clock();

        for (int i = 0; i < numberOfTries; ++i) {
            class Element *tempElement = new Element();
            List->findAndRemoveElement(tempElement);
            delete tempElement;
        }

        timeAfter = clock();
        timeTaken = (timeAfter - timeBefore) / (double)CLOCKS_PER_SEC;
        avgTime = timeTaken/numberOfTries*1000000000;
        //Printing list info and time taken
        cout << "================================================================================" << endl;
        cout << "    REMOVING RANDOM ELEMENTS" << endl;
        cout << "================================================================================" << endl;
        cout << "Order: " << order << "    "
             << "Number of elements: " << List->size
             << "    " << "Total time:  " << timeTaken << "s" << "    " << "Avg. time: " << avgTime << "ns" << endl;
        cout << "================================================================================" << endl;
        List->printListInfo();

        List->clearList();
    }
    delete List;
    return 0;
}

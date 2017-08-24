//a demonstration of how an event system could work in a videogame or something
//i suppose it could be used for anything really

#include <iostream>
#include <string>
#include <unordered_map>    //yeah i know it's a lot of includes
#include <vector>
#include <functional>
#include <typeinfo>

using namespace std;    //i don't want to do this but i do not want to type std:: for everything

//event for when there are no arguments or the arguments are [void]
struct voidEvent {
    voidEvent(string eName, void(*eCallback)()) {
        eventName = eName;
        eventCallback = eCallback;
    }

    string eventName;
    function<void()> eventCallback;
};

//event for when there are arguments, if there needs to be multiple arguments pass them in class or struct form
template<typename argType> struct argEvent {
    argEvent(string eName, void(*eCallback)(argType)) {
        eventName = eName;
        eventCallback = eCallback;
    }
    
    string eventName;
    function<void(argType)> eventCallback;
};

//the greatest thing i've ever written
class eventSystem {
public:
    //registers events with no arguments
    void registerEvent(voidEvent eventToRegister) {
        eventList[eventToRegister.eventName]["void"].push_back(new function<void()>(eventToRegister.eventCallback));
    }

    //registers events that have arguments
    template<typename argType> void registerEvent(argEvent<argType> eventToRegister) {
        //stores the event by its name and its typename
        //then pushes back the function to the vector at that location
        //it's magic
        eventList[eventToRegister.eventName][typeid(argType).name()].push_back(new function<void(argType)>(eventToRegister.eventCallback));
    }

    void fireEvent(string eventName) {
        //searches the vector at map[event name][void] to call all the functions in the list
        for (auto iterator = eventList[eventName]["void"].begin(); iterator != eventList[eventName]["void"].end(); iterator++) {
            (*(function<void()>*)*iterator)();  //magic
        }
    }

    template<typename argType> void fireEvent(string eventName, argType args) {
        //does the same thing as the other fireEvent, but searches map[event name][type name] instead
        for (auto iterator = eventList[eventName][typeid(args).name()].begin(); iterator != eventList[eventName][typeid(args).name()].end(); iterator++) {
            (*(function<void(argType)>*)*iterator)(args);   //more magic
        }
    }
private:
    unordered_map<string, unordered_map<string, vector<void*>>> eventList;  //stores events by name and type name
};


//TEST FUNCTIONS
void pString(string words) {
    cout << words << endl;
}

void pNumber(int number) {
    cout << number << endl;
}

void pHello(void) {
    cout << "Hello" << endl;
}

//main routine
int main(void) {
    eventSystem eSystem;    //creates the event system
    
    //the next three blocks do the following:
    //creates an event, gives it a type, and registers it
    argEvent<int> printNumber("print", pNumber); 
    eSystem.registerEvent<int>(printNumber);
    
    argEvent<string> printString("print", pString);
    eSystem.registerEvent<string>(printString);

    voidEvent printHello("hello", pHello);
    eSystem.registerEvent(printHello);

    eSystem.fireEvent<int>("print", 2); //fire the print event with type int
    eSystem.fireEvent<string>("print", "hello");    //fires the print event with type string
    eSystem.fireEvent("hello"); //fires a void event called hello
    return 0;
}
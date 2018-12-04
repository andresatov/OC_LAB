#include "Action.h"
#include "view.h"

int main() {
    outputMenu("menu");
    int value= inputChoice();
    Action run;
    while (value){
        run.getAction(value)();
        value=inputChoice();
    }
}
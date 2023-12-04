#include <iostream>

using namespace std;

void action1() {
    cout << "Performing action 1" << endl;
}

void action2() {
    cout << "Performing action 2" << endl;
}

void action3() {
    cout << "Performing action 3" << endl;
}

void action4() {
    cout << "Performing action 4" << endl;
}

int main() {
    char choice;
    
    cout << "Select an action:" << endl;
    cout << "a. Action 1" << endl;
    cout << "b. Action 2" << endl;
    cout << "c. Action 3" << endl;
    cout << "d. Action 4" << endl;
    cout << "Enter your choice (a/b/c/d): ";

    cin >> choice;

    switch(choice) {
        case 'a':
            action1();
            break;
        case 'b':
            action2();
            break;
        case 'c':
            action3();
            break;
        case 'd':
            action4();
            break;
        default:
            cout << "Invalid choice!" << endl;
            break;
    }

    return 0;
}

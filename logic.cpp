#include <iostream>
#include <string.h>
#include <vector>
#include <string.h>
#include <bitset>

#include "logic.h"

class Component {
    public:
    Component() {}
    Component(std::string name) {
        this->name = name;
    }

    std::string name;

    void eval() {}
};

class Connector {
    public:
    Connector() {
        this->value = value;
    }

    Connector(std::string name) {
        this->name = name;
        this->value = value;
        this->connects = connects;
        this->parent = parent;
    }

    std::string name;
    bool value = 0;
    std::vector<Connector*> connects;
    Component *parent;

    void connect(std::vector<Connector*> inputs) {
        for (Connector* i: inputs) {
            this->connects.push_back(i);
        }
    }

    void set(bool value) {
        if (this->value == value) return;
        
        this->value = value; 
        this->parent->eval();
        for (Connector* con: connects) {
            con->set(value);
        }
    }
};

class Elem1: public Component {
    public: 
    Elem1() {
        this->i0 = Connector("i0");
        this->o0 = Connector("o0");
    }

    Connector i0, o0;
};

class Elem2: public Component {
    public: 
    Elem2() {
        this->i0 = Connector("i0");
        this->i1 = Connector("i1");
        this->o0 = Connector("o0");
    }

    Connector i0, i1, o0;
};

class Not: public Elem1 {
    public:
    Not() {}
    Not(std::string name) {
        this->name = name;
    }

    void eval() {
        this->o0.set(!i0.value);
    }
};

class And: public Elem2 {
    public:
    And() {}
    And(std::string name) {
        this->name = name;
    };

    void eval() {
        this->o0.set(i0.value && i1.value);
    }
};

class Or: public Elem2 {
    public:
    Or() {}
    Or(std::string name) {
        this->name = name;
    };

    void eval() {
        this->o0.set(i0.value || i1.value);
    }
};

class Xor: public Elem2 {
    public:
    Xor() {}
    Xor(std::string name) {
        this->name = name;
    };

    void eval() {
        this->o0.set(i0.value != i1.value);
    }
};

// int main() {
//     Xor xor1;
//     Xor xor2;
//     And and1;
//     And and2;
//     Or or1;

//     Connector A;
//     Connector B;
//     Connector C;
    
//     A.connects = {&xor1.i0, &and2.i0};
//     B.connects = {&xor1.i1, &and2.i1};
//     C.connects = {&xor2.i1, &and1.i0};

//     xor1.o0.connects = {&xor2.i0, &and1.i1};
    
//     and1.o0.connects = {&or1.i0};
//     and2.o0.connects = {&or1.i1};

//     A.set(1);
//     B.set(0);
//     C.set(0);

//     std::cout << "Sum: "   << xor2.o0.value << ", carry: " << or1.o0.value;
// }
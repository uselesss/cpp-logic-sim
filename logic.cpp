#include <iostream>
#include <string.h>
#include <vector>
#include <string.h>
#include <bitset>

class Component {
    public:
    Component() {}
    Component(std::string name) {
        this->name = name;
    }

    std::string name;

    virtual void eval() = 0;
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
        
        if (this->parent) this->parent->eval();
        for (Connector* con: this->connects) {
            con->set(value);
        }
    }
};

class Elem1: public Component {
    public: 
    Elem1() {
        this->i0 = new Connector("i0");
        this->o0 = new Connector("o0");
    }

    Connector *i0, *o0;
    virtual void eval() = 0;
};

class Elem2: public Component {
    public: 
    Elem2() {
        this->i0 = new Connector("i0");
        this->i1 = new Connector("i1");
        this->o0 = new Connector("o0");
    }

    Connector *i0, *i1, *o0;
    virtual void eval() = 0;
    
};

class Not: public Elem1 {
    public:
    Not() {}
    Not(std::string name) {
        this->name = name;
        this->i0->parent = this;
        this->o0->parent = this;
    }

    virtual void eval() {
        this->o0->set(!i0->value);
    }
};

class Xor: public Elem2 {
    public:
    Xor() {}
    Xor(std::string name) {
        this->name = name;
        this->i0->parent = this; 
        this->i1->parent = this;
        this->o0->parent = this;
    };

    void eval() {
        this->o0->set(i0->value != i1->value);
    }
}; 

class And: public Elem2 {
    public:
    And() {}
    And(std::string name) {
        this->name = name;
        this->i0->parent = this; 
        this->i1->parent = this;
        this->o0->parent = this;
    };

    virtual void eval() override {
        this->o0->set(i0->value && i1->value);
    }
};

class Or: public Elem2 {
    public:
    Or() {}
    Or(std::string name) {
        this->name = name;
        this->i0->parent = this; 
        this->i1->parent = this;
        this->o0->parent = this;
    };

    virtual void eval() override {
        this->o0->set(i0->value || i1->value);
    }
};

int main() {
    Xor xor1("xor1");
    Xor xor2("xor2");
    And and1("and1");
    And and2("and2");
    Or or1("or1");

    Connector A("A");
    Connector B("B");
    Connector C("C");
    
    A.parent = &xor1;
    B.parent = &xor1;
    C.parent = &and1;
    
    A.connect({xor1.i0, and2.i1});
    B.connect({xor1.i1, and2.i0});
    C.connect({xor2.i1, and1.i0});
    xor1.o0->connect({xor2.i0, and1.i1});
    and1.o0->connect({or1.i0});
    and2.o0->connect({or1.i1});

    A.set(1);
    B.set(1);
    C.set(1);
    
    std::cout << "Sum value:" << xor2.o0->value << std::endl;
    std::cout << "Carry value:" << or1.o0->value << std::endl;
}
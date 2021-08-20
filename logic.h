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

    virtual void eval() {};
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
    virtual void eval() {};
};

class Elem2: public Component {
    public: 
    Elem2() {
        this->i0 = new Connector("i0");
        this->i1 = new Connector("i1");
        this->o0 = new Connector("o0");
    }

    Connector *i0, *i1, *o0;
    virtual void eval() {};
    
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

    virtual void eval() {
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

    virtual void eval() {
        this->o0->set(i0->value || i1->value);
    }
};
#ifndef _VRP_
#define _VRP_
#include <fstream>
#include <string>

struct Node{
    int value;
    int needs;

    Node(int v = 0, int n = 0): value{v}, needs{n} {}
};

struct Truck{
    int capacity;

    Truck(int c = 0): capacity{c} {}
};

struct Edge{

    int weight;
    Node * a;
    Node * b;

    Edge(int w = 0, Node * p = nullptr, Node * n = nullptr): weight{w}, a{p}, b{n} {}
};

class vrp{
    private:
        Edge ** mat;
    public:
        vrp(std::string filename){

        }
        ~vrp();
};

#endif
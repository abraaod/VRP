#ifndef _VRP_CW_
#define _VRP_CW_
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h> 
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <math.h>
#include <unordered_set>

struct Node{
    int value;
    int needs;
    int x;
    int y;

    Node(int v = 0, int n = 0): value{v}, needs{n} {}

    friend std::ostream & operator<<( std::ostream& os_, const Node & p){
        os_ << p.value << " " << p.needs << " " << p.x << " " << p.y << " " << std::endl;
        return os_;
    }

    bool operator ==(const Node& a) const{
        return (a.value == this->value and a.needs == this->needs and a.x == this->x and a.y == this->y);
    }
};

struct Edge{

    int weight;
    int a;
    int b;

    Edge(int w = 0, int p = 0, int n = 0): weight{w}, a{p}, b{n} {}
};

struct Truck{
    int capacity;

    Truck(int c = 0): capacity{c} {}
};

class vrp_CW{

    private:
        Truck * truck;
        int sizeMat;
        std::vector<Node> vec;

        void readFile(std::string filename){
            int c = 0;
            int aux;
            Node * n;
            Edge * e;

            std::string line;

            std::ifstream file(filename);

            std::getline(file, line);
            truck = new Truck(stoi(line));

            std::getline(file, line);
            sizeMat = stoi(line);
            
            std::vector<std::string> aux_s;
            int cont = 0;
            while(std::getline(file, line) and line.compare("#") != 0){
                std::istringstream ss(line);
                for(std::string s; ss >> s;){
                    aux_s.push_back(s);
                }
                n = new Node(stoi(aux_s[0]), stoi(aux_s[1]));
                vec.push_back(*n);
                delete n;
                aux_s.clear();
            }
            
            while(std::getline(file, line)){
                if(cont < sizeMat){
                    std::istringstream ss(line);
                    for(std::string s; ss >> s;){
                        aux_s.push_back(s);
                    }
                    vec[cont].x = stoi(aux_s[1]);
                    vec[cont].y = stoi(aux_s[2]);
                    aux_s.clear();
                }
                cont++;
            }
        }

        class Route{
            
        public:
            struct saving{
                int a;
                int b;
                int value;

                saving(int a_, int b_, int value_) : a{a_}, b{b_}, value{value_}{}

                friend std::ostream & operator<<( std::ostream& os_, const saving & s){
                    os_ << s.a << " " << s.b << " " << s.value << std::endl;
                    return os_;
                }
            };

            std::vector<int> cost;
            std::vector<saving> savings;
            int totalCost = 0;
            //calculates the savings between 2 pairs of clients nodes with depot 
            saving calculateSavings(int a, int b, std::vector<Node> &_vec){
                int a_deposit = round(sqrt(pow(_vec[a].x -_vec[0].x,2) + pow(_vec[a].y - _vec[0].y,2)));
                int b_deposit = round(sqrt(pow(_vec[b].x -_vec[0].x,2) + pow(_vec[b].y - _vec[0].y,2)));
                int a_b = round(sqrt(pow(_vec[a].x -_vec[b].x,2) + pow(_vec[a].y - _vec[b].y,2)));
                int _savings = a_deposit + b_deposit - a_b;
                saving * s = new saving(a,b, _savings);
                savings.push_back(*s);
                delete s;

                return savings.back();
            }
            //initialize distance between each client node with depot
            void initialize(std::vector<Node> &_vec){
                int _cost;
                for(int i = 0; i < _vec.size(); i++){
                    _cost = 2 * round(sqrt(pow(_vec[i].x - _vec[0].x, 2) + pow(_vec[i].y - _vec[0].y, 2)));
                    cost.push_back(_cost);
                    totalCost += _cost;
                }
            }

            void printCost(){
                std::copy(cost.begin(), cost.end(),std::ostream_iterator<double>(std::cout, " "));
            }

            class Compare{
                public:
                    bool operator()(const saving& a, const saving& b) const{
                        return a.value < b.value;
                    }
            };
        };

    public:
        vrp_CW(std::string filename){
            auto start = std::chrono::steady_clock::now();
            readFile(filename);
            Route* r = new Route();
            
            std::priority_queue<Route::saving, std::vector<Route::saving>, Route::Compare> pq;
            //Calculate the savings and stores then in priority queue where thhe higher values will be in top
            r->initialize(vec);
            for(int i = 1; i < vec.size(); i++){
                for(int j = i +1; j < vec.size(); j++){
                    pq.push(r->calculateSavings(i,j,vec));
                }
            }

            int totalcost = 0, a, b;
            auto routes = generateRoutes(pq);
             //Calculate the total cost by visiting all created routes
            for(int i = 0; i < routes.size(); i++){
                for(int j = 0; j < routes[i].size() - 1; j++){
                    a = routes[i][j];
                    b = routes[i][j+1];
                    totalcost += round(sqrt(pow(vec[a].x - vec[b].x, 2) + pow(vec[a].y - vec[b].y, 2)));
                }
                //Adds to total cost the distance between first and last nodes, in a route, to depot
                totalcost += round(sqrt(pow(vec[routes[i][0]].x - vec[0].x, 2) + pow(vec[routes[i][0]].y - vec[0].y, 2)));
                totalcost += round(sqrt(pow(vec[routes[i][routes[i].size()-1]].x - vec[0].x, 2) + pow(vec[routes[i][routes[i].size()-1]].y - vec[0].y, 2)));
            }

            std::cout << routes.size() << " trucks " << totalcost << " Solution " << filename << std::endl;
            auto finish = std::chrono::steady_clock::now();
            auto time = finish - start;
            std::cout << std::chrono::duration <double, std::ratio<60>> (time).count() << std::endl;
           
        }

        std::vector<std::vector<int>>  generateRoutes(std::priority_queue<Route::saving, std::vector<Route::saving>, Route::Compare> & pq){
            std::unordered_set<int> help;
            std::vector<std::vector<int>> routes;
            std::vector<int> aux;
            int weight = 0, temp_weight;
             /*Determine the first position in the list which can be added to one of the end of the route, adding the node and removing it from the list
	        if the routte can't be expanded by the last form it chooses the first bound in the list to start a new route, and remove it from the list*/
            for(int i = 0; i < pq.size(); i++){
                auto s = pq.top();
                temp_weight = weight + vec[s.a].needs + vec[s.b].needs;
                if(temp_weight <= truck->capacity){
                    if(help.find(s.a) == help.end()){
                        help.insert(s.a);
                        aux.push_back(s.a);
                        weight += vec[s.a].needs;
                    }
                    if(help.find(s.b) == help.end()){
                        help.insert(s.b);
                        aux.push_back(s.b);
                        weight += vec[s.b].needs;
                    }
                    pq.pop();
                } else {
                    routes.push_back(aux);
                    aux.clear();
                    weight = 0;
                }
            }
            if(!aux.empty()){
                routes.push_back(aux);
            } 

            return routes;
        }
        ~vrp_CW() = default;
};

#endif
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
#include <unordered_map>

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
            private:
                int _capacity, _weight;
	            double _cost, _savings;

                void calculateSavings(){
                    double originalCost = 0;
		            double newCost = 0;
		            double tempcost =0;
		            Node * prev = nullptr;

                    for(Node n: nodes){ 
			            // Distance from Depot
			            tempcost = sqrt((n.x*n.x)+(n.y*n.y));
			            originalCost += (2.0*tempcost);

			            if(prev != nullptr){
				            // Distance from previous customer to this customer
				            double x = (prev->x - n.x);
				            double y = (prev->y - n.y);
				            newCost += sqrt((x*x)+(y*y));
                            prev = &n;
			            }else{
				            //If this is the first customer in the route, no change
				            newCost += tempcost;
                            prev = &n;
			            }
		            }
		            newCost += tempcost;
		            _cost = newCost;
		            _savings = originalCost - newCost;
                }

            public:
	            std::vector<Node> nodes;
                
                Route(int capacity){
                    _capacity = capacity;
                    _weight = 0;
                    _cost  = 0;
                    _savings = 0;
                }

                void addNode(Node a, bool order = false){
                    if(order){
                        nodes.insert(nodes.begin(), a);
                    }else{
                        nodes.push_back(a);
                    }

                    _weight += a.needs;

                    calculateSavings();
                }

                int getWeight(){
                    return _weight;
                }

                friend std::ostream & operator<<( std::ostream& os_, const Route p){
                    os_ << p.nodes[0].value << " " << p.nodes[1].value << " " << p._savings << std::endl;
                    return os_;
                }
                
                bool operator ==(const Route& r) const{
                    return r._capacity == this->_capacity or r._weight == this->_weight or r._cost == this->_cost or r._savings == this->_savings;
                }
                bool operator !=(const Route& r) const{
                    return !(*this == r);
                }

                class Compare{
                    public:
                        bool operator()(const Route a, const Route b) const{
                            return a._savings < b._savings;
                        }
                };
        };

    public:
        vrp_CW(std::string filename){
            std::priority_queue<Route, std::vector<Route>, Route::Compare> pq_routes;
            readFile(filename);
            for(int i = 0; i < vec.size(); i++){
                for(int j = i + 1; j < vec.size(); j ++){
                    Route r(truck->capacity);
                    r.addNode(vec[i]);
                    r.addNode(vec[j]);
                    pq_routes.push(r);
                }
            }

            std::vector<Route> routes;
            routes.push_back(pq_routes.top());
            pq_routes.pop();

            outerloop : for(int i = 0; i < routes.size(); i++){
                Route r = routes[0];
                Node n1 = r.nodes[0];
                Node n2 = r.nodes[r.nodes.size()-1];

                for(Route ro : routes){
                    Node nr1 = ro.nodes[0];
                    Node nr2 = ro.nodes[ro.nodes.size()-1];

                    bool edge = false;
                    for(int a = 0; a < 2; a++){
                        edge = !edge;
                        Node e1 = (!edge) ? n1 : n2;
					    Node e2 = (edge) ? n1 : n2;
                        
                        if(e1 == nr1 || e1 == nr1){
                            if(e2.needs + ro.getWeight() <= truck->capacity){
                                bool inNodes = false;
                                for(auto it = ro.nodes.begin(); it < ro.nodes.end(); it++){
                                    if(*it == e2){
                                        inNodes = true;
                                    }
                                }
                                if(!inNodes){
                                    bool isTaken = false;
                                    for(Route rr : routes){
                                        bool inNodes2 = false;
                                        for(auto it2 = ro.nodes.begin(); it2 < ro.nodes.end(); it2++){
                                            if(*it2 == e2){
                                                inNodes2 = true;
                                            }
                                        }
                                        if(!inNodes2){
                                            isTaken = true;
                                            break;
                                        }
                                    }
                                    if(isTaken){
                                        if(n1 == nr1){
                                            ro.addNode(e2, true);
                                        } else {
                                            ro.addNode(e2);
                                        }
                                    }
                                }
                                //abandoned.remove(e2);
                                std::priority_queue<Route, std::vector<Route>, Route::Compare> aux;
                                while(!pq_routes.empty()){
                                    if(r != pq_routes.top()){
                                        aux.push(pq_routes.top());
                                    }
                                    pq_routes.pop();
                                }
                                pq_routes = aux;
							    i--;
							    goto outerloop;
                            }
                        } 
                    }
                }
            }
        }

        ~vrp_CW() = default;
};

#endif
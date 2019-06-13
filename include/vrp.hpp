#ifndef _VRP_
#define _VRP_
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h> 
#include <iostream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <math.h>

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


class Vrp{
    private:
        Edge ** mat;
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
            
            mat = new Edge * [sizeMat];
            for(int i = 0; i < sizeMat; i++){
                mat[i] = new Edge[sizeMat];
            }
            for(int i = 0; i < sizeMat; i++){
                for(int k = i; k < sizeMat; k++){
                    if(i == k){
                        e = new Edge(0, vec[k].value, vec[k].value);
                        mat[i][k] = *e;
                        delete e;
                    } else {

                        aux = (int) round(sqrt(pow(vec[i].x - vec[k].x, 2) + pow(vec[i].y - vec[k].y, 2)));

                        e = new Edge(aux, vec[k].value, vec[i].value);
                        mat[k][i] = *e;
                        delete e;

                        e = new Edge(aux, vec[i].value, vec[k].value);
                        mat[i][k] = *e;
                        delete e;
                    }
                }
            }
            file.close();
        }
    public:
        Vrp(std::vector<std::string> files){
            for(int i = 0; i < files.size(); i++){
                readFile(files[i]);
            }
           
            for(int i = 0; i < files.size();i ++){
                readFile(files[i]);
                auto start = std::chrono::steady_clock::now();
                std::cout << VRP_NaiveSolution(0) << " solution " << files[i] << std::endl;
                auto finish = std::chrono::steady_clock::now();

                auto time = finish - start;
                std::cout << std::chrono::duration <double, std::ratio<60>> (time).count() << std::endl;
                if(i < files.size()-1){
                    clearVariables();
                }
            }

        }
        ~Vrp(){
            for(int i = 0; i < sizeMat; i++){
                delete [] mat[i];
            }
            delete [] mat;
            delete truck;
        }

        void clearVariables(){
            for(int i = 0; i < sizeMat; i++){
                delete [] mat[i];
            }
            delete [] mat;
            delete truck;
            vec.clear();
        }
        int VRP_NaiveSolution(int s){ 
            // store all vertex apart from source vertex and clusterizes  
            std::vector<std::vector<int>> vertex;
            std::vector<int> aux;
            int c = 0;
             /* cluterizes by respecting truck's capacity, note that vec is a vector containing all the nodes 
             containing the clientes and depot in our graph, we start first truck with 0 capacity and then 
             client by client this truck capacity is increased, when a truck in a route is full the 
             algorithm starts a new route*/
            for (int i = 0; i < sizeMat; i++){
                if(i != s){
                    c += vec[i].needs;
                    if( c <= truck->capacity){
                        aux.push_back(i);
                    } else {
                        c = 0;
                        vertex.push_back(aux);
                        aux.clear();
                        i--;
                    }
                }
            }
            if(!aux.empty()){
                vertex.push_back(aux);
            }
           
            std::cout << vertex.size() << " trucks ";
            
            // store minimum weight 
            int min_path = __INT_MAX__;
            // store current path cost 
            int current_pathcost = 0; 
            int total_pathcost = 0;
            int k;
            //for each cluster of clients finds the cost for that and add it to the total cost
            for(int y = 0; y < vertex.size(); y++){
                do { 
                
                //resets path in each iteration
                current_pathcost = 0; 
                min_path = __INT_MAX__;

                // compute current path cost of the cluster 
                    k = s; 
                    for (int i = 0; i < vertex[y].size(); i++) { 
                        current_pathcost += mat[k][vertex[y][i]].weight; 
                        k = vertex[y][i];
                    } 
                    current_pathcost += mat[k][s].weight;
                    
                min_path = std::min(min_path, current_pathcost);
                
                //makes all permutations in a cluster 
                } while (next_permutation(vertex[y].begin(), vertex[y].end()));

                total_pathcost += min_path;
            } 
  
        return total_pathcost;
        }
};

#endif
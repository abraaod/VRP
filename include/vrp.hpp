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

struct Node{
    int value;
    int needs;

    Node(int v = 0, int n = 0): value{v}, needs{n} {}

    friend std::ostream & operator<<( std::ostream& os_, const Node & p){
        os_ << p.value << " " << p.needs << std::endl;
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
        void readFile(std::string filename){
            int c = 0;
            int aux;
            std::vector<Node> vec;
            Node * n;
            Edge * e;

            std::string line;

            std::ifstream file(filename);

            std::getline(file, line);
            truck = new Truck(stoi(line));

            std::getline(file, line);
            sizeMat = stoi(line);
            
            while(std::getline(file, line) and line.compare("#") != 0){
                if(c == 0){
                    aux = stoi(line);
                    c++;
                } else {
                    n = new Node(aux, stoi(line));
                    vec.push_back(*n);
                    delete n;
                    c = 0;
                }
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
                        getline(file, line);
                        aux = stoi(line);

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
        Vrp(std::string filename){
            int tmp = 7;
            /*int graph[][tmp] = { { 0, 10, 20, 25, 25, 20, 10},
                       { 10, 0, 12, 20, 25, 30, 20},
 		       { 20, 12, 0, 10, 11, 22, 30},
		       { 25, 20, 10, 0, 2, 11, 25},
		       { 25, 25, 11, 2, 0, 10, 20},
		       { 20, 30, 22, 11, 10, 0, 12},
		       { 10, 20, 30, 25, 20, 12, 0}};
            mat = new Edge *[tmp];
            Edge * aux;
            for(int i =  0; i < tmp; i++){
                mat[i] = new Edge [tmp];
                for(int y = 0; y < tmp; y++){
                    aux = new Edge(graph[i][y]);
                    mat[i][y] = *aux;
                }
            }

            Truck truck(3);
            std::cout << VRP_NaiveSolution(mat, tmp, 0, truck) << std::endl;;*/
            readFile(filename);

            for(int i = 0; i < sizeMat; i++){
                for(int y = 0; y < sizeMat; y++){
                    std::cout << mat[i][y].a << " " << mat[i][y].b << "   ";
                }
                std::cout << std::endl;
            }

        }
        ~Vrp(){
            for(int i = 0; i < sizeMat; i++){
                delete [] mat[i];
            }
            delete [] mat;
            delete truck;
        }

        int VRP_NaiveSolution(Edge ** mat_, int V, int s, Truck truck){ 
            // store all vertex apart from source vertex and clusterizes  
            std::vector<std::vector<int>> vertex;
            std::vector<int> aux;

            for (int i = 0; i < V; i++){
                if(i != s){
                    for(int c = 0; c < truck.capacity; c++){
                        aux.push_back(i);
                        i++;
                    }
                    vertex.push_back(aux);
                    aux.clear();
                    i--;
                }
            }
           
            std::cout << vertex.size() << " tamanho" << std::endl;
            // store minimum weight 
            int min_path = __INT_MAX__;
            // store current path cost 
            int current_pathcost = 0; 
            int total_pathcost = 0;
            std::vector<int> path;
            std::vector<int> true_path;
            int k;
            for(int y = 0; y < vertex.size(); y++){
                do { 
                
                //resets path in each iteration
                current_pathcost = 0; 
                min_path = __INT_MAX__;

                // compute current path cost of the cluster 
                    k = s; 
                    for (int i = 0; i < vertex[y].size(); i++) { 
                        current_pathcost += mat_[k][vertex[y][i]].weight; 
                        k = vertex[y][i];
                        path.push_back(vertex[y][i]); 
                    } 
                    current_pathcost += mat_[k][s].weight;
                    path.push_back(0); 
  
                // update minimum 
                if(min_path > current_pathcost){
                    true_path = path;
                }
                min_path = std::min(min_path, current_pathcost);
                path.clear(); 
         
                } while (next_permutation(vertex[y].begin(), vertex[y].end()));

                total_pathcost += min_path;
                std::copy(true_path.begin(), true_path.end(), std::ostream_iterator<int>(std::cout, " "));
            } 
  
        return total_pathcost;
        }
};

#endif
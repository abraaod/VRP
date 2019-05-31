#ifndef _VRP_
#define _VRP_
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h> 
#include <iostream>
#include <algorithm>
#include <iterator>

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

class Vrp{
    private:
        Edge ** mat;
    public:
        Vrp(std::string filename){
            int tmp = 7;
            int graph[][tmp] = { { 0, 10, 20, 25, 25, 20, 10},
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
            std::cout << VRP_NaiveSolution(mat, tmp, 0, truck) << std::endl;;


        }
        ~Vrp() = default;

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
            int min_path = INT_MAX;
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
                min_path = INT_MAX;

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
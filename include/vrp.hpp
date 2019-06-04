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
            for(int i = 0; i < files.size();i ++){
                readFile(files[i]);
                auto start = std::chrono::steady_clock::now();
                std::cout << VRP_NaiveSolution(0) << " solution " << files[i] << std::endl;
                auto finish = std::chrono::steady_clock::now();

                auto time = finish - start;
                std::cout << std::chrono::duration <double, std::ratio<86400>> (time).count() << std::endl;
                if(i < files.size()-1){
                    clearVariables();
                }
            }
           /*for(int i = 0; i < sizeMat; i++){
                for(int y = 0; y < sizeMat; y++){
                    std::cout << mat[i][y].weight << "   ";
                }
                std::cout << std::endl;
            }*/

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
            /*for(int i = 0; i < vertex.size(); i++){
                for(int k = 0; k < vertex[i].size(); k++){
                    std::cout << vertex[i][k] << std::endl;
                }
                std::cout << std::endl;
            }*/
           
            //std::cout << vertex.size() << " tamanho" << std::endl;
            
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
                        current_pathcost += mat[k][vertex[y][i]].weight; 
                        k = vertex[y][i];
                        //path.push_back(vertex[y][i]); 
                    } 
                    current_pathcost += mat[k][s].weight;
                    //path.push_back(0); 
  
                // update minimum 
                /*if(min_path > current_pathcost){
                    true_path = path;
                }*/
                min_path = std::min(min_path, current_pathcost);
                //path.clear(); 
         
                } while (next_permutation(vertex[y].begin(), vertex[y].end()));

                total_pathcost += min_path;
                //std::copy(true_path.begin(), true_path.end(), std::ostream_iterator<int>(std::cout, " "));
            } 
  
        return total_pathcost;
        }
};

#endif
#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <queue>
#include <random>
#include <cstdlib>
#include <ctime>
#include <functional>

using namespace std;
int N;
// storing the 
int* x;
int* y;

// cities = genes = integer or index
// string of genes = chromosome = [ genes, genes, genes ]
// fitness score = the length of the path described by the genes (lesser = fitter)
double generate_distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((x1 - x2),2) + pow((y1 - y2),2));
}

struct Individual
{
    vector<int> genes; // chromosome
    double fitness = 0.0;

    void calculate_fitness()
    {
        for(int i = 0; i < genes.size() - 1; i++)
        {
            int first = genes[i];
            int second = genes[i + 1];
            fitness += generate_distance(x[first], y[first], x[second], y[second]);
        }
    }

    friend bool operator> (Individual& lhs, Individual& rhs)
    {
        return lhs.fitness > rhs.fitness;
    }
};

struct Comparator
{
    bool operator () (Individual& lhs, Individual& rhs)
    {
        return lhs > rhs;
    }
};

priority_queue<Individual, vector<Individual>, Comparator> current_population;
priority_queue<Individual, vector<Individual>, Comparator> future_population;

Individual find_best_individual()
{
    return current_population.top();
}
// one-point crossover
void create_child_from_parent(Individual& child, Individual& parent, int limit)
{
    bool found;
    int iter = 0;
    int nextGen;
    for(int i = limit + 1; i < N; i++)
    {
        found = true;
        while(found)
        {
            nextGen = parent.genes[iter];
            found = false;
            for(int j = 0; j < child.genes.size(); j++)
            {
                if(child.genes[j] == nextGen)
                {
                    found = true;
                    iter += 1;
                    break;
                }
            }
        }
        child.genes.push_back(nextGen);
    }
}

void mutate_genes(Individual& lhs)
{
    int first_rand = rand() % N;
    int second_rand = rand() % N;
    swap(lhs.genes[first_rand], lhs.genes[second_rand]);
}

void cross_genes(Individual lhs, Individual rhs)
{
    Individual first_child;
    Individual second_child;
    int limit = rand() % N;

    for(int i = 0; i <= limit; i++)
    {
        first_child.genes.push_back(lhs.genes[i]);
        second_child.genes.push_back(rhs.genes[i]);
    }
    create_child_from_parent(first_child, rhs, limit);
    create_child_from_parent(second_child, lhs, limit);

    mutate_genes(first_child);
    mutate_genes(second_child);

    first_child.calculate_fitness();
    second_child.calculate_fitness();

    future_population.push(first_child);
    future_population.push(second_child);
}
void reproduce()
{
    Individual first_individual;
    Individual second_individual;   
    int original_size = current_population.size();

    while(current_population.size() > original_size/2)
    {
        first_individual = current_population.top();
        current_population.pop();
        second_individual = current_population.top();
        current_population.pop();
        cross_genes(first_individual, second_individual);
        future_population.push(first_individual);
        future_population.push(second_individual);
    }

    //clear queue
    while(!current_population.empty()) 
    {
        current_population.pop();
    }
}

void initializeNextGeneration()
{
    current_population = future_population;
    //clear queue
    while(!future_population.empty())
    {
        future_population.pop();
    }
}

void print(Individual individual)
{
    cout << "Shortest path: " << individual.fitness << endl;
}

// generate cities coordinates in range (0 - 10)
void generate_coordinates()
{
    for(int i = 0; i < N; i++)
    {
        x[i] = rand() % 10;
        y[i] = rand() % 10;
        cout << "x: " << x[i] << " y: " << y[i] << endl;
    }
}

int main()
{
    srand(time(NULL)); // make it more random
    cin >> N;
    if(N > 100)
    {
        cout << "N should be <= 100" << endl;
        return 0;
    }
    x = new int[N];
    y = new int[N];
    generate_coordinates();
    vector<Individual> parents;
    int numberOfIndividuals = 10; // coordinates in (0 - 10)

    for(int i = 0; i < numberOfIndividuals; i++)
    {
        Individual current_individ;
        parents.push_back(current_individ);
        for(int j = 0; j < N; j++)
        {
            parents[i].genes.push_back(j);
        }
        unsigned seed = 0;
        shuffle(parents[i].genes.begin(), parents[i].genes.end(), default_random_engine(seed));
        parents[i].calculate_fitness();
        current_population.push(parents[i]);
    }

    int iter = 0;
    Individual best_individual;

    while(iter <= 5000)
    {
        if(iter == 1 || iter == 10 || iter == 50 || iter == 100)
        {
            cout << "Generation " << iter << ":";
            best_individual = find_best_individual();
            print(best_individual);
        }
        reproduce();
        initializeNextGeneration();
        iter += 1;
    }

    delete[] x;
    delete[] y;

    return 0;
}
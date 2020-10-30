#include <iostream>
#include <random>
#include <cmath>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>

#include "TextTable.h"

double min = -2;
double max = 2;
int INDIVIDAUL_COUNT = 4;
int generation_num = 0;

// declared for creation of generation 0
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(min, max);
TextTable t;
// declared here to avoid creation every time using move_with possibility
std::random_device rd2;
std::mt19937 gen2(rd2());
std::uniform_real_distribution<double> dis2(0, 1);

double get_func_value(double x, double y) {
    return cos(x) * cos(y) * exp(y / 2);
}

void Crossover(std::vector<std::pair<double, double>>& individuals) {
    // sorting individuals
    std::sort(begin(individuals), end(individuals),
              [](const std::pair<double, double>& lhs, const std::pair<double, double>& rhs) {
        return get_func_value(lhs.first, lhs.second) > get_func_value(rhs.first, rhs.second);
    });

    // making new_generation
    auto best_individ = individuals[0];
    individuals[0].first = individuals[1].first;
    individuals[1].first = best_individ.first;
    individuals[3].second = individuals[2].second;
    individuals[3].first = best_individ.first;
    individuals[2].second = best_individ.second;
}

void Mutation(std::vector<std::pair<double, double>>& individuals) {
    for (auto& [x,y] : individuals) {
        // posibility mutation
        double pos = dis(gen);
        if (pos < 0.25) {
            // do mutation
            x = x * dis(gen);
            x = x < -2 || x > 2 ? std::fmod(x, 2) : x;
            y = y * dis(gen);
            y = y < -2 || y > 2 ? std::fmod(y, 2) : y;
        }
    }
}

void FillTable(std::vector<std::pair<double, double>>& individuals) {
    double average_value, max_value, cur_value;
    max_value = get_func_value(individuals[0].first, individuals[0].second);
    cur_value = max_value;
    for(int i = 0; i < INDIVIDAUL_COUNT; ++i) {
        if (i == 0) {
            t.add(std::to_string(generation_num++));
        } else {
            t.add("\0");
        }
        auto [x, y] = individuals[i];
        cur_value = get_func_value(x, y);
        t.add(std::to_string(x));
        t.add(std::to_string(y));
        t.add(std::to_string(cur_value));
        if(i < INDIVIDAUL_COUNT - 1) {
            t.add("\0"); t.add("\0");
            t.endOfRow();
        }
        if (cur_value > max_value) {
            max_value = cur_value;
        }
        average_value += cur_value;
    }
    average_value /= INDIVIDAUL_COUNT;
    t.add(std::to_string(max_value));
    t.add(std::to_string(average_value));
    t.endOfRow();
}


void Genetic_Search() {
    // table creation
    t.add("Generation num"); t.add("X"); t.add("Y"); t.add("FIT"); t.add("Max Result"); t.add("Avg Result");
    t.endOfRow();

    // creating first 0 generation and calculating fit-function
    std::vector<std::pair<double, double>> individuals;
    for (int i =0; i < INDIVIDAUL_COUNT; ++i) {
        auto point = std::make_pair(dis(gen), dis(gen));
        individuals.push_back(std::move(point));
    }
    FillTable(individuals);

    while (generation_num < 11) {
        Crossover(individuals);
        Mutation(individuals);
        FillTable(individuals);
    }

    std::sort(begin(individuals), end(individuals),
              [](const std::pair<double, double>& lhs, const std::pair<double, double>& rhs) {
                  return get_func_value(lhs.first, lhs.second) > get_func_value(rhs.first, rhs.second);
              });
    auto [x ,y] = individuals[0];
    std::cout << "Result: (x;y)=" << "(" << x << ";" << y << ")   "
    << "f(x,y)=" << get_func_value(x,y) << "\n";
}

int main() {
    Genetic_Search();
    std::cout << t << std::endl;
    return 0;
}

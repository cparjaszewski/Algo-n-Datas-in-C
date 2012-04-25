/**
 * Plansza 3000x3000 z samymi 9
 */
#include <iostream>

const size_t W = 3000;
const size_t K = 3000;

int main( ) {
    std::cout << W << " " << K << "\n";
    for (size_t i = 0; i < W; i++) {
	for (size_t j = 0; j < K; j++)
	    std::cout << "9";
	std::cout << "\n";
    }
}

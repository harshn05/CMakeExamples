#include <iostream>
#include <armadillo>

using namespace std;

int main()
{
    arma::arma_rng::set_seed_random();
    auto A = arma::randu(5,5);
    auto B = arma::randu(5,5);
    std::cout<<B*arma::inv(A)<<std::endl;
    return 0;
}
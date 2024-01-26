#include <iostream>
#include <vector>
#include <mpi.h>
#include <chrono>
#include <random>

int main(int argc, char *argv[]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist_mil(-100, 100);
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int n = 1e3; n <= 1e7; n *= 10) {
        std::vector <int> vec1;
        std::vector <int> vec2;

        chrono::time_point <chrono::system_clock, chrono::nanoseconds> start_time;
        if (rank == 0) {
            for (auto i = 0; i < n; i++) {
                vec1.push_back(dist_mil(gen));
                vec2.push_back(dist_mil(gen));
            }
            start_time = chrono::high_resolution_clock::now();
        }

        int local_size = n / size;
        std::vector <int> local_vec1(local_size);
        std::vector <int> local_vec2(local_size);
        MPI_Scatter(vec1.data(), local_size, MPI_INT, local_vec1.data(), local_size,
                    MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(vec2.data(), local_size, MPI_INT, local_vec2.data(), local_size,
                    MPI_INT, 0, MPI_COMM_WORLD);

        int local_res = 0;
        for (int i = 0; i < local_size; ++i) {
            local_res += local_vec1[i] * local_vec2[i];
        }

        int global_res;
        MPI_Reduce(&local_res, &global_res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            if (n % size != 0) {
                for (int i = size * local_size; i < n; i++) {
                    global_res += vec1[i] * vec2[i];
                }
            }
            auto end_time = chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end_time - start_time;
            std::cout << n << " " << duration.count() << std::endl;
        }
    }
    MPI_Finalize();
    return 0;
}

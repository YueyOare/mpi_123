#include <iostream>
#include <string>
#include <mpi.h>
#include <chrono>


int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (int n = 1024*1024; n >= 1; n /= 2) {
        std::chrono::time_point <chrono::system_clock, chrono::nanoseconds> start_time;
        if (rank == 0) {
            start_time = chrono::high_resolution_clock::now();
        }
        for (int count = 1; count <= 1000; count++) {
            if (rank == 0) {
                string send0(n, '0');
                MPI_Rsend(send0.data(), static_cast<int>(send0.size()), MPI_CHAR, 1, 0, MPI_COMM_WORLD);

                string recv0(n, ' ');
                MPI_Recv(const_cast<char *>(recv0.data()), static_cast<int>(recv0.size()), MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            if (rank == 1) {
                string recv1(n, ' ');
                MPI_Recv(const_cast<char *>(recv1.data()), static_cast<int>(recv1.size()), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                string send1(n, '1');
                MPI_Rsend(send1.data(), static_cast<int>(send1.size()), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            }
        }
        if (rank == 0) {
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end_time - start_time;
            std::cout << n << " " << duration.count() << std::endl;
        }
    }
    MPI_Finalize();
    return 0;
}

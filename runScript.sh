#!/bin/bash

module add openmpi

task_array=($(seq 1 1 3))

for n in "${task_array[@]}"; do
    mpic++ "task$n.cpp" -o "Task$n"
done

for n in "${task_array[@]}"; do
    touch "Job$n.sh"
    mkdir "TaskOut$n"
    echo "#!/bin/sh" > "Job$n.sh"
    echo "module add openmpi" >> "Job$n.sh"
    echo "mpirun Task$n" >> "Job$n.sh"
done

for task_num in "${task_array[@]}"; do
  for num_processes in {1..128}; do
   echo "task $task_num n $num_processes"
   sbatch --output=./"TaskOut$task_num"/"num_processes_$num_processes" -n "$num_processes" "Job$task_num.sh"
  done
done

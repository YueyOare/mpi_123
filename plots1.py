import glob

import matplotlib.pyplot as plt
import pandas as pd

files = glob.glob('TaskOut2/num_processes_*')

data = []

for file in files:
    if file == files[0]:
        one_process = pd.read_csv(file, delimiter=' ', header=None, names=['Vector Size', 'Execution Time'])
        num_processes = int(file.split('_')[-1])
        one_process['Num Processes'] = num_processes
    else:
        df = pd.read_csv(file, delimiter=' ', header=None, names=['Vector Size', 'Execution Time'])
        num_processes = int(file.split('_')[-1])
        df['Num Processes'] = num_processes
        df['Speedup'] = one_process['Execution Time'].values / df['Execution Time']
        if num_processes <= 20:
            data.append(df)

df_combined = pd.concat(data, ignore_index=True)
df_combined = df_combined.sort_values(by=['Vector Size', 'Num Processes'])
grouped_data = df_combined.groupby('Vector Size')

# Построение графика
plt.figure(figsize=(10, 6))
for vector_size, group in grouped_data:
    plt.plot(group['Num Processes'], group['Speedup'], label=f'{vector_size}')

# plt.xticks(df_combined['Num Processes'].unique())
plt.xlabel('Num Processes')
plt.ylabel('Speedup')
plt.title('Speedup vs Number of Processes for Different Vector Size')
plt.legend()
plt.savefig('task2_plot2.png')
plt.show()

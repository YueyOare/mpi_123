import matplotlib.pyplot as plt
import pandas as pd

# Чтение данных из файла 'output'
df_output = pd.read_csv('TaskOut3/output', delimiter=' ', header=None, names=['Message Length', 'Execution Time'])
df_output = df_output.sort_values(by='Message Length')

# Построение графика
plt.figure(figsize=(10, 6))
plt.plot(df_output['Message Length'], df_output['Execution Time'], marker='o')

# plt.xscale('log', base=2)
plt.xticks(df_output['Message Length'], df_output['Message Length'], rotation='vertical')
plt.xlabel('Message Length')
plt.ylabel('Execution Time')
plt.title('Execution Time vs Message Length (in Bytes)')
plt.savefig('task3_plot1.png')
plt.show()

import matplotlib.pyplot as plt
import pandas as pd

# Função para plotar os gráficos
def plot_graphs(file_path):
    # Ler o arquivo como um DataFrame
    data = pd.read_csv(file_path, sep='\t', header=None, names=['time', 'xref', 'yref'])
    
    # Gráfico 1: xref e yref ao longo do tempo
    plt.figure(figsize=(10, 5))
    plt.plot(data['time'], data['xref'], label='xref (m)', marker='o')
    plt.plot(data['time'], data['yref'], label='yref (m)', marker='x')
    plt.title('xref e yref ao longo do tempo')
    plt.xlabel('Tempo (ms)')
    plt.ylabel('Valores (m)')
    plt.legend()
    plt.grid(True)
    plt.show()

    # Gráfico 2: yref em função de xref
    plt.figure(figsize=(10, 5))
    plt.plot(data['xref'], data['yref'], label='yref em função de xref', marker='.')
    plt.title('yref em função de xref')
    plt.xlabel('xref (m)')
    plt.ylabel('yref (m)')
    plt.grid(True)
    plt.legend()
    plt.show()

# Substitua 'data.txt' pelo caminho do arquivo contendo os dados
file_path = 'lab4_ref.txt'
plot_graphs(file_path)

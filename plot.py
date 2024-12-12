import matplotlib.pyplot as plt
import pandas as pd

# Função para carregar os dados do arquivo gerado pela simulação
def load_simulation_data(file_path: str):
    """
    Carrega os dados da simulação a partir de um arquivo.
    
    :param file_path: Caminho do arquivo contendo os dados da simulação.
    :return: DataFrame contendo tempo (t), u(t) e y(t).
    """
    columns = ["t", "u1", "u2", "y1", "y2", "y3"]
    data = []
    with open(file_path, 'r') as f:
        for line in f.readlines():
            parts = line.strip().split('\t')
            t = float(parts[0])
            u = list(map(float, parts[1].strip("[]").split()))
            y = list(map(float, parts[2].strip("[]").split()))
            data.append([t] + u + y)
    return pd.DataFrame(data, columns=columns)

# Função para plotar os gráficos
def plot_simulation_results(data: pd.DataFrame):
    """
    Plota os resultados da simulação.
    
    :param data: DataFrame contendo os resultados da simulação.
    """
    plt.figure(figsize=(12, 8))
    
    # Gráfico de u(t)
    plt.subplot(3, 1, 1)
    plt.plot(data["t"], data["u1"], label="u1 (v)", marker='o')
    plt.plot(data["t"], data["u2"], label="u2 (ω)", marker='o')
    plt.title("Comandos de entrada: u(t)")
    plt.xlabel("Tempo (t)")
    plt.ylabel("Valores de u(t)")
    plt.legend()
    plt.grid()

    # Gráfico de y(t)
    plt.subplot(3, 1, 2)
    plt.plot(data["t"], data["y1"], label="y1 (x1)", marker='o')
    plt.plot(data["t"], data["y2"], label="y2 (x2)", marker='o')
    plt.plot(data["t"], data["y3"], label="y3 (θ)", marker='o')
    plt.title("Saídas: y(t)")
    plt.xlabel("Tempo (t)")
    plt.ylabel("Valores de y(t)")
    plt.legend()
    plt.grid()

    # Trajetória x1(t) vs x2(t)
    plt.subplot(3, 1, 3)
    plt.plot(data["y1"], data["y2"], label="Trajetória", marker='o')
    plt.title("Trajetória do robô: x1(t) vs x2(t)")
    plt.xlabel("x1 (posição x)")
    plt.ylabel("x2 (posição y)")
    plt.legend()
    plt.grid()
    
    plt.tight_layout()
    plt.show()

# Exemplo de uso:
# Salve os resultados da simulação em um arquivo como "simulation_results.txt"
# Altere o caminho abaixo para o local do arquivo gerado pela simulação
file_path = "output_lab3.txt"
# Aqui seria necessário carregar os dados de teste simulados.
# A execução real deste código exigiria que você forneça um arquivo correto.
# Quando estiver pronto, basta substituir o caminho pelo local correto do arquivo.
simulation_data = load_simulation_data(file_path)
plot_simulation_results(simulation_data)

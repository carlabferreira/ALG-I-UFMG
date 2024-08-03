function gerarGrafoPlanarConexo()
    % Solicita ao usuário para inserir o número de vértices e arestas
    N = input('Digite o número de vértices (N): ');
    M = input('Digite o número de arestas (M): ');
    
    % Inicializa matriz de arestas
    arestas = zeros(M, 5);
    
    % Verifica se o número de arestas é válido para um grafo planar
    if M > 3 * N - 6
        error('Número de arestas é muito grande para um grafo planar.');
    end

    % Lista de adjacência para verificar conexões
    adjacencia = cell(N, 1);

    % Coordenadas dos vértices distribuídas em um círculo
    theta = linspace(0, 2*pi, N+1);
    theta(end) = []; % Remove o último ponto, pois é igual ao primeiro
    coordenadas = [cos(theta)', sin(theta)'];

    % Primeiro vértice
    num_arestas = 0;

    % Cria uma conexão inicial entre dois vértices
    for v = 2:N
        % Conecta novo vértice a um vértice já existente (planaridade)
        vertice_conectado = randi([1, v-1]);

        % Adiciona aresta entre vertice_conectado e v
        num_arestas = num_arestas + 1;
        arestas(num_arestas, :) = [vertice_conectado, v, randi(1e8), randi(1e5), randi(1e5)];

        % Atualiza lista de adjacência
        adjacencia{vertice_conectado} = [adjacencia{vertice_conectado}, v];
        adjacencia{v} = [adjacencia{v}, vertice_conectado];

        % Para adicionar arestas adicionais, escolha dois vértices não adjacentes
        while num_arestas < M
            % Escolhe dois vértices aleatórios
            v1 = randi(N);
            v2 = randi(N);

            % Verifica se os vértices são diferentes e não são adjacentes
            if v1 ~= v2 && isempty(find(adjacencia{v1} == v2, 1))
                % Adiciona a nova aresta
                num_arestas = num_arestas + 1;
                arestas(num_arestas, :) = [v1, v2, randi(1e8), randi(1e5), randi(1e5)];

                % Atualiza a lista de adjacência
                adjacencia{v1} = [adjacencia{v1}, v2];
                adjacencia{v2} = [adjacencia{v2}, v1];
            end
        end
    end

    % Salva a organização do grafo em um arquivo
    fileID = fopen('saida.txt', 'w');
    fprintf(fileID, 'u\tv\ta\tl\tc\n');
    for i = 1:M
        fprintf(fileID, '%d\t%d\t%d\t%d\t%d\n', arestas(i, :));
    end
    fclose(fileID);

    % Plot do grafo
    figure;
    hold on;
    axis equal;
    axis off;
    
    % Desenha arestas
    for i = 1:M
        u = arestas(i, 1);
        v = arestas(i, 2);
        plot([coordenadas(u, 1), coordenadas(v, 1)], [coordenadas(u, 2), coordenadas(v, 2)], 'k-', 'LineWidth', 1.5);
    end
    
    % Desenha vértices
    for i = 1:N
        plot(coordenadas(i, 1), coordenadas(i, 2), 'o', 'MarkerSize', 8, 'MarkerFaceColor', 'm', 'MarkerEdgeColor', 'k');
        text(coordenadas(i, 1), coordenadas(i, 2), sprintf(' %d', i), 'FontWeight', 'bold', 'FontSize', 10);
    end
    
    title('Grafo Planar Conexo');
    hold off;
end

% Exemplo de uso
gerarGrafoPlanarConexo();

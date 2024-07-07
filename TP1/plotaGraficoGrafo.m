% Limpar variáveis e fechar figuras anteriores
clear;
close all;

% Abrir o arquivo de entrada
fid = fopen('entrada.txt', 'r');

% Verificar se o arquivo foi aberto corretamente
if fid == -1
    error('Erro ao abrir o arquivo.');
end

try
    % Ler N e M
    NM = fscanf(fid, '%d', 2);
    if numel(NM) ~= 2
        error('Formato inválido para N e M.');
    end
    N = NM(1);
    M = NM(2);

    % Inicializar matrizes e células para armazenar coordenadas e vizinhos
    vertices_coords = zeros(N, 2);
    vizinhos = cell(N, 1);

    % Ler as coordenadas dos vértices e seus vizinhos
    for i = 1:N
        % Ler coordenadas do vértice
        coords = fscanf(fid, '%f', 2);
        if numel(coords) ~= 2
            error('Formato inválido para coordenadas do vértice %d.', i);
        end
        vertices_coords(i, :) = coords';

        % Ler quantidade de vizinhos
        num_vizinhos = fscanf(fid, '%d', 1);

        % Ler vizinhos do vértice
        viz = fscanf(fid, '%d', num_vizinhos);
        if numel(viz) ~= num_vizinhos
            error('Formato inválido para vizinhos do vértice %d.', i);
        end
        vizinhos{i} = viz';
    end

    % Fechar o arquivo
    fclose(fid);

    % Tamanho do círculo do vértice
    circulo_tamanho = 6;
    % Margem ao redor dos vértices
    margem = 0.5;

    % Plotar os vértices
    figure;
    hold on;
    for i = 1:N
        % Coordenadas do vértice i
        x = vertices_coords(i, 1);
        y = vertices_coords(i, 2);

        % Plotar o vértice i com cor roxa
        plot(x, y, 'o', 'MarkerFaceColor', 'c', 'MarkerSize', circulo_tamanho);

        % Anotar o número do vértice em negrito
        text(x, y, num2str(i), 'FontWeight', 'bold', 'VerticalAlignment','bottom', ...
            'HorizontalAlignment','right');

        % Conectar vértice i com seus vizinhos
        for j = 1:length(vizinhos{i})
            vizinho = vizinhos{i}(j);
            % Coordenadas do vizinho
            x_vizinho = vertices_coords(vizinho, 1);
            y_vizinho = vertices_coords(vizinho, 2);

            % Plotar a aresta entre vértice i e seu vizinho
            plot([x, x_vizinho], [y, y_vizinho], 'k');
        end
    end

    % Ajustes de plotagem
    axis equal;
    xlim([min(vertices_coords(:,1)) - margem, max(vertices_coords(:,1)) + margem]);
    ylim([min(vertices_coords(:,2)) - margem, max(vertices_coords(:,2)) + margem]);
    title('Representação do Grafo');
    xlabel('Eixo X');
    ylabel('Eixo Y');
    grid on;
    hold off;

catch ME
    % Fechar o arquivo em caso de erro
    fclose(fid);
    rethrow(ME);
end

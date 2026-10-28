function Visualize_Lighthouses(P, d, N)

side_num = 60;

figure;
scatter3(P(:, 1), P(:, 2), P(:, 3), "blue", "o");
hold on;

for i = 1:N
    text(P(i, 1), P(i, 2), P(i, 3), sprintf('%d', i-1), ...
        'VerticalAlignment', 'bottom', ...
        'HorizontalAlignment', 'right', ...
        'FontSize', 12, ...
        'Color', 'black');
end


% for i=1:N
%     [x, y, z] = sphere();
%     x = x*d(i) + P(i, 1);
%     y = y*d(i) + P(i, 2);
%     z = z*d(i) + P(i, 3);
%     surf(x, y, z, "FaceAlpha", 0.1, "EdgeColor", "none", "FaceColor", "yellow");
% end

set(gca, 'YDir', 'reverse');

% legend(["Calculated position", "Ground truth position", "Lighthouses", "Distances to lighthouses"])

title('3D visualization');
% subtitle(sprintf("Distance measurement accuracy: %0.2fm \n Position measurement accuracy: %0.2fm", distance_accuracy, accuracy));
xlabel('x');
ylabel('y');
zlabel("z");
axis equal; % Ensures the circle looks round, not elliptical


end
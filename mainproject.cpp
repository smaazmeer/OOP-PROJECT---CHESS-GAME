#include <SFML/Graphics.hpp>
#include "Game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 640), "SFML Chess Game");
    Game game;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Mouse click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / 80;
                int y = event.mouseButton.y / 80;
                if (!game.isAwaitingPromotion())
                    game.handleClick(x, y);
            }

            // Key input for promotion
            if (event.type == sf::Event::TextEntered && game.isAwaitingPromotion()) {
                char c = static_cast<char>(event.text.unicode);
                if (c == 'q' || c == 'Q' || c == 'r' || c == 'R' || c == 'b' || c == 'B' || c == 'n' || c == 'N') {
                    game.handlePromotionInput(c);
                }
            }
        }

        window.clear();
        game.drawBoard(window);
        game.drawHighlights(window);
        game.drawPieces(window);
        game.drawPromotion(window);
        window.display();
    }

    return 0;
}

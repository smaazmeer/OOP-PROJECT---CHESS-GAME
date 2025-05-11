#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Game.h"
#include <iostream>
#include <sstream>

enum GameState { MENU, MODE_SELECT, TIMER_SELECT, PLAY };
enum PlayerMode { NONE, VS_PLAYER, VS_AI };

std::pair<sf::RectangleShape, sf::Text> createButton(
    sf::Vector2f size,
    sf::Vector2f pos,
    const std::string& label,
    unsigned int fontSize,
    const sf::Font& font
) {
    sf::RectangleShape button(size);
    button.setPosition(pos);
    button.setFillColor(sf::Color(139, 69, 19));
    button.setOutlineThickness(4);
    button.setOutlineColor(sf::Color(192, 192, 192));

    sf::Text text;
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(fontSize);
    text.setFillColor(sf::Color::White);

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
    text.setPosition(pos.x + size.x / 2, pos.y + size.y / 2);

    return std::make_pair(button, text);
}

std::string formatTime(sf::Time time) {
    int seconds = static_cast<int>(time.asSeconds());
    int mins = seconds / 60;
    int secs = seconds % 60;
    std::ostringstream ss;
    ss << (mins < 10 ? "0" : "") << mins << ":" << (secs < 10 ? "0" : "") << secs;
    return ss.str();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 640), "SFML Chess Game");
    window.setFramerateLimit(60);

    GameState state = MENU;
    PlayerMode mode = NONE;
    Game game;

    sf::Font font;
    font.loadFromFile("arial.ttf");

    // Sound loading
    sf::SoundBuffer moveBuffer, captureBuffer, checkBuffer, checkmateBuffer;
    moveBuffer.loadFromFile("move.wav");
    captureBuffer.loadFromFile("capture.wav");
    checkBuffer.loadFromFile("check.wav");
    checkmateBuffer.loadFromFile("checkmate.wav");

    sf::Sound moveSound(moveBuffer);
    sf::Sound captureSound(captureBuffer);
    sf::Sound checkSound(checkBuffer);
    sf::Sound checkmateSound(checkmateBuffer);

    // Menu background
    sf::Texture menuTexture;
    menuTexture.loadFromFile("menu_background.png");
    sf::Sprite menuBackground(menuTexture);
    menuBackground.setScale(800.0f / menuTexture.getSize().x, 640.0f / menuTexture.getSize().y);

    // Buttons
    auto playPair = createButton({250, 60}, {275, 400}, "PLAY", 28, font);
    auto twoPair = createButton({250, 60}, {275, 200}, "2 PLAYER", 24, font);
    auto aiPair = createButton({250, 60}, {275, 300}, "PLAY VS AI", 24, font);
    auto timer10Pair = createButton({250, 60}, {275, 200}, "10 MIN TIMER", 24, font);
    auto timer5Pair = createButton({250, 60}, {275, 300}, "5 MIN TIMER", 24, font);
    auto backPair = createButton({100, 40}, {20, 580}, "BACK", 20, font);

    // Timers
    sf::Time player1Time, player2Time;
    sf::Clock turnClock;

    sf::Text timerWhite, timerBlack, timeoutText;
    timerWhite.setFont(font);
    timerBlack.setFont(font);
    timeoutText.setFont(font);
    timerWhite.setCharacterSize(24);
    timerBlack.setCharacterSize(24);
    timeoutText.setCharacterSize(36);
    timeoutText.setFillColor(sf::Color::Red);
    timeoutText.setPosition(230, 300);

    timerWhite.setPosition(660, 600);
    timerBlack.setPosition(660, 10);

    sf::RectangleShape timerWhiteBox({140, 35});
    timerWhiteBox.setFillColor(sf::Color(200, 200, 200, 230));
    timerWhiteBox.setPosition(650, 595);
    timerWhite.setFillColor(sf::Color::Black);

    sf::RectangleShape timerBlackBox({140, 35});
    timerBlackBox.setFillColor(sf::Color(30, 30, 30, 230));
    timerBlackBox.setPosition(650, 5);
    timerBlack.setFillColor(sf::Color::White);

    // Promotion popup
    sf::RectangleShape promotionBox({450, 150});
    promotionBox.setFillColor(sf::Color(50, 50, 50, 230));
    promotionBox.setOutlineColor(sf::Color::White);
    promotionBox.setOutlineThickness(3);
    promotionBox.setPosition(100, 245);

    sf::Text promotionTitle("Promote Pawn", font, 30);
    promotionTitle.setFillColor(sf::Color::White);
    promotionTitle.setPosition(210, 260);

    sf::Text promotionKeys("Q = Queen   R = Rook   B = Bishop   K = Knight", font, 20);
    promotionKeys.setFillColor(sf::Color::White);
    promotionKeys.setPosition(115, 310);
    
    //winner textbox
    sf::Text winnerText;
    winnerText.setFont(font);
    winnerText.setCharacterSize(40);
    winnerText.setFillColor(sf::Color::Green);
    winnerText.setPosition(180, 250);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            int mx = event.mouseButton.x;
            int my = event.mouseButton.y;

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (state == MENU && playPair.first.getGlobalBounds().contains(mx, my)) {
                    state = MODE_SELECT;
                } else if (state == MODE_SELECT) {
                    if (twoPair.first.getGlobalBounds().contains(mx, my)) {
                        mode = VS_PLAYER;
                        state = TIMER_SELECT;
                    } else if (aiPair.first.getGlobalBounds().contains(mx, my)) {
                        mode = VS_AI;
                        state = TIMER_SELECT;
                    }
                } else if (state == TIMER_SELECT) {
                    if (timer10Pair.first.getGlobalBounds().contains(mx, my)) {
                        player1Time = sf::seconds(600);
                        player2Time = sf::seconds(600);
                        turnClock.restart();
                        state = PLAY;
                    } else if (timer5Pair.first.getGlobalBounds().contains(mx, my)) {
                        player1Time = sf::seconds(300);
                        player2Time = sf::seconds(300);
                        turnClock.restart();
                        state = PLAY;
                    } else if (backPair.first.getGlobalBounds().contains(mx, my)) {
                        state = MODE_SELECT;
                    }
                } else if (state == PLAY && !game.isAwaitingPromotion() && !game.isCheckmate(WHITE) && !game.isCheckmate(BLACK)) {
                    int before = 0, after = 0;
                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            if (game.getPiece(x, y)) before++;

                    bool moveMade = game.handleClick(mx / 80, my / 80);

                    for (int y = 0; y < 8; y++)
                        for (int x = 0; x < 8; x++)
                            if (game.getPiece(x, y)) after++;

                    if (moveMade) {
                        if (after < before)
                            captureSound.play();
                        else
                            moveSound.play();

                       if (game.isGameOver()) {
    checkmateSound.play();
    if (mode == VS_AI) {
        winnerText.setString(game.getCurrentTurn() == WHITE ? "AI Wins!" : "Player Wins!");
    } else {
        winnerText.setString(game.getCurrentTurn() == WHITE ? "Black Wins!" : "White Wins!");
    }
}
						else if (game.isKingInCheck(game.getCurrentTurn())) {
                        checkSound.play();				
                            }


                        turnClock.restart();
                        if (mode == VS_AI && game.getCurrentTurn() == BLACK && !game.isGameOver()) {
                        game.makeAIMove();
                        turnClock.restart();
}

                    }
                }
            }

            if (event.type == sf::Event::TextEntered && game.isAwaitingPromotion()) {
                char c = static_cast<char>(event.text.unicode);
                if (c == 'q' || c == 'Q' || c == 'r' || c == 'R' || c == 'b' || c == 'B' || c == 'n' || c == 'N' || c == 'k' || c == 'K') {
                    if (c == 'k' || c == 'K') c = 'N';
                    game.handlePromotionInput(c);
                    turnClock.restart();
                }
            }
        }

        // Timer logic
        if (state == PLAY && !game.isCheckmate(WHITE) && !game.isCheckmate(BLACK)) {
            sf::Time elapsed = turnClock.getElapsedTime();
            if (!game.isAwaitingPromotion()) {
                if (game.getCurrentTurn() == WHITE)
                    player1Time -= elapsed;
                else
                    player2Time -= elapsed;
            }
            turnClock.restart();

           if (player1Time.asSeconds() <= 0) {
    winnerText.setString(mode == VS_AI ? "AI Wins by Timeout!" : "Player 2 Wins by Timeout!");
    checkmateSound.play();
    game.setGameOver(true);
}

          if (player2Time.asSeconds() <= 0) {
    winnerText.setString(mode == VS_AI ? "Player Wins by Timeout!" : "Player 1 Wins by Timeout!");
    checkmateSound.play();
    game.setGameOver(true);
}


            timerWhite.setString("P1: " + formatTime(player1Time));
            timerBlack.setString("P2: " + formatTime(player2Time));
        }

        window.clear();
        window.draw(menuBackground);

        if (state == MENU) {
            window.draw(playPair.first); window.draw(playPair.second);
        } else if (state == MODE_SELECT) {
            window.draw(twoPair.first); window.draw(twoPair.second);
            window.draw(aiPair.first);  window.draw(aiPair.second);
        } else if (state == TIMER_SELECT) {
            window.draw(timer10Pair.first); window.draw(timer10Pair.second);
            window.draw(timer5Pair.first);  window.draw(timer5Pair.second);
            window.draw(backPair.first);    window.draw(backPair.second);
        } else if (state == PLAY) {
            game.drawBoard(window);
            game.drawHighlights(window);
            game.drawPieces(window);
			game.drawMoveHistory(window);

            window.draw(timerBlackBox); window.draw(timerBlack);
            window.draw(timerWhiteBox); window.draw(timerWhite);

            if (game.isAwaitingPromotion()) {
                window.draw(promotionBox);
                window.draw(promotionTitle);
                window.draw(promotionKeys);
            }

            if (game.isGameOver()) {
    window.draw(winnerText); // ? use only this
}

        }

        window.display();
    }

    return 0;
}

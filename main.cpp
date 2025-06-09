#include <SFML/Graphics.hpp>
#include <iostream>

sf::Vector2f getPixelPosition(const sf::Vector2i &pos, const sf::Vector2u &blockSize) {
	return sf::Vector2f(static_cast<float>(pos.x * blockSize.x), static_cast<float>(pos.y * blockSize.y));
}

int main() {
	const int fieldWidth = 6;
	const int fieldHeight = 10;
	bool field[fieldWidth][fieldHeight] = {};
	for (int y = 3; y < fieldHeight; ++y) {
		for (int k = 1; k <= 3; ++k) {
			field[rand() % fieldWidth][y] = true;
		}
	}
	sf::Texture blockTexture;
	blockTexture.loadFromFile("block.png");
	sf::Vector2u blockSize(blockTexture.getSize());
	const int windowWidth = blockSize.x * fieldWidth;
	const int windowHeight = blockSize.y * fieldHeight;
	sf::VideoMode mode(windowWidth, windowHeight);
	sf::String title(L"簡易消行遊戲");
	sf::RenderWindow window(mode, title);

	sf::Sprite block(blockTexture);
	sf::Vector2i origin(fieldWidth / 2, 0);
	sf::Vector2i pos(origin);
	block.setPosition(getPixelPosition(pos, blockSize));
	
	sf::Clock clock;
	while (window.isOpen()) {
		enum class Action {
			Hold,
			MoveLeft,
			MoveRight,
			MoveDown
		};

		Action action = Action::Hold;
		sf::Event evt;
		if (window.pollEvent(evt)) {
			if (evt.type == sf::Event::Closed) {
				window.close();
			}
			else if (evt.type == sf::Event::KeyPressed) {
				if (evt.key.code == sf::Keyboard::Left) {
					action = Action::MoveLeft;
				}
				else if (evt.key.code == sf::Keyboard::Right) {
					action = Action::MoveRight;
				}
				else if (evt.key.code == sf::Keyboard::Down) {
					action = Action::MoveDown;
				}
			}
		}

		if (clock.getElapsedTime().asSeconds() >= 0.5f) {
			action = Action::MoveDown;
			clock.restart();
		}


		sf::Vector2i nextPos = pos;
		switch (action) {
		case Action::Hold:
			break;
		case Action::MoveLeft:
			--nextPos.x;
			break;
		case Action::MoveRight:
			++nextPos.x;
			break;
		case Action::MoveDown:
			++nextPos.y;
			break;
		}
		if (nextPos.x >= 0 && nextPos.x < fieldWidth &&
			nextPos.y < fieldHeight &&
			field[nextPos.x][nextPos.y] == false) {
			pos = nextPos;
		}
		else {
			if (action == Action::MoveDown) {
				field[pos.x][pos.y] = true;
				bool isFull = true;
				for (int x = 0; x < fieldWidth; ++x) {
					if (field[x][pos.y] == false) {
						isFull = false;
						break;
					}
				}
				if (isFull) {
					for (int y = pos.y; y > 0; y--) {
						for (int x = 0; x < fieldWidth; ++x) {
							field[x][y] = field[x][y - 1];
						}
					}
					for (int x = 0; x < fieldWidth; ++x) {
						field[x][0] = false;
					}
					// 檢查方塊是否全被消除了 是的話勝利
					bool isWin = true;
					for (int x = 0; x < fieldWidth && isWin; ++x) {
						for (int y = 0; y < fieldHeight && isWin; ++y) {
							if (field[x][y] == true) {
								isWin = false;
							}
						}
					}
					if (isWin) {

					}
				}
				pos = origin;
			}
		}
		

		window.clear();

		// 目前所控制的方塊
		block.setPosition(getPixelPosition(pos, blockSize));
		window.draw(block);

		// 繪製場地
		for (int x = 0; x < fieldWidth; ++x) {
			for (int y = 0; y < fieldHeight; ++y) {
				if (field[x][y] == true) {
					sf::Vector2i p(x, y);
					block.setPosition(getPixelPosition(p, blockSize));
					window.draw(block);
				}
			}
		}
		window.display();
	}
	return EXIT_SUCCESS;
}
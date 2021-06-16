#include <kdtree.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "utils.h"
using namespace std;

enum class SearchType { NN, KNN, SR, RR};
constexpr int _factor = 2;
// globals
constexpr int width = 1024*_factor;
constexpr int height = 1024*_factor;
int n_balls = 500*_factor*_factor;
int k = 5;
float r = 100;

float rect_width = 500;
float rect_height = 300;
std::vector<Ball> balls;
kdtree::KdTree<Ball> tree;

void placeBalls() {
    balls.clear();
    // place balls randomly
    for (int i = 0; i < n_balls; ++i) {
        balls.emplace_back(sf::Vector2f(width * rnd(), height * rnd()), 3.0f*_factor);
    }

    // build kd-tree
    tree = {balls};
    tree.buildTree();
}


int main() {
    // create window
    sf::RenderWindow window(sf::VideoMode(width, height), "kdtree - Search",sf::Style::Default);
    window.setFramerateLimit(144);

    ImGui::SFML::Init(window);

    // setup points and balls
    placeBalls();
    tree.toGraphviz("../../test.gv");
    // mouse ball
    Ball mouseBall(sf::Vector2f(), r);
    mouseBall.setColor(sf::Color::Blue);

    // app loop
    sf::Clock deltaClock;
    while (window.isOpen()) {
        // event handling
        sf::Event event{};
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        // draw imgui
        ImGui::Begin("Parameters");
        if (ImGui::Button("Refresh")) {
            placeBalls();
        }
        if (ImGui::Button("Show KD-Tree Structure")) {
            tree.showStructure();
        }

        if (ImGui::InputInt("Number of balls", &n_balls)) {
            placeBalls();
        }

        static SearchType search_type = SearchType::NN;
        ImGui::Combo("Search Type", reinterpret_cast<int*>(&search_type),
                     "Nearest Neighbor\0k-Nearest Neighbor\0Spherical Range\0Ortho Window Range\0\0");

        switch (search_type) {
            case SearchType::NN:{
                break;
            }
            case SearchType::KNN: {
                ImGui::InputInt("k", &k);
                break;
            }
            case SearchType::SR: {
                ImGui::InputFloat("radius", &r);
                break;
            }
            case SearchType::RR: {
                ImGui::InputFloat("rect width", &rect_width);
                ImGui::InputFloat("rect height", &rect_height);
                break;
            }
        }

        ImGui::End();

        window.clear(sf::Color::White);

        // draw sfml
        switch (search_type) {
            case SearchType::NN: {
                // query nearest point to mouse cursor
                const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                const int idx_nearest = tree.searchNearest(Point2f(mousePos));

                // draw balls
                for (int i = 0; i < balls.size(); ++i) {
                    // make nearest point red
                    if (i == idx_nearest) {
                        balls[i].setColor(sf::Color::Red);
                    } else {
                        balls[i].setColor(sf::Color::Black);
                    }

                    window.draw(balls[i]);
                }

                // draw line between mouse cursor and nearest point
                sf::Vertex line[2];
                line[0].position = sf::Vector2f(mousePos);
                line[0].color = sf::Color::Blue;
                line[1].position = balls[idx_nearest].getPosition();
                line[1].color = sf::Color::Blue;
                window.draw(line, 2, sf::LineStrip);

                break;
            }

            case SearchType::KNN: {
                // query k-nearest point to mouse cursor
                const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                const std::vector<int> idx_nearests =
                        tree.searchKNearest(Point2f(mousePos), k);

                // make nearest point red
                for (auto & ball : balls) {
                    ball.setColor(sf::Color::Black);
                }

                for (int idx : idx_nearests) {
                    balls[idx].setColor(sf::Color::Red);
                }

                // draw balls
                for (const auto & ball : balls) {
                    window.draw(ball);
                }

                // draw line between mouse cursor and nearest point
                for (int idx_nearest : idx_nearests) {
                    sf::Vertex line[2];
                    line[0].position = sf::Vector2f(mousePos);
                    line[0].color = sf::Color::Blue;
                    line[1].position = balls[idx_nearest].getPosition();
                    line[1].color = sf::Color::Blue;
                    window.draw(line, 2, sf::LineStrip);
                }

                break;
            }

            case SearchType::SR: {
                // draw mouse ball
                const sf::Vector2f mousePos =
                        static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                mouseBall.setRadius(r);
                mouseBall.setPosition(mousePos);
                window.draw(mouseBall);

                // spherical range search
                const std::vector<int> indices =
                        tree.sphericalRangeSearch(Point2f(mousePos), r);

                // make nearest point red
                for (auto & ball : balls) {
                    ball.setColor(sf::Color::Black);
                }
                for (int idx : indices) {
                    balls[idx].setColor(sf::Color::Red);
                }

                // draw balls
                for (const auto & ball : balls) {
                    window.draw(ball);
                }
                break;
            }

            case SearchType::RR: {
                // draw mouse ball
                const sf::Vector2f mousePos =
                        static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                sf::RectangleShape rectangle;
                rectangle.setSize(sf::Vector2f(rect_width, rect_height));
                rectangle.setOutlineColor(sf::Color::Blue);
                rectangle.setOutlineThickness(1);
                rectangle.setPosition(mousePos.x - rect_width/2,mousePos.y - rect_height/2);
                window.draw(rectangle);

                const std::vector<int> indices =
                        tree.OrthoRangeSearch(Point2f(mousePos), rect_width, rect_height);

                // make nearest point red
                for (auto & ball : balls) {
                    ball.setColor(sf::Color::Black);
                }
                for (int idx : indices) {
                    balls[idx].setColor(sf::Color::Red);
                }

                // draw balls
                for (const auto & ball : balls) {
                    window.draw(ball);
                }
                break;
            }
        }

        // render
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
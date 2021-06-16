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
constexpr int height = 768*_factor;
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

void showStructure(sf::RenderWindow& window,const kdtree::Node* node = tree.root,
        const kdtree::Node* node_parent = nullptr){
    if(!node) return;
    sf::Vertex line[2];
    if(node->axis == 0) {
        if (!node_parent) {
            line[0].position = sf::Vector2f(tree.points[node->idx][0], 0);
            line[1].position = sf::Vector2f(tree.points[node->idx][0], height);

        }else {
            line[0].position = sf::Vector2f(tree.points[node->idx][0], 0);
            line[1].position = sf::Vector2f(tree.points[node->idx][0], tree.points[node_parent->idx][1]);
        }
        line[0].color = sf::Color::Cyan;
        line[1].color = sf::Color::Cyan;
        window.draw(line, 2, sf::LineStrip);
    } else{
        sf::Vertex line[2];
        line[0].position = sf::Vector2f(0, tree.points[node->idx][1]);
        line[1].position = sf::Vector2f(tree.points[node_parent->idx][0], tree.points[node->idx][1]);
        line[0].color = sf::Color::Cyan;
        line[1].color = sf::Color::Cyan;
        window.draw(line, 2, sf::LineStrip);
    }
    showStructure(window,node->leftChild,node);




}
int main() {
    // create window
    sf::RenderWindow window(sf::VideoMode(width, height), "kdtree - Search",sf::Style::Default);
    window.setFramerateLimit(144);

    ImGui::SFML::Init(window);
    bool drawStructure = false;
    bool add_point_manually = false;

    // setup points and balls
    placeBalls();
    //tree.toGraphviz("../../test.gv");
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
        if (ImGui::Button("Generate")) {
            placeBalls();
        }
        if (ImGui::Button("Show KD-Tree Structure")) {
            drawStructure = !drawStructure;
        }
        if (ImGui::Button("Add Points Manually")) {
            if(!add_point_manually){
                add_point_manually = true;
                balls.clear();
            } else{
                add_point_manually = false;
                placeBalls();
            }
        }

        if (ImGui::InputInt("Number of Balls", &n_balls)) {
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

        window.clear(sf::Color::Black);
        for (auto & ball : balls) {
            // make nearest point red
            ball.setColor(sf::Color::White);
        }
        if(drawStructure){
            showStructure(window);
        }
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
                    }
                    window.draw(balls[i]);
                }

                // draw line between mouse cursor and nearest point
                sf::Vertex line[2];
                line[0].position = sf::Vector2f(mousePos);
                line[0].color = sf::Color::Green;
                line[1].position = balls[idx_nearest].getPosition();
                line[1].color = sf::Color::Green;
                window.draw(line, 2, sf::LineStrip);

                break;
            }

            case SearchType::KNN: {
                // query k-nearest point to mouse cursor
                const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                const std::vector<int> idx_nearests =
                        tree.searchKNearest(Point2f(mousePos), k);

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
                    line[0].color = sf::Color::Green;
                    line[1].position = balls[idx_nearest].getPosition();
                    line[1].color = sf::Color::Green;
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
                rectangle.setFillColor(sf::Color::Transparent);
                rectangle.setPosition(mousePos.x - rect_width/2,mousePos.y - rect_height/2);
                window.draw(rectangle);

                const std::vector<int> indices =
                        tree.OrthoRangeSearch(Point2f(mousePos), rect_width, rect_height);

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
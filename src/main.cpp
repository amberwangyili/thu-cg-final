#include <kdtree.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "utils.h"
#include "imfilebrowser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <string>
#include<random>


using namespace std;

auto my_red = sf::Color(217,150,148);
auto my_green = sf::Color(195,214,155);
auto my_blue = sf::Color(183, 222, 231);


enum class SearchType {
    NN, KNN, SR, RR
};
constexpr int _factor = 2;
// globals
constexpr int width = 1024 * _factor;
constexpr int height = 768 * _factor;
int n_balls = 500 * _factor * _factor;
int k = 5;
float r = 300;

float rect_width = 500;
float rect_height = 300;
float ball_radius = 3.5f*_factor;
std::vector<Ball> balls;
kdtree::KdTree<Ball> tree;

void placeBalls() {
    balls.clear();
    // place balls randomly
    for (int i = 0; i < n_balls; ++i) {
        balls.emplace_back(sf::Vector2f(width * rnd(), height * rnd()), ball_radius);
    }

    // build kd-tree
    tree = {balls};
    tree.buildTree();
}

void showStructure(sf::RenderWindow &window, const kdtree::Node *node = tree.root,
                   const kdtree::Node *node_parent = nullptr) {
    if (!node) return;
    sf::Vertex line[2];
    if (node->axis == 0) {
        if (!node_parent) {
            line[0].position = sf::Vector2f(tree.points[node->idx][0], 0);
            line[1].position = sf::Vector2f(tree.points[node->idx][0], height);

        } else {
            line[0].position = sf::Vector2f(tree.points[node->idx][0], 0);
            line[1].position = sf::Vector2f(tree.points[node->idx][0], tree.points[node_parent->idx][1]);
        }
        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        window.draw(line, 2, sf::LineStrip);
    } else {
        line[0].position = sf::Vector2f(0, tree.points[node->idx][1]);
        line[1].position = sf::Vector2f(tree.points[node_parent->idx][0], tree.points[node->idx][1]);
        line[0].color = sf::Color::Black;
        line[1].color = sf::Color::Black;
        window.draw(line, 2, sf::LineStrip);
    }
    showStructure(window, node->leftChild, node);

}

void testRandom();
void testCircle();
void testDiagonal();
void testKNearset();
void testKNearset2();

void testSperical();


int main() {
    //testRandom();
    //testDiagonal();
    //testKNearset2();
    // create window
/*
    sf::RenderWindow window(sf::VideoMode(width, height), "kdtree - Search", sf::Style::Default);
    window.setFramerateLimit(144);

    ImGui::SFML::Init(window);

    ImGui::FileBrowser fileDialog;

    // (optional) set browser properties
    fileDialog.SetTitle("title");
    fileDialog.SetTypeFilters({ ".txt" });

    bool drawStructure = false;
    bool add_point_manually = false;

    // setup points and balls
    placeBalls();
    //tree.toGraphviz("../../test.gv");

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
        ImGui::Begin("Menu");

        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {
                fileDialog.Open();
            }
            if (ImGui::MenuItem("Cose")) {
                window.close();
            }
            ImGui::EndMenu();
        }


        if (ImGui::Button("Generate")) {
            placeBalls();
        }

        if (ImGui::Button("Show KD-Tree Structure")) {
            drawStructure = !drawStructure;
        }
        if (ImGui::InputInt("Number of Balls", &n_balls)) {
            placeBalls();
        }

        static SearchType search_type = SearchType::NN;
        ImGui::Combo("Search Type", reinterpret_cast<int *>(&search_type),
                     "Nearest Neighbor\0k-Nearest Neighbor\0Spherical Range\0Ortho Window Range\0\0");

        switch (search_type) {
            case SearchType::NN: {
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

        fileDialog.Display();
        if(fileDialog.HasSelected())
        {
            balls.clear();
            ifstream input(fileDialog.GetSelected().string(), ios::in);
            std::string line;
            while(getline(input,line)){
                istringstream istrm(line);
                float x,y;
                istrm>>x>>y;
                balls.emplace_back(sf::Vector2f(x*width,y*height),ball_radius);
            }
            fileDialog.ClearSelected();
            tree = {balls};
            tree.buildTree();
        }

        window.clear(sf::Color::White);
        // TODO: 点击一次鼠标加一个点


        for (auto &ball : balls) {
            ball.setColor(my_green);
        }
        if (drawStructure) {
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
                        balls[i].setColor(my_red);
                    }
                    window.draw(balls[i]);
                }
                if (!balls.empty()) {
                    // draw line between mouse cursor and nearest point
                    sf::Vertex line[2];
                    line[0].position = sf::Vector2f(mousePos);
                    line[0].color = my_blue;
                    line[1].position = balls[idx_nearest].getPosition();
                    line[1].color = my_blue;
                    window.draw(line, 2, sf::LineStrip);
                }
                break;
            }

            case SearchType::KNN: {
                // query k-nearest point to mouse cursor
                const sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                const std::vector<int> idx_nearests =
                        tree.searchKNearest(Point2f(mousePos), k);

                for (int idx : idx_nearests) {
                    balls[idx].setColor(my_red);
                }

                // draw balls
                for (const auto &ball : balls) {
                    window.draw(ball);
                }

                // draw line between mouse cursor and nearest point
                if (!balls.empty()) {
                    for (int idx_nearest : idx_nearests) {
                        sf::Vertex line[2];
                        line[0].position = sf::Vector2f(mousePos);
                        line[0].color = my_blue;
                        line[1].position = balls[idx_nearest].getPosition();
                        line[1].color = my_blue;
                        window.draw(line, 2, sf::LineStrip);
                    }
                }

                break;
            }

            case SearchType::SR: {
                // draw mouse ball
                const sf::Vector2f mousePos =
                        static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                sf::CircleShape mouseBall;
                mouseBall.setRadius(r);
                mouseBall.setPointCount(300);
                mouseBall.setOutlineColor(my_blue);
                mouseBall.setOutlineThickness(3);
                mouseBall.setFillColor(sf::Color::Transparent);
                mouseBall.setPosition(mousePos - sf::Vector2f(r, r));
                window.draw(mouseBall);

                // spherical range search
                const std::vector<int> indices =
                        tree.sphericalRangeSearch(Point2f(mousePos), r);

                for (int idx : indices) {
                    balls[idx].setColor(my_red);
                }

                // draw balls
                for (const auto &ball : balls) {
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
                rectangle.setOutlineColor(my_blue);
                rectangle.setOutlineThickness(3);
                rectangle.setFillColor(sf::Color::Transparent);
                rectangle.setPosition(mousePos.x - rect_width / 2, mousePos.y - rect_height / 2);
                window.draw(rectangle);

                const std::vector<int> indices =
                        tree.OrthoRangeSearch(Point2f(mousePos), rect_width, rect_height);

                for (int idx : indices) {
                    balls[idx].setColor(my_red);
                }

                // draw balls
                for (const auto &ball : balls) {
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
*/
    testSperical();
    return 0;
}



void testRandom(){
    string number[13] = { "0","1","5","25","50","100" ,"10000","20000","40000","80000","100000","200000" ,"400000" };
    for (const auto & i : number) {
        ifstream input("../../data/size-random/random" + i + ".txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };

        auto start_epoch = chrono::high_resolution_clock::now();
        tree.buildTree();
        auto finish_epoch = chrono::high_resolution_clock::now();
        auto epoches = chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        cout <<i<<" "<<  epoches << endl;
    }
}

void testCircle(){
    string number[13] = { "0","1","5","25","50","100" ,"10000","20000","40000","80000","100000","200000" ,"400000" };
    for (const auto & i : number) {
        ifstream input("../../data/size-circle/circle" + i + ".txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };

        auto start_epoch = chrono::high_resolution_clock::now();
        tree.buildTree();
        auto finish_epoch = chrono::high_resolution_clock::now();
        auto epoches = chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        cout <<i<<" "<<  epoches << endl;
    }
}

void testDiagonal(){
    string number[13] = { "0","1","5","25","50","100" ,"10000","20000","40000","80000","100000","200000" ,"400000" };
    for (const auto & i : number) {
        ifstream input("../../data/size-diagonal/diagonal" + i + ".txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };

        auto start_epoch = chrono::high_resolution_clock::now();
        tree.buildTree();
        auto finish_epoch = chrono::high_resolution_clock::now();
        auto epoches = chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        cout <<i<<" "<<  epoches << endl;
    }
}

void testKNearset(){
    uniform_real_distribution<float> u(0,1);
    default_random_engine e;
    int number[12] = {1,2,5,10,20,50,100,200,400,600,800,1000};
    for(int i = 0;i<12;i = i+1){
        auto k = number[i];

        ifstream input("../../data/size-random/random10000.txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.searchKNearest(Point2f(query_x * width, query_y * height), k);
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"\"random\" "<<k<<" "<<  result/1000.0 << endl;
    }
    for(int i = 0;i<12;i = i+1){
        auto k = number[i];

        ifstream input("../../data/size-circle/circle10000.txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.searchKNearest(Point2f(query_x * width, query_y * height), k);
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"\"circle\" "<<k<<" "<<  result/1000.0 << endl;
    }

    for(int i = 0;i<12;i = i+1){
        auto k = number[i];

        ifstream input("../../data/size-diagonal/diagonal10000.txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.searchKNearest(Point2f(query_x * width, query_y * height), k);
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"\"diagonal\" "<<k<<" "<<  result/1000.0 << endl;
    }

}


void testKNearset2(){
    uniform_real_distribution<float> u(0,1);
    default_random_engine e;
    string number[8] = {"25","50","100" ,"10000","20000","40000","80000","100000" };
    const int query_k = 20;
    for(int i = 0;i<8;i = i+1){

        ifstream input("../../data/size-random/random"+number[i]+".txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.searchKNearest(Point2f(query_x * width, query_y * height), query_k );
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"random " <<number[i]<<" "<<  result/1000.0 << endl;
    }
    for(int i = 0;i<8;i = i+1){

        ifstream input("../../data/size-circle/circle"+number[i]+".txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.searchKNearest(Point2f(query_x * width, query_y * height), query_k );
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"circle " <<number[i]<<" "<<  result/1000.0 << endl;
    }
    for(int i = 0;i<8;i = i+1){

        ifstream input("../../data/size-diagonal/diagonal"+number[i]+".txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.searchKNearest(Point2f(query_x * width, query_y * height), query_k );
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"diagonal " <<number[i]<<" "<<  result/1000.0 << endl;
    }
}

void testSperical(){
    uniform_real_distribution<float> u(0,1);
    default_random_engine e;
    float radius[7] = {0.05,0.08,0.1,0.15,0.2,0.4,0.5};
    for(float & radiu : radius){
        radiu = radiu*height;
    }
    for(int i = 0;i<7;i = i+1){
        auto my_r = radius[i];

        ifstream input("../../data/size-random/random10000.txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.sphericalRangeSearch(Point2f(query_x,query_y),my_r);
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"random "<<my_r/height<<" "<<  result/1000.0 << endl;
    }
    for(int i = 0;i<7;i = i+1){
        auto my_r = radius[i];

        ifstream input("../../data/size-circle/circle10000.txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.sphericalRangeSearch(Point2f(query_x,query_y),my_r);
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"circle "<<my_r/height<<" "<<  result/1000.0 << endl;
    }

    for(int i = 0;i<7;i = i+1){
        auto my_r = radius[i];

        ifstream input("../../data/size-diagonal/diagonal10000.txt", ios::in);
        balls.clear();
        std::string line;
        while (getline(input, line)) {
            istringstream istrm(line);
            float x, y;
            istrm >> x >> y;
            balls.emplace_back(sf::Vector2f(x * width, y * height), ball_radius);
        }
        tree = { balls };
        tree.buildTree();
        long long result = 0;
        for(int j = 0;j<1000;j++) {
            const auto query_x = u(e);
            const auto query_y = u(e);
            auto start_epoch = chrono::high_resolution_clock::now();
            tree.sphericalRangeSearch(Point2f(query_x,query_y),my_r);
            auto finish_epoch = chrono::high_resolution_clock::now();
            result += chrono::duration_cast<chrono::microseconds>(finish_epoch - start_epoch).count();
        }
        cout <<"diagonal "<<my_r/height<<" "<<  result/1000.0 << endl;
    }

}
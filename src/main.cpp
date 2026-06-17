#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <cmath>
#include <functional>

const int WIDTH = 1200, HEIGHT = 700;
const int N = 120;

struct Bar {
    int value;
    sf::Color color;
};

std::vector<Bar> bars;

void shuffle() {
    bars.clear();
    for(int i = 1; i <= N; i++)
        bars.push_back({i, sf::Color(100, 180, 255)});
    for(int i = N-1; i > 0; i--) {
        int j = rand() % (i+1);
        std::swap(bars[i], bars[j]);
    }
}

// --- Sorting step generators ---
struct SortStep { int a, b, type; }; // type: 0=compare, 1=swap, 2=sorted

std::vector<SortStep> steps;
int stepIdx = 0;

void genBubble() {
    steps.clear();
    std::vector<int> arr(N);
    for(int i=0;i<N;i++) arr[i]=bars[i].value;
    for(int i=0;i<N-1;i++)
        for(int j=0;j<N-1-i;j++){
            steps.push_back({j,j+1,0});
            if(arr[j]>arr[j+1]){
                steps.push_back({j,j+1,1});
                std::swap(arr[j],arr[j+1]);
            }
        }
}

void genInsertion() {
    steps.clear();
    std::vector<int> arr(N);
    for(int i=0;i<N;i++) arr[i]=bars[i].value;
    for(int i=1;i<N;i++){
        int key=arr[i], j=i-1;
        while(j>=0 && arr[j]>key){
            steps.push_back({j,j+1,0});
            steps.push_back({j,j+1,1});
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=key;
    }
}

void mergeHelper(std::vector<int>& arr, int l, int r){
    if(l>=r) return;
    int m=(l+r)/2;
    mergeHelper(arr,l,m);
    mergeHelper(arr,m+1,r);
    std::vector<int> tmp;
    int i=l,j=m+1;
    while(i<=m&&j<=r){
        steps.push_back({i,j,0});
        if(arr[i]<=arr[j]) tmp.push_back(arr[i++]);
        else { tmp.push_back(arr[j++]); }
    }
    while(i<=m) tmp.push_back(arr[i++]);
    while(j<=r) tmp.push_back(arr[j++]);
    for(int k=l;k<=r;k++){
        if(arr[k]!=tmp[k-l]){
            steps.push_back({k,k,1});
            arr[k]=tmp[k-l];
        }
    }
}

void genMerge() {
    steps.clear();
    std::vector<int> arr(N);
    for(int i=0;i<N;i++) arr[i]=bars[i].value;
    mergeHelper(arr,0,N-1);
}

void quickHelper(std::vector<int>& arr, int l, int r){
    if(l>=r) return;
    int pivot=arr[r], i=l-1;
    for(int j=l;j<r;j++){
        steps.push_back({j,r,0});
        if(arr[j]<=pivot){
            i++;
            steps.push_back({i,j,1});
            std::swap(arr[i],arr[j]);
        }
    }
    steps.push_back({i+1,r,1});
    std::swap(arr[i+1],arr[r]);
    quickHelper(arr,l,i);
    quickHelper(arr,i+2,r);
}

void genQuick() {
    steps.clear();
    std::vector<int> arr(N);
    for(int i=0;i<N;i++) arr[i]=bars[i].value;
    quickHelper(arr,0,N-1);
}

void genSelection() {
    steps.clear();
    std::vector<int> arr(N);
    for(int i=0;i<N;i++) arr[i]=bars[i].value;
    for(int i=0;i<N-1;i++){
        int minIdx=i;
        for(int j=i+1;j<N;j++){
            steps.push_back({minIdx,j,0});
            if(arr[j]<arr[minIdx]) minIdx=j;
        }
        if(minIdx!=i){
            steps.push_back({i,minIdx,1});
            std::swap(arr[i],arr[minIdx]);
        }
    }
}

enum Algo { BUBBLE, INSERTION, MERGE, QUICK, SELECTION };
std::string algoNames[] = {"Bubble Sort","Insertion Sort","Merge Sort","Quick Sort","Selection Sort"};
Algo curAlgo = BUBBLE;

void genSteps(){
    if(curAlgo==BUBBLE) genBubble();
    else if(curAlgo==INSERTION) genInsertion();
    else if(curAlgo==MERGE) genMerge();
    else if(curAlgo==QUICK) genQuick();
    else genSelection();
    stepIdx=0;
}

int main(){
    sf::RenderWindow window(sf::VideoMode({(unsigned)WIDTH,(unsigned)HEIGHT}),
        "Sorting Visualizer | 1-5: Algorithm | Space: Shuffle+Sort | F: Fast | P: Pause");
    window.setFramerateLimit(60);
    srand(42);

    shuffle();
    genSteps();

    bool paused = false;
    bool fast = false;
    int speed = 6; // steps per frame normal
    int highlight1=-1, highlight2=-1;

    sf::Font font;
    bool hasFont = font.openFromFile("C:/Windows/Fonts/arial.ttf");
    sf::Text hud(font);
    hud.setCharacterSize(15);
    hud.setFillColor(sf::Color::White);
    hud.setOutlineColor(sf::Color::Black);
    hud.setOutlineThickness(1.5f);
    hud.setPosition({8.f, (float)HEIGHT - 38.f});

    sf::Text title(font);
    title.setCharacterSize(20);
    title.setFillColor(sf::Color(255,220,80));
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2.f);
    title.setPosition({8.f, 8.f});

    sf::Clock clock;

    while(window.isOpen()){
        while(auto ev = window.pollEvent()){
            if(ev->is<sf::Event::Closed>()) window.close();
            if(auto* k = ev->getIf<sf::Event::KeyPressed>()){
                if(k->code == sf::Keyboard::Key::Escape) window.close();
                if(k->code == sf::Keyboard::Key::Space){
                    shuffle(); genSteps(); paused=false; highlight1=highlight2=-1;
                }
                if(k->code == sf::Keyboard::Key::P) paused=!paused;
                if(k->code == sf::Keyboard::Key::F) fast=!fast;
                if(k->code == sf::Keyboard::Key::Num1){curAlgo=BUBBLE;    shuffle();genSteps();paused=false;}
                if(k->code == sf::Keyboard::Key::Num2){curAlgo=INSERTION; shuffle();genSteps();paused=false;}
                if(k->code == sf::Keyboard::Key::Num3){curAlgo=MERGE;     shuffle();genSteps();paused=false;}
                if(k->code == sf::Keyboard::Key::Num4){curAlgo=QUICK;     shuffle();genSteps();paused=false;}
                if(k->code == sf::Keyboard::Key::Num5){curAlgo=SELECTION; shuffle();genSteps();paused=false;}
                if(k->code == sf::Keyboard::Key::Up)   speed=std::min(60,speed+2);
                if(k->code == sf::Keyboard::Key::Down) speed=std::max(1,speed-2);
            }
        }

        int spf = fast ? 300 : speed;

        if(!paused && stepIdx < (int)steps.size()){
            for(int s=0; s<spf && stepIdx<(int)steps.size(); s++, stepIdx++){
                auto& step = steps[stepIdx];
                highlight1=step.a; highlight2=step.b;
                if(step.type==1){
                    std::swap(bars[step.a].value, bars[step.b].value);
                }
            }
        }

        bool done = (stepIdx >= (int)steps.size());

        window.clear(sf::Color(15,17,26));

        float barW = (float)(WIDTH-20) / N;
        float maxH = HEIGHT - 70.f;

        for(int i=0; i<N; i++){
            float h = (bars[i].value / (float)N) * maxH;
            float x = 10.f + i * barW;
            float y = HEIGHT - 45.f - h;

            sf::Color col;
            if(done){
                // rainbow when sorted
                float t = (float)i / N;
                col = sf::Color(
                    (uint8_t)(50 + 205*std::abs(std::sin(t*3.14f))),
                    (uint8_t)(80 + 175*std::abs(std::sin(t*3.14f + 2.09f))),
                    (uint8_t)(80 + 175*std::abs(std::sin(t*3.14f + 4.19f)))
                );
            } else if(i==highlight1){
                col = sf::Color(255, 80, 80);   // red = comparing
            } else if(i==highlight2){
                col = sf::Color(255, 220, 50);  // yellow
            } else {
                float t = bars[i].value / (float)N;
                col = sf::Color(
                    (uint8_t)(60 + t*80),
                    (uint8_t)(120 + t*80),
                    (uint8_t)(200 + t*55)
                );
            }

            sf::RectangleShape bar({barW - 1.f, h});
            bar.setPosition({x, y});
            bar.setFillColor(col);
            window.draw(bar);
        }

        // Bottom line
        sf::RectangleShape line({(float)WIDTH, 2.f});
        line.setPosition({0.f, (float)HEIGHT - 45.f});
        line.setFillColor(sf::Color(60,65,80));
        window.draw(line);

        if(hasFont){
            title.setString(algoNames[curAlgo] + (done ? "  ✓ Sorted!" : ""));
            window.draw(title);

            std::string info = "1:Bubble 2:Insert 3:Merge 4:Quick 5:Select | Space:Shuffle | F:Fast | P:Pause | ↑↓:Speed";
            if(done) info = "Sorting complete! Press Space for new shuffle or 1-5 to switch algorithm.";
            hud.setString(info);
            window.draw(hud);

            // Step counter
            if(!done && hasFont){
                sf::Text stepTxt(font);
                stepTxt.setCharacterSize(13);
                stepTxt.setFillColor(sf::Color(180,180,180));
                stepTxt.setPosition({(float)WIDTH-220.f, 10.f});
                stepTxt.setString("Step: "+std::to_string(stepIdx)+"/"+std::to_string(steps.size()));
                window.draw(stepTxt);
            }
        }

        window.display();
    }
    return 0;
}

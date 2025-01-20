//
// Created by probs on 4/24/2024.
//
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <chrono>

//Total time spent.

//4/15 4:00-5:20 pm welcome window
//4/22 3:00-4:30 pm Displaying tiles
//4/22 8:10-8:40 pm Flags
//4/22 8:50-9:50 pm Bombs
//4/22 10:30-10:55 pm Counter....
//4/23 1:00-2:45 pm Counter + Negative Counter
//4/23 2:50-4:30 Pause/Smiley/Leaderboard Buttons
//4/23 11:00-12:00am Pause
//4/24 2:30-5:40 Game logic - buttons
//4/24 7:00-12:00 am Final touches

using namespace std;


// ADD SETTEXT FUNCTION FROM FILES
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

class Tile {
public:
    int x;
    int y;
    string state; //State = hidden, shown, count
    int count; //Count = 0-8
    bool bomb; //Bomb = true, false
    bool debbie = false;
    sf::Sprite sprite;
    sf::Sprite top;

    Tile(int xcord, int ycord, string stateE, int countE, bool bombE) : x(xcord),
                                                                        y(ycord), state(stateE), count(countE), bomb(bombE) {}

    void setTexture(sf::Texture& tex){
        sprite.setTexture(tex); //////////
    }
    void setTextureTop(sf::Texture& tex){
        top.setTexture(tex); //////////
    }

};

class Button {
public:
    int x;
    int y;
    string state;
    sf::Sprite sprite1;
    sf::Sprite sprite2;
    sf::Sprite sprite3;
    sf::Sprite sprite4;

    Button(int xcord, int ycord) : x(xcord),
                                   y(ycord) {}
};

int main() {

    //Isaac Probst's submission for COP3503  Project 3


    //Reading config.cfg for initial variables.
    int cols;
    int rows;
    int mines;
    ifstream file("files/config.cfg");
    int a;
    file >> cols;
    file >> rows;
    file >> mines;
    file.close();
    int xsize = (cols*32)+100;
    int ysize = (rows*32)+100;

    //WELCOME WINDOW

    sf::RenderWindow window(sf::VideoMode(xsize, ysize), "Minesweeper"); // RENDER WELCOME WINDOW
    sf::RectangleShape shape(sf::Vector2f(xsize, ysize));
    shape.setFillColor(sf::Color::Blue);
    sf::Text welcome;
    string welcomeTxt = "WELCOME TO MINESWEEPER!";
    welcome.setString(welcomeTxt);
    welcome.setCharacterSize(24);
    welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::Font font; // USING SFML FONT
    font.loadFromFile("font.ttf");
    welcome.setFont(font);
    welcome.setPosition((xsize/2)-150, (ysize/2)-150);
    sf::Text name;
    string nameTxt = "Enter your name:";
    name.setString(nameTxt);
    name.setCharacterSize(20); //PROPER SIZE
    name.setFont(font);
    name.setPosition((xsize/2)-100, ysize/2-100);
    name.setStyle(sf::Text::Bold);
    std::string inputname = "|";
    sf::Text displayName;
    displayName.setFont(font);
    setText(displayName, 440, 290);
    displayName.setCharacterSize(18);
    displayName.setFillColor(sf::Color::Yellow);
    bool ent = false;
    bool win = false;
    int players = 5;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                setText(displayName, 440, 290);
                if (!isalpha(event.text.unicode)) {
                    if(event.text.unicode == 8) {
                        if (inputname.length() <= 2) {
                            inputname = "|";
                            displayName.setString(inputname);
                        } else {
                            inputname.pop_back();
                            inputname.pop_back();
                            inputname += "|";
                            displayName.setString(inputname);
                        }
                    }
                    if(event.text.unicode == 13) { //CHANGE CUZ MAC ENTER DIFFERENT?
                        if(inputname.length() > 1){
                            window.close();
                            ent = true;
                        }
                    }
                } else if (inputname.length() > 10 ) {
                    if (event.text.unicode == 8) {
                        inputname.pop_back();
                        inputname.pop_back();
                        inputname += "|";
                        displayName.setString(inputname);
                    }
                } else if (inputname.length() == 1 ) {
                    inputname.pop_back();
                    inputname += toupper(static_cast<char>(event.text.unicode));
                    inputname += "|";
                    displayName.setString(inputname);

                } else {
                    inputname.pop_back();
                    inputname += tolower(static_cast<char>(event.text.unicode));
                    inputname += "|";
                    displayName.setString(inputname);
                }
                // after changed, call setText function!!
            }
        }
        window.clear();
        window.draw(shape);
        window.draw(welcome);
        window.draw(name);
        window.draw(displayName);
        window.display();
    }
    //END OF WELCOME WINDOW. CHECK IF ENTER PRESSED


    //OFFICE HRS 4/23 2 PM c birthday and econ

    //BEGINNING OF GAME WINDOW
    if(ent) {
        //GAME WINDOW
        bool loss = false;
        sf::RenderWindow gamewin(sf::VideoMode(32 * cols, 32 * rows + 128), "Minesweeper"); // Window and title
        sf::RectangleShape rect(sf::Vector2f((32 * cols), 32 * rows + 128)); // Background of game window
        rect.setFillColor(sf::Color::White); // Set background to black

        // ENTER UNICODE DIFFERENT ON MAC? sf::Enter !!!!!!!!!!! /////////

        //First load of the board, making 2d vector for each tile and class object.
        vector<vector<Tile>> board;
        sf::Texture hiddenT;
        hiddenT.loadFromFile("files/images/tile_hidden.png");
        sf::Texture shownT;
        shownT.loadFromFile("files/images/tile_revealed.png");
        sf::Texture bomb;
        bomb.loadFromFile(("files/images/mine.png"));
        sf::Texture faceHappy;
        faceHappy.loadFromFile("files/images/face_happy.png");
        sf::Texture faceSad;
        faceSad.loadFromFile(("files/images/face_lose.png"));
        sf::Texture faceWin;
        faceWin.loadFromFile(("files/images/face_win.png"));
        sf::Texture flag;
        flag.loadFromFile(("files/images/flag.png"));
        sf::Texture mine;
        mine.loadFromFile(("files/images/mine.png"));
        sf::Texture debug;
        debug.loadFromFile(("files/images/debug.png"));
        sf::Texture play;
        play.loadFromFile(("files/images/play.png"));
        sf::Texture pause;
        pause.loadFromFile(("files/images/pause.png"));
        sf::Texture digits;
        digits.loadFromFile(("files/images/digits.png"));
        sf::Texture leaderboard;
        leaderboard.loadFromFile(("files/images/leaderboard.png"));
        sf::Texture n1, n2, n3, n4, n5, n6, n7, n8;
        n1.loadFromFile(("files/images/number_1.png"));
        n1.loadFromFile("files/images/number_1.png");
        n2.loadFromFile("files/images/number_2.png");
        n3.loadFromFile("files/images/number_3.png");
        n4.loadFromFile("files/images/number_4.png");
        n5.loadFromFile("files/images/number_5.png");
        n6.loadFromFile("files/images/number_6.png");
        n7.loadFromFile("files/images/number_7.png");
        n8.loadFromFile("files/images/number_8.png");



        //Adding buttons and timers

        // BUTTON GENERATOR -----------------
        int ourycord = (32*(rows + 0.5)) + 16;
        int minesleft = mines;
        Button count(33, ourycord);

        //COUNT OF FLAGS/MINES LEFT
        //21x32 for each digit?
        count.sprite1.setPosition(33, ourycord);
        count.sprite2.setPosition(33 + 21, ourycord);
        count.sprite3.setPosition(33 + 42, ourycord);
        count.sprite4.setPosition(12, ourycord);
        count.sprite1.setTexture(digits);
        count.sprite2.setTexture(digits);
        count.sprite3.setTexture(digits);
        count.sprite4.setTexture(digits);
        count.sprite1.setTextureRect(sf::IntRect(21 * (abs(minesleft)/100), 0, 21, 32));
        count.sprite2.setTextureRect(sf::IntRect(21 * (abs(minesleft / 10)), 0, 21, 32));
        count.sprite3.setTextureRect(sf::IntRect(21 * (abs(minesleft/10)%10), 0, 21, 32)); /////////////
        count.sprite4.setTextureRect(sf::IntRect(digits.getSize().x - 21, 0, 21, 32));

        //TIMER
        int displayMinutes = 0;
        int displaySeconds = 0;
        Button timerMin((cols*32)-97, 32*(rows+0.5)+16);
        Button timerSec((cols*32)-54,32*(rows+0.5)+16);
        timerMin.sprite1.setPosition((cols*32)-97, ourycord);
        timerMin.sprite2.setPosition((cols*32)-97+21,ourycord);
        timerSec.sprite1.setPosition((cols*32)-54,ourycord);
        timerSec.sprite2.setPosition((cols*32)-54+21,ourycord);
        chrono::time_point<chrono::high_resolution_clock> start;  //////
        chrono::time_point<chrono::high_resolution_clock> current;
        chrono::time_point<chrono::high_resolution_clock> startPaused;
        chrono::time_point<chrono::high_resolution_clock> currentPaused;
        timerSec.sprite1.setTexture(digits);
        timerSec.sprite2.setTexture(digits);
        timerMin.sprite1.setTexture(digits);
        timerMin.sprite2.setTexture(digits);

        chrono::duration<float> duration;
        chrono::duration<float> pausedDuration;

        timerSec.sprite1.setTextureRect(sf::IntRect(21 * (abs(minesleft)/100), 0, 21, 32));
        timerSec.sprite2.setTextureRect(sf::IntRect(21 * (abs(minesleft / 10)), 0, 21, 32));
        timerMin.sprite1.setTextureRect(sf::IntRect(21 * (abs(minesleft)/100), 0, 21, 32));
        timerMin.sprite2.setTextureRect(sf::IntRect(21 * (abs(minesleft / 10)), 0, 21, 32));

        //FACE
        Button face((cols*16)-32,32*(rows+0.5));
        face.sprite1.setTexture(faceHappy);
        face.state = "happy";
        face.sprite1.setPosition((cols*16)-32, 32*(rows+0.5));


        //PAUSE
        Button pauseB(cols*32-240, 32*(rows+0.5));
        pauseB.state = "active";
        pauseB.sprite1.setTexture(pause);
        pauseB.sprite1.setPosition(cols*32-240, 32*(rows+0.5));


        //LEADERBOARD BUTTON
        Button leaderboardB(cols*32-176, 32*(rows+0.5));
        leaderboardB.sprite1.setTexture(leaderboard);
        leaderboardB.sprite1.setPosition(cols*32-176, 32*(rows+0.5));


        //DEBUG BUTTON
        Button DebugB(cols*32-304, 32*(rows+0.5));
        DebugB.sprite1.setTexture(debug);
        DebugB.sprite1.setPosition(cols*32-304, 32*(rows+0.5));
        bool DebugT = false;




        //Initialize all tiles as class objects and make them hidden, no bombs.
        // TILE GENERATOR -------------------------------------------------
        for (int i = 0; i < rows; i++) {
            vector<Tile> row;
            for (int j = 0; j < cols; j++) {
                int xc = j * 32;
                int yc = i * 32;
                Tile tile(xc, yc, "hidden", 0, false);
                tile.setTexture(hiddenT);
                tile.sprite.setPosition(xc, yc);
                tile.top.setPosition(xc, yc);
                row.push_back(tile);
            }
            board.push_back(row);
        }
        int bombcount = 0;
        while (bombcount < mines) {
            int runrow = rand() % rows;
            int runcol = rand() % cols;
            if (board[runrow][runcol].bomb == false) {
                board[runrow][runcol].bomb = true;
                bombcount++;
            }
            if (bombcount >= mines) {
                break;
            }
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int assignedCount = 0;
                if(board[i][j].bomb == true){
                    continue;
                } else {
                    for(int k = i-1; k <= i+1; k++){
                        for(int l = j-1; l <= j+1; l++){
                            if(((k>=0)&&(k<rows))&&((l>=0)&&(l<cols))){
                                if((k==i)&&(l==j)){
                                    continue;
                                }
                                if(board[k][l].bomb == true){
                                    assignedCount++;
                                }
                            }
                        }
                    }
                }
                board[i][j].count = assignedCount;
            }
        }


        //Main loop for game.
        int flags = 0;
        bool stop;
        bool pauseloop = false;
        bool stoptime = false;
        start = chrono::high_resolution_clock::now();


        bool notyet = true;
        while (gamewin.isOpen()) {
            gamewin.display();


            gamewin.draw(face.sprite1);
            gamewin.draw(pauseB.sprite1);
            gamewin.draw(leaderboardB.sprite1);
            float seconds;
            float subseconds;

            if(stop || stoptime){
                if(stoptime && stop) {
                    currentPaused = chrono::high_resolution_clock::now();

                } else {
                    stop = false;
                    stoptime = false;
                }
            }
            current = chrono::high_resolution_clock::now();
            // Time passed = present time - time at window open
            duration = current - start;
            seconds = duration.count() - subseconds;
            displayMinutes = static_cast<int>(seconds)/60;
            displaySeconds = static_cast<int>(seconds)%60;
            if(!stop && !loss) {
                timerSec.sprite1.setTextureRect(sf::IntRect(21 * ((displaySeconds) / 10), 0, 21, 32));
                timerSec.sprite2.setTextureRect(sf::IntRect(21 * ((displaySeconds % 10)), 0, 21, 32));
                timerMin.sprite1.setTextureRect(sf::IntRect(21 * ((displayMinutes) / 10), 0, 21, 32));
                timerMin.sprite2.setTextureRect(sf::IntRect(21 * ((displayMinutes % 10)), 0, 21, 32));
            }



            sf::Event event;


            gamewin.draw(timerSec.sprite1);
            gamewin.draw(timerSec.sprite2);
            gamewin.draw(timerMin.sprite1);
            gamewin.draw(timerMin.sprite2);
            while (gamewin.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gamewin.close();
                } else if (event.type == sf::Event::TextEntered) {

                } else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(gamewin);


                        // HAPPY FACE / RESET GAME
                        if ((abs(mousePosition.x - 24 - face.x) < 32) &&
                            (abs(mousePosition.y - 24 - face.y) < 32)) {
                            face.state = "happy";
                            face.sprite1.setTexture(faceHappy);
                            gamewin.draw(face.sprite1);
                            loss = false;
                            stop = false;
                            start = chrono::high_resolution_clock::now();
                            DebugT=true;
                            notyet = true;

                            for (int i = 0; i < rows; i++) {
                                for (int j = 0; j < cols; j++) {
                                    board[i][j].bomb = false;
                                    board[i][j].count = 0;
                                }
                            }





                            for (int i = 0; i < rows; i++) {
                                vector<Tile> row;
                                for (int j = 0; j < cols; j++) {
                                    int xc = j * 32;
                                    int yc = i * 32;
                                    Tile tile(xc, yc, "hidden", 0, false);
                                    tile.setTexture(hiddenT);
                                    tile.sprite.setPosition(xc, yc);
                                    tile.top.setPosition(xc, yc);
                                    row.push_back(tile);
                                }
                                board.push_back(row);
                            }
                            int bombcount = 0;
                            while (bombcount < mines) {
                                int runrow = rand() % rows;
                                int runcol = rand() % cols;
                                if (board[runrow][runcol].bomb == false) {
                                    board[runrow][runcol].bomb = true;
                                    bombcount++;
                                }
                                if (bombcount >= mines) {
                                    break;
                                }
                            }
                            for (int i = 0; i < rows; i++) {
                                for (int j = 0; j < cols; j++) {
                                    int assignedCount = 0;
                                    if(board[i][j].bomb == true){
                                        continue;
                                    } else {
                                        for(int k = i-1; k <= i+1; k++){
                                            for(int l = j-1; l <= j+1; l++){
                                                if(((k>=0)&&(k<rows))&&((l>=0)&&(l<cols))){
                                                    if((k==i)&&(l==j)){
                                                        continue;
                                                    }
                                                    if(board[k][l].bomb == true){
                                                        assignedCount++;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    board[i][j].count = assignedCount;
                                }
                            }

                            for (int i = 0; i < rows; i++) {
                                for (int j = 0; j < cols; j++) {
                                    board[i][j].state = "hidden";
                                    board[i][j].sprite.setTexture(hiddenT);
                                    gamewin.draw(board[i][j].sprite);
                                }
                            }







                            //Main loop for game.
                            int flags = 0;
                            bool stop;
                            bool pauseloop = false;
                            bool stoptime = false;
                            start = chrono::high_resolution_clock::now();


                            bool notyet = true;





















                        }


                        //DEBUG BUTTON
                        if ((abs(mousePosition.x - 24 - DebugB.x) < 32) &&
                            (abs(mousePosition.y - 24 - DebugB.y) < 32)) {
                            if (!DebugT) {
                                for (int i = 0; i < rows; i++) {
                                    for (int j = 0; j < cols; j++) {
                                        if (board[i][j].bomb == true) {
                                            board[i][j].debbie = true;
                                        }
                                    }
                                }
                                DebugT = !DebugT;
                                break;
                            } else {
                                DebugT = false;
                                for (int i = 0; i < rows; i++) {
                                    for (int j = 0; j < cols; j++) {
                                        if (board[i][j].bomb == true) {
                                            board[i][j].debbie = false;
                                        }
                                    }
                                }
                            }
                        }







                        // LEADERBOARD BUTTON
                        if ((abs(mousePosition.x - 24 - leaderboardB.x) < 32) &&
                            (abs(mousePosition.y - 24 - leaderboardB.y) < 32)) {
                            stop = true;
                            sf::RenderWindow leadWin(sf::VideoMode(xsize, ysize),
                                                     "Leaderboard"); // RENDER WELCOME WINDOW
                            sf::Text leaderTxt;
                            string ldtxt = "LEADERBOARD";

                            vector<string>playerNames(0);
                            string line;
                            string nocomm;
                            sf::Text p1, p2, p3, p4, p5;
                            string pl1, pl2, pl3, pl4, pl5;
                            ifstream leaderFile("files/leaderboard.txt");
                            while (getline(leaderFile, line)){
                                nocomm = "";
                                nocomm.append(line.substr(0,5));
                                nocomm.append("    ");
                                nocomm.append(5,line.length()-1);

                                playerNames.push_back(nocomm);
                                nocomm = "";
                            }

                            pl1.append(playerNames[0]);
                            pl2.append(playerNames[1]);
                            pl3.append(playerNames[2]);
                            pl4.append(playerNames[3]);
                            pl5.append(playerNames[4]);
                            p1.setString(pl1);
                            p1.setCharacterSize(20);
                            p1.setFont(font);
                            p1.setPosition(xsize/2-130, (ysize/2)-60);
                            p2.setString(pl2);
                            p2.setCharacterSize(20);
                            p2.setFont(font);
                            p2.setPosition(xsize/2-130, (ysize/2));
                            p3.setString(pl3);
                            p3.setCharacterSize(20);
                            p3.setFont(font);
                            p3.setPosition(xsize/2-130, (ysize/2)+60);
                            p4.setString(pl4);
                            p4.setCharacterSize(20);
                            p4.setFont(font);
                            p4.setPosition(xsize/2-130, (ysize/2)+120);
                            p5.setString(pl5);
                            p5.setCharacterSize(20);
                            p5.setFont(font);
                            p5.setPosition(xsize/2-130, (ysize/2)+180);



                            leaderTxt.setString(ldtxt);
                            leaderTxt.setStyle(sf::Text::Bold | sf::Text::Underlined);
                            leaderTxt.setCharacterSize(20);
                            leaderTxt.setFont(font);
                            setText(leaderTxt, xsize/2, (ysize/2)-180);
                            bool close=false;

                            while (leadWin.isOpen() && !close) {
                                leadWin.draw(shape);
                                leadWin.draw(leaderTxt);
                                leadWin.draw(p1);
                                leadWin.draw(p2);
                                leadWin.draw(p3);
                                leadWin.draw(p4);
                                leadWin.draw(p5);
                                leadWin.display();
                                sf::Event leadEvent;
                                while (leadWin.pollEvent(leadEvent)) {
                                    if (leadEvent.type == sf::Event::Closed) {
                                        leadWin.close();
                                        close = true;
                                    }
                                }
                            }
                        }

                        //PAUSE BUTTON
                        if ((abs(mousePosition.x - 24 - pauseB.x) < 32) &&
                            (abs(mousePosition.y - 24 - pauseB.y) < 32) && !loss) {
                            if (pauseB.state == "active") {
                                startPaused = chrono::high_resolution_clock::now();
                                stop = true;
                                stoptime = true;
                                pauseB.sprite1.setTexture(play);
                                gamewin.draw(pauseB.sprite1);
                                pauseloop = true;
                                pauseB.state = "passive";
                                break;
                            } else {
                                stop = false;
                                subseconds += chrono::duration_cast<chrono::seconds>(currentPaused - startPaused).count();
                                pauseB.sprite1.setTexture(pause);
                                pauseB.state = "active";
                                stoptime = false;
                                gamewin.draw(pauseB.sprite1);
                                pauseloop = true;
                                break;
                            }

                        }

                        for (int i = 0; i < rows; i++) {
                            for (int j = 0; j < cols; j++) {
                                if ((abs(mousePosition.x - 8 - board[i][j].x - 8) < 16) &&
                                    (abs(mousePosition.y - 8 - board[i][j].y - 8) < 16) && !stop && !loss) {
                                    if (board[i][j].state != "flagged") {
                                        board[i][j].state = "revealed";
                                        board[i][j].sprite.setTexture(shownT);
                                    }
                                    if (board[i][j].bomb == true) {
                                        loss = true;
                                        stop = true;
                                        board[i][j].top.setTexture(bomb);
                                        board[i][j].state = "revealed";
                                        board[i][j].sprite.setTexture(shownT);
                                    } else {
                                        if (board[i][j].state != "flagged") {
                                            board[i][j].state = "revealed";
                                            board[i][j].sprite.setTexture(shownT);
                                            if ((board[i][j].count > 0) && (board[i][j].bomb == false)) {
                                                board[i][j].state = "partial";
                                                board[i][j].sprite.setTexture(shownT);
                                                gamewin.draw(board[i][j].sprite);
                                                if ((board[i][j].count == 1)) {
                                                    board[i][j].top.setTexture(n1);
                                                    board[i][j].top.setPosition(board[i][j].x, board[i][j].y);
                                                    gamewin.draw(board[i][j].top);
                                                }
                                                if ((board[i][j].count == 2)) {
                                                    board[i][j].top.setTexture(n2);
                                                    board[i][j].top.setPosition(board[i][j].x, board[i][j].y);
                                                    gamewin.draw(board[i][j].top);
                                                }
                                                if ((board[i][j].count == 3)) {
                                                    board[i][j].top.setTexture(n3);
                                                    board[i][j].top.setPosition(board[i][j].x, board[i][j].y);
                                                    gamewin.draw(board[i][j].top);
                                                }
                                                if ((board[i][j].count == 4)) {
                                                    board[i][j].top.setTexture(n4);
                                                    board[i][j].top.setPosition(board[i][j].x, board[i][j].y);
                                                    gamewin.draw(board[i][j].top);
                                                }
                                                if ((board[i][j].count == 5)) {
                                                    board[i][j].top.setTexture(n5);
                                                    board[i][j].top.setPosition(board[i][j].x, board[i][j].y);
                                                    gamewin.draw(board[i][j].top);
                                                }
                                                if ((board[i][j].count == 6)) {
                                                    board[i][j].top.setTexture(n6);
                                                    board[i][j].top.setPosition(board[i][j].x, board[i][j].y);
                                                    gamewin.draw(board[i][j].top);
                                                }
                                                if ((board[i][j].count == 7)) {
                                                    board[i][j].top.setTexture(n7);
                                                    board[i][j].top.setPosition(board[i][j].x, board[i][j].y);
                                                    gamewin.draw(board[i][j].top);
                                                }
                                                if ((board[i][j].count == 8)) {
                                                    board[i][j].top.setTexture(n8);
                                                    board[i][j].top.setPosition(board[i][j].x, board[i][j].y);
                                                    gamewin.draw(board[i][j].top);
                                                }
                                            }
                                        }
                                    }
                                }
                            }















                        }

                        if (pauseloop) {
                            pauseloop = false;
                            break;
                        }

                    } else if (event.mouseButton.button == sf::Mouse::Right && !loss && !stop) {
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(gamewin);
                        for (int i = 0; i < rows; i++) {
                            for (int j = 0; j < cols; j++) {
                                if ((abs(mousePosition.x - 8 - board[i][j].x - 8) < 16) &&
                                    (abs(mousePosition.y - 8 - board[i][j].y - 8) < 16)) {
                                    if (board[i][j].state == "flagged" && !stop) {
                                        board[i][j].state = "hidden";
                                        flags--;
                                        minesleft++;
                                    } else if (board[i][j].state != "revealed" && !stop) {
                                        board[i][j].state = "flagged";
                                        flags++;
                                        minesleft--;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //END OF POLLED EVENT

            int goodbombs=0;
            win = true;
            for(int i = 0; i<rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if(board[i][j].bomb != true){
                        if(board[i][j].state == "revealed" || board[i][j].state == "partial") {
                            goodbombs += i;
                        } else {
                            win=false;
                        }
                    }
                }
            }


            //DRAWING ONLY UNDER
            // Drawing everything, buttons, flags, updating stuff. AFTER click event.
            gamewin.clear();
            gamewin.draw(rect);
            count.sprite1.setTextureRect(sf::IntRect(21*abs(minesleft/100), 0, 21, 32));
            count.sprite2.setTextureRect(sf::IntRect(21*(abs(minesleft%100)/10), 0, 21, 32));
            count.sprite3.setTextureRect(sf::IntRect(21*abs(minesleft%10), 0, 21, 32)); /////////////////
            gamewin.draw(count.sprite1);
            gamewin.draw(count.sprite2);
            gamewin.draw(count.sprite3);
            displayMinutes = static_cast<int>(seconds)/60;
            displaySeconds = static_cast<int>(seconds)%60;

            if(!stop && !loss) {
                timerSec.sprite1.setTextureRect(sf::IntRect(21 * ((displaySeconds) / 10), 0, 21, 32));
                timerSec.sprite2.setTextureRect(sf::IntRect(21 * ((displaySeconds % 10)), 0, 21, 32));
                timerMin.sprite1.setTextureRect(sf::IntRect(21 * ((displayMinutes) / 10), 0, 21, 32));
                timerMin.sprite2.setTextureRect(sf::IntRect(21 * ((displayMinutes % 10)), 0, 21, 32));
            }

            timerSec.sprite1.setTexture(digits);
            gamewin.draw(timerSec.sprite1);
            timerSec.sprite2.setTexture(digits);
            gamewin.draw(timerSec.sprite2);
            gamewin.draw(timerMin.sprite2);
            timerMin.sprite1.setTexture(digits);
            gamewin.draw(timerMin.sprite1);
            timerMin.sprite2.setTexture(digits);

            gamewin.draw(face.sprite1);
            gamewin.draw(pauseB.sprite1);
            gamewin.draw(leaderboardB.sprite1);

            if(minesleft < 0){
                gamewin.draw(count.sprite4);
            }
            for(int i = 0; i<rows; i++){
                for(int j=0; j < cols; j++){
                    gamewin.draw(board[i][j].sprite);
                    if(board[i][j].state == "flagged"){
                        board[i][j].top.setTexture(flag);
                        gamewin.draw(board[i][j].top);
                    }
                    if(board[i][j].state == "revealed"){
                        board[i][j].sprite.setTexture(shownT);
                        gamewin.draw(board[i][j].sprite);
                        if(board[i][j].bomb == true){
                            gamewin.draw(board[i][j].top);
                        }
                    }
                    if(board[i][j].state == "hidden"){
                        board[i][j].sprite.setTexture(hiddenT);
                    }
                    if(loss){
                        face.sprite1.setTexture(faceSad);
                        gamewin.draw(face.sprite1);
                        gamewin.draw(board[i][j].sprite);
                        if(board[i][j].bomb == true) {
                            board[i][j].state = "revealed";
                            board[i][j].top.setTexture(bomb);
                            gamewin.draw(board[i][j].top);
                        }
                    }
                    if(win&&notyet){
                        face.sprite1.setTexture(faceWin);
                        gamewin.draw(face.sprite1);
                        gamewin.draw(board[i][j].sprite);

                        ofstream leaderFile("files/leaderboard.txt");
                        int intSeconds = static_cast<int>(seconds);
                        string finalname = inputname.substr(0,inputname.length()-2);
                        if(intSeconds < 10){
                            leaderFile << "0" << displayMinutes << ":0" << intSeconds << "," << inputname << endl;
                        } else {
                            leaderFile << displayMinutes << ":" << intSeconds << "," << inputname << std::endl;
                        }
                        for(int i = 0; i < players; i++){
                            leaderFile << "0" << displayMinutes << ":0" << intSeconds << "," << inputname  << std::endl;
                        }

                        notyet=false;
                        players++;
                    }
                }
            }
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if(board[i][j].state == "revealed"){
                        for(int k = i-1; k <= i+1; k++){
                            for(int l = j-1; l <= j+1; l++){
                                if(((k>=0)&&(k<rows))&&((l>=0)&&(l<cols))){
                                    if((k==i)&&(l==j)){
                                        continue;
                                    }
                                    if((board[k][l].count == 0) && (board[k][l].bomb == false)){
                                        board[k][l].state = "revealed";
                                    }
                                    if((board[k][l].count > 0) && (board[k][l].bomb == false)){
                                        board[k][l].state = "partial";
                                        board[k][l].sprite.setTexture(shownT);
                                        gamewin.draw(board[k][l].sprite);
                                        if((board[k][l].count == 1)){
                                            board[k][l].top.setTexture(n1);
                                            board[k][l].top.setPosition(board[k][l].x, board[k][l].y);
                                            gamewin.draw(board[k][l].top);
                                        }
                                        if((board[k][l].count == 2)){
                                            board[k][l].top.setTexture(n2);
                                            board[k][l].top.setPosition(board[k][l].x, board[k][l].y);
                                            gamewin.draw(board[k][l].top);
                                        }
                                        if((board[k][l].count == 3)){
                                            board[k][l].top.setTexture(n3);
                                            board[k][l].top.setPosition(board[k][l].x, board[k][l].y);
                                            gamewin.draw(board[k][l].top);
                                        }
                                        if((board[k][l].count == 4)){
                                            board[k][l].top.setTexture(n4);
                                            board[k][l].top.setPosition(board[k][l].x, board[k][l].y);
                                            gamewin.draw(board[k][l].top);
                                        }
                                        if((board[k][l].count == 5)){
                                            board[k][l].top.setTexture(n5);
                                            board[k][l].top.setPosition(board[k][l].x, board[k][l].y);
                                            gamewin.draw(board[k][l].top);
                                        }
                                        if((board[k][l].count == 6)){
                                            board[k][l].top.setTexture(n6);
                                            board[k][l].top.setPosition(board[k][l].x, board[k][l].y);
                                            gamewin.draw(board[k][l].top);
                                        }
                                        if((board[k][l].count == 7)){
                                            board[k][l].top.setTexture(n7);
                                            board[k][l].top.setPosition(board[k][l].x, board[k][l].y);
                                            gamewin.draw(board[k][l].top);
                                        }
                                        if((board[k][l].count == 8)){
                                            board[k][l].top.setTexture(n8);
                                            board[k][l].top.setPosition(board[k][l].x, board[k][l].y);
                                            gamewin.draw(board[k][l].top);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            gamewin.draw(DebugB.sprite1);

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if(board[i][j].state == "partial"){
                        gamewin.draw(board[i][j].top);
                    }
                }
            }

            for(int i = 0; i<rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if(board[i][j].debbie == true){
                        board[i][j].top.setTexture(bomb);
                        board[i][j].sprite.setTexture(hiddenT);
                        gamewin.draw(board[i][j].sprite);
                        gamewin.draw(board[i][j].top);

                    }
                }
            }

            //gamewin.display();

        }
    }



    return 0;
};

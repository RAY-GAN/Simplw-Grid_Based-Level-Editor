#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <iostream>
#include <fstream>
#include <string>


//Namespaces
using namespace sf;
using namespace std;

//Global variables

const int Num_tile = 21;
const int ScreenSize = 700;
const int TileSize = 70;
const int MatrixSize = ScreenSize / TileSize;

Texture tileTexture[Num_tile];
Sprite tileSprite;
Sprite drawnSprite;

int tile_Array[10][10];
int Mouseposition[2];
int Mousedown;
int cur_Texture = 0;

//prototypes

int handleLoading();
void handleInput(RenderWindow& window, Event& e);
void update(RenderWindow& window);
void render(RenderWindow& window);

int main()
{
    sf::RenderWindow window(sf::VideoMode(ScreenSize, ScreenSize), "Simple Grid-Based Level Editor");
    
    //Load Texture and Sprite
    handleLoading();
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            handleInput(window, event);
           
        }
       
        update(window);

        render(window);
    }
    return 0;
}


// calculate the index of the tile should go to
void GetMousePosition(RenderWindow& window) {
    //column
    Mouseposition[0] = Mouse::getPosition(window).x / TileSize;
    //row
    Mouseposition[1] = Mouse::getPosition(window).y / TileSize;
}


int handleLoading() {
    
    // Load the all the avliable texture into the buffer
    for (int i = 0; i < Num_tile; i++) {
        if (!tileTexture[i].loadFromFile("70x70/Platformer-" + to_string(i) + ".png")) {
            cout << "failed to load the tile" + to_string(i) << std::endl;
            return -1;
        }
    }

    //initialize the 2d array with empty value -1

    for (int i = 0; i < MatrixSize; i++) {

        // go over the column
        for (int j = 0; j < MatrixSize; j++) {

            tile_Array[i][j] = -1;

        }
    }

}


void handleInput(RenderWindow& window, Event& e)
    {
        if (e.type == sf::Event::Closed)
            window.close();

        // Space key creates a screenshot
        else if (e.key.code == Keyboard::Space) {
            Texture texture;
            texture.create(window.getSize().x, window.getSize().y);
            texture.update(window);
            if (texture.copyToImage().saveToFile("Homework 2 Screenshot.png"))
            {
                std::cout << "Screenshot saved to Homework 2 Screenshot.png" << std::endl;
            }
        }

        // save the tile into the text file
        else if (e.key.code == Keyboard::S) {
            ofstream myfile;
            myfile.open("level.text");
            
            // go over the row
            for (int i = 0; i < MatrixSize; i++) {

                // go over the column
                for (int j = 0; j < MatrixSize; j++) {

                    myfile << to_string(tile_Array[i][j]) + " ";
                    
                }

                myfile << "\n";
            }
            
            myfile.close();

            cout << "The tile map is saved";
        }


        //reading from the file
        else if (e.key.code == Keyboard::L) {
            string line;
            ifstream myfile("level.text");
            if (myfile.is_open()) {
                // put the file text into the 2d array
                for (int i = 0; i < MatrixSize; i++) {
                    for (int j = 0; j < MatrixSize; j++) {
                        myfile >> tile_Array[i][j];
                    }
                }

                myfile.close();
            }

            else cout << "unable to open file";
        }


        // change the texture
        else if (e.key.code == Keyboard::Up) {

            if (cur_Texture >= 20)
                cur_Texture = 0;
            else
                ++cur_Texture;

        }

        else if (e.key.code == Keyboard::Down) {

            if (cur_Texture <= 0)
                cur_Texture = 20;
            else
                --cur_Texture;

        }



        GetMousePosition(window);

        // get the mouse input
        if (e.type == Event::MouseButtonPressed)
            Mousedown = 1;
        else if (e.type == Event::MouseButtonReleased)
            Mousedown = 0;

    }

void update(RenderWindow& window)
    {
        // write the texture into the array when mouse pressed
        if (Mousedown == 1)
            tile_Array[Mouseposition[1]][Mouseposition[0]] = cur_Texture;
        
    }

void render(RenderWindow& window)
    {
        window.clear();

        //draw all the previous tiles
        // go over the row
        for (int i = 0; i < MatrixSize; i++) {

            // go over the column
            for (int j = 0; j < MatrixSize; j++) {

                if (tile_Array[i][j] != -1) {

                    int index = tile_Array[i][j];
                    // set the right texture from the loaded png
                    drawnSprite.setTexture(tileTexture[index]);
                    drawnSprite.setPosition(TileSize * j, TileSize * i);
                    window.draw(drawnSprite);
                }

            }
        }

        //set the current texture to the Sprite
        tileSprite.setTexture(tileTexture[cur_Texture]);

        //Set the current tile to the mouse position calculated
        tileSprite.setPosition(TileSize * Mouseposition[0], TileSize * Mouseposition[1]);

        window.draw(tileSprite);

        window.display();
    }
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "CellMap.h"                                       // Import header file for creating map

using namespace std;

SDL_Window *window = NULL;                                                 
SDL_Surface *surface = NULL;

int CELL_MAP_WIDTH = 256;                                   // Number of Cells
int CELL_MAP_HEIGHT = 256;
int BUTTON_HEIGHT = 30;                                     // Buttons displayed at the bottom
int BUTTON_WIDTH =30;
int manual = 1;                                         // Manual(1) or random data(0)
double prob_start = 0.1;                                   // Random Probability
int CELL_SIZE = 32;                                        // Number of pixels in each cell in 1 direction
int snake = 0;                                         // Like the snake game
string TextFile = "trial.txt";

int SCREEN_WIDTH = 512;                                    // Display Screen Width
int SCREEN_HEIGHT = 512;                                   // Display Screen Height
int TOTAL_SCREEN_HEIGHT = 600;                             // Screen Height with the buttons
double delay_time = 0;  


//========================================================================================================================================


void delay(double number_of_seconds)                          // Delay Function
{  
    int milli_seconds = 1000 * number_of_seconds; 
    clock_t start_time = clock(); 
    while (clock() < start_time + milli_seconds); 
} 

int in_rect(int x, int y, struct SDL_Rect *r) {           // Check if (x,y) is in rectangle
    return (x >= r->x) && (y >= r->y) &&
           (x < r->x + r->w) && (y < r->y + r->h);
}

void DrawCell(int x, int y, int colour)                  // Color (x,y)
{
    if((x<SCREEN_WIDTH/CELL_SIZE)&&(y<SCREEN_HEIGHT/CELL_SIZE))                     // Draw only if inside bounds, else ignore
    {    
        Uint8* pixel_ptr = (Uint8*)surface->pixels + (y*CELL_SIZE*SCREEN_WIDTH + x*CELL_SIZE)*4;    // surface variable is a 1D array
                                                                                                    // *4 for RGBA

        for(int i=0; i<CELL_SIZE;i++){
            for(int j=0;j<CELL_SIZE;j++){
                *(pixel_ptr + j*4) = colour;                           // Setting all RGB to the given color
                *(pixel_ptr + j*4 + 1) = colour;
                *(pixel_ptr + j*4 + 2) = colour;
            }
            pixel_ptr += SCREEN_WIDTH*4;                              // Color all pixels for a cell
        }
    }
}


CellMap::CellMap(int width, int height) : w(width), h(height)         // Constructor
{
    length = w*h;
    cells = new char[length];                                         // Each char will store info of a cell --> 5 bits will be used
                                                                      // 0th bit for 'is alive' and next 4 for number of neighbours
    temp_cells = new char[length];
    memset(cells, 0, length);
};

CellMap::~CellMap()                                                    // Destructor
{
    delete[] cells;
    delete[] temp_cells;
}

void CellMap::Reset()                                                   // Reset the map
{
    memset(cells,0,length);
}

void CellMap::SetCell(int x, int y)                                     // Set Cell value to 1 (Cell Alive)
{
    char *cell_ptr = cells + (y*w + x);

    int xleft, xright, yabove, ybelow;

    *(cell_ptr) |= 0x01;

    if(snake)                                                        // Never ending...exits from one end enters from another
    {
        if(x==0)
            xleft = w - 1;
        else
            xleft = -1;

        if(x==w-1)
            xright = -(w-1);
        else
            xright = 1; 

        if(y==0)
            yabove = length - w;
        else
            yabove = -w;
        
        if(y==h-1)
            ybelow = w-length;
        else
            ybelow = w;


        *(cell_ptr + yabove + xleft) += 0x02;                       // Increment # of neighbours for all neighbours
        *(cell_ptr + yabove) += 0x02;
        *(cell_ptr + yabove + xright) += 0x02;
        *(cell_ptr + xright) += 0x02;
        *(cell_ptr + ybelow + xright) += 0x02;
        *(cell_ptr + ybelow) += 0x02;
        *(cell_ptr + ybelow + xleft) += 0x02;
        *(cell_ptr + xleft) += 0x02;
    }

    else
    {
        if(x==0)
            xleft = 0;
        else
            xleft = -1;

        if(x==w-1)
            xright = 0;
        else
            xright = 1; 

        if(y==0)
            yabove = 0;
        else
            yabove = -w;
        
        if(y==h-1)
            ybelow = 0;
        else
            ybelow = w;

        if(yabove&&xleft)
            *(cell_ptr + yabove + xleft) += 0x02;                       // Increment # of neighbours for all neighbours
        if(yabove)
            *(cell_ptr + yabove) += 0x02;
        if(yabove&&xright)
            *(cell_ptr + yabove + xright) += 0x02;
        if(xright)
            *(cell_ptr + xright) += 0x02;
        if(ybelow&&xright)
            *(cell_ptr + ybelow + xright) += 0x02;
        if(ybelow)
            *(cell_ptr + ybelow) += 0x02;
        if(ybelow&&xleft)
            *(cell_ptr + ybelow + xleft) += 0x02;
        if(xleft)
            *(cell_ptr + xleft) += 0x02;
    }
    
}

void CellMap::ClearCell(int x, int y)                           // Clear cell value to 0 (Cell Dead)
{
    char *cell_ptr = cells + (y*w + x);

    int xleft, xright, yabove, ybelow;

    *(cell_ptr) &= ~0x01;

    if(snake==1)                                                        // Never ending...exits from one end enters from another
    {
        if(x==0)
            xleft = w - 1;
        else
            xleft = -1;

        if(x==w-1)
            xright = -(w-1);
        else
            xright = 1; 

        if(y==0)
            yabove = length - w;
        else
            yabove = -w;
        
        if(y==h-1)
            ybelow = w-length;
        else
            ybelow = w;


        *(cell_ptr + yabove + xleft) -= 0x02;                       // Increment # of neighbours for all neighbours
        *(cell_ptr + yabove) -= 0x02;
        *(cell_ptr + yabove + xright) -= 0x02;
        *(cell_ptr + xright) -= 0x02;
        *(cell_ptr + ybelow + xright) -= 0x02;
        *(cell_ptr + ybelow) -= 0x02;
        *(cell_ptr + ybelow + xleft) -= 0x02;
        *(cell_ptr + xleft) -= 0x02;
    }

    else
    {
        if(x==0)
            xleft = 0;
        else
            xleft = -1;

        if(x==w-1)
            xright = 0;
        else
            xright = 1; 

        if(y==0)
            yabove = 0;
        else
            yabove = -w;
        
        if(y==h-1)
            ybelow = 0;
        else
            ybelow = w;

        if(yabove&&xleft)
            *(cell_ptr + yabove + xleft) -= 0x02;                       // Increment # of neighbours for all neighbours
        if(yabove)
            *(cell_ptr + yabove) -= 0x02;
        if(yabove&&xright)
            *(cell_ptr + yabove + xright) -= 0x02;
        if(xright)
            *(cell_ptr + xright) -= 0x02;
        if(ybelow&&xright)
            *(cell_ptr + ybelow + xright) -= 0x02;
        if(ybelow)
            *(cell_ptr + ybelow) -= 0x02;
        if(ybelow&&xleft)
            *(cell_ptr + ybelow + xleft) -= 0x02;
        if(xleft)
            *(cell_ptr + xleft) -= 0x02;
    }
    
}

int CellMap::CellState(int x, int y)                                    // Check state of cell
{
    char *cell_ptr = cells + (y*w) + x;

    return *cell_ptr & 0x01;
}

void CellMap::Init(double prob_start)                                   // Initialize if manual set to 0
{
    srand(time(0));                                                     // Set random seed each time
    int x,y;

    for(int i=0;i<length*prob_start;i++)
    {
        x = rand() % w;
        y = rand() % h;
        if(!CellState(x,y))
        {
            SetCell(x,y);
            DrawCell(x,y,0x00);
        }
    }
}

void CellMap::Text()                                                        // Import a particular pattern
{
    ifstream inFile;
    inFile.open(TextFile);
    string line;
    int y=0;
    while(getline(inFile,line))
    {
        int length = line.length();
        for(int x=0;x<length;x++)
        {
            char ch=line.at(x);
            if(ch == 'O')
            {
                SetCell(x,y);
                DrawCell(x,y,0x00);
            }
        }
        y++;
    }
}

void CellMap::NextGen()                                             // Calculate next gen based on info given
{
    int x,y,live_neighbours;
    char* cell_ptr;
    memcpy(temp_cells, cells, length);

    cell_ptr = temp_cells;

    for(int y=0; y<h; y++)
    {
        x = 0;
        do
        {
            while(*cell_ptr==0)                                    // If no neighbours and dead, then *cell_ptr = 00000 and no change needed, hence skip
            {
                cell_ptr++;                                        // Increment Pointer
                
                if(++x >= w) goto NextRow;                         // Go to next row at the end of the row
            }

            live_neighbours = *cell_ptr>>1;                        // Right shifting the cell_ptr to get the number of live_neighbours
            if(*cell_ptr & 0x01)                                   // Check if cell is alive
            {
                if((live_neighbours!=2)&&(live_neighbours!=3))    
                {
                    ClearCell(x,y);
                    DrawCell(x,y,0xFF);
                }
            }
            else                                                       // If cell dead
            {
                if(live_neighbours==3)
                {
                    SetCell(x,y);
                    DrawCell(x,y,0x00);
                }
            }
            
            cell_ptr++;                                                 // Increment Cell Pointer

         } while(++x<w);
        
        NextRow:;
    }
}


void CellMap::Scope()                                                   // To zoom in and out
{
    int x,y,live_neighbours;
    char* cell_ptr;
    memcpy(temp_cells, cells, length);

    cell_ptr = temp_cells;

    for(int y=0; y<h; y++)                                              // Redraw the cells with the current cell_size
        for(int x=0;x<w;x++)
        {
            if(*cell_ptr&0x01)
            {
                DrawCell(x,y,0x00);
            }
            else
            {
                DrawCell(x,y,0xFF);
            }
            cell_ptr++;
        }
}


void Clear_Screen(SDL_Rect Start,SDL_Rect Stop,SDL_Rect Re,SDL_Rect Dash,SDL_Rect Snake,SDL_Rect Glider,SDL_Rect GG,SDL_Rect Simkin, SDL_Rect Pulsar, int manual, int snake)
{
            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255)); 
            SDL_FillRect(surface, &Start, SDL_MapRGB(surface->format, 0, 255, 0));
            SDL_FillRect(surface, &Stop, SDL_MapRGB(surface->format, 255, 0, 0));
            SDL_FillRect(surface, &Re, SDL_MapRGB(surface->format, 255, 255, 0));
            switch(manual)                                                                                              // Clear Screen
            {
                case(0) : SDL_FillRect(surface, &Dash, SDL_MapRGB(surface->format, 144,238,144));
                break;
                case(1) : SDL_FillRect(surface, &Dash, SDL_MapRGB(surface->format, 255,182,193));
                break;
                case(2) : SDL_FillRect(surface, &Dash, SDL_MapRGB(surface->format, 173,216,230));
                break;
            }

            switch(snake)
            {
                case(0) : SDL_FillRect(surface, &Snake, SDL_MapRGB(surface->format, 192,192,192));
                break;
                case(1) : SDL_FillRect(surface, &Snake, SDL_MapRGB(surface->format, 255,69,0));
                break;
            }
            SDL_FillRect(surface, &Glider, SDL_MapRGB(surface->format, 182, 137, 214));
            SDL_FillRect(surface, &GG, SDL_MapRGB(surface->format, 153, 105, 199));
            SDL_FillRect(surface, &Pulsar, SDL_MapRGB(surface->format, 128, 79, 179));
            SDL_FillRect(surface, &Simkin, SDL_MapRGB(surface->format, 106, 53, 156));
            
}

//======================================================================================================================================

int main(int arg, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);                                   // Initializes the SDL and event worksystem
    window = SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,TOTAL_SCREEN_HEIGHT,SDL_WINDOW_SHOWN);   // Creating window
    surface = SDL_GetWindowSurface(window);                     // Creating surface for the window

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));                // Fill background with white

    SDL_Rect StartButton = {120,SCREEN_HEIGHT+30,BUTTON_WIDTH,BUTTON_HEIGHT};               // Template for START Button
    SDL_FillRect(surface, &StartButton, SDL_MapRGB(surface->format, 0, 255, 0));

    SDL_Rect StopButton = {370,SCREEN_HEIGHT+30,BUTTON_WIDTH,BUTTON_HEIGHT};                // Template for STOP Button
    SDL_FillRect(surface, &StopButton, SDL_MapRGB(surface->format, 255, 0, 0));

    SDL_Rect ReButton = {245,SCREEN_HEIGHT+30,BUTTON_WIDTH,BUTTON_HEIGHT};                  // Template for Reset Button
    SDL_FillRect(surface, &ReButton, SDL_MapRGB(surface->format, 255, 255, 0));

    SDL_Rect Dash = {0,SCREEN_HEIGHT,SCREEN_WIDTH,20};                                      // Template for Manual/Random Button
    if(manual==1)
        SDL_FillRect(surface, &Dash, SDL_MapRGB(surface->format, 255,182,193));             // Pink if manual
    else if(manual==0)
        SDL_FillRect(surface, &Dash, SDL_MapRGB(surface->format, 144,238,144));                 // Dark Green if random
    else
        SDL_FillRect(surface, &Dash, SDL_MapRGB(surface->format, 173,216,230));                 // Blue if Tect File
    
    
    SDL_Rect Snake_Button = {0,SCREEN_HEIGHT+20,20,68};                                     // Template for Snake Button
    if(snake)
        SDL_FillRect(surface, &Snake_Button, SDL_MapRGB(surface->format, 255,69,0));             // Orange if snake
    else
        SDL_FillRect(surface, &Snake_Button, SDL_MapRGB(surface->format, 192,192,192));

    SDL_Rect GliderButton = {20,TOTAL_SCREEN_HEIGHT-20,(SCREEN_WIDTH-20)/4,20};                // Template for GLIDER Button
    SDL_FillRect(surface, &GliderButton, SDL_MapRGB(surface->format, 181,137,214));

    SDL_Rect GGButton = {20+(SCREEN_WIDTH-20)/4,TOTAL_SCREEN_HEIGHT-20,(SCREEN_WIDTH-20)/4,20};                // Template for GG Button
    SDL_FillRect(surface, &GGButton, SDL_MapRGB(surface->format, 153, 105, 199));

    SDL_Rect PulsarButton = {20+(SCREEN_WIDTH-20)/2,TOTAL_SCREEN_HEIGHT-20,(SCREEN_WIDTH-20)/4,20};                // Template for Pulsar Button
    SDL_FillRect(surface, &PulsarButton, SDL_MapRGB(surface->format, 128, 79, 179));

    SDL_Rect SimkinButton = {20+((SCREEN_WIDTH-20)/4)*3,TOTAL_SCREEN_HEIGHT-20,(SCREEN_WIDTH-20)/4,20};                // Template for Simkin Button
    SDL_FillRect(surface, &SimkinButton, SDL_MapRGB(surface->format, 106, 53, 156));

    SDL_Event e;

    CellMap map(CELL_MAP_WIDTH, CELL_MAP_HEIGHT);                                           // Define an object of type CellMap
    
    int Implement = 0;                                                                      // If Implement=0:Stop, Implement=1:Run, Implement=2:Reset
    
    bool quit = false;                                                                      
    int generation=0;                                                                       // Number of generations passed
 
    while(!quit)                                                                            
    {
        while(SDL_PollEvent(&e) != 0)                                                       // While event list is not empty
            if (e.type==SDL_QUIT)                                                           // When window is closed
                quit = true;
            else if(e.type == SDL_MOUSEBUTTONDOWN)                                          // Check for mousebutton event
            {
                if((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &StartButton)))                  // Implement START Button
                    Implement = 1;
                else if(((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &StopButton))))            // Implement STOP Button
                    Implement = 0;
                else if((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &ReButton)))                // Implement RESET Button
                    Implement = 2;
                else if((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &Dash))&&(Implement==0))    // Implement MANUAL Line
                {
                    manual = (manual+1)%3;
                    if(manual==1)
                    {
                        Clear_Screen(StartButton,StopButton,ReButton,Dash,Snake_Button,GliderButton,GGButton,SimkinButton,PulsarButton,manual,snake);
                        map.Reset();                                                                        
                    }
                    else if(manual==2)
                    {
                        Clear_Screen(StartButton,StopButton,ReButton,Dash,Snake_Button,GliderButton,GGButton,SimkinButton,PulsarButton,manual,snake);
                        map.Reset();
                        map.Text(); 
                    }              
                     else if(manual==0)
                     {
                         Clear_Screen(StartButton,StopButton,ReButton,Dash,Snake_Button,GliderButton,GGButton,SimkinButton,PulsarButton,manual,snake);
                        map.Reset();
                        map.Init(prob_start);
                     }
                }

                else if((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &GliderButton))&&(manual==2))            
                    {
                        TextFile = "Glider.txt";
                        Clear_Screen(StartButton,StopButton,ReButton,Dash,Snake_Button,GliderButton,GGButton,SimkinButton,PulsarButton,manual,snake);
                        map.Reset();
                        map.Text();
                    }
                else if((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &GGButton))&&(manual==2))            
                    {
                        TextFile = "GG.txt";
                        Clear_Screen(StartButton,StopButton,ReButton,Dash,Snake_Button,GliderButton,GGButton,SimkinButton,PulsarButton,manual,snake);
                        map.Reset();
                        map.Text();
                    }
                else if((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &SimkinButton))&&(manual==2))            
                    {
                        TextFile = "Simkin.txt";
                        Clear_Screen(StartButton,StopButton,ReButton,Dash,Snake_Button,GliderButton,GGButton,SimkinButton,PulsarButton,manual,snake);
                        map.Reset();
                        map.Text();
                    }
                else if((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &PulsarButton))&&(manual==2))            
                    {
                        TextFile = "Pulsar.txt";
                        Clear_Screen(StartButton,StopButton,ReButton,Dash,Snake_Button,GliderButton,GGButton,SimkinButton,PulsarButton,manual,snake);
                        map.Reset();
                        map.Text();
                    }
                else if((e.button.button == SDL_BUTTON_LEFT)&&(in_rect(e.button.x, e.button.y, &Snake_Button))&&(Implement==0))    
                {
                    snake=!snake;
                    if(snake)
                      SDL_FillRect(surface, &Snake_Button, SDL_MapRGB(surface->format, 255,69,0));
                    else
                      SDL_FillRect(surface, &Snake_Button, SDL_MapRGB(surface->format, 192,192,192));
                }
                
                else if((e.button.button == SDL_BUTTON_LEFT)&&(manual==1))                                                 // Fill cells if manual = 1
                {
                        map.SetCell(static_cast<int>(e.button.x/CELL_SIZE), static_cast<int>(e.button.y/CELL_SIZE));
                        DrawCell(static_cast<int>(e.button.x/CELL_SIZE), static_cast<int>(e.button.y/CELL_SIZE), 0x00);
                }
            
            }
            else if(e.type==SDL_MOUSEWHEEL)                                                      // Check for mouse scroll
            {
                if((e.wheel.y > 0)&&(CELL_SIZE<=SCREEN_WIDTH/8))                                // Zoom in by 2X
                {
                    CELL_SIZE = CELL_SIZE*2;
                    map.Scope();
                }
                else if((e.wheel.y < 0)&&(CELL_SIZE>=4))                                        // Zoom out by 2X
                {
                    CELL_SIZE = CELL_SIZE/2;
                    map.Scope();
                }

            }
        
        if(Implement==2)                                                                        // Reset Map
        {
            generation = 0;
            Clear_Screen(StartButton,StopButton,ReButton,Dash,Snake_Button,GliderButton,GGButton,SimkinButton,PulsarButton,manual,snake);
            map.Reset();
            Implement = 0;
        }

        if(Implement==1)                                                                        // Run the game
        {
            if ((generation==0)&&(manual==0))
                map.Init(prob_start);
             
            delay(delay_time);
            map.NextGen();
            generation++;
        }


        SDL_UpdateWindowSurface(window);                                                       // Update window interface
 
    }

    SDL_DestroyWindow(window);                                                                 // Clean Memory
    SDL_Quit();

    return 0;
}
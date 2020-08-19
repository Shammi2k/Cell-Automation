We are executing the famous Conway's Game Of Life.

In this program, I am using SDL2(Simple DirectMedia Layer) for GUI on CPP under the environment of MinGW.

The download link and instuctions for SDL2 --> https://programmer.help/blogs/vscode-sdl2-configuration-tutorial.html

The final GUI and all the buttons are described in the snippet file named 'GUI.jpg' attached

The change 'input method' has 3 colors -->
    
        1) Light Red : Manually filling the pixels.

        2) Light Blue : Input any of the provided patterns. Use the purple gradient below for choosing the pattern.
                        From left to right, the patterns in the gradient are 'Glider', 'Gosper's Gun', 'Pulsar' and
                        'Simkin's double barrel' respectively.

        3) Light Green : It will auto input a randomized plot.

        
        Once done with it, use the 'GREEN' Button to run the game. To pause, press the 'RED' Button and 'YELLOW' Button
        to Reset. You can change the 'input method' in the 'RESET' as well as 'PAUSE' mode. But if you change the 
        'input method' in 'PAUSE' mode, the program will reset.


        The 'SNAKE' Button comes from the mobile game SNAKE. If the SNAKE Button is ORANGE, it means that the mode is on
        and if you exit the graph from below you enter from above and same for all other directions. If the 'SNAKE' Button
        is off, represented by the Button being 'GRAY', the boundaries act as walls and dont allow further movement.

        Use the 'SCROLLWHEEL' for zoom in and out.


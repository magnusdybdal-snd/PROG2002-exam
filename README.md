# Reference



## Requirements implemented

**R 1.1**
Since the side walls are the same size we use one VAO for them and another VAO for the back wall. Each wall is given its place by using seperate model matrices. Added Instanced rendering to the rendering tools we developed during assignment to use instanced drawing for the 4 side walls. Uploaded the matrices to an array in shader and used opengl gl_InstanceID to draw in two draw calls (back wall and other walls)

**R 1.2**
Changed window size in constructor. To ensure game is centered camera is looking at origin, where all the pieces is moved in relation from.
Note: Misread the way tasks where split up. All tasks for task 1 was done in one branch, therefore no branch connected to this issue, same goes for task 1.3

**R 1.3**
The color is set to green directly in the shader since it will be static for the whole game. To create the borders I have used blending to display only the edges with alpha 1.0, the rest of the square has alpha 0.0.
This is done in the fragment shader by setting a border width and using step to make a hard cutoff from 0.0 to 1.0 whenever we are within the borders coordinates.
Note: Misread the way tasks where split up. All tasks for task 1 was done in one branch, therefore no branch connected to this issue.

**R 2.1**
Used the geometric tool UnitCube made during assignment to make one unit cube with super simple shader (semi transparent). Changed some of the values used to construct my tube so that it became easier to place the cube within the grid spaces / navigational grid. Placed cube in front, bottom, center

**R 2.2**
Not much to say here. Changed color to something pleasing and made it transparent.

**R 2.3**
Reused movement system implemented in assignemnt checking for arrow key presses

**R 2.4**
Added a check on X press and applied transform to the cube in negative z direction
For the space key I added a variable to the cube to keep track of its z position within the grid/tunnel, then calculate the distance to the end and apply that transformation.

The movement every 2 sec uses another member variable that keeps track of the time since last movement and simply applies one transform every time the timer exceeds 2 seconds using glfwGetTime()
Additionally made it so when pressing X the timer resets, so we dont get double moves that would make gameplay frustrating.

**R 2.5**
Added glm::ivec3 to active block to keep track of grid position in tunnel, simply checking before allowing movement by keys.
Added a new shader/VAO setup for solid blocks. 

When you would hit the wall we call a helper function
void ExamApplication::MakeActiveCubeSolid() makes an instance of a solid block that is defined in the h file and copies over the attributes it needs from the active block. 

Color is decided with a modulo within another helper function
glm::vec3 ExamApplication::GetColorForSolidBlock(int zPos)

**R 2.6**
To respawn the active block we simply move it to the start position when it collides
To detect collision and make block solid I made two functions

One detects if a coordinate is occupied, handles both walls and solid blocks
The other one determines if a block should become solid on the next move. This makes it so we can become solid on z axis collision but restrict movement on x and y if a block is in the way.

For the spacebar key press we use the same system but we have to go trough every tile from the active block, all the way to the wall to check for collisions on the way

**R 3.1**
Found textures for walls and cubes and uploaded to resources/textures

Modified tunnel verticies and bufferlayout to also include texture coordinates using the Geometric tool from assignment

Used texture manager from framework made in assignment to initiate and upload the textures.

Modified shaders to use texture coordinates with sampler to generate texture color and blend it using mix.

To preserve the borders of the tunnel that I made earlier in the exam I did 2x mix in the wall fragment shader. One to blend the texture and the color, and then another using the texture blend and the border color with a hard cutoff mix (0.0 or 1.0)

This makes it so the borders have no texture (100% blend) and the textureblend has no border color in it (0% blend)

**R 3.2**
Added a global bool and input check for T press. The variable is uploaded to the wall and solid block fragment shader as a uniform.

In the shaders we check for the flag and decides if the fragcolor should be a solid color or a blend with texture.

Note: This was completed in the last issue together with textures, therefore only one commit on this branch / issue.

**R 4.1**
Implemented lighting using the PHONG model:
- ambient light / background light
- diffuse lighting with the active block as light source       
- specular light with the active block as light source

To make the solid blocks look more metallic than the walls of the tunnel I upped the shine factor and specular strength in the shaders for the solid blocks.

Updated the gemoetric tool used for the cubes to include normals as well and updated the buffer layout.

Had some trouble when adding the light in the final frag color in the shaders. This was because I used blending to make the walls transparent when not in texture mode. because blending is enabled I cannot just multiply the vec4 with the lighting value as we did in the assignment because this will affect the alpha as well. The solution was to extract the rgb values, multiply with those and then add the alpha in after.

**R 5.1**
Made 3 functions to setup the arrangment of blocks to make the shapes, still using the 1x1 block.
Refactored all movement inputs to move the whole piece. This is done by directly manipulating their world coordinates and grid position (struct members).

Used instanced rendering for the active piece. Always 4 pieces so can use the same method as for walls. Translation when making the matrices can then directly use the worldgrid position of each cube.

Finally refactored the respawning of pieces to randomize which shape is spawned. Also changed how blocks solidifies by wrapping the old functionality in a for loop and using the data already stored within each block of the complex piece (worldpos and gridpos)

**R 5.2**
This has mostly been solved in previous issues. The function used for solidifying blocks is just wrapped in a for loop iterating trough the blocks in the activePiece vector

Changed rotation of initial L piece to test, and the blocks seem to take the correct color.
Solid blocks still uses 1 draw call each.

**R 5.3 and 5.4**
5.3 and 5.4
Added checks for q,w,e,a,s,d in input handler function. They each call the respective function for pitch, roll yaw with true/false for direction.

I first tried to apply rotation by using glm::rotate but that would not work well. Then I realized im already rendering based on wolrd position that every active block is assigned and decided to manipulate them directly.

First I decide on a pivot point in the piece. thats the 2nd block in the vector, which is set as the most central block in the piece. I then calculate the relative difference in grid space to that block.

Then we calculate the new values for the axises we are not rotating around by multiplying the opposite coordinate with the direction using this formula
(x, y) → (-y, x). (this also works for the other axis, just swap out the one we are rotating around).

After checking if we can rotate for all blocks using the same collision check function on the new positions we apply the movement if no collision is detected.
## (1/3) General graph algorithms

### Description
In this project we had to implement different general graph theory algorithms. The project consists of 4 parts as follows:
1. General graph representation read from an adjacency list
2. Solving a labyrinth by determining the exit points, sorting them by distance and the finding the shortest path to them using A* algorithm
3. Determining the topological sort (order + graph), connected components & strongly connected components
4. Determining the minimum spanning tree

### Visualizers (User Interface)
The user has the possibility to select one of the 4 visualizers from the console window, corresponding to the 4 parts described above:
<br>![](showcase/L1_menu_0.png)
<br>
The visualizers are as follows:
1. General graph representation
<br><img src="showcase/L1_view_1.png" alt="" width="400px">
2. Labyrinth

|![](showcase/L1_view_2.gif)|![](showcase/L1_view_2.png)|
|:-:|:-:|

3. Topo, CC, SCC
<img src="showcase/L1_menu_3.png" alt="" width="600px">

|![](showcase/L1_view_3_1.png)|![](showcase/L1_view_3_2.png)|
|:-:|:-:|
4. MST
<br>![](showcase/L1_view_4.png)

### Input Data
The input data for each part is as follows:
1. An adjacency list (where each line represents the connections a node has and @ represents no connections for the specific node)
<br>![](showcase/L1_view_1_in.png)
2. A matrix with a given size, representing the labyrinth's structure. (0 = wall; 1 = free; 2 = source)
<br>![](showcase/L1_view_2_in.png)
3. An adjacency list (same as pt. 1)
<br>![](showcase/L1_view_3_in.png)
4. An adjacency matrix (where matrix[i][j] position represents the weight of the arc that goes from node i to node j, or -1 if no such arc exists)
<br>![](showcase/L1_view_4_in.png)

### Project Structure
|Structure|Details|
|:-|:-|
|<img src="showcase/L1_structure.png" alt="" width="300px">|- **Header files**: Declarations of used classes<br>- **Resource files**: Static resources, mostly input files<br>- **Source Files**: Definitions of the prior declared classes|

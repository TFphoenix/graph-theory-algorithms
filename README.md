


<br>

# General Presentation
This repository represents the work I've done for the Graph Theory Algorithms laboratories, in 2019, at Transylvania University of Brașov, Romania.

## Purpose
In order to pass this laboratory, we had to do 3 projects. Each project consisted of implementations of graph theory algorithms learned at class, adding visualizers to each of them:
1. **[General graph algorithms](Algoritmica%20Grafurilor%20App)**: In this project we had to implement different general graph theory algorithms, such as: a general graph representation, solving a labyrinth, topological sort, connected & strongly connected components, etc.
2. **[Luxembourg](Luxembourg)**: In this project we had to use a path detection algorithm, in order to find the shortest available path between two selected points on a Luxembourg map loaded and rendered accordingly (from read nodes and vertices)
3. **[Flow network](Fluxuri)**: In this project we had to implement the minimum and maximum value flow network graph algorithms

## Visualizers showcase

Some of the visualizers are displayed below:

|<img src="showcase/L1_view_2.png" alt="" width="800px"><br>Labyrinth|<img src="showcase/L2_view_2.png" alt="" width="800px"><br>Luxembourg|
|:-:|:-:|
|<img src="showcase/L3_view_3.png" alt="" width="800px"><br>**Flow network**|<img src="showcase/L1_view_3_1.png" alt="" width="800px"><br>**Strongly connected<br>components**|

## Technologies and dependencies
For the implementation of these projects I've used the following technologies and dependencies:
- **Visual Studio**: IDE
- **C++**: Programming language
- **[SFML](https://www.sfml-dev.org/index.php)**: A lightweight multi-media C++ library used for fast rendering, visual programming and media management
- **[RapidXML](http://rapidxml.sourceforge.net/)**: A lightweight XML C++ library used for reading and parsing XML files

<br><br><br>

# Projects

## (1/3) General graph algorithms
> Location: [1/3 General graph algorithms](Algoritmica%20Grafurilor%20App)

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

## (2/3) Luxembourg
> Location: [2/3 Luxembourg](Luxembourg)

### Description
In this project we had to use a path detection algorithm (I used A*), in order to find the shortest available path between two selected points on a Luxembourg map loaded and rendered accordingly (from XML read nodes and vertices)

### Visualizer (User Interface)
The user has the possibility to select two points on the map, and the shortest path will pe displayed as soon as the second point is selected and the calculations are finished. Map rendering, points determination and path finding times are displayed in the console window.
|![](showcase/L2_view_0.png)|![](showcase/L2_view_1.png)|
|:-:|:-:|
![](showcase/L2_view_2.png)

### Input Data
The input data consists of a XML file, containing all the nodes and arcs (vertices) of a given map (Luxembourg in this case), and their corresponding details:
<br>![](showcase/L2_view_in_0.png)
|![](showcase/L2_view_in_1.png)|![](showcase/L2_view_in_2.png)|
|:-:|:-:|

### Project Structure
|Structure|Details|
|:-|:-|
|<img src="showcase/L2_structure.png" alt="" width="300px">|- **Graph**: Declarations and definitions of the classes used for graph theory logic and algorithms<br>- **Resource files**: Static resources (xml maps)<br>- **Utility**: Utility classes used for different purposes (such as the rapidxml library, used for reading and parsing the XML files)|

## (3/3) Flow network
> Location: [3/3 Flow network](Fluxuri)

### Description
In this project we had to implement the minimum and maximum value flow network graph algorithms. The algorithms I've implemented also determine and display the minimal cut, the flow optimization points and other auxiliary information.

### Visualizer (User Interface)
The user has the possibility to load a specific graph, by pressing one of the two buttons on the left top corner of the window. After choosing a graph, the selected graph will be loaded and rendered and the user will have the possibility to apply the selected algorithm, by pressing the 3rd button. After that, the algorithm will be applied and the results will be displayed accordingly.
|![](showcase/L3_view_0.png)|![](showcase/L3_view_1.png)|
|:-:|:-:|
|![](showcase/L3_view_2.png)|![](showcase/L3_view_3.png)|
|![](showcase/L3_view_4.png)||

### Input Data
The input data consists of adjacency lists in which every detail needed for each algorithm are given (such as: number of nodes, source node, destination node, arcs, arc flows, arc flow capacities, etc.)
<br>![](showcase/L3_view_in_1.png)
<br>![](showcase/L3_view_in_0.png)

### Project Structure
|Structure|Details|
|:-|:-|
|<img src="showcase/L3_structure.png" alt="" width="300px">|- **Graph**: Declarations and definitions of the classes used for graph theory logic and algorithms<br>- **Input**: Static resources (input files)<br>- **Utility**: Utility classes used for different purposes (such as the rapidxml library, used for reading and parsing the XML files)<br>- **Source**: Source/launch (main function)<br>- **UI**: UI components used (such as UIButton)|

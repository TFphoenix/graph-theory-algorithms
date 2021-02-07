## (3/3) Flow network

### Description
In this project we had to implement the minimum and maximum value flow network graph algorithms. The algorithms I've implemented also determine and display the minimal cut, the flow optimization points and other auxiliary information.

### Visualizer (User Interface)
The user has the possibility to load a specific graph, by pressing one of the two buttons on the left top corner of the window. After choosing a graph, the selected graph will be loaded and rendered and the user will have the possibility to apply the selected algorithm, by pressing the 3rd button. After that, the algorithm will be applied and the results will be displayed accordingly.
|![](../showcase/L3_view_0.png)|![](../showcase/L3_view_1.png)|
|:-:|:-:|
|![](../showcase/L3_view_2.png)|![](../showcase/L3_view_3.png)|
|![](../showcase/L3_view_4.png)||

### Input Data
The input data consists of adjacency lists in which every detail needed for each algorithm are given (such as: number of nodes, source node, destination node, arcs, arc flows, arc flow capacities, etc.)
<br>![](../showcase/L3_view_in_1.png)
<br>![](../showcase/L3_view_in_0.png)

### Project Structure
|Structure|Details|
|:-|:-|
|<img src="../showcase/L3_structure.png" alt="" width="300px">|- **Graph**: Declarations and definitions of the classes used for graph theory logic and algorithms<br>- **Input**: Static resources (input files)<br>- **Utility**: Utility classes used for different purposes (such as the rapidxml library, used for reading and parsing the XML files)<br>- **Source**: Source/launch (main function)<br>- **UI**: UI components used (such as UIButton)|

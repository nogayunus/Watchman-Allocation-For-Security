#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/*
--IMPLEMENTATION REPORT--
I used graph data-type implementation when i solve this watchman-allocation-for-security problem.
At first i take size of graph which is equal number of hallway junctions of the single storey building in my problem.
I create graph that given size then ask for junctions and these hallways that connected with current junction.
When i designing my graph i used craeteHallway() method.
In this method i create hallways according to given junction and hallways of this junction. I need to check all hallways in my graph
because in my addEgde() method that connects junction and hallway, i connects both in my graph so i should not create again.
For example if i craete edge [1-2], it means i create also [2-1] in my graph so program should check [2-1] for junction 2's hallways.
Thats why i used checkVertex() method.
After taking these informations, my graph designed.
Then my main part of algorithm started.
I kept the number of hallways where junctions  are connected in an integer array pointer.
Then I ran my allocateJunctions() method.
In this method, I find the maximum number of junctions used initially.
Then I add the junction used in the graph to a new graph, starting with the maximum number used in a for loop.
In this way, by controlling the junctions that occur while going from maximum to minimum, I reach all hallways by using junctions at minimum level.
In the end, i print all of these informations.

*/


// A structure to represent an adjacency list node
struct AdjListNode {
	int dest;
	struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList {
	struct AdjListNode* head;
};

// A structure to represent a graph. A graph
// is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph {
	int V;
	struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
	struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates a graph of V vertices
// Big O Notation of given method : O(N)
struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists. Size of
	// array will be V
	graph->array = (struct AdjList*)malloc(
		V * sizeof(struct AdjList));

	// Initialize each adjacency list as empty by
	// making head as NULL
	int i;
	for (i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph
// Big O Notation of given method : O(N)
void addEdge(struct Graph* graph, int src, int dest)
{
	// Add an edge from src to dest. A new node is
	// added to the adjacency list of src. The node
	// is added at the beginning
	struct AdjListNode* check = NULL;
	struct AdjListNode* newNode = newAdjListNode(dest);

	if (graph->array[src].head == NULL) {
		newNode->next = graph->array[src].head;
		graph->array[src].head = newNode;
	}
	else {

		check = graph->array[src].head;
		while (check->next != NULL) {
			check = check->next;
		}
		// graph->array[src].head = newNode;
		check->next = newNode;
	}

	// Since graph is undirected, add an edge from
	// dest to src also
	newNode = newAdjListNode(src);
	if (graph->array[dest].head == NULL) {
		newNode->next = graph->array[dest].head;
		graph->array[dest].head = newNode;
	}
	else {
		check = graph->array[dest].head;
		while (check->next != NULL) {
			check = check->next;
		}
		check->next = newNode;
	}

}


// Checks vertexes that added before. If vertex exist returns true else false.
// Big O Notation of given method : O(N)
bool checkVertex(struct Graph* graph,int src,int dest){
    struct AdjListNode* srcHead = graph->array[src].head;
    struct AdjListNode* destHead = graph->array[dest].head;

    bool checked = false;
    if(graph->array[src].head == NULL || graph->array[dest].head == NULL){
        return checked;
    }
        while(srcHead){
            if(srcHead->dest == dest){
                checked = true;
                return checked;
            }else{ srcHead = srcHead->next;}
        }
         while(destHead){
            if(destHead->dest == src){
                checked = true;
                return checked;
            }else{ destHead = destHead->next;}
        }
        return checked;

}

// Finds max integer value given integer array pointer.
int findMax(int *array,int size){
    int max = array[1];



    for(int i=0;i<size;i++){
      if(max < array[i]){
        max = array[i];
      }

    }
    return max;


}

// Checks hallway connection count in given junction and returns this count.
// Big O Notation of given method : O(N^2)
int checkAllocation(struct Graph* graph,int index){
    int count = 0;
    for(int i=1;i<graph->V;i++){
        struct AdjListNode* nodeHead = graph->array[i].head;

        while(nodeHead){
            if(graph->array[i].head == index){
                count++;
            }
            if(nodeHead->dest == index){
                count++;
            }
            nodeHead = nodeHead->next;
        }
    }
    return count;


}

// Main method of algorithm that executes solve watchman-allocation-for-security problem.
// Big O Notation of given method : O(N^3)
int allocateJunctions(struct Graph* graph,struct  Graph* originalGraph,int *arr,int *resultArr){
    int max = findMax(arr,graph->V);
    int numberOfWatchman = 0;
    int end = 0;
    //printf("%d\n",max);
    for(int i=max;i>0;i--){

        for(int index=1;index<graph->V;index++){
            int temp = arr[index] - checkAllocation(graph,index);
            if(temp == 0){
                arr[index] = 0;
            }
            if(temp == i){
                    struct AdjListNode* nodeHead = originalGraph->array[index].head;

                    while(nodeHead){
                        addEdge(graph,index,nodeHead->dest);
                        nodeHead = nodeHead->next;
                    }
                    arr[index] = 0;
                    resultArr[index] = index;
                    if(end < index+1){
                        end = index+1;
                    }

                    numberOfWatchman++;


            }
        }

    }
    resultArr[end] = -2;
    return numberOfWatchman;

}

// Adds edges given graph in parameter by using program input.
// Big O Notation of given method : O(N)
void createHallway(struct Graph* graph,int *countOfHallway,int junctionID,char *dests){
   dests = strtok(dests," ");
   int dest = atoi(dests);
   int count = 0;
    while(1){
        if(dest==0){break;}
        bool check = checkVertex(graph,junctionID,dest);
        if(check == false){
            addEdge(graph,junctionID,dest);
        }
        count++;
        dests = strtok(NULL," ");
        dest = atoi(dests);

    }

    countOfHallway[junctionID] = count;
}

// Driver program to test above functions
int main()
{
    char *size = (char*) malloc(sizeof(char)*3);
    gets(size);
    int V = atoi(size)+1;
	struct Graph* graph = createGraph(V);
	struct Graph* temp = createGraph(V);

    int *result = (int*) calloc(V,sizeof(int));
    for(int m=0;m<V;m++){
        result[m] = -1;
    }
    int *countOfHallway = (char*) calloc(V,sizeof(char));
    countOfHallway[0] = 0;
	char *adj = (char*) calloc(V,sizeof(char));
	int junctionID=1;
	while(junctionID<V){
        gets(adj);
        createHallway(graph,countOfHallway,junctionID,adj);
        junctionID++;
	}
    countOfHallway[8] = checkAllocation(graph,8);

    int num = allocateJunctions(temp,graph,countOfHallway,result);
    printf("As a possible solution, we need %d watchman to be allocated to junctions: ",num);

    for(int i=0;i<V;i++){

        if(result[i]!=-1){
            if(result[i+1]==-2){
               printf("%d",result[i]);
               break;
            } else {printf("%d, ",result[i]); }
        }
    }


	return 0;
}

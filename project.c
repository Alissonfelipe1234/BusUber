#include<stdio.h>
#include<stdlib.h>

typedef struct network{
	int nodes;
	int links;
	int **matrix;
} Network;

typedef struct route{
	int cost;
	int moves;
} Route;

typedef struct estate{
	int prev;
	int size;
	enum{fixed, finding} now;
} Estate;

Route* routes;
int totalRoutes;

int **initNetwork(int height, int width){
	int i;
	int **graph = calloc(height, sizeof (int*));
	for(i = 0; i < height; i++)
        graph[i] = calloc(width, sizeof (int*));
	return graph;
}

Network* createNetwork(int size){
	Network* net = calloc(0, sizeof(Network));
	net->nodes = size;
	net->matrix = initNetwork(size,size);
	return net;	
}

int insertLink(Network* net, int line, int column, int cost){
	if(net->matrix[line][column] == 0){
		net->matrix[line][column] = cost;
		net->links++;
        return 1;
	}
    return 0;
}

int redeRemove(Network* net, int line, int column){
	if(net->matrix[line][column] != 0){
		net->matrix[line][column] = 0;
		net->links--;
        return 1;
	}
    return 0;
}

void printNetwork(Network* net){
	int line, column;
    if(net->nodes == 0)
        printf("Empty network");
	for(line = 0; line < net->nodes; line++ ){
		printf("%2d -> ", line);
		for(column = 0; column < net->nodes; column++){
			if(net->matrix[line][column] > 0){
				printf("%2d", column);
			}
		}
		printf("\n");
	}
}

void initPath(int* path, int size){
	int iterator;
	for(iterator = 0; iterator < size; iterator++)
		path[iterator] = -1;
}

void printPath(int* path, int size){
	int iterator;
	for(iterator = 0; iterator < size; iterator++)
		if(path[iterator] == -1)
			break;
		else
			printf("%d ", path[iterator]);
}

int calcCost(Network* net, int* route, int level){
	int i;
	int actual, next, cost = 0;
	if(level == 0){
		return 0;
	}
	for (i = 0; i < level; i++)
    {
		actual = route[i];
		next = route[i + 1];
		cost += net->matrix[actual][next];
	}
	return cost;
}

void printRoute(Network* net, int* route, int level){
	int i;
	printf("%d: ", totalRoutes+1);
	for(i = 0; i < level; i++){
		printf("%d ", route[i]);		
	}
	routes[totalRoutes].cost = calcCost(net, route, level);
	routes[totalRoutes].moves = level-1;
	printf(" - Custo: %d", routes[totalRoutes].cost);
	printf(" - Saltos: %d\n", routes[totalRoutes].moves);
	totalRoutes++;
}

void dijkstra(Network* net, int startPoint, int destination, int* path){
	
	Estate* status = calloc(net->nodes, sizeof(Estate));
 	
	int interator, min, aux;
 	
	for (interator = 0; interator < net->nodes; interator++){
		status[interator].prev =-1;
		status[interator].size = 1000;
		status[interator].now = finding;
	}

	status[destination].size = 0;
	status[destination].now = fixed;
	aux = destination;
	do{
		for (interator = 0; interator < net->nodes ;interator++){
			if(net->matrix[aux][interator] != 0 && status[interator].now == finding){
				if(status[aux].size + net->matrix[aux][interator] < status[interator].size){
					status[interator].prev = aux;
					status[interator].size = status[aux].size + net->matrix[aux][interator];
 				}
 			}
		}
 		
		aux = 0;
 		min = 1000;
 		
 		for(interator = 0 ;interator < net->nodes; interator++){
 			if(status[interator].now == finding && status[interator].size < min){
 				min = status[interator].size;
 				aux = interator;
 			}
 		}
 		
 		status[aux].now = fixed;
 		
 	}while(aux != startPoint);
 	
 	interator = 0;
     	
 	do
    {
 	 	path[interator] = aux;
 		aux = status[aux].prev;
 	} while(aux >= 0);
}


int checkRoute(int* route, int node, int level){
	int i;
	for(i = 0; i < level; i++)
		if(route[i] == node)
			return 1;
	return 0;
}

void showRoutes(Network* net, int line, int column, int* route, int level){
	if(line == column){
		printRoute(net, route, level);
		return;
	}
    
    if(level == 0){
        route[0] = line;
        level++;
    }

    int i;
    for(i = 0; i < net->nodes; i++){
        if(net->matrix[line][i] > 0 && (checkRoute(route, i, level) != 1)){
            route[level] = i;
            showRoutes(net, line, column, route, level + 1);
        }
    }
	
}
void printLegend(){
    printf("Legend\n\t0-> New York\n\t1-> Paris\n\t2-> London\n\t3-> Paris\n\t4-> Dubai\n");
}
int main(){
	
	int startPoint, destination;
	
	int nodes = 5;
	int links = 6;
	
	Network* graph = createNetwork(nodes);
	insertLink(graph, 0, 1, 1);
	insertLink(graph, 0, 2, 2);
	insertLink(graph, 1, 0, 1);
	insertLink(graph, 1, 3, 2);
	insertLink(graph, 1, 4, 3);
	insertLink(graph, 2, 0, 2);
	insertLink(graph, 2, 3, 1);
	insertLink(graph, 3, 1, 2);
	insertLink(graph, 3, 2, 1);
	insertLink(graph, 3, 4, 1);
	insertLink(graph, 4, 1, 3);
	insertLink(graph, 4, 3, 1);
		
	printNetwork(graph);
	printLegend();

	printf("Enter a valid destination and origin\n");
	
	do{
		printf("Start:");
		scanf("%d", &startPoint);
	}while(startPoint >= graph->nodes || startPoint < 0);
	
	do{
		printf("Destination:");
		scanf("%d", &destination);
	}while(destination >= graph->nodes || destination < 0);
	
	routes = calloc(links * links, sizeof(Route));
	totalRoutes = 0;
	
	int* path = calloc(nodes, sizeof(int));
	initPath(path, nodes);
	printf("All routes:\n");
	showRoutes(graph, startPoint, destination, path, 0);
	
	
	int i, minimumCost = routes[0].cost, minimumMove = routes[0].moves, bestRouteCost = 0, bestRouteMoves = 0;
	for(i = 1; i < totalRoutes; i++){
		if(routes[i].cost < minimumCost){
			minimumCost = routes[i].cost;
			bestRouteCost = i;
		}
		if(routes[i].moves < minimumMove){
			minimumMove = routes[i].moves;
			bestRouteMoves = i;
		}
	}

	int* pathToDestination = calloc(nodes, sizeof(int));	
	
	dijkstra(graph, startPoint, destination, path);	
	printf("\n--------Best route according to Dijkstra--------\n");
	printPath(path, nodes);		
	
	return 0;
}
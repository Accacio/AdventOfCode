#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node
{
  char id[6];
  int numId;
  int big;
  int timesVisited;
  int edges[50];
  int totalEdges;
} Node;

typedef struct Graph
{
  Node *nodes;
  int totalNodes;
} Graph;

void
init_node (Node *node, char id[6], int numId)
{
  for (int i = 0; i < 6; i++)
    node->id[i] = id[i];
  node->big = isupper (id[0]);
  node->timesVisited = 0;
  node->totalEdges = 0;
  node->numId = numId;
}

void
init_graph (Graph *graph)
{
  graph->nodes = malloc (50 * sizeof (Node));
  graph->totalNodes = 0;
}

int
graph_find_node (Graph *graph, char id[6])
{
  for (int i = 0; i < graph->totalNodes; i++)
    {
      if (strcmp (id, (graph->nodes + i)->id) == 0)
        {
          return i;
        }
    }
  return -1;
}

void
graph_add_node (Graph *graph, char id[6])
{
  /* printf("Adding %s\n", id); */
  int found = graph_find_node (graph, id);
  if (found < 0)
    {
      Node *node;
      node = malloc (sizeof (Node));
      init_node (node, id, graph->totalNodes);
      *(graph->nodes + graph->totalNodes) = *node;
      graph->totalNodes++;
    }
}

void
graph_add_edge (Graph *graph, char first[6], char second[6])
{
  graph_add_node (graph, first);
  graph_add_node (graph, second);
  int firstId = graph_find_node (graph, first);
  int secondId = graph_find_node (graph, second);
  Node *firstNode = (graph->nodes + firstId);
  Node *secondNode = (graph->nodes + secondId);
  // considering they do not appear twice
  firstNode->edges[firstNode->totalEdges] = secondId;
  firstNode->totalEdges++;
  secondNode->edges[secondNode->totalEdges] = firstId;
  secondNode->totalEdges++;
}

void
print_graph (Graph graph)
{
  for (int i = 0; i < graph.totalNodes; i++)
    {
      Node *currNode = graph.nodes + i;
      printf ("%s -> { ", currNode->id);
      for (int j = 0; j < currNode->totalEdges; j++)
        {
          printf ("%s ", (graph.nodes + currNode->edges[j])->id);
        }
      printf ("}\n");
    }
}

long
graph_search_paths (Graph *graph, int initial, int final,
                    int visitedTwiceAlready)
{
  long ret = 0;
  Node *cur = graph->nodes + initial;
  cur->timesVisited++;

  if (cur->numId == final)
    {
      cur->timesVisited--;
      return 1;
    }

  for (int i = 0; i < cur->totalEdges; i++)
    {
      Node *next = graph->nodes + cur->edges[i];
      int visited = visitedTwiceAlready;
      if (next->numId == 0
          || ((!next->big && next->timesVisited > 0) && visitedTwiceAlready))
        {
          continue;
        }
      if (!next->big && next->timesVisited > 0 && !visitedTwiceAlready)
        {
          visited = 1;
        }
      ret += graph_search_paths (graph, next->numId, final, visited);
    }
  cur->timesVisited--;
  return ret;
}

long
graph_list_paths (Graph *graph)
{
  return graph_search_paths (graph, 0, 1, 0);
}

int
main (int argc, char *argv[])
{
  if (!argv[1])
    {
      printf ("no file given\n");
      exit (EXIT_FAILURE);
    }

  FILE *file = fopen (argv[1], "r");
  char *line;

  Graph graph;
  init_graph (&graph);
  graph_add_node (&graph, "start");
  graph_add_node (&graph, "end");

  while (fscanf (file, "%ms", &line) != EOF)
    {
      char *saveptr = line;
      char *firstNode = strsep (&saveptr, "-");
      char *secondNode = strsep (&saveptr, "-");
      graph_add_edge (&graph, firstNode, secondNode);
    }

  long paths = graph_list_paths (&graph);
  printf ("%ld\n", paths);

  return 0;
}

//
// Project 3:  C program that will find its way through a maze using the depth-first search
// algorithm
//
// Author: Allen Thomas, University of Illinois Chicago, Spring 2021
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

/* This program will read the first 3 lines of input
   and prints a static 2D maze*/
int debugMode;
typedef struct mazeStruct
{
 char** arr;  /* allows for a maze of size 30x30 plus outer walls */
 int xsize, ysize;
 int xstart, ystart;
 int xend, yend;
} maze;

typedef struct nodeStruct    // creates a stack for the linked list
{
    int xposition, yposition;
    struct nodeStruct* next;
} node;

/** function prototypes */
void init(node** hd);
int isEmpty(node* hd);
void push(node** hd, int x, int y);
void reset(node** hd);
void pop(node** hd);
int xtop(node* hd);
int ytop(node* hd);
void printCoordinates(node* hd);


int main (int argc, char **argv)
{
  maze m1;

  int xpos, ypos;
  int i,j;

  node* hd = NULL;   // initializes the head to NULL

  FILE *src;
    
  m1.arr = (char**) malloc(sizeof(char*) * m1.xsize);
  for(int a = 0; a < m1.xsize; a++)
  {
      m1.arr[a] = (char*) malloc(sizeof(char) * m1.ysize);
  }
    
    
  if(argc > 3) // checks if the input files are more than 3
  {
      printf("Too many input files.\n");
      exit(-1);
  }
    
  /** Debug mode Testing*/
  debugMode = FALSE;
  int k;
  for ( k = 0 ; k < argc ; k++ )
  {
      if ( strcmp(argv[k], "-d") == 0 )
      {
          debugMode = TRUE;
      }
  }
     

  /* verify the proper number of command line arguments were given */

  if(argc < 2 || argc >= 4)
  {
     printf("Usage: %s <input file name>\n", argv[0]);
     exit(-1);
  }
  
  /* Try to open the input file. */
  if(argc == 2)
  {
      if ( ( src = fopen( argv[1], "r" )) == NULL )
      {
          printf ( "Can't open input file: %s", argv[1] );
          exit(-1);
      }
  }
  else
  {
      if (strcmp (argv[1], "-d") == 0 )
      {
         if ((src = fopen( argv[2], "r")) == NULL)
         {
             printf ( "Can't open input file: %s\n", argv[2] );
             exit(-1);
         }
      }
      else
      {
        if ((src = fopen( argv[1], "r")) == NULL)
         {
             printf ( "Can't open input file: %s\n", argv[1] );
             exit(-1);
         }
      }
  }
    
      
    
  /* read in the size, starting and ending positions in the maze */
  if(fscanf (src, "%d %d", &m1.xsize, &m1.ysize) == EOF)
  {
      printf("Invalid data file.\n");
      exit(-1);
  }
  else if(fscanf (src, "%d %d", &m1.xstart, &m1.ystart) == EOF)
  {
      printf("Invalid data file.\n");
      exit(-1);
  }
  else if(fscanf (src, "%d %d", &m1.xend, &m1.yend) == EOF)
  {
      printf("Invalid data file.\n");
      exit(-1);
  }
    
    
  if ((m1.xsize <= 0) || (m1.ysize <= 0))
  {
      printf("Maze sizes must be greater than 0.\n");
      exit(-1);
  }
  else
      printf ("size: %d, %d\n", m1.xsize, m1.ysize);
    
    
  if ((m1.xstart > m1.xsize) || (m1.ystart > m1.ysize))
  {
      printf ("Start/End position outside of maze range.\n");
      exit(-1);
  }
  else
      printf ("start: %d, %d\n", m1.xstart, m1.ystart);
    
  
 if ((m1.xend > m1.xsize) || (m1.yend > m1.ysize))
 {
     printf ("Start/End position outside of maze range.\n");
     exit(-1);
 }
 else
     printf ("end: %d, %d\n", m1.xend, m1.yend);
      

  /* initialize the maze to empty */
  for (i = 0; i < m1.xsize+2; i++)
     for (j = 0; j < m1.ysize+2; j++)
       m1.arr[i][j] = '.';

  /* mark the borders of the maze with *'s */
  for (i=0; i < m1.xsize+2; i++)
    {
     m1.arr[i][0] = '*';
     m1.arr[i][m1.ysize+1] = '*';
    }
  for (i=0; i < m1.ysize+2; i++)
    {
     m1.arr[0][i] = '*';
     m1.arr[m1.xsize+1][i] = '*';
    }

  /* mark the starting and ending positions in the maze */
  m1.arr[m1.xstart][m1.ystart] = 's';
  m1.arr[m1.xend][m1.yend] = 'e';
          
  /*This code will only read the first three lines */
  /*Complete this code by reading the rest of the input
    and placing blocked and coin positions. */
    char charac;
    while (fscanf (src, "%d %d %c", &xpos, &ypos, &charac) != EOF)
    {
        if(xpos > m1.xsize || ypos > m1.ysize )  // checks if the maze sizes are out of range
        {
            // if yes then prints out error message
            printf("Invalid coordinates: outside of maze range.\n");
        }
        // checks if the start and end positions for x and y are not blocked
        else if((xpos == m1.xstart && ypos == m1.ystart) || (xpos == m1.xend && ypos == m1.yend))
        {
            // if yes then it prints out an error statement
            printf("Invalid coordinates: attempting to block start/endposition.\n");
        }
        // checks if the input file has a c then it adds coin to that position in the maze
        else if(charac == 'c')
        {
            m1.arr[xpos][ypos] = 'C';
        }
        //  checks if the input file has a b then it puts a * to block that position in the maze
        else if(charac == 'b')
        {
            m1.arr[xpos][ypos] = '*';
        }
        else   // if there is any other character then it gives an error statement
        {
            printf("Invalid type: type is not recognized.\n");
        }
             
    }
    /* print out the initial maze */
    for (i = 0; i < m1.xsize+2; i++)
      {
       for (j = 0; j < m1.ysize+2; j++)
         printf ("%c", m1.arr[i][j]);
       printf("\n");
      }
    
    int x = m1.xstart , y = m1.ystart;
    int count = 0;  // sets count to collect the coins
    
    init(&hd);  // initializes the stack
    push(&hd, x, y);  // pushes the starting position into the stack
    m1.arr[x][y] = 'v';  // it sets the positions pushed into the stack as visited
    
    // checks while stack is not empty and end has not been found
    while(!isEmpty(hd) && !(hd->xposition == m1.xend && hd->yposition == m1.yend))
    {
        
// checks if the coordinate at the Top of the Stack is the end position then it breaks the loop
        if(xtop(hd) == m1.xend && ytop(hd) == m1.yend)
            break;
        
        // Down move
 // checks if the Top of the Stack has an unvisited and unblocked neighbor
        if (m1.arr[x + 1][y] != '*' && m1.arr[x + 1][y] != 'v')
        {
            // then it pushes the coordinates of one unvisited neighbor on the stack
            push(&hd,x + 1, y);
            
            if(m1.arr[x + 1][y] == 'C') // checks if the coordinate contains a coin
                count++;   // then increases count
            
            m1.arr[x + 1][y] = 'v';  // marks that coordinate as visited
            
        }
        //Right move
        // checks if the Top of the Stack has an unvisited and unblocked neighbor
        else if(m1.arr[x][y + 1] != '*' && m1.arr[x][y + 1] != 'v')
        {
            // then it pushes the coordinates of one unvisited neighbor on the stack
            push(&hd,x, y + 1);
            
            if(m1.arr[x][y + 1] == 'C') // checks if the coordinate contains a coin
                count++; // then increases count
            
            m1.arr[x][y + 1] = 'v';  // marks that coordinate as visited
            
        }
        //Up move
        // checks if the Top of the Stack has an unvisited and unblocked neighbor
        else if(m1.arr[x - 1][y] != '*' && m1.arr[x - 1][y] != 'v')
        {
            // then it pushes the coordinates of one unvisited neighbor on the stack
            push(&hd,x - 1, y);
            
            if(m1.arr[x - 1][y] == 'C') // checks if the coordinate contains a coin
                count++;  // then increases count
            
            m1.arr[x - 1][y] = 'v';   // marks that coordinate as visited
            
        }
        //Left move
        // checks if the Top of the Stack has an unvisited and unblocked neighbor
        else if (m1.arr[x][y - 1] != '*' && m1.arr[x][y - 1] != 'v')
        {
            // then it pushes the coordinates of one unvisited neighbor on the stack
            push(&hd,x, y - 1);
            
            if(m1.arr[x][y - 1] == 'C') // checks if the coordinate contains a coin

                count++; // then increases count
            
            m1.arr[x][y - 1] = 'v'; // marks that coordinate as visited
            
        }
        else
            pop(&hd); // pop the coordinate at the Top of the Stack
        
        x = xtop(hd);
        y = ytop(hd);
    }
    
    
    if(isEmpty(hd)) // checks if the stack is empty
    {
        printf("\n");
        printf("This Maze has no solution.\n"); // then maze has no solution
    }
    //The items on the stack contain the coordinates of the solution from the end of the maze to the start of the maze.
    else
    {
        printf("\n");
        printf ("The maze has a solution.\n");
        printf ("The amount of coins collected:%d\n", count); // prints the number of coins
        printf ("The path from start to end:\n");
        printCoordinates(hd);  // prints the stack in reverse
        printf("\n");
    }
    
    
  /* Blocked positions are marked with 'b' in the input file
     They should be marked by * in the maze */

  /*Coin positions are marked by 'c' in the input file
    They should be marked by C in the maze*/
 

  /*Close the file*/
   fclose(src);
   reset(&hd);
   
   for(int b=0 ; b < m1.xsize ; b++)
   {
       free(m1.arr[b]);
   }
    
   free(m1.arr);

  
}

// initializes the stack
void init(node** hd)
{
    *hd = NULL;  // sets the head of the stack as null
}

//clears out the stack
void reset(node** hd)
{
    while(!isEmpty(*hd))
    {
        pop(hd);
    }
}

//checks if the stack is empty
int isEmpty(node* hd)
{
    if(hd == NULL)
        return TRUE;
    else
        return FALSE;
}

// function which pushes the coordinate to the stack
void push(node** hd, int x, int y)
{
    node* newNode = (node*)malloc(sizeof(node));
    newNode->xposition = x;
    newNode->yposition = y;
    newNode->next = *hd;
    *hd = newNode;
    
    if (debugMode == TRUE)
    {
        printf("(%d,%d) pushed into the stack.\n", newNode->xposition, newNode->yposition);
    }
    
}

// function to pop the top of the stack
void pop(node** hd)
{
    if(*hd == NULL )
        return;
    
    node* tmp;
    tmp = *hd;
    *hd = (*hd)->next;
    
    //checks if debug mode is applied
    if (debugMode == TRUE)
    {
        printf("(%d,%d) popped off the stack.\n", tmp->xposition, tmp->yposition);
    }
    
    free(tmp);
}

// top function for x coordinate
int xtop(node* hd)
{
    if(hd == NULL)
        return -1;
    
    return (hd->xposition);
}

// top function for y coordinate
int ytop(node* hd)
{
    if(hd == NULL)
        return -1;
    return (hd->yposition);
}

//prints the path of the coordinates in reverse
void printCoordinates(node* hd)
{
    if(hd == NULL)
        return;
    
    printCoordinates(hd->next);
    printf("(%d,%d) ", hd->xposition, hd->yposition);
    
}

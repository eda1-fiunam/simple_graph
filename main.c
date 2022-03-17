/*Copyright (C) 
 * 2022 - francisco dot rodriguez at ingenieria dot unam dot edu
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */


#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef PRINT_LEVEL
#define PRINT_LEVEL 1
#endif  

#ifndef DBG_HELP
#define DBG_HELP 0
#endif  

#if DBG_HELP > 0
#define DBG_PRINT( ... ) do{ fprintf( stderr, "DBG:" __VA_ARGS__ ); } while( 0 )
#else
#define DBG_PRINT( ... ) ;
#endif  


typedef struct Node
{
   int neighbor;
   struct Node* next;
} Node;

typedef struct
{
   int data;
   Node* neighbors;
} Vertex;

typedef enum
{
   eGraphType_UNDIRECTED,
   eGraphType_DIRECTED,
} eGraphType;

typedef struct
{
   Vertex* list;
   size_t size;
   size_t len;

   eGraphType type;
} Graph;

Graph* Graph_New( size_t size, eGraphType type )
{
   Graph* g = (Graph*) malloc( sizeof( Graph ) );
   if( g )
   {
      g->type = type;
      g->len = 0;
      g->size = size;
      g->list = (Vertex*) calloc( size, sizeof( Vertex ) );
      if( !g->list )
      {
         free( g );
         g = NULL;
      }
   }
   return g;
}

void Graph_Delete( Graph** g )
{
   assert( g );

   Graph* graph = *g;
   // para simplificar la notación

   for( size_t i = 0; i < graph->size; ++i )
   {
      for( Node* it = graph->list[ i ].neighbors; it;  )
      {
         Node* next = it->next;
         free( it );
         it = next;
      }
   }

   free( graph->list );

   free( graph );

   *g = NULL;
}

void Graph_AddVertex( Graph* g, int data )
{
   assert( g );
   assert( g->len < g->size );

   Vertex* v = &g->list[ g->len ];
   // para simplificar la notación

   v->data = data;
   v->neighbors = NULL;

   ++g->len;
}

size_t find( Vertex* list, size_t size, int key )
{
   size_t i;
   for( i = 0; i < size; ++i )
   {
      if( list[i].data == key )
      {
         break;
      }
   }
   return i;
}

void insert( Vertex* v, int data )
{
   Node* n = (Node*) malloc( sizeof( Node ) );
   n->neighbor = data;
   n->next = v->neighbors;
   v->neighbors = n;
}

bool Graph_AddEdge( Graph* g, int start, int end )
{
   assert( g->len > 0 );

   size_t start_idx = find( g->list, g->size, start );
   size_t end_idx = find( g->list, g->size, end );

   if( start_idx == g->size || end_idx == g->size ) return false;

   insert( &g->list[ start_idx ], end );

   if( g->type == eGraphType_UNDIRECTED ) insert( &g->list[ end_idx ], start );

   return true;
}

void Graph_Print( Graph* g, int depth )
{
   for( size_t i = 0; i < g->len; ++i )
   {
      Vertex* vertex = &g->list[ i ];
      // para simplificar la notación. 

      printf( "\nVertex[%ld].data=%d\n", i, vertex->data );

      // LEVEL 0:
      printf( vertex->neighbors ? " Has neighbors:\n " : " Has no neighbors\n" );

      // LEVEL 1:
      if( depth > 0 )
      {
         for( Node* node = vertex->neighbors; node != NULL; node = node->next )
         {
            DBG_PRINT( "Print():(Node:%p, (*Node.neighbor:%d, *Node.next:%p))\n", 
                  node, node->neighbor, node->next );
            
            printf( " %d ",  node->neighbor );

            // LEVEL 2:
            if( depth > 1 )
            {
               printf( "(Node:%p) ", node );
            }

            printf( "->" );
         } if( vertex->neighbors ) printf( " Nil\n" );
      }
   } printf( "\n" );
}




typedef struct
{
   int start;
   int end;
} Adyacency;

int main()
{
   int data_list[ 6 ] = { 100, 200, 300, 400, 500, 600 };

   Adyacency ady_list[ 7 ] =
   {
      {100,200}, {100,300}, {100,400}, {200,500}, {200,600}, {300,400}, {400,500},
   };

   Graph* grafo = Graph_New( 6, eGraphType_UNDIRECTED );
   assert( grafo );

   for( size_t i = 0; i < 6; ++i ) Graph_AddVertex( grafo, data_list[ i ] );

   for( size_t i = 0; i < 7; ++i ) Graph_AddEdge( grafo, ady_list[ i ].start, ady_list[ i ].end );

   Graph_Print( grafo, 1 );

   Graph_Delete( &grafo );
   assert( grafo == NULL );
}

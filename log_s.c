/*
	 * Medium Difficulty - Dijkstra's Algorithm
	class ShortestPath
	{
	    // A utility function to find the vertex with minimum distance value,
	    // from the set of vertices not yet included in shortest path tree
	    static final int V=9;
	    int minDistance(int dist[], Boolean sptSet[])
	    {
	        // Initialize min value
	        int min = Integer.MAX_VALUE, min_index=-1;
	 
	        for (int v = 0; v < V; v++)
	            if (sptSet[v] == false && dist[v] <= min)
	            {
	                min = dist[v];
	                min_index = v;
	            }
	 
	        return min_index;
	    }
	 
	    // A utility function to print the constructed distance array
	    void printSolution(int dist[], int n)
	    {
	        System.out.println("Vertex   Distance from Source");
	        for (int i = 0; i < V; i++)
	            System.out.println(i+" \t\t "+dist[i]);
	    }
	 
	    // Funtion that implements Dijkstra's single source shortest path
	    // algorithm for a graph represented using adjacency matrix
	    // representation
	    void dijkstra(int graph[][], int src)
	    {
	        int dist[] = new int[V]; // The output array. dist[i] will hold
	                                 // the shortest distance from src to i
	 
	        // sptSet[i] will true if vertex i is included in shortest
	        // path tree or shortest distance from src to i is finalized
	        Boolean sptSet[] = new Boolean[V];
	 
	        // Initialize all distances as INFINITE and stpSet[] as false
	        for (int i = 0; i < V; i++)
	        {
	            dist[i] = Integer.MAX_VALUE;
	            sptSet[i] = false;
	        }
	 
	        // Distance of source vertex from itself is always 0
	        dist[src] = 0;
	 
	        // Find shortest path for all vertices
	        for (int count = 0; count < V-1; count++)
	        {
	            // Pick the minimum distance vertex from the set of vertices
	            // not yet processed. u is always equal to src in first
	            // iteration.
	            int u = minDistance(dist, sptSet);
	 
	            // Mark the picked vertex as processed
	            sptSet[u] = true;
	 
	            // Update dist value of the adjacent vertices of the
	            // picked vertex.
	            for (int v = 0; v < V; v++)
	 
	                // Update dist[v] only if is not in sptSet, there is an
	                // edge from u to v, and total weight of path from src to
	                // v through u is smaller than current value of dist[v]
	                if (!sptSet[v] && graph[u][v]!=0 &&
	                        dist[u] != Integer.MAX_VALUE &&
	                        dist[u]+graph[u][v] < dist[v])
	                    dist[v] = dist[u] + graph[u][v];
	        }
	 
	        // print the constructed distance array
	        printSolution(dist, V);
	    }
	 
	 /******************************************************************************
 *  Compilation:  javac DijkstraSP.java
 *  Execution:    java DijkstraSP input.txt s
 *  Dependencies: EdgeWeightedDigraph.java IndexMinPQ.java Stack.java DirectedEdge.java
 *  Data files:   http://algs4.cs.princeton.edu/44sp/tinyEWD.txt
 *                http://algs4.cs.princeton.edu/44sp/mediumEWD.txt
 *                http://algs4.cs.princeton.edu/44sp/largeEWD.txt
 *
 *  Dijkstra's algorithm. Computes the shortest path tree.
 *  Assumes all weights are nonnegative.
 *
 *  % java DijkstraSP tinyEWD.txt 0
 *  0 to 0 (0.00)  
 *  0 to 1 (1.05)  0->4  0.38   4->5  0.35   5->1  0.32   
 *  0 to 2 (0.26)  0->2  0.26   
 *  0 to 3 (0.99)  0->2  0.26   2->7  0.34   7->3  0.39   
 *  0 to 4 (0.38)  0->4  0.38   
 *  0 to 5 (0.73)  0->4  0.38   4->5  0.35   
 *  0 to 6 (1.51)  0->2  0.26   2->7  0.34   7->3  0.39   3->6  0.52   
 *  0 to 7 (0.60)  0->2  0.26   2->7  0.34   
 *
 *  % java DijkstraSP mediumEWD.txt 0
 *  0 to 0 (0.00)  
 *  0 to 1 (0.71)  0->44  0.06   44->93  0.07   ...  107->1  0.07   
 *  0 to 2 (0.65)  0->44  0.06   44->231  0.10  ...  42->2  0.11   
 *  0 to 3 (0.46)  0->97  0.08   97->248  0.09  ...  45->3  0.12   
 *  0 to 4 (0.42)  0->44  0.06   44->93  0.07   ...  77->4  0.11   
 *  ...
 *
 ******************************************************************************/

/*
 *  The {@code DijkstraSP} class represents a data type for solving the
 *  single-source shortest paths problem in edge-weighted digraphs
 *  where the edge weights are nonnegative.
 *  <p>
 *  This implementation uses Dijkstra's algorithm with a binary heap.
 *  The constructor takes time proportional to <em>E</em> log <em>V</em>,
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the {@code distTo()} and {@code hasPathTo()} methods take
 *  constant time and the {@code pathTo()} method takes time proportional to the
 *  number of edges in the shortest path returned.
 *  <p>
 *  For additional documentation,    
 *  see <a href="http://algs4.cs.princeton.edu/44sp">Section 4.4</a> of    
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne. 
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 *
public class DijkstraSP {
    private double[] distTo;          // distTo[v] = distance  of shortest s->v path
    private DirectedEdge[] edgeTo;    // edgeTo[v] = last edge on shortest s->v path
    private IndexMinPQ<Double> pq;    // priority queue of vertices

    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every other
     * vertex in the edge-weighted digraph {@code G}.
     *
     * @param  G the edge-weighted digraph
     * @param  s the source vertex
     * @throws IllegalArgumentException if an edge weight is negative
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     *
    public DijkstraSP(EdgeWeightedDigraph G, int s) {
        for (DirectedEdge e : G.edges()) {
            if (e.weight() < 0)
                throw new IllegalArgumentException("edge " + e + " has negative weight");
        }

        distTo = new double[G.V()];
        edgeTo = new DirectedEdge[G.V()];

        validateVertex(s);

        for (int v = 0; v < G.V(); v++)
            distTo[v] = Double.POSITIVE_INFINITY;
        distTo[s] = 0.0;

        // relax vertices in order of distance from s
        pq = new IndexMinPQ<Double>(G.V());
        pq.insert(s, distTo[s]);
        while (!pq.isEmpty()) {
            int v = pq.delMin();
            for (DirectedEdge e : G.adj(v))
                relax(e);
        }

        // check optimality conditions
        assert check(G, s);
    }

    // relax edge e and update pq if changed
    private void relax(DirectedEdge e) {
        int v = e.from(), w = e.to();
        if (distTo[w] > distTo[v] + e.weight()) {
            distTo[w] = distTo[v] + e.weight();
            edgeTo[w] = e;
            if (pq.contains(w)) pq.decreaseKey(w, distTo[w]);
            else                pq.insert(w, distTo[w]);
        }
    }

    /**
     * Returns the length of a shortest path from the source vertex {@code s} to vertex {@code v}.
     * @param  v the destination vertex
     * @return the length of a shortest path from the source vertex {@code s} to vertex {@code v};
     *         {@code Double.POSITIVE_INFINITY} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     *
    public double distTo(int v) {
        validateVertex(v);
        return distTo[v];
    }

    /**
     * Returns true if there is a path from the source vertex {@code s} to vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return {@code true} if there is a path from the source vertex
     *         {@code s} to vertex {@code v}; {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     *
    public boolean hasPathTo(int v) {
        validateVertex(v);
        return distTo[v] < Double.POSITIVE_INFINITY;
    }

    /**
     * Returns a shortest path from the source vertex {@code s} to vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return a shortest path from the source vertex {@code s} to vertex {@code v}
     *         as an iterable of edges, and {@code null} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     *
    public Iterable<DirectedEdge> pathTo(int v) {
        validateVertex(v);
        if (!hasPathTo(v)) return null;
        Stack<DirectedEdge> path = new Stack<DirectedEdge>();
        for (DirectedEdge e = edgeTo[v]; e != null; e = edgeTo[e.from()]) {
            path.push(e);
        }
        return path;
    }


    // check optimality conditions:
    // (i) for all edges e:            distTo[e.to()] <= distTo[e.from()] + e.weight()
    // (ii) for all edge e on the SPT: distTo[e.to()] == distTo[e.from()] + e.weight()
    private boolean check(EdgeWeightedDigraph G, int s) {

        // check that edge weights are nonnegative
        for (DirectedEdge e : G.edges()) {
            if (e.weight() < 0) {
                System.err.println("negative edge weight detected");
                return false;
            }
        }

        // check that distTo[v] and edgeTo[v] are consistent
        if (distTo[s] != 0.0 || edgeTo[s] != null) {
            System.err.println("distTo[s] and edgeTo[s] inconsistent");
            return false;
        }
        for (int v = 0; v < G.V(); v++) {
            if (v == s) continue;
            if (edgeTo[v] == null && distTo[v] != Double.POSITIVE_INFINITY) {
                System.err.println("distTo[] and edgeTo[] inconsistent");
                return false;
            }
        }

        // check that all edges e = v->w satisfy distTo[w] <= distTo[v] + e.weight()
        for (int v = 0; v < G.V(); v++) {
            for (DirectedEdge e : G.adj(v)) {
                int w = e.to();
                if (distTo[v] + e.weight() < distTo[w]) {
                    System.err.println("edge " + e + " not relaxed");
                    return false;
                }
            }
        }

        // check that all edges e = v->w on SPT satisfy distTo[w] == distTo[v] + e.weight()
        for (int w = 0; w < G.V(); w++) {
            if (edgeTo[w] == null) continue;
            DirectedEdge e = edgeTo[w];
            int v = e.from();
            if (w != e.to()) return false;
            if (distTo[v] + e.weight() != distTo[w]) {
                System.err.println("edge " + e + " on shortest path not tight");
                return false;
            }
        }
        return true;
    }

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        int V = distTo.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }

    /**
     * Unit tests the {@code DijkstraSP} data type.
     *
     * @param args the command-line arguments
     *
    public static void main(String[] args) {
        In in = new In(args[0]);
        EdgeWeightedDigraph G = new EdgeWeightedDigraph(in);
        int s = Integer.parseInt(args[1]);

        // compute shortest paths
        DijkstraSP sp = new DijkstraSP(G, s);


        // print shortest path
        for (int t = 0; t < G.V(); t++) {
            if (sp.hasPathTo(t)) {
                StdOut.printf("%d to %d (%.2f)  ", s, t, sp.distTo(t));
                for (DirectedEdge e : sp.pathTo(t)) {
                    StdOut.print(e + "   ");
                }
                StdOut.println();
            }
            else {
                StdOut.printf("%d to %d         no path\n", s, t);
            }
        }
    }

}

	    // Driver method
	    public static void main (String[] args)
	    {
	     	Let us create the example graph discussed above 
	       int graph[][] = new int[][]{{0, 4, 0, 0, 0, 0, 0, 8, 0},
	                                  {4, 0, 8, 0, 0, 0, 0, 11, 0},
	                                  {0, 8, 0, 7, 0, 4, 0, 0, 2},
	                                  {0, 0, 7, 0, 9, 14, 0, 0, 0},
	                                  {0, 0, 0, 9, 0, 10, 0, 0, 0},
	                                  {0, 0, 4, 14, 10, 0, 2, 0, 0},
	                                  {0, 0, 0, 0, 0, 2, 0, 1, 6},
	                                  {8, 11, 0, 0, 0, 0, 1, 0, 7},
	                                  {0, 0, 2, 0, 0, 0, 6, 7, 0}
	                                 };
	        ShortestPath t = new ShortestPath();
	        t.dijkstra(graph, 0);
	    }
	}
	http://algs4.cs.princeton.edu/44sp/DijkstraSP.java.html
	*/
	
	
	/*
	 * http://cs.fit.edu/~ryan/java/programs/graph/Dijkstra-java.html
	 public class Dijkstra {
		  2 
		  3    // Dijkstra's algorithm to find shortest path from s to all other nodes
		  4    public static int [] dijkstra (WeightedGraph G, int s) {
		  5       final int [] dist = new int [G.size()];  // shortest known distance from "s"
		  6       final int [] pred = new int [G.size()];  // preceeding node in path
		  7       final boolean [] visited = new boolean [G.size()]; // all false initially
		  8 
		  9       for (int i=0; i<dist.length; i++) {
		 10          dist[i] = Integer.MAX_VALUE;
		 11       }
		 12       dist[s] = 0;
		 13 
		 14       for (int i=0; i<dist.length; i++) {
		 15          final int next = minVertex (dist, visited);
		 16          visited[next] = true;
		 17 
		 18          // The shortest path to next is dist[next] and via pred[next].
		 19 
		 20          final int [] n = G.neighbors (next);
		 21          for (int j=0; j<n.length; j++) {
		 22             final int v = n[j];
		 23             final int d = dist[next] + G.getWeight(next,v);
		 24             if (dist[v] > d) {
		 25                dist[v] = d;
		 26                pred[v] = next;
		 27             }
		 28          }
		 29       }
		 30       return pred;  // (ignore pred[s]==0!)
		 31    }
		 32 
		 33    private static int minVertex (int [] dist, boolean [] v) {
		 34       int x = Integer.MAX_VALUE;
		 35       int y = -1;   // graph not connected, or no unvisited vertices
		 36       for (int i=0; i<dist.length; i++) {
		 37          if (!v[i] && dist[i]<x) {y=i; x=dist[i];}
		 38       }
		 39       return y;
		 40    }
		 41 
		 42    public static void printPath (WeightedGraph G, int [] pred, int s, int e) {
		 43       final java.util.ArrayList path = new java.util.ArrayList();
		 44       int x = e;
		 45       while (x!=s) {
		 46          path.add (0, G.getLabel(x));
		 47          x = pred[x];
		 48       }
		 49       path.add (0, G.getLabel(s));
		 50       System.out.println (path);
		 51    }
		 52 
		 53 }
	 */
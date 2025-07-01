//Athanasios Papanastasiou 3057

import java.util.ArrayList;
import java.util.Arrays;
import java.util.PriorityQueue;
import java.util.HashSet;

public abstract class Search {
	
	class Node implements Comparable<Object>{
		ArrayList<String> array;
		int cost;
		Node parent;
		
		public Node(String[] str) {
			array = new ArrayList<String>(Arrays.asList(str));
			parent = null;
			cost = 0;
		}
		public Node(Node parent) {
			array = (ArrayList<String>) parent.array.clone();
			this.parent = parent;
		}
		public boolean isFinal() {
			return !array.subList(0, N).contains("A") && array.indexOf("-") == 2*N-1;
		}
		public void swapDashWith(int k) {
			int dashIndex = array.indexOf("-");
			int newDashIndex = dashIndex + k;
			if(newDashIndex >= 0 && newDashIndex < array.size()) {
				array.set(dashIndex, array.get(newDashIndex));
				array.set(newDashIndex, "-");
			}
		}
		public String toString() {
			return String.join("", array);
		}
		public int hashCode() {
			return toString().hashCode();
		}
		public boolean equals(Object arg0) {
			return array.toString().equals(((Node)arg0).array.toString());
		}
		public int compareTo(Object arg0) {
			Node other = (Node) arg0;
			return this.cost-other.cost;
		}
	}
	public PriorityQueue<Node> frontier;
	public HashSet<Node> closedSet;
	static int N;
	
	public Search() {
		frontier = new PriorityQueue<Node>();
		closedSet = new HashSet<Node>();
	}
	
	public abstract void appendChildrensToFrontier(Node parent);
	
	public void searchFrontier() {
		while(!frontier.isEmpty()) {
			Node thisNode = frontier.poll();
			if(!closedSet.add(thisNode)) {
				continue;
			} else if(thisNode.isFinal()) {
				reportFinal(thisNode);
				return;
			}
			appendChildrensToFrontier(thisNode);
		}
	}
	public void reportFinal(Node finalNode) {
		while(finalNode != null) {
			System.out.println(finalNode);
			finalNode = finalNode.parent;
		}
	}

	public static void printStatistics(Search search, String[] array) {
		
		Node root = search.new Node(array);
		search.frontier.add(root);
		
		long start = System.currentTimeMillis();
		search.searchFrontier();	
		long finish = System.currentTimeMillis();
		
		long timeElapsed = finish-start;
		System.out.println(search+"\nTime elapsed:"+timeElapsed);
	}
	
	public static void main(String[] args) {

		String [] array = args.length == 0 ? 
			new String[] {"A","A","M","Μ","M","A","A","A","M","Μ", "A",
							"M","A","A","A","M","Μ","M","A","A","A",
							"M","Μ","M","A","A","A","M","Μ","M","A",
							"A","A","-","M","M","Μ","M","A","A","A",
							"M","M","A","A","M","Μ","M","A","A","A",
							"M","Μ","M","A","A","A","M","Μ","M","A",
						} : args[0].split("");

		N = (array.length-1)/2;

		Search searchMethod = (N < 7) ? new UniformCostSearch():new AstarSearch();

		printStatistics(searchMethod, array);
	}
}
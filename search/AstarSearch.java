//Athanasios Papanastasiou 3057

public class AstarSearch extends Search {

	public int heuristic(Node n) {
		String[] subArr = n.array.subList(0, N).toArray(new String[] {});
		int heuristicValue = 0;
		for(int i=0;i < N;i++) {
			String item = subArr[i];
			if(item.equals("A")) {
				heuristicValue += N/(i+1);
			}
		}
		return heuristicValue;
	}
	
	public void appendChildrensToFrontier(Node parent) {
		for(int k = -N;k <= N; k++) {
			Node children = new Node(parent);
			children.swapDashWith(k);
			children.cost += heuristic(children);
			frontier.add(children);
		}
	}

	public String toString() { 
		return "A* Search";
	}
}

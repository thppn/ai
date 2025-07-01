//Athanasios Papanastasiou 3057

public class UniformCostSearch extends Search{
	
	public void appendChildrensToFrontier(Node parent) {
		for(int k = -N;k <= N; k++) {
			Node children = new Node(parent);
			children.swapDashWith(k);
			children.cost = parent.cost+1;
			frontier.add(children);
		}
	}

	public String toString() { 
		return "Uniform Cost Search";
	}
}

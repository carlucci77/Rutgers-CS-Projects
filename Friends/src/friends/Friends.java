package friends;
import structures.Queue;
import structures.Stack;
import java.util.*;
public class Friends {
	private static ArrayList<String> BFS(Graph g, Person current, Person goal, Person friend, Person temp){
		ArrayList<String> shortest = new ArrayList<String>();
		Person temporary[] = new Person[g.members.length];
		HashMap<Person, Person> peeps = new HashMap<Person, Person>();
		HashMap<Person, Boolean> visit = new HashMap<Person, Boolean>();
		Queue<Person> people = new Queue<Person>();
		Person p = current;
		boolean found = false;
		peeps.put(current, current);
		people.enqueue(current);
		for(int k = 0; k < g.members.length; k++) {
			visit.put(g.members[k], false);
		}
		visit.put(current, true);
		while(!people.isEmpty()) {
			current = people.dequeue();
			Friend f = current.first;
			friend = g.members[f.fnum];
			while(f != null) {
				friend = g.members[f.fnum];
				if(!visit.get(friend)) {
					found = true;
					visit.put(friend, true);
					peeps.put(friend, current);
					people.enqueue(friend);
					if(friend.equals(goal)) {
						temporary[0] = friend;
						temporary[1] = current;
						temp = peeps.get(current);
						if(current.equals(p)) {
							shortest.add(temporary[0].name);
							shortest.add(0,temporary[1].name);
							return shortest;
						}else {
							break;
						}
					}
				}
				f = f.next;
			}
		}
		if(found) {
			int nullp = 0;
			int l = 2;
			while(!peeps.isEmpty()) {
				boolean check = false;
				for(int z = 0; z < temporary.length; z++) {
					if(temporary[z] == null) {
						break;
					}
					if(temporary[z].equals(temp)) {
						check = true;
					}
				}
				if(!check) {
					temporary[l] = temp;
				}
				temp = peeps.get(temp);
				if(temp.equals(peeps.get(p))) {
					nullp = l;
					for(int z = 0; z < temporary.length; z++) {
						if(temporary[z] == null) {
							break;
						}
						if(temporary[z].equals(temp)) {
							check = true;
						}
					}
					if(!check) {
						l++;
						temporary[l] = temp;
						nullp = l;
					}
					break;
				}
				l++;
			}
			for(int i = 0; i <= nullp; i++) {
				shortest.add(0, temporary[i].name);
			}
			return shortest;
		}
		return null;
	}
	/**
	 * Finds the shortest chain of people from p1 to p2.
	 * Chain is returned as a sequence of names starting with p1,
	 * and ending with p2. Each pair (n1,n2) of consecutive names in
	 * the returned chain is an edge in the graph.
	 * 
	 * @param g Graph for which shortest chain is to be found.
	 * @param p1 Person with whom the chain originates
	 * @param p2 Person at whom the chain terminates
	 * @return The shortest chain from p1 to p2. Null if there is no
	 *         path from p1 to p2
	 */
	public static ArrayList<String> shortestChain(Graph g, String p1, String p2) {
		ArrayList<String> shortest = new ArrayList<String>();
		int pe = g.map.get(p1);
		int pep = g.map.get(p2);
		Person current = g.members[pe];
		Person goal = g.members[pep];
		Person friend = new Person();
		Person temp = new Person();
		if(current.equals(goal)) {
			shortest.add(current.name);
			return shortest;
		}
		return BFS(g, current, goal, friend, temp);
	}
	private static ArrayList<String> cliqueSearch(Graph g, ArrayList<String> clique, Person p, String school){
		ArrayList<String> cliqueTemp = new ArrayList<String>();
		Friend f = p.first;
		if(f == null) {
			return clique;
		}
		Person friend = g.members[f.fnum];
		if(friend.student) {
			if(friend.school.equals(school)) {
				if(!clique.contains(friend.name)) {
					clique.add(friend.name);
					cliqueTemp = cliqueSearch(g, clique, friend, school);
					for(int l = 0; l < cliqueTemp.size(); l++) {
						if(!clique.contains(cliqueTemp.get(l))){
							clique.add(cliqueTemp.get(l));
						}
					}
				}
			}
		}
		f = f.next;
		while(f != null) {
			friend = g.members[f.fnum];
			if(friend.student) {
				if(friend.school.equals(school)) {
					if(!clique.contains(friend.name)) {
						clique.add(friend.name);
						cliqueTemp = cliqueSearch(g, clique, friend, school);
						for(int l = 0; l < cliqueTemp.size(); l++) {
							if(!clique.contains(cliqueTemp.get(l))){
								clique.add(cliqueTemp.get(l));
							}
						}
					}
				}
			}
			f = f.next;
		}
		return clique;
	}
	/**
	 * Finds all cliques of students in a given school.
	 * 
	 * Returns an array list of array lists - each constituent array list contains
	 * the names of all students in a clique.
	 * 
	 * @param g Graph for which cliques are to be found.
	 * @param school Name of school
	 * @return Array list of clique array lists. Null if there is no student in the
	 *         given school
	 */
	public static ArrayList<ArrayList<String>> cliques(Graph g, String school) {
		ArrayList<ArrayList<String>> result = new ArrayList<ArrayList<String>>();
		boolean inList = false;
		for(int k = 0; k < g.members.length; k++) {
			if(g.members[k].student) {
				if(g.members[k].school.equals(school)) {
					for(int l = 0; l < result.size(); l++) {
						if(result.get(l).contains(g.members[k].name)) {
							inList = true;
							break;
						}
					}
					if(!inList) {
						ArrayList<String> clique = new ArrayList<String>();
						clique.add(g.members[k].name);
						Person p = g.members[k];
						cliqueSearch(g, clique, p, school);
						result.add(clique);
					}
					inList = false;
				}else {
					continue;
				}
			}else {
				continue;
			}
		}
		if(result.isEmpty()) {
			return null;
		}
		return result;	
	}
	private static ArrayList<String> DFS(Graph g, HashMap<Person, Boolean> visited, Person current, 
	HashMap<Person, Integer> dfsNum, HashMap<Person, Integer> backNum, Person starter, int count, boolean check[]){
		count++;
		dfsNum.put(current, count);
		backNum.put(current, count);
		ArrayList<String> result = new ArrayList<String>();
		ArrayList<String> temp = new ArrayList<String>();
		Friend friendtmp = current.first;
		visited.put(current, true);
		while(friendtmp != null) {
			Person friend = g.members[friendtmp.fnum];
			if(!visited.get(friend)) {
				temp.addAll(DFS(g, visited, friend, dfsNum, backNum, starter, count, check));
				for(int l = 0; l < temp.size(); l++) {
					if(!result.contains(temp.get(l))) {
						result.add(temp.get(l));
					}
				}
				if(dfsNum.get(current) > backNum.get(friend)) {
					backNum.put(current, Math.min(backNum.get(current), backNum.get(friend)));
				}else {
					if(!current.equals(starter)) {
						result.add(current.name);
					}else {
						if(check[g.map.get(current.name)]) {
							result.add(current.name);
						}
					}
				}
				check[g.map.get(current.name)] = true;
			}else {
				backNum.put(current, Math.min(backNum.get(current), dfsNum.get(friend)));
			}
			friendtmp = friendtmp.next;
			if(friendtmp != null) {
				count++;
			}
		}
		return result;
	}
	/**
	 * Finds and returns all connectors in the graph.
	 * 
	 * @param g Graph for which connectors needs to be found.
	 * @return Names of all connectors. Null if there are no connectors.
	 */
	public static ArrayList<String> connectors(Graph g) {
		ArrayList<String> result = new ArrayList<String>();
		HashMap<Person, Boolean> visited = new HashMap<Person, Boolean>();
		HashMap<Person, Integer> dfsNum = new HashMap<Person, Integer>();
		HashMap<Person, Integer> backNum = new HashMap<Person, Integer>();
		ArrayList<String> temp = new ArrayList<String>();
		boolean check[] = new boolean[g.members.length];
		Person current = g.members[0];
		Person starter = current;
		int count = 0;
		for(int k = 0; k < g.members.length; k++) {
			visited.put(g.members[k], false);
		}
		for(int k = 0; k < g.members.length; k++) {
			if(!visited.get(current)) {
				starter = current;
				temp.addAll(DFS(g, visited, current, dfsNum, backNum, starter, count, check));
				for(int b = 0; b < temp.size(); b++) {
					if(!result.contains(temp.get(b))) {
						result.add(temp.get(b));
					}
				}
			}
			current = g.members[k];
		}
		if(result.isEmpty()) {
			return null;
		}else {
			return result;
		}
	}
}
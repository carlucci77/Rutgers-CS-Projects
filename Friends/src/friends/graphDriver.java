package friends;
import java.util.*;
import java.io.*;
public class graphDriver {
static Scanner sc = new Scanner(System.in);
	
	static String getOption() {
		System.out.print("getKeyWord(): ");
		String response = sc.next();
		return response;
	}
	public static void main(String[] args) throws FileNotFoundException{
		Scanner sci = new Scanner(new File("FriendTest1"));
		Graph g = new Graph(sci);
		System.out.println(Friends.shortestChain(g, "samir", "nick"));
		//System.out.println(Friends.cliques(g, "rutgers"));
		//System.out.println(Friends.connectors(g));
	}
}
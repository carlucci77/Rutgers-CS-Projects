package lse;

import java.io.*;
import java.util.*;

public class EngineTest 
{
	static Scanner sc = new Scanner(System.in);
	
	static String getOption() {
		System.out.print("getKeyWord(): ");
		String response = sc.next();
		return response;
	}
	
	public static void main(String args[]){
		String input = "";
		String keyword = "";
		String document = "";
		ArrayList<Occurrence> insertList = new ArrayList<Occurrence>();
		Occurrence occ = new Occurrence("test1", 12);
		insertList.add(occ);
		Occurrence occ1 = new Occurrence ("test2", 8);
		insertList.add(occ1);
		Occurrence occ2 = new Occurrence("test3", 7);
		insertList.add(occ2);
		Occurrence occ3 = new Occurrence("test4", 5);
		insertList.add(occ3);
		Occurrence occ4 = new Occurrence("test5", 3);
		insertList.add(occ4);
		Occurrence occ5 = new Occurrence("test6", 2);
		insertList.add(occ5);
		Occurrence occ6 = new Occurrence("test7", 6);
		insertList.add(occ6);
		LittleSearchEngine lse = new LittleSearchEngine();
		try{
			lse.makeIndex("docs.txt", "noisewords.txt");
		} 
		catch (FileNotFoundException e){
			System.out.println("failed bitch, try again");
		}	
		System.out.println("Enter method number: getKeyword (1), loadKeywordsFromDocument (2), insertLastOccurrence (3), "
				+ "mergeKeywords (4), top5search (5): ");
		Scanner scan1 = new Scanner(System.in);
		input = scan1.nextLine();
		if(Integer.valueOf(input) == 1) {
			System.out.println("Please enter word: ");
			Scanner scan = new Scanner(System.in);
			keyword = scan.nextLine();
			keyword = lse.getKeyword(keyword);
			if(keyword == null) {
				System.out.println("Not a keyword");
			}else {
				System.out.println("Keyword: " + keyword);
			}
			scan.close();
		}else if(Integer.valueOf(input) == 2) {
			System.out.println("Please enter file name: ");
			Scanner scan = new Scanner(System.in);
			document = scan.nextLine();
			try {
				HashMap<String, Occurrence> test = lse.loadKeywordsFromDocument(document);
				System.out.println(test.keySet());
				String tester = "zealand";
				System.out.println(test.get(tester).frequency);
			}catch(FileNotFoundException e) {
			}	
			scan.close();
		}else if(Integer.valueOf(input) == 3) {
			ArrayList<Integer> test = lse.insertLastOccurrence(insertList);
			System.out.println(insertList);
			System.out.println(test);
		}else if(Integer.valueOf(input) == 4) {
			System.out.println(lse.keywordsIndex.entrySet());
			System.out.println(lse.noiseWords);
		}else if(Integer.valueOf(input) == 5) {
			System.out.println("Please enter your two keywords: ");
			Scanner scan = new Scanner(System.in);
			String word1 = scan.nextLine();
			String word2 = scan.nextLine();
			System.out.println(lse.top5search(word1, word2));
			scan.close();
		}
		scan1.close();
	}
}